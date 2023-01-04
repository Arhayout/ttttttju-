package allumettes;

public class StrategieTricheur implements Strategie {

	// Cette stratégie consiste a retirer toutes les allumettes et ne laisser que 2

	@Override
	public int getPrise(Jeu jeu) {
		try {
			while (jeu.getNombreAllumettes() > 2) {
				jeu.retirer(1);
			}
		} catch (CoupInvalideException e) {
			System.out.print("Le nombre d'allumettes que vous"
					+ " avez pris est invalide");
		}
		return 1;
	}

}
