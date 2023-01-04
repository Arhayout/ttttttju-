#include <stdio.h>    /* printf */
#include <stdlib.h>   /* EXIT_SUCCESS */
#include  <unistd.h> /* fork */
#include <sys/wait.h> /* wait */
#include "readcmd.c"


int quit=0;

int main(int argc,char *argv[]) {
	

	struct cmdline *line;
	while( !quit){

		printf("miniSh:./$ ");
		line=readcmd();
				
				
		int r=fork();
		if(r==-1){
			printf("Fork error\n");
		} else if (r==0){ //fils
			execvp(line->seq[0][0],line->seq[0]);
			exit(1); // fin du fils si execvp ne fonctionne pas
		} else { // père
			
		}
			
				
	}	
			

	exit(0);
}


