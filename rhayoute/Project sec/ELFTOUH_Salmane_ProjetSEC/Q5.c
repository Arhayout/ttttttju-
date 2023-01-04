#include <stdio.h>    /* printf */
#include <stdlib.h>   /* EXIT_SUCCESS */
#include  <unistd.h> /* fork */
#include <sys/wait.h> /* wait */
#include "readcmd.c"

int quit=0;

char *ref;

int main(int argc,char *argv[]) {
	
	ref=calloc(PATH_MAX,sizeof(char));
	getcwd(ref,PATH_MAX); //Retourne le chemin courant
	if(ref==NULL){
		perror("getcwd() error\n");
		ref="./";
	}
	
	struct cmdline *line;
	while( !quit){

		printf("miniSh:%s$ ",ref);
		line=readcmd();
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
		
		} else{	
			int r=fork();
			if(r==-1){
				printf("Fork error\n");
			} else if (r==0){ //fils
				execvp(line->seq[0][0],line->seq[0]);
				exit(1); // fin du fils si execvp ne fonctionne pas
			} else { // père
				if(line->backgrounded==NULL){
					int status;
					wait(&status);
				}
			}
		}
	}	

	exit(0);
}


