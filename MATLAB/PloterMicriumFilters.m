%Load CSV and plot data
pkg load control
pkg load signal
close all

function output = FilterHandmade (input,a,b)
      
      output = zeros(length(input),1);
      buffer_in = zeros(length(b),1);
      buffer_out = zeros(length(a),1);
      for j = 1:length(input)
        num_aux = 0;
        den_aux = 0;
        for i = 1:length(buffer_in)-1
          num_aux += b(i+1)*buffer_in(i);
          den_aux += a(i+1)*buffer_out(i);
        endfor
        num_aux += b(1)*input(1);
        out_aux = 1/a(1)*(num_aux - den_aux);
        for i = length(buffer_in):-1:2
          buffer_in(i) = buffer_in(i-1);
          buffer_out(i) = buffer_out(i-1);
        endfor
        buffer_in(1) = input(j);
        buffer_out(1) = out_aux;
        output(j) = out_aux;
      endfor
      return 
endfunction


Filter_10 = csvread("Filter_10.csv");
Frequency = 10;
TimeFrame = Filter_10(:,1)*10e-6;

figure(1);
subplot(3,1,1);hold on;
title("Filter and unfiltered currents (third order IIR butterworth)");
ylabel("Current Phase A(A)");
plot(TimeFrame,Filter_10(:,2),"displayname","Filtered Current");
hold on;
plot(TimeFrame,Filter_10(:,9),"displayname","Unfiltered Current");
legend("show");

subplot(3,1,2);hold on;
ylabel("Current Phase B(A)");
plot(TimeFrame,Filter_10(:,3));
plot(TimeFrame,Filter_10(:,8));
subplot(3,1,3);hold on;
ylabel("Current Phase C(A)");
plot(TimeFrame,Filter_10(:,4));
plot(TimeFrame,Filter_10(:,7));
xlabel("Time (s)");


figure(8);hold on;
plot(TimeFrame(100:200),Filter_10(100:200,4));
plot(TimeFrame(100:200),Filter_10(100:200,7));


figure(2);

subplot(2,1,1);hold on;
plot(TimeFrame,Filter_10(:,2));
plot(TimeFrame,Filter_10(:,3));
plot(TimeFrame,Filter_10(:,4));
subplot(2,1,2);hold on;
plot(TimeFrame,Filter_10(:,9));
plot(TimeFrame,Filter_10(:,8));
plot(TimeFrame,Filter_10(:,7));

%Test filter 
[b,a] = butter (3, 100/4000);
%a(1:4) = [1.000000000000,-2.842960524202,2.698010527552,-0.854601442430];
%b(1:4) = [0.000056070115,0.000168210345,0.000168210345,0.000056070115];
%TestFilter = filter(b,a,Filter_10(:,9));
[b1,a1] = cheby1(2,0.07,200/4000);
TestFilter1 = filter(b1,a1,Filter_10(:,9));
TestFilter = FilterHandmade(Filter_10(:,9),a,b);

figure(3);

subplot(3,1,1);hold on;
plot(TimeFrame,Filter_10(:,9));
plot(TimeFrame,TestFilter);
subplot(3,1,2);hold on;
plot(TimeFrame,Filter_10(:,9));
plot(TimeFrame,TestFilter1);
subplot(3,1,3);hold on;
plot(TimeFrame,Filter_10(:,9));
plot(TimeFrame,Filter_10(:,2));

%Filter Design
figure(4);
freqz(b1,a1,[],8000);
figure(5);
freqz(b,a,[],8000);

%FilterValue_10 = H(find(W == Frequency));
%FilterGain_10hz = abs(FilterValue_10)
%PhaseShift_10hz = angle(FilterValue_10)*180/pi

%FFT Analysis
figure(6);
hold on;
FFT_10 = fft(Filter_10(:,9));
plot(abs(FFT_10(1:500)))
FFT_10_Filt = fft(Filter_10(:,2));
plot(abs(FFT_10_Filt(1:500)))

figure(7);
clf
hold on;
subplot(2,3,1);
plot(TimeFrame,Filter_10(:,9)-Filter_10(:,2));
subplot(2,3,2);
plot(TimeFrame,Filter_10(:,9)-TestFilter);
subplot(2,3,3);
plot(TimeFrame,Filter_10(:,9)-TestFilter1);
subplot(2,3,4);
hist(Filter_10(:,9)-Filter_10(:,2),500);
subplot(2,3,5);
hist(Filter_10(:,9)-TestFilter,500);
subplot(2,3,6);
hist(Filter_10(:,9)-TestFilter1,500);



