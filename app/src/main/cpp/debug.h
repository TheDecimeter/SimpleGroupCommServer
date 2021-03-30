#ifndef SIMPLEGROUPCOMMSERVER_DEBUG_H
#define SIMPLEGROUPCOMMSERVER_DEBUG_H

#define DEBUG
#define PRINT


#ifdef DEBUG
#include <android/log.h>
    #define PDBG(TAG, args...) __android_log_print(ANDROID_LOG_DEBUG, TAG, args);
    #define PDBGF(TAG,MSG,ARG) __android_log_print(ANDROID_LOG_DEBUG,TAG,MSG,ARG);
#else
    #define PDBG(TAG,MSG)
    #define PDBGF(TAG,MSG,ARG)
#endif

#ifdef PRINT
#include <android/log.h>
    #define PPRNT(TAG,MSG) __android_log_print(ANDROID_LOG_INFO,TAG,MSG);
    #define PPRNTF(TAG,MSG,ARG) __android_log_print(ANDROID_LOG_INFO,TAG,MSG,ARG);
#else
    #define PPRNT(TAG,MSG)
    #define PPRNTF(TAG,MSG,ARG)
#endif

#endif //SIMPLEGROUPCOMMSERVER_DEBUG_H
