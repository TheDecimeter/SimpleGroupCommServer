package com.curiousorigins.simplegroupcommserver;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import com.curiousorigins.simplegroupcommserver.network.client.ClientCreator;
import com.curiousorigins.simplegroupcommserver.network.client.ClientTCP;
import com.curiousorigins.simplegroupcommserver.network.server.ServerTCP;

import java.util.LinkedList;


public class MainActivity extends Activity {

    private ServerTCP server;
    private ClientCreator.Bundle c1;
    private LinkedList<String> screenPrintMessages = new LinkedList<>();
    TextView console;

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }
    public String val =" hi i'm in java ";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = findViewById(R.id.sample_text);
        console=findViewById(R.id.console_text);
        //tv.setText(stringFromJNI());
        tv.setText(stringToJNI(" (sent through) "));


//        c1 = ClientCreator.N(5,"127.0.0.1");
//        c1.thread.start();

//        server = new ServerTCP(c1.thread);

//        server = new ServerTCP(null);
//        server.start();
    }

    public void nativePrint(String msg){
        //add message to end of list
        String[] ts = msg.split("\n");
        for (String t : ts) {
            if (t.length() > 0) {
                screenPrintMessages.addLast(t);
            }
        }


        //remove old messages if they go over
        while(screenPrintMessages.size() > 34)
            screenPrintMessages.removeFirst();

        final StringBuilder screenPrintConsole = new StringBuilder();

        for(String line : screenPrintMessages)
            screenPrintConsole.append(line).append("\n");

//        if(screenPrintConsole.length() > 4096){
//            screenPrintConsole.delete(0,screenPrintConsole.length()-4096);
//        }
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                console.setText(screenPrintConsole.toString());
            }
        });
    }

    @Override
    protected  void onResume(){
        Log.d("MAIN_ACTIVITY", "resuming");
        super.onResume();
        createNative();
    }

    @Override
    protected void onPause() {
        //server.stop();
        //c1.stop();
        Log.d("MAIN_ACTIVITY", "pausing");
        deleteNative();
        super.onPause();
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native void createNative();
    public native void deleteNative();
    public native String stringFromJNI();
    public native String stringToJNI(String s);
}
