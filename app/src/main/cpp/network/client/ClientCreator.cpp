//
// Created by dawnoftherent on 3/20/21.
//

#include "ClientCreator.h"
#include "../../debug.h"
#include "Client.h"
#include "../../util/StringTools.h"
#include "../../ScreenConsole.h"

#include<iterator>
#include <pthread.h>

#define TAG "ClientCreatorCpp"
#define ST_UNSET 0


namespace com_curiousorigins_simplegroupcommserver {

    ClientCreator::ClientCreator(Config * c):
    config(c){
        for(int i=0; i<3; ++i)
            clients.push_back(new Client(c));
    }

    ClientCreator::~ClientCreator() {
        std::vector<Client*>::iterator i;
        for(i=clients.begin(); i!=clients.end(); i++) {
            (*i)->stop();
            delete *i;
            (*i) = nullptr;
        }
        clients.clear();
    }

    /**
     * non blocking
     * creates a thread which connects all clients, then runs communication tests on each one
     *
     */
    void ClientCreator::start() {
        PDBG(TAG,"starting client creator")
        pthread_create(&creatorThread, NULL, &creationWorkWrapper, this);
    }

    /**
     * non blocking
     * send data from one client to another, operates in one thread, sends data, then waits to
     * receive data.
     * @param from - the client sending data
     * @param to - the client receiving data
     * @param data - the data sent (this is deleted when the test completes)
     * @param status - a reference to an int which reflects the current status of the message
     */
    pthread_t ClientCreator::speak(Client *from, Client *to, const char *data, int& status) {
        SpeakData * d = new SpeakData(from,to,data,status);

        pthread_t speakThread;
        pthread_create(&speakThread, NULL, &speak_work, d);
        return speakThread;
    }


    void ClientCreator::creationWork() {
        int MAX = config->bufferSize;
        //create connections
        std::vector<Client*>::iterator i;
        for(i=clients.begin(); i!=clients.end(); i++)
            (*i)->start();


        //TODO send data tests

        std::vector<pthread_t> onGoingWork;

        clients[2]->stop();

        clients[0]->sendChunk("\x06",1);
        clients[0]->sendChunk("1ts",3);

        clients[1]->send("2tst1\0",6);

        usleep(500000);
        clients[0]->sendChunk("t1\0",3);

        clients[1]->send("2tst2\0",6);

        clients[0]->send("1tst2\0",6);
        clients[0]->send("1tst3\0",6);

        clients[1]->stop();

        //char* data = new char[MAX];
        //StringTools::fill("R0\x1F""1\x1E""test from 0 to 1\x1D", NULL, MAX);
        int status = ST_UNSET;
        onGoingWork.push_back(speak(clients[0],clients[1],"R0\x1F""1\x1E""test from 0 to 1\x1D",status));





        std::vector<pthread_t>::iterator ti;
        for(ti=onGoingWork.begin(); ti!=onGoingWork.end(); ti++)
            pthread_join(*ti, NULL);

        PDBG(TAG,"finished tests");
        ScreenConsole::print("finished server tests\n");
    }

    void *ClientCreator::speak_work(void *speakData) {
        PDBG(TAG,"in speak thread");
        SpeakData * transmission = (SpeakData *) speakData;
        transmission->status=ST_UNSET;
        size_t dataLen=StringTools::len(transmission->data);

        transmission->from->send(transmission->data, dataLen);

        delete transmission;

        PDBG(TAG,"finished speak test");
        return NULL;
    }

    void *ClientCreator::creationWorkWrapper(void *clientCreator) {
        ((ClientCreator*)clientCreator)->creationWork();
        return nullptr;
    }

    ClientCreator::SpeakData::SpeakData(Client *from, Client *to, const char *data, int & status) :
    status(status), from(from), to(to), data(data){    }

    ClientCreator::SpeakData::~SpeakData() {
        //delete data;
        data= nullptr;
        from=nullptr;
        to=nullptr;
    }
}