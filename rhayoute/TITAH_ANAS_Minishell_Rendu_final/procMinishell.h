/** La gestion des processus lancés depuis 
 ** le (mini)shell.
 ** @author : Anas Titah
 ** @version : 1.0
*/

#ifndef __PROCMINISHELL_H
#define __PROCMINISHELL_H

#define TAILLE_ELEMENT_LISTE 512

/* Structure de stockage des informations d'un proessus
   (pour la commande list). */

typedef struct infoproc infoproc;
struct infoproc {
        int id_proc;    /* Identifiant propre au minishell du processus. */
        pid_t pid_proc; /* Pid du processus. */
        char etat[10]; /* Etat du processus. */
        char commande[256]; /* La commande lancée. */
        };


/* Fonctions et procédures pour manipuler Liste*/

int tailleListe (infoproc **liste_proc);

int positionProc(infoproc **liste_proc,pid_t pid_processus);

pid_t IdPidProc(infoproc **liste_proc,int id);

void ajouterProc(infoproc **liste_proc, infoproc *processus);

void supprimerProc(infoproc **liste_proc, pid_t pid_processus);

void afficherListe(infoproc **liste_proc);


#endif













