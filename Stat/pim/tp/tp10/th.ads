with SDA_Exceptions; use SDA_Exceptions;
with LCA;

-- Définition de structures de données associatives sous forme d'une liste
-- chaînée associative (LCA).
generic
   type T_Cle is private;
   type T_Donnee is private;
   Capacite :Integer;
   with function fonction_Hachage (
         Cle : in  T_Cle )
     return Integer;
package TH is

	type T_TH is limited private;
Package T_lca_private is
        new lca(T_Cle =>T_cle, T_Donnee =>T_Donnee );
use T_lca_private;

	-- Initialiser une Sda.  La Sda est vide.
	procedure Initialiser_th(Sda: out T_TH) with
		Post => Est_Vide_th (Sda);


	-- Est-ce qu'une Sda est vide ?
	function Est_Vide_th (Sda : T_TH) return Boolean;


	-- Obtenir le nombre d'éléments d'une Sda.
	function Taille_th (Sda : in T_TH) return Integer with
		Post => Taille_th'Result >= 0
			and (Taille_th'Result = 0) = Est_Vide_TH (Sda);


	-- Enregistrer une Donnée associée à une Clé dans une Sda.
	-- Si la clé est déjà présente dans la Sda, sa donnée est changée.
	procedure Enregistrer_th (Sda : in out T_TH ; Cle : in T_Cle ; Donnee : in T_Donnee) with
		Post => Cle_Presente_th (Sda, Cle)
			and then (La_Donnee_th (Sda, Cle) = Donnee)			-- donnée insérée
			-- and then (if not (Cle_Presente (Sda, Cle)'Old) then Taille (Sda) = Taille (Sda)'Old)
			-- and then (if Cle_Presente (Sda, Cle)'Old then Taille (Sda) = Taille (Sda)'Old + 1)
			;


	-- Supprimer la Donnée associée à une Clé dans une Sda.
	-- Exception : Cle_Absente_Exception si Clé n'est pas utilisée dans la Sda
	procedure Supprimer_TH (Sda : in out T_TH ; Cle : in T_Cle) with
		Post =>  Taille_TH (Sda) = Taille_TH (Sda)'Old - 1 -- un élément de moins
			and not Cle_Presente_th (Sda, Cle);         -- la clé a été supprimée


	-- Savoir si une Clé est présente dans une Sda.
	function Cle_Presente_th (Sda : in T_TH ; Cle : in T_Cle) return Boolean;


	-- Obtenir la donnée associée à une Cle dans la Sda.
	-- Exception : Cle_Absente_Exception si Clé n'est pas utilisée dans l'Sda
	function La_Donnee_th (Sda : in T_TH ; Cle : in T_Cle) return T_Donnee;


	-- Supprimer tous les éléments d'une Sda.
	procedure Vider_th (Sda : in out T_TH) with
		Post => Est_Vide_th (Sda);


	-- Appliquer un traitement (Traiter) pour chaque couple d'une Sda.
	generic
		with procedure Traiter (Cle : in T_Cle; Donnee: in T_Donnee);
	procedure Pour_Chaque_th (Sda : in T_TH);

private
type T_Tab_Elements is array (1..Capacite) of T_lca_private.T_LCA;
type T_TH is
        record
            Elements : T_Tab_Elements;
        end record;



end TH;
