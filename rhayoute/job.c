/** La gestion des processus lancés depuis 
 ** le (mini)shell.
 ** @author : Abdelmalek RHAYOUTE
*/

#define ANSI_COLOR_RED "\x1b[31m"
#include <stdlib.h> /* EXIT_SUCCESS */
#include <stdio.h>  /* entrées sorties: printf ... */
#include <unistd.h> /* pimitives de base : fork, ...*/
#include <string.h> /* opérations sur les chaînes */
/**atoi:converts the string argument str to an integer (type int).
*/
#include <sys/types.h>
#include <sys/wait.h> /* wait */



/* Structure de stockage des informations d'un proessus
   (pour la commande list). */
enum etat {
    bg, fg, suspendu
};


typedef enum etat etat_t;


typedef struct {
    int job_id;  /* Identifiant propre au minishell du processus. */
    pid_t job_pid; /* Pid du processus. */
    etat_t job_etat; /* Etat du processus. */
    char *ligne_cmdl;  /* La commande lancée. */
} job_t ;



typedef struct {
    int N ; //nombre de fils créés(actifs et suspendus)
    int max; //max des id
    job_t list[];
} list_jobs;

/* Fonctions et procédures pour manipuler Liste*/

/*Déclaration de liste_jobs*/
list_jobs liste_jobs;

/* Declaration du pid du processus en avant-plan */
pid_t pid_fg;


    
/*Trouver l'indice de job dans list_jobs à partir de son pid*/
/* Fonction  retourne -1 si le le job n'existe pas */
/* dans la liste. */
int get_job_indice(pid_t pid) {
    int i = 0;
    while(i<= liste_jobs.N) {
        if (liste_jobs.list[i].job_pid == pid) {
            return i;   
        }
        i++;
    }
    return -1;
}

/* Retourner le pid d'un job (processus) dans la liste       */
/* en cherchant avec l'identifiant géré par le minishell. */
/* Fonction  retourne -1 si le processus n'existe pas    */
/* dans la liste. */


    
int get_pid(int id) {
    int i = 0;
    while(i< liste_jobs.N) {
        if (liste_jobs.list[i].job_id == id) {
            return liste_jobs.list[i].job_pid;   
        }
        i++;
    }
    return -1;   
}
/*chnager l'état d'un proccessus*/

void set_etat(pid_t pid, etat_t e){
    int i;
    i=get_job_indice(pid);
    if (i>=0) {
    liste_jobs.list[i].job_etat = e;
    }
    else{printf(ANSI_COLOR_RED "aucune processus en fg.\n");
   }
}
        
    
/* Ajouter le processus fils dans liste_jobs. */
void add_job(int pid_fils, char* cmdl, etat_t etat) {
    liste_jobs.list[liste_jobs.N].job_id = liste_jobs.max +1;
    liste_jobs.list[liste_jobs.N].job_pid = pid_fils;
    /* enregistrer le nom de la première commande elementaire */
    char *ligne_cmdle = malloc(10*sizeof(char));
    strcpy(ligne_cmdle, cmdl);
    liste_jobs.list[liste_jobs.N].ligne_cmdl = ligne_cmdle;
    liste_jobs.list[liste_jobs.N].job_etat = etat;
    free(ligne_cmdle);
    liste_jobs.N++ ;
    liste_jobs.max ++;
}

int chercher_max_id() {
    int max = 0;
    for (int i= 0; i<= liste_jobs.N; i++) {
        if (liste_jobs.list[i].job_id > max) {
             max = liste_jobs.list[i].job_id ;
        }
    }
    return max;
}
/* supprimer un job de list_jobs à partir de son pid*/
void delete_job(pid_t pid) {
    int start = get_job_indice(pid);
    if (start != -1) {
        for (int i= start; i<= liste_jobs.N; i++) {
            liste_jobs.list[i] = liste_jobs.list[i + 1];
        }
        liste_jobs.N--;
        liste_jobs.max = chercher_max_id();
   }
}

/*Afficher un job*/
void print_job(job_t job){
    if (job.job_etat == bg){
        printf("[%d]      %d           Actif(bg)           %s \n" ,job.job_id ,job.job_pid, job.ligne_cmdl);
        fflush(stdout);
    } else if (job.job_etat == fg){
        printf("[%d]      %d           Actif(fg)           %s \n" ,job.job_id ,job.job_pid, job.ligne_cmdl);
        fflush(stdout);
    } else {
        printf("[%d]      %d           Suspendu            %s \n" ,job.job_id ,job.job_pid, job.ligne_cmdl);
        fflush(stdout);
    }
}

 /* Afficher la liste des processus suspendus ou en exécution
    ** en arrière plan. 
    ** (la commande interne 'list')
    */
void lister_jobs() {
    if (liste_jobs.N >= 1) {
        printf("Id         PID            État           ligne de commande lancée\n");

        for (int i =0; i< liste_jobs.N; i++) {
            print_job(liste_jobs.list[i]);
        }
    }
}

/* La commande interne stop du minishell */
void stop_processus ( char ** cmdl) {
    //La saisie de "stop   " uniquement.
    if (cmdl[1] == 0) {
          printf("\033[1;33m");
        printf(ANSI_COLOR_RED "Erreur : L'identifiant du processus est non saisi.\n");
    //La saisie d'un ID inexistant.
    } else if ((atoi(cmdl[1]) >liste_jobs.max) | (get_pid(atoi(cmdl[1])) == -1 )){

        printf( ANSI_COLOR_RED "stop : Processus non trouvé.\n");

    } else {
        int pid = get_pid(atoi(cmdl[1]));
        kill(pid, SIGSTOP);
        liste_jobs.list[get_job_indice(pid)].job_etat = suspendu;
    }
}

/* la commande 'bg' */
void background(char** cmdl) {

    //La saisie de "bg  ".
   if  (cmdl[1] == 0) {

        printf(ANSI_COLOR_RED  "Erreur : L'identifiant du processus est non saisi.\n");
    //La saisie d'un ID inexistant.
    } else if (get_pid(atoi(cmdl[1])) < 0){

        printf(ANSI_COLOR_RED "bg : Processus non trouvé.\n");

    } else {
        int pid = get_pid(atoi(cmdl[1]));
        /* traiter la reprise */
        liste_jobs.list[get_job_indice(pid)].job_etat = bg;
        kill(pid, SIGCONT);
    }
}

/*la commande 'fg' */
pid_t foreground(char** cmdl) {
    //La saisie de "fg  "
    if (cmdl[1] == 0) {

        printf(ANSI_COLOR_RED "Erreur : L'identifiant du processus est non saisi.\n");
    //La saisie d'un ID inexistant.
    } else if (get_pid(atoi(cmdl[1])) < 0){

        printf(ANSI_COLOR_RED "fg : Processus non trouvé.\n");

    } else {
        int pid = get_pid(atoi(cmdl[1]));
        int i = get_job_indice(pid);
        liste_jobs.list[i].job_etat = fg;
        
        //Afficher la première commande élémentaire de la ligne de commande
        kill(pid, SIGCONT);
        return pid;
    }
    return -1;
}

