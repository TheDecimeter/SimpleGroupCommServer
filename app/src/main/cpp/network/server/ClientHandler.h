//
// Created by dawnoftherent on 3/18/21.
//

#ifndef SIMPLEGROUPCOMMSERVER_CLIENTHANDLER_H
#define SIMPLEGROUPCOMMSERVER_CLIENTHANDLER_H


#include <unordered_map>
#include <mutex>

#include "../../util/Config.h"
#include "../../debug.h"
#include "ClientProcessor.h"

namespace com_curiousorigins_simplegroupcommserver {

    class ClientHandler {
    private:
#ifdef MEM_TEST
        char MEM_TEST_BLOCK[MEM_TEST_SIZE];
#endif

        const Config* config;
        pthread_t handlerThread;
        bool listen=false, stop=false;
        ClientHandler *const active, *child=NULL;
        std::unordered_map<int, ClientProcessor*> processors;
        std::mutex processorListLock;
        ClientManager * allClients;

        ClientHandler(const Config * c, ClientHandler *const active, std::vector<ClientProcessor*> * clients, ClientManager * allClients);
        static void *clientWorkWrapper(void *thiz);
        void clientWork();
        void clean();
        void start();
        void pruneDeadChild();

    public:
        std::string name;
        int childCount=0;
        static std::atomic<int> entityCount;

        ClientHandler(const Config * c, ClientManager * allClients);
        ~ClientHandler();
        void addProcessor(struct sockaddr * connectionInfo, int connfd);
        void mergeWithActive();
        void split();
    };
}


#endif //SIMPLEGROUPCOMMSERVER_CLIENTHANDLER_H
