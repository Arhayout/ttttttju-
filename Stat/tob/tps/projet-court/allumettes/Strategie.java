package allumettes;

/*les stratégies sont choisies par le joueur.
 */

public interface Strategie {


    /** Donner le nombre d'allumettes qui veut prendre l'utilisateur d'après sa stratégie.
	 * @param jeu donné
	 * @return un nombre d'allumettes entre 1 et Jeu.PRISE_MAX suivant la stratégie
	*/

	int getPrise(Jeu jeu);

}
