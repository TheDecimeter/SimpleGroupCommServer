//
// Created by dawnoftherent on 3/29/21.
//

#include "ClientResponder.h"
#include "responders/Relay.h"

namespace com_curiousorigins_simplegroupcommserver {

    ClientResponder::ClientResponder(){

    }
    /**
     * Start processing data, be aware that this data should remain untouched until
     * <code>process()</code> returns false;
     * @param data - a pointer to some data somewhere, this is never deleted
     * @param len - the length of the data
     */
    void ClientResponder::process(const char *data, unsigned char len) const{
        if(len>0)
            process(data[0], data+1, --len);
    }

    /**
     * work for processing with data in a more meaningful state
     * @param dataType
     * @param data
     * @param len
     */
    void ClientResponder::process(const char dataType, const char *data, const unsigned char len) const{

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
}