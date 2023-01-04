package allumettes;

/**
 * Lance une partie des 13 allumettes en fonction des arguments fournis sur la
 * ligne de commande.
 * @version $Revision: 1.5 $
 */
public class Partie {


	/**
	 * Lancer une partie. En argument sont donnés les deux joueurs sous la forme
	 * nom@stratégie.
	 * @param args la description des deux joueurs
	 */

	public static void main(String[] args) {
	/** Nombre d'allumettes.*/
        final int nballumettes = 13;

		try {
			verifierNombreArguments(args);
			Jeu jeu = new JeuImplante(nballumettes);
			int i;
            int j;
			String line;
			/**Si on argument confiant on a faire un split pour l'argument
			  * 1 et 2, sinon pour les 2 premiers arguments.*/
			if (args[0].equals("-confiant") && args.length == Jeu.PRISE_MAX) {
				i = 1;
				j = 2;
			} else if (args.length == 2) {
				if (args[0].indexOf("@") != -1
                        && args[1].indexOf("@") != -1) {
					i = 0;
					j = 1;
				} else {
					throw new ConfigurationException("");
				}
			} else {
				throw new ConfigurationException("");
			}
			line = new StringBuilder(args[i])
                            .append("@").append(args[j]).toString();
			String[] joueurStrategie = line.split("@");
			Joueur j1 = new Joueur(joueurStrategie[0],
                             getStrategie(joueurStrategie[1]));
			Joueur j2 = new Joueur(joueurStrategie[2],
                             getStrategie(joueurStrategie[Jeu.PRISE_MAX]));
			Arbitre arbitre = new Arbitre(j1, j2);
			if (args.length == 2) {
				arbitre.setConfiant(EnumerationConfiant.MEFIANT);
			} else if (args.length == Jeu.PRISE_MAX) {
				if (args[0].equals("-confiant")) {
					arbitre.setConfiant(EnumerationConfiant.CONFIANT);

				} else {
					System.out.println("La commande est indéfinie");
				}
			} else {
				System.out.println("La commande est indéfinie");
			}
			arbitre.arbitrer(jeu);
		} catch (ConfigurationException e) {
			System.out.println();
			System.out.println("Erreur : " + e.getMessage());
			if (args[0].equals("-confiant")) {
                    afficherUsage();
                    afficherUsageConfiant();
            } else {
                    afficherUsage();
            }
			System.exit(1);
		}
	}

    /**
	 * Récuperer la stratégie donnée par l'utilisateur.
	 * @param nomStrategie le nom de la strategie donnée par l'utilisateur
	 * @return la stratégie
	 */

	public static Strategie getStrategie(String nomStrategie) {
		Strategie strategie;
		switch (nomStrategie) {
		case "naif":
			strategie = new StrategieNaif();
			break;
		case "rapide":
			strategie = new StrategieRapide();
			break;
		case "expert":
			strategie = new StrategieExpert();
			break;
		case "humain":
			strategie = new StrategieHumain();
			break;
		case "tricheur":
			strategie = new StrategieTricheur();
			break;
		default:
			throw new ConfigurationException("");
		}
		return strategie;
	}

	private static void verifierNombreArguments(String[] args) {
		final int nbJoueurs = 2;
		if (args.length < nbJoueurs) {
			throw new ConfigurationException("Trop peu d'arguments : "
                        + args.length);
		}
		if (args.length > nbJoueurs + 1) {
			throw new ConfigurationException("Trop d'arguments : "
                                    + args.length);
		}
	}

	/** Afficher des indications sur la manière d'exécuter cette classe. */
	public static void afficherUsage() {
		System.out.println("\n" + "Usage :" + "\n\t"
                    + "java allumettes.Partie joueur1 joueur2" + "\n\t\t"
				+ "joueur est de la forme nom@stratégie" + "\n\t\t"
				+ "strategie = naif | rapide | expert | humain | tricheur"
                + "\n" + "\n\t" + "Exemple :" + "\n\t"
	 + "	java allumettes.Partie Xavier@humain " + "Ordinateur@naif" + "\n");
	}

    	/** Afficher des indications sur la manière d'exécuter cette classe
                au cas ou on a un mode confiant. */
	public static void afficherUsageConfiant() {
		System.out.print("\n" + "Usage Pour le mode confiant : \n\t "
                + "java allumettes.Partie -confiant joueur1 joueur2\n\t");
System.out.print("\nExemple :\n\t"  + " java allumettes.Partie -confiant"
            + "  Xavier@humain " + "Ordinateur@naif \n\n");
	}

}
