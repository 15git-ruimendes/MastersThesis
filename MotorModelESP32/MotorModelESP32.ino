#include <math.h>

#define VOLTAGE_A   34
#define VOLTAGE_B   35
#define VOLTAGE_C   32

#define CURRENT_A   25
#define CURRENT_B   26

#define VDC         430.0f
#define Rs          0.0001f
#define Ls          0.01f
#define Rr          0.001f
#define Lr          0.01f
#define Lm          0.15f

#define Load_NM     0.0f
#define J           0.0f

#define PI          3.141592654f

double Voltages[3],Currents[3],Voltages_ab[2],Currents_ab[2],Currents_dq[2];
double Theta,RPM,Torque,Flux[2],PrevFlux[2] = {0,0},PrevCurrent[2] = {0,0};

double Delt = 1 - Lm^2/(Ls*Lr);
double R1,R2,R3,R4,W1,W2;

void GetVoltages  (void)
{
  Voltages[0] = VDC * (double)(analogRead(VOLTAGE_A))  *   0.0002442;
  Voltages[1] = VDC * (double)(analogRead(VOLTAGE_B))  *   0.0002442;
  Voltages[2] = VDC * (double)(analogRead(VOLTAGE_C))  *   0.0002442;
}

void DisplayCurrents  (void)
{
  dacWrite(CURRENT_A,Currents[0]  * 1.275);
  dacWrite(CURRENT_B,Currents[1]  * 1.275);
}

void PhaseTransform   (void)
{
  Voltages_ab[0] = 0.816496581  * (Voltages[0] -0.5        *Voltages[1]    -0.5        *Voltages[2]);
  Voltages_ab[1] = 0.816496581  * (             0.816496581*Voltages[1]    -0.816496581*Voltages[2]);
}

void CalcFlux         (double time_step)
{
  Flux[0] = 
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  R1  =   -(Rs/(Delt*Ls) + (1 - Delt)*Rr/(Delt*Lr));
  R2  =   Lm*Rr/(Delt*Ls*Lr^2);
  R3  =   Lm*Rr/Lr;
  R4  =   -Rr/Lr;
  W1  =   Lm*RPM*2*PI/(60*Delt*Ls*Lr);
  W2  =   -RPM*2*PI/60;
}

void loop() {
  // put your main code here, to run repeatedly:
  
}
