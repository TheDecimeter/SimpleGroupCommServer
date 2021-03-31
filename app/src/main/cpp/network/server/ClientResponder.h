//
// Created by dawnoftherent on 3/29/21.
//

#ifndef SIMPLEGROUPCOMMSERVER_CLIENTRESPONDER_H
#define SIMPLEGROUPCOMMSERVER_CLIENTRESPONDER_H


#include "../../util/Runnable.h"
#include "storage/ClientManager.h"

namespace com_curiousorigins_simplegroupcommserver {

    class ClientResponder {
    private:
        Runnable * worker = NULL;
        ClientManager * allClients;
        void process(const char dataType, const char * data, const unsigned char len) const;

    public:
        ClientResponder(ClientManager * allClients);
        void process(const char * data, unsigned char len) const;
        bool process();
    };

}


#endif //SIMPLEGROUPCOMMSERVER_CLIENTRESPONDER_H
