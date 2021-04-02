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
        PDBG(TAG, "stopping client %d", clientID);
        if(connected)
            close(socketID);
        connected=false;
    }

    Client::Client(Config * c):
    config(c){
        connected=false;
    }

    void Client::receive(char *data) {
        if(connected){
            ssize_t bytesRead;
            unsigned char lenLeft, len;
//            do {
                PDBG(TAG, "reading for %d", clientID);
                bytesRead = read(socketID, &len, 1);
                if(bytesRead==0){
                    PDBG(TAG,"should close client, received 0");
                    return;
                }
                else if(bytesRead<0){
                    PDBG(TAG,"error received %d", errno);
                    return;
                }
                else{
                    PDBG(TAG, "ready to receive object of %d bytes", len);
                }
                lenLeft=len;
                bytesRead = read(socketID, data, lenLeft);

                if(bytesRead == lenLeft){
                    PDBG(TAG, "finished reading: %d of %d", bytesRead, len)
                    return;
                }else if(bytesRead < lenLeft) {
                    PDBG(TAG,"didn't get all bytes %d < %d", bytesRead, len);
                }else if(bytesRead==0){
                    PDBG(TAG,"should close client, received 0");
                    return;
                }
                else if(bytesRead<0){
                    PDBG(TAG,"error received %d", errno);
                    return;
                }

                data+=bytesRead;
                lenLeft-=bytesRead;
//            }while(true);



        }
        else{
            PDBG(TAG,"can't receive data with unconnected socket %d", clientID)
        }
    }

    void Client::send(const char *data, size_t dataLen) {
        if(connected){
            //PDBGF(TAG,"Client snd: %s", data)
//            ScreenConsole::print({"Client snd: ",data,"\n"});

            ssize_t bytesL = write(socketID, &dataLen, 1);
            if(bytesL < 1){
                PDBG(TAG,"failed to send size: %d e: %d", bytesL, errno);
            }
//            else{
//                PDBG(TAG,"Client sent Size: %d 1", bytesL);
//            }

            ssize_t bytes = write(socketID, data, dataLen);
            if(bytes < dataLen){
                PDBG(TAG,"failed to send size: %d of %d e: %d", bytes, dataLen, errno);
            }
//            else{
//                PDBG(TAG,"Client sent Size: %d %d", bytes, dataLen);
//            }
        }
        else{
            PDBG(TAG,"can't send data with unconnected socket %d", clientID)
        }
    }

    void Client::sendChunk(const char *data, size_t dataLen) {
        if(connected){
//            PDBGF(TAG,"Client snd: %s", data)
//            ScreenConsole::print({"Client snd: ",data,"\n"});

            ssize_t bytes = write(socketID, data, dataLen);
            if(bytes <= 0){
                PDBG(TAG,"failed to send all %d", errno);
            }
//            else{
//                PDBGF(TAG,"Client sent Size: %d", bytes);
//            }
        }
        else{
            PDBG(TAG,"can't send data with unconnected socket %d", clientID)
        }
    }

    void Client::greet() {
        char r[4];
        PDBG(TAG, "   GREETING")
        receive(r);
        clientID=r[0];
        PDBG(TAG, "greetings from client %d", clientID)
    }

    int Client::getID() {
        return clientID;
    }


}