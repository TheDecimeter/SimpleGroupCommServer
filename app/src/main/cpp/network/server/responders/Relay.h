//
// Created by dawnoftherent on 3/29/21.
//

#ifndef SIMPLEGROUPCOMMSERVER_RELAY_H
#define SIMPLEGROUPCOMMSERVER_RELAY_H

#include "../../../util/Runnable.h"

namespace com_curiousorigins_simplegroupcommserver {
    class Relay : public Runnable {
    private:
        const char * data;
        const char const * tst;
        const unsigned char msgLen;
        unsigned char dataLen;

    public:
        Relay(const char * data, const unsigned char dataLen);
        virtual ssize_t run();

    };
}


#endif //SIMPLEGROUPCOMMSERVER_RELAY_H
