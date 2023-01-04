package allumettes;

public class Joueur {

	/** le nom du joueur.*/
	private String nom;

	/** La stratégie choisi.*/
	private Strategie strategie;

	 /** Consruire un joueur à partir de son nom et la stratégie qu'il a choisi.
	   * @param name :le nom du joueur
	   * @param strategie : la stratégie choisi*/

	public Joueur(String name, Strategie strategie) {
		assert strategie != null;
		assert name != null;
		this.nom = name;
		this.strategie = strategie;
	}


	 /** Obtenir le nom du joueur.
	 * @return le nom du joueur
	 */

	public String getNom() {
		return this.nom;
	}


	 /** Obtenir le nom du joueur.
     *@param jeu
	 * @return le nom du joueur
	 */

	public int getPrise(Jeu jeu) {
		return this.strategie.getPrise(jeu);
	}

}
