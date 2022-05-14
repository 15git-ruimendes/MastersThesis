%Transform Validation
pkg load control
pkg load signal 
close all

%Confirmation function
function ClarkeTransform (CurrentA,CurrentB,CurrentC,Time,CurrentAlpha,CurrentBeta)
    SoftAlpha = (CurrentA - 0.5 * CurrentB - 0.5 * CurrentC) * 0.66666666666666666666666667;
		SoftBeta  = ((sqrt(3) * 0.5 * CurrentB - sqrt(3) * 0.5 * CurrentC)) * 0.66666666666666666666666667;
    figure(2);
    hold on;
    plot(Time,CurrentAlpha,"displayname","Current Alpha");
    plot(Time,CurrentBeta,"displayname","Current Beta");
    plot(Time,SoftAlpha,"displayname","Calculated Alpha");
    plot(Time,SoftBeta,"displayname","Calculated Beta");
    legend("show");
endfunction

function ParkTransform  (CurrentA,CurrentB,CurrentC,Time,CurrentAlpha,CurrentBeta,CurrentD,CurrentQ)
    ClarkeTransform (CurrentA,CurrentB,CurrentC,Time,CurrentAlpha,CurrentBeta);
    %Theta = atan2(CurrentBeta,CurrentAlpha);
    Theta = pi*(sawtooth(Time*10*2*pi) + 1);
    SoftD =  CurrentAlpha .* cos(Theta) + CurrentBeta .* sin(Theta);
	  SoftQ =  CurrentAlpha .* sin(Theta) - CurrentBeta .* cos(Theta);
    figure(3);
    subplot(2,1,1);hold on;
    plot(Time,CurrentD,"displayname","Current D");
    plot(Time,CurrentQ,"displayname","Current Q");
    plot(Time,SoftD,"displayname","Calculated D");
    plot(Time,SoftQ,"displayname","Calculated Q");
    legend("show");
    subplot(2,1,2);hold on;
    plot(Time,CurrentA,"displayname","Current A");
    plot(Time,Theta,"displayname","Calculated Angle");
    legend("show");
endfunction

%Transform = csvread("DQ_Transform_10Hz.csv");
Transform = csvread("DQ_Transform_Start.csv");

Time          = Transform(:,1)*1e-6;
CurrentA      = Transform(:,2);
CurrentB      = Transform(:,3);
CurrentC      = Transform(:,4);

CurrentAlpha  = Transform(:,5);
CurrentBeta   = Transform(:,6);

CurrentD      = Transform(:,7);
CurrentQ      = Transform(:,8);

%Plot all transforms

figure(1);
hold on;

subplot(3,1,1);
title("ABC to Alpha-Beta to DQ Transforms");
ylabel("I(A)");
hold on;
plot(Time,CurrentA,"displayname","Current A");
plot(Time,CurrentB,"displayname","Current B");
plot(Time,CurrentC,"displayname","Current C");
legend("show");

subplot(3,1,2);
ylabel("I(A)");
hold on;
plot(Time,CurrentAlpha,"displayname","Current Alpha");
plot(Time,CurrentBeta,"displayname","Current Beta");
legend("show");

subplot(3,1,3);
ylabel("I(A)");
xlabel("Time(s)");
hold on;
plot(Time,CurrentD,"displayname","Current D");
plot(Time,CurrentQ,"displayname","Current Q");
legend("show");

ParkTransform(CurrentA,CurrentB,CurrentC,Time,CurrentAlpha,CurrentBeta,CurrentD,CurrentQ);



