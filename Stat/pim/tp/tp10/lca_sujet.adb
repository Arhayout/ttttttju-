with ada.Text_IO;               use Ada.Text_IO;
with Ada.Integer_Text_IO;       use Ada.Integer_Text_IO;
with Ada.Strings.Unbounded;     use Ada.Strings.Unbounded;
with SDA_Exceptions;            use SDA_Exceptions;
with LCA;

procedure lca_sujet is
   package lca_string_integer is
     new LCA(T_Cle=>Unbounded_String,T_donnee=>Integer);
   use lca_string_integer;
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
   procedure Afficher_LCA is new Pour_Chaque (Traiter => Afficher);
       lca : T_LCA;
   begin
       Initialiser(lca);
       Enregistrer(lca , To_Unbounded_String("un"), 1);
       Enregistrer(lca , To_Unbounded_String("deux"), 2);
       Afficher_LCA(lca);
end lca_sujet;
