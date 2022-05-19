
#define R     0.00010f
#define L     0.00600f

#define VDC   32.0000f

#define DAC1  25
#define DAC2  26

#define PWM1  12
#define PWM2  14
#define PWM3  27

float Voltage[3][2],Current[3][2];

void CalculateCurrents()
{
  Current[0][0] = (Voltage[0][0] + L * Current[0][1])/(R + L);
  Current[1][0] = (Voltage[1][0] + L * Current[1][1])/(R + L);
  Current[2][0] = (Voltage[2][0] + L * Current[2][1])/(R + L);
}

void DisplayCurrents()
{
  //Serial.print(Current[0][0]);
  //Serial.print("   ");
  //Serial.println(Current[1][0]);
  dacWrite(DAC1,Current[0][0]*255/400000);
  dacWrite(DAC2,Current[1][0]*255/400000);
  
  Current[0][1] = Current[0][0];
  Current[1][1] = Current[1][0];
  Current[2][1] = Current[2][0];
}

void ReadVoltages()
{
  Voltage[0][0] = VDC * (2 * digitalRead(PWM1) - 1);
  Voltage[1][0] = VDC * (2 * digitalRead(PWM2) - 1);
  Voltage[2][0] = VDC * (2 * digitalRead(PWM3) - 1);  
}

void setup() {
    Serial.begin(115200);
    pinMode(PWM1,INPUT);
    pinMode(PWM2,INPUT);
    pinMode(PWM3,INPUT);
}

void loop() {
  ReadVoltages();
  CalculateCurrents();
  DisplayCurrents();
  ets_delay_us(20);
}
