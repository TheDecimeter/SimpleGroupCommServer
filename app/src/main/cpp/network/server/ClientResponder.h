//
// Created by dawnoftherent on 3/29/21.
//

#ifndef SIMPLEGROUPCOMMSERVER_CLIENTRESPONDER_H
#define SIMPLEGROUPCOMMSERVER_CLIENTRESPONDER_H


#include "../../util/Runnable.h"
#include "../../debug.h"
#include "storage/ClientManager.h"
#include "storage/Buffer.h"


#define NO_REACT 0
#define RELAY_REACT 'R'

#define NO_WORK NO_REACT
#define SEND_WORK 'S'


namespace com_curiousorigins_simplegroupcommserver {

    class ClientResponder {
    private:
#ifdef MEM_TEST
        char MEM_TEST_BLOCK[MEM_TEST_SIZE];
#endif
        Runnable * worker = NULL;
        ClientManager * allClients;
        ClientInfo * infoSpot;
        Buffer * buffer;
        char reactionType = NO_REACT;
        void process(const char reactionType, Buffer * buffer);
        void relay(Buffer * buffer);
        void send();

    public:
        ClientResponder(ClientManager * allClients);
        void process(Buffer * buffer);
        bool continueProcessing();
    };


}


#endif //SIMPLEGROUPCOMMSERVER_CLIENTRESPONDER_H
