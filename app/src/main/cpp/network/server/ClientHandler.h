//
// Created by dawnoftherent on 3/18/21.
//

#ifndef SIMPLEGROUPCOMMSERVER_CLIENTHANDLER_H
#define SIMPLEGROUPCOMMSERVER_CLIENTHANDLER_H


#include <unordered_map>
#include <vector>
#include <mutex>

#include "../../util/Config.h"
#include "ClientProcessor.h"

namespace com_curiousorigins_simplegroupcommserver {

    class ClientHandler {
    private:
        const Config* config;
        pthread_t handlerThread;
        bool listen=false;
        int id;
        ClientHandler *const active;
        std::unordered_map<int, ClientProcessor*> processors;
        std::mutex processorListLock;

        ClientHandler(const Config * c, int id, ClientHandler *const active);
        static void *clientWorkWrapper(void *thiz);
        void clientWork();

    public:
        ClientHandler(const Config * c, int id);
        ~ClientHandler();
        void addProcessor(struct sockaddr * connectionInfo, int connfd, int clientID);
        void merge(ClientHandler * other);
        int key();
    };
}


#endif //SIMPLEGROUPCOMMSERVER_CLIENTHANDLER_H
