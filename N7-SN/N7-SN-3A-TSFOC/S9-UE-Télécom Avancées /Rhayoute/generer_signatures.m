function [Sig,Rest] = generer_signatures(Nb_chip,R);

% [S,Rest]=GENERER_SIGNATURES(Nc,R) : renvoie 
%	S    : matrice des signatures normalis�es avec Nc chips
%         permettant d'obtenir une matrice
%         de corr�lation approximativement �gale � R
%         on prend 4 points par chip
%	Rest : matrice de corr�lation obtenue avec ces signatures

% *** Matrice de corr�lation approchant au mieux R ***
% *** �tant donn� le nb de chips                   *** 
corr   = floor(R*Nb_chip);
Nusers = size(R,1);

% *** Initialisation de la 1�re signature ***
Np       = 4;
sig      = zeros(Nusers,Nb_chip);
Sig      = zeros(Nusers,Nb_chip*Np);
sig(1,:) = sign(randn(1,Nb_chip));

for i=2:Nusers,
  
   l_p1=[];
   
   for k=1:Nb_chip
      
      % *** corr�lation actuelle entre les signatures ***
      c = corr(1:i-1,i)-sig(1:i-1,:)*sig(i,:)';
         
      % *** On fait baisser la correlation la plus �loign�e de la corr�lation souhait�e ***
      [cr p1] = max(abs(c));
      l_p1    = [l_p1 p1];
         
      % *** p est l'indice de la corr�lation la plus �loign�e ***
      sig(i,k) = sign(c(p1)+randn/10)*sig(p1,k);    %le randn sert � eviter que la signature soit nulle
   end;
      
end;

% *** Normalisation ***
for i=1:Nusers,
   Sig(i,:) = kron(sig(i,:),ones(1,Np));
   Sig(i,:) = Sig(i,:)/(norm(Sig(i,:)));
end;

% *** Matrice de Correlation ***
Rest = Sig*Sig';

