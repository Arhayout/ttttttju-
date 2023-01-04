/* Variables externes utilisées */
extern infoproc **liste;
extern int processus_en_cours;
extern int signal_ctrl_c;
extern int signal_ctrl_z;

/* Traitant des signaux*/
void traitant_signaux(int sig){
    switch(sig)
    {
        case SIGCHLD:
            traitement_SIGCHLD();
            break;
        case SIGTSTP:
            traitement_SIGTSTP();
            break;
        case SIGINT:
            traitement_SIGINT();
            break;
        default:
            /* Le cas des signaux non traités par le minishell */
            printf(ROUGE "Signal non traité par le minishell\n" AUCUN);
            EXIT_FAILURE;
    }
}

/* Traitement du signal SIGCHLD*/
void traitement_SIGCHLD() {
    int etat_fils;
    pid_t pid_fils;
    do {
        pid_fils = (int) waitpid(-1, &etat_fils, WNOHANG | WUNTRACED | WCONTINUED);
        if ((pid_fils == -1) && (errno != ECHILD)) {
            perror("waitpid");
            exit(EXIT_FAILURE);
        } else if (pid_fils > 0) {
            if (WIFSTOPPED(etat_fils)) {
                /* Traiter la suspension. */
                /* Changer l'état du processus dans la liste des 
                ** processus à l'état suspendu. */
                if (signal_ctrl_z == 0 ){
                    int indice = positionProc(liste,pid_fils);
                    strcpy(liste[indice]->etat ,"suspendu");
                }
            } else if (WIFCONTINUED(etat_fils)) {
                /* Traiter la reprise. */
                /* Changer l'état du processus dans la liste à des
                ** processus à l'état actif.  */
                int indice = positionProc(liste,pid_fils);
                strcpy(liste[indice]->etat ,"actif");
            } else if (WIFEXITED(etat_fils)) {
                /* Supprimer le processus de la liste des processus 
                non encore temrinées. */
                supprimerProc(liste,pid_fils);
                processus_en_cours = 0;
            }
        }
    } while (pid_fils > 0);
}

/* Traitement du signal SIGTSTP */
void traitement_SIGTSTP() {
    if (!(processus_en_cours == 0)) {
        kill(processus_en_cours,SIGSTOP);
        printf("\n");
        int indice = positionProc(liste,processus_en_cours);
        strcpy(liste[indice]->etat ,"suspendu");
        signal_ctrl_z = 1;
    } else {
        printf(ROUGE "\nPour quitter le minishell, utilisez la commande exit\n" AUCUN);
    }
    fflush(stdin);
}

/* Traitement du signal SIGINT */
void traitement_SIGINT() {
    if (!(processus_en_cours == 0)) {
        kill(processus_en_cours,SIGKILL); 
        printf("\n");
        supprimerProc(liste,processus_en_cours);
        signal_ctrl_c = 1;
    } else {
        printf(ROUGE "\nPour quitter le minishell, utilisez la commande exit\n" AUCUN);
    }
    fflush(stdin);
}
