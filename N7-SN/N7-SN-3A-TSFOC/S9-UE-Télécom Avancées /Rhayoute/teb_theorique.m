function [teb_fa,teb_dec,teb_mmse] = teb_theorique(A,R,SNR);

% *** programme teb_theorique ***

% *** Nb d'utilisateurs ***
K     = 2;
alpha = 1;

for ns=1:length(SNR),
   teb_fa(:,ns)   = teb_fa_th(A,R,K,alpha,SNR(ns));
   teb_dec(:,ns)  = teb_dec_th(A,R,K,alpha,SNR(ns));
   teb_mmse(:,ns) = teb_mmse_th(A,R,K,alpha,SNR(ns));
end;

