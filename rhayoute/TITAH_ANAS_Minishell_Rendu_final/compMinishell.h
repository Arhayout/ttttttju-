/** Les composantes de notre minishell
**  Les procédures et fonctions utilisées 
**  @author : Anas Titah
**  @version : 1.0 
*/
#ifndef __COMPMINISHELLL_H
#define __COMPMINISHELL_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h> /* wait */
#include <string.h>
#include "readcmd.h"
#include "procMinishell.h"
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include "cmdinternes.h"


/* Définition de quelque couleurs utilisées pour
** l'affichage des messages*/
#define ROUGE "\033[00;31m"
#define VERT  "\033[00;32m"
#define CYANCLAIR "\033[00;96m"
#define AUCUN "\033[00m"


/* Pointeur sur la liste où on stocke les processus non encore terminées. */
infoproc **liste;

/* Le pid du processus en avant-plan. */
pid_t processus_en_cours;

/* Variables utilisées pour savoir si ctrl-z  ou ctrl-c est entrée. */
int signal_ctrl_z;
int signal_ctrl_c;

/* Variable n pour attribuer à chaque processur son id unique par rapport
** au minishell */
int n;

/* Afficher le prompt du minishell */
void afficher_prompt();

/* Exécuter les commandes internes.
** Renvoie 1 si il s'agit d'une commande interne,
** sinon renvoie 0*/
int execution_cmd_interne(struct cmdline *c);

/* Exécuter les commandes externes sans (Pipes)*/
void execution_cmd_simple(struct cmdline *c);

/* Exécuter les commandes externes avec (Pipes)*/
void execution_cmd_pipes(struct cmdline *c);




#endif