#include <Dave.h>
#include "FOC.h"
#include "ADC_MEASUREMENT.h"
#include <probe_scope.h>
#include <probe_scope_cfg.h>

#define SECOND_PER_TICKS 0.001f

#define KpSpeed 10
#define KiSpeed 0.01
#define T_PI_RPM 0.1
#define ACQUISITION_TIME 0.000050
#define CONTROL_TIME 0.00083
#define FAULT_CHECK_TIME 0.001

/*
 * 0 - Stand alone inverter (SVM & Bellow Nominal Current Ref)
 * 1 - Open loop motor test (Nominal Id and Iq Curents)
 * 2 - Closed loop motor test (no-load,0.75 nominal speed ref)
 * 3 - Closed loop motor test (0.5 load,nominal speed ref)
 * 4 - Closed loop nominal load test
 */
#define TestVal 0

// Global Variables
/*
 * Main controller structure defined in FOC.h.
 * Manipulated and engaged in CONTROL_LOOP
 */
FOC Controller;

// ADC Measurements
/*
 * Primary measurements, filtered and compensated.
 * Read and manipulated in ACQUISITION_LOOP
 */
float32_t COS_RPM, SIN_RPM, VAL_RPM;
float32_t Ia, Ib, Ic;
float32_t Val, Vbet;
float32_t V_DC;
float32_t Temp_Mot, Temp_IGBT;
float32_t Theta;

// Reference and timing variables
float32_t RPM_REF;
float32_t time = 0, unfilt[FIR_CURRENT_blockSize], filt, aux;

// Cycle counters
uint32_t ControlCycleCounter = 0;
uint32_t MeasurmentCycleCounter = 0;

// Speed Controller
/*
 * @brief PI Speed controller tuned to a slower dynamic response then current controller
 * @params RPM_Val Current speed value in RPM
 * @params Controller_Var FOC structure, function will change IqRef value limited to IQ_LIM
 */
void SpeedController(float32_t RPM_Val, FOC *Controller_Var);

// Field Weakening
/*
 * @brief Implements field weakening by reducing Id reference from nominal when current speed is greater than nominal by
 *  					wNominal
 * Id = IdNominal * ----------------
 * 							w
 * @params RPM_Val Current speed value in RPM
 * @params Controller_Var FOC structure, function will change IdRef value limited to ID_MIN
 */
void FieldWeakening(float32_t RPM_Val, FOC *Controller_Var);

////////////////////////////////////////////////
/*
 * ISR ROUTINES
 */
////////////////////////////////////////////////
/*
 * Acquisition Loop
 * Saved ADC_QUEUE variables to global variables namely:
 * CURRENT_A - Ia
 * Current_B - Ib
 * Current_C - Ic
 * V_DC - V_DC
 * RPM_COS - RPM_COS
 * RPM_SIN - RPM_SIN
 * TEMP_SENS_MOT - Temp_Mot
 * TEMP_SENS_IGBT - Temp_IGBT
 */
void ACQUISITION_LOOP_FUNC()
{
	CURRENT_A_UNFILT = ((float)(ADC_MEASUREMENT_ADV_GetResult(&ADC_CURRENT_A) - CURRENT_SENS_A_OFFSET)) * CURRENT_SENS_A_GAIN + CURRENT_SENS_A_BIAS;
	CURRENT_B_UNFILT = ((float)(ADC_MEASUREMENT_ADV_GetResult(&ADC_CURRENT_B) - CURRENT_SENS_B_OFFSET)) * CURRENT_SENS_B_GAIN + CURRENT_SENS_B_BIAS;
	CURRENT_C_UNFILT = ((float)(ADC_MEASUREMENT_ADV_GetResult(&ADC_CURRENT_C) - CURRENT_SENS_C_OFFSET)) * CURRENT_SENS_C_GAIN + CURRENT_SENS_C_BIAS;
	V_DC_UNFILT = ((float)(ADC_MEASUREMENT_ADV_GetResult(&ADC_V_DC) - V_DC_SENS_OFFSET)) * V_DC_SENS_GAIN + V_DC_SENS_BIAS;
	TEMP_SENS_MOT_UNFILT = ((float)(ADC_MEASUREMENT_ADV_GetResult(&ADC_TEMP_SENS_MOT) - TEMP_MOT_SENS_OFFSET)) * TEMP_MOT_SENS_GAIN + TEMP_MOT_SENS_BIAS;
	TEMP_SENS_IGBT_UNFILT = ((float)(ADC_MEASUREMENT_ADV_GetResult(&ADC_TEMP_SENS_IGBT) - TEMP_IGBT_SENS_OFFSET)) * TEMP_IGBT_SENS_GAIN + TEMP_IGBT_SENS_BIAS;
	RPM_COS_UNFILT = ((float)(ADC_MEASUREMENT_ADV_GetResult(&ADC_RPM_COS) - RPM_COS_SENS_OFFSET)) * RPM_COS_SENS_GAIN + RPM_COS_SENS_BIAS;
	RPM_SIN_UNFILT = ((float)(ADC_MEASUREMENT_ADV_GetResult(&ADC_RPM_SIN) - RPM_SIN_SENS_OFFSET)) * RPM_SIN_SENS_GAIN + RPM_SIN_SENS_BIAS;

	arm_fir_f32(&FIR_CURRENT_A, &(CURRENT_A_UNFILT), &Ia, FIR_CURRENT_blockSize);
	arm_fir_f32(&FIR_CURRENT_B, &(CURRENT_B_UNFILT), &Ib, FIR_CURRENT_blockSize);
	arm_fir_f32(&FIR_CURRENT_C, &(CURRENT_B_UNFILT), &Ic, FIR_CURRENT_blockSize);

	arm_fir_f32(&FIR_V_DC, &(V_DC_UNFILT), &V_DC, FIR_V_DC_blockSize);

	arm_fir_f32(&FIR_RPM_COS, &(RPM_COS_UNFILT), &COS_RPM, FIR_RPM_blockSize);
	arm_fir_f32(&FIR_RPM_SIN, &(RPM_SIN_UNFILT), &SIN_RPM, FIR_RPM_blockSize);

	CheckValues();
	VAL_RPM = SpeedSensorCoarse(COS_RPM, SIN_RPM);

	time += ACQUISITION_TIME;
	ControlCycleCounter++;
	ProbeScope_Sampling();
}

void CONTROL_LOOP_FUNC()
{
	// if (ControlCycleCounter % ) //Subroutine @ 1000Hz
	SpeedController(VAL_RPM, &Controller);
	FieldWeakening(VAL_RPM, &Controller);

	ThetaEstimation(VAL_RPM, &Controller);
	ClarkeTransform(0, Ia, Ib, Ic, 0, 0, 0, &Controller);
	ParkTransform(&Controller);

	CurrentController(&Controller);
	InvParkTransform(&Controller, &Val, &Vbet);

	PWM_SVM_SVMUpdate(&PWM_SVM_0, Controller.V_Ref, Controller.Theta_Ref);
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
	arm_fir_init_f32(&FIR_CURRENT_A, FIR_CURRENT_numTaps, FIR_CURRENT_pCoeffs, FIR_CURRENT_A_pState, FIR_CURRENT_blockSize);
	ProbeScope_Init(SystemCoreClock / TICKS_PER_SECOND); /// for micrium uScope
	/* Placeholder for user application code. The while loop below can be replaced with user application code. */
	while (1U)
	{
	}
}

void SpeedController(float32_t RPM_Val, FOC *Controller_Var)
{
	static float AccmErrRPM = 0, AuxRPM = 0;

	float32_t ErrRPM = RPM_REF - RPM_Val;

	AuxRPM += 0.5 * T_PI_RPM * (ErrRPM + AccmErrRPM);

	Controller_Var->Iq_Ref = AuxRPM * KiSpeed + ErrRPM * KpSpeed;

	AccmErrRPM = ErrRPM;
}
void FieldWeakening(float32_t RPM_Val, FOC *Controller_Var)
{
	if (RPM_Val > RPM_NOMINAL)
	{
		Controller_Var->Id_Ref = ID_NOMINAL * RPM_NOMINAL / RPM_Val;
	}
	else
		Controller_Var->Id_Ref = ID_NOMINAL;
}
