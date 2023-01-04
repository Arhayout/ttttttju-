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
        int retour = fork(); //Le père attend qu'un fils quelconque finisse, on va donc en créer un pour le terminer 
	                   //immédiatement (sinon le père attendre pidCHILDFG alors qu'il sera suspendu)
        if (retour == 0)
        { //Nouveau Fils
            exit(EXIT_SUCCESS);
        }
        else if (retour > 0)
        { //Père
            kill(retour, SIGKILL);
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
* handler de SIGCHLD
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

                    //reprendre le processus mais en avant plan
                    //waitpid(PID, &etat_fils, WNOHANG | WUNTRACED | WCONTINUED);
                    //if ( WIFSTOPPED(etat_fils) || WIFSIGNALED(etat_fils)) {
                    kill(PID, SIGCONT);
                    waitpid(PID, 0, 0);
                    updateCommand(&processList, PID, Done);
                    //}
                    continue;
                }
                else
                {
                    printf("Ce processus n'existe pas!\n");
                    continue;
                }
            }
        }else{

        retour = fork();

         if (retour < 0){ /* échec du fork */
            printf("Erreur fork\n");
            /* Convention : s'arrêter avec une valeur > 0 en cas d'erreur */
            exit(1);
        }

        /* fils */
        if (retour == 0)
        {
            signal(SIGTSTP, SIG_IGN);    //Ignorer CTRL Z
            signal(SIGINT, SIG_IGN);     //Ignorer CTRL C
            signal(SIGCHLD, suivi_fils); //Attribuer la handler suivi_fils au signal SIGCHLD
     
          
            if (execvp(cmd->seq[0][0], cmd->seq[0]) < 0)
            {
                printf("%s: commande invalide!\n", cmd->seq[0][0]);
                exit(EXIT_FAILURE);
            }
            else
            {
                exit(EXIT_SUCCESS);  /* Terminaison normale (0 = sans erreur) */
            }
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
	}}
    }
   
    return 0;

}
