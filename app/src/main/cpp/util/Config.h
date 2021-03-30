//
// Created by dawnoftherent on 3/22/21.
//

#ifndef SIMPLEGROUPCOMMSERVER_CONFIG_H
#define SIMPLEGROUPCOMMSERVER_CONFIG_H

#include <unistd.h>

namespace com_curiousorigins_simplegroupcommserver {
    class Config {
    public:
        const uint32_t port;
        const size_t bufferSize;
        const int longestLoad, shortestLoad;
        Config();
    };
}


#endif //SIMPLEGROUPCOMMSERVER_CONFIG_H
