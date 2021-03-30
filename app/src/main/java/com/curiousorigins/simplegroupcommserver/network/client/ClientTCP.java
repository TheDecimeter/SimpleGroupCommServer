package com.curiousorigins.simplegroupcommserver.network.client;

import android.util.Log;

import com.curiousorigins.simplegroupcommserver.network.server.ServerTCP;

import java.io.IOException;
import java.net.Socket;

public class ClientTCP {
    public static final String TAG ="ClientTCP";
    private Socket socket;
    private String addr;

    public ClientTCP(String addr){
        this.addr=addr;
    }

    public void start(){
        try {
            socket=new Socket(addr, ServerTCP.port);
            Log.d(TAG, "connected with server at "+addr);
        } catch (IOException e) {
            Log.d(TAG, "failed to establish connection with "+addr);
            e.printStackTrace();
        }
    }

    public void stop(){
        if(socket!=null && !socket.isClosed()) {
            try {
                socket.close();
            } catch (IOException e) {
                Log.d(TAG, "failed to close socket");
                e.printStackTrace();
            }
        }
    }
}
