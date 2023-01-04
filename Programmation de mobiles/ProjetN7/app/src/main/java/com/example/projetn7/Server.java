package com.example.projetn7;

import android.annotation.SuppressLint;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothServerSocket;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.Button;

import java.io.IOException;
import java.util.UUID;

public class Server extends AppCompatActivity {

    private static final String TAG = "BT_CONN_SERV";


    private static final UUID MY_UUID = UUID.fromString("8ce255c0-223a-11e0-ac64-0803450c9a66");// UUID utilise par le client et le serveur
    private final BluetoothAdapter MyBtHandler = BluetoothAdapter.getDefaultAdapter();
    private static final String AppName = "N7PROJ"; // Nom de l'application
    byte[] bytes = new byte[1024];


    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_server);
        Intent intent = getIntent();
        ServerClass toto = new ServerClass();
        toto.start();// Lance la methode run()
        BT_Conn HandleMsgs =new BT_Conn();
        HandleMsgs.start();// Permet de lire les msgs
        HandleMsgs.write(bytes); // Permet d'ecrire les msgs
        toto.cancel();


    }

    private class ServerClass extends Thread {

        private BluetoothServerSocket monServerSocket = null;

        @SuppressLint("MissingPermission")
        public ServerClass() {

            BluetoothServerSocket tmp = null;
            try {

                tmp = MyBtHandler.listenUsingRfcommWithServiceRecord("Projet_N7", MY_UUID); // Server socket temporaire
                Log.d(TAG, "Le serveur se connecte");

            } catch (IOException e) {
                Log.e(TAG, "Socket's listen() method failed");
            }
            monServerSocket = tmp;
        }
        public void run(){
            Log.d(TAG,"Accept thread running...");
            BluetoothSocket socket=null;
            try{
                Log.d(TAG,"Server starts...");
                socket=monServerSocket.accept(); // Accepte la connexion du client
                Log.d(TAG,"Connection accepted...");
                Button btn = (Button) findViewById(R.id.serv_bt);
                btn.setText("Ready !");

            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        public void cancel(){
            Log.d(TAG,"Canceling Thread");
            try{

                monServerSocket.close();


            } catch (Exception e) {
                e.printStackTrace();
                Log.d(TAG,"Closing failed"+e.getMessage());
            }
        }
    }
    }




