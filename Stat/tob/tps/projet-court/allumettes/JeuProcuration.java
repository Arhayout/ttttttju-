package allumettes;

public class JeuProcuration implements Jeu {

    /** le jeu pour laquel on veut faire une procuration.*/
	private Jeu jeu;


	 /** Construire une procuration à partir du nombre d'allumettes choisi.
	 * @param jeu
     */

	public JeuProcuration(Jeu jeu) {
		assert jeu != null;
		this.jeu = jeu;
	}

	@Override
	public int getNombreAllumettes() {
		return this.jeu.getNombreAllumettes();
	}

	@Override
	public void retirer(int nbPrises) throws OperationInterditeException {

		throw new OperationInterditeException("heu");
	}

}
