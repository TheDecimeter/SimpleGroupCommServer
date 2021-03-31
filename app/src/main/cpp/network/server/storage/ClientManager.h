//
// Created by dawnoftherent on 3/30/21.
//

#ifndef SIMPLEGROUPCOMMSERVER_CLIENTMANAGER_H
#define SIMPLEGROUPCOMMSERVER_CLIENTMANAGER_H

#include "ClientInfo.h"

#include <shared_mutex>
#include <unordered_map>
namespace com_curiousorigins_simplegroupcommserver {
    class ClientManager {
    private:
        uint32_t clientCounter=0;
        std::unordered_map<uint32_t, ClientInfo*> clients;
        std::shared_timed_mutex lock;
        bool next();

    public:
        ClientManager();
        ~ClientManager();
        bool add(int socketID, uint32_t &outID);
        void remove(uint32_t id);
        bool contains(uint32_t id);
        bool tryGet(uint32_t id, ClientInfo * out);
    };
}


#endif //SIMPLEGROUPCOMMSERVER_CLIENTMANAGER_H
