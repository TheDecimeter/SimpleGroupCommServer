//
// Created by dawnoftherent on 3/30/21.
//

#ifndef SIMPLEGROUPCOMMSERVER_CLIENTINFO_H
#define SIMPLEGROUPCOMMSERVER_CLIENTINFO_H


#include "../../../debug.h"
#include <cstdint>
namespace com_curiousorigins_simplegroupcommserver {
    class ClientInfo {
    public:
#ifdef MEM_TEST
    char MEM_TEST_BLOCK[MEM_TEST_SIZE];
#endif
        const uint32_t id;
        const int socketID;
        ClientInfo(uint32_t id, int socketID);
        ClientInfo(ClientInfo &other);
        ClientInfo();
        ~ClientInfo();
    };
}


#endif //SIMPLEGROUPCOMMSERVER_CLIENTINFO_H
