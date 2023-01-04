package supermarche;

import java.util.ArrayList;
import java.util.List;

/** La classe TicketBuilder permet de définir les différentes informations
 * nécessaires à la création d'un ticket avant de fournir le ticket
 * correspondant.
 *
 * @author	Xavier Crégut
 */
public class TicketBuilder {

	//@ private invariant LG_SOMME >= 5;
	//@ private invariant LG_TEXTE >= 5;
	//@ private invariant LG_LIGNE == LG_SOMME + LG_TEXTE;

	/** Une ligne de séparation d'un ticket. */
	private final static String SEPARATION = "---------------------------------";

	/** Le nombre de caractère sur lequel est affiché une somme. */
	private static int LG_SOMME = 6;

	/** La longueur d'une ligne du ticket. */
	private static int LG_LIGNE = SEPARATION.length();

	/** Le nombre de caractères sur une ligne du ticket. */
	private static int LG_TEXTE = LG_LIGNE - LG_SOMME;

	/** Une ligne blanche. */
	private final static String BLANC = SEPARATION.replace('-', ' ');

	/** La classe Ligne permet de conserver les informations relatives à un
	 * article du ticket.
	 */
	private static class Ligne {
		private String libelle;
		private long prixUnitaire;
		private int quantite;

		public Ligne(String libelle, long prixUnitaire, int quantite) {
			this.libelle = libelle;
			this.prixUnitaire = prixUnitaire;
			this.quantite = quantite;
		}

		public String libelle() {
			return this.libelle;
		}

		public long prixUnitaire() {
			return this.prixUnitaire;
		}

		public int quantite() {
			return this.quantite;
		}

		public long prixTotal() {
			return this.prixUnitaire * this.quantite;
		}
	}

	/** Les lignes du ticket. */
	private List<Ligne> lignes;

	/** Le numéro du ticket. */
	private int numero;

	/** Commencer à construire un nouveau ticket, initialement vide. */
	public TicketBuilder() {
		this.lignes = new ArrayList<Ligne>();
		this.numero = 0;
	}

	/** Ajouter un article.
	 * @param libelle libelle de l'article
	 * @param prixUnitaire prix unitaire de l'article
	 * @param quantite quantité achetée de l'article
	 */
	//@ requires libelle != null && libelle.length() > 0;
	//@ requires prixUnitaire >= 0;
	//@ requires quantite > 0;
	public void ajouterArticle(String libelle, long prixUnitaire, int quantite) {
		this.lignes.add(new Ligne(libelle, prixUnitaire, quantite));
	}

	/* Définir le numéro du ticket.
	 * @param numéro le numéro du ticket
	 * @throws RuntimeException si le numéro a déjà été défini
	 */
	//@ requires numero > 0;
	public void definirNumero(int numero) {
		if (this.numero != 0 && this.numero != numero) {
			throw new RuntimeException("Le numéro de ticket a déjà été défini à " + this.numero);
		}
		this.numero = numero;
	}

	/** Afficher une ligne faisant apparaître un texte et une somme.  La somme
	 * est affichée en utilisant somme2txt.  Si le texte ou la somme sont trop
	 * longs, ils sont tronqués.
	 * @param texte le texte à afficher à gauche
	 * @param somme la somme à afficher à droite
	 * @return le texte de la ligne
	 */
	//@ requires texte != null;
	//@ requires somme >= 0;
	private String ligne2String(String texte, long somme) {
		String libelle = string2txt(texte, LG_TEXTE);
		String sommeTxt = somme2txt(somme, LG_SOMME);
		return libelle + sommeTxt;
	}

	/** Obtenir une chaîne de largeur caractères qui contient un texte aligné à
	 * gauche.
	 * @param texte le texte à aligner à gauche
	 * @param largeur le nombre de caractères de la chaîne résultat
	 */
	//@ requires texte != null;
	//@ requires largeur > 0;
	//@ ensures \result.length() == largeur;
	private static String string2txt(String texte, int largeur) {
		return (texte + BLANC).substring(0, LG_TEXTE);
	}


	/** Obtenir une chaîne de caractères représentant la somme sur largeur
	 * caractères. Des caractères blancs sont ajoutés devant la somme pour
	 * atteindre largeur.  Si la somme est trop grande, elle est tronquée.
	 * Si largeur est nulle, somme est affichée sur le nombre de caractères
	 * nécessaires.
	 * @param somme la somme à traduire
	 * @param largeur le nombre de caractères pour traduire somme
	 */
	//@ requires largeur >= 0;
	//@ requires largeur <= SEPARATION.length();
	//@ requires somme >= 0;
	//@ ensures largeur != 0 ==> \result.length() == largeur;
	private static String somme2txt(long somme, int largeur) {
		String texte = ""
					+ somme / 100	// partie entière
					+ "."
					+ somme % 100 / 10	// décimes
					+ somme % 10;	// centimes
		String result = texte;
		if (largeur > 0) {
			String txt = BLANC + texte;
			result = txt.substring(txt.length()-largeur, txt.length());
		}
		return result;
	}

	/** Obtenir le ticket.
	 * @return le ticket
	 * @throws RuntimeException si aucun numéro a été défini ou s'il n'y a pas
	 * d'articles.
	 */
	public Ticket getTicket() {
		if (this.numero == 0) {
			throw new RuntimeException("Le numéro de ticket n'est pas défini !");
		}
		if (this.lignes.size() == 0) {
			throw new RuntimeException("Pas d'articles !");
		}

		String texte = "";
		int nbArticles = 0;		// nombre total d'articles sur le ticket
		int totalTicket = 0;	// prix total du ticket

		// Afficher l'entête
		texte += SEPARATION + '\n';
		texte += string2txt("   Ticket numéro " + this.numero, LG_TEXTE) + '\n';
		texte += SEPARATION + '\n';

		// Afficher les articles du ticket
		for (Ligne l : this.lignes) {
			long prixLigne = l.prixTotal();
			String texteLigne = "  " + l.libelle();
			if (l.quantite() == 1) {
				texte += ligne2String(texteLigne, prixLigne) + '\n';
			} else {
				String quantiteTexte = "        " + l.quantite() + " x "
					+ somme2txt(l.prixUnitaire(), 0);
				texte += string2txt(texteLigne, LG_TEXTE) + '\n';
				texte += ligne2String(quantiteTexte, prixLigne) + '\n';
			}
			nbArticles += l.quantite();
			totalTicket += prixLigne;
		}

		// Afficher le total
		texte += '\n';
		texte += ligne2String(" Total " + nbArticles + " articles :", totalTicket) + '\n';
		texte += SEPARATION + '\n';
		texte += '\n';

		return new Ticket(texte, totalTicket);
	}

}
