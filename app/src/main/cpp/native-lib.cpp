

#include <jni.h>
#include <pthread.h>
#include "debug.h"
#include <string>
#include <unistd.h>
#include "Tests.h"
#include "network/server/ServerTCP.h"
#include "util/Config.h"
#include "ScreenConsole.h"


#define TAG "MainCpp"

namespace com_curiousorigins_simplegroupcommserver {
    static ServerTCP * server = NULL;
    static ClientCreator * clients = NULL;
    static JavaVM* jvm = NULL;
    static jobject activity = NULL; // GlobalRef
    static Config * config;
    static ScreenConsole * screenConsole = NULL;
//    static pthread_t hThread;

    void *threadFunction(void *irrelevant) {
        JNIEnv* env;
        PDBG("threadFunction", "thread start");
        usleep(5000000);


        PDBG("threadFunction", "Attaching");
        (jvm)->AttachCurrentThread(&env,NULL);
//
        jclass clazz = (env)->GetObjectClass(activity);
        jmethodID methodID = env->GetMethodID(clazz, "nativePrint", "(Ljava/lang/String;)V");//(env)->GetMethodID(clazz, "finish", "()V" );
        (env)->CallVoidMethod(activity, methodID, env->NewStringUTF("test"));
//
////        PDBG("threadFunction", "Not Detaching");
        PDBG("threadFunction", "Detaching");
        (jvm)->DetachCurrentThread();
        return NULL;
    }

    static jstring stringFromJNI(JNIEnv *env, jobject /* this */) {
        std::string hello = "Hello from C++111";
        return env->NewStringUTF(hello.c_str());
    }

    static jstring stringToJNI(JNIEnv *env, jobject thiz, jstring s) {

        std::string r = "6Relayed: ";
        Tests t = Tests();
        t.testInt(5);

//        ServerTCP server(NULL);
//        server.start();

        const char *nativeInput = env->GetStringUTFChars(s, NULL);
        std::string hello = r + std::string(nativeInput) + t.testStr();
        env->ReleaseStringUTFChars(s, nativeInput);

        return env->NewStringUTF(hello.c_str());
    }

    static void createNative(JNIEnv *env, jobject thiz){
        PDBG(TAG, "starting native methods")
        //env->GetJavaVM(&jvm);
       // ScreenConsole::bind(jvm,env);

        activity = env->NewGlobalRef(thiz);
        screenConsole=new ScreenConsole(5000000, 5000000, jvm, env, & activity);

        config = new Config();

        clients = new ClientCreator(config);

        server = new ServerTCP(config, clients);
        server->start();



//        pthread_t hThread;
//        pthread_create(&hThread, NULL, &threadFunction, NULL);


        //return (*env)->NewStringUTF(env, "Hello from JNI !");


//        server = new ServerTCP(NULL);
//        server->start();
    }
    static void deleteNative(JNIEnv *env, jobject thiz){
        PDBG(TAG, "cleaning up native methods")
        //ScreenConsole::clean();
        delete server;
        delete clients;
        delete config;
        delete screenConsole;
    }

    static JNINativeMethod functions[] = {
            {"createNative", "()V",                   reinterpret_cast<void*>(createNative)},
            {"deleteNative", "()V",                   reinterpret_cast<void*>(deleteNative)},
            {"stringFromJNI", "()Ljava/lang/String;",                   reinterpret_cast<void*>(stringFromJNI)},
            {"stringToJNI",   "(Ljava/lang/String;)Ljava/lang/String;", reinterpret_cast<void*>(stringToJNI)}
    };
}

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    com_curiousorigins_simplegroupcommserver::jvm = vm;
    JNIEnv* env;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }

    PDBG(TAG, "on load")

    // Find your class. JNI_OnLoad is called from the correct class loader context for this to work.
    jclass c = env->FindClass("com/curiousorigins/simplegroupcommserver/MainActivity");
    if (c == nullptr) return JNI_ERR;


    int rc = env->RegisterNatives(c,
                                  com_curiousorigins_simplegroupcommserver::functions,
                                  sizeof(com_curiousorigins_simplegroupcommserver::functions)/sizeof(JNINativeMethod));
    if (rc != JNI_OK) return rc;


    return JNI_VERSION_1_6;
}