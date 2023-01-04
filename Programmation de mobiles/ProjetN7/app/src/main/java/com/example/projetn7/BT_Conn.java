package com.example.projetn7;

import android.annotation.SuppressLint;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothServerSocket;
import android.bluetooth.BluetoothSocket;
import android.content.Context;
import android.nfc.Tag;
import android.util.Log;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.UUID;



public class BT_Conn extends Thread {
    private BluetoothSocket bluetoothSocket = null;
    private  InputStream inputStream=null;
    private  OutputStream outputStream = null;

    public void BT_Conn (BluetoothSocket socket) {
        bluetoothSocket = socket;
        InputStream temponIn = null;
        OutputStream temponOut = null;

        try {
            temponIn = bluetoothSocket.getInputStream();
            temponOut = bluetoothSocket.getOutputStream();
        } catch (IOException e) {
            e.printStackTrace();
        }

        inputStream = temponIn;
        outputStream = temponOut;
    }



    public void run() { // Methode pour lire les messages
        byte[] buffer = new byte[1024];
        int bytes;

        while (true) {
            try {
                bytes = inputStream.read(buffer);

            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    public void write(byte[] bytes) { //Methode pour ecrire les messages
        try {
            outputStream.write(bytes);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }}