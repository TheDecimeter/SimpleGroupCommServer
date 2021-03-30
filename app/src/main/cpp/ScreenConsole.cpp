//
// Created by dawnoftherent on 3/22/21.
//
#include <pthread.h>
#include <unistd.h>
#include "debug.h"
#include "ScreenConsole.h"

#define TAG "ScreenConsoleCpp"

namespace com_curiousorigins_simplegroupcommserver {
    std::vector<std::string> *  ScreenConsole::messageRef=NULL;
    std::mutex * ScreenConsole::printLockRef=NULL;

    void ScreenConsole::print(std::string msg) {
        if(printLockRef==NULL){
            PDBG(TAG,"not adding item to print to screen, null pointer")
            return;
        }
        //PDBGF(TAG,"adding element to print que %s",msg.c_str())
        printLockRef->lock();

        messageRef->push_back(msg);

        printLockRef->unlock();
    }


    void ScreenConsole::print(std::vector<std::string> msg) {
        if(printLockRef==NULL){
            PDBG(TAG,"not adding item to print to screen, null pointer")
            return;
        }
        if(msg.size() > 0) {
            std::string buf;
            std::vector<std::string>::iterator i;
            for (i = msg.begin(); i != msg.end(); i++)
                buf+=(*i);

            printLockRef->lock();

//            std::vector<std::string>::iterator i;
//            for (i = msg.begin(); i != msg.end(); i++)
//                messageRef->push_back(*i);
            messageRef->push_back(buf);
            msg.clear();
            printLockRef->unlock();
        }
    }

    ScreenConsole::~ScreenConsole() {
        if(packet!=NULL) {
            delete packet;
            packet=NULL;
        }
        ScreenConsole::printLockRef=NULL;
        ScreenConsole::messageRef=NULL;
    }

    ScreenConsole::ScreenConsole(useconds_t sleepTime, useconds_t updateTime, JavaVM *vm, JNIEnv *env, jobject * obj):
    sleepTime(sleepTime), updateTime(updateTime){
        listen=true;
        PDBG(TAG,"starting screen console");
        packet=new Packet(vm,obj);


        ScreenConsole::printLockRef=&printLock;
        ScreenConsole::messageRef=&messages;

        pthread_create(&threadID, NULL, &printWorkWrapper, this);
    }

    void *ScreenConsole::printWorkWrapper(void *thiz) {
        ((ScreenConsole*) thiz) -> printWork();
        return nullptr;
    }

    void ScreenConsole::printWork() {
        JNIEnv* env;
        usleep(sleepTime);


        (packet->vm)->AttachCurrentThread(&env,NULL);

        jclass clazz = env->GetObjectClass(packet->inst);
        jmethodID methodID = env->GetMethodID(clazz, "nativePrint", "(Ljava/lang/String;)V");



        //while listening, periodically go through the print log and print any new messages to the screen
        while(listen){

            if(messages.size() > 0) {
                printLock.lock();

                std::vector<std::string>::iterator i;
                for (i = messages.begin(); i != messages.end(); i++) {
                    jstring s = env->NewStringUTF((*i).c_str());
                    env->CallVoidMethod(packet->inst, methodID, s);
                    env->DeleteLocalRef(s);
                }

                messages.clear();
                printLock.unlock();
            }
            usleep(updateTime);
        }

        (packet->vm)->DetachCurrentThread();
    }

    std::string ScreenConsole::s(const char *str, int len) {
        char s[len+1];
        for(int i=0; i<len; ++i)
            s[i]=str[i];
        s[len]=0;
        return std::string(s);
    }

}