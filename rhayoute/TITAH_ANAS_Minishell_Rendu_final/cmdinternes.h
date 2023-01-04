/** Les commandes internes du notre processus
 ** @author : Anas Titah
 ** @version : 1.0
*/
#ifndef __CMDINTERNES_H
#define __CMDINTERNES_H

#include "procMinishell.h"
#include "readcmd.h"

/* La commande interne exit du minishell */
void cmd_exit();

/* La commande interne cd du minishell */
void cmd_cd(struct cmdline *c);

/* La commande interne list du minishell */
void cmd_list(infoproc **liste);

/* La commande interne stop du minishell */
void cmd_stop(struct cmdline *c, infoproc **liste);

/* La commande interner bg ou fg du minishell */
int cmd_bg_fg(struct cmdline *c, infoproc **liste);

#endif
