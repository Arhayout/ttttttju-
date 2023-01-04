package supermarche;
import java.util.Collection;
import java.util.Iterator;
import java.util.ArrayList;//on aura besoin d'un tableau dynamique pour modéliser le caddie
/** Un caddie conserve les références des articles ajoutés par le client.
 * Les articles peuvent bien sûr être retirés.
 *
 * @author	Xavier Crégut et abdelmalek rhayoute
 */
public class Caddie implements Iterable<String> {

	/** Les références des produits ajoutés à ce caddie. */
	private Collection<String> references;
        public Caddie () { this.references = new ArrayList<>();
		}

	/** Ajouter une nouvelle référence au caddie.
	 * @param uneReference la référence à ajouter
	 */
	public void ajouter(String uneReference) {
         references.add(uneReference);
	}

	/** Supprimer du caddie une seule occurrence de la référence.
	 * Si la référence n'est pas dans le caddie, le caddie reste
	 * inchangé.
	 * @param uneReference la référence à supprimer
	 */
	public void enlever(String uneReference) {
		if (references.contains(uneReference)){
		    references.remove(uneReference);
			}
		else {
		        }
        
	}

	/** Supprimer du caddie toutes les occurrences de la référence.
	 * Si la référence n'est pas dans le caddie, le caddie reste
	 * inchangé.
	 * @param uneReference la référence à supprimer
	 */
	public void enleverTous(String laReference) {
		while (references.contains(laReference)){
		      references.remove(laReference);
		}
        }
        @Override
	public Iterator<String> iterator() {
		return references.iterator();
	}
}
