//
// Created by dawnoftherent on 4/5/21.
//

#ifndef SIMPLEGROUPCOMMSERVER_BUFFER_H
#define SIMPLEGROUPCOMMSERVER_BUFFER_H


#include <unistd.h>

namespace com_curiousorigins_simplegroupcommserver {
    class Buffer {
    public:
        char * data, * readerPosition;
        size_t dataLen, objLen;
        size_t bytesToRead;
        Buffer * next;

        Buffer(size_t dataLen);
        ~Buffer();

        void resize(size_t dataLen);
        void reset(size_t len);
        void release();
        bool isReleased();
        ssize_t fillBuffer(int socketID);
    };
}


#endif //SIMPLEGROUPCOMMSERVER_BUFFER_H
