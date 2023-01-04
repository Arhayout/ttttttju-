    #include <stdlib.h> 
    #include <stdio.h>
    #include <assert.h>
    #include <stdbool.h>
    #define TAILLE 5

    // Definition du type monnaie
    struct monnaie {
        int valeur;
        char devise;
    }; 
    typedef struct monnaie Monnaie;

    /**
     * \brief Initialiser une monnaie 
     * \param[]
     * \pre 
     * // TODO
     */ 
    void initialiser(Monnaie *m, float valeur, char devise) {
        assert(valeur > 0);
        m->valeur = valeur;
        m->devise = devise;
    }


    /**
     * \brief Ajouter une monnaie m2 à une monnaie m1 
     * \param[]
     * // TODO
     */ 
    bool ajouter(Monnaie* m1, Monnaie* m2) {
	    if (m1->devise == m2->devise) {
		    m2->valeur += m1->valeur;
		    return true;
        	} 
        	else { 
        		return false;
        	}
    }
    /**
     * \brief Tester Initialiser 
     * \param[]
     * // TODO
     */ 
    void tester_initialiser() {
        	Monnaie m1, m2;

        	initialiser(&m1, 12, 'e');
        	assert (12 == m1.valeur);
        	assert ('e' == m1.devise);

        	initialiser(&m2, 26, 'f');
        	assert (26 == m2.valeur);
        	assert ('f' == m2.devise);
       
	    printf("\n le test tester_initialiser est passé avec succés.\n");	
    }

    /**
     * \brief Tester Ajouter 
     * \param[]
     * // TODO
     */ 
    void tester_ajouter() {   
	    Monnaie m1, m2, m3;

            initialiser(&m1, 149, 'e');
            assert (149 == m1.valeur);
            assert ('e' == m1.devise);

            initialiser(&m2, 26, 'f');
            assert (26 == m2.valeur);
            assert ('f' == m2.devise);

	    assert (!ajouter(&m1, &m2));

	    initialiser(&m3, 24, 'f');
            assert (24 == m3.valeur);
            assert ('f' == m3.devise);

            assert (ajouter(&m2, &m3));
	    assert (26 == m2.valeur);
            assert ('f' == m2.devise);
	    assert (50 == m3.valeur);
            assert ('f' == m3.devise);

	    printf("\nle test tester_ajouter est passé avec succés.\n");
    }


    int main(void) {
        	// Déclaration des variables
        	int i;
        	float sum, valeur;
        	char devise;
       	
        	// Déclarer un tableau de 5 monnaies
       	Monnaie porte_monnaie[TAILLE];

	    //Initialiser les monnaies
        	for (i = 0 ; i < TAILLE ; i++){
		    printf("\nEntrer le nom de votre monnaie et la valeur séparées par une virgule : ");
		    scanf("%f,%c",&valeur,&devise);
	    
		    initialiser(&porte_monnaie[i], valeur, devise);
        	}
	    
	    // Afficher la somme de toutes les monnaies qui sont dans une devise entrée par l'utilisateur.
        printf("choix de la devise ? \n");
        char devise_somme;
        scanf(" %c",&devise_somme);
        	printf("\nAfficher la somme de toutes les monnaies qui sont dans une devise\nde votre choix.\n");

        	for (i = 0 ; i < TAILLE ; i++) {
		    if (porte_monnaie[i].devise == devise_somme){
	        		sum += porte_monnaie[i].valeur;
		    }
        	}
        	
	    printf("\nLa somme de toutes les monnaies qui sont en %c est %1.2f.\n", devise_somme, sum);
	    
    return EXIT_SUCCESS;
    }
