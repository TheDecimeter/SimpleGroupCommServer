//
// Created by dawnoftherent on 3/30/21.
//

#include "../../../debug.h"
#include "ClientInfo.h"

#define TAG "ClientInfoCpp"
namespace com_curiousorigins_simplegroupcommserver {
    ClientInfo::ClientInfo(uint32_t id, int socketID):
    id(id), socketID(socketID){
//        PDBG(TAG, "           creating defined at %p", this)
    }

    /**Dummy constructor for preallocation
     */
    ClientInfo::ClientInfo():
    id(0),socketID(-1){
//        PDBG(TAG, "            creating dummy at %p", this)
    }

    ClientInfo::~ClientInfo() {
//        PDBG(TAG, "                  deleting %d at %p",socketID, this)
    }

    ClientInfo::ClientInfo(ClientInfo &other):
    id(other.id), socketID(other.socketID){

    }

}