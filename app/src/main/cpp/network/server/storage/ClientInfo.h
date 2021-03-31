//
// Created by dawnoftherent on 3/30/21.
//

#ifndef SIMPLEGROUPCOMMSERVER_CLIENTINFO_H
#define SIMPLEGROUPCOMMSERVER_CLIENTINFO_H


#include <cstdint>
namespace com_curiousorigins_simplegroupcommserver {
    class ClientInfo {
    public:
        const uint32_t id;
        const int socketID;
        ClientInfo(uint32_t id, int socketID);
        ClientInfo();
        ~ClientInfo();
    };
}


#endif //SIMPLEGROUPCOMMSERVER_CLIENTINFO_H
