package com.example.bluetooth_connexion;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothServerSocket;
import android.bluetooth.BluetoothSocket;
import android.os.Handler;
import android.os.Message;
import android.os.Bundle;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.TextView;
import android.content.Intent;
import android.view.View;
import androidx.appcompat.app.AppCompatActivity;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Set;
import java.util.UUID;

/**
 * Classe principale, elle permet de verifier les droits d'accès et de lancer la connexion.
 */
public class MainActivity extends AppCompatActivity {

    // Les composantes de l'interface.
    Button listen, send, listDevices;
    ListView listView;
    TextView msg_box, status;
    EditText writeMsg;

    // BluetoothAdapter pour prendre l'état de Bluetooth et faire les connexions.
    BluetoothAdapter bluetoothAdapter;
    BluetoothDevice[] btArray;

    // Un Thread pour envoyer les messages à l'autre appreil
    // (La classe est à dessous de cette class).
    SendReceive sendReceive;

    // Les états de connexions.
    static final int STATE_LISTENING = 1;
    static final int STATE_CONNECTING = 2;
    static final int STATE_CONNECTED = 3;
    static final int STATE_CONNECTION_FAILED = 4;
    static final int STATE_MESSAGE_RECEIVED = 5;

    int REQUEST_ENABLE_BLUETOOTH = 1;

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
        // La méthode setContentView() permet de déterminer quel fichier layout utiliser.
        setContentView(R.layout.activity_main);

        findViewByIdes();
        // Prendre le BluetoothAdapter par défault.
        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();

        if(!bluetoothAdapter.isEnabled()) {
            Intent enableIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableIntent,REQUEST_ENABLE_BLUETOOTH);
        }

        implementListeners();
    }

    private void implementListeners() {

        /** Pour détecter que l'utilisateur a cliqué sur le bouton,
         *  il est nécessaire d'implémenter un listener.
         *  Il faut pour cela appeler la méthode setOnClickListener() sur nos buttons,
         *  puis d'implémenter l'interface OnClickListener de la méthode View.
         *  La méthode onClick() est appelée chaque fois que l'utilisateur appuie sur le bouton.
         */

        listDevices.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Set<BluetoothDevice> bt = bluetoothAdapter.getBondedDevices();
                String[] strings = new String[bt.size()];
                btArray = new BluetoothDevice[bt.size()];
                int index = 0;

                if( bt.size()>0) {
                    for(BluetoothDevice device : bt) {
                        btArray[index] = device;
                        strings[index] = device.getName();
                        index++;
                    }
                    ArrayAdapter<String> arrayAdapter = new ArrayAdapter<String>(getApplicationContext(),
                            android.R.layout.simple_list_item_1,strings);
                    listView.setAdapter(arrayAdapter);
                }
            }
        });

        listen.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                ServerClass serverClass = new ServerClass();
                // Provoque le changement, par le système d'exploitation,
                // de l'état de l'instance actuelle en Running.
                serverClass.start();
            }
        });

        listView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> adapterView, View view, int i, long l) {
                ClientClass clientClass = new ClientClass(btArray[i]);
                // Provoque le changement, par le système d'exploitation,
                // de l'état de l'instance actuelle en Running.
                clientClass.start();
                // Mise à jour le statut de connexion.
                status.setText("Connecting");
            }
        });

        send.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                String string = String.valueOf(writeMsg.getText());
                sendReceive.write(string.getBytes());
            }
        });
    }

    Handler handler = new Handler(new Handler.Callback() {
        @Override
        public boolean handleMessage(Message msg) {
            // Les messages qui indique un chagment d'état de connexion.
            switch (msg.what) {
                case STATE_LISTENING:
                    status.setText("Listening");
                    break;
                case STATE_CONNECTING:
                    status.setText("Connecting");
                    break;
                case STATE_CONNECTED:
                    status.setText("Connected");
                    break;
                case STATE_CONNECTION_FAILED:
                    status.setText("Connection Failed");
                    break;
                case STATE_MESSAGE_RECEIVED:
                    byte[] readBuff = (byte[]) msg.obj;
                    String tempMsg = new String(readBuff,0,msg.arg1);
                    msg_box.setText(tempMsg);
                    break;
            }
            return true;
        }
    });

    /**
     * La méthode suivante permettra de récupérer une référence vers des boutons, des champs texte
     * et étiquettes dans le XML.
     * Bien sûr, si vous voulez exploiter ses caractéristiques liées à son état,
     * alors il faut penser à caster  le résultat obtenu.
     */
    private void findViewByIdes() {

        // Trouver les composantes dans l'interface.
        listen = (Button) findViewById(R.id.listen);
        send = (Button) findViewById(R.id.send);
        listView = (ListView) findViewById(R.id.listview);
        msg_box = (TextView) findViewById(R.id.msg);
        status = (TextView) findViewById(R.id.status);
        writeMsg = (EditText) findViewById(R.id.writemsg);
        listDevices = (Button) findViewById(R.id.listDevices);
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
                    // Créer et envoyer un message.
                    Message message = Message.obtain();
                    message.what = STATE_CONNECTING;
                    handler.sendMessage(message);
                    // Retourner un socket si la connexion et établit avec le client.
                    socket = serverSocket.accept();
                } catch (IOException e) {
                    Message message = Message.obtain();
                    message.what = STATE_CONNECTION_FAILED;
                    handler.sendMessage(message);
                    e.printStackTrace();
                }

                if(socket != null) {
                    Message message = Message.obtain();
                    message.what = STATE_CONNECTED;
                    handler.sendMessage(message);
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
                Message message = Message.obtain();
                message.what = STATE_CONNECTED;
                handler.sendMessage(message);

                sendReceive = new SendReceive(socket);
                sendReceive.start();

            } catch (IOException e) {
                Message message = Message.obtain();
                message.what = STATE_CONNECTION_FAILED;
                handler.sendMessage(message);
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
                    handler.obtainMessage(STATE_MESSAGE_RECEIVED,bytes,-1,
                            buffer).sendToTarget();
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