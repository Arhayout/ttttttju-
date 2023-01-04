#ifndef __LISTE_H
#define __LISTE_H
#include <stdbool.h>


/*On définit un enregistrement qui contient les caractéristique d'un processus*/

struct caractProcessus {
    int idProcessus;        // Identifiant propre au minishell du processus. 
    pid_t pidProcessus;     // Pid du processus. 
    char state[10];           // Etat du processus.
    char commande[200];      // La commande lancée. 
};

typedef struct caractProcessus *caractProcessus;

//Fonctions utilisées.

//retourn la postion d'un processus
int positionProcessus(caractProcessus *liste_processus,pid_t pid_processus); 

//On vérifie si un processus appartient au liste 
bool exiteProcessus(caractProcessus *liste_processus,pid_t pid_processus); 
//ajouter un processus a la liste
//void addProcessus(caractProcessus *liste_processus, caractProcessus processus); 
void addProcessus(caractProcessus *liste_processus, int id , pid_t pid, char commande[200]);
//supprimer un processus a la liste
void deleteProcessus(caractProcessus *liste_processus, pid_t pid_processus); 
//afficher la liste des processus lancés au shell
void showListProcessus(caractProcessus *liste_processus); 
//Taille de liste.
int length(caractProcessus *liste_processus);


#endif
