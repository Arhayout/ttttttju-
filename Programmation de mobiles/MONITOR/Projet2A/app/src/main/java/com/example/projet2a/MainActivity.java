package com.example.projet2a;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        // Création du Button
        Button btn = (Button) findViewById(R.id.monitorbtn);
        btn.setOnClickListener(this);
    }

    @Override
    public void onClick(View v) {
        switch(v.getId()){
            case R.id.monitorbtn:
                Button btn = (Button) findViewById(R.id.monitorbtn);
                btn.setText(R.string.ready_label_clic);
                //Création d'une Intent
                Intent playIntent = new Intent(this, MonitoringActivity.class);
                // Ajout d’un parametre à l'intent
                playIntent.putExtra("name", "My Name");
                startActivity(playIntent);
                break;
        }
    }
}
