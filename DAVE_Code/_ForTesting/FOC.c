#include "FOC.h"

void ThetaEstimation(float32_t RPM,FOC* Controller)
{
	if(Controller->Id > 0)
		Controller->Theta += Controller->T_Theta * (abs(RPM) * 2 * PI / 60 * P / 2 + (Controller->Iq / Controller->Id * Rr / (Lr + Lm)));
	else
		Controller->Theta = 0;
	if (abs(Controller->Theta) >= 2 * PI)
	  {
		Controller->Theta = -2 * PI;
	  }
}

float32_t SpeedSensorCoarse(float32_t RPM_COS,float32_t RPM_SIN)
{
	static float32_t prev_RPM_COS = 0;
	static float32_t theta_Mec = 0,time_step = 0;
	float32_t RPM = 0;
	if (SignVal(RPM_COS) != SignVal(prev_RPM_COS))
	{
		if (theta_Mec >= 2*PI)
			theta_Mec = 0;
		theta_Mec += ANGLE_PER_ZERO;
		RPM = 1/(2*time_step)*60;
		time_step = 0;

	}
	prev_RPM_COS = RPM_COS;
	time_step += ACQUISITION_TIME;
	return RPM;
}

float32_t SpeedSensorFine(float32_t RPM_COS,float32_t RPM_SIN)
{
	static float32_t theta = 0;
	theta += arm_atan2();
	return 0;
}

void CurrentController(FOC* Controller)
{
	static float32_t AccmErrD = 0,AccmErrQ = 0,AuxD = 0,AuxQ = 0;

	float32_t ErrD = Controller->Id_Ref - Controller->Id;
	float32_t ErrQ = Controller->Iq_Ref - Controller->Iq;

	AuxD += 0.5 * Controller->T_PI * (ErrD + AccmErrD);
	AuxQ += 0.5 * Controller->T_PI * (ErrQ + AccmErrQ);

	Controller->Vd_Ref = AuxD * Controller->Ki + ErrD * Controller->Kp;
	Controller->Vq_Ref = AuxQ * Controller->Ki + ErrQ * Controller->Kp;

	AccmErrD = ErrD;
	AccmErrQ = ErrQ;
}

void ClarkeTransform(int opt,float32_t Ia,float32_t Ib,float32_t Ic,float32_t Va,float32_t Vb,float32_t Vc,FOC* Controller)
{
	if (opt == 0)
	{
		Controller->Ial = ((Ia - 0.5f * Ib - 0.5f * Ic)) * 0.66666666666666666666666667f;
		Controller->Ibet = ((sqrt(3) * 0.5f * Ib - sqrt(3) * 0.5f * Ic)) * 0.66666666666666666666666667f;
	}
	else if (opt == 1)
	{ // Power Invariant
	    Controller->Val = sqrt(3/2) * Va;
	    Controller->Vbet = sqrt(2) / 2 * Va + sqrt(2) * Vb;
	}
}

void ParkTransform(FOC* Controller)
{
	// Only used after Clarke(0) is performed
	Controller->Id = Controller->Ial * cos(Controller->Theta) + Controller->Ibet * sin(Controller->Theta);
	Controller->Iq = -Controller->Ial * sin(Controller->Theta) + Controller->Ibet * cos(Controller->Theta);
}

void InvParkTransform(FOC* Controller,float32_t* Val,float32_t* Vbet)
{
	 // Limit voltage references between 1 and -1
	 float32_t aux1, aux2;
	 aux1 = Controller->Vd_Ref  * cos(Controller->Theta) - Controller->Vq_Ref  * sin(Controller->Theta);
	 aux2 = Controller->Vd_Ref  * sin(Controller->Theta) + Controller->Vq_Ref  * cos(Controller->Theta);

	 /////////////////////////////////////////////
	 if (abs(aux1) > 1)
	 {
		 *Val = (aux1 > 0) - (aux1 < 0);
	 }
	 else
	     *Val = aux1;
	 /////////////////////////////////////////////
	 if (abs(aux2) > 1)
	 {
	    *Vbet = (aux2 > 0) - (aux2 < 0);
	 }
	 else
	     *Vbet = aux2;


	 Controller->V_Ref = sqrt(aux1*aux1 + aux2*aux2);
	 Controller->Theta_Ref = atan2(aux2,aux1);
}

uint32_t SignVal(float32_t sign)
{
	if (sign > 0)
		return 1;
	else if (sign < 0)
		return -1;
	else
		return 0;
}
