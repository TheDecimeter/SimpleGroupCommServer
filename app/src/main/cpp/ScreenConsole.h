//
// Created by dawnoftherent on 3/22/21.
//

#ifndef SIMPLEGROUPCOMMSERVER_SCREENCONSOLE_H
#define SIMPLEGROUPCOMMSERVER_SCREENCONSOLE_H

#include<pthread.h>
#include<mutex>
#include<string>
#include <vector>
#include<jni.h>

namespace com_curiousorigins_simplegroupcommserver {
    class ScreenConsole {
    private:

        struct Packet{
            Packet(JavaVM* vm, jobject * obj):
            vm(vm),inst(*obj){}
            jmethodID method;
            JavaVM * vm;
            jobject inst;
        };

        Packet * packet;
        useconds_t sleepTime, updateTime;
        bool listen=true;
        pthread_t  threadID;

        std::vector<std::string> messages;
        std::mutex printLock;
        static std::vector<std::string> * messageRef;
        static std::mutex * printLockRef;

        static void* printWorkWrapper(void* thiz);
        void printWork();

    public:
        ScreenConsole(useconds_t sleepTime, useconds_t updateTime, JavaVM* vm, JNIEnv* env, jobject * obj);
        ~ScreenConsole();
        static void print(std::string msg);
        static void print(std::vector<std::string> msg);
        static std::string s(const char* str, int len);

    };
}


#endif //SIMPLEGROUPCOMMSERVER_SCREENCONSOLE_H
