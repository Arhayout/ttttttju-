package supermarche.articles;
import java.util.HashMap;
import java.util.Map;

/** BaseArticles enregistre et donne accès aux articles à partir de leur
 * référence.
 *
 * @author	Xavier Crégut et Abdelmalek Rhayoute
 */
public class BaseArticles {

	private Map<String, Article> articles;//donc on va modéliser les articles contenus dans BaseArticles par un clé de type string et la valeur sera un objet qui implement l'interface Articles

	/** Construire une base d'articles vide. */
	public BaseArticles() {
               this.articles = new HashMap<>();//on n'a pas ajouter de paramétres à HashMap puisque on veut construire une base d'Article vide et on modélise BaseArticles par un tableau de hashage  
	}

	//@ requires reference != null;
	//@ requires article != null;
	//@ ensures this.article(reference) == article;
	public void enregistrer(String reference, Article article) {
	
		assert(reference != null && article != null);
		
		/**la méthode put va ajouter un nouveau élément<string,Article> si il n'existe pas
		 *si la referance existe il va remplacer l'article par le nouvel
		 */     
                 articles.put(reference , article);                         
        
	}
	//@ requires reference != null;
	public void supprimer(String reference) throws ReferenceInconnueException{
         	assert(reference != null);
                if (articles.containsKey(reference)) {
			articles.remove(reference);
                }
                else { throw new ReferenceInconnueException();
                }
	}

	//@ requires reference != null;
        //@ signals_only ReferenceInconnueException;
	public Article article(String reference) throws ReferenceInconnueException {
		
		if (articles.containsKey(reference)) {
			return articles.get(reference);
                }
                else { throw new ReferenceInconnueException();
                }
		
	}

	public Map<String, Article> getArticles() {
		
		
			return this.articles;
		
	}
}
