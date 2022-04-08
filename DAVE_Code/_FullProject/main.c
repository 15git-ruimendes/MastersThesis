#include <DAVE.h>
#include "FOC.h"
#include <probe_scope.h>
#include <probe_scope_cfg.h>

#define TICKS_PER_SECOND 12000
#define SECOND_PER_TICKS 0.001f

#define KpSpeed 10
#define KiSpeed 0.01
#define T_PI_RPM 0.1
#define ControlLoopTime 8.3333333333333333333333333333333e-5;

/*
 * 0 - Stand alone inverter (SVM & Bellow Nominal Current Ref)
 * 1 - Open loop motor test (Nominal Id and Iq Curents)
 * 2 - Closed loop motor test (no-load,0.75 nominal speed ref)
 * 3 - Closed loop motor test (0.5 load,nominal speed ref)
 * 4 - Closed loop nominal load test
 */
#define TestVal 0

// Global Variables
FOC Controller;
float RPM;
float time = 0;
float RPM_REF;
float Ia,Ib,Ic;

// Speed Controller
/*
 * @brief PI Speed controller tuned to a slower dynamic response then current controller
 * @params RPM_Val Current speed value in RPM
 * @params Controller_Var FOC structure, function will change IqRef value limited to IQ_LIM
 */
void SpeedController(float RPM_Val, FOC Controller_Var);

// Field Weakening
/*
 * @brief Implements field weakening by reducing Id reference from nominal when current speed is greater than nominal by
 *  					wNominal
 * Id = IdNominal * ----------------
 * 							w
 * @params RPM_Val Current speed value in RPM
 * @params Controller_Var FOC structure, function will change IdRef value limited to ID_MIN
 */
void FieldWeakening(float RPM_Val, FOC Controller_Var);

//ADC Aquisition Loop
void AquisitonLoop(void)
{

}

void ControlLoop(void)
{
	Ia = sin(time*2*PI*50);
	Ib = sin(time*2*PI*50 + 0.5*PI);
	Ic = sin(time*2*PI*50 + 1.333333333333333*PI);

	RPM = SpeedSensor(Ia,Ib);
	time += ControlLoopTime;
	if(time*2*PI*50 >= 2*PI)
		time = 0;
	/*
	ClarkeTransform(0,Ia,Ib,Ic,0,0,0,&Controller);
	Controller.Theta = time*2*PI*50;
	ParkTransform(&Controller);*/
	ProbeScope_Sampling();
}

int main(void)
{
	DAVE_STATUS_t status;
	status = DAVE_Init();
	/* Initialization of DAVE APPs  */
	if (status != DAVE_STATUS_SUCCESS)
	{
		/* Placeholder for error handler code. The while loop below can be replaced with an user error handler. */
		XMC_DEBUG("DAVE APPs initialization failed\n");
		while (1U)
		{
		}
	}
	PWM_SVM_Start(&PWM_SVM_0);
	ProbeScope_Init(SystemCoreClock / TICKS_PER_SECOND); /// for micrium uScope
	/* Placeholder for user application code. The while loop below can be replaced with user application code. */
	while (1U)
	{

	}
}

void SpeedController(float RPM_Val, FOC Controller_Var)
{
	static float AccmErrRPM = 0, AuxRPM = 0;

	float ErrRPM = RPM_REF - RPM_Val;

	AuxRPM += 0.5 * T_PI_RPM * (ErrRPM + AccmErrRPM);

	Controller_Var.Iq_Ref = AuxRPM * KiSpeed + ErrRPM * KpSpeed;

	AccmErrRPM = ErrRPM;
}
void FieldWeakening(float RPM_Val, FOC Controller_Var)
{
	if (RPM_Val > RPM_NOMINAL)
	{
		Controller_Var.Id_Ref = ID_NOMINAL * RPM_NOMINAL / RPM_Val;
	}
	else
		Controller_Var.Id_Ref = ID_NOMINAL;
}
