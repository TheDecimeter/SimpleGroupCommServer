//
// Created by dawnoftherent on 3/18/21.
//


#include "ClientHandler.h"
#include "ServerTCP.h"
#include "../../debug.h"
#include "../../ScreenConsole.h"


#include <arpa/inet.h>
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <linux/in6.h>
#include <stdlib.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <chrono>
#include <thread>

#include <time.h>

#define TAG "      clientHandlerSERVcpp"

namespace com_curiousorigins_simplegroupcommserver {

    void *ClientHandler::clientWorkWrapper(void *thiz) {
        ClientHandler* client = (ClientHandler*) thiz;
        client->clientWork();
        return NULL;
    }

    void ClientHandler::clientWork() {
        ssize_t read;
        std::unordered_map<int,ClientProcessor*>::iterator processor;
        struct timespec start, end;
        const int longestLoad=config->longestLoad, shortestLoad = config->shortestLoad;


        //usleep(5000000);
        PDBG(TAG,"starting client handler")

        // infinite loop for chat
        while (listen) {

            //get any incoming messages
            processorListLock.lock();
            clock_gettime(CLOCK_MONOTONIC, &start);
            for(processor=processors.begin(); processor!=processors.end(); ) {
                if(processor->second->process() == PROCESS_CLOSE_REQUEST){
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
//            PDBGF(TAG,"gap time %d", dur);
            if(dur > 0){
                if(dur > longestLoad && processors.size() > 1){
                    PDBG(TAG, "long process time, need to split");
                }
                else {
                    if (dur < shortestLoad && !server->isCurrent(this)) {
                        PDBG(TAG, "workload too light, need to merge");
                    }

                    std::this_thread::sleep_for(std::chrono::nanoseconds(longestLoad - dur));
                }
            }
            else{
                std::this_thread::sleep_for(std::chrono::nanoseconds(longestLoad));
            }


        }

        PDBG(TAG,"Shouldn't have gotten here")
        return;
    }

    ClientHandler::ClientHandler(const Config * c, ServerTCP * server, int id):
    config(c), id(id), server(server){

    }

    ClientHandler::~ClientHandler(){
        listen=false;
        std::unordered_map<int,ClientProcessor*>::iterator processor;
        for(processor=processors.begin(); processor!=processors.end(); processor++) {
            delete processor->second;
        }
//        close(socketID);

//        delete clientAddr;
    }




    void ClientHandler::addProcessor(struct sockaddr *connectionInfo, int connfd, int clientID) {

        ClientProcessor * p = new ClientProcessor(config, connfd, connectionInfo, server, this, clientID);

        processorListLock.lock();
        if(processors.find(p->key()) == processors.end())
            processors.insert(std::pair<int,ClientProcessor*>(p->key(), p));
        else{
            PDBG(TAG,"\n\nERROR: already inserted item\n\n");
            processorListLock.unlock();
            return;
        }
        processorListLock.unlock();

        if(!listen){
            PDBG(TAG,"Starting listen thread");
            listen=true;
            pthread_create(&handlerThread, NULL, &clientWorkWrapper, this);
        }
        else{
            PDBG(TAG,"listening thread already started");
        }
    }

    int ClientHandler::key() {
        return id;
    }

    void ClientHandler::merge(ClientHandler * other) {
        if(other == this)
            return;
        if(server->isCurrent(other)){
            PDBG(TAG, "\n\nERROR, can't merge currently active client handler into another handler. That would remove the currently active handler (you can merge other handlers into the currently active one though)\n\n")
            return;
        }
        other->processorListLock.lock();
        processorListLock.lock();

        std::unordered_map<int,ClientProcessor*>::iterator processor;
        for(processor=other->processors.begin(); processor!=other->processors.end(); processor++) {
            processors.insert(std::pair<int,ClientProcessor*>(processor->second->key(), processor->second));
        }
        processorListLock.unlock();

        other->listen=false;
        other->processors.clear();

        other->processorListLock.unlock();

        delete other;

        if(!listen){
            PDBG(TAG,"Starting listen thread");
            listen=true;
            pthread_create(&handlerThread, NULL, &clientWorkWrapper, this);
        }
        else{
            PDBG(TAG,"listening thread already started");
        }
    }


}
