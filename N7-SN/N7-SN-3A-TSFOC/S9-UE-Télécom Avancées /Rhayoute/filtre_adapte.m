function y = filtre_adapte(y_cont,signatures);

% *** renvoie pour chaque utilisateur la sortie des filtres adaptés      ***
% *** çàd, calcule pour chaque utilisateurs les produits scalaires entre ***
% *** le signal "continu" reçu et la forme d'onde rectangulaire décalée  ***
% *** on obtient pour chaque user un vecteur de longueur Ns*Nbits        ***

y = signatures*y_cont(:);

