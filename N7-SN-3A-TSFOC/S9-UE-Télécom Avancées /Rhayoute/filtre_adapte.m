function y = filtre_adapte(y_cont,signatures);

% *** renvoie pour chaque utilisateur la sortie des filtres adapt�s      ***
% *** ��d, calcule pour chaque utilisateurs les produits scalaires entre ***
% *** le signal "continu" re�u et la forme d'onde rectangulaire d�cal�e  ***
% *** on obtient pour chaque user un vecteur de longueur Ns*Nbits        ***

y = signatures*y_cont(:);

