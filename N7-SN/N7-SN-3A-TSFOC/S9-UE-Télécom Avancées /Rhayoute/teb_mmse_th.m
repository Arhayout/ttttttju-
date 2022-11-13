function teb = teb_mmse_th(A,R,Nusers,alpha,SNR);

% *** calcule le TEB théorique pour chacun des 2 utilisateurs ***
% *** pour une détection multi-utilisateur CDMA par MMSE      ***
% *** on utilise une formule exacte                           ***

sigma2 = sum(A.^2*alpha^2)*10^(-SNR/10);
C      = inv(R+sigma2*diag(1./(A.^2)));

if Nusers == 2,
   CR       = C*R;
   CRC      = CR*C;
   sig      = sqrt(sigma2);
   teb(1,1) = 0.5*( (1-normcdf( (A(1)*CR(1,1)+A(2)*CR(1,2)) /(sig*sqrt(CRC(1,1)))))+...
      (1-normcdf( (A(1)*CR(1,1)-A(2)*CR(1,2)) /(sig*sqrt(CRC(1,1))))) );
   teb(2,1) = 0.5*( (1-normcdf( (A(2)*CR(2,2)+A(1)*CR(2,1)) /(sig*sqrt(CRC(2,2)))))+...
      (1-normcdf( (A(2)*CR(2,2)-A(1)*CR(2,1)) /(sig*sqrt(CRC(2,2))))) );
end;

