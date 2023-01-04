package allumettes;

import org.junit.*; 
import static org.junit.Assert.*;


public class StrategieLenteTest {


         public  static final double EPSILON = 0.001;
         private Jeu jeu1, jeu2, jeu3, jeu4;
         private StrategieLente StrategieLente;
     
         @Before 
         public void setUp() {
         	jeu1 = new JeuImplante(1);
         	jeu2 = new JeuImplante(2);
         	jeu3 = new JeuImplante(10);
		    jeu4 = new JeuImplante(3);
         	StrategieLente = new StrategieLente();
         }

         @Test
         public void Test1(){
              assertEquals(1, StrategieLente.getPrise(jeu1),EPSILON);
		System.out.print(StrategieLente.getPrise(jeu1));
         }  
        
         @Test
         public void Test2(){
              assertEquals(1, StrategieLente.getPrise(jeu2),EPSILON);
		System.out.print(StrategieLente.getPrise(jeu2));
         }

         @Test
         public void Test3(){
              assertEquals(1, StrategieLente.getPrise(jeu3),EPSILON);
		System.out.print(StrategieLente.getPrise(jeu3));
         }

	 @Test
         public void Test4(){
              assertEquals(1, StrategieLente.getPrise(jeu4),EPSILON);
		System.out.print(StrategieLente.getPrise(jeu4));
         }

		public static void main(String[] args)  {
			new StrategieLenteTest();
		}


}


