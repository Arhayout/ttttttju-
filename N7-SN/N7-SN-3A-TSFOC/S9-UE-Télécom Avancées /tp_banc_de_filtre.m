clear all 
close all

%% partie 1

%paramètres de la chaine de trasmission 
N_bit= 10000; 
Ts = 1;
Rs = 1/Ts; 
Freq_porteuse = 2*Rs;
Freq_ech = 8*Rs; 
Temps_ech = 1/Freq_ech;
alpha = 1;
B=(1+alpha)*Rs; 
Ns=8;
Fe=8*Rs;
Te=1/Rs;
facteur_interpolation=8; 

%filtres de la chaine 
h1=rcosdesign(alpha,8,8);
h2=rcosdesign(alpha,8,2,'normal');
h3=rcosdesign(alpha,4,2,'normal');


%% partie 1

%génération des bits 
bits=randi([0 1],1,N_bit) ;
s=qammod(bits.',4,'InputType','bit').';

%modulation QPSK
suite_dirac_pondere=kron(s,[1 zeros(1,facteur_interpolation-1)]);
signal_emis=filter(h1,1,suite_dirac_pondere);
n=0:length(signal_emis)-1 ;

%frequence porteuse 
signal_porteuse = signal_emis.*exp(1j*2*pi*0.25*n); 
signal_reel=real(signal_porteuse);

%dsp du signal à la sortie du filtre d'interpolation :
figure
[DSP,f] = pwelch(signal_emis,[],[],[],8,'centered');
semilogy(f,DSP)
title("DSP à la sortie du filtre d'interpolation")

%DSP de la partie réel du signal après décalage (Xn) 
[DSP,f] = pwelch(signal_reel,[],[],[],8,'centered');
figure
semilogy(f,DSP)
title("DSP de la partie réel du signal après décalage (Xn) ")

%% partie 2

signal_decale=signal_reel.*exp(-0.5*j*pi*n);
[DSP,f] = pwelch(signal_decale,[],[],[],8,'centered');
figure
semilogy(f,DSP)
title("DSP du signal après décalage '-B'")

signal_xe=filter(h2,1,signal_decale);            
yn=signal_xe(33+9:2:end);  %on prend en considèration le retard des filtres 

[DSP,f] = pwelch(yn,[],[],[],8,'centered');
semilogy(f,DSP)
title("DSP du signal yn")

%filtre adapté 
yn_r=filter(h3,1,yn);
yn_r_decime=yn_r(5:4:end);
%constellations
scatterplot(yn_r_decime)
%diagramme de l'oeil 
eyediagram(yn_r,4)

%% partie 3


xn_1=signal_reel(33:2:end);
xn_2=signal_reel(34:2:end);


h_0=h2(1:2:end);
h_1=[0 h2(2:2:end)];

xn_1=xn_1.*kron(ones(1,length(xn_1)/2),[1 -1]);
xn_2=xn_2.*kron(ones(1,length(xn_2)/2),[1 -1]);

ReYn=filter(h_0,1,xn_1);
ImYn=(-1)*filter(h_1,1,xn_2);


yn=ReYn(5:end)+j*ImYn(5:end);









