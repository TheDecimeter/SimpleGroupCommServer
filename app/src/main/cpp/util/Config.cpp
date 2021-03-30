//
// Created by dawnoftherent on 3/22/21.
//



#include "../debug.h"
#include "Config.h"
#include "StringTools.h"

namespace com_curiousorigins_simplegroupcommserver {

    Config::Config() :
    port(12358), bufferSize(80), longestLoad(30000000), shortestLoad(10000000){

    }
}