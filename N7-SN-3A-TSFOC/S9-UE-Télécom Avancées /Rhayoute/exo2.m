clear all;
close all;

Nbits = 150000;
Nbits=Nbits;
%%
%%%%%% k=2
SNR_db=linspace(-5,5,50);
SNR=10.^(SNR_db/10);
K=2;
A1=1;
A2=10;
sigma2_vecteur=(A1^2+A2^2)./SNR;
p= 0.1;
A = diag([A1,A2]);
R = [1 p ; p 1];
SNR = linspace(-5 , 15 ,50);
b = 2*randi([0 1],2,Nbits) - 1;
sR = sqrtm (R);

for i=1:length(sigma2_vecteur)
sigma2=sigma2_vecteur(i);
n = sqrt (sigma2) * sR * randn (2,Nbits);
y = (R * A)*b + n;

%%%%%

b_dp(1,:) = sign(y(1,:)-sigma2/(2*A1)*log(cosh((A2*y(2,:)+A1*A2*0.8)/sigma2)/cosh((A2*y(2,:)-A1*A2*0.8)/sigma2)));

b_dp(2,:)= sign(y(2,:)-sigma2/(2*A2)*log(cosh((A1*y(1,:)+A1*A2*0.8)/sigma2)/cosh((A1*y(1,:)-A1*A2*0.8)/sigma2)));

%%%%%
b_fa = sign(y);
%%%%%
bdec = sign(inv(R)*y);

%%%%%
yfafa= y-p.*[A1,A2]*b_fa;
bfafa = sign(yfafa);
%%%%%%%
ydecfa= y-p.*[A1,A2]*bdec;
bdecfa=sign(ydecfa);
%%%%%%%%%
yfadec=y-p.*[A1,A2]*b_fa;
bfadec=sign(inv(R)*yfadec);
%%%%%%
ydecdec=y-p.*[A1,A2]*bdec;
bdecdec=sign(inv(R)*ydecdec);
TEB_dp(i)=length(find(b_dp(1,:)~=b(1,:)))/length(b);

TEB_fa(i)=length(find(b_dp(1,:)~=b(1,:)))/length(b);
TEB_dec(i)=length(find(bdec(1,:)~=b(1,:)))/length(b);
TEB_fafa(i)=length(find(bfafa(1,:)~=b(1,:)))/length(b);

TEB_decfa(i)=length(find(bdecfa(1,:)~=b(1,:)))/length(b);

TEB_fadec(i)=length(find(bfadec(1,:)~=b(1,:)))/length(b);

TEB_decdec(i)=length(find(bdecdec(1,:)~=b(1,:)))/length(b);
end

figure
semilogy(SNR,TEB_dp)
hold on
semilogy(SNR,TEB_fa)
hold on
semilogy(SNR,TEB_dec)
hold on
semilogy(SNR,TEB_fafa)
hold on
semilogy(SNR,TEB_decfa)
hold on
semilogy(SNR,TEB_fadec)
hold on
semilogy(SNR,TEB_decdec)
hold on
R_inv=inv(R);
fa_th=qfunc(A1./sigma2_vecteur);
dec_th= qfunc(A1./(sigma2_vecteur*sqrt(R(1,1))));
semilogy(SNR,fa_th)

hold on 
semilogy(SNR,dec_th)
hold off

title("TEB")
xlabel("SNR")
ylabel("TEB")
legend("Détecteur optimal","fa" ,"dec ","fafa","decfa","fadec","decdec","fath","decth")
%%%%%%%%%

%Question 3: observation de l'effet uniquement pour le filtre adapté
%%
K=3;
A3=1;
sigma2_vecteur=(A1^2+A2^2+A3^2)./SNR;
q= 0.3;
r=0.2;
A = diag([A1,A2,A3]);
R = [1 p q ; p 1 r ; q r 1];
SNR = linspace(-5 , 15 ,50);
b = 2*randi([0 1],3,Nbits) - 1;
sR = sqrtm (R);

for i=1:length(sigma2_vecteur)
sigma2=sigma2_vecteur(i);
n = sqrt (sigma2) * sR * randn (3,Nbits);
y = (R * A)*b + n;

%%%%%
b_fa = sign(y);
%%%%%
bdec = sign(inv(R)*y);
Rd=(R-diag(ones(1,3)))';
%%%%%
yfafa= y-([A1,A2,A3].'.*Rd)'*b_fa;
bfafa = sign(yfafa);
%%%%%%%
ydecfa= y-([A1,A2,A3].'.*Rd)'*bdec;
bdecfa=sign(ydecfa);
%%%%%%%%%
yfadec=y-([A1,A2,A3].'.*Rd)'*b_fa;
bfadec=sign(inv(R)*yfadec);
%%%%%%
ydecdec=y-([A1,A2,A3].'.*Rd)'*bdec;
bdecdec=sign(inv(R)*ydecdec);
TEB_dp(i)=length(find(b_dp(1,:)~=b(1,:)))/length(b);

TEB_fa(i)=length(find(b_dp(1,:)~=b(1,:)))/length(b);
TEB_dec(i)=length(find(bdec(1,:)~=b(1,:)))/length(b);
TEB_fafa(i)=length(find(bfafa(1,:)~=b(1,:)))/length(b);

TEB_decfa(i)=length(find(bdecfa(1,:)~=b(1,:)))/length(b);

TEB_fadec(i)=length(find(bfadec(1,:)~=b(1,:)))/length(b);

TEB_decdec(i)=length(find(bdecdec(1,:)~=b(1,:)))/length(b);
end

figure

semilogy(SNR,TEB_fa)
hold on
semilogy(SNR,TEB_dec)
hold on
semilogy(SNR,TEB_fafa)
hold on
semilogy(SNR,TEB_decfa)
hold on
semilogy(SNR,TEB_fadec)
hold on
semilogy(SNR,TEB_decdec)
hold on
R_inv=inv(R);
fa_th=qfunc(A1./sigma2_vecteur);
semilogy(SNR,fa_th)
hold on 
dec_th= qfunc(A1./(sigma2_vecteur*sqrt(R(1,1))));
semilogy(SNR,dec_th)
hold off
title("TEB")
xlabel("SNR")
ylabel("TEB")
legend("fa" ,"dec ","fafa","decfa","fadec","decdec","fath","decth")

%%%%%%%%%%%%
% Pour k=3, on ramarque presque les même courbes que le cours ou les /le
% courbes change presquement de pente

