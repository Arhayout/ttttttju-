
M = [16];                     % Constellation Order
Constellation=qammod((0:M-1)',M,'PlotConstellation', true);
BinaryTable=de2bi((0:M-1)','left-msb');
Ns=10;
    s = randi([0 M-1],1,Ns);
    x = qammod(s, M);