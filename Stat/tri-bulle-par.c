#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "fournitures/taille.h"
 
int pipe_asc[2];
int pipe_desc[2];
int value_courant;
int successeur;
int length;
 
int main(int argc,char *argv[]) {
    if(argc != 2) {
        printf("Usage: tri bulle fichier.txt\n");
        return EXIT_FAILURE;
    }
 
    int fdesc = open(argv[1], O_RDONLY);
    if (fdesc == -1) {
        perror("open");
        return EXIT_FAILURE;
    }
 
    length = taille(fdesc);
    read(fdesc, &value_courant, sizeof(int));
 
    int pid;
    for(int i = 0; i < length-1; i++) {
        int p1[2], p2[2], val;
        if(i > 0) {
            value_courant = successeur;
        }
 
        read(fdesc, &successeur, sizeof(int));
        if(pipe(p1) < 0 || pipe(p2) < 0) {
            perror("pipe");
            return EXIT_FAILURE;
        }
        //printf("pipe: id = %d, %d %d\n", i, p1[0], p1[1]);
 
        if((pid = fork()) == -1) {
            perror("fork");
            return EXIT_FAILURE;
        }
        if(pid == 0) {
            close(fdesc);
            close(p1[0]);
            close(p2[1]);
            pipe_asc[1] = p1[1];
            pipe_desc[0] = p2[0];
 
            main_loop(i);
 
            return EXIT_SUCCESS;
        }
        else {
            close(p1[1]);
            close(p2[0]);
 
            if(i > 0) {
                close(pipe_asc[0]);
                close(pipe_desc[1]);
            }
 
            pipe_asc[0] = p1[0];
            pipe_desc[1] = p2[1];
        }
    }
 
    close(fdesc);
    value_courant = successeur;
    switch(fork()) {
        case -1:
            perror("fork");
            return EXIT_FAILURE;
        case 0:
            main_loop(length - 1);
            return EXIT_SUCCESS;
    }
 
    for(int i = 0; i < length; i++) {
        wait(NULL);
    }
 
    return EXIT_SUCCESS;
}
 
void main_loop(int id) {
    int val_succ, val;
    for(int i = 0; i < length-id; i++) {
        if(id > 0) {
            read(pipe_asc[0], &value_courant, sizeof(int));
        }
 
        if(i < length-id-1) {
            if(value_courant > successeur) {
                val = value_courant;
                value_courant = successeur;
                successeur = val;
            }
            write(pipe_asc[1], &successeur, sizeof(int));
        }
        if(id > 0) {
            write(pipe_desc[1], &value_courant, sizeof(int));
        }
        if(i < length-id-1) {
            read(pipe_desc[0], &successeur, sizeof(int));
        }
    }   
 
    if(id > 0)  close(pipe_asc[0]);
    if(id < length-1) close(pipe_asc[1]);
    if(id < length-1) close(pipe_desc[0]);
    if(id > 0) close(pipe_desc[1]);
}
 
