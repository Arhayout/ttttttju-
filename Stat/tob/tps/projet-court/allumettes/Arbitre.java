package allumettes;

public class Arbitre {
    /** le 1er joueur.*/
	private Joueur joueur1;
    /** le 2ème joueur.*/
	private Joueur joueur2;
    /** l'état de tricheur.*/
	private Confiance confiant;

    /** on construit un arbitre d'après deux joueurs.
        @param j1
        @param j2
     */
	public Arbitre(Joueur j1, Joueur j2) {
		this.joueur1 = j1;
		this.joueur2 = j2;
	}

    /** une méthode qui donne le joueur qui gagne.
            @param joueur
            @return joueur*/
	public Joueur victoire(Joueur joueur) {
		Joueur joueur3;
		if (joueur == this.joueur1) {
			joueur3 = this.joueur2;
		} else {
			joueur3 = this.joueur1;
		}
		return joueur3;
	}

    /** une méthode qui va arbiter le jeu et donne la main a chaque joueur.
            @param jeu*/
	public void arbitrer(Jeu jeu) {
		assert jeu != null;
		assert jeu.getNombreAllumettes() > 0;
		Jeu jeuproxy = new JeuProcuration(jeu);
		boolean tourJoueur = true;
		int nbPrises1;
		Joueur joueur = this.joueur1;
		while (jeu.getNombreAllumettes() > 0) {
			try {
				System.out.println("\nNombre d'allumettes restantes : "
                                 + jeu.getNombreAllumettes());
				if (tourJoueur) {
					joueur = this.joueur1;
				} else {
					joueur = this.joueur2;
				}
				System.out.println("Au tour de " + joueur.getNom() + ".");
				if (this.confiant == EnumerationConfiant.CONFIANT) {
					nbPrises1 = joueur.getPrise(jeu);
				} else if (this.confiant == EnumerationConfiant.MEFIANT) {
					nbPrises1 = joueur.getPrise(jeuproxy);
				} else {
					nbPrises1 = 0;
				}
				if (nbPrises1 <= 1) {
					System.out.print(joueur.getNom() + " prend "
                                + nbPrises1 + " allumette.\n");
				} else {
					System.out.print(joueur.getNom() + " prend "
                                        + nbPrises1 + " allumettes.\n");
				}
				jeu.retirer(nbPrises1);
				tourJoueur = !tourJoueur;

			} catch (CoupInvalideException e) {
				System.out.print("Erreur ! ");
				System.out.println(e.getMessage());
				System.out.println("Recommencez !");
			} catch (OperationInterditeException e) {
				System.out.print("Partie abandonnée car"
                            + joueur.getNom() + " a triché !");
				Runtime.getRuntime().exit(1);
			}
		}
		System.out.println(joueur.getNom() + " a perdu !\n"
                 + victoire(joueur).getNom() + " a gagné !");

	}
    /** Si on donne l'état de tricheur s'il est confiant ou mefiant.
                   @param conf */
	public void setConfiant(Confiance conf) {
		this.confiant = conf;
	}

}
