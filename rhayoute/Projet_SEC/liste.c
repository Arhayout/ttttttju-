
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "liste.h"

#define taille 500




/*le nombre l'element d'une liste*/
int length(caractProcessus *liste_processus){
    int i = 0;
    while (!(liste_processus[i] == NULL)){ 
            i++;
    }
    return i;
}

/* Donne la position d'un processus dans la liste des processus.*/
int positionProcessus(caractProcessus *liste_processus, pid_t pid) {
    int n = length(liste_processus);
    int i;
    for (i = 0; i < n; i++) {
        if (liste_processus[i]->pidProcessus == pid) {
            return i;
        }
    }
    return -1;
}

/*Savoir si un processus appartient au liste à partie son PID.*/
bool exiteProcessus(caractProcessus *liste_processus,pid_t pid) {
        int n = length(liste_processus);
        bool t=false;
        int i;
        for (i = 0; i < n; i++) {
            if (liste_processus[i]->pidProcessus == pid) {
                t=true;
            }
        }
        return t;
}

/* Ajouter un processus à la liste des processus. */
void addProcessus(caractProcessus *liste_processus, int id , pid_t pid, char commande[200]) {
    int n = length(liste_processus)+1;
    if (!exiteProcessus(liste_processus,pid)) {
        liste_processus[n - 1] = malloc(taille);
        liste_processus[n - 1]->idProcessus = id;
        liste_processus[n - 1]->pidProcessus = pid;
        strcpy(liste_processus[n - 1]->state, "ACTIF");
        strcpy(liste_processus[n - 1]->commande, commande);
    }
}



/* Supprimer un processus de la liste des processus. */
void deleteProcessus(caractProcessus *liste_processus, pid_t pid_processus) {
    int n = length(liste_processus);
    int p = positionProcessus(liste_processus, pid_processus);
    if(exiteProcessus(liste_processus,pid_processus)){
        int i;
        for (i = p+1; i < n; i++) {       
            liste_processus[i] = liste_processus[i + 1];
        } 
            free(liste_processus[n-1]);
            liste_processus[n-1] = NULL;
            //liste_processus = calloc(n-1,sizeof(liste_processus));
      
    }
}


/* Afficher la liste des processus. */
void showListProcessus(caractProcessus *liste_processus){
    int n = length(liste_processus);
    int i;
    printf("Id     PID       STAT          COMMAND\n");
    for (i=0 ; i < n ; i++) {
        if(!strcmp(liste_processus[i]->state,"ACTIF")){
        printf("%d      %d     %s       %s\n",liste_processus[i]->idProcessus,liste_processus[i]->pidProcessus,
                liste_processus[i]->state,liste_processus[i]->commande);}
        else{printf("%d      %d     %s    %s\n",liste_processus[i]->idProcessus,liste_processus[i]->pidProcessus,
                liste_processus[i]->state,liste_processus[i]->commande);}
    }
}

