%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Illustration du fonctionnement de Turbo codes  %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
clear all;


%Simulation parameters
%%%%%%%%%%%%%%%%%%%%%%% 
EbN0dB=[0:0.25:2.5];
K= 4096; %information frame size
M=2;% Modulation parameter
MC=10000;% Nb frame
ITERMAX=15;

%Interleaver definition
s = RandStream('mt19937ar', 'Seed', 11);
intrlvrIndices = randperm(s, K);

%Parralell TC definition
hTEnc  = comm.TurboEncoder('TrellisStructure', poly2trellis(5, ...
             [37 21], 37), 'InterleaverIndices', intrlvrIndices);
R=1/3;

%Modulation
hMod   = comm.BPSKModulator;% BPSK modulator
    
    
%handling errour count       
hErrorMAP = comm.ErrorRate;
    
%Allocation
BER_MAP=zeros(ITERMAX,length(EbN0dB));

for nbiter=ITERMAX:ITERMAX
   
    %On regarde l'influence du nombre de decodage
    %Coding parameters
    hTDec  = comm.TurboDecoder('TrellisStructure', poly2trellis(5, ...
             [37 21], 37), 'InterleaverIndices', intrlvrIndices, ...
             'NumIterations', nbiter);
     mc=0;
    for ii=1:length(EbN0dB)
      
      if mc==MC
          STOP=0;
      else
            STOP=1;
      end
      %for mc=1:MC
      mc=0;
      NbErrors=0;
      
      while ((NbErrors <100)||(mc<1000))&&(mc<MC) && STOP   
      disp([ii,mc])
           
      %encode turbo codes
      data = randi(s, [0 1], K, 1);
      encodedData = step(hTEnc, data);
      modSignal = step(hMod, encodedData);
      
      %add noise
       EbN0=10^(EbN0dB(ii)/10);
       Px= mean(abs(modSignal).^2);
       sigma_2= Px/(2*log2(M)*R*(EbN0));
       noiseAWGN= sqrt(sigma_2)*randn(1,length(modSignal));
       receivedSignal = modSignal + noiseAWGN.';
      
      
      % Convert received signal to log-likelihood ratios for decoding
      receivedBits  = step(hTDec, (-2/sigma_2)*real(receivedSignal));
      
      %Demod
      % Convert soft decisions to hard decisions
      errorStatsMAP = step(hErrorMAP, data, receivedBits);
      mc=mc+1;
      NbErrors=errorStatsMAP(2);
      end
      
      BER_MAP(nbiter,ii)=errorStatsMAP(1)
      
      reset(hErrorMAP);
     end
    semilogy(EbN0dB,BER_MAP(:,:)','ro-');
    drawnow;
    
    end
    
    semilogy(EbN0dB,BER_MAP(1:5:end,:)','ro-');