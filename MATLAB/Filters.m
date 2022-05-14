%Filter Options 
pkg load control 
pkg load signal

clf
fc = 120;
fsamp = 12000;

s = tf('s');
figure(1); 
FirstOrder =  fc*2*pi/(s + fc*2*pi);
bode(FirstOrder);

figure(2); 
[b,a] = butter (3, 120*2, "s");
ButterFilt_6thOrder = tf(b,a);
bode(ButterFilt_6thOrder);

figure(3); 
[b,a] = cheby1(3,0.5,120*2,"s");
Cheby1Filt_6thOrder = tf(b,a);
bode(Cheby1Filt_6thOrder);

%Discrete
figure(4); 
[b,a] = butter (3, 200/8000);
ButterFilt_6thOrder = tf(b,a);
freqz(b,a,10000);
printf("b = ");
for i = 1:4
      printf("%.12f,",b(i));
endfor
printf("\n");
printf("a = ");
for i = 1:4
      printf("%.12f,",a(i));
endfor
printf("\n");

figure(5); 
[b,a] = cheby1(2,0.5,100/8000);
Cheby1Filt_6thOrder = tf(b,a);
freqz(b,a,10000);
printf("b = ");
for i = 1:3
      printf("%.12f,",b(i));
endfor
printf("\n");
printf("a = ");
for i = 1:3
      printf("%.12f,",a(i));
endfor
printf("\n");
