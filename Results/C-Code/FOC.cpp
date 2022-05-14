#include <Stdlib.h>
#include <String.h>
#include <math.h>
#include <Psim.h>

// Mathematical Constants
#define PI 3.14159265359

// Motor Parameters
#define Rs 0.03552
#define Ls 0.000335
#define Rr 0.02092
#define Lr 0.000335
#define Lm 0.0151
#define P 4
#define J 1.2

// Converter Parameters
#define VDC 1
#define T_SW 1.2e-4

// Current Controller Parameters
#define KiQ 0.075
#define KpQ 0.00095
#define KiD 0.075
#define KpD 0.00095

// Speed Controller Parameters
#define KiRPM 10
#define KpRPM 0.25

// Program Constants
#define FILTER_SIZE 200

//////////////////////////////////////////////////////////////////////
// Timing Variables
double _SimTime, _PITime = 0, _SVMTime = 0, _ThetaTime = 0;
double _PrevPI = 0, _PrevSVM = 0, _PrevTheta = 0,PI_AUX = 0;

// Input Variables
double _ABCCurrents[3], _ABVoltages[2], _RPM, _DQCurrentRef[2];

// Altered Input
double ABCFilter[3][FILTER_SIZE];
double AlBetCurrents[2], DQCurrents[2], AlBetVoltages[2];

// Aux Variables
double Theta = 0, PI_Out[2], PI_Out_Prev[2], SVM_Out[3], DQVoltageRef[2], AlBetVoltageRef[2], ABCVoltageRef[3];
double AccmErrD = 0, AccmErrQ = 0, WindUpD = 1, WindUpQ = 1;
double ErrQ, ErrD;
double _SVM_TimeAux, SVM_Theta, SVM_Gain, SVM_Sector, SVM_Times[3];
double _RPMRef;

// Slip Detection Variables
double PrevCurrentQ = 0, PrevRPM = 0, PrevDiffTime = 0;

// OutPut Variables
double Commutation[3];
//////////////////////////////////////////////////////////////////////
/*                          Functions                               */
//////////////////////////////////////////////////////////////////////
void Theta_Calculation(void)
{
if( _DQCurrentRef[0] > 0)
    Theta += _ThetaTime * (_RPM * 2 * PI / 60 * P / 2 + (_DQCurrentRef[1] / _DQCurrentRef[0] * Rr / (Lr + Lm)));
else 
	Theta = 0;
    if (abs(Theta) >= 2 * PI)
    {
        Theta = 0;
    }
}
//////////////////////////////////////////////////////////////////////
void Clarke(int Opt)
{
    // if Opt = 0 (Input currents); Opt = 1 (Input Voltages);
    if (Opt == 0)
    {
        AlBetCurrents[0] = ((_ABCCurrents[0] - 0.5 * _ABCCurrents[1] - 0.5 * _ABCCurrents[2])) * 2 / 3;
        AlBetCurrents[1] = ((sqrt(3) / 2 * _ABCCurrents[1] - sqrt(3) / 2 * _ABCCurrents[2])) * 2 / 3;
    }
    else if (Opt == 1)
    { // Power Invariant
        AlBetVoltages[0] = sqrt(3 / 2) * _ABVoltages[0];
        AlBetVoltages[1] = sqrt(2) / 2 * _ABVoltages[0] + sqrt(2) * _ABVoltages[1];
    }
}
//////////////////////////////////////////////////////////////////////
void Park(void)
{ // Only used after Clarke(0) is performed
    DQCurrents[0] = AlBetCurrents[0] * cos(Theta) + AlBetCurrents[1] * sin(Theta);
    DQCurrents[1] = -AlBetCurrents[0] * sin(Theta) + AlBetCurrents[1] * cos(Theta);
}
//////////////////////////////////////////////////////////////////////
void Inv_Clarke(void)
{
    ABCVoltageRef[0] = AlBetVoltageRef[0];
    ABCVoltageRef[1] = AlBetVoltageRef[0] * (-0.5) + AlBetVoltageRef[1] * sqrt(3) / 2;
    ABCVoltageRef[2] = -AlBetVoltageRef[0] * 0.5 - AlBetVoltageRef[1] * sqrt(3) / 2;
}
//////////////////////////////////////////////////////////////////////
void Inv_Park(void)
{ // Limit voltage references between 1 and -1
    double aux1, aux2;
    aux1 = PI_Out[0] * cos(Theta) - PI_Out[1] * sin(Theta);
    aux2 = PI_Out[0] * sin(Theta) + PI_Out[1] * cos(Theta);

    if (abs(aux1) > VDC)
    {
        AlBetVoltageRef[0] = (aux1 > 0) - (aux1 < 0);
    }
    else
        AlBetVoltageRef[0] = aux1;

    if (abs(aux2) > VDC)
    {
        AlBetVoltageRef[1] = (aux2 > 0) - (aux2 < 0);
    }
    else
        AlBetVoltageRef[1] = aux2;
}
//////////////////////////////////////////////////////////////////////
void CurrentPI(double *in)
{
    ErrD = _DQCurrentRef[0] - DQCurrents[0];
    ErrQ = _DQCurrentRef[1] - DQCurrents[1];

    PI_Out_Prev[0] += 0.5 * PI_AUX * (ErrD + AccmErrD)*WindUpD;
    PI_Out_Prev[1] += 0.5 * PI_AUX * (ErrQ + AccmErrQ)*WindUpQ;

    double OutD = PI_Out_Prev[0] * KiD + ErrD * KpD;
    double OutQ = PI_Out_Prev[1] * KiQ + ErrQ * KpQ;

    AccmErrD = ErrD;
    AccmErrQ = ErrQ;

    if (abs(OutD) > VDC)
    {
        WindUpD = 0;
        PI_Out[0] = VDC * ((OutD > 0) - (OutD < 0));
    }
    else
    {
        WindUpD = 1;
        PI_Out[0] = OutD;
    }
    if (abs(OutQ) > VDC)
    {
        WindUpQ = 0;
        PI_Out[1] = VDC * ((OutQ > 0) - (OutQ < 0));
    }
    else
    {
        WindUpQ = 1;
        PI_Out[1] = OutQ;
    }
}
//////////////////////////////////////////////////////////////////////
void SV_PWM(double *in)
{
    if (_PITime == 0)
    {
		//PI_AUX = _ThetaTime
        CurrentPI(in);
        Inv_Park();
    }
    _PITime = _PITime + _PrevSVM;

    if (_PITime >= T_SW)
    {
		PI_AUX = _PITime;
        _PITime = 0;
    }

    if (_SVMTime >= T_SW * 10)
    {
        _SVMTime = 0;
    }
}
//////////////////////////////////////////////////////////////////////
double prev_RPM_REF = 0;
void SlipDetection(double *out)
{
    double diffSpeed = (_RPM - PrevRPM) / 0.01;
    double diffCurr = (DQCurrents[1] - PrevCurrentQ) / 0.01;
	double diffRef = (_RPMRef - prev_RPM_REF);
	prev_RPM_REF = _RPMRef;
    PrevRPM = _RPM;
    PrevCurrentQ = DQCurrents[1];
    out[17] = diffSpeed;
    out[18] = diffCurr;

    if (diffSpeed > 10 && diffCurr < -10 && diffRef < 0) // If Iq current drops while speed rises (SLIP FLAG)
    {
        printf("Slip Found At: %f\n", _SimTime);
    }
}
//////////////////////////////////////////////////////////////////////
/*                         Aux Functions                           */
/////////////////////////////////////////////////////////////////////
void output(double *out)
{
    out[0] = DQCurrents[0];
    out[1] = DQCurrents[1];

    out[2] = AlBetCurrents[0];
    out[3] = AlBetCurrents[1];

    out[4] = AlBetVoltages[0];
    out[5] = AlBetVoltages[1];

    out[6] = PI_Out[0];
    out[7] = PI_Out[1];

    out[8] = AlBetVoltageRef[0];
    out[9] = AlBetVoltageRef[1];

    out[10] = Commutation[0];
    out[11] = Commutation[1];
    out[12] = Commutation[2];

    out[13] = Theta;

    out[14] = ErrD;
    out[15] = ErrQ;

    out[16] = SVM_Sector;
}
void input(double *in)
{
    _RPMRef = in[8];
    _ABCCurrents[0] = in[0];
    _ABCCurrents[1] = in[1];
    _ABCCurrents[2] = in[2];

    _ABVoltages[0] = in[3];
    _ABVoltages[1] = in[4];

    _RPM = in[5];

    _DQCurrentRef[0] = in[6];
    _DQCurrentRef[1] = in[7];
}
//////////////////////////////////////////////////////////////////////
/*                         Main Cycle                              */
/////////////////////////////////////////////////////////////////////
// Main Cycle Variables

void SimulationStep(
    double t, double delt, double *in, double *out,
    int *pnError, char *szErrorMsg,
    void **reserved_UserData, int reserved_ThreadIndex, void *reserved_AppPtr)
{

    input(in);

    _SimTime = t;

    _ThetaTime = _SimTime - _PrevTheta;
    Theta_Calculation();
    Clarke(0);
    Clarke(1);
    Park();

    _PrevTheta = _SimTime;

    _PrevSVM = _ThetaTime;
    SV_PWM(in);

    /*if (_SimTime - PrevDiffTime >= 0.01)
    {
        PrevDiffTime = _SimTime;
        SlipDetection(out);
    }*/

    output(out);
}