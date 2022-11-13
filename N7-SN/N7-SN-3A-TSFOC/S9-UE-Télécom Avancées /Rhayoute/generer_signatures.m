function [Sig,Rest] = generer_signatures(Nb_chip,R);

% [S,Rest]=GENERER_SIGNATURES(Nc,R) : renvoie 
%	S    : matrice des signatures normalisées avec Nc chips
%         permettant d'obtenir une matrice
%         de corrélation approximativement égale à R
%         on prend 4 points par chip
%	Rest : matrice de corrélation obtenue avec ces signatures

% *** Matrice de corrélation approchant au mieux R ***
% *** étant donné le nb de chips                   *** 
corr   = floor(R*Nb_chip);
Nusers = size(R,1);

% *** Initialisation de la 1ère signature ***
Np       = 4;
sig      = zeros(Nusers,Nb_chip);
Sig      = zeros(Nusers,Nb_chip*Np);
sig(1,:) = sign(randn(1,Nb_chip));

for i=2:Nusers,
  
   l_p1=[];
   
   for k=1:Nb_chip
      
      % *** corrélation actuelle entre les signatures ***
      c = corr(1:i-1,i)-sig(1:i-1,:)*sig(i,:)';
         
      % *** On fait baisser la correlation la plus éloignée de la corrélation souhaitée ***
      [cr p1] = max(abs(c));
      l_p1    = [l_p1 p1];
         
      % *** p est l'indice de la corrélation la plus éloignée ***
      sig(i,k) = sign(c(p1)+randn/10)*sig(p1,k);    %le randn sert à eviter que la signature soit nulle
   end;
      
end;

% *** Normalisation ***
for i=1:Nusers,
   Sig(i,:) = kron(sig(i,:),ones(1,Np));
   Sig(i,:) = Sig(i,:)/(norm(Sig(i,:)));
end;

% *** Matrice de Correlation ***
Rest = Sig*Sig';

