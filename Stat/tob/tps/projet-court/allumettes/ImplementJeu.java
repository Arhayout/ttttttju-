package allumettes;

public class ImplementJeu implements Jeu {

    /** Nombre d'allumettes restantes .*/
	private int nombreAllumettes;


	 /** Construire le jeu 
	 * @param n
	 */
	public ImplementJeu(int n) {
		assert n > 0;
		this.nombreAllumettes = n;
	}

	@Override
	public int getNombreAllumettes() {
		return nombreAllumettes;
	}

	@Override
	public void retirer(int nombrePris) throws CoupInvalideException {
		if (nombrePris > Jeu.PRISE_MAX) {
            if (Jeu.PRISE_MAX < this.nombreAllumettes) {
                throw new CoupInvalideException(nombrePris, "> " + Jeu.PRISE_MAX);
            } else {
                throw new CoupInvalideException(nombrePris, "> " + this.nombreAllumettes);
            }
        } else if (nombrePris <= 0) {
            throw new CoupInvalideException(nombrePris, "< 1");
        } else if (nombrePris > this.nombreAllumettes) {
            throw new CoupInvalideException(nombrePris, "> " + this.nombreAllumettes);
        } else {
            this.nombreAllumettes -= nombrePris;
		}
	}

}
