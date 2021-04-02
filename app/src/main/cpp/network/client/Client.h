//
// Created by dawnoftherent on 3/19/21.
//

#ifndef SIMPLEGROUPCOMMSERVER_CLIENT_H
#define SIMPLEGROUPCOMMSERVER_CLIENT_H

#include <unistd.h>
#include "../../util/Config.h"
#include "../../debug.h"

namespace com_curiousorigins_simplegroupcommserver {
    class Client {
    private:
#ifdef MEM_TEST
        char MEM_TEST_BLOCK[MEM_TEST_SIZE];
#endif
        const Config * config;
        int socketID=0, clientID;
        bool connected=false;

    public:
        Client(Config * c);
        void send(const char* data, size_t dataLen);
        void sendChunk(const char* data, size_t dataLen);
        void receive(char* data);
        bool start();
        void stop();
    };
}


#endif //SIMPLEGROUPCOMMSERVER_CLIENT_H
