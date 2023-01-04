#include <stdio.h>    /* printf */
#include <stdlib.h>   /* EXIT_SUCCESS */
#include  <unistd.h> /* fork */
#include  <fcntl.h> /* les fichiers */
#include <sys/wait.h> /* wait */
#include "readcmd.c"
#define max_path 500

int taille=0;

// descriptif d'un processus
struct process {
	int id;
	int pid;
	int actif; //=1 si actif 0 sinon
	char *ligneCommande;
	int backgrounded;
	int fini;
};

// tableau de processus
struct process *tabProcess; 
typedef struct process process;


int quit=0;
char *ref;

// l'id du processus en avant-plan
int forgroundedId=-1; 

int executer(struct cmdline* line){
	int outDescr=1;
	int inDescr=0;
	if(line->in!=NULL){
		int inFile=open(line->in,O_RDONLY,NULL);
		inDescr=dup2(inFile,inDescr);
		close(inFile);
	}
	if(line->out!=NULL){
		int outFile=open(line->out,O_WRONLY|O_CREAT,0640);
		outDescr=dup2(outFile,outDescr);
		close(outFile);
	}
	int r=execvp(line->seq[0][0],line->seq[0]);
	return r;
}

//traitant du signal CTRL+C (il ne fait pas grand chose pour le moment mais arrête le processus quand même)
void ctrlC (int sig) {
printf("\n");
	if(forgroundedId>-1){
		kill(tabProcess[forgroundedId].pid,SIGKILL);
		forgroundedId=-1;
	}
}

//traitant du signal CTRL+Z
void ctrlZ (int sig) {
printf("\n");
	if(forgroundedId>-1){
		kill(tabProcess[forgroundedId].pid,SIGSTOP);
		forgroundedId=-1;
	}
}


//Retourne le pid du processus à partir de son id 
int getPid(int pid){

	for(int j=0;j<taille;j++){
		if (tabProcess[j].pid==pid){
			return j;
		}					
	}
	return -1;
}


//Traitant du signal SIGCHLD
void suivi_fils (int sig) {

    int etat_fils, pid_fils;
	
    do { 
        pid_fils = (int) waitpid(-1, &etat_fils, WNOHANG | WUNTRACED | WCONTINUED);
	
        if ((pid_fils == -1) && (errno != ECHILD)) {

            perror("waitpid");

            exit(EXIT_FAILURE);

        } else if (pid_fils > 0) {
		int id =getPid(pid_fils);
            if (WIFSTOPPED(etat_fils)) {
		tabProcess[id].actif=0; // on indique qu'il n'est plus actif
		if(forgroundedId>-1){
			forgroundedId=-1; //Et pas d'autre processus en avant-plan
		}
                printf("\n[%d][%d] est stopé \n",id,pid_fils);

            } else if (WIFCONTINUED(etat_fils)) {

		tabProcess[id].actif=1;
                printf("\n[%d][%d] a repris \n",id,pid_fils);

            } else if (WIFEXITED(etat_fils)) {
		
		tabProcess[id].actif=0;
		tabProcess[id].fini=1;// on indique qu'il est fini
		if(forgroundedId>-1){
			forgroundedId=-1; //Et pas d'autre processus en avant-plan
		}
		printf("\n[%d][%d] est fermé \n",id,pid_fils);


            } else if (WIFSIGNALED(etat_fils)) {
            		//Si le signal est SIGINT on indique dans la description du processus qu'il est interrompu
            		//printf("\n[%d][%d] a été signalé \n",id,pid_fils);
			if (WTERMSIG(etat_fils)==SIGKILL){ 
				tabProcess[id].actif=0;// on indique qu'il est fini
				tabProcess[id].fini=1;// et n'est plus actif
				if(forgroundedId>-1){
					forgroundedId=-1; //Et pas d'autre processus en avant-plan
				}
				printf("\n[%d][%d] a été interrompu \n",id,pid_fils);
			}
		

            }
        }
    } while (pid_fils > 0);

}



// Convertir une liste de paramètre de commande en une chaîne de caractères
char *toString(struct cmdline *l){

	int j=0;
	char* current=l->seq[0][j];
	char* ret=malloc(50);
		while(current!=NULL){
		ret=strncat(ret,current,50);
		ret=strncat(ret," ",1);

		j++;
		current=l->seq[0][j];

	}
	if(l->backgrounded!=NULL){
		ret=strncat(ret,"&",1);
	}
	return ret;
}




int main(int argc,char *argv[]) {
	ref=calloc(PATH_MAX,sizeof(char));
	getcwd(ref,PATH_MAX);
	if(ref==NULL){
		perror("getcwd() error\n");
		ref="./";
	}

	//Sigaction liant SIGCHLD et son traitant 
	struct sigaction s; 
	s.sa_handler = suivi_fils; 
	s.sa_flags=0;
	sigaction(SIGCHLD,&s,NULL);
	
	//Sigaction liant SIGTSTP et son traitant 
	struct sigaction s2; 
	s2.sa_handler = ctrlZ; 
	s2.sa_flags=0;
	sigaction(SIGTSTP,&s2,NULL);
	
	//Sigaction liant SIGINT et son traitant 
	struct sigaction s3; 
	s3.sa_handler = ctrlC; 
	s3.sa_flags=0;
	sigaction(SIGINT,&s3,NULL);

	struct cmdline *line;
	while( !quit){
			
			//Tant qu'on insert pas une ligne vide
			
			line=NULL;
			while(line==NULL || *(line->seq)==NULL){
					printf("miniSh:%s$ ",ref);
					line=readcmd();
					
					
			}
		
		if(!strcmp(line->seq[0][0],"cd")){
				if (line->seq[0][1]==NULL){
					chdir(getenv("HOME"));
					getcwd(ref,PATH_MAX);
					if(ref==NULL){
						perror("getcwd() error\n");
						ref="./";
					}
				}
				else {
					if(chdir(line->seq[0][1])<0 ){
					printf("Une erreur est survenue :%d :",errno);
					if(errno==2){printf("Le chemin n'existe pas.\n");}
					}
					
					getcwd(ref,PATH_MAX);
					if(ref==NULL){
						perror("getcwd() error\n");
						ref="./";
					}
					
				}
				
				
		} else if (!strcmp(line->seq[0][0],"exit")) {
				quit=1;
				
		} else if (!strcmp(line->seq[0][0],"stop")) {
				int id=atoi(line->seq[0][1]);
				kill(tabProcess[id].pid,SIGSTOP); 
				
		} else if (!strcmp(line->seq[0][0],"bg")) {
				int id=atoi(line->seq[0][1]); //string to int
				tabProcess[id].backgrounded=1; //On indique qu'il est en arrière plan dans la description du processus
				kill(tabProcess[id].pid,SIGCONT);
				
		} else if (!strcmp(line->seq[0][0],"fg")) {
				int id=atoi(line->seq[0][1]);
				tabProcess[id].backgrounded=0; //On indique qu'il est en avant plan dans la description du processus
				forgroundedId=id;// On indique le le processus qui est en avant    
				kill(tabProcess[id].pid,SIGCONT);
				
				
		} else if (!strcmp(line->seq[0][0],"jobs")) {
				for(int j=0;j<taille;j++){
					if (!tabProcess[j].fini){ //Afficher que les processu pas encore quittés
						printf("[%d] Pid: %d Actif: %d Commande: %s\n",j,tabProcess[j].pid,tabProcess[j].actif,tabProcess[j].ligneCommande);
					}					
				}
		} else{		
				//Recherche de place vidé dans le tableau des processus par un processus fini
				int j=0;int quitLoop=0;
				while(j<taille && !quitLoop){
					if (tabProcess[j].fini){
						quitLoop=1;
					}
					j+=(1-quitLoop); 	//Si on trouve un processu fini avant la fin du tableau on n'incrémente pas j dans la dernière itération 				
				}
				
				if(j==taille){ // Si on a atteint la fin du tableau, on ajoute un élément à la fin du tableau
					taille++; 
				
					//printf("\n%ld %d\n",sizeof(tabProcess),taille);
					process *tempTabProcess=realloc(tabProcess,taille*sizeof(struct process));
					

					if(tempTabProcess!=NULL){
						tabProcess=tempTabProcess;
					} else{
						printf("Erreur\n");
						exit(1);
					}
				}
				
				

				
				process temp={j,0,0,toString(line),line->backgrounded!=NULL,0}; //On crée la déscription du processus (id,pid,actif ou pas,la commande, backgrounded,fini ou pas)
				tabProcess[j]=temp;
				
				int r=fork();
				if(r==-1){
					printf("Fork error\n");
				} else if (r==0){ //fils
					//On masque pour tout nouveau fils le signal SIGTSTP pour que seul le père en a accès
					sigset_t ens;
					sigemptyset(&ens);	
					sigaddset(&ens, SIGTSTP); 
					sigaddset(&ens, SIGINT); //on masque aussi SIGINT pou la même raison
					sigprocmask (SIG_SETMASK, &ens, NULL);
					//execution de la commande
					executer(line);
					
					exit(1); // fin du fils si execvp ne fonctionne pas
				} else { // père
					//On complète la description du processus
					tabProcess[j].pid=r;
					tabProcess[j].actif=1;
					//Si la commande est en avant plan, l'id du processus est réservé dans forgroundedId
					if(!tabProcess[j].backgrounded){
						forgroundedId=j;
					}
					
					
				}	
			
		}
		
		//Si un processus est en avant-plan
		while(forgroundedId!=-1){
			//on attend qu'il se termine
			pause();
		}
			
		}
	
	exit(0);
}


