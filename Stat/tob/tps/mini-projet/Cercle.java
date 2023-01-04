import java.awt.Color;

/** Un cercle est une courbe plane fermée constituée des points
* Situés à égale distance d’un point nommé centre
* La valeur de cette distance est appeléerayondu cercle
* On appelle diamètre la distance entre deux points diamétralement opposés.
* @author RHAYOUTE Abdelmalek
*/
	public class Cercle implements Mesurable2D {
	// centre
	private Point centre;
	//rayon
	private double rayon;
	// couleur du cercle
	private Color couleur;
	//PI
	public final static double PI = Math.PI;
	
	/** Construire un cercle à partir d'un point et d'un rayon
	*  @param centre Centre du cercle
	*  @param rayon Rayon du cercle
	*/
		public Cercle(Point centre, double rayon){
		assert(centre != null && rayon > 0);
		this.centre = new Point (centre.getX(),centre.getY());;
		this.rayon = rayon;
		this.couleur = Color.BLUE;
	}
	
	/** Construire un cercle à partir de deux points diamétralement opposés
	* @param  point1 premier point.
	* @param  point2 deuxième point.
	*/
		public Cercle(Point point1, Point point2) {
		assert(point1!=null && point2!=null);
		assert(point1.getX() != point2.getX() || point1.getY()!=point2.getY());
		this.centre = new Point((point1.getX() + point2.getX())/2,(point1.getY() + point2.getY())/2);
		this.rayon  = point1.distance(point2)/2;
		this.couleur = Color.BLUE;
	}
	/** Construire un cercle à partir de deux points diamétralement opposés et sa couleur
	* @param  point1 premier point
	* @param  point2 2 ème point
	* @param  Color couleur
	*/
		public Cercle(Point point1, Point point2,Color Color) {
		this(point1,point2);
		assert Color!=null;
		this.couleur=Color;
		
	}
	
	/** Construire un cercle  à partir de deux points
	*@param centre  le premier correspond au centre du cercle
	*@param  point le deuxième est un point du cercle(de  sa  circonférence)
	*@return cercle
	*/
		public static Cercle creerCercle(Point centre, Point point){
		
		assert(centre !=  null && point != null && centre != point);
		assert(centre.getX() != point.getX() || centre.getY() != point.getY());
		return new Cercle(centre,point.distance(centre));
	}
	
	
	
	/** Obtenir le centre du cercle.
	* @return le centre du cercle.
	*/
		public Point getCentre() {
		double x = this.centre.getX();
		double y= this.centre.getY();
		Point centre = new Point(x,y);
		return centre;
	}
	
	
	/** Obtenir le rayon du cercle.
	* @return le rayon du cercle.
	*/
	//@ requires ensures \result>0;
		public double getRayon() {
		return this.rayon;
	}
	
	/** Obtenir le diametre du cercle.
	* @return le diametre du cercle.
	*/
		public double getDiametre() {
		return this.rayon * 2;
	}
	
	/** Obtenir la couleur du cercle.
	* @return la couleur du cercle
	*/
		public Color getCouleur() {
		return this.couleur;
	}
	
	/** Changer la couleur du cercle.
	* @param nouvelleCouleur nouvelle couleur
	*/
		public void setCouleur(Color color ) {
		assert color != null;
		this.couleur = color;
	}
	/** Changer le rayon du cercle .
	* @param r nouveau rayon.
	*/
	//@ requires r > 0;
		public void setRayon(double r) {
		assert r > 0;
		this.rayon= r;
	}
	
	
	/** Changer le diametre du cercle .
	* @param d nouveau rayon.
	*/
	//@ requires d > 0;
		public void setDiametre(double d) {
		assert d > 0;
		this.rayon= d / 2;
	}
	/** Translater le cercle.
	* @param dx déplacement suivant l'axe des X
	* @param dy déplacement suivant l'axe des Y
	*/
		public void translater(double dx, double dy) {
		this.centre.translater(dx,dy);
	}
	
	/** Changer le centre du cercle .
	* @param C le nouveau.
	*/
		public void setCentre(Point centre ) {
		assert centre != null;
		this.translater(centre.getX() , centre.getY());
	}
	
	/** savoir si un point est à l’intérieur (au sens large) d’un cercle.
	*@param p un point
	*@return un "boolean"
	*/
		public boolean contient(Point point) {
		assert(point != null);
		return (point.distance(this.centre)<=this.rayon);
	}
	
	/**obtenir le péri-mètre du cercle
	*il est donné par la formule 2&#960;R où R représente le rayon du cercle.
	*@return le péri-mètre du cercle
	*/
		public double perimetre() {
		return 2 * PI * this.rayon;
	}
	
	/**obtenir l'aire du cercle.
	*elle est donnée par la formule &#960;R^2 où R représente le rayon du cercle
	*@return l'aire du cercle
	*/
		public double aire() {
		return PI * Math.pow(this.rayon , 2);
	}
	
	/** forme d'affichage du cercle
	/*Afficher le cercle.
	*/
		public String toString() {
		String str = "C" + this.rayon + "@" + this.centre.toString();
		return str;
	}
		public void afficher(){
		System.out.print(this);
	}
	
}
