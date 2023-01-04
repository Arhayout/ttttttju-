package allumettes;

import java.util.Random;

public class StrategieExpert implements Strategie {
	/*
	 * Dans cette stratégie ,l'astuce c'est de ne
	 * pas commencer , et de passer de 13 à 9 et après à 5
	 */

	@Override
	public int getPrise(Jeu jeu) {
		Random ran = new Random();
		int nbAllumettes = jeu.getNombreAllumettes();
		if ((nbAllumettes - 1) % (Jeu.PRISE_MAX + 1) == 0) {
			return ran.nextInt(Math.min(Jeu.PRISE_MAX, nbAllumettes)) + 1;
		} else {
			return (nbAllumettes - 1) % (Jeu.PRISE_MAX + 1);
		}
	}

}

