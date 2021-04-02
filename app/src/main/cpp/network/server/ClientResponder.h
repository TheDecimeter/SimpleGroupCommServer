//
// Created by dawnoftherent on 3/29/21.
//

#ifndef SIMPLEGROUPCOMMSERVER_CLIENTRESPONDER_H
#define SIMPLEGROUPCOMMSERVER_CLIENTRESPONDER_H


#include "../../util/Runnable.h"
#include "../../debug.h"
#include "storage/ClientManager.h"

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
        ClientInfo infoSpot;
        char reactionType = NO_REACT;
        void process(const char reactionType, char * data, const unsigned char len);
        void relay(unsigned char * data, const unsigned char len);
        void send(const void * data, const unsigned char len);

    public:
        ClientResponder(ClientManager * allClients);
        void process(char * data, unsigned char len);
        bool process();
    };

}


#endif //SIMPLEGROUPCOMMSERVER_CLIENTRESPONDER_H
