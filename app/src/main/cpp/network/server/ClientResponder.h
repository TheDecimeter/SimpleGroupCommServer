//
// Created by dawnoftherent on 3/29/21.
//

#ifndef SIMPLEGROUPCOMMSERVER_CLIENTRESPONDER_H
#define SIMPLEGROUPCOMMSERVER_CLIENTRESPONDER_H


#include "../../util/Runnable.h"

namespace com_curiousorigins_simplegroupcommserver {
    class ClientProcessor;
    class ServerTCP;

    class ClientResponder {
    private:
        ServerTCP * server;
        ClientProcessor * owner;
        Runnable * worker = NULL;
        void process(const char dataType, const char * data, const unsigned char len) const;

    public:
        ClientResponder(ServerTCP * server, ClientProcessor * owner);
        void process(const char * data, unsigned char len) const;
        bool process();
    };

}


#endif //SIMPLEGROUPCOMMSERVER_CLIENTRESPONDER_H
