package supermarche.articles;


public class ArticleSimple  implements Article{
 /*on aura besoin de 2  attricuts: le prix de type long,et le libellé de type String*/
     private long prix;
     private String libelle;
     
     public ArticleSimple (long pr , String lib) { 
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
