#include <math.h>
double FREQ_SIGNAL = 50ffff.0;
double AMPLITUDE_SIGNAL = 40.0;
double OFFSET_SIGNAL = 0.20;

double FREQ_NOISE = 2400.0;
double AMPLITUDE_NOISE = 0.0;

double PHASE_SHIFT = 120.0;

#define DAC_1 25
#define DAC_2 26

double pi = 3.14159265359;

double time_cycle = 0.0;
void setup() {
  Serial.begin(115200);
}

void loop() {
  dacWrite(DAC_1, 158 * AMPLITUDE_SIGNAL * 0.01 * sin(2 * pi * FREQ_SIGNAL * time_cycle) + 158 * AMPLITUDE_NOISE * 0.01 * sin(2 * pi * FREQ_NOISE * time_cycle) + (1 - OFFSET_SIGNAL) * 158);
  dacWrite(DAC_2, 158 * AMPLITUDE_SIGNAL * 0.01 * sin(2 * pi * FREQ_SIGNAL * time_cycle + pi*0.66666666) + 158 * AMPLITUDE_NOISE * 0.01 * sin(2 * pi * FREQ_NOISE * time_cycle) + (1 - OFFSET_SIGNAL) * 158);
  time_cycle += 0.00001;
  if (time_cycle >= 1/FREQ_SIGNAL)
    time_cycle = 0;
  delayMicroseconds(10);
}
