/** (mini)Projet (mini)Shell
 ** Développer un interpréteur de commandes simplifié
 ** @author : abdelmalek rhayoute
 */

int main(int argc, char *argv[]) {

    /* Variables utilisées*/
    int codeTerm;
    extern pid_t processus_en_cours;
    extern infoproc **liste;
    liste =  malloc(TAILLE_ELEMENT_LISTE);

    /* Boucle Infini qui décrit le comportement de base de l'interpréteur */
    while (1){
        /* Variables qui s'initialisnet à l'avant entrée de 
        ** chaque nouvelle commande*/
        struct cmdline *c;
        processus_en_cours = 0;
        signal_ctrl_z = 0;
        signal_ctrl_c = 0;

        /* Traiter les signaux SIGCHLD, SIGTSTP et SIGINT*/
        signal(SIGCHLD,traitant_signaux);
        signal(SIGTSTP,traitant_signaux);
        signal(SIGINT,traitant_signaux);

        /* Afficher le prompt du minishell */
        afficher_prompt();

        /* Lire la commande. */
        c = readcmd();

        /* Traiter la commande. */
        /* La condition (si) suivante est faite pour ne pas quitter le 
        ** minishell si on clique sur entrée sans passer une commande. */
        if (!(c->seq[0] == NULL)){
            /* On lande execution_cmd_interne(c), si elle renvoie 0, donc 
            ** c'est une commande externe, sinon, elle renvoie 1 ( ainsi elle 
            ** exécute le code correspondant) et on ne rentre
            ** pas dans la condition si*/
            if (execution_cmd_interne(c) == 0){
                if (c->seq[1] == NULL){
                    /* Exécution sans Pipes*/
                    execution_cmd_simple(c);

                } else {
                    /* Exécution avec Pipes*/
                    execution_cmd_pipes(c);
                }
                
            }
        }
    }
}
