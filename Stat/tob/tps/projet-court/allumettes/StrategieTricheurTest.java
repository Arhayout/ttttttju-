package allumettes;

import org.junit.*; 
import static org.junit.Assert.*;


public class StrategieTricheurTest {

         private StrategieTricheur StrategieTricheur;
     
         @Before 
            public void setUp() {
             	StrategieTricheur = new StrategieTricheur();
            }

         @Test
            public void test() {
                int i;
		        for (i = 3; i <= 13; i++) {
		            Jeu jeu = new JeuImplante(i);
		            assertTrue(StrategieTricheur.getPrise(jeu) == 1 );
		           
		         }
	      }

		public static void main(String[] args)  {
			new StrategieTricheurTest();
		}


}


