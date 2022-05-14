% For octave
pkg load signal
pkg load control
clf
%
% Induction Motor Tests
% Parameter Analysis
%

%
% Short Circuit Tests
%

% Phase U
File_SC_U = load('ShortCircuit_PhaseU.mat');
Iu_unfilt = File_SC_U.A(1450000:1460000);
Uu_unfilt = File_SC_U.B(1450000:1460000);
Time = File_SC_U.Tinterval*(0:(1460000-1450000));
L = 1460000-1450000 + 1;
%% FIR Filter 
SampFreq = 1/File_SC_U.Tinterval;
NormCutFreq = 500/SampFreq;
Coeffs = fir1(40,NormCutFreq);
Iu = filter(Coeffs, 1, Iu_unfilt);
Uu = filter(Coeffs, 1, Uu_unfilt);

figure(3);
hold on;
subplot(2,1,1);
plot(Time,Iu_unfilt,Time,Uu_unfilt);
title('Raw Inputs');
subplot(2,1,2);
plot(Time,Iu,Time,Uu);
title('Filtered Inputs');

%% FFT analysis
FFT_Iu = fft(Iu,L);
FFT_Uu = fft(Uu,L);

f = SampFreq*(0:(L/2))/L;

figure(4);
hold on;
subplot(2,1,1);
plot(f(1:10),abs(FFT_Iu/L)(1:10),f(1:10),2*abs(FFT_Uu/L)(1:10));
subplot(2,1,2);
plot(f(1:10),angle(FFT_Iu)(1:10)*180/pi,f(1:10),angle(FFT_Uu)(1:10)*180/pi);

f_50 = find(f > 49 & f < 51);
Z_U = abs(FFT_Uu/L)(f_50)/abs(FFT_Iu/L)(f_50) * (cos(50*2*pi - (angle(FFT_Uu)(f_50) - angle(FFT_Iu)(f_50))) + i * sin(50*2*pi - (angle(FFT_Uu)(f_50) - angle(FFT_Iu)(f_50))));
Z_U = 2/3*1/Z_U

% Phase V
File_SC_V = load('ShortCircuit_PhaseV.mat');
IV_unfilt = -File_SC_V.A(1450000:1460000);
UV_unfilt = File_SC_V.B(1450000:1460000);
Time = File_SC_V.Tinterval*(0:(1460000-1450000));
L = 1460000-1450000 + 1;
%% FIR Filter 
SampFreq = 1/File_SC_V.Tinterval;
NormCutFreq = 500/SampFreq;
Coeffs = fir1(40,NormCutFreq);
IV = filter(Coeffs, 1, IV_unfilt);
UV = filter(Coeffs, 1, UV_unfilt);

figure(5);
hold on;
subplot(2,1,1);
plot(Time,IV_unfilt,Time,UV_unfilt);
title('Raw Inputs');
subplot(2,1,2);
plot(Time,IV,Time,UV);
title('Filtered Inputs');

%% FFT analysis
FFT_IV = fft(IV,L);
FFT_UV = fft(UV,L);

f = SampFreq*(0:(L/2))/L;

figure(6);
hold on;
subplot(2,1,1);
plot(f(1:10),abs(FFT_IV/L)(1:10),f(1:10),2*abs(FFT_UV/L)(1:10));
subplot(2,1,2);
plot(f(1:10),angle(FFT_IV)(1:10)*180/pi,f(1:10),angle(FFT_UV)(1:10)*180/pi);

f_50 = find(f > 49 & f < 51);
Z_V = abs(FFT_UV/L)(f_50)/abs(FFT_IV/L)(f_50) * (cos(50*2*pi - (angle(FFT_UV)(f_50) - angle(FFT_IV)(f_50))) + i * sin(50*2*pi - (angle(FFT_UV)(f_50) - angle(FFT_IV)(f_50))));
Z_V = 2/3*1/Z_V

% Phase W
File_SC_W = load('ShortCircuit_PhaseW.mat');
IW_unfilt = File_SC_W.A(1450000:1460000);
UW_unfilt = File_SC_W.B(1450000:1460000);
Time = File_SC_W.Tinterval*(0:(1460000-1450000));
L = 1460000-1450000 + 1;
%% FIR Filter 
SampFreq = 1/File_SC_W.Tinterval;
NormCutFreq = 500/SampFreq;
Coeffs = fir1(40,NormCutFreq);
IW = filter(Coeffs, 1, IW_unfilt);
UW = filter(Coeffs, 1, UW_unfilt);

figure(7);
hold on;
subplot(2,1,1);
plot(Time,IW_unfilt,Time,UW_unfilt);
title('Raw Inputs');
subplot(2,1,2);
plot(Time,IW,Time,UW);
title('Filtered Inputs');

%% FFT analysis
FFT_IW = fft(IW,L);
FFT_UW = fft(UW,L);

f = SampFreq*(0:(L/2))/L;

figure(8);
hold on;
subplot(2,1,1);
plot(f(1:10),abs(FFT_IW/L)(1:10),f(1:10),2*abs(FFT_UW/L)(1:10));
subplot(2,1,2);
plot(f(1:10),angle(FFT_IW)(1:10)*180/pi,f(1:10),angle(FFT_UW)(1:10)*180/pi);

f_50 = find(f > 49 & f < 51);
Z_W = abs(FFT_UW/L)(f_50)/abs(FFT_IW/L)(f_50) * (cos(50*2*pi - (angle(FFT_UW)(f_50) - angle(FFT_IW)(f_50))) + i * sin(50*2*pi - (angle(FFT_UW)(f_50) - angle(FFT_IW)(f_50))));
Z_W = 2/3*1/Z_W

R_target = real(Z_U + Z_W + Z_V)/3
L_target = imag(Z_U + Z_W + Z_V)/3*1/(2*pi*50)
w = 2*pi*50
Z_target = R_target + i*w*L_target;

% Model Estimation
% Initial Values 

Rs = 0.03552;
L = 0.000335;
Rr = 0.02092;
Lm = 0.0151;
%Rs = 0;
%Rr = 0;
%L = 1e-12;
%Lm = 1e-12;
%Rs = 1;
%Rr = 1;
%L = 1;
%Lm = 1;

Error = 100+i;
count = 0;
ABS_ERROR = 100;
Prev_ABS = 1000;
while (count <= 100000 && ABS_ERROR > 1e-12)
      Z = (Rs + (w^2*(L+Lm)*Lm*Rr - w^2*L*Lm*Rr)/(Rr^2 - (w*(L+Lm))^2)) + i*(w*L + (w*Lm*Rr^2 + w^3*L*Lm*(L+Lm))/(Rr^2 - (w*(L+Lm))^2));
      Error = Z_target - Z;
      real_error = real(Error);
      imag_error = imag(Error);
      ABS_ERROR = abs(Error);
      
      Rs += 0.005 * real_error + 0.005*imag_error;
      L += 0.0001 * real_error + 0.005*imag_error;
      Rr += 0.0001 * real_error + 0.005*imag_error;
      Lm += 0.00001 * real_error + 0.0005*imag_error;

      if(Prev_ABS > ABS_ERROR)
        printf("Z_Target = %.4f + i*%.4f Z = %.4f + i*%.4f\n",real(Z_target),imag(Z_target),real(Z),imag(Z));
        printf("Rs = %.4f L = %.4f Rr = %.4f Lm = %.4f\n", Rs,L,Rr,Lm);
        Prev_ABS = ABS_ERROR;
      endif
      
      count++;
endwhile 
printf("Z_Target = %.4f + i*%.4f Z = %.4f + i*%.4f\n",real(Z_target),imag(Z_target),real(Z),imag(Z));
printf("Rs = %.4f L = %.4f Rr = %.4f Lm = %.4f\n", Rs,L,Rr,Lm);
