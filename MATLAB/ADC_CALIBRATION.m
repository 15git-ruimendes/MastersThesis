% ADC and Sensor Calibration
% Linear correspondence only

MaxBitValue =   2044;
MinBitValue =   2037;

MaxRealValue =  140e-3;
MinRealValue =  -140e-3;

Gain    = (MaxRealValue - MinRealValue)/(MaxBitValue - MinBitValue);
Offset  = MaxRealValue - MaxBitValue*Gain;

printf("Gain = %.16f\n",Gain);
printf("Offset = %.16f\n",Offset);