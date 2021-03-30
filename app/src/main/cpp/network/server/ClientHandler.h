//
// Created by dawnoftherent on 3/18/21.
//

#ifndef SIMPLEGROUPCOMMSERVER_CLIENTHANDLER_H
#define SIMPLEGROUPCOMMSERVER_CLIENTHANDLER_H


#include <unordered_map>
#include <netinet/in.h>
#include <vector>
#include<mutex>

#include "ServerTCP.h"
#include "../../util/Config.h"
#include "ClientProcessor.h"

namespace com_curiousorigins_simplegroupcommserver {
    class ServerTCP;
    class ClientProcessor;

    class ClientHandler {
    private:
        const Config* config;
        pthread_t handlerThread;
        bool listen=false;
        int id;
        std::unordered_map<int, ClientProcessor*> processors;
        std::mutex processorListLock;

        static void *clientWorkWrapper(void *thiz);
        void clientWork();

        ServerTCP * server;
    public:
        ClientHandler(const Config * c, ServerTCP * server, int id);
        ~ClientHandler();
        void addProcessor(struct sockaddr * connectionInfo, int connfd, int clientID);
        void merge(ClientHandler * other);
        int key();
    };
}


#endif //SIMPLEGROUPCOMMSERVER_CLIENTHANDLER_H
