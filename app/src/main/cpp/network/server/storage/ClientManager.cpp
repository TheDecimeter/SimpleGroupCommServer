//
// Created by dawnoftherent on 3/30/21.
//

#include "ClientManager.h"
#include "../../../debug.h"

#include <stdlib.h>

#define TAG "ClientManagerCpp"

namespace com_curiousorigins_simplegroupcommserver {

    ClientManager::ClientManager() {
        lock.unlock();
    }

    ClientManager::~ClientManager() {

    }

    /**
     * add a client to the manager
     * @param socketID - the client's socket id
     * @param outID - the new ID of the client (only valid if true is returned)
     * @return true if the client was successfully added
     */
    bool ClientManager::add(int socketID, uint32_t &outID) {
        ClientInfo * infoSpot = new ClientInfo;
//        PDBG(TAG, "before lock")
        lock.lock();
//        PDBG(TAG, "after lock")
        if(next()){
//            PDBG(TAG, "after next")
            ClientInfo * info = new(infoSpot) ClientInfo(clientCounter, socketID);
//            PDBG(TAG, "adding Info %d at %p %p", clientCounter, infoSpot, info);
            clients.insert(std::pair<int, ClientInfo*>(clientCounter, info));
            outID=clientCounter;
            lock.unlock();
            return true;
        }
        lock.unlock();

        delete infoSpot;
        return false;
    }

    /**
     * WARNING: not thread safe, must use <code>lock</code> first.
     * set <code>clientCounter</code> to the next available client index
     * @return true if able to find new client index
     */
    bool ClientManager::next() {
        clientCounter++;
        std::unordered_map<uint32_t, ClientInfo*>::iterator it;
        it = clients.find(clientCounter);

        if(it == clients.end())
            return true;

        if(clients.size() * 2 >= UINT32_MAX) {
            PDBG(TAG, "too many clients to create more %d", clients.size())
            return false;
        }
        while(it != clients.end()) {
//            PDBG(TAG, "trying %d", clientCounter)
            clientCounter = clientCounter = static_cast<uint32_t>(rand() + rand());
            it = clients.find(clientCounter);
        }
        return true;
    }

    /**
     * remove a client
     * @param id - the id of the client to remove
     */
    void ClientManager::remove(uint32_t id) {
//        PDBG(TAG, "pre lock removing %d", id);
        lock.lock();
//        PDBG(TAG, "post lock removing %d", id);
        std::unordered_map<uint32_t, ClientInfo*>::iterator it;
        it = clients.find(id);
        if(it != clients.end()) {
//            PDBG(TAG, "found %d", id);
            delete it->second; //XXX This might need to be changed out for destructor if I swap out for a pre allocated array
//            PDBG(TAG, "deleted %d", id);
            clients.erase(it);
//            PDBG(TAG, "erased %d", id);
        }
        lock.unlock();
    }

    bool ClientManager::tryGet(uint32_t id, ClientInfo ** outSpot){
        bool r=false;
//        PDBG(TAG, "tryGet prelock for %d", id);
        lock.lock_shared();
//        PDBG(TAG, "tryGet postLock for %d", id);
        std::unordered_map<uint32_t,ClientInfo*>::iterator it = clients.find(id);
        if(it != clients.end()){
            r=true;
            //new(outSpot) ClientInfo(*(it->second));
            *outSpot = it->second;
//            PDBG(TAG, "caching Info at %p, but info is at %p", outSpot, (it->second));
        }
        lock.unlock_shared();

        return r;
    }

    bool ClientManager::contains(uint32_t id) {
        lock.lock_shared();
        std::unordered_map<uint32_t,ClientInfo*>::iterator it = clients.find(id);
        lock.unlock_shared();

        return it == clients.end();
    }

}