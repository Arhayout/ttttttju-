package supermarche.articles;
import org.junit.*;
import static org.junit.Assert.*;

/**
  * Classe de test  de la classe BArticleSimple.
  * @author	RHAYOUTE ABDELMALEK.
  */
public class ArticleSimpleTest {

	// Les articles
	private ArticleSimple A0 , A2 ;


	@Before public void setUp() {
		// Construire les articles
                A0 = new ArticleSimple(132, "Lait 1L");
		A2 = new ArticleSimple(102, "Coca");
	};
	@Test public void testerPrix() {
		assertEquals("erreur au  constructeur ou la méthode prix ", A0.prix () , 132);
         }
         
	@Test public void testerLebelle() {
		assertEquals("erreur au  constructeur ou la méthode libelle", A2.libelle () , "Coca");
        }
}
