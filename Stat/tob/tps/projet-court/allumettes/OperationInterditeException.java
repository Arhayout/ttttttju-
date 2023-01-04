package allumettes;

public class OperationInterditeException extends RuntimeException {


        //cette operation détecte s'il y a un triche
	public OperationInterditeException(String message) {
		super(message);
	}

}
