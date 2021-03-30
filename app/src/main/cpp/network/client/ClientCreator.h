//
// Created by dawnoftherent on 3/20/21.
//

#ifndef SIMPLEGROUPCOMMSERVER_CLIENTCREATOR_H
#define SIMPLEGROUPCOMMSERVER_CLIENTCREATOR_H

#include "Client.h"
#include "../../util/Config.h"
#include <vector>

namespace com_curiousorigins_simplegroupcommserver {
    class ClientCreator {
    private:
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
        static void* speak_work(void* speakData);
        static pthread_t speak(Client * from, Client * to, const char* data, int& status);

    public:
        ClientCreator(Config * config);
        ~ClientCreator();
        void start();
    };
}


#endif //SIMPLEGROUPCOMMSERVER_CLIENTCREATOR_H
