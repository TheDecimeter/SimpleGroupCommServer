//
// Created by dawnoftherent on 3/28/21.
//

#ifndef SIMPLEGROUPCOMMSERVER_CLIENTPROCESSOR_H
#define SIMPLEGROUPCOMMSERVER_CLIENTPROCESSOR_H

#include "../../util/Config.h"
#include "ClientResponder.h"

#include <netinet/in.h>

#define PROCESS_CLOSE_REQUEST 0

namespace com_curiousorigins_simplegroupcommserver {
    class ServerTCP;
    class ClientHandler;
    class ClientResponder;

    class ClientProcessor {

        friend class ClientResponder;

    private:
        char * buf, * readerPos, *clientAddr;
        //buflen - length of buffer (>=bytesToRead and dataLen),
        // bytesToRead - bytes left in stream (<= dataLen),
        // dataLen, total length of data
        unsigned char bufLen, dataLen, bytesToRead, state;
        int socketID, id;
        ServerTCP * server;
        ClientHandler * handler;
        ClientResponder responder;
        void setClientAddr(struct sockaddr * connectionInfo);
        ssize_t processLen();
        ssize_t processData();
        ssize_t processResponse();
        void terminate();

    public:
        ClientProcessor(const Config * c, int socketID, struct sockaddr * connectionInfo, ServerTCP * server, ClientHandler * handler, int id);
        ~ClientProcessor();
        ssize_t process();
        int key();
    };
}

#endif //SIMPLEGROUPCOMMSERVER_CLIENTPROCESSOR_H
