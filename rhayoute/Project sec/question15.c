/* TP-Fichiers
** @author RHAYOUTE Abdelmalek
*/

/* Réponse 15 : On ouvre le fichier temp.txt en mode ecriture 
** dans le processus fils, et on écrit de 1 à 30 en revenant 
** au début du fichier tous les 10 entiers, et on ouvre le  même
** fichier en mode lecture dans le processus père et on affiche 
** régulièrement le contenu du fichier sur la sortie standard.
** on peut alors prévoir qu'on va afficher les entiers de 1 à 10
** et on s'arrêtera après car on arrive à la fin du fichier puisque 
** l'écriture des entiers de 11 à 20 écrasent les entiers précédents
** du 1 à 10 et du même pour les entiers de 21 à 30. Par contre, 
** si on ne revient pas au début du fichier tous les 10 entiers, on
** affiche bien les entiers de 1 à 30.
*/

#include <stdio.h>    /* entrées sorties */
#include <unistd.h>   /* pimitives de base : fork, ...*/
#include <stdlib.h>   /* exit */
#include <sys/wait.h> /* wait */
#include <string.h>   /* opéations sur les chaines */
#include <fcntl.h>    /* opéations sur les fichiers */
#include <fcntl.h>


#define BUFSIZE 5


int main (int argc, char *argv[]){
    int desc_fic, status, retour_fermer,retour;
    int nb_octets = BUFSIZE;
    pid_t pid_Fils;
    char fichier[] = "temp.txt";

    char tampon[BUFSIZE];

    // Vider le tampon.
    bzero(tampon,sizeof(tampon));

    // Créer le fils.
    pid_Fils = fork();

    if (pid_Fils < 0) {
        // Echec du fork
        perror("Erreur Fork");
        exit(1);
    } else if (pid_Fils == 0){

        // Ouvrir le fichier. 
        desc_fic = open(fichier,O_WRONLY | O_TRUNC | O_CREAT,0600);

        // Traiter le cas d'erreur d'ouverture.
        if (desc_fic < 0) {
            perror("Erreur d'ouverture");
            exit(1);
        } 

        for (int i = 1; i < 31; i++){

            //Ecrire dans le fichier.
            sprintf(tampon,"%d\n",i);
            retour = write(desc_fic,tampon,sizeof(tampon));

            // Traiter le cas d'erreur d'ecriture
            if (retour == -1){
                perror("Erreur d'ecriture ");
                exit(1);
            }

            // Revenir au debut du fichier tous les 10 entiers.
            if (i % 10 == 0){
                lseek(desc_fic,0,SEEK_SET);
            }

            // Vider le tampon.
            bzero(tampon,sizeof(tampon));

            // Attendre une seconde.
            sleep(1);

        }

        // Terminaison normale sans erreur.
        exit(EXIT_SUCCESS);

    } else { // Père

        // Ouvrir le fichier en mode lecture. 
        desc_fic = open(fichier,O_RDONLY);

        // Traiter le cas d'erreur d'ouverture.
        if (desc_fic < 0) {
            perror("Erreur d'ouverture");
            exit(1);
        } 

        while (nb_octets == BUFSIZE){

            
            // On attend ici jusqu'à la première écriture
            // dans le fichier par notre fils.
            sleep(1);

            nb_octets = read(desc_fic,tampon,sizeof(tampon));

            printf("%s",tampon);

            // Vider le tampon.
            bzero(tampon,sizeof(tampon));

        }
    
    }

    // Attendre la fin du fils.
    wait(&status);

    // Fermeture du fichier.
    retour_fermer = close(desc_fic);
    if (retour_fermer  == -1){
        perror("Erreur de fermeture");
        exit(1);
    }

    printf("Terminé\n");

    return EXIT_SUCCESS;

}
