with Ada.Text_IO;            use Ada.Text_IO;
with SDA_Exceptions;         use SDA_Exceptions;
with LCA;

package body TH is
	procedure Initialiser_th(Sda: out T_TH) is
	begin
             for i in 1..capacite loop
		Initialiser(sda.elements(i));
             end loop;
	end Initialiser_th;

   function Taille_th (Sda : in T_th) return Integer is
        t:integer;
        begin
            t:=0;
            for i in 1..capacite loop
                t:=t+taille(sda.elements(i));
            end loop;
            return t;


	end Taille_th;


        function Est_Vide_th (Sda : T_th) return Boolean is
           l:  Boolean;
        begin
        l:=True;
        for i in 1..Capacite loop
            if not (Est_Vide(sda.elements(i))) then
                l:=False;
            end if;
        end loop;
        return l;

        end Est_Vide_th;

        function cle_div(cle:in t_cle) return integer is
        begin
              return ((fonction_Hachage(cle)-1)mod capacite)+1;
        end cle_div;

        procedure Enregistrer_th (Sda : in out T_TH ; Cle : in T_Cle ; Donnee : in T_Donnee) is
        begin
           Enregistrer(Sda.elements(cle_div(Cle)), Cle,donnee);
        end Enregistrer_th;


	function Cle_Presente_th (Sda : in T_TH; Cle : in T_Cle) return Boolean is
        a:integer;
        begin
            a:=cle_div(cle);
            return Cle_Presente(sda.elements(a),cle);

      end Cle_Presente_th;


        function La_Donnee_th (Sda : in T_TH ; Cle : in T_Cle) return T_Donnee is
        begin
                return La_Donnee(sda.elements(cle_div(cle)),cle);
        end La_Donnee_th;


        procedure Supprimer_th (Sda : in out T_TH ; Cle : in T_Cle) is
	begin

                   Supprimer(sda.elements(cle_div(cle)),cle);
	end Supprimer_th;


	procedure Vider_th(Sda : in out T_TH) is
	begin
		FOR i in 1..capacite loop
                    vider(sda.elements(i));
                end LOOP;
	end Vider_th;
        procedure Pour_Chaque_th (Sda : in T_TH) IS
        procedure pour_chaque_lca is new Pour_Chaque(traiter);
        begin
             FOR i in 1..capacite loop
                    pour_chaque_lca((sda.elements(i)));
             end LOOP;
        end Pour_Chaque_th;


end TH;
