clear;
close all;
load donneesCouleur;
load exercice;
figure('Name','Image tiree aleatoirement','Position',[0.2*L,0.2*H,0.6*L,0.5*H]);
% Seuil de reconnaissance a regler convenablement
s = 0.5;

% Pourcentage d'information
per = 0.95;

% Tirage aleatoire d'une image de test :
individu = randi(37);
posture = randi(6);
chemin = './Images_Projet_2020';
fichier = [chemin '/' num2str(individu+3) '-' num2str(posture) '.jpg'];
Im=importdata(fichier);
%I=rgbgray(Im);
I=im2double(Im);
image_test=I(:)';


% Affichage de l'image de test :
colormap gray;
imagesc(I);
axis image;
axis off;

% Nombre N de composantes principales
N = 8;

% Composantes principales des donnees d'apprentissage
C = X_centre*Vecteur_propre_normalise;

% N premieres composantes principales des images d'apprentissage :
Donnees_image = C( : , 1:N );

% N premieres composantes principales de l'image de test :
image_test = image_test - individu_moyen;
Donnees_test = image_test* Vecteur_propre_normalise;
Donnees_test = Donnees_test( : , 1:N );

% Determination de la liste des classes :
ListeClasse = 1:37;

%Calcul des 3 plus proches voisins en s'inspirant du méthode kppv 
k = 3;
%définir les labels images
labels = repmat(numeros_individus, nb_postures, 1);
%récupérer le résultat du requete, la distance minimale entre le résultat
%et lindividu, la matrice des données relatives au résultat, les indices 
%des K + proches voisins, et le taux d'erreur déduit de la matrice de
%confusion
[individu_reconnu_image,distance_min,Donnees_nouvelles, kppv,taux]= kppv(Donnees_image,Donnees_test,labels,k,ListeClasse,nb_postures);

% Affichage du resultat :
if distance_min < s
    individu_reconnu = individu_reconnu_image;
    % Affichage de l'image requête
figure('Name',"FIGURE 3- Résultat d'une requête sur une base de visages",'Position',[0.2*L,0.2*H,0.6*L,0.5*H]);
subplot(1, k + 1, 1);
colormap gray;
imagesc(I);
axis image;
title("Requête");

for i = 1:k
    subplot(1, k+1, i+1);
    fichier = [chemin '/' num2str(Donnees_nouvelles(i, 1) + 3) '-' num2str( Donnees_nouvelles(i, 2) ) '.jpg'];
    Im = importdata(fichier);
    %I = rgb2gray(Im);
    I = im2double(Im);
    imagesc(I);
    axis image;
    title("Trouvée - choix " + i);
end
else
    title({['Posture numero ' num2str(posture) ' de l''individu numero ' num2str(individu+3)];...
        'Je ne reconnais pas cet individu !'},'FontSize',20);
end

taux_erreur = taux/100