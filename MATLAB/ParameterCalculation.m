%Parameter Calculation

%DC test
Vdc = 415;
Idc = 328.043;
Vcmpr = -0.75;
Rs = Vdc*(Vcmpr*0.5 + 0.5)/(2*Idc);
%Ls Lr Calculation
t3 = 3.5e-2;
t4 = 5e-2;
It3 = 3.39467e2;
It4 = -2.49283e2;

L = 1/3*(Vdc*(t4-t3))/(2*It3 - 2*It4);

%Lm Rr Calculation
%HighFreq
w1 = 2*pi*30;
theta1 = 1.29465; %rad
U1 = 269.589*0.5 - Rs*204.461*(cos(theta1) - j*sin(theta1));
I1 = 204.461*(cos(theta1) - j*sin(theta1));
Re1 = real(U1/I1)
Xe1 = imag(U1/I1)
%LowFreq
w2 = 2*pi*10;
theta2 = 0.88464; %rad
U2 = 269.218*0.5 - Rs*490.094*(cos(theta2) - j*sin(theta2));
I2 = 490.094*(cos(theta2) - j*sin(theta2));
Re2 = real(U2/I2)
Xe2 = imag(U2/I2)

K1 = w1^2*w2^2*(Re1 - Re2)/(Re1*Re2*(w1^2 - w2^2));
K2 = sqrt((Re2*w1^2 - Re1*w2^2)/(Re1 - Re2))/(w1*w2);

Lm = sqrt((Xe1*K1 + Xe1*K1*K2^2*w1^2 + K2*w1^3)/(K1^2*K2*w1 + K1^2*K2^3*w1^3));
Rr = K1*(Lm)^2;

%%%%%%%%%%%%%%%%%%%%%%%%
Rs
Ls = L
Lr = L
Lm = Lm
Rr  