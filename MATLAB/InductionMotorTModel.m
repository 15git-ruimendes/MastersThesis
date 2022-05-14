pkg load control
%%%%%%%%%%%%%%%%%%%%%%%
%%%%Motor LGR Model%%%%
%%%%%%%%%%%%%%%%%%%%%%%


%Motor model based on T electrical circuit 
%model and variable load resistance. Motor
% parameters must be loaded first.

%%%%Motor Parameters%%%%

Rs = 0.03552;
Ls = 0.000335;
Rr = 0.02092;
Lr = 0.000335;
Lm = 0.0151;
P = 4;
J = 1.2;

%%Current Model Results in 
%     k1*s^2 + k2*s + k3
% k * --------------------
%           s + k4
%Where:
%Sum to Rr Load specific parameters;
Rr = Rr/0.05;
%
k1 = Lm*Lr + Ls*(Lm + Lr);
k2 = Rs*(Lm + Lr) + Rr*Ls + Rr*Lm;
k3 = Rr*Rs;
k4 = Rr;
k5 = Lm + Lr; 

s = tf('s');
MotorModel = k4/k1*(s+k5/k4)/(s^2 + s*k2/k1 + k3/k1)

figure(1);
rlocus(MotorModel);
title("Uncontroled Motor");
%%Controller
%%Find pole to cancel
roots([1 k2/k1 k3/k1])
pole = min((roots([1 k2/k1 k3/k1])))

Kp = k1/k4
Ki = -pole*Kp
Ki/Kp
Controller = (Kp*s + Ki)/s;
c2d(Controller,0.833e-3)

figure(2);
rlocus(Controller*MotorModel);
title("Controlled Process");

figure(3);
c2d(Controller*MotorModel,0.833e-3)
rlocus(c2d(Controller*MotorModel,0.833e-3));