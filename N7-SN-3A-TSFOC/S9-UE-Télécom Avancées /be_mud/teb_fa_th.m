function teb = teb_fa_th(A,R,Nusers,alpha,SNR);

% *** calcule le TEB théorique pour chacun des 2 utilisateurs        ***
% *** pour une détection multi-utilisateur CDMA par filtre adapté    ***
% *** on utilise une formule exacte                                  ***

sigma2 = sum(A.^2*alpha^2)*10^(-SNR/10);

if Nusers == 2,
   r        = abs(R(1,2));
   sig      = sqrt(sigma2);
   teb(1,1) = 0.5*((1-normcdf((A(1)-A(2)*r)/sig))+(1-normcdf((A(1)+A(2)*r)/sig)));
   teb(2,1) = 0.5*((1-normcdf((A(2)-A(1)*r)/sig))+(1-normcdf((A(2)+A(1)*r)/sig)));
end;

