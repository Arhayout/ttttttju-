/** Traitants des signaux gérés par notre
 ** minishell
 ** @author : Anas Titah
 ** @version : 1.1
 **/

#ifndef __SIGNALMINISHELL_H
#define __SIGNALMINISHELL_H

#include "compMinishell.h"

/* Définir un traitant pour traiter les différents signaux 
** (SIGCHLD, SIGINT et SIGTSTP). */
void traitant_signaux(int sig);

/* Traitement du signal SIGCHLD*/
void traitement_SIGCHLD();

/* Traitement du signal SIGTSTP*/
void traitement_SIGTSTP();

/* Traitement du signal SIGINT*/
void traitement_SIGINT();


#endif
