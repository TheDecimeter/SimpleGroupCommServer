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
    void ClientResponder::process(Buffer * buffer){
        if(buffer->objLen>0) {
            buffer->readerPosition=buffer->data+1;
            buffer->objLen--;
            process(buffer->data[0], buffer);
        }
    }

    /**
     * work for processing with data in a more meaningful state
     * @param reactionType
     * @param data
     * @param len
     */
    void ClientResponder::process(const char reactionType, Buffer * buffer){
        this->reactionType = reactionType;
        switch(reactionType){
            case RELAY_REACT:{
                relay(buffer);
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
    bool ClientResponder::continueProcessing() {
//        if(worker != NULL)
//            return worker->run() == 0;
//        return false;
        switch(reactionType){
        case NO_WORK: default:{
                return false;
            }
            case SEND_WORK:{
                return false;
            }
        }
    }

    void ClientResponder::relay(Buffer * buffer) {
        if(buffer->objLen > 1) {
            uint32_t relayTo = (uint32_t) (buffer->readerPosition[0]);//ntohl(*(reinterpret_cast<const uint32_t*>(data)));
            buffer->readerPosition[0] = static_cast<unsigned char>(buffer->objLen - 1);
            //PDBG(TAG, "rcv relay to %d : %s", relayTo, ScreenConsole::s(data, l).c_str())
            //ScreenConsole::print({"S rcv: ", ScreenConsole::s(data, l), "\n"});

            if(allClients->tryGet(relayTo, &infoSpot, buffer)){
                PDBG(TAG, "rcv send size %d to %d at socket %d", buffer->objLen, relayTo, infoSpot->socketID)
                buffer->bytesToRead=buffer->objLen;
                this->buffer=buffer;
                send();
            }
            else{
                PDBG(TAG, "can't find referenced client %d", relayTo)
                ScreenConsole::print({"can't find referenced client: ", relayTo});
            }
        }
        else{
            PDBG(TAG, "can't parse relay message")
            ScreenConsole::print("S can't parse relay message\n");
        }
    }

    void ClientResponder::send() {
//        ssize_t bytesWritten = write(infoSpot->socketID,buffer->readerPosition, buffer->objLen);
//        if(bytesWritten != buffer->objLen){
//            PDBG(TAG, "mismatch l: %d, w: %d, e: %d", buffer->objLen, bytesWritten, errno)
//        }
        ssize_t written = infoSpot->send();


        if(buffer->isReleased()){
            infoSpot->removeDibb();
        }
    }
}