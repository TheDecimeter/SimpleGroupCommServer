//
// Created by dawnoftherent on 3/29/21.
//

#include "Relay.h"
namespace com_curiousorigins_simplegroupcommserver {

    Relay::Relay(const char *data, const unsigned char dataLen):
    msgLen(dataLen), dataLen(dataLen), data(data), tst(NULL){

    }

    ssize_t Relay::run() {
        return 0;
    }

}