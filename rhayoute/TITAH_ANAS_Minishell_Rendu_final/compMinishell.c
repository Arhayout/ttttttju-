/** Les composantes de notre minishell
**  Les procédures et fonctions utilisées 
**  @author : Anas Titah
**  @version : 1.0 
*/

#include "compMinishell.h"
#include "cmdinternes.c"
#include "procMinishell.c"
#include "readcmd.c"

void afficher_prompt(){
    /* Récuperer le répertoire courant. */
    char cwd[256];
    getcwd(cwd,sizeof(cwd));
    /* Afficher un message d'attente avec le répertoire courant en
    ** attendant l'entrée d'une commande. */
    printf(CYANCLAIR "minishell@1.1:~%s$ " AUCUN, cwd);
    fflush(stdin);
}

int execution_cmd_interne(struct cmdline *c){
    if (!strcmp(c->seq[0][0],"exit")) {
        cmd_exit();
        return 1;
    } else if (!strcmp(c->seq[0][0],"cd")) {
        cmd_cd(c);
        return 1;
    } else if (!strcmp(c->seq[0][0],"list")){
        cmd_list(liste);
        return 1;
    } else if (!strcmp(c->seq[0][0],"stop")){
        cmd_stop(c,liste);
        return 1;
    } else if (!strcmp(c->seq[0][0],"bg") || !strcmp(c->seq[0][0],"fg")){
        int proc_attendre = cmd_bg_fg(c,liste);
        /* Si proc_attendre est différent de -1, on doit
        ** attendre le processus qui est en avant-plan jusqu'il
        ** terminer ou il reçoit un signal SIGINT ou SIGTSTP*/
        if (!(proc_attendre == -1)){
            processus_en_cours = proc_attendre;
            while((signal_ctrl_z == 0) && (signal_ctrl_c == 0)){                   
                sleep(0);
            }
            processus_en_cours = 0;
        }
        return 1;
    } else {
        /* Une commande externe, on renvoie 0*/
        return 0;
    }
}

void execution_cmd_simple(struct cmdline *c){
    /* Traiter les autres commandes. */
    pid_t pidFils = fork();

    /* bonne pratique : tester systématiquement le retour des appels système. */
    if (pidFils == -1) {
        printf("Erreur fork\n");
        exit(1);
        /* par convention, renvoyer une valeur > 0 en cas d'erreur,
        * différente pour chaque cause d'erreur.
        */
    }
    if (pidFils == 0) {  /* fils */
        if (!(c->backgrounded == NULL)){
            /*Masquer les signaux SIGINT et SIGTSTP.
            ** pour les commandes en arrière plan. */
            sigset_t signaux;
            sigemptyset(&signaux);
            sigaddset(&signaux,SIGINT);
            sigaddset(&signaux,SIGTSTP);
            sigprocmask(SIG_BLOCK,&signaux,NULL);
        }

        /* Gestion des redirections. */
        /* Rediriger l'entrée */
        if (!(c->in == NULL)){
            int desc_fic_read = open(c->in,O_RDONLY);
            if (desc_fic_read < 0){
                perror("Erreur d'ouverture");
                exit(EXIT_FAILURE);
            }
            dup2(desc_fic_read,0);  
            close(desc_fic_read);
        }
        /* Rediriger la sortie */
        if (!(c->out == NULL)){
            int desc_fic_write = open(c->out,O_WRONLY | O_TRUNC | O_CREAT, 0700);
            if (desc_fic_write < 0){
                perror("Erreur d'ouverture");
                exit(EXIT_FAILURE);
            }
            dup2(desc_fic_write,1);
            close(desc_fic_write);
        }

        /* Vérifier si la commande est valide ou non. */
        /* L'appel à execvp lance la commande et renvoie
        ** si la commande a été bien executée ou non, si 
        ** valeur envoyée est inférieur strictement à 0,
        ** alors une erreur est s'est produite et on rentre
        ** dans la condition (if) pour afficher un message
        ** d'erreur */
        if (execvp(c->seq[0][0],c->seq[0]) < 0){
            printf(ROUGE "Erreur : Commande invalide\n" AUCUN);
            exit(EXIT_FAILURE);
        }

    } else {  /* père */
        /* Ajouter le processus à la liste des processus
        ** non encore terminés. */
        infoproc *processus = malloc(512);
        processus->id_proc = ++n;
        processus->pid_proc = pidFils;
        strcpy(processus->etat,"actif");
        int i = 0;
        while (!(c->seq[0][i] == NULL)) {
            strcat(processus->commande,c->seq[0][i]);
            strcat(processus->commande," ");
            i++;
        }
        ajouterProc(liste,processus);
        /* Attendre le processus fils si la commande 
        ** n'est pas en tâche de fond. */
        if (c->backgrounded == NULL) {  
            int status;
            processus_en_cours = pidFils;
            waitpid(pidFils,&status,WUNTRACED);
            /* La commande est bien terminée à cette
            ** étape, on la supprime donc de la liste
            ** des processus en cours d'exécution ou
            ** suspendus. */
            if (!WIFSTOPPED(status)){
                supprimerProc(liste,pidFils);
            }
            processus_en_cours = 0;    
        }
    }
}

void execution_cmd_pipes(struct cmdline *c){
    /* Variables utilisées*/
    pid_t pidFils;
    int p[2];
    int entree = 0;
    int num_cmd = 0;


    while (!(c->seq[num_cmd] == NULL)) {
        /* Création du pipe */
        pipe(p);

        /* Traiter les autres commandes. */
        pidFils = fork();

        /* bonne pratique : tester systématiquement le retour des appels système. */
        if (pidFils == -1) {
            printf("Erreur fork\n");
            exit(1);
            /* par convention, renvoyer une valeur > 0 en cas d'erreur,
            * différente pour chaque cause d'erreur.
            */
        }
        if (pidFils == 0) {  /* fils */
            if (!(c->backgrounded == NULL)){
                /*Masquer les signaux SIGINT et SIGTSTP.
                ** pour les commandes en arrière plan. */
                sigset_t signaux;
                sigemptyset(&signaux);
                sigaddset(&signaux,SIGINT);
                sigaddset(&signaux,SIGTSTP);
                sigprocmask(SIG_BLOCK,&signaux,NULL);
            }
            /* Rediriger l'entree vers celle mémorisée
            ** dans la variable entree*/
            dup2(entree,0);

            /* Rediriger la sortie si elle existe une commande
            ** suivante*/
            if (!(c->seq[num_cmd + 1] == NULL)){
                dup2(p[1],1);
                close(p[0]);
                close(p[1]);
            }

            /* Gestion des redirections. */
            /* Rediriger l'entrée */
            if (!(c->in == NULL) && (num_cmd == 0)){
                int desc_fic_read = open(c->in,O_RDONLY);
                if (desc_fic_read < 0){
                    perror("Erreur d'ouverture");
                    exit(EXIT_FAILURE);
                }
                dup2(desc_fic_read,0);  
                close(desc_fic_read);
            }
            /* Rediriger la sortie */
            if (!(c->out == NULL) && (c->seq[num_cmd + 1] == 0)){
                int desc_fic_write = open(c->out,O_WRONLY | O_TRUNC | O_CREAT, 0700);
                if (desc_fic_write < 0){
                    perror("Erreur d'ouverture");
                    exit(EXIT_FAILURE);
                }
                dup2(desc_fic_write,1);
                close(desc_fic_write);
            }

            /* Vérifier si la commande est valide ou non. */
            /* L'appel à execvp lance la commande et renvoie
            ** si la commande a été bien executée ou non, si 
            ** valeur envoyée est inférieur strictement à 0,
            ** alors une erreur est s'est produite et on rentre
            ** dans la condition (if) pour afficher un message
            ** d'erreur */
            if (execvp(c->seq[num_cmd][0],c->seq[num_cmd]) < 0){
                printf(ROUGE "Erreur : Commande invalide\n" AUCUN);
                exit(EXIT_FAILURE);
            }

        } else {  /* père */
            /* Ajouter le processus à la liste des processus
            ** non encore terminés. */
            infoproc *processus = malloc(512);
            processus->id_proc = ++n;
            processus->pid_proc = pidFils;
            strcpy(processus->etat,"actif");
            int i = 0;
            while (!(c->seq[num_cmd][i] == NULL)) {
                strcat(processus->commande,c->seq[num_cmd][i]);
                strcat(processus->commande," ");
                i++;
            }
            ajouterProc(liste,processus);
            /* Attendre le processus fils si la commande 
            ** n'est pas en tâche de fond. */
            if (c->backgrounded == NULL) {  
                int status;
                processus_en_cours = pidFils;
                waitpid(pidFils,&status,WUNTRACED);
                /* La commande est bien terminée à cette
                ** étape, on la supprime donc de la liste
                ** des processus en cours d'exécution ou
                ** suspendus. */
                if (!WIFSTOPPED(status)){
                    supprimerProc(liste,pidFils);
                }
                processus_en_cours = 0;    
            }
            close(p[1]);
            /* Mémoriser la sortie de cette commande pour 
            ** l'entrée de la prochaine commande*/
            entree = p[0];

            num_cmd ++;
        }
    }
}