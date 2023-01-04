package allumettes;

public class StrategieLente implements Strategie {

	/**
	 * le nombre d'allumettes qu'on doit prendre est 1.
	 * Car il s'agit d'une stratégie lente
	 */
	@Override
	public int getPrise(Jeu jeu) {
		    assert jeu != null;
			return 1;
	}

}
