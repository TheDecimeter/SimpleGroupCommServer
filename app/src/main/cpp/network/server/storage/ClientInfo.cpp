//
// Created by dawnoftherent on 3/30/21.
//

#include "ClientInfo.h"


#define TAG "ClientInfoCpp"
namespace com_curiousorigins_simplegroupcommserver {
    ClientInfo::ClientInfo(uint32_t id, int socketID):
    id(id), socketID(socketID), dibs(0), deleteSoon(false){
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

    bool ClientInfo::addDibb(Buffer *bufferToSend) {
        if(deleteSoon)
            return false;

        if(currentBuffer == NULL)
            currentBuffer = bufferToSend;
        else
            lastBuffer->next = bufferToSend;

        lastBuffer=bufferToSend;
        lastBuffer->next=NULL;

        dibs++;
        return true;
    }

    void ClientInfo::removeDibb() {
        lock.lock();
        dibs--;
        if(dibs == 0){
            if(deleteSoon)
                delete this;
        }
        lock.unlock();
    }

    ssize_t ClientInfo::send() {
        if(lock.try_lock()) {
            if (deleteSoon || currentBuffer == NULL) {
                lock.unlock();
                return FORGET_ME;
            }

            int totalBytesWritten = 0;

            while(totalBytesWritten <= BYTES_TO_WRITE) {
                ssize_t bytesWritten = write(socketID, currentBuffer->readerPosition,
                                             currentBuffer->objLen);

                if (bytesWritten == currentBuffer->bytesToRead) { //progress to next buffer
                    Buffer *tmp = currentBuffer->next;
                    currentBuffer->release();
                    currentBuffer = tmp;
                    if(tmp==NULL) {
                        lock.unlock();
                        return DONE_SENDING;
                    }
                } else if (bytesWritten > 0) {
                    currentBuffer->readerPosition += bytesWritten;
                    currentBuffer->bytesToRead -= bytesWritten;
                    lock.unlock();
                    return bytesWritten;
                } else if (bytesWritten <= 0) {
                    lock.unlock();
                    return bytesWritten;
                }

                if (bytesWritten != currentBuffer->bytesToRead) {
                    PDBG(TAG, "mismatch l: %d, w: %d, e: %d", currentBuffer->objLen, bytesWritten,
                         errno)
                }

                totalBytesWritten+=bytesWritten;
            }


            lock.unlock();
            return totalBytesWritten;
        }
        else
            return IN_USE;
    }

}