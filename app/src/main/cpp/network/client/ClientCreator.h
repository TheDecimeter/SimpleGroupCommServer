//
// Created by dawnoftherent on 3/20/21.
//

#ifndef SIMPLEGROUPCOMMSERVER_CLIENTCREATOR_H
#define SIMPLEGROUPCOMMSERVER_CLIENTCREATOR_H

#include "Client.h"
#include "../../util/Config.h"
#include "../../debug.h"
#include <vector>

namespace com_curiousorigins_simplegroupcommserver {
    class ClientCreator {
    private:
#ifdef MEM_TEST
        char MEM_TEST_BLOCK[MEM_TEST_SIZE];
#endif
        struct SpeakData{
            SpeakData(Client *from, Client *to, const char *data, int& status);
            ~SpeakData();
            Client * from;
            Client * to;
            const char * data;
            int & status;
        };

        const Config * config;

        std::vector <Client*> clients;
        pthread_t creatorThread;

        static void* creationWorkWrapper(void* clientCreator);
        void creationWork();
        void disconnectStress(int times);
        void relayTest(int times);
        bool relay(const int count, const uint32_t from, const uint32_t to);
        static void* speak_work(void* speakData);
        static pthread_t speak(Client * from, Client * to, const char* data, int& status);
        void startClients();
        void stopClients();

    public:
        ClientCreator(Config * config);
        ~ClientCreator();
        void start();
    };
}


#endif //SIMPLEGROUPCOMMSERVER_CLIENTCREATOR_H
