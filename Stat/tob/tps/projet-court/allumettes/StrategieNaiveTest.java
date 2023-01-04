package allumettes;

import org.junit.*; 
import static org.junit.Assert.*;


public class StrategieNaifTest {

         private StrategieNaif StrategieNaif;
     
         @Before 
            public void setUp() {
             	StrategieNaif = new StrategieNaif();
            }

         @Test
            public void test() {
                int i;
		        for (i = 1; i <= 13; i++) {
		            Jeu jeu = new JeuImplante(i);
		            if (i == 1) {
		                assertTrue(StrategieNaif.getPrise(jeu) == i);
		            } else {
		                assertTrue(StrategieNaif.getPrise(jeu) >= 1 && StrategieNaif.getPrise(jeu) <= 3);
		            }
		         }
	      }

		public static void main(String[] args)  {
			new StrategieNaifTest();
		}


}


