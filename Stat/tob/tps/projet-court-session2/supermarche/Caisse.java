package supermarche;
import java.util.List;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import supermarche.articles.BaseArticles;
import supermarche.articles.Article;
import supermarche.articles.ReferenceInconnueException;

/** Une caisse produit un ticket pour chaque caddie qui se présente à elle.
 * Elle fournit des statistiques sur le nombre de tickets produits à le total
 * des sommes encaissées.
 *
 * @authorXavier Crégut et abdelmalek rhayoute
 */
public class Caisse {

	/** Obtenir le total des tickets traités.
	 * @return le total des tickets traités
	 */
        /** Un objet implémentant la classe Caisse va être caractérisé par les 4 attributs suivants
	*/
 	private long total;
	private BaseArticles articles;//La base d'articles sur laquelle la caisse sera basé pour traiter les articles
	private int numeroticket;//Il représente l'ordre du ticket, par exemple numeroticket pour l'exemple fourni au sujet est 1.
        private List<String> refrencesNonExistants ;//Chaque caisse doit être caractérisé par une liste des réferences qui n'existent pas dans la base des articles
        /** La construction de l'objet Caisse sera basé  la base des articles puisque il permet de gérer les articles présentés par le caddie

	*/
	public Caisse(BaseArticles articles) {
		this.total = 0;
                this.articles = articles;
		this.numeroticket = 0;
		this.refrencesNonExistants = new ArrayList<>();
        }
	public long total() {
		return total;
	}

	/** Obtenir le nombre de tickets traités.
	 * @return le nombre de tickets traités
	 */
	public int nbTicketsTraites() {
		return this.numeroticket;
	}

	/** Scanner tous les articles du caddie et éditer le ticket.
	 * @param caddie le caddie à scanner
	 */
	public void scanner(Caddie caddie) {
		this.numeroticket++;
                TicketBuilder ticketBuilder = new TicketBuilder();
                ticketBuilder.definirNumero(this.numeroticket);
                ArrayList<String> references = new ArrayList<>();
                for (String reference : caddie) {
		        try {
		            articles.article(reference);
		            references.add(reference);
		        }
		        catch (ReferenceInconnueException e) {
		           if (!refrencesNonExistants.contains(reference)) {
		               refrencesNonExistants.add(reference);
		           }
		        }
		}
               int s = 1;
	       for (int i = 0; i < references.size() - 1; i++) {
		    String reference = references.get(i);
		    String nextReference = references.get(i+1);
		    if (!reference.equals(nextReference)) {
		        Article article = articles.article(reference);
		        ticketBuilder.ajouterArticle(article.libelle(), article.prix(), s);
		        s = 1;
		    }
		    else {
		        s++;
		    }
		}
		Article finArticle = articles.article(references.get(references.size()-1));
		ticketBuilder.ajouterArticle(finArticle.libelle(), finArticle.prix(), 1);

		Ticket ticket = ticketBuilder.getTicket();
		total += ticket.getTotal();
		ticket.imprimer();
	        
        }
	/** Imprimer sur le terminal des références inconnues dans l'ordre
	 * alphabétique et sans redondance.
	 */
	public void imprimerReferencesInconues() {
		     refrencesNonExistants.sort(String::compareTo);
        	     refrencesNonExistants.forEach(System.out::println);

	}
  
}
