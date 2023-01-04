package allumettes;

import java.util.Random;

public class StrategieNaif implements Strategie {

	/**
	 * le nombre d'allumettes qu'on doit prendre est lié au nombre d'allumettes
	 * restantes. Si ce dernier est supérieur à Jeu.PRISE_MAX On donne la liberté de
	 * prendre soit une ou 2 ou Jeu.PRISE_MAX allumettes sinon on prend soit 1 soit
	 * 2 ça dépend au nombre d'allumettes restantes.
	 */

	@Override
	public int getPrise(Jeu jeu) {
		assert jeu != null;
		Random ran = new Random();
		if (jeu.getNombreAllumettes() > Jeu.PRISE_MAX) {
			return ran.nextInt(Jeu.PRISE_MAX) + 1;
		} else {
			return ran.nextInt(jeu.getNombreAllumettes()) + 1;
		}
	}

}
