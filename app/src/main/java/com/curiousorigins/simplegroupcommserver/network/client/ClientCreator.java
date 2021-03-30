package com.curiousorigins.simplegroupcommserver.network.client;

public class ClientCreator {
    public static class Bundle{
        public final Thread thread;
        public final ClientTCP [] clients;

         public Bundle(Thread thread, ClientTCP[] clients){
            this.thread=thread;
            this.clients=clients;
        }

        public void stop(){
             for(ClientTCP c : clients)
                 c.stop();
        }
    }
    public static Bundle one(String address){

        final ClientTCP client=new ClientTCP(address);
        Thread t= new Thread(new Runnable() {
            @Override
            public void run() {
                client.start();
            }
        });
        ClientTCP [] a = new ClientTCP[1];
        a[0]=client;
        return new Bundle(t, a);
    }

    public static Bundle N(int N, String address){


        final ClientTCP [] a = new ClientTCP[N];
        for(int i=0; i<N; ++i)
            a[i]=new ClientTCP(address);

        Thread t= new Thread(new Runnable() {
            @Override
            public void run() {
                for(int i=0; i<a.length; ++i)
                    a[i].start();
            }
        });

        return new Bundle(t, a);
    }

}
