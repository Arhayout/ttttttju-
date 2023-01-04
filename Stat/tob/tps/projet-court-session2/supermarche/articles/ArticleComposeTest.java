package supermarche.articles;
import org.junit.*;
import static org.junit.Assert.*;

/**
  * Classe de test  de la classe BarticleCompose.
  * @author	RHAYOUTE ABDELMALEK.
  */
public class ArticleComposeTest {

	// Les articles
	private ArticleCompose A1;
        private ArticleSimple A0 ;


	@Before public void setUp() {
        // Construire les articles
        
        A0 = new ArticleSimple(132, "Lait 1L");
        A1 = new ArticleCompose(500, "Lait 6L", A0 ,6);
        };
		
	@Test public void testerPrix() {
		assertEquals("erreur au  constructeur ou la méthode prix ", A1.prix () , 500);
         }
         
	@Test public void testerLibelle() {
		assertEquals("erreur au  constructeur ou la méthode libelle", A1.libelle () ,"Lait 6L");
        }
}
