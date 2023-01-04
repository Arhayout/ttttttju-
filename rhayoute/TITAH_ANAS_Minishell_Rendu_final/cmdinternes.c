/** Les commandes internes du notre processus
 ** @author : Anas Titah
 ** @version : 1.0
*/
#include "cmdinternes.h"

/* Définition de quelque couleurs utilisées pour
** l'affichage des messages*/
#define ROUGE "\033[00;31m"
#define VERT  "\033[00;32m"
#define AUCUN "\033[00m"

/* La commande interne exit du minishell */
void cmd_exit(){
    exit(0);
}

/* La commande interne cd du minishell */
void cmd_cd(struct cmdline *c) {
    /* Traiter la commande cd sans lancer de processus fils. */
    if (c->seq[0][1] == NULL) {
        chdir("/");
    } else {
        chdir(c->seq[0][1]);
    }
}

/* La commande interne list du minishell */
void cmd_list(infoproc **liste){
    /* Afficher la liste des processus suspendus ou en exécution
    ** en arrière plan. */
    afficherListe(liste);
}

/* La commande interne stop du minishell */
void cmd_stop(struct cmdline *c, infoproc **liste){
    /* Suspendre le processus en utilisant son identifiant pour
    ** le minishell. */
    if (!(c->seq[0][1] == NULL)){
        pid_t pid_proc = IdPidProc(liste,atoi(c->seq[0][1]));
        int position = positionProc(liste,pid_proc);
        /* Envoyer le signal Stop au processus. */
        if (!(pid_proc == -1)) { 
            kill(pid_proc,19); 
            printf(VERT "Suspension du processus : ( Id = %d | Commande = %s )\n" AUCUN,liste[position]->id_proc,liste[position]->commande);
        } else {
            printf(ROUGE "Processus non trouvé ( Id = %d )\n" AUCUN,atoi(c->seq[0][1]));
        }
    } else {
        printf(ROUGE "Erreur : L'identifiant du processus est non saisi\n" AUCUN);
    }
}

/* La commande interner bg ou fg du minishell */
int cmd_bg_fg(struct cmdline *c, infoproc **liste){
    /* Traiter la commande fg ou bien bg. */
    if (!(c->seq[0][1] == NULL)){
        pid_t pid_proc = IdPidProc(liste,atoi(c->seq[0][1]));
        int position = positionProc(liste,pid_proc);
        if (!(position == -1)) { 
            /* Envoyer le signal Reprise au processus demandée. */
            kill(pid_proc,18);
            char plan[15] = "arrière-plan";
            if (!strcmp(c->seq[0][0],"fg")) { strcpy(plan,"avant-plan");};
            printf(VERT "Reprise du processus en %s: ( Id = %d | Commande = %s )\n" AUCUN,plan,liste[position]->id_proc,liste[position]->commande);
            
            /* Si la commande en entrée est bien fg, on attend jusqu'à la
            ** fin du processus. On peut traquer la fin en vérifiant le 
            ** changement de la valeur du processus_en_cours (C'est le PID du
            ** processus en cours d'exécution en avant plan, et si elle est  
            ** égale à 0, cela veut dire qu'il y en a aucun processus en avant 
            ** plan, on traite aussi le cas si un ctrl_z ou un ctrl_c est tapé,
            ** dans ce cas, la valeur de retour de cette fonction est égale
            ** au pid du processus, qui sera traité après dans le minishell.*/
            if (!strcmp(c->seq[0][0],"fg")) {
                return pid_proc;
            } 
        } else {
            printf(ROUGE "Processus non trouvé ( Id = %d )\n" AUCUN,atoi(c->seq[0][1]));
            return -1;
        }
    } else {
        printf(ROUGE "Erreur : L'identifiant du processus est non saisi\n" AUCUN);
        return -1;
    }
    return -1;
}