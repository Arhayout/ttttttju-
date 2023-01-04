package supermarche.articles;
import org.junit.*;
import static org.junit.Assert.*;

/**
  * Classe de test  de la classe BaseArticles.
  * @author	RHAYOUTE ABDELMALEK.
  */
public class BaseArticlesTest {


	// Les articles
	private ArticleSimple A0 , A2 ;
        private ArticleCompose A1;

	// une base D'article
	private BaseArticles B , C;

	@Before public void setUp() {
		// Construire les articles
                A0 = new ArticleSimple(132, "Lait 1L");
		A1 = new ArticleCompose(500, "Lait 6L", A0 , 6);
		A2 = new ArticleSimple(102, "Coca");

		// Construire  les BaseArticles vides
		B= new BaseArticles() ;
		B.enregistrer ("3370" , A0);
		B.enregistrer ("3371" , A1);
		B.enregistrer ("3372" , A2);
		C= new BaseArticles() ;
		C.enregistrer ("3370" , A0);
		C.enregistrer ("3371" , A1);
	};
	@Test public void testerArticle() {
		assertEquals("reur dans la méthode article ou enregistrer ", A1 , B.article ("3371"));
         }
         
	@Test public void testerSupprimer() {
                B.supprimer("3370");
		assertFalse("erreur dans la méthode supprimer" , B.getArticles().containsKey("3370"));
        }
}
