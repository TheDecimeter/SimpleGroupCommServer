//
// Created by dawnoftherent on 3/19/21.
//

#include "Client.h"
#include "../../debug.h"
#include "../../ScreenConsole.h"


#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SA struct sockaddr
#define TAG "                 ClientCpp"

namespace com_curiousorigins_simplegroupcommserver {

    bool Client::start() {

        int sockfd;
        struct sockaddr_in servaddr, cli;

        // socket create and varification
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == -1) {
            PDBG(TAG,"socket creation failed...\n")
            return false;
        }
        else
            PDBG(TAG,"Socket successfully created..\n")

        bzero(&servaddr, sizeof(servaddr));

        // assign IP, PORT
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        servaddr.sin_port = htons(config->port);

        // connect the client socket to server socket
        if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
            PDBG(TAG,"connection with the server failed...\n")
            return false;
        }
        else {
            socketID=sockfd;
            connected=true;
            PDBG(TAG,"connected to the server..\n")
            return true;
        }
    }

    void Client::stop() {
        PDBG(TAG, "stopping client");
        if(connected)
            close(socketID);
        connected=false;
    }

    Client::Client(Config * c):
    config(c){
        connected=false;
    }

    void Client::receive(char *data, size_t dataLen) {
        if(connected){
            ssize_t bytesRead = read(socketID, data, dataLen);
            if(bytesRead==0){
                PDBG(TAG,"should close client, received 0");
            }
        }
        else{
            PDBG(TAG,"can't send data with unconnected socket")
        }
    }

    void Client::send(const char *data, size_t dataLen) {
        if(connected){
            //PDBGF(TAG,"Client snd: %s", data)
//            ScreenConsole::print({"Client snd: ",data,"\n"});

            ssize_t bytesL = write(socketID, &dataLen, 1);
            if(bytesL <= 0){
                PDBG(TAG,"failed to send all %d", bytesL);
            }
//            else{
//                PDBGF(TAG,"Client sent Size: %d", bytesL);
//            }

            ssize_t bytes = write(socketID, data, dataLen);
            if(bytes <= 0){
                PDBG(TAG,"failed to send all %d", bytes);
            }
//            else{
//                PDBGF(TAG,"Client sent Size: %d", bytes);
//            }
        }
        else{
            PDBG(TAG,"can't send data with unconnected socket")
        }
    }

    void Client::sendChunk(const char *data, size_t dataLen) {
        if(connected){
//            PDBGF(TAG,"Client snd: %s", data)
//            ScreenConsole::print({"Client snd: ",data,"\n"});

            ssize_t bytes = write(socketID, data, dataLen);
            if(bytes <= 0){
                PDBG(TAG,"failed to send all %d", bytes);
            }
//            else{
//                PDBGF(TAG,"Client sent Size: %d", bytes);
//            }
        }
        else{
            PDBG(TAG,"can't send data with unconnected socket")
        }
    }



}