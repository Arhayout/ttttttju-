package supermarche;
import supermarche.articles.ArticleSimple;
import supermarche.articles.BaseArticles;
import supermarche.articles.ArticleCompose;
import supermarche.articles.ReferenceInconnueException;
import supermarche.articles.*;

/** L'exemple du sujet.
 *
 * @author	Xavier Crégut et abdelmalek rhayoute
 */
public class ExempleCaisse {
	public static void main(String[] args) throws ReferenceInconnueException {
        BaseArticles baseArticles = new BaseArticles();
        ArticleSimple moutarde = new ArticleSimple(102, "Moutarde");
        ArticleSimple brocoli = new ArticleSimple(198, "Brocoli");
        ArticleSimple coulomier = new ArticleSimple(220, "Coulomier");
        ArticleSimple lait = new ArticleSimple(67, "Lait 1L");
        ArticleSimple lecteurDiVX = new ArticleSimple(6999, "Lecteur DiVX");
        ArticleCompose Laitpack = new ArticleCompose(365 , "Lait pack 6L" , lait  , 6 );
        // j'ai choisi d'écrire les références en lettres pour ne pas avoir des confusions initules
        baseArticles.enregistrer("moutarde", moutarde);
        baseArticles.enregistrer("brocoli", brocoli);
        baseArticles.enregistrer("coulomier", coulomier);
        baseArticles.enregistrer("lecteur", lecteurDiVX);
        baseArticles.enregistrer("lait", lait);
        baseArticles.enregistrer("packLait", Laitpack);
        Caddie caddie = new Caddie();
        caddie.ajouter("moutarde");
        caddie.ajouter("brocoli");
        caddie.ajouter("coulomier");
        caddie.ajouter("brocoli");
        caddie.ajouter("brocoli");
        caddie.ajouter("brocoli");
        caddie.ajouter("lecteur");
        caddie.ajouter("lait");
        caddie.ajouter("packLait");
        Caisse caisse = new Caisse(baseArticles);
        caisse.scanner(caddie);
    }

}

