package com.curiousorigins.simplegroupcommserver.network.server;

import android.util.Log;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

public class ServerTCP {
    public static final String TAG = "ServerTCP";
    public static final int port=12345;
    private static int entityCount=0;
    private boolean stopFlag=false;
    private ServerSocket serverSocket=null;
    private Thread afterCreated;

    Map<SocketState,SocketState> connections;

    public ServerTCP(Thread afterCreated){
        stop();
        stopFlag=false;
        connections=new ConcurrentHashMap<SocketState,SocketState>();
        this.afterCreated=afterCreated;
    }

    public void stop(){
        Log.d(TAG, "ServerTCP: requesting server closes");
        stopFlag=true;
        try {
            if(serverSocket!=null && !serverSocket.isClosed()) {
                serverSocket.close();
                serverSocket=null;
            }
        } catch (IOException e) {
            e.printStackTrace();
        }

        if(connections!=null)
            for(SocketState ss : connections.values())
                ss.close();

    }

    public void start(){




        new Thread(new Runnable(){



            @Override
            public void run() {


                try {
                    serverSocket = new ServerSocket(port);
                } catch (IOException e) {
                    Log.d(TAG, "failed to establish server on port "+port);
                    e.printStackTrace();
                }

                Log.d(TAG, "ServerTCP: server started "+serverSocket.getLocalSocketAddress());

                if(afterCreated!=null)
                    afterCreated.start();


                stopFlag=false;


                    // Loop forever
                    while(true) {
                        //System.out.println("before creating socket");
                        // Create a socket
                        Socket socket = null;
                        try {
                            socket = serverSocket.accept();
                            Log.d(TAG,"ServerTCP: new socketAccepted from " + socket.getRemoteSocketAddress());

                            if (stopFlag) {
                                System.out.println("received stop signal, ignoring new connections and closing server");
                                break;
                            }
                            //                    else
                            //                        System.out.println("received new connection");

                            handleResponse(new SocketState(socket, entityCount++));
                        } catch (IOException e) {
                            Log.d(TAG, "failed to accept connection");
                            e.printStackTrace();
                        }

                    }


            }
        }).start();
    }


    private void handleResponse(final SocketState ss){
        connections.put(ss,ss);
        Log.d(TAG," added socket, total sockets: "+connections.size());

        new Thread(new Runnable() {

            @Override
            public void run() {
                /*
                set up socket readers
                store socket in dictionary

                 */

                // Read data from the socket into a BufferedReader
                BufferedReader buffer = null;
                try {
                    buffer = new BufferedReader(new InputStreamReader(ss.socket.getInputStream()));
                } catch (IOException e) {
                    Log.d(TAG, "failed to get input stream");
                }

                // Read to the next newline (\n) and display that text on labelMessage
                //labelMessage.setText(buffer.readLine());

                //String s = StringParse.readAll(buffer);

                try {
                while(true){
                    String s= null;
                        s = buffer.readLine();
                        //System.out.println("GameComm: inputted line:\n" + s);

                        //if the "pipe" is not ready to be closed.
                        if(s!=null&&s.length()>0) {
                            //use the socket wrapper to verify that the input is correctg
                            Log.d(TAG, "received message "+s);


    //                        String response="un answered response";
    //                        if(s.equals(ConnectionStrings.queryForServerQuery.trim()))
    //                            response=ConnectionStrings.queryForServerResponse;
    //                        else
    //                            System.out.println("GameComm: message was not a query");
    //
    //                        //and send it
    //                        try {
    //                            socket.getOutputStream().write(response.getBytes());
    //                            System.out.println("GameComm: sent response message");
    //                        } catch (IOException e) {
    //                            System.out.println( "      Error: failed to send" + e.getMessage());
    //                        }

                        }
                        else {
                            ss.close();
                            break;
                        }
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                    ss.close();
                }
            }

        }).start();
    }

    class SocketState{
        public Socket socket;
        public final int id;
        public SocketState(Socket socket, int id){
            this.socket=socket;
            this.id=id;
        }

        public void close(){
            try {
                if(!socket.isClosed())
                    socket.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        @Override
        public int hashCode() {
            return socket.hashCode();
        }

        @Override
        public boolean equals(Object obj) {
            if(!(obj instanceof SocketState))
                return false;
            SocketState o = (SocketState) obj;
            return socket.equals(o.socket);
        }
    }

}
