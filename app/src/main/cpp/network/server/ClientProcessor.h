//
// Created by dawnoftherent on 3/28/21.
//

#ifndef SIMPLEGROUPCOMMSERVER_CLIENTPROCESSOR_H
#define SIMPLEGROUPCOMMSERVER_CLIENTPROCESSOR_H

#include "../../util/Config.h"
#include "../../debug.h"
#include "ClientResponder.h"
#include "storage/ClientManager.h"
#include "storage/Buffer.h"

#include <netinet/in.h>

#define PROCESS_CLOSE_REQUEST 0

namespace com_curiousorigins_simplegroupcommserver {

    class ClientProcessor {

        friend class ClientResponder;

    private:
#ifdef MEM_TEST
        char MEM_TEST_BLOCK[MEM_TEST_SIZE];
#endif
        char *clientAddr;
        unsigned char state;
        Buffer buffer;
        int socketID;
        uint32_t id;
        ClientResponder responder;
        ClientManager * allClients;
        void setClientAddr(struct sockaddr * connectionInfo);
        ssize_t processLen();
        ssize_t processData();
        ssize_t processResponse();
        void terminate();
        void welcome();

    public:
        ClientProcessor(const Config * c, int socketID, struct sockaddr * connectionInfo, uint32_t id, ClientManager * allClients);
        ~ClientProcessor();
        ssize_t process();
        uint32_t key();
    };
}

#endif //SIMPLEGROUPCOMMSERVER_CLIENTPROCESSOR_H
