//
// Created by dawnoftherent on 3/17/21.
//

#ifndef SIMPLEGROUPCOMMSERVER_SERVERTCP_H
#define SIMPLEGROUPCOMMSERVER_SERVERTCP_H

#include<pthread.h>
#include "ClientHandler.h"
#include "storage/ClientManager.h"
#include "../client/ClientCreator.h"
#include <unordered_map>
#include "../../util/Config.h"

namespace com_curiousorigins_simplegroupcommserver {

    class ServerTCP {
    private:
        const Config * config;
        ClientCreator * afterCreated;
        unsigned int clientCount=0;
        ClientManager allClients;
        bool stopFlag = false;
        int serverSocketID;
        pthread_t serverThread;
        ClientHandler * current;
        std::unordered_map<int, ClientHandler*> clientHandlers;

        static void* serverWorkWrapper(void* thiz);
        void serverWork();


    public:
        ServerTCP(const Config * c, ClientCreator * afterCreated);
        ~ServerTCP();

        void start();
        void stop();
    };
}


#endif //SIMPLEGROUPCOMMSERVER_SERVERTCP_H
