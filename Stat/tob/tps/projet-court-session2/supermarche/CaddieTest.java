package supermarche;
import org.junit.*;
import static org.junit.Assert.*;
import java.util.Iterator;
/**
  * Classe de test  de la classe Caddie.
  * @author	RHAYOUTE ABDELMALEK.
  */
public class CaddieTest {

    private static final String reference1 = "3370";
    private static final String reference2 = "3371";

    private Caddie caddie1 , caddie2;

    @Before public void setUp() {
                caddie1 = new Caddie();
                caddie2 = new Caddie();
                caddie2.ajouter(reference1);
                caddie2.ajouter(reference2);
                caddie2.ajouter(reference2);
    };

    @Test   public void testAjouter() {
                caddie1.ajouter(reference1);
                String reference = caddie1.iterator().next();
                assertEquals("erreur dans la méthode ajouter" , reference1 , reference);
        }

    @Test  public void testEnlever() {
                caddie2.enlever(reference2);
                Iterator<String> iterateur = caddie2.iterator();
                String ref1 = iterateur.next();
                String ref2 = iterateur.next();
                assertEquals("erreur dans la méthode enlever" ,reference1 , ref1);
                assertEquals("erreur dans la méthode enlever" ,reference2 , ref2);
                assertFalse("erreur dans la méthode enlever" , iterateur.hasNext());
            }

    @Test   public void testEnleverTous() {
                caddie2.enleverTous(reference2);
                Iterator<String> iterateur = caddie2.iterator();
                String reference = iterateur.next();
                assertEquals("erreur dans la méthode TousEnlever" , reference1, reference);
                assertFalse("erreur dans la méthode TousEnlever" ,iterateur.hasNext());
            }

 }
