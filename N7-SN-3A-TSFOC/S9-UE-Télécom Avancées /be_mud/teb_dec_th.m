function teb = teb_dec_th(A,R,Nusers,alpha,SNR);

% *** calcule le TEB théorique pour chacun des 2 utilisateurs     ***
% *** pour une détection multi-utilisateur CDMA par décorrélateur ***
% *** on utilise une formule exacte                               ***

sigma2  = sum(A.^2*alpha^2)*10^(-SNR/10);
inv_R   = inv(R);
d_inv_R = diag(inv_R);
d_inv_R = d_inv_R(:);

teb = 1-normcdf(A(:)./sqrt(sigma2*d_inv_R));
