/*  TP-Fichiers
**  @author RHAYOUTE Abdelamlek
 */

#include <stdio.h>    /* entrées sorties */
#include <unistd.h>   /* pimitives de base : fork, ...*/
#include <stdlib.h>   /* exit */
#include <string.h>   /* opéations sur les chaines */
#include <fcntl.h>    /* opéations sur les fichiers */
#include <fcntl.h>

#define BUFSIZE 4096


int main (int argc, char **argv){
    int desc_read, desc_write;
    int nb_octets = BUFSIZE;
    int retour, retour_fermer1, retour_fermer2;
    char buffer[BUFSIZE];

    // Vider le buffer.
    bzero(buffer,sizeof(buffer));

    // Ouvrir les fichiers nécessaires
    desc_read  = open(argv[1], O_RDONLY);
    desc_write = open(argv[2], O_WRONLY |O_TRUNC | O_CREAT, 0600);

    // Traiter le cas d'erreur d'ouverture.
    if ((desc_read < 0) | (desc_write < 0)) {
        perror("Erreur d'ouverture");
        exit(1);
    } 
    
    // On s'arrête lorsque le dernier nombre d'octets lus 
    // est inférieur strictement à BUFSIZE parce que on est
    // dans la fin du fichier.

    while (nb_octets == BUFSIZE) {

        // Lire les BUFSIZE octets du fichier ouvert en lecture
        // et les recopier dans le buffer.
        nb_octets = read(desc_read,buffer,sizeof(buffer));

        // Cas d'erreur de la lecture
        if (nb_octets == -1 ){
            perror("Erreur de lecture");
            exit(1);
        }

        // Ecrire les BUFSIZE octets du buffer dans le fichier 
        // qui a été ouvert en écriture.
        retour = write(desc_write,buffer,nb_octets);

        // Cas d'erreur d'ecriture
        if (retour == -1){
            perror("Erreur d'ecriture");
            exit(1);
        }

        // Vider le buffer.
        bzero(buffer,sizeof(buffer));
    }  

    // Fermer les fichiers ouverts.
    retour_fermer1 = close(desc_read);
    retour_fermer2 = close(desc_write);

    if ((retour_fermer1 == -1) | (retour_fermer2 == -1)){
        perror("Erreur de fermeture");
        exit(1);
    }

    exit(EXIT_SUCCESS);

}
