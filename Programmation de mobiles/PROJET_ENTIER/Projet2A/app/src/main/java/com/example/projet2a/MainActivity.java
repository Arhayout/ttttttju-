package com.example.projet2a;

import androidx.appcompat.app.AppCompatActivity;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothServerSocket;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.TextView;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Set;
import java.util.UUID;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    // BluetoothAdapter pour prendre l'état de Bluetooth et faire les connexions.
    BluetoothAdapter bluetoothAdapter;
    BluetoothDevice[] btArray;

    // Un Thread pour envoyer les messages à l'autre appreil
    // (La classe est à dessous de cette class).
    SendReceive sendReceive;

    // Le nom de notre Application
    private static final String APP_NAME = "Bluetooth Connexion";

    // C'est l'UUID pour que les appareils peuvent s'identifier entre eux.
    private static final UUID MY_UUID = UUID.fromString("8ce255c0-223a-11e0-ac64-0803450c9a66");


    /**
     * La méthode onCreate() est appelée lorsque l'activité est créée.
     */
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        // Button du servuer
        Button btnServeur = (Button) findViewById(R.id.btn_serveur);
        btnServeur.setOnClickListener(this);
        // Button du cient
        Button btnClient = (Button) findViewById(R.id.btn_client);
        btnClient.setOnClickListener(this);
    }


    /** Pour détecter que l'utilisateur a cliqué sur le bouton,
     *  il est nécessaire d'implémenter un listener.
     *  Il faut pour cela appeler la méthode onClick()
     *  La méthode onClick() est appelée chaque fois que l'utilisateur appuie sur le bouton.
     */
    @Override
    public void onClick(View v) {
        // Les composantes de l'interface.
        Button btnServeur = (Button) findViewById(R.id.btn_serveur);
        Button btnClient = (Button) findViewById(R.id.btn_client);
        TextView tv = (TextView) findViewById(R.id.textView);
        switch(v.getId()) {

            // Partie traitement serveur
            case R.id.btn_serveur:
                // Cacher le button du client
                btnClient.setVisibility(View.INVISIBLE);
                // Changer le texte affiché dans le button du servuer
                btnServeur.setText("SERVEUR EN ATTENTE");
                // Changer le texte view afficher dans l'application
                tv.setText("* ATTENTE CONNECTION CLIENT *");
                // Appeler la classe du serveur
                ServerClass serverClass = new ServerClass();
                // Provoque le changement, par le système d'exploitation,
                // de l'état de l'instance actuelle en Running.
                serverClass.start();
                break;

            // Partie traitement client
            case R.id.btn_client:
                // Cacher le button du serveur
                btnServeur.setVisibility(View.INVISIBLE);
                // Changer le texte affiché dans le button du client
                btnClient.setText("CLIENT SE CONNECTE");
                // Changer le texte view afficher dans l'application
                tv.setText("* ATTENTE CONNECTION SERVEUR *");
                Set<BluetoothDevice> bt = bluetoothAdapter.getBondedDevices();
                String[] strings = new String[bt.size()];
                btArray = new BluetoothDevice[bt.size()];
                int index = 0;

                if (bt.size() > 0) {
                    for (BluetoothDevice device : bt) {
                        btArray[index] = device;
                        index++;
                    }
                    ArrayAdapter<String> arrayAdapter = new ArrayAdapter<String>(getApplicationContext(),
                            android.R.layout.simple_list_item_1, strings);
                }
                // Appeler la classe du client
                ClientClass clientClass = new ClientClass(btArray[0]);
                // Provoque le changement, par le système d'exploitation,
                // de l'état de l'instance actuelle en Running.
                clientClass.start();
                break;
        }
        //Création d'une Intent
        Intent playIntentS = new Intent(this, MonitoringActivity.class);
        // Ajout d’un parametre à l'intent
        startActivity(playIntentS);
    }


    public static class SocketHandler {

        private static BluetoothSocket socketblt;
        public static synchronized BluetoothSocket getSocket() {
            return socketblt;
        }
        public static synchronized void setSocket(BluetoothSocket socket){
            SocketHandler.socketblt = socket;
        }
    }


    /**
     * Le Thread de Socket Serveur pour recevoir les connexions envoyé par autre Socket Client.
     */
    private class ServerClass extends Thread {
        private BluetoothServerSocket serverSocket;

        public ServerClass() {
            try {
                serverSocket = bluetoothAdapter.listenUsingRfcommWithServiceRecord(APP_NAME,MY_UUID);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        public void run() {
            BluetoothSocket socket = null;
            while (socket == null) {
                try {
                    // Retourner un socket si la connexion et établit avec le client.
                    socket = serverSocket.accept();
                    SocketHandler.setSocket(socket);
                } catch (IOException e) {
                    e.printStackTrace();
                }
                if(socket != null) {
                    // Envoyer-Recevoir
                    sendReceive = new SendReceive(socket);
                    sendReceive.start();
                    break;
                }
            }
        }
    }


    /**
     * Le Thread pour le Socket Client de faire la connexion à autre Socket Serveur.
     */
    private class ClientClass extends Thread {
        private BluetoothDevice device;
        private BluetoothSocket socket;

        public ClientClass (BluetoothDevice nv_device) {
            device = nv_device;
            // Obtenez un BluetoothSocket pour vous connecter avec le BluetoothDevice donné.
            try {
                // MY_UUID est la chaîne UUID de l'application,
                // également utilisée par le code serveur.
                socket = device.createRfcommSocketToServiceRecord(MY_UUID);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        public void run() {
            bluetoothAdapter.cancelDiscovery();
            try {
                socket.connect();
                // Après réussite de la connexion.
                sendReceive = new SendReceive(socket);
                sendReceive.start();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }


    /**
     * Le Thread pour envoyer les messages (par Bytes).
     */
    private class SendReceive extends Thread {
        private final BluetoothSocket bluetoothSocket;
        private final InputStream inputStream;
        private final OutputStream outputStream;

        public SendReceive (BluetoothSocket socket) {
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

        public void run() {
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

        public void write(byte[] bytes) {
            try {
                outputStream.write(bytes);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}
