with ada.Text_IO;               use Ada.Text_IO;
with Ada.Integer_Text_IO;       use Ada.Integer_Text_IO;
with Ada.Strings.Unbounded;     use Ada.Strings.Unbounded;
with SDA_Exceptions;            use SDA_Exceptions;
with TH;

procedure TH_sujet is
    Function Hachage_nbr_cle(Cle: Unbounded_String) return Integer is
    begin
        return To_String(Cle)'Length;
    end Hachage_nbr_cle;

    package Th_string_integer is new TH(T_Cle=>Unbounded_String,T_donnee=>Integer, fonction_Hachage=>Hachage_nbr_cle, Capacite=>11);
    use Th_string_integer;

   function guillemet(s:unbounded_String)return String is
   begin
      return'"'&To_String(s) &'"';
   end;
   procedure Afficher (S : in Unbounded_String; N: in Integer) is
   begin
        Put (Guillemet (S));
        Put (" : ");
        Put (N, 1);
        New_Line;
   end Afficher;
   procedure Afficher is new Pour_Chaque_th (Traiter => Afficher);
       Th : T_TH;
   begin
   Initialiser_th(th);
   Enregistrer_th (th, To_Unbounded_String("un"), 1);
   Enregistrer_th (th, To_Unbounded_String("deux"), 2);
   Enregistrer_th (th, To_Unbounded_String("trois"), 3);
   Enregistrer_th (th, To_Unbounded_String("quatre"), 4);
   Enregistrer_th (th, To_Unbounded_String("cinq"), 5);
   Enregistrer_th (th, To_Unbounded_String("quatre-vingt-dix-neuf"), 99);
   Enregistrer_th (th, To_Unbounded_String("vingt-et-un"), 21);
   Afficher(th);
end Th_sujet;
