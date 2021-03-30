//
// Created by dawnoftherent on 3/29/21.
//

#ifndef SIMPLEGROUPCOMMSERVER_RUNNABLE_H
#define SIMPLEGROUPCOMMSERVER_RUNNABLE_H

#include <unistd.h>

namespace com_curiousorigins_simplegroupcommserver {
    class Runnable {
    public:
        virtual ssize_t run() = 0;
    };
}


#endif //SIMPLEGROUPCOMMSERVER_RUNNABLE_H
