/* version 0.2 (PM, 13/5/19) :
	Le serveur de conversation
	- crée un tube (fifo) d'écoute (avec un nom fixe : ./ecoute)
	- gère un maximum de maxParticipants conversations : select
		* tube d'écoute : accepter le(s) nouveau(x) participant(s) si possible
			-> initialiser et ouvrir les tubes de service (entrée/sortie) fournis
		* tubes (fifo) de service en entrée -> diffuser sur les tubes de service en sortie
	- détecte les déconnexions lors du select
	- se termine à la connexion d'un client de pseudo "fin"
	Protocole
	- suppose que les clients ont créé les tube d'entrée/sortie avant
		la demande de connexion, nommés par le nom du client, suffixés par _C2S/_S2C.
	- les échanges par les tubes se font par blocs de taille fixe, dans l'idée d'éviter
	  le mode non bloquant
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#include <stdbool.h>
#include <sys/stat.h>

#define MAXPARTICIPANTS 5			/* seuil au delà duquel la prise en compte de nouvelles
								 						 	   connexions sera différée */
#define TAILLE_MSG 128				/* nb caractères message complet (nom+texte) */
#define TAILLE_NOM 25					/* nombre de caractères d'un pseudo */
#define NBDESC FD_SETSIZE-1		/* pour le select (macros non definies si >= FD_SETSIZE) */
#define TAILLE_RECEPTION 512	/* capacité du tampon de messages reçus */

typedef struct ptp { 					/* descripteur de participant */
    bool actif;
    char nom [TAILLE_NOM];
    int in;		/* tube d'entrée (C2S) */
    int out;	/* tube de sortie (S2C) */
} participant;


participant participants [MAXPARTICIPANTS];

char buf[TAILLE_RECEPTION]; 	/* tampon de messages reçus/à rediffuser */
int nbactifs = 0;

void effacer(int i) { /* efface le descripteur pour le participant i */
    participants[i].actif = false;
    bzero(participants[i].nom, TAILLE_NOM*sizeof(char));
    participants[i].in = -1;
    participants[i].out = -1;
}

void diffuser(char *dep) { /* envoi du message référencé par dep à tous les actifs */
    /* à faire */
    for (int i = 0; i < MAXPARTICIPANTS; i++){
        if (participants[i].actif == true){
            write(participants[i].out,dep,TAILLE_MSG);
        }
    }
}

void desactiver (int p) {
    /* On envoie un message pour notifier les autres utilisateurs
    ** que l'utilisateur p a quitté la conversation*/
    char message[TAILLE_MSG];
    sprintf(message,"[Service] %s a quitté la conversation !",participants[p].nom);
    diffuser(message);

    /* On ferme les tubes*/
    close(participants[p].in);
    close(participants[p].out);

    /* On efface le descripteur pour le participant p*/
    effacer(p);
    nbactifs --;
}

void ajouter(char *dep) { // traite la demande de connexion du pseudo référencé par dep
    /*  Si le participant est "fin", termine le serveur (et gère la terminaison proprement)
        Sinon, ajoute le participant de pseudo référencé par dep
        (à faire)
    */
    if (!strcmp(dep,"fin")){
        /* Envoyer un message de la déconnexion du serveur*/
        diffuser("Fin serveur ...");
        /* Attendre la diffusion du message*/
        sleep(1);

        /* Fermer les tubes ouverts aves le client*/
        for (int i = 0; i < MAXPARTICIPANTS; i++){
            if (participants[i].actif == true){
                close(participants[i].in);
                close(participants[i].out);
            }
        }

        /* Fermer le tube d'écoute ? variable ecoute n'est pas globale*/
        
        /* Fermer le serveur*/
        exit(0);
    } else if (nbactifs < MAXPARTICIPANTS){
        int indice = 0;
        /* Chercher l'indice où on ajoute*/
        while (participants[indice].actif == true){
            indice ++;
        }
        /* Ajouter le participant*/
        participants[indice].actif = true;
        sprintf(participants[indice].nom ,"%s",dep);
        char nom_fichier_in[TAILLE_NOM + 4];
        char nom_fichier_out[TAILLE_NOM + 4];
        sprintf(nom_fichier_in,"%s_c2s",dep);
        sprintf(nom_fichier_out,"%s_s2c",dep);
        int desc_in = open(nom_fichier_in,O_RDONLY);
        int desc_out = open(nom_fichier_out,O_WRONLY | O_TRUNC);
        participants[indice].in = desc_in;
        participants[indice].out = desc_out;
        nbactifs ++;

        /* Diffuser le message de rejoint*/
        char message[TAILLE_MSG];
        sprintf(message,"[Service] %s a rejoint la conversation !",participants[indice].nom);
        diffuser(message);
    }
}

int main (int argc, char *argv[]) {
    int i,nlus,necrits,res;
    int ecoute;					/* descripteur d'écoute */
    fd_set readfds; 		/* ensemble de descripteurs écoutés par le select */
    char * buf0; 				/* pour parcourir le contenu du tampon de réception */
	char bufDemandes [TAILLE_NOM*sizeof(char)*MAXPARTICIPANTS]; 
	/* tampon requêtes de connexion. Inutile de lire plus de MAXPARTICIPANTS requêtes */

    /* création (puis ouverture) du tube d'écoute */
    mkfifo("./ecoute",S_IRUSR|S_IWUSR); // mmnémoniques sys/stat.h: S_IRUSR|S_IWUSR = 0600
    ecoute=open("./ecoute",O_RDONLY);

    for (i=0; i<= MAXPARTICIPANTS; i++) {
        effacer(i);
    }
		
    while (true) {
        printf("participants actifs : %d\n",nbactifs);

		/* boucle du serveur : traiter les requêtes en attente 
				 * sur le tube d'écoute : lorsqu'il y a moins de MAXPARTICIPANTS actifs.
				 	ajouter de nouveaux participants et les tubes d'entrée.			  
				 * sur les tubes de service : lire les messages sur les tubes c2s, et les diffuser.
				   Note : tous les messages comportent TAILLE_MSG caractères, et les constantes
           sont fixées pour qu'il n'y ait pas de message tronqué, ce qui serait  pénible 
           à gérer. Enfin, on ne traite pas plus de TAILLE_RECEPTION/TAILLE_MSG*sizeof(char)
           à chaque fois.
           - dans le cas où la terminaison d'un participant est détectée, gérer sa déconnexion
		*/

        /* Initialisation de l'ensemble des descripteurs à écouter
        ** (XXX_c2s et ecoute)*/
        FD_ZERO(&readfds);
        FD_SET(ecoute,&readfds);
        for (int i = 0; i < MAXPARTICIPANTS; i++){
            if (participants[i].actif == true){
                FD_SET(participants[i].in,&readfds);
            }
        }
        
        /* Vérifier si un descripteur est prêt*/
        res = select(FD_SETSIZE,&readfds,NULL,NULL,NULL);
        switch(res){
            case -1: /* erreur */
                //perror("Erreur select");
                if (FD_ISSET(ecoute,&readfds) && read(ecoute,bufDemandes,TAILLE_NOM) == 0){
                    printf("Aucune personne n'est connectée !\n");
                    exit(EXIT_SUCCESS);
                }
                break;
            case 0: /*Rien à faire*/

                break;
            default:
                /* Au moins un descripteur est prêt*/
                if ((nbactifs < MAXPARTICIPANTS) && (FD_ISSET(ecoute,&readfds))){
                    /* Traiter le cas des requêtes en attente sur le tube d'écoute*/
                    nlus = read(ecoute,bufDemandes,TAILLE_NOM);
                    ajouter(bufDemandes);

                } else {
                    /* Traiter le cas des requêtes en attente sur les tubes de service*/
                    for (int i = 0; i < MAXPARTICIPANTS; i++){
                        if ((participants[i].actif == true) && (FD_ISSET(participants[i].in,&readfds))){
                            nlus =  read(participants[i].in,buf,TAILLE_MSG);
                            diffuser(buf);
                            char aurevoir[TAILLE_MSG];
                            sprintf(aurevoir,"[%s] au revoir",participants[i].nom);
                            if (!strcmp(buf,aurevoir)){
                                desactiver(i);
                            }
                        }
                    }
                }

                /* Vider les tampons*/
                bzero(buf,TAILLE_RECEPTION);
                bzero(bufDemandes,TAILLE_NOM*sizeof(char)*MAXPARTICIPANTS);

        }   

    }
}