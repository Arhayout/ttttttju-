#include <stdio.h>  /* entrées sorties */
#include <unistd.h> /* pimitives de base : fork, ...*/
#include <stdlib.h> /* exit */
#include <signal.h> /* traitement des signaux */
#include "readcmd.h" /* lecture des commandes */
#include <sys/wait.h> /* wait */
#include <string.h>   /* opérations sur les chaînes */
#include <errno.h>   /* errno */
#include <fcntl.h>  /* opérations sur les fichiers */
#include "processList.h" //Contient tout ce qui est relatif au stockage des processus, leur informations ainsi que leur affichage

pList processList; //La liste des eventuels processus
bool executionFG;  //Verifier si l'execution se fait en ForeGround FG
int pidCHILDFG;    //le PID du fils executant en FG
char *commandFG;   //Commande exécutée en FG

void suivi_pere(int sig)
{
    //Si l'execution se fait en FG et que CTRL Z est tapé
    if (executionFG && sig == SIGTSTP)
    {
        //executionFG = false;
        printf(" Suspension du processus\n");
        int fils = fork(); 
       	if (fils == 0)
        { //Nouveau Fils
            exit(EXIT_SUCCESS);
        }
        else if (fils > 0)
        { //Père
            kill(fils, SIGKILL);
            kill(pidCHILDFG, SIGSTOP); //Suspension du processus
        }
        else
        {
            /* échec du fork */
            printf("Erreur fork\n");
            /* Convention : s'arrêter avec une valeur > 0 en cas d'erreur */
            exit(1);
        }
    }
    //Si l'execution se fait en FG et que CTRL C est tapé
    else if (executionFG && sig == SIGINT)
    {
        printf(" Terminaison du processus\n");
        executionFG = false;
        kill(pidCHILDFG, SIGKILL);
        if (pExists(processList, pidCHILDFG))
        {
            popCommand(&processList, pidCHILDFG); //Suppression de processus
        }
    }
    //Si l'execution ne se fait pas en FG et que CTRL C est tapé
    else if (!executionFG && sig == SIGINT)
    {
        NULL; //Ne rien faire, puisque le miniShell est libre!
    }
}


/**
* handler de SIGCHLD, copié du tutoriel de Mr. Hamrouni
*/
void suivi_fils(int sig)
{

    int etat_fils, pid_fils;

    do
    {

        pid_fils = (int)waitpid(-1, &etat_fils, WNOHANG | WUNTRACED | WCONTINUED);

        if ((pid_fils == -1) && (errno != ECHILD))
        {

            perror("waitpid");

            exit(EXIT_FAILURE);
        }
        else if (pid_fils > 0)
        {

            if (WIFCONTINUED(etat_fils))
            {
                /* traiter la reprise */
                if (pExists(processList, pid_fils))
                {
                    updateCommand(&processList, pid_fils, actif);
                }
            }
            else if (WIFSTOPPED(etat_fils) || WIFSIGNALED(etat_fils))
            {
                /* trailer l'arrêt */
                if (pExists(processList, pid_fils))
                {
                    updateCommand(&processList, pid_fils, suspendu);
                }
            }
            else if (WIFEXITED(etat_fils))
            {
                /* traiter l'arrêt normal */
                if (pExists(processList, pid_fils))
                {
                    updateCommand(&processList, pid_fils, Done);
                }
            }
        }

    } while (pid_fils > 0);

    /* autres actions après le suivi des changements d'état */
}

int main()
{
    char s[200]; // répertoire courrant
    int retour;
    struct cmdline *cmd; //ligne de commande

    signal(SIGTSTP, suivi_pere); //handler CTRL Z
    signal(SIGINT, suivi_pere); // handle CTRL C

    initializeList(&processList); //initialisation de la liste de processus
    signal(SIGCHLD, suivi_fils); //handler SIGCHLD
    int desc_ent, desc_res, dupdesc; /* descripteurs de fichiers */
    

    while (1)
    {
        //MODIFICATION QUESTION 2
        printf("\033[0;32m"); //VERT
        printf("houkhnin");
        printf("\033[0m"); //DEFAUT
        printf(":");
        printf("\033[0;34m");         //BLEU
        printf("%s", getcwd(s, 200)); //Repertoire de travail
        printf("\033[0m");            //DEFAUT
        printf("$ ");
        //FIN QUESTION 2
        cmd = readcmd();

        //MODIFICATION QUESTION 4 ET 6
	
	if( cmd ==NULL){
  		continue; //erreur inconnue dans readcmd()
         }
        if(cmd->err != NULL){
               perror("cmdline: "); // readcmd() renvoie un code erreur
                continue;
         }
         if(cmd->seq[0] == NULL){
           continue; // on a donné une chaine vide a readcmd()
                }

        if (strcmp(cmd->seq[0][0], "cd") == 0)
        {
            chdir(cmd->seq[0][1]); //changer de répertoire
            continue;
        }
        else if (strcmp(cmd->seq[0][0], "exit") == 0)
        {
            exit(0);
        }
        else if (strcmp(cmd->seq[0][0], "jobs") == 0)
        {
            jobs(&processList); //afficher la liste de processus
            continue;
        }
        else if (strcmp(cmd->seq[0][0], "stop") == 0)
        {
            if (cmd->seq[0][1] == NULL)
            {
                printf("Il faut préciser le numéro du processus!\n");
                continue;
            }
            else
            {
                char *strPID = malloc(sizeof(cmd->seq[0][1]));
                strcpy(strPID, cmd->seq[0][1]);
                int PID;
                sscanf(strPID, "%d", &PID); //récuper le PID integer

                if (pExists(processList, PID))
                {
                    //stop le processus
                    kill(PID, SIGSTOP);
                    continue;
                }
                else
                {
                    printf("Ce processus n'existe pas!\n");
                    continue;
                }
            }
        }
        else if (strcmp(cmd->seq[0][0], "bg") == 0)
        {
            if (cmd->seq[0][1] == NULL)
            {
                printf("Il faut préciser le numéro du processus!\n");
                continue;
            }
            else
            {
                char *strPID = malloc(sizeof(cmd->seq[0][1])); //pid String
                strcpy(strPID, cmd->seq[0][1]);
                int PID;
                sscanf(strPID, "%d", &PID); //conversion du pid String en pid int

                if (pExists(processList, PID))
                {

                    //reprendre le processus mais en arrière plan
                    kill(PID, SIGCONT);
                    continue;
                }
                else
                {
                    printf("Ce processus n'existe pas!\n");
                    continue;
                }
            }
        }
        else if (strcmp(cmd->seq[0][0], "fg") == 0)
        {
            if (cmd->seq[0][1] == NULL)
            {
                printf("Il faut préciser le numéro du processus!\n");
                continue;
            }
            else
            {
                char *strPID = malloc(sizeof(cmd->seq[0][1]));
                strcpy(strPID, cmd->seq[0][1]);
                int PID;
                sscanf(strPID, "%d", &PID);

                if (pExists(processList, PID))
                {

                    kill(PID, SIGCONT);
                    waitpid(PID, 0, 0);
                    updateCommand(&processList, PID, Done);
                    
                    continue;
                }
                else
                {
                    printf("Ce processus n'existe pas!\n");
                    continue;
                }
            }
        }

        

        retour = fork();

        /* Bonne pratique : tester systématiquement le retour des appels système */
        if (retour < 0)
        { /* échec du fork */
            printf("Erreur fork\n");
            /* Convention : s'arrêter avec une valeur > 0 en cas d'erreur */
            exit(1);
        }

        /* fils */
        if (retour == 0)
        {
            signal(SIGTSTP, SIG_IGN);    //Ignorer CTRL+Z
            signal(SIGINT, SIG_IGN);     //Ignorer CTRL+C
            signal(SIGCHLD, suivi_fils); //Attribuer la handler suivi_fils au signal SIGCHLD

            //S'il y a redirection de stdin ou stdout
            if (cmd->in != NULL || cmd->out != NULL)
            {
                if (cmd->in != NULL && cmd->err == NULL) //S'il y a redirection de stdin vers un fichier
                {
                    /*ouvrir le fichier des entrées */
                    desc_ent = open(cmd->in, O_RDONLY);
                    /* traiter systématiquement les retours d'erreur des appels */
                    if (desc_ent < 0)
                    {
                        fprintf(stderr, "Erreur ouverture %s\n", cmd->in);
                        exit(1);
                    }
                    //rediriger stdin vers le fichier de desc_ent
                    dupdesc = dup2(desc_ent, 0);
                    if (dupdesc == -1)
                    {
                        /* échec du dup2 */
                        perror("Erreur dup2\n");
                        exit(5);
                    }
                }
                if (cmd->out != NULL && cmd->err == NULL) //S'il y a rediréction de stdout vers un fichier
                {

                    /* ouvrir le fichier résultats */
                    desc_res = open(cmd->out, O_WRONLY | O_CREAT | O_TRUNC, 0640);
                    /* traiter systématiquement les retours d'erreur des appels */
                    if (desc_res < 0)
                    {
                        fprintf(stderr, "Erreur ouverture %s\n", cmd->out);
                        exit(2);
                    }
                    //rediriger stdout vers le fichier de desc_res
                    dupdesc = dup2(desc_res, 1);
                    if (dupdesc == -1)
                    {
                        /* échec du dup2 */
                        perror("Erreur dup2\n");
                        exit(5);
                    }
                }
            }

            //S'il y a au moins un pipe
            if (cmd->seq[1] != NULL)
            {

                int nbr_pipes = 0; //nombre de pipes

                while (cmd->seq[nbr_pipes + 1] != NULL)
                {
                    nbr_pipes++;
                }

                int pipes[nbr_pipes*2]; //pipes

                for (int j = 0; j < nbr_pipes; j++)
                {
                    int retour_pipe = pipe(pipes + j*2); //création de pipes
                    if (retour_pipe == -1)
                    {
                        perror("Erreur pipe\n");
                        exit(1);
                    }
                }
                int i = 0;
                int cmd_nbr = 0; //numéro de la commande à éxecuter

                while(cmd_nbr < nbr_pipes +1)
                {
                    int fils = fork();
                    if (fils == -1)
                    {
                        //erreur fork
                        perror("Erreur fork\n");
                        exit(1);
                    }
                    else if (fils == 0)
                    {
                        //fils
                        if (cmd_nbr > 0) //si ce n'est pas la première commande
                        {
                            //rediriger stdin vers le résultat de la commande précédente
                            dupdesc = dup2(pipes[i - 2], 0);
                            if (dupdesc == -1)
                            {
                                perror("Erreur dup2 Lecture");
                                fprintf(stderr, "iteration %d\n", cmd_nbr);
                                exit(1);
                            }
                        }

                        if (cmd_nbr != nbr_pipes) //si ce n'est pas la dernière commande
                        {
                            //rediriger stdout vers l'entrée de la commande suivante
                            dupdesc = dup2(pipes[i +1], 1);
                            if (dupdesc == -1)
                            {
                                perror("Erreur dup2 Ecriture");
                                fprintf(stderr, "iteration %d\n", cmd_nbr);
                                exit(1);
                            }
                        }

                        //fermer les pipes inutiles
                        for (int k = 0; k < 2*nbr_pipes; k++)
                        {
                            close(pipes[k]);
                        }

                        //execution de la commande actuelle
                        if (execvp(cmd->seq[cmd_nbr][0], cmd->seq[cmd_nbr]) < 0)
                        {
                            printf("%s: Unknown Command!\n", cmd->seq[cmd_nbr][0]);
                            exit(EXIT_FAILURE);
                        }
                        else
                        {
                            exit(EXIT_SUCCESS);
                        }
                    }
                    else
                    {
                        //père
                        cmd_nbr++; //pour exécuter la commande suivante
                        i+=2; //pour lire et écrire dans les bons pipes
                    }
                }
                //fermer les pipes puisque la commande a été exécutée correctement
                for (int k = 0; k < 2*nbr_pipes; k++)
                {
                    close(pipes[k]);
                }

                //attendre que tous les fils finissent
                int status;
                for(i = 0; i < nbr_pipes + 1; i++)
                {
                    wait(&status);
                }
                exit(EXIT_SUCCESS); //revenir au minishell!
            }
            //Execution de la commande (n'arrive jamais ici dans le cas d'un pipe)
            if (execvp(cmd->seq[0][0], cmd->seq[0]) < 0)
            {
                printf("%s: Unknown Command!\n", cmd->seq[0][0]);
                exit(EXIT_FAILURE);
            }
            else
            {
                exit(EXIT_SUCCESS);  /* Terminaison normale (0 = sans erreur) */
            }
            //execvp(cmd->seq[0][0], cmd->seq[0]); //Executer la commande
            //exit(EXIT_SUCCESS);                  /* Terminaison normale (0 = sans erreur) */
        }

        /* pere */
        else
        {
            if (cmd->backgrounded == NULL)
            {
                executionFG = true;
                pidCHILDFG = retour;
                char *commandFG = malloc(sizeof(cmd->seq[0][0]));
                strcpy(commandFG, cmd->seq[0][0]);
                addCommand(&processList, retour, commandFG);
                updateCommand(&processList, retour, Done);
                wait(NULL);
            }
            else
            {

                executionFG = false;
                char *newCommand = malloc(sizeof(cmd->seq[0][0]));
                strcpy(newCommand, cmd->seq[0][0]);
                addCommand(&processList, retour, newCommand); //Ajout de la commande à liste de processus
            }
        }
    }
    return EXIT_SUCCESS;
}
