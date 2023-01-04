/** La gestion des processus lancés depuis 
 ** le (mini)shell.
 ** @author : Anas Titah
 ** @version : 1.0
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "procMinishell.h"

/* Ajouter un processus à la liste des processus. */
void ajouterProc(infoproc **liste_proc, infoproc *processus) {   
    if (positionProc(liste_proc,processus->pid_proc) == -1) {
        int longueur = tailleListe(liste_proc);
        liste_proc[longueur] = malloc(TAILLE_ELEMENT_LISTE);
        liste_proc[longueur]->id_proc = processus->id_proc;
        liste_proc[longueur]->pid_proc = processus->pid_proc;
        strcpy(liste_proc[longueur]->etat,"actif");
        strcpy(liste_proc[longueur]->commande,processus->commande); 
    }
}

/* Supprimer un processus de la liste des processus. */
void supprimerProc(infoproc **liste_proc, pid_t pid_processus){
    int longueur = tailleListe(liste_proc);
    int position = positionProc(liste_proc,pid_processus);
    if (!(position == -1)){
        int i;
        for (i = position+1; i < longueur ;i++){
            *(liste_proc[i-1]) = *(liste_proc[i]);
        }
        liste_proc[longueur-1] = NULL;
        free(liste_proc[longueur-1]);
        liste_proc = calloc(longueur-1,sizeof(liste_proc));
    }
}

/* Afficher la liste des processus. */
void afficherListe(infoproc **liste_proc){
    int longueur = tailleListe(liste_proc);
    int i;
    printf("Id minishell        PID            etat           ligne de commande lancée\n");
    for (i=0 ; i < longueur ; i++) {
        // Améliorer la présentation de la commande, le décalage.
        char espace[20]  =  "               ";
        char espace2[15] =  "          ";
        char espace3[15] =  "     ";
        char pid_chaine[10];
        char id_chaine[10];
        sprintf(pid_chaine,"%d",liste_proc[i]->pid_proc);
        sprintf(id_chaine,"%d",liste_proc[i]->id_proc);
        int j;
        for (j=0; j < 5 - strlen(id_chaine); j++) {
            strcat(espace," ");
        }
        for (j=0; j < 5 - strlen(pid_chaine); j++){
            strcat(espace2," ");
        }
        for (j=0; j < 10 - strlen(liste_proc[i]->etat); j++){
            strcat(espace3," ");
        }
        printf("%d%s%d%s%s%s%s\n",liste_proc[i]->id_proc,espace,liste_proc[i]->pid_proc,espace2,liste_proc[i]->etat,espace3,liste_proc[i]->commande);
    }
}

/* Retourner la position du processus dans la liste.  */ 
/* Fonction  retourne -1 si le processus n'existe pas */
/* dans la liste. */
int positionProc(infoproc **liste_proc,pid_t pid_processus){
    int position = -1;
    int i = 0;
    while(!(liste_proc[i] == NULL)) {
        if (liste_proc[i]->pid_proc == pid_processus) {position = i; break;}
        i++;
    }
    return position;
}

/* Retourner le pid d'un processus dans la liste       */
/* en cherchant avec l'identifiant géré par le minishell. */
/* Fonction  retourne -1 si le processus n'existe pas    */
/* dans la liste. */
pid_t IdPidProc(infoproc **liste_proc,int id){
    pid_t pid = -1;
    int i = 0;
    while(!(liste_proc[i] == NULL)) {
        if (liste_proc[i]->id_proc == id) {pid = liste_proc[i]->pid_proc ; break;}
        i++;
    }
    return pid;

}

/* La taille de la liste des processus.*/
int tailleListe (infoproc **liste_proc){
    int i = 0;
    while (!(liste_proc[i] == NULL)){ 
            i++;
            }
    return i;
}
