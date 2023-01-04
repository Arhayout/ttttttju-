package allumettes;

public class StrategieRapide implements Strategie {

	/**
	 * le nombre d'allumettes qu'on doit prendre est lié au nombre d'allumettes
	 * restantes. Si ce dernier est supérieur à Jeu.PRISE_MAX On prend Jeu.PRISE_MAX
	 * allumettes car on veut finir la partie le plus vite possible. Sinon on prend
	 * le nombre d'allumettes restantes.
	 */
	@Override
	public int getPrise(Jeu jeu) {
		assert jeu != null;
		if (jeu.getNombreAllumettes() > Jeu.PRISE_MAX) {
			return Jeu.PRISE_MAX;
		} else {
			return jeu.getNombreAllumettes();
		}
	}

}
