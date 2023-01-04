#include <stdio.h>    /* printf */
#include <stdlib.h>   /* EXIT_SUCCESS */
#include  <unistd.h> /* fork */
#include  <fcntl.h> /* les fichiers */
#include <sys/wait.h> /* wait */
#include "readcmd.c"
#define max_path 500

//LE FICHIER DE LA QUESTION 10 EST LA MEME QUE LE FICHIER DE LA QUESTION 9
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


//Fonction sui determine le nombre de pipes à partie de "line"
int getNbPipe(struct cmdline* line){
	int nb=0;
	while(line->seq[nb+1]!=NULL){
		nb++;
	}
	return nb;
}


//Fonction qui ferme toutes les E/S des tubes sauf ceux en paramètre puis duplique les descripteur du tube aux descripteur standard du processus
int linkPipe(int indicePipeIn,int indicePipeOut,int pipeDescr[][2],int size){
	
	for(int i=1;i<size-1;i++){
		if(i==indicePipeIn){close(pipeDescr[i][1]); }
		else if(i==indicePipeOut){close(pipeDescr[i][0]); }
		else{close(pipeDescr[i][0]);close(pipeDescr[i][1]); }
	}
	int r=0; 
	if(pipeDescr[indicePipeIn][1]!=0) {
		r|=dup2(pipeDescr[indicePipeIn][0],0);
		close(pipeDescr[indicePipeIn][1]);
	}
	if(pipeDescr[indicePipeOut][0]!=1) {
		r|=dup2(pipeDescr[indicePipeOut][1],1);
		close(pipeDescr[indicePipeOut][0]);
	}
	return r;
}


//Fonction récursive qui execute le pipeline
int executerPipeline(struct cmdline* line,int indiceCmd,int pipeDescr[][2],int size){
	
	
	int retour=0;
	
	
	
	
	if (indiceCmd!=size-2){// Si la commande à executer n'es pas la dérnière
	
		
		int rf2=fork();
		if (rf2==-1){
			printf("Erreur fork()\n");
			exit(1);
		}
		else if(rf2==0){ //process 1 (fils)		
			
			linkPipe(indiceCmd,indiceCmd+1,pipeDescr,size);	//Fermer les descripteurs inutilisés et lier le tube et le processus	
			
			execvp(line->seq[indiceCmd][0],line->seq[indiceCmd]); // executer la commande
			 
			retour|=1;//Si excevp echoue
			return retour;
		}
		else{ //process 2 (père)
			retour|=executerPipeline(line,indiceCmd+1,pipeDescr,size);	// Executer le reste de la pipeline
		}
		
	}
	else   {//Si la fin de la pipeline (Dans ce cas on ne partage le processus en 2 (père et fils))

		linkPipe(indiceCmd,indiceCmd+1,pipeDescr,size);	//Fermer les descripteurs inutilisés et lier le tube et le processus
		execvp(line->seq[indiceCmd][0],line->seq[indiceCmd]);// executer la commande
		retour|=1;//Si excevp echoue
		return retour;
	}
	
	
	return retour;
}


//Fonction qui execute la commande "line"
int executer(struct cmdline* line){
	int outDescr=1;// la sortie de la commande est par défaut la sortie standard
	int inDescr=0; // l'entree de la commande est par défaut l'entrée standard
	
	
	if(line->in!=NULL){ //Si redirection d'entree
		int inFile=open(line->in,O_RDONLY,NULL);
		inDescr=dup2(inFile,inDescr);
		close(inFile);
	}
	
	if(line->out!=NULL){ //Si redirection de sortie
		int outFile=open(line->out,O_WRONLY|O_CREAT,0640);
		outDescr=dup2(outFile,outDescr);
		close(outFile);
	}
	
	int size=getNbPipe(line)+2; //Le nombre de pipe à créer
	int pipelineDescr[size][2]; // tableau contenant toutes les pipes
	pipelineDescr[0][1]=inDescr;
	pipelineDescr[size-1][0]=outDescr;
	
	//Création des pipes 
	for(int i=1;i<size-1;i++){
		pipe(pipelineDescr[i]);
	}
	
	//execution de la pipeline
	int r=executerPipeline(line,0,pipelineDescr,size);
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
					int r=executer(line);
					printf("Une erreur est survenue: %d\n",r);
					exit(1+r); // fin du fils si execvp ne fonctionne pas
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


