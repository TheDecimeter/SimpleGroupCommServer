//
// Created by dawnoftherent on 3/18/21.
//


#include "ClientHandler.h"
#include "../../debug.h"
#include "../../ScreenConsole.h"


#include <unistd.h>

#include <chrono>
#include <thread>
#include <vector>
#include <time.h>

#define TAG "      clientHandlerSERVcpp"

namespace com_curiousorigins_simplegroupcommserver {
    std::atomic<int> ClientHandler::entityCount;
    /**
     * Constructor for creating active client handler
     *  (the client handler created by the server from which all other client handlers
     *   split)
     * @param c - config file settings
     */
    ClientHandler::ClientHandler(const Config * c,ClientManager * allClients):
            config(c), active(this), allClients(allClients), name("unsetA"){
        entityCount++;
        //PDBG(TAG,"default constructor %p",this);
    }

    /**
     * Constructor for creating a new client handler which is not actively taking new connections
     * from the server. This is used as a fork from the active client handler
     * @param c - config file settings
     * @param active - the active client handler
     */
    ClientHandler::ClientHandler(const Config * c, ClientHandler *const active, std::vector<ClientProcessor*> * clients, ClientManager * allClients):
            config(c), active(active), allClients(allClients), name("unsetB"){
        entityCount++;
        std::vector<ClientProcessor*>::iterator p;
        for(p=clients->begin(); p != clients->end(); ++p){
            processors.insert(std::pair<int,ClientProcessor*>((*p)->key(), *p));
        }
        //PDBG(TAG,"split constructor %p",this);
    }


    ClientHandler::~ClientHandler(){
        //PDBG(TAG, "start delete %s, %d entities left", name.c_str(), --entityCount)

        ClientHandler * c = child;
        child=NULL; // don't try deleting the child from the run thread, deleting it below
        stop=true;
        if(c!=NULL)
            delete c;

//        listen=false;

        pthread_join(handlerThread, NULL);

        processorListLock.lock();//theoretically, maybe someone could try adding a processor as I am deleting it?
        std::unordered_map<int,ClientProcessor*>::iterator processor;
        for(processor=processors.begin(); processor!=processors.end(); processor++) {
            delete processor->second;
        }
        processorListLock.unlock();

        //PDBG(TAG, "end delete %s", name.c_str())
    }

    void *ClientHandler::clientWorkWrapper(void *thiz) {
        ClientHandler* client = (ClientHandler*) thiz;
        client->clientWork();
        return NULL;
    }

    void ClientHandler::clientWork() {
        std::unordered_map<int,ClientProcessor*>::iterator processor;
        struct timespec start, end;
        const int longestLoad=config->longestLoad, shortestLoad = config->shortestLoad;


        //usleep(5000000);
        //PDBG(TAG,"starting client handler, %s", name.c_str())


        while (listen) {

            //get any incoming messages
            processorListLock.lock();

            if(stop) {
                clean();
                return;
            }

            clock_gettime(CLOCK_MONOTONIC, &start);
            for(processor=processors.begin(); processor!=processors.end(); ) {
                if(processor->second->process() == PROCESS_CLOSE_REQUEST){
                    allClients->remove(processor->second->key());
                    delete processor->second;
                    processors.erase(processor++);
                }
                else
                    ++processor;
            }
            clock_gettime(CLOCK_MONOTONIC, &end);
            processorListLock.unlock();


            //see if this thread is over or under worked
            // and sleep it for a period to keep processor from being needlessly overworked
            long dur = end.tv_nsec-start.tv_nsec;

            pruneDeadChild();

            if(dur > 0){//TODO change this back to using timing to see whether too much work on the thread

                if(processors.size() > 4){//dur > longestLoad && processors.size() > 1){
                    PDBG(TAG, "long process time, need to split %s", name.c_str());
                    split();
                }
                else {//TODO change this back to using timing to see whether too little work on the thread
                    if (processors.size() <2 && this != active) {// dur < shortestLoad && this != active) {
                        PDBG(TAG, "workload too light, need to merge %s, this: %p  active: %p", name.c_str(), this, active);
                        mergeWithActive();
                        return;
                    }

                    std::this_thread::sleep_for(std::chrono::nanoseconds(longestLoad - dur));
                }



//                if(/*processors.size() > 4){*/dur > longestLoad && processors.size() > 1){
//                    PDBG(TAG, "long process time, need to split %s", name.c_str());
//                    split();
//                }
//                else {//TODO change this back to using timing to see whether too little work on the thread
//                    if (/*processors.size() <2 && this != active) {*/ dur < shortestLoad && this != active) {
//                        PDBG(TAG, "workload too light, need to merge %s, this: %p  active: %p", name.c_str(), this, active);
//                        mergeWithActive();
//                        return;
//                    }
//
//                    std::this_thread::sleep_for(std::chrono::nanoseconds(longestLoad - dur));
//                }


            }
            else{
                std::this_thread::sleep_for(std::chrono::nanoseconds(longestLoad));
            }


        }

        PDBG(TAG,"Shouldn't have gotten here %s, %p", name.c_str(), this)
        return;
    }






    void ClientHandler::addProcessor(struct sockaddr *connectionInfo, int connfd) {
        if(stop)
            return;
        uint32_t outClientID;

//        PDBG(TAG, "trying to add client %d", connfd)
        if(allClients->add(connfd, outClientID)) {

            //PDBG(TAG, "adding client %d to %s", outClientID, name.c_str())
            ClientProcessor *p = new ClientProcessor(config, connfd, connectionInfo, outClientID,
                                                     allClients);


            processorListLock.lock();
            if (processors.find(p->key()) == processors.end())
                processors.insert(std::pair<int, ClientProcessor *>(p->key(), p));
            else {
                PDBG(TAG, "\n\nERROR: already inserted item at %s\n\n", name.c_str());
                processorListLock.unlock();
                return;
            }
            processorListLock.unlock();

            if (!listen) {
                //PDBG(TAG, "Starting listen thread %s", name.c_str());
                listen = true;
                pthread_create(&handlerThread, NULL, &clientWorkWrapper, this);
            } else {
                //PDBG(TAG, "listening thread already started %s", name.c_str());
            }
        }
    }

    /**
     * Merge the client processors in this handler with the processors in the active handler.
     * This should be used to free up a handler's thread if it doesn't seem to have much work
     * on it.
     */
    void ClientHandler::mergeWithActive() {
        if(stop)
            return;
        processorListLock.lock();
        active->processorListLock.lock();

        std::unordered_map<int,ClientProcessor*>::iterator processor;
        for(processor=processors.begin(); processor!=processors.end(); processor++) {
            active->processors.insert(std::pair<int,ClientProcessor*>(processor->second->key(), processor->second));
        }

        listen=false;
        processors.clear();
        processorListLock.unlock();

        active->start();
        active->processorListLock.unlock();

        stop=true;
    }

    /**
     * Put half of this handler's jobs in another handler on another thread.
     * NOTE: if this is the active handler, all jobs will be moved to the new
     * handler (it is assumed that the act of adding clients to a handler will slow it down.
     * Once a thread stops taking active connections it will probably have more time to devote to
     * handling clients. If it doesn't, then it will split in half after that.
     */
    void ClientHandler::split() {
        processorListLock.lock(); //this shouldn't technically need to lock the whole thing, it just keeps new clients from being added while splitting

        if(stop) {
            processorListLock.unlock();
            return;
        }

        int count = 0, stopAt=processors.size();
        if(this != active)
            stopAt/=2;
        if(stopAt > 0) {

            //move half of the processors into a new vector
            std::vector<ClientProcessor*> kids;
            std::unordered_map<int, ClientProcessor *>::iterator processor;
            for (processor = processors.begin(); processor != processors.end(); processor++) {
                if(count++ == stopAt)
                    break;
                kids.push_back(processor->second);
            }

            std::vector<ClientProcessor*>::iterator kid;
            for(kid = kids.begin(); kid !=kids.end(); ++kid){
                processor= processors.find((*kid)->key());
                if(processor != processors.end())
                    processors.erase(processor);
            }

            ClientHandler * newChild = new ClientHandler(config, active, &kids, allClients);

            if(newChild->processors.size() > 0) {
                newChild->name = name + std::to_string(++childCount);
                PDBG(TAG, "%s created new child: %s", name.c_str(), newChild->name.c_str());

                newChild->child = child;
                child = newChild;
                newChild->start();
            } else {
                PDBG(TAG, "ERROR, failed to add any processors to the new client handler when splitting, should have added %d", stopAt)
                delete newChild;
            }
        }


        processorListLock.unlock();
    }

    /**
     * clean up the handler
     */
    void ClientHandler::clean() {
        std::unordered_map<int,ClientProcessor*>::iterator processor;
        for(processor=processors.begin(); processor!=processors.end(); processor++) {
            delete processor->second;
        }
        processors.clear();
        listen=false;
        stop=true;
    }

    /**
     * start the handler thread if there are client processors present, and
     * the thread hasn't started yet.
     * Note, calling addProcessor should also start the handler thread.
     * This method allows you to add as many processors as you want, and then
     * start it when you are ready.
     */
    void ClientHandler::start() {
        if(!listen && processors.size() > 0){
            listen = true;
            pthread_create(&handlerThread, NULL, &clientWorkWrapper, this);
        }
    }

    /**
     * See if the child merged with the active handler.
     * If it has, delete it and remove it from the child linked list.
     */
    void ClientHandler::pruneDeadChild() {
        if(child!=NULL && child->stop){
            //PDBG(TAG, "%p is pruning %p", this,child);
            //PDBG(TAG, "%s at %p on %d pruning %s at %p", name.c_str(), this, handlerThread, child->name.c_str(), child)
            ClientHandler *newChild=child->child;
            child->child=NULL;
            delete child;
            child = newChild;
        }
    }


}
