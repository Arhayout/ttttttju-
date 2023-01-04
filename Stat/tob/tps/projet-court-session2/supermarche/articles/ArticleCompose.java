package supermarche.articles;

public class ArticleCompose  implements Article {
 /**on a besoin de 2 attricuts: le prix de type long,et le libellé de type string
   */
     private long prix;
     private String libelle;
     /** la construction ArticleCompose sera basé :prix,libellle,l'article simple qui le compose ,quantite.
     */
     public ArticleCompose (long pr , String lib , ArticleSimple art , int nbr) { 
                assert pr <= art.prix() * nbr;
                this.prix = pr;
                this.libelle = lib;
      }
      public String libelle () {
            return this.libelle;
      }
      public long prix () {
            return this.prix;
      }
}
