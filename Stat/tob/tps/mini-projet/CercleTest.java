import java.awt.Color;
import org.junit.*;
import static org.junit.Assert.*;

/**
 * Classe de test (incomplète) de la classe Cercle.
 * @author	RHAYOUTE ABDELMALEK.
 */
public class CercleTest {

  // précision pour les comparaisons réelle
  public final static double EPSILON = 0.001;

  // Les points du sujet
  private Point A, B, C, O;

  // Les cercles du sujet
  private Cercle C1, C2, C3;

  @Before public void setUp() {
    // Construire les points
    A = new Point(1, 1);
    B = new Point(3, 2);
    C = new Point(4, 9);

    // Construire les cercles
    C1 = new Cercle(A, B);
    C2 = new Cercle(A, C, Color.GREEN);
    C3 = Cercle.creerCercle(B, C);
  };

  /** Vérifier si deux points ont mêmes coordonnées.
   * @param p1 le premier point
   * @param p2 le deuxième point
   */
  static void memesCoordonnees(String message, Point p1, Point p2) {
    assertEquals(message + " (x)", p1.getX(), p2.getX(), EPSILON);
    assertEquals(message + " (y)", p1.getY(), p2.getY(), EPSILON);
  }
  /**On peut construire un cercle à partir de
   *  deux points diamétralement opposés
   * Sa couleur est considérée comme étant le bleu
   * Par exemple, le cercle C1 est construit à partir des deuxpoints A et B
   */
  @Test public void testerE12() {
    memesCoordonnees("E12 : Centre de C1 incorrect", new Point(2, 1.5), C1.getCentre());
    assertEquals("E12 : Rayon de C1 incorrect",
      Math.sqrt(5) / 2, C1.getRayon(), EPSILON);
    assertEquals(Color.blue, C1.getCouleur());
  }
  /**On peut construire un cercle à partir de 
   *deux points diamétralement opposés et 
   *de sa couleur
   */
  @Test public void testerE13() {
    memesCoordonnees("E13 : Centre de C2 incorrect", new Point(2.5, 5), C2.getCentre());
    assertEquals("E13 : Rayon de C2 incorrect",
      Math.sqrt(73) / 2, C2.getRayon(), EPSILON);
    assertEquals("E13 : Couleur de C2 incorrecte", Color.GREEN, C2.getCouleur());
  }

  /**Une méthode de classecreerCercle(Point, Point)
   * permet de créer un cercle à partir de deux points,
   *  le premier correspond au centre du cercle et
   *   le deuxième est un point du cercle(de  sa  circonférence)  
   * Ces deux  points  forment  donc  un  rayon  du  cercle 
   * Par  exemple,  le cercleC3 est construit à partir des points C(centre) et B(circonférence) 
   * Le cercle est bleu
   */
  @Test public void testerE14() {
    memesCoordonnees("E14 : Centre de C3 incorrect", B, C3.getCentre());
    assertEquals("E14 : Rayon de C3 incorrect",
      Math.sqrt(50), C3.getRayon(), EPSILON);
    assertEquals(Color.blue, C3.getCouleur());
  }
}
