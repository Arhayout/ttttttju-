package supermarche;

/** La classe Ticket modélise a minima un ticket. Seul appararaît son total et
 * une méthode pour imprimer.
 *
 * @author	Xavier Crégut
 */
public class Ticket {
	//@ public invariant getTotal() > 0;
	//@ private invariant getTotal() == total;

	/** Le texte du ticket. */
	private String texte;

	/** Le total du ticket. */
	private long total;

	/** Construire un ticket.
	 * @param texte le texte du ticket
	 * @param total le total du ticket
	 */
	//@ requires texte != null && texte.length() > 0;
	//@ requires total > 0;
	//@ ensures getTotal() == total;
	public Ticket(String texte, long total) {
		this.texte =texte;
		this.total = total;
	}

	/** Obtenir le total du ticket.
	 * @return le total du ticket
	 */
	//@ pure
	public long getTotal() {
		return this.total;
	}

	/** Imprimer le ticker... sur la sortie standard. */
	public void imprimer() {
		System.out.println(texte);
	}

	@Override public String toString() {
		return this.texte;
	}

}
