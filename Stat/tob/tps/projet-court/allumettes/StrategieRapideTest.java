package allumettes;

import org.junit.*; 
import static org.junit.Assert.*;


public class StrategieRapideTest {


         public  static final double EPSILON = 0.001;
         private Jeu jeu1, jeu2, jeu3, jeu4;
         private StrategieRapide StrategieRapide;
     
         @Before 
         public void setUp() {
         	jeu1 = new JeuImplante(1);
         	jeu2 = new JeuImplante(2);
         	jeu3 = new JeuImplante(10);
		jeu4 = new JeuImplante(3);
         	StrategieRapide = new StrategieRapide();
         }

         @Test
         public void Test1(){
              assertEquals(1, StrategieRapide.getPrise(jeu1),EPSILON);
		System.out.print(StrategieRapide.getPrise(jeu1));
         }  
        
         @Test
         public void Test2(){
              assertEquals(2, StrategieRapide.getPrise(jeu2),EPSILON);
		System.out.print(StrategieRapide.getPrise(jeu2));
         }

         @Test
         public void Test3(){
              assertEquals(3, StrategieRapide.getPrise(jeu3),EPSILON);
		System.out.print(StrategieRapide.getPrise(jeu3));
         }

	 @Test
         public void Test4(){
              assertEquals(3, StrategieRapide.getPrise(jeu4),EPSILON);
		System.out.print(StrategieRapide.getPrise(jeu4));
         }

		public static void main(String[] args)  {
			new StrategieRapideTest();
		}


}


