//
// Created by dawnoftherent on 3/28/21.
//

#include "ClientProcessor.h"
#include "../../debug.h"
#include "../../ScreenConsole.h"


#include <arpa/inet.h>
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <linux/in6.h>
#include <stdlib.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#define TAG "    ClientPROCessorSERVcpp"
#define ST_READ_LEN 0
#define ST_READ_DATA 1
#define ST_PROCESS_DATA 2

namespace com_curiousorigins_simplegroupcommserver {


    ClientProcessor::ClientProcessor(const Config *c, int socketID, struct sockaddr * connectionInfo, uint32_t id, ClientManager * allClients):
    socketID(socketID), state(ST_READ_LEN), id(id), allClients(allClients), responder(allClients),
    buffer(255){ //TODO change bufLen to be a respectable size
        setClientAddr(connectionInfo);
        PDBG(TAG,"connection from %s",clientAddr);
        ScreenConsole::print({"Svr cnnt to: ",clientAddr,"\n"});

        fcntl(socketID, F_SETFL, O_NONBLOCK);

        welcome();
    }

    ClientProcessor::~ClientProcessor() {
        delete clientAddr;
        close(socketID);
    }

    void ClientProcessor::welcome(){
        buffer.data[0]=RELAY_REACT;
        buffer.data[1] = buffer.data[2] = static_cast<unsigned char>(id);
        buffer.objLen=3;
        responder.process(&buffer);
    }


    ssize_t ClientProcessor::process() {
//        if(id==0)
//            PDBG(TAG, "process num: %d", state);
        switch(state){
            case ST_READ_LEN:{
                ssize_t l = processLen();
                if(l<=0)
                    return l;
            }
            case ST_READ_DATA: {
                ssize_t l = processData();
                if(l != buffer.objLen)
                    return l;
            }
            case ST_PROCESS_DATA: {
                processResponse();
            }
            default:
                return 1;
        }
    }

    ssize_t ClientProcessor::processLen() {
//        PDBG(TAG, "PROCESSING LENGTH\n\n")
        unsigned char len=0;
        ssize_t  bytesRead = read(socketID, &len, 1);
        if(bytesRead == 0){
            terminate();
            return 0;
        }
        else if(bytesRead < 0){
            if(errno == EWOULDBLOCK || errno == EAGAIN){
                return -1;
            }
            PDBG(TAG," failed to read %d", errno);
            return -1;
        }

//        dataLen=len; //increase length size by one to compensate for message type byte
//        PDBG(TAG, "need to read of length %d", len);

        if(buffer.dataLen < len){
            buffer.resize(len);
        }
        buffer.reset(len);
        state = ST_READ_DATA;
        return 1;
    }

    ssize_t ClientProcessor::processData() {
        ssize_t bytesRead = buffer.fillBuffer(socketID);

        if(bytesRead == buffer.bytesToRead){ //done reading object
            state=ST_PROCESS_DATA;
//            PDBG(TAG, "rcv frm %d : %s", id, ScreenConsole::s(buf,dataLen).c_str())

//            ScreenConsole::print({"Server rcv: ", ScreenConsole::s(buf,dataLen),"\n"});
            //ScreenConsole::print({"Server rcv: ", buf,"\n"});

            responder.process(&buffer);

            return buffer.objLen;
        }else if(bytesRead>0){ //need to read more
//            PDBG(TAG, "Server rcv partial: %s", ScreenConsole::s(readerPos,bytesRead).c_str())
            buffer.bytesToRead -= bytesRead;
            buffer.readerPosition += bytesRead;
            return bytesRead;
        }else if(bytesRead == 0){ //need to close socket
            terminate();
            return 0;
        } else if (bytesRead < 0) { //error
            if(errno == EWOULDBLOCK || errno == EAGAIN){
                return -1;
            }
            PDBG(TAG, " failed to read %d", errno);
            if(errno == EINTR)
                return -2;
            return -1;
        }
        return -1;
    }

    ssize_t ClientProcessor::processResponse() {
        if(!responder.continueProcessing())
            state=ST_READ_LEN;
        return 1;
    }

    void ClientProcessor::setClientAddr(struct sockaddr *connectionInfo) {
        int port=-1;
        switch(connectionInfo->sa_family) {
            case AF_INET: {
                struct sockaddr_in *addr_in = (struct sockaddr_in *) &connectionInfo;
                clientAddr = new char[INET_ADDRSTRLEN+1];
                port=addr_in->sin_port;
                inet_ntop(AF_INET, &(addr_in->sin_addr), clientAddr, INET_ADDRSTRLEN);
                break;
            }
            case AF_INET6: {
                struct sockaddr_in6 *addr_in6 = (struct sockaddr_in6 *) &connectionInfo;
                clientAddr = new char[INET6_ADDRSTRLEN+1];
                port=addr_in6->sin6_port;
                inet_ntop(AF_INET6, &(addr_in6->sin6_addr), clientAddr, INET6_ADDRSTRLEN);
                break;
            }
            default:
                clientAddr = new char[1];
                clientAddr[0]=0;
                break;
        }
    }

    void ClientProcessor::terminate() {
//        PDBG(TAG,"%d received close message", id)
    }

    uint32_t ClientProcessor::key() {
        return id;
    }



}