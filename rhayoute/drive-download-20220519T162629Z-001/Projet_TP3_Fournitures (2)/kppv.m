%--------------------------------------------------------------------------
% 
% TP4 - Reconnaissance de chiffres manuscrits par k plus proches voisins
% fonction kppv.m
%EN s'inspirant de la méthode kppv , cette fonction renvoie l'image de l'individu reconnu, la distance minimal
%avec l'individu recherché, la matrice contenant les résultats relative à
%cette requete, et les indices des k plus proche voisins
%--------------------------------------------------------------------------
function [individu_reconnu_image,distance_min,Donnees_nouvelles,kppv,taux_erreur] = kppv(DataA,DataT,labelA,k,ListeClass,nb)

[Na,~] = size(DataA);
[Nt,~] = size(DataT);
Partition = zeros(Nt,1);
for i=1:Nt
    % Calcul des distances entre les vecteurs de test 
    % et les vecteurs d'apprentissage (voisins)
    distance_euclidienne = sqrt(sum((DataA -  ones(Na,1)*DataT(i,:)).^2, 2));
    
    % On ne garde que les indices des K + proches voisins
    [~,Indices] = sort(distance_euclidienne,'ascend');
    kppv = Indices(1:k);
    %Vectorisation du vecteur labels des images tests
    labelA = labelA(:);
    labelA = labelA(kppv);

    % Comptage du nombre de voisins appartenant à chaque classe
    occurrences = histcounts(labelA, ListeClass);

    % Recherche de la classe contenant le maximum de voisins
    [apparition, maxIndice] = max(occurrences);

% Si l'image test a le plus grand nombre de voisins dans plusieurs
% classes différentes, alors on lui assigne celle du voisin le + proche,
% sinon on lui assigne l'unique classe contenant le plus de voisins
if length(find(occurrences == apparition)) > 1
    individu_reconnu_image = labelA(kppv(1));
else
    individu_reconnu_image = ListeClass(maxIndice);
end
%récupérer la distance minimale entre images et images test
distance_min = min(distance_euclidienne);
%Calcul de postures
postures = mod(kppv, nb).';
postures(postures == 0) = nb;
Donnees_nouvelles = [labelA postures'];
%resultats test
Partition(i) = individu_reconnu_image;
end
%Matrice de confusion
MatConfusion = zeros(Na,Na);
%Nombre_Erreur
nb_erreur = 0;
for i=1:Na
      for j=1:Na
          for k=1:Nt
              if (labelA(k) == i) && (Partition(k) == j)
                  nb_erreur = nb_erreur+1;
              end
          end
         MatConfusion(i,j) = nb_erreur;
      end
end
 %Taux_Erreur
   taux_erreur = 0;
for i=1:Na
    for j=1:Na
        if i~=j && MatConfusion(i,j)~=0
            taux_erreur = taux_erreur+1;
        end
    end
end
end

