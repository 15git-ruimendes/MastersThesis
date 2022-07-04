#include "FOC.h"

const float32_t A_COEFF[FILTER_ORDER + 1] = {1.000000000000,-1.929943387609,0.932579065283};
const float32_t B_COEFF[FILTER_ORDER + 1] = {0.000655137270,0.001310274539,0.000655137270};

static float32_t PI_H = 1.57079632679;
static float32_t PI_H_NEG = -1.57079632679;
static float32_t ATAN2[SizeAc + 1];

void ThetaEstimation(float32_t RPM, FOC *Controller)
{
	//Controller->Theta += Controller->T_Theta * (abs(RPM) * 2 * PI / 60 * P * 0.5 + (Controller->Iq / Controller->Id * Rr / (Lr + Lm)));
	//Controller->Theta += ACQUISITION_TIME * (RPM * 2 * PI * 0.0166666666666667 * P * 0.5);
	if (Controller->Id > 0)
		Controller->Theta += (Controller->Iq / Controller->Id * 0.001 / 0.006);
	else
		Controller->Theta = 0;
	if (abs(Controller->Theta) >= 2 * PI)
	{
		Controller->Theta = 0;
	}
}

float32_t SpeedSensorCoarse(float32_t RPM_COS, float32_t RPM_SIN)
{
	static float32_t prev_RPM_COS = 0;
	static float32_t theta_Mec = 0, time_step = 0;
	float32_t RPM = 0;
	if (SignVal(RPM_COS) != SignVal(prev_RPM_COS))
	{
		if (theta_Mec >= 2 * PI)
			theta_Mec = 0;
		theta_Mec += ANGLE_PER_ZERO;
		RPM = 1 / (2 * time_step) * 60;
		time_step = 0;
	}
	prev_RPM_COS = RPM_COS;
	time_step += ACQUISITION_TIME;
	return RPM;
}

float32_t SpeedSensorFine(float32_t RPM_COS, float32_t RPM_SIN)
{
	return 0;
}

void CurrentController(FOC *Controller,uint32_t Reset,float32_t V_DC)
{
	static float32_t AccmErrD = 0, AccmErrQ = 0, AuxD = 0, AuxQ = 0,Anti_WindUp = 1;

	if(Reset == 1){
		AuxD = 0;
		AuxQ = 0;
	}

	/*float32_t ErrD = Controller->Id_Ref - Controller->Id;
	float32_t ErrQ = Controller->Iq_Ref - Controller->Iq;

	AuxD += 0.5 * Controller->T_PI * (ErrD + AccmErrD) * Anti_WindUp;
	AuxQ += 0.5 * Controller->T_PI * (ErrQ + AccmErrQ) * Anti_WindUp;

	Controller->Vd_Ref = (AuxD * Controller->Ki + ErrD * Controller->Kp);
	Controller->Vq_Ref = (AuxQ * Controller->Ki + ErrQ * Controller->Kp);

	AccmErrD = ErrD;
	AccmErrQ = ErrQ;
	*/

	float32_t ErrD = Controller->Id_Ref - Controller->Id;
	float32_t ErrQ = Controller->Iq_Ref - Controller->Iq;

	float32_t PI_Out_PrevD = Controller->Vd_Ref;
	float32_t PI_Out_PrevQ = Controller->Vq_Ref;

	float32_t OutD = (PI_Out_PrevD) + (Controller->Ki*0.5*Controller->T_PI*(ErrD + AccmErrD) + (ErrD - AccmErrD) * Controller->Kp)*Anti_WindUp;
	float32_t OutQ = (PI_Out_PrevQ) + (Controller->Ki*0.5*Controller->T_PI*(ErrQ + AccmErrQ) + (ErrQ - AccmErrQ) * Controller->Kp)*Anti_WindUp;

	AccmErrD = ErrD;
	AccmErrQ = ErrQ;



	if (abs(OutD) >= 1 || abs(OutQ) >= 1)
	{
		Anti_WindUp = 0;
		Controller->Vd_Ref = (float32_t)SignVal(OutD);
		Controller->Vq_Ref = (float32_t)SignVal(OutQ);
	}
	else
	{
		Anti_WindUp = 1;
		Controller->Vd_Ref = OutD;
		Controller->Vq_Ref = OutQ;
	}

}

void ClarkeTransform(int opt, float32_t Ia, float32_t Ib, float32_t Ic, float32_t Va, float32_t Vb, float32_t Vc, FOC *Controller)
{
	if (opt == 0)
	{
		Controller->Ial = ((Ia - 0.5f * Ib - 0.5f * Ic)) * 0.66666666666666666666666667f;
		Controller->Ibet = -((SQRT_3 * 0.5f * Ib - SQRT_3 * 0.5f * Ic)) * 0.66666666666666666666666667f;
	}
	else if (opt == 1)
	{ // Power Invariant
		Controller->Val = SQRT_3_2 * Va;
		Controller->Vbet = SQRT_2 * 0.5 * Va + SQRT_2 * Vb;
	}
}

void ParkTransform(FOC *Controller)
{
	// Only used after Clarke(0) is performed
	Controller->Id = Controller->Ial * arm_cos_f32(Controller->Theta) + Controller->Ibet * arm_sin_f32(Controller->Theta);
	Controller->Iq = -Controller->Ial * arm_sin_f32(Controller->Theta) + Controller->Ibet * arm_cos_f32(Controller->Theta);
}

void InvParkTransform(FOC *Controller, float32_t *Val, float32_t *Vbet)
{
	// Limit voltage references between 1 and -1
	float32_t aux1, aux2;
	aux1 = Controller->Vd_Ref * arm_cos_f32(Controller->Theta) - Controller->Vq_Ref * arm_sin_f32(Controller->Theta);
	aux2 = Controller->Vd_Ref * arm_sin_f32(Controller->Theta) + Controller->Vq_Ref * arm_cos_f32(Controller->Theta);

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

	float32_t aux;
	float32_t angle = icecoreATAN2(aux2, aux1) + PI;
	arm_sqrt_f32((aux1 * aux1 + aux2 * aux2),&aux);
	if (aux >= 1)
		aux = 0.999;
	Controller->V_Ref = (uint16_t)(aux * MAX_SVM_VAL);
	Controller->Theta_Ref = (uint32_t)(angle / (2 * PI) * 0xFFFFFF);
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

float32_t Derivative(float32_t input, derivative *deriv_inst)
{
	float32_t aux;
	aux = (input - deriv_inst->previous_in) / deriv_inst->time_step;
	deriv_inst->previous_in = input;
	return aux;
}

float32_t IIRFilter(float32_t input, Butter *Filter)
{
	float32_t num_aux = 0, den_aux = 0, out_aux = 0;
	int i = 0;
	for (i = 0; i < FILTER_ORDER; i++)
	{
		num_aux += B_COEFF[i+1]*Filter->buff[i];
		den_aux += A_COEFF[i+1]*Filter->buff_out[i];
	}
	num_aux += B_COEFF[0]*input;
	out_aux = (num_aux - den_aux);
	for (i = FILTER_ORDER; i > 0; i--)
	{
		Filter->buff[i] = Filter->buff[i-1];
		Filter->buff_out[i] = Filter->buff_out[i-1];
	}
	Filter->buff[0] = input;
	Filter->buff_out[0] = out_aux;
	return out_aux;
}

float32_t ButterworthFilter(float32_t input, Butter *Filter)
{

	float32_t num_aux =0, den_aux = 0,aux_out = 0;
	int i = 1;

	for (i = FILTER_ORDER; i > 0; i--)
	{

		Filter->buff[i] = Filter->buff[i - 1];
	}
	Filter->buff[0] = input;

	for (i = 0; i < FILTER_ORDER + 1; i++)
	{
		num_aux += B_COEFF[i] * Filter->buff [i];
		if(i > 0)
			den_aux += A_COEFF[i] * Filter->buff_out[i];
	}

	for (i = FILTER_ORDER; i > 1; i--)
	{
		Filter->buff_out[i] = Filter->buff_out[i - 1];
	}
	aux_out = (num_aux - den_aux);
	Filter->buff_out[1] = aux_out;

	return  aux_out;
}

uint32_t CheckValues(float32_t Ia_Not_Filt, float32_t Ib_Not_Filt, float32_t Ic_Not_Filt, float32_t RPM_VAL, float32_t V_DC_UNFILT,uint32_t* WarnValue)
{
	if (Ia_Not_Filt >= CURRENT_LIM ||Ib_Not_Filt >= CURRENT_LIM || Ic_Not_Filt >= CURRENT_LIM)
	{
		*WarnValue = 3;
		return 1;
	}
	if ((Ia_Not_Filt + Ib_Not_Filt + Ic_Not_Filt) >= CURRENT_LIM_BALANCE)
	{
		*WarnValue = 3;
		return 2;
	}
	if (RPM_VAL >= MAX_RPM)
	{
		*WarnValue = 3;
		return 3;
	}
	if (V_DC_UNFILT >= MAX_VDC)
	{
		*WarnValue = 3;
		return 4;
	}
	if (V_DC_UNFILT >= MAX_VDC_WARN || RPM_VAL >= MAX_RPM_WARN || Ia_Not_Filt >= CURRENT_LIM_WARN ||Ib_Not_Filt >= CURRENT_LIM_WARN || Ic_Not_Filt >= CURRENT_LIM_WARN || (Ia_Not_Filt + Ib_Not_Filt + Ic_Not_Filt) >= CURRENT_LIM_BALANCE_WARN)
		*WarnValue = 2;
	return 0;
}

void initializeATAN2()
{
    for (int i = 0; i <= SizeAc; i++)
    {
        float32_t d = (float32_t)i / SizeAc;
        float32_t x = 1;
        float32_t y = x * d;
        ATAN2[i] = (float32_t)atan2(y, x);
    }
}

float32_t icecoreATAN2(float32_t y, float32_t x)
{
    if (y < 0)
    {
        if (x < 0)
        {
            // (y < x) because == (-y > -x)
            if (y < x)
            {
                return PI_H_NEG - ATAN2[(int)(x / y * SizeAc)];
            }
            else
            {
                return ATAN2[(int)(y / x * SizeAc)] - PI;
            }
        }
        else
        {
            y = -y;
            if (y > x)
            {
                return ATAN2[(int)(x / y * SizeAc)] - PI_H;
            }
            else
            {
                return -ATAN2[(int)(y / x * SizeAc)];
            }
        }
    }
    else
    {
        if (x < 0)
        {
            x = -x;
            if (y > x)
            {
                return PI_H + ATAN2[(int)(x / y * SizeAc)];
            }
            else
            {
                return PI - ATAN2[(int)(y / x * SizeAc)];
            }
        }
        else
        {
            if (y > x)
            {
                return PI_H - ATAN2[(int)(x / y * SizeAc)];
            }
            else
            {
                return ATAN2[(int)(y / x * SizeAc)];
            }
        }
    }
}

float32_t auxAngleCalc(float32_t Frequency, float32_t TimeStep, float32_t CurrentAngle ,uint32_t* CurrentAngleU, uint32_t option)
{
	float32_t IncrementVariable = 2*PI*Frequency*TimeStep,NextAngle = 0;
	uint32_t IncrementVariableU = Frequency*TimeStep*0xFFFFFF;
	if (option == 0)
	{
		NextAngle = CurrentAngle + IncrementVariable;
		if (NextAngle >= 2*PI)
			NextAngle = 0;
		return NextAngle;
	}
	else if (option == 1)
	{
		*CurrentAngleU = *CurrentAngleU + IncrementVariableU;
		if (*CurrentAngleU >= 0xFFFFFF)
			*CurrentAngleU = 0;
		return 0;
	}

}
