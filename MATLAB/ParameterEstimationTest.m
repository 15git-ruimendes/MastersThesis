%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Parameter Estimation Test%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%Select Delta or Y Configuration (not in use)
Delta = true;
Y = false;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% DC Test Results
%%%%%%%%%%%%%%%%%%%%%%%%%%%%
IDC = 439;
VDC = 415;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% AC Test Results
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%High Frequency
f1 = 10;
w1 = 2*pi*f1;
Vab1 = 110;
Ia1 = 1061.07*0.5*3;
time_delay1 =9.66000e-03;
theta1 = w1*time_delay1;

%%Low Frequency
f2 = 2.5;
w2 = 2*pi*f1;
Vab2 = 110;
Ia2 = 1312.12*0.5*3;
time_delay2 = 1.30100e-02;
theta2 = w2*time_delay2;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Measurment Test
%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%Aux Variables
a1 = Vab1/(2*Ia1)*cos(theta1)
b1 = Vab1/(2*Ia1)*sin(theta1)
a2 = Vab2/(2*Ia2)*cos(theta2)
b2 = Vab2/(2*Ia2)*sin(theta2)

Rs = VDC*(-0.75*0.5+0.5)/(IDC)*2/3
%Rs = 0.03552
R2 = a1 - Rs;
Lsr = R2/w2*sqrt((a2 - Rs)/(Rs + R2 - a2));
L1s = b1/w1;

Ls = L1s + Lsr
Rr = R2*(L1s + Lsr)/Lsr
Lm = sqrt(Lsr*(L1s + Lsr));
Lr = Lm^2/Lsr
Lm = Lm
