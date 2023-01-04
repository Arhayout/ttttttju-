package supermarche.articles;

/** Un article a un prix et un libellé.
 * @author	Xavier Crégut
 */
public interface Article {

	//@ public invariant prix() >= 0;
	//@ public invariant libelle() != null && libelle().length() > 0;

	/** Le prix de l'article. */
	long prix();

	/** Libellé de l'article. */
	String libelle();

}
