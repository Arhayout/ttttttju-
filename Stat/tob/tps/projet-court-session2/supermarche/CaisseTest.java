package supermarche;
import supermarche.articles.*;
import org.junit.*;
import static org.junit.Assert.*;

/**
  * Classe de test  de la classe .
  * @author	RHAYOUTE ABDELMALEK.
  */
public class CaisseTest {
            //précision
             public static final double EPSILON = 1e-1;

     	    // Les articles
            private static final ArticleSimple A0 =  new ArticleSimple(132, "Lait 1L");
	    private static final ArticleCompose A1 = new ArticleCompose(500, "Lait 6L", A0 , 6);
	    private static final  ArticleSimple A2 = new ArticleSimple(102, "Coca");
	    //autres
	    private BaseArticles B;
	    private Caddie caddie;
	    private Caisse caisse;


	@Before public void setUp() {
		B= new BaseArticles() ;
		B.enregistrer ("3370" , A0);
		B.enregistrer ("3371" , A1);
		B.enregistrer ("3372" , A2);
        caddie = new Caddie();
        caddie.ajouter("3371");
        caddie.ajouter("3370");
        caddie.ajouter("3372");
        caddie.ajouter("3370");
        caisse = new Caisse(B);
	};
	@Test public void testScanner() throws ReferenceInconnueException {
                caisse.scanner(caddie);
                assertEquals("erreur dans la méthode scanner"  , 1 , caisse.nbTicketsTraites());
                assertEquals("erreur dans la méthode scanner", 866 , caisse.total() , EPSILON);
          }

    @Test public void testImprimerReferencesInconnues() throws ReferenceInconnueException {
                caddie.ajouter("3374");
                caddie.ajouter("3375");
                caisse.scanner(caddie);
                caisse.imprimerReferencesInconues();
         }
}
