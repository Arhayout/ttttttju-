/**Implanter en langage c, un interreteur de commande Minishell.*/

#include <unistd.h>  /*On importe les bibiothéques qui seront utilisées pour */ 
#include <string.h>    /* la réalisation de ce minishell*/
#include <stdbool.h>
#include <stdlib.h>
#include <sys/wait.h> 
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "readcmd.h" /*Ce module est utile pour manipuler une ligne de commande*/
#include "liste.h" /* Ce module est utile pour manipuler la liste des commandes*/

#define taille 500 /* La taille maximale pour D'UNE LISTE.*/

#define GREEN  "\033[00;32m"   /* Couleur verte */
#define NONE "\033[00m"        /* On rendre la couleur blanche. */

/* Liste des processus sera utilisable dans les parties du projet.*/
caractProcessus *list;
/*nombre de processus.*/
int n=1;
/*processus courant.*/
pid_t processus_courant=0;
/*ctrl+z, c'est-à-dire*/
bool ctrl_z=false;
/*ctrl+c.*/
bool ctrl_c=false;

           /*le Traitant de SIGINT.*/
            void handler_sigint(int signal) {
                        if (processus_courant != 0) {
                            kill(processus_courant,SIGKILL); 
                            deleteProcessus(list,processus_courant);
                            ctrl_c = true;
                        } else {
                            printf( "\nPour quitter utilisez 'exit' sinon tapez"); 
                            printf( " la commande que vous voulez faire.\n" );
                        }
            }

            /*le Traitant de SIGSTOP.*/
              void handler_sigstop(int signal){
                  if (processus_courant != 0) {
                        kill(processus_courant,SIGSTOP);
                        int indice = positionProcessus(list,processus_courant);
                        strcpy(list[indice]->state ,"SUSPENDU");
                        ctrl_z = true;
                   } else {
                        printf( "\nPour quitter utilisez 'exit' sinon tapez"); 
                        printf( " la commande que vous voulez faire.\n" );
                  }
               }

            /*le Traitant de SIGCHLD.*/
            void handler_sigchld(int signal){
                int wstatus;
                pid_t pid_child;
                    do {
                        pid_child = (int) waitpid(-1, &wstatus, WNOHANG | WUNTRACED | WCONTINUED);
                        if ((pid_child == -1) && (errno != ECHILD)) {
                            perror("waitpid");
                            exit(-1);
                        } else if (pid_child > 0) {
                            if (WIFSTOPPED(wstatus)) {    
                                if (!(ctrl_z ) ){
                                    int i = positionProcessus(list,pid_child);
                                    strcpy(list[i]->state ,"SUSPENDU");
                                }
                            } else if (WIFCONTINUED(wstatus)) {  
                                /*Reprise de processus.*/   
                                int i= positionProcessus(list,pid_child);
                                strcpy(list[i]->state ,"ACTIF");
                            } else if (WIFEXITED(wstatus) || WIFSIGNALED(wstatus)) {
                                /*le processus à été arreté.*/ 
                                deleteProcessus(list,pid_child);
                                n=n-1;
                                processus_courant= 0;
                            }
                        }
                    } while (pid_child > 0);
             }

	    /*la commande fg*/
           void fg(struct cmdline *commande){
                    if (!(commande->seq[0][1] == NULL)){
                        pid_t PID = atoi(commande->seq[0][1]);
                        if (exiteProcessus(list,PID)) { 
			     ctrl_z=false;
		             ctrl_c=false;
                            /* Envoyer le signal Reprise au processus demandée. */
                            kill(PID,SIGCONT);
                            printf("On reprend le processus de PId  %d en avant plan .\n" ,
                                 PID);
                                processus_courant = PID;
				        /*On attend jusqu'à la fin de processus en avant plan, et 
				         ce traitement continue, tant qu'on a pas tapé ctrl_c
			                 ou ctrl_z.*/
                                while((processus_courant==PID) && !(ctrl_z) && !(ctrl_c)){                   
                                    sleep(0);
                                }  
                        } else {
                            printf( "Le processus de  PID %d est introuvable. \n" ,PID);
                        }
                    } else {
                        printf( "Vous devez entrer le PID de processus\n" );
                    }
                    processus_courant = 0;

            }

		/*la commande bg*/
             void bg(struct cmdline *commande){
                    if (!(commande->seq[0][1] == NULL)){
                        pid_t PID = atoi(commande->seq[0][1]);
                        if (exiteProcessus(list,PID)) { 
                            /* Envoyer le signal Reprise au processus demandée. */
                            kill(PID,SIGCONT);   
                            printf("On reprend le processus de PId  %d en arrière plan .\n" ,
                                 PID);
     		         } else {
                            printf( "Le processus de  PID %d est introuvable. \n" ,PID);
                        }
                    } else {
                        printf( "Vous devez entrer le PID de processus\n" );
                    }
                    processus_courant = 0;

               }
  
int main(int argc, char *argv[]) {

    /* Variables utilisées*/ 
    pid_t pidChld;
    extern caractProcessus *list;
    list =  malloc(taille);

    
    /*Boucle infinie gere les commandes.*/
    while (true){
        /* Déclarer la ligne de commande.*/
        struct cmdline *commande;
        /* Descripteur de pipe.*/
        int pipe1[2],pipe2[2];
        /* Récuperer le répertoire courant. */
        char cwd[150];
        getcwd(cwd,sizeof(cwd));
        /*l'afficher.*/
        printf(GREEN "~%s Monminishell$ " NONE, cwd);
        /* Traitant les signaux SIGCHLD, SIGTSTP et SIGINT*/
        signal(SIGCHLD,handler_sigchld);
        signal(SIGTSTP,handler_sigstop);
        signal(SIGINT,handler_sigint);

        /* Lire la commande. */
        commande = readcmd();

        /* On reste sur le minishell malgré si on clique sur entrer.*/
        if (!(commande->seq[0] == NULL)){
            
              /* Traiter la commande exit.*/
            if (!strcmp(commande->seq[0][0],"exit")) {
                      exit(0);  
            } 
              /*Traiter la commande cd. On change le rep courant.*/
            else if (!strcmp(commande->seq[0][0],"cd")) {
                if((commande->seq)[0][1]==NULL || strcmp((commande->seq)[0][0], "~")==0) {
                    chdir("/");
                } else {
                    chdir(commande->seq[0][1]);
                }

            } 
               /*Traiter la commande list ou jobs. On affiche la liste de processus.*/
            else if (!strcmp(commande->seq[0][0],"list") || !strcmp(commande->seq[0][0],"jobs")){
                   showListProcessus(list);
            } 
               /*Si on tape la commande stop ou on fait ctrl_z.*/
            else if (!strcmp(commande->seq[0][0],"stop")) {
                /*On suspendre le processus à partir de son PID*/
                if (!(commande->seq[0][1] == NULL)){
                    pid_t PID = atoi(commande->seq[0][1]);
                    if (exiteProcessus(list,PID)){
                        /* Envoyer le signal Stop au processus. */ 
                        kill(PID,SIGSTOP); 
                        printf( "le processus de Pid %d a été suspendu\n" ,PID);
                    } else {
                        printf( "Le processus de  PID %d est introuvable. \n" ,atoi(commande->seq[0][1]));
                    }
                } else {
                        printf( "Vous devez entrer le PID de processus\n" );
                }
                processus_courant = 0;
            }  

            /* On reprend le signal suspendu en arrière plan.*/
            else if (!strcmp(commande->seq[0][0],"bg")){
                        bg(commande);}
                
            else if (!strcmp(commande->seq[0][0],"fg")){
                fg(commande);

            } 
            else {
                /* Traiter les autres commandes. */
                pidChld = fork();

                /* bonne pratique : tester systématiquement le retour des appels système. */
                if (pidChld == -1) {
                    printf("Erreur fork\n");
                    exit(1);
                }
                if (pidChld == 0) {  /* fils  */ 
  
                  /*Q10.Pipelines Traitant le cas de 3 commandes Dans ce cas on va 
                   utiliser un petit fils et son petit fils chacun va exécuter 
                    une commande, et le fils va exécuter la dernière.*/         
                      
                     if (commande->seq[2]!=NULL ) {
                         int fils1,fils2,retour1, retour2,dp1,dp2,dp3 ;
                         
                          retour1 = pipe(pipe2) ;
                          /* Bonne pratique : tester systématiquement le retour des appels système */
                          if (retour1 == -1) {   /* échec du pipe_wc */
                             printf("Erreur pipe\n") ;
                              /* Convention : s'arrêter avec une valeur > 0 en cas d'erreur */
                              exit(1) ;
                          }   
			  /*On définit le petit fils 1.*/        
                          fils1 = fork() ;            
                         /* Bonne pratique : tester systématiquement le retour des appels système */
                          if (fils1 < 0) {   /* échec du fork */
                             printf("Erreur fork\n") ;
                             /* Convention : s'arrêter avec une valeur > 0 en cas d'erreur */
                              exit(1) ;
                          }
                         /* fils */
                          if (fils1 == 0) {
                              /* fermer l'extrémité 0 : le fils va écrire dans le pipe */
                              close(pipe2[0]) ;
                               dup2(pipe2[1],1);
                                close(pipe2[1]);
                              retour2 = pipe(pipe1);

                                if (retour2 == -1) {   /* échec du pipe1 */
                                     printf("Erreur pipe\n") ;
                                      /* Convention : s'arrêter avec une valeur > 0 en cas d'erreur */
                                      exit(1) ;
                                  }
			            /*On définit le petit fils 2.*/
                                    fils2 =fork();
                                    if (fils2 < 0) {   /* échec du fork */
                                         printf("Erreur fork\n") ;
                                         /* Convention : s'arrêter avec une valeur > 0 en cas d'erreur */
                                          exit(1) ;
                                      }
                                    if(fils2==0){
                                        close(pipe1[0]);
                                        dp1=dup2(pipe1[1],1);
                                        if(dp1==-1){printf("Erreur dup2");}
                                        close(pipe1[1]);
                                        execlp(commande->seq[0][0],commande->seq[0][0],NULL);
                                        exit(1);
                                     } else{
                                       close(pipe1[1]);
                                        dp2=dup2(pipe1[0],0);
                                        if(dp2==-1){printf("Erreur dup2");}
                                        close(pipe1[1]);
                                        execlp(commande->seq[1][0],commande->seq[1][0],commande->seq[1][1],NULL);
                                        exit(2);
                                    }
                                }
                              else{
                                    close(pipe2[1]);
                                    dp3 = dup2(pipe2[0],0);
                                    if(dp3==-1){printf("Erreur dup3");}
                                    close(pipe2[0]);
                                    execlp(commande->seq[2][0],commande->seq[2][0],commande->seq[2][1], NULL);
                                    exit(3);            
                               }             
                   }
                                      
				
                  /*Q9. tupes simples, exécution de deux commande.*/
                  if (commande->seq[1]!= NULL) {
                      /*On definit un petit fils qui va exécuter la 1ère commande
                              et le fils va exécuter la 2ème commande.*/
                      int retour,dp1,dp2;
                       /*On cree le pipe.*/
                      retour = pipe(pipe1) ;
                      if (retour == -1) {
                          printf("Erreur pipe\n") ;
                          exit(1) ;
                      }
                       /*On definit le petit fils.*/
                      int p_fils;
                      p_fils = fork() ;
                      if (p_fils < 0) { /*Check le bon fonctionnement.*/
                          printf("Erreur fork\n") ;
                          exit(1) ;
                      }
                      if (p_fils == 0) {
                        close(pipe1[0]);
                        dp1=dup2(pipe1[1],1);
                        if(dp1==-1){
                        printf("Erreur dup2");
                        }
                        close(pipe1[1]);
                        execlp((commande->seq)[0][0],(commande->seq)[0][0],NULL) ;     
                      } else {
                            close(pipe1[1]) ;
                            dp2 = dup2(pipe1[0], 0) ;
                            if (dp2 == -1) { 
                               printf("Erreur dup2\n") ;
                               exit(1) ;
                            }
                            close(pipe1[0]);
                            execlp((commande->seq)[1][0],(commande->seq)[1][0], (commande->seq)[1][1], NULL) ;
                            perror((commande->seq)[1][0]) ;
                            exit(1) ;
                      }
                }
                                
                /*On masque les signaux sigint et sigstp pour les 
                      processus on arriere plan. */
                     if (!(commande->backgrounded == NULL)){
                        sigset_t ens;
                        sigemptyset(&ens);
                        sigaddset(&ens,SIGINT);
                        sigaddset(&ens,SIGTSTP);
                        sigprocmask(SIG_BLOCK,&ens,NULL);
                     }
                //lancement de la commande sans entrée ou sortie standard
		        if (commande->in  ==NULL && commande->out ==NULL){
                            if (execvp(commande->seq[0][0],commande->seq[0]) ==-1){
                                printf("La commande saisi est invalide.\n" );
                                exit(1);
                            }
		        }
                //lancement de la commande avec une entrée et une sortie standard
                else if (commande->in !=NULL && commande->out != NULL) {
                   int src = open(commande->in,O_RDONLY);
                   int des = open(commande->out, O_WRONLY | O_CREAT, 0644);
                   dup2(des,1);
                   close(des);
                   dup2(src,0);
                   close(src);
                   execlp(commande->seq[0][0],commande->seq[0][0],commande->seq[0][1],NULL);
               }
                //lancement de la commande avec une sortie standard et sans entree standard
                 else if (commande->in ==NULL && commande->out != NULL) {
                       int des = open(commande->out, O_WRONLY | O_CREAT, 0644);
                       dup2(des,1);
                       close(des);
                       execlp(commande->seq[0][0],commande->seq[0][0],commande->seq[0][1],NULL);
                }
                //lancement de la commande avec une entrée standard et sans sortie standard
                   else if (commande->in !=NULL && commande->out == NULL) {
                       int src = open(commande->in,O_RDONLY);
                       dup2(src,0);
                       close(src);
                       execlp(commande->seq[0][0],commande->seq[0][0],commande->in,NULL);
                        exit(0);
                }      
        
                } else {  /* père */
                    /* Ajouter le processus qui est en cours de l'éxécution.*/
                    char command[200] ="  " ;
                    int i = 0;
                    while (!(commande->seq[0][i] == NULL)) {
                        strcat(command,commande->seq[0][i]);
                        strcat(command," ");
                        i++;
                    }
                     addProcessus(list,n,pidChld,command);
                     n=n+1;
                    /* le père attendre la terminison de ses fils si la commande
                      n'est pas en arrière plan.*/
                    if (commande->backgrounded == NULL) {  
                        int wstatus;
                        processus_courant = pidChld;
                        waitpid(pidChld,&wstatus,WUNTRACED);
                        /*On supprime le processus car on a terminé son traitement.*/
                        if (!WIFSTOPPED(wstatus)) {
                               deleteProcessus(list,pidChld);
                               n=n-1;
                        }
                        processus_courant= 0; 
                    }
                }
            }
        }
    }
}
