import java.awt.Color;
import org.junit.*;
import static org.junit.Assert.*;
import java.awt.Color;
import org.junit.*;
import static org.junit.Assert.*;

/**
 * @author	abdelmalek rhayoute
 */

public class ComplementsCercleTest {

  // précision pour les comparaisons réelle
  public final static double EPSILON = 0.001;

  // Les points du sujet
  private Point A, B, C, D, E, F, G;

  // Les cercles du sujet
  private Cercle C1, C2, C3;

  @Before public void setUp() {
    // Construire les points
    A = new Point(1, 1);
    C = new Point(0, 0);
    D = new Point(2, 0);
    E = new Point(1, 0);

    // Construire les cercles
    C1 = new Cercle(A, 2);
    C2 = new Cercle(C, 1);
    C3 = new Cercle(D, 1);
    C1.setCouleur(Color.blue);
  }

  /** Vérifier si deux points ont mêmes coordonnées.
   * @param p1 le premier point
   * @param p2 le deuxième point
   */
  static void memesCoordonnees(String message, Point p1, Point p2) {
    assertEquals(message + " (x)", p1.getX(), p2.getX(), EPSILON);
    assertEquals(message + " (y)", p1.getY(), p2.getY(), EPSILON);
  }

  //Verifier si un point sur la circonference appartient a la cercle
  @Test public void testerE5() {
    B = new Point(C1.getCentre().getX() + C1.getRayon(), C1.getCentre().getY());
    memesCoordonnees("E5 : Centre de C1 incorrect", A, C1.getCentre());
    assertEquals("E5 : Rayon de C1 incorrect",
      2, C1.getRayon(), EPSILON);
    assertEquals(Color.blue, C1.getCouleur());
    assertTrue("E5", C1.contient(B));
  }

  //E20 : Verifier le point d'intersection entre deux cercles
  @Test public void testerE20() {

    //Pour ce point on a fait une translation positive de centre de distance de rayon
    F = new Point(C2.getCentre().getX() + C2.getRayon(), 0);
    //Pour ce point on a fait une translation négative de centre de distance de rayon   
    G = new Point(C3.getCentre().getX() - C3.getRayon(), 0);
    memesCoordonnees("E20 : Centre de C1 incorrect", E, F);
    memesCoordonnees("E20 : Centre de C1 incorrect", E, G);

  }

}
