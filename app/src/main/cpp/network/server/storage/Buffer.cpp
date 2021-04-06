//
// Created by dawnoftherent on 4/5/21.
//

#include "Buffer.h"

namespace com_curiousorigins_simplegroupcommserver {

    Buffer::~Buffer() {
        delete data;
    }

    Buffer::Buffer(size_t dataLen) {
        this->dataLen=dataLen;
        this->data=new char[dataLen];
        this->next=NULL;
        this->objLen=0;
    }

    void Buffer::resize(size_t dataLen) {
        delete data;
        this->dataLen=dataLen;
        this->data=new char[dataLen];
    }

    void Buffer::reset(size_t len) {
        readerPosition=data;
        bytesToRead=len;
        objLen=len;
    }



    ssize_t Buffer::fillBuffer(int socketID) {
        return read(socketID, readerPosition, bytesToRead);
    }

    void Buffer::release() {
        objLen = bytesToRead = 0;
    }

    bool Buffer::isReleased() {
        return objLen == 0;
    }
}