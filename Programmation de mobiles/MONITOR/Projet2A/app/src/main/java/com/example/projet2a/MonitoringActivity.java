package com.example.projet2a;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.content.pm.ResolveInfo;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;

import org.w3c.dom.Text;

import java.io.IOException;
import java.io.InputStream;
import java.util.List;
import java.util.Scanner;

public class MonitoringActivity extends AppCompatActivity  implements View.OnClickListener{

    // Id du processus qu'il faut mettre à jour lors du click sur le button monitor
    int id_maj;
    final int MSG_CALCUL = 1;
    Runnable r = new Runnable() {

        public void run() {
            LinearLayout proclayout = (LinearLayout) findViewById(R.id.SVlayout);
            // Récupération des informations relatives aux processus.
            // Obtention de la liste des applications installées.
            final Intent principalIntent = new Intent(Intent.ACTION_MAIN, null);
            principalIntent.addCategory(Intent.CATEGORY_LAUNCHER);
            final List paquetAppList = getPackageManager()
                    .queryIntentActivities(principalIntent, 0);
            String appNom_actual;
            String rss_actual;
            int uid_actual = 0;
            String messageString = "Calcul en cours";

            for(Object object : paquetAppList) {
                ResolveInfo info = (ResolveInfo) object;
                String strPaquetNom = info.activityInfo
                        .applicationInfo.packageName.toString();
                System.out.println(strPaquetNom);

                // Lancement de la commande système ps
                Process processus = null;
                try {
                    processus = new ProcessBuilder("ps").start();
                } catch (IOException e) {
                    e.printStackTrace();
                    return;
                }

                InputStream input = processus.getInputStream();
                Scanner scanner = new Scanner(input);

                while (scanner.hasNextLine()) {
                    String ligne = scanner.nextLine();
                    if (ligne.startsWith("u0_")) {
                        String[] temp = ligne.split(" ");
                        String packageNom = temp[temp.length - 1];

                        // La mémoire qu’occupe le processus
                        if (strPaquetNom.equals(packageNom)) {
                            uid_actual = info.activityInfo.applicationInfo.uid;

                            if (id_maj == uid_actual) {
                                // Mettre à jour le RSS
                                rss_actual = temp[temp.length - 5];
                                // Mettre à jour le nom
                                appNom_actual = strPaquetNom;
                                // Mise à jour Global
                                String Messagetosend = Integer.toString(uid_actual)
                                        +" " + appNom_actual + " " + rss_actual;
                                Message msg = mHandler.obtainMessage(
                                        MSG_CALCUL, (Object) Messagetosend);
                                mHandler.sendMessage(msg);
                            }
                        }
                    }
                }
            }
            mHandler.postDelayed(r,5000);
        }
    };


    final Handler mHandler = new Handler(){
        public void handleMessage(Message msg){
            if (msg.what==MSG_CALCUL){
                LinearLayout proclayout = (LinearLayout) findViewById(R.id.SVlayout);
                String sentmsg = msg.obj.toString();
                String[] temp = sentmsg.split(" ");

                Button removebt = (Button) findViewById(Integer.parseInt(temp[0]));
                TextView removetv = (TextView)  findViewById(Integer.parseInt(temp[0])+70000);
                TextView removetv2 = (TextView)  findViewById(Integer.parseInt(temp[0])+140000);

                removebt.setVisibility(View.GONE);
                removetv.setVisibility(View.GONE);
                removetv2.setVisibility(View.GONE);

                View v = createProcessView(Integer.parseInt(temp[0]), temp[1],temp[2]);

                proclayout.addView(v);

                Toast.makeText(MonitoringActivity.this, "RSS updated",
                        Toast.LENGTH_LONG).show();
            }
        }
    };


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_monitoring);

        Intent intent = getIntent();
        String NomRecu = intent.getStringExtra("name");

        LinearLayout proclayout = (LinearLayout) findViewById(R.id.SVlayout);

        // Récupération des informations relatives aux processus
        // Obtention de la liste des applications installées.
        final Intent principalIntent = new Intent(Intent.ACTION_MAIN, null);
        principalIntent.addCategory(Intent.CATEGORY_LAUNCHER);
        final List packageAppList = getPackageManager()
                .queryIntentActivities(principalIntent, 0);
        String[] Tab_appnames = new String[packageAppList.size()];
        int[] Tab_UIDs = new int[packageAppList.size()];
        String[] Tab_RSSs = new String[packageAppList.size()];
        int compteur = 0;

        for(Object object : packageAppList) {
            ResolveInfo info = (ResolveInfo) object;
            String strPackNom = info.activityInfo
                    .applicationInfo.packageName.toString();
            System.out.println(strPackNom);

            //Lancement de la commande système ps
            Process process = null;
            try {
                process = new ProcessBuilder("ps").start();
            } catch (IOException e) {
                return;
            }
            InputStream in = process.getInputStream();
            Scanner scanner = new Scanner(in);
            while (scanner.hasNextLine()) {
                String line = scanner.nextLine();

                if (line.startsWith("u0_")) {
                    String[] temp = line.split(" ");
                    String packageName = temp[temp.length - 1];

                    // Mémoire qu’occupe le processus
                    if (strPackNom.equals(packageName)) {
                        Tab_appnames[compteur] = strPackNom;
                        Tab_UIDs[compteur] = info.activityInfo.applicationInfo.uid;
                        Tab_RSSs[compteur] = temp[temp.length - 5];
                        View v = createProcessView(Tab_UIDs[compteur], Tab_appnames[compteur],
                                Tab_RSSs[compteur]);
                        proclayout.addView(v);
                        compteur++;
                    }
                }
            }
        }
    }


    public View createProcessView(int UID, String appname, String monitorvalue){
        String  topleft, botleft;

        RelativeLayout rltvlayout = new RelativeLayout(this);

        // Nom de l'application
        RelativeLayout.LayoutParams paramsTopLeft =
                new RelativeLayout.LayoutParams(
                        RelativeLayout.LayoutParams.WRAP_CONTENT,
                        RelativeLayout.LayoutParams.WRAP_CONTENT);
        paramsTopLeft.addRule(RelativeLayout.ALIGN_PARENT_LEFT,
                RelativeLayout.TRUE);
        paramsTopLeft.addRule(RelativeLayout.ALIGN_PARENT_TOP,
                RelativeLayout.TRUE);

        RelativeLayout.LayoutParams paramsBottomLeft =
                new RelativeLayout.LayoutParams(
                        RelativeLayout.LayoutParams.WRAP_CONTENT,
                        RelativeLayout.LayoutParams.WRAP_CONTENT);
        paramsBottomLeft.addRule(RelativeLayout.ALIGN_PARENT_LEFT,
                RelativeLayout.TRUE);
        paramsBottomLeft.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM,
                RelativeLayout.TRUE);

        RelativeLayout.LayoutParams paramsTopRight =
                new RelativeLayout.LayoutParams(
                        RelativeLayout.LayoutParams.WRAP_CONTENT,
                        RelativeLayout.LayoutParams.WRAP_CONTENT);
        paramsTopRight.addRule(RelativeLayout.ALIGN_PARENT_RIGHT,
                RelativeLayout.TRUE);
        paramsTopRight.addRule(RelativeLayout.ALIGN_PARENT_TOP,
                RelativeLayout.TRUE);

        topleft = "[" + UID + "]" + appname;
        botleft = R.string.rss + monitorvalue;

        TextView topleft_tv = new TextView(this);
        topleft_tv.setText(topleft);
        // La méthode setid prend un int, pour cela on utilisera UID,
        // plus un grand nombre pour identifier les texts views.
        // Values of UID are up to 65536 IDs, ceci est fait poour simplification de code.
        // On pourra créer un tableau pour une correspondance des valeurs.
        topleft_tv.setId(UID+70000);

        TextView botleft_tv = new TextView(this);
        botleft_tv.setText(botleft);
        botleft_tv.setId(UID+140000);
        rltvlayout.addView(topleft_tv, paramsTopLeft);
        rltvlayout.addView(botleft_tv, paramsBottomLeft);

        Button btn = new Button(this);
        btn.setText(R.string.monitor_bt_txt);
        btn.setId(UID);
        rltvlayout.addView(btn, paramsTopRight);
        btn.setOnClickListener((View.OnClickListener) this);

        return rltvlayout;
    }


    @Override
    public void onClick(View v) {
        id_maj = v.getId();
        // UID, lancement du processus
        new Thread(r).start();
    }
}