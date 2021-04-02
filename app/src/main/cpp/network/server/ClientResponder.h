//
// Created by dawnoftherent on 3/29/21.
//

#ifndef SIMPLEGROUPCOMMSERVER_CLIENTRESPONDER_H
#define SIMPLEGROUPCOMMSERVER_CLIENTRESPONDER_H


#include "../../util/Runnable.h"
#include "storage/ClientManager.h"

#define NO_REACT 0
#define RELAY_REACT 'R'

namespace com_curiousorigins_simplegroupcommserver {

    class ClientResponder {
    private:
        Runnable * worker = NULL;
        ClientManager * allClients;
        char reactionType = NO_REACT;
        void process(const char reactionType, const char * data, const unsigned char len);
        void relay(const char * data, const unsigned char len);

    public:
        ClientResponder(ClientManager * allClients);
        void process(const char * data, unsigned char len);
        bool process();
    };

}


#endif //SIMPLEGROUPCOMMSERVER_CLIENTRESPONDER_H
