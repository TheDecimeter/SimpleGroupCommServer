//
// Created by dawnoftherent on 3/17/21.
//

#include "ServerTCP.h"
#include "../client/ClientCreator.h"
#include "../../debug.h"
#include "../../ScreenConsole.h"


#include <pthread.h>
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unordered_map>
#include <fstream>
#include <unistd.h>

#define TAG "ServerCppTCP"

namespace com_curiousorigins_simplegroupcommserver{

    ServerTCP::ServerTCP(const Config * c, ClientCreator * afterCreated):
    config(c){
//        PDBG(TAG,"created server")
        stopFlag = true;
        current=new ClientHandler(c,this, handlerCount++);
        this->afterCreated=afterCreated;
//        clients = new std::unordered_map<int,ClientHandler*>;
    }

    void ServerTCP::start(){
        PDBG(TAG,"starting server")
        ScreenConsole::print("Starting server\n");
        if(!stopFlag)
            return;
        stopFlag=false;
        pthread_create(&serverThread, NULL, &serverWorkWrapper, this);
    }

    void ServerTCP::stop(){
        if(stopFlag)
            return;
        PDBG(TAG,"stopping server");
        stopFlag=true;

//        if(close(serverSocketID) == 0){
//            PDBG(TAG,"closed correctly")
//        }
//        else{
//            PDBGF(TAG,"failed to close server %d", errno);
//        }
    }

    void* ServerTCP::serverWorkWrapper(void *thiz) {
        ServerTCP * server = (ServerTCP*)thiz;
        server->serverWork();
        return NULL;
    }

    void ServerTCP::serverWork() {



        int connfd;
        socklen_t len;
        struct sockaddr_in servaddr;
        struct sockaddr clientAddr;

        // socket create and verification
        serverSocketID = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocketID == -1) {
            PDBG(TAG,"socket creation failed...\n")
            ScreenConsole::print("Server Socket Creation failed\n");
            return;
        }
        else {
            PDBG(TAG, "Socket successfully created..\n")
            ScreenConsole::print("Server socket created successfully\n");
        }


        bzero(&servaddr, sizeof(servaddr));

        // assign IP, PORT
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        servaddr.sin_port = htons(config->port);

        // Binding newly created socket to given IP and verification
        if ((bind(serverSocketID, (sockaddr*) &servaddr, sizeof(servaddr))) != 0) {
            PDBG(TAG,"socket bind failed...\n")
            ScreenConsole::print("socket bind failed...\n");
            return;
        }
        else {
            PDBG(TAG, "Socket successfully binded..\n")
            ScreenConsole::print("successfully binded\n");
        }

        // Now server is ready to listen and verification
        if ((listen(serverSocketID, 5)) != 0) {
            PDBG(TAG,"Listen failed...\n")
            ScreenConsole::print("Listen failed\n");
            return;
        }
        else {
            PDBG(TAG, "Server listening..\n")
        }
        len = sizeof(clientAddr);

        if(afterCreated != NULL)
            afterCreated->start();

        while(!stopFlag) {
            // Accept the data packet from client and verification
            connfd = accept(serverSocketID, &clientAddr, &len);
            if (connfd < 0) {
                PDBGF(TAG, "server acccept failed %d", errno);
//                switch(errno){
//                    case EFAULT:
//                        PDBG(TAG, "server accept fault\n")
//                        break;
//                    case ECONNABORTED:
//                        PDBG(TAG, "server accept aborted...\n")
//                        break;
//                    case EINTR:
//                        PDBG(TAG, "server accept aborted\n")
//                        break;
//                    case ENOTSOCK:
//                        PDBG(TAG, "server accept aborted, no valid socket\n")
//                        break;
//                    case EOPNOTSUPP:
//                        PDBG(TAG, "server accept aborted, wrong type of socket\n")
//                        break;
//                    case EBADF:
//                        PDBG(TAG, "server accept aborted, socket not open\n")
//                        stop();
//                        break;
//                }

                switch(errno) {
                    case EBADF:
                        PDBG(TAG, "server accept aborted, socket not open\n")
                        return;
                    case ENETDOWN: case EPROTO: case ENOPROTOOPT: case EHOSTDOWN: case ENONET: case EHOSTUNREACH: case EOPNOTSUPP: case ENETUNREACH:
                        PDBG(TAG,"received error, but should try again")
                        usleep(5000000);
                        break;
                    default:
                        PDBG(TAG,"received unrecoverable error")
                        return;
                }
            } else {
                PDBG(TAG, "server acccept the client...\n")
                current->addProcessor(&clientAddr, connfd, clientCount++);
//                ClientHandler * c = new ClientHandler(config, &clientAddr, connfd, entityCount++,this);
//                (*clients)[c->key()] = c;

            }
        }

        return;
    }

    ServerTCP::~ServerTCP() {
        stop();

        if(close(serverSocketID) == 0){
            PDBG(TAG,"closed correctly")
        }
        else{
            PDBGF(TAG,"failed to close server %d", errno);
        }

        for(std::unordered_map<int,ClientHandler*>::iterator it = clientHandlers.begin(); it != clientHandlers.end(); it++){
            delete it->second;
        }
        delete current;

            //delete clients;
//            clients= nullptr;


    }

    bool ServerTCP::isCurrent(ClientHandler *handler) {
        return handler->key()==current->key();
    }

}
