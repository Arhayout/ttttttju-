package allumettes;

import java.util.Scanner;

public class StrategieHumain implements Strategie {

	/** On va utiliser un constructeur car scan doit se fermer.*/
	private Scanner scan;
	/** Constructeur de cette stratégie.*/
	public StrategieHumain() {
		this.scan = new Scanner(System.in);
	}

	/** On laisse la liberté au joueur de choisir le nombre d'allumettes qu'il veut.*/

	@Override

	public int getPrise(Jeu jeu) {
		boolean estPrise = true;
		while (estPrise) {
			System.out.print("Combien prenez-vous d'allumettes ? ");
			try {
				int prise = Integer.parseInt(this.scan.nextLine());
				estPrise = false;
				return prise;
			} catch (IllegalArgumentException e) {
				System.out.println("Vous devez donner un entier.");
			}
		}
		return 0;
	}

}
