#include "FOC.h"

float SpeedSensor(float inA,float inB)
{
	static int counter = 0;
	static float RPM = 0;
	static float prevIa = 0;
	static float prevIb = 0;
	if(((prevIa>0 && inA < 0) || (prevIa < 0 && inA > 0))  && counter == 0)
	{
		counter = 0;
	}
	if(((prevIb>0 && inB < 0) || (prevIb < 0 && inB > 0)) && counter != 0){
		RPM = 1/(4*counter)*60*TICKS_PER_SECOND;
		counter = 0;
	}
	counter++;
	prevIb = inB;
	prevIa = inA;
	return RPM;
}

void CurrentController(FOC* Controller)
{
	static float AccmErrD = 0,AccmErrQ = 0,AuxD = 0,AuxQ = 0;

	float ErrD = Controller->Id_Ref - Controller->Id;
	float ErrQ = Controller->Iq_Ref - Controller->Iq;

	AuxD += 0.5 * Controller->T_PI * (ErrD + AccmErrD);
	AuxQ += 0.5 * Controller->T_PI * (ErrQ + AccmErrQ);

	Controller->Vd_Ref = AuxD * Controller->Ki + ErrD * Controller->Kp;
	Controller->Vq_Ref = AuxQ * Controller->Ki + ErrQ * Controller->Kp;

	AccmErrD = ErrD;
	AccmErrQ = ErrQ;
}

void ThetaCalulation(float RPM,FOC* Controller)
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

void ClarkeTransform(int opt,float Ia,float Ib,float Ic,float Va,float Vb,float Vc,FOC* Controller)
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

void InvParkTransform(FOC* Controller,float* Val,float* Vbet)
{
	 // Limit voltage references between 1 and -1
	 float aux1, aux2;
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
