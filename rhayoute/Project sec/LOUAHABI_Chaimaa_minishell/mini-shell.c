
/*redirection de la sortie standard*/
void rediriger_sortie(char* fichier) {
    int desc_fich_out, dup_desc;
    desc_fich_out = open (fichier, O_WRONLY| O_CREAT | O_TRUNC, 0640);

    if (desc_fich_out < 0) {
        perror(fichier);
        exit(1);
    }

    dup_desc = dup2(desc_fich_out, 1);

    if (dup_desc == -1) {
        printf("Erreur dup2 \n");
        exit(1);
    }
}

/*redirection de l'entrée standard*/
void rediriger_entree(char* fichier) {
    int desc_fich_in, dup_desc;
    desc_fich_in = open (fichier, O_RDONLY);
    if (desc_fich_in < 0) {
        perror(fichier);
        exit(1);
    }
    dup_desc = dup2(desc_fich_in, 0);
    if (dup_desc == -1) {
        printf("Erreur dup2 \n");
        exit(1);
    }
}

/*******************************MAIN *******************************************/

/*******************************************************************************/

int main() {
    int wstatus, wstatus1, dup_desc, desc_fich_in, desc_fich_out, pid , pid2;
    liste_jobs.N = 0;
    //struct cmdline *ligneCommande;

    signal(SIGCHLD, suivi_fils);
    signal(SIGINT, sigint_handler);
    signal(SIGTSTP, SIG_IGN);

	while(1) {
        printf("mini-shell > ");
		fflush(stdout);
        struct cmdline *ligneCommande;
            /* Création d'un processus fils.*/
            else if (ligneCommande->seq[1] == NULL){
                pid_t pid_fils = fork();

		        if (pid_fils == -1) {
                    printf("Erreur fork\n");
                    exit(1);
                }
                if (pid_fils == 0){
                    //fils 
                    if (ligneCommande->in != NULL) {
                        /*redirection de l'entrée standard*/
                        rediriger_entree(ligneCommande->in);
                    }
                    if (ligneCommande->out != NULL) {
    
                        /*redirection de la sortie standard*/
                        rediriger_sortie(ligneCommande->out);
                    }
                    
                    int retour = execvp(ligneCommande->seq[0][0], ligneCommande->seq[0]);
                    if (retour == -1) {
                        
                        perror("Erreur ");
                    }
                    exit(1);
                } else {
                    
                    /* BG ou FG ?*/
                    if (ligneCommande->backgrounded == NULL){
                        //FG
                        /* Enregistrer le processus fils dans liste_jobs. */
                        add_job(pid_fils, *ligneCommande->seq[0], FG);
                        pid_fg = pid_fils;
                        wait(&wstatus1);
                        delete_job(pid_fils);
                        
                    } else {
                        /* Enregistrer le processus fils dans liste_jobs. */
                        add_job(pid_fils, *ligneCommande->seq[0], BG);
                        /*afficher l'ID et PID du processus en BG*/
                        printf("[%d]       %d   \n" ,liste_jobs.list[liste_jobs.N - 1].job_id ,liste_jobs.list[liste_jobs.N - 1].job_pid);
                    }
	            }
            }   
            /*le cas des tubes */ 
            else {
                /*calcul nbr tubes */
                int nbr_tubes = 1;
                while( ligneCommande->seq[nbr_tubes] != NULL) {
                    nbr_tubes++;
                }
                nbr_tubes--;
                int wstatus, pid;
                int pipes[2*nbr_tubes];
                for (int i = 0; i< nbr_tubes; i++) {
                    if (pipe(pipes + i*2) < 0) {
                        perror("erreur pipe");
                        exit(1);
                    }
                }
                int j = 0;
                int k = 0;// l'indice de la commande
                while (ligneCommande->seq[k] != NULL) {
                    pid = fork();
                    if (pid < 0) {
                        perror("erreur fork");
                        exit(1);
                    }
                    else if (pid == 0) {
                        //les redirections < et > pour la première et dernière commande
                        if (k == 0) {
                            if (ligneCommande->in != NULL) {
                                /*redirection de l'entrée standard*/
                                rediriger_entree(ligneCommande->in);
                            }
                        }
                        if ( k == nbr_tubes ) {
                            if (ligneCommande->out != NULL) {
    
                                /*redirection de la sortie standard*/
                                rediriger_sortie(ligneCommande->out);
                            }
                        }
                        //si ce n'est pas la peremiere commande , 
                        //rediriger l'entrée standard vers le sortie du dernier tube pipes[j-2]
                        if (j != 0) {
                            if (dup2(pipes[j-2], 0) < 0 ) {
                                perror("erreur dup2");
                                exit(1);
                            }
                        }
                        //si ce n'est pas la derniére commande, 
                        //rediriger la sortie standard vers l'entrée pipes[j+]
                        if (ligneCommande->seq[k+1] != NULL) {
                            if (dup2(pipes[j+1], 1) < 0) {
                                perror("erreur dup2");
                                exit(1);
                            }
                        }
                        //fermer les tubes après les redirections
                        for (int i =0; i < 2*nbr_tubes; i++) {
                            close(pipes[i]);
                        }
                        if (execvp(ligneCommande->seq[k][0], ligneCommande->seq[k]) < 0) {
                            perror("erreur exec");
                            exit(1);
                        }
                    }
                    k++; j+=2;
                }
                //le père ferme tous les tubes et attend ses fils
                for (int i =0; i < 2*nbr_tubes; i++) {
                    close(pipes[i]);
                }
                for (int i =0; i < nbr_tubes +1; i++) {
                    wait(&wstatus);
                }
        }
    }
}
}

