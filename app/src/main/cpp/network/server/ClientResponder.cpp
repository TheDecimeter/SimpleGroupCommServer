//
// Created by dawnoftherent on 3/29/21.
//

#include "ClientResponder.h"
#include "responders/Relay.h"
#include "../../debug.h"
#include "../../ScreenConsole.h"


#include <endian.h>

#define TAG "ClientResponderCpp"

namespace com_curiousorigins_simplegroupcommserver {

    ClientResponder::ClientResponder(ClientManager * allClients):
    allClients(allClients){

    }
    /**
     * Start processing data, be aware that this data should remain untouched until
     * <code>process()</code> returns false;
     * @param data - a pointer to some data somewhere, this is never deleted
     * @param len - the length of the data
     */
    void ClientResponder::process(const char *data, unsigned char len){
        if(len>0)
            process(data[0], data+1, --len);
    }

    /**
     * work for processing with data in a more meaningful state
     * @param reactionType
     * @param data
     * @param len
     */
    void ClientResponder::process(const char reactionType, const char *data, const unsigned char len){
        this->reactionType = reactionType;
        switch(reactionType){
            case RELAY_REACT:{
                relay(data,len);
                break;
            }
            case NO_REACT: default:{
                break;
            }
        }
    }

    /**
     * Finishes any processing work. Call this 1 or more times after
     *  <code>process(data, len)</code>. As long as this method returns true, it should be called
     *  again.
     * @return false if no processing needs to be done.
     */
    bool ClientResponder::process() {
        if(worker != NULL)
            return worker->run() == 0;
        return false;
    }

    void ClientResponder::relay(const char *data, const unsigned char len) {
        if(len > 4) {
            uint32_t relayTo = ntohl(*(reinterpret_cast<const uint32_t*>(data)));
            data += 4;
            int l = len - 4;
            PDBG(TAG, "rcv relay to %d : %s", relayTo, ScreenConsole::s(data, l).c_str())
            ScreenConsole::print({"S rcv: ", ScreenConsole::s(data, l), "\n"});

            ClientInfo to;
            if(allClients->tryGet(relayTo, &to)){
                PDBG(TAG, "rcv found socket %d", to.socketID)
                write(to.socketID,data, l);
            }
        }
        else{
            PDBG(TAG, "can't parse relay message")
            ScreenConsole::print("S can't parse relay message\n");
        }
    }
}