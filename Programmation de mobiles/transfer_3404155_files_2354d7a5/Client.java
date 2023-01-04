package com.example.projetn7;

import static android.Manifest.permission.BLUETOOTH_CONNECT;

import android.Manifest;
import android.Manifest.permission;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Message;
import android.support.annotation.RequiresApi;
import android.support.v4.app.ActivityCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;

import java.io.IOException;
import java.net.Socket;
import java.util.Optional;
import java.util.Set;
import java.util.UUID;

public class Client extends AppCompatActivity {

    //private static final UUID UUID = ;
    private static final UUID MY_UUID = UUID.fromString("8ce255c0-223a-14e0-ac64-0803758c9a66"); // UUID utilise par le client et le serveur
    private static final String TAG = "BT_CONN_SERV";
    Context MonCont;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_client);
        Intent intent = getIntent();

        System.out.println("TOTO HARP");


        BluetoothAdapter Adapter = BluetoothAdapter.getDefaultAdapter();

        @SuppressLint("MissingPermission") Set<BluetoothDevice> pairedDevices = Adapter.getBondedDevices();
        BluetoothDevice mydev = null;

        if (pairedDevices.size() > 0) {
            //  obtenir l'@ MAC et le nom  des appareils appairés.
            for (BluetoothDevice device : pairedDevices) {
                @SuppressLint("MissingPermission") String deviceName = device.getName();// Nom de l'appareil
                String deviceHardwareAddress = device.getAddress(); // MAC addresse de l'appareil
                mydev = Adapter.getRemoteDevice(deviceHardwareAddress);  // Obtenir un appareil appairé
            }
        }


        ClientClass Client_sock = new ClientClass(mydev);
        Client_sock.start(); // Lance la méthode run()
        BT_Conn HandleMsgs =new BT_Conn();
        HandleMsgs.start(); //Permet au client de lire les messages provenant du serveur



    }

    private class ClientClass extends Thread {
        private BluetoothDevice device;
        private BluetoothSocket socket;


        @SuppressLint("MissingPermission")
        public ClientClass(BluetoothDevice mon_appareil) {
            device = mon_appareil;
            // Obtenez un BluetoothSocket pour vous connecter avec le BluetoothDevice donné.
            try {




                socket = device.createRfcommSocketToServiceRecord(MY_UUID); // Creation du socket client

            } catch (IOException e) {
                e.printStackTrace();

            }
        }

        @SuppressLint("MissingPermission")
        public void run() {


            try {
                Log.d(TAG,"Starting to connect");
                socket.connect();
                Log.d(TAG,"Connection made");

            } catch (IOException e) {


                e.printStackTrace();
            }
        }
    }




        }

