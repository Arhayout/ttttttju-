function [y,bits] = signal(signatures,A,sigma2);

% *** Nb d'utilisateurs ***
A      = A(:);
Nusers = size(A,1);

% *** generation des bits +1/-1 ***
bits = rand(Nusers,1)<0.5;
bits = 2*bits-1;

% *** génération du signal ***
y = zeros(Nusers,size(signatures,2));
for k=1:Nusers,
   % *** mise en forme des bits par les signatures données ***
   xk     = kron(bits(k),signatures(k,:));
   % *** multiplication par l'amplitude ***
   y(k,:)     = A(k)*xk;
end;

% *** signal total = somme des signaux des utilisateurs ***
if Nusers>1,
   y     = sum(y);
end;

% *** Ajout de bruit ***
bruit  = sqrt(sigma2)*randn(size(signatures,2),1);
y      = y(:)+bruit;

