#ifndef SIMPLEGROUPCOMMSERVER_DEBUG_H
#define SIMPLEGROUPCOMMSERVER_DEBUG_H

#define DEBUG
#define PRINT


#ifdef DEBUG
#include <android/log.h>
    #define PDBG(TAG, args...) __android_log_print(ANDROID_LOG_DEBUG, TAG, args);
#else
    #define PDBG(TAG,args...)
#endif

#ifdef PRINT
#include <android/log.h>
    #define PPRNT(TAG,args...) __android_log_print(ANDROID_LOG_INFO,TAG, args);
#else
    #define PPRNT(TAG,args...)
#endif

#endif //SIMPLEGROUPCOMMSERVER_DEBUG_H
