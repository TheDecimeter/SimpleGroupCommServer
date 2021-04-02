//
// Created by dawnoftherent on 3/20/21.
//

#include "../server/storage/ClientInfo.h"

#include "ClientCreator.h"
#include "../../debug.h"
#include "Client.h"
#include "../../util/StringTools.h"
#include "../../ScreenConsole.h"

#include<iterator>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <endian.h>


#define TAG "ClientCreatorCpp"
#define ST_UNSET 0


namespace com_curiousorigins_simplegroupcommserver {

    ClientCreator::ClientCreator(Config * c):
    config(c){
        for(int i=0; i<20; ++i)
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
        std::vector<Client*>::iterator i;
        for(i=clients.begin(); i!=clients.end(); i++)
            (*i)->start();

        relayTest(100);



        for(i=clients.begin(); i!=clients.end(); i++)
            (*i)->stop();
//int x=4;
//PDBG(TAG, "x init: %d", x);
//int &y = x;
//int z=6;
//y=z;
//        PDBG(TAG, "x after y=z=6: %d", x);
//        z++;
//        PDBG(TAG, "x after z++: %d", x);

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

    void ClientCreator::disconnectStress(int times) {
        std::vector<pthread_t> onGoingWork;
//
        for(int i=0; i<times; ++i) {
            int r = rand() % clients.size();
            clients[r]->stop();
            //usleep(10000);
            clients[r]->start();
        }
        clients[2]->send("3tst1\0",6);
        clients[2]->send("3tst2\0",6);
        clients[2]->send("3tst3\0",6);
        clients[2]->send("3tst4\0",6);
        clients[2]->send("3tst5\0",6);
        clients[2]->send("3tst6\0",6);
        clients[2]->send("3tst7\0",6);
        clients[2]->send("3tst8\0",6);
        clients[2]->send("3tst9\0",6);
    }

    /**
     * test relaying messages from one client to another
     */
    bool ClientCreator::relay(const int count, const uint32_t from, const uint32_t to) {
        const int ex=2, l=ex+80;
        char data[l], rcv[l], msg[l];

        bzero(data,l);
        data[0]='R';
        //*(reinterpret_cast<uint32_t*>(data+1))=htonl(to+1);
        data[1]= static_cast<char>(to + 1);

        bzero(msg,l);
        int msgLen = sprintf(msg, "msg: %d %d", rand(), rand());
        if(msgLen > 0 && msgLen <=l) {

            PDBG(TAG, "from: %d, to %d: %s", from+1, data[1], msg);
//            ScreenConsole::print({"sending msg: ", std::to_string(count)});
            int dataLen = sprintf(data + ex, "%s", msg) + ex;
            if (dataLen > 0 && dataLen <= l) {
                clients[from]->send(data, static_cast<size_t>(dataLen));

//                return true;

                bzero(rcv,l);
                clients[to]->receive(rcv);
                std::string prt = ScreenConsole::s(rcv, dataLen - ex);

                ScreenConsole::print({prt, " i:",std::to_string(count)});
                PDBG(TAG, "received %s", prt.c_str());

                if(strncmp(msg, rcv, static_cast<size_t>(msgLen)) == 0)
                    return true;

                ScreenConsole::print({"no match{\ns: ", msg, "\nr: ", rcv, "  }"});
                PDBG(TAG, "no match \ns:%s\nr:%s", msg, prt.c_str());
                return false;

            } else{
                PDBG(TAG, "unable to package msg: %s", msg)
                ScreenConsole::print({"unable to package msg", msg});
            }
        }else{
            PDBG(TAG, "unable to package create message")
            ScreenConsole::print("unable to package create message");
        }
        return true;
    }

    void ClientCreator::relayTest(int times) {
//        usleep(500000);
        uint32_t size=clients.size();

        for(int i=0; i<times; ++i)
            if(!relay(i, rand() % size, rand() % size)) {
                PDBG(TAG, "failed test")
                ScreenConsole::print("failed test");
                return;
            }

        PDBG(TAG, "passed all tests")
        ScreenConsole::print("passed all tests");
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