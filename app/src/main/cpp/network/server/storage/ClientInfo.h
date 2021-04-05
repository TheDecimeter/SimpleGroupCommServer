//
// Created by dawnoftherent on 3/30/21.
//

#ifndef SIMPLEGROUPCOMMSERVER_CLIENTINFO_H
#define SIMPLEGROUPCOMMSERVER_CLIENTINFO_H


#include "../../../debug.h"
#include <cstdint>
#include <atomic>

namespace com_curiousorigins_simplegroupcommserver {
    class ClientManager;

    class ClientInfo {
        friend class ClientManager;

    private:
#ifdef MEM_TEST
        char MEM_TEST_BLOCK[MEM_TEST_SIZE];
#endif
        uint32_t id;
        bool deleteSoon;
        std::atomic<bool> atomicRequested;
        std::atomic<int> atomicDibbs;


    public:
        int socketID;
        ClientInfo(uint32_t id, int socketID);
        ClientInfo(ClientInfo &other);
        ClientInfo();
        ~ClientInfo();
    };
}


#endif //SIMPLEGROUPCOMMSERVER_CLIENTINFO_H
