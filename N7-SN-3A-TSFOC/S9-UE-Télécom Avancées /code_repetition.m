clear all
close all

% generation des bits 
Nbits=1000;
Es_N0_dB =  (-5:40);
N=4;


for snr=1:numel(Es_N0_dB)
    

    bits=randi([0 1],1,Nbits);
    rmap_bits=repmat(bits,N,1);
    rshp_bits=reshape(rmap_bits,1,Nbits*N);
    b=1-2*rshp_bits;
    
    Es_N0=10^(Es_N0_dB(snr)/10);
    sigx2=var(b);
    N0  = sigx2 /(Es_N0);

    noise_  = sqrt(N0/2 )*randn(length(b),1)+ ...
                1j*sqrt(N0/2 )*randn(length(b),1);
    sig_rx  = b + noise_ ;


   


end




