//
// Created by dawnoftherent on 3/30/21.
//

#ifndef SIMPLEGROUPCOMMSERVER_CLIENTINFO_H
#define SIMPLEGROUPCOMMSERVER_CLIENTINFO_H


#include "../../../debug.h"
#include "Buffer.h"
#include <cstdint>
#include <mutex>


#define DONE_SENDING 65535
#define FORGET_ME -2
#define IN_USE -3

#define BYTES_TO_WRITE 512

namespace com_curiousorigins_simplegroupcommserver {
    class ClientManager;

    class ClientInfo {
        friend class ClientManager;

    private:
#ifdef MEM_TEST
        char MEM_TEST_BLOCK[MEM_TEST_SIZE];
#endif
        uint32_t id, dibs;
        bool deleteSoon;
        std::mutex lock;
        Buffer * currentBuffer, *lastBuffer;
        bool addDibb(Buffer * bufferToSend);


    public:
        int socketID;
        ClientInfo(uint32_t id, int socketID);
        ClientInfo();
        ~ClientInfo();

        void removeDibb();
        ssize_t send();
    };
}


#endif //SIMPLEGROUPCOMMSERVER_CLIENTINFO_H
