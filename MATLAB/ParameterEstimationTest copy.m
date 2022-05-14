%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Parameter Estimation Test%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%Select Delta or Y Configuration (not in use)
Delta = true;
Y = false;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% DC Test Results
%%%%%%%%%%%%%%%%%%%%%%%%%%%%
IDC = ;
VDC = ;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% AC Test Results
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%High Frequency
f1 = ;
w1 = 2*pi*f1;
Vab1 = ;
Ia1 = ;
time_delay1 = ;
theta1 = w1*time_delay1;

%%Low Frequency
f2 = ;
w2 = 2*pi*f1;
Vab2 = ;
Ia2 = ;
time_delay2 = ;
theta2 = w2*time_delay2;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Measurment Test
%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%Aux Variables
a1 = Vab1/(2*Ia1)*cos(theta1);
b1 = Vab1/(2*Ia1)*sin(theta1);
a2 = Vab2/(2*Ia2)*cos(theta2);
b2 = Vab2/(2*Ia2)*sin(theta2);

Rs = VDC/IDC

R2 = a1 - Rs;
Lsr = R2/w2*sqrt((a2 - Rs)/(Rs + R2 - a2));
L1s = b1/w1;

Ls = L1s + Lsr
Rr = R2*(L1s + Lsr)/Lsr
Lm = sqrt(Lsr*(L1s + Lsr));
Lr = Lm^2/Lsr
Lm = Lm
