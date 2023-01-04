package com.example.projetn7;

import android.app.Activity;
import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Button clt_button = (Button) findViewById(R.id.clt);
        Button srv_button = (Button) findViewById(R.id.srv);
        clt_button.setOnClickListener(this);
        srv_button.setOnClickListener(this);


    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.clt:
                Button btn = (Button) findViewById(R.id.clt);
                // Création d’une intention
                Intent cltIntent = new Intent(this, Client.class);
                // Lancer l'activité client
                startActivity(cltIntent);
                break;


            case R.id.srv:
                Button server = (Button) findViewById(R.id.srv);
                // Création d’une intention
                Intent servIntent = new Intent(this, Server.class);
                // Lancer l'activité serveur
                startActivity(servIntent);
                break;

        }

    }
}
