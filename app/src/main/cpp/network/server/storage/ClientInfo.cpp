//
// Created by dawnoftherent on 3/30/21.
//

#include "ClientInfo.h"


#define TAG "ClientInfoCpp"
namespace com_curiousorigins_simplegroupcommserver {
    ClientInfo::ClientInfo(uint32_t id, int socketID):
    id(id), socketID(socketID){
//        PDBG(TAG, "           creating defined at %p", this)
    }

    /**WARNING: doesn't create a good object state
     * Dummy constructor for memory preallocation
     */
    ClientInfo::ClientInfo(){
//        PDBG(TAG, "            creating dummy at %p", this)
    }

    ClientInfo::~ClientInfo() {
//        PDBG(TAG, "                  deleting at %p", this)
//        PDBG(TAG, "                  deleting %d at %p",socketID, this)
    }

    ClientInfo::ClientInfo(ClientInfo &other):
    id(other.id), socketID(other.socketID){
    }

}