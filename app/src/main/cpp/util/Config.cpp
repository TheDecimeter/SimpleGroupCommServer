//
// Created by dawnoftherent on 3/22/21.
//



#include "../debug.h"
#include "Config.h"
#include "StringTools.h"

#include <stdlib.h>

namespace com_curiousorigins_simplegroupcommserver {

    Config::Config() :
            port(static_cast<const uint32_t>((rand() % 10000) + 12000)),
            bufferSize(80),
             longestLoad(30000000),
            shortestLoad(10000000){

    }
}