clear all;
close all;

A = [1 1];
R = [1 0.1;0.1 1];
SNR = linspace (-5 , 15 ,50);
[teb_fa,teb_dec,teb_mmse] = teb_theorique (A ,R ,SNR);


 %first test
%k=1
figure
semilogy (SNR,teb_fa(1 , :))
hold on 
semilogy (SNR,teb_dec(1 , :))
hold on
semilogy (SNR,teb_mmse(1 , :))
hold off
title('Combine Plots of TEB k=1')
legend ("fa" , "dec" , "mmse")
%k=2
figure
semilogy (SNR,teb_fa(2 , :))
hold on 
semilogy (SNR,teb_dec(2 , :))
hold on
semilogy (SNR,teb_mmse(2, :))
hold off
title('Combine Plots of TEB k=2')
legend ("fa" , "dec" , "mmse")




%---------------------------
%Pour une corrélation faible, les courbes sont quasiment identiques.
% Il n' ya pas de différence entre utilsatuer 1 et 2 

%second test
R = [1 0.8 ;0.8 1];

[teb_fa,teb_dec,teb_mmse] = teb_theorique(A,R,SNR);

%k=1
figure
semilogy (SNR,teb_fa(1 , :))
hold on 
semilogy (SNR,teb_dec(1 , :))
hold on
semilogy (SNR,teb_mmse(1 , :))
hold off
title('Combine Plots of TEB k=1')
legend ("fa" , "dec" , "mmse")
%k=2
figure
semilogy (SNR,teb_fa(2 , :))
hold on 
semilogy (SNR,teb_dec(2 , :))
hold on
semilogy (SNR,teb_mmse(2, :))
hold off
title('Combine Plots of TEB k=2')
legend ("fa" , "dec" , "mmse")


 %---------------
 %Pour une corrélation forte avec un niveau de puissance identique, les
 %courbes commencent  à diverger entre eux, et le fa adapter devient une
 %droite même si le SNR augemente
 % Preque , il n' ya pas de différence entre utilsatuer 1 et 2 

%Third test

A = [1 10];
[teb_fa,teb_dec,teb_mmse] = teb_theorique(A,R,SNR);

%k=1
figure
semilogy (SNR,teb_fa(1 , :))
hold on 
semilogy (SNR,teb_dec(1 , :))
hold on
semilogy (SNR,teb_mmse(1 , :))
hold off
title('Combine Plots of TEB k=1')
legend ("fa" , "dec" , "mmse")
%k=2
figure
semilogy (SNR,teb_fa(2 , :))
hold on 
semilogy (SNR,teb_dec(2 , :))
hold on
semilogy (SNR,teb_mmse(2, :))
hold off
title('Combine Plots of TEB k=2')
legend ("fa" , "dec" , "mmse")
 
 % pour l'utlisateur 1 et fa, l'erreur augmente quand le SNR augememnte (vu en
 % cours ce cas quand le rapport est supperieur à l'inverse de corrélation
 % entre les deux  utilisaturs) ce qui contradictoire avec l'intuition
 % qu'on a en télécom
 % (dec ,mmse) reste inéchangé
 %l'utlistaeur k = 2 n'est pas influencé dans la detéction par fa !!!





