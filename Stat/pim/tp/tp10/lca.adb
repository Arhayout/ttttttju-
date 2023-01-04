with Ada.Text_IO;            use Ada.Text_IO;
with Ada.Unchecked_Deallocation;

package body LCA is

	procedure Free is
		new Ada.Unchecked_Deallocation (Object => T_Cellule, Name => T_LCA);


	procedure Initialiser(Sda: out T_LCA) is
	begin
		Sda:=null;
	end Initialiser;


        function Est_Vide (Sda : T_LCA) return Boolean is
        begin
		return Sda=null;
	end;


	function Taille (Sda : in T_LCA) return Integer is
        begin
            while Sda/=null loop
               return 1+Taille(Sda.all.suivant);
            end loop;
            return 0;

	end Taille;


        procedure Enregistrer (Sda : in out T_LCA ; Cle : in T_Cle ; Donnee : in T_Donnee) is

        begin
           if sda=null then
              sda := new t_cellule;
              sda.all.cle:=cle;
              sda.all.donnee:=Donnee;


           elsif
              sda.all.cle=Cle then
              sda.all.donnee:=Donnee;
           else
              Enregistrer(sda.all.suivant,Cle,Donnee );

           end if;

        end Enregistrer;


	function Cle_Presente (Sda : in T_LCA ; Cle : in T_Cle) return Boolean is
        begin

            if sda=null then
               return false;
            elsif sda.all.cle=cle then
                return true;
            else
                  return Cle_Presente(sda.all.suivant,cle);
            end if;



        end Cle_Presente;


        function La_Donnee (Sda : in T_LCA ; Cle : in T_Cle) return T_Donnee is
           l: T_LCA;
        begin
        l:=sda;
            if not Cle_Presente(sda , Cle) then
                raise Cle_Absente_Exception;
            else
                while l.all.cle /= cle loop
                    l := l.all.suivant;
                end loop;
                return l.all.donnee;
            End if;
        end La_Donnee;


        procedure Supprimer (Sda : in out T_LCA ; Cle : in T_Cle) is
            sda_sup: T_LCA;
	begin
	if not Cle_Presente(Sda , Cle) then
               raise Cle_Absente_Exception;
        else
               Initialiser(sda_sup);
               sda_sup:=sda;
               Vider(sda); -- Vous perdez tout avec cette etape !
               while sda_sup.all.cle /= cle loop
                     Enregistrer(sda,sda_sup.all.cle,sda_sup.all.donnee);
                     sda_sup:= sda_sup.all.suivant;
               end loop;
               sda.all.suivant:=sda_sup.all.suivant;
	end if;
      end Supprimer;


	procedure Vider (Sda : in out T_LCA) is
        begin
                if Sda = Null then -- KJR : Ce if est inutile ici - while fait le test.
                        null;
                end if;
                while Sda /= Null loop
                        Supprimer(sda,sda.all.cle);

                end loop;
        end Vider;


        procedure Pour_Chaque (Sda : in T_LCA) is
           l:T_LCA;
        begin
                  l:=sda;
                  while l/=null loop
                     traiter(l.all.cle,l.all.Donnee);
                     l:=l.all.suivant;
	          end loop;
        end Pour_Chaque;


end LCA;
