#include <Dave.h>
#include "FOC.h"
#include "ADC_MEASUREMENT.h"
#include <probe_scope.h>
#include <probe_scope_cfg.h>

#define KpSpeed 0.105f
#define KiSpeed 0.0075f

/*
 * Stopping running of XMC code
 */
uint32_t Stop = 0;
uint32_t Enable = 0;
uint32_t ResetPI = 0;

/*
 * 0 - Stand alone inverter (SVM & No current feedback (no aquisition loop))
 * 1 - Open loop test (Control of Id and Iq Curents (w/ load or not))
 * 2 - Closed loop motor test (no-load,CurrentControl & RPM reading)
 * 3 - Closed loop motor test (0.5 load,nominal speed ref)
 */
#define TEST_CONFIG 0

//Test Configuration Extra Variables
uint32_t Theta_Aux_No_Use = 0;
uint16_t Amplitude_SVM_No_Use = 16000;

float32_t Frequency_Ref_No_Use = 50.0f;

float32_t Theta_Incm_VarFreq_1k2_No_Use = 0;

float32_t Id_Ref_No_Use = 0;
float32_t Iq_Ref_No_Use = 0;

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
float32_t Ia, IaFilt, Ib, Ic;
float32_t Val, Vbet;
float32_t V_DC;
float32_t Temp_Mot, Temp_IGBT;
float32_t Theta;

// Reference and timing variables
float32_t RPM_REF;

// Cycle counters
uint32_t ControlCycleCounter = 0;
uint32_t MeasurmentCycleCounter = 0;
float32_t Timer_1K2Hz = 0;
float32_t Timer_500Hz = 0;
float32_t Timer_Ref = 45;

// Filter init
Butter Ia_Current_Filt;
Butter Ib_Current_Filt;
Butter Ic_Current_Filt;
Butter V_DC_Filt;

// Error Variables
float32_t ErrorCode = 0;
uint32_t WarningValue = 1;
float32_t RefD[5] = {0.75, 0.450, 3.12, 2.2, 0.40}, RefQ[5] = {6.0, 5.0, 12.0, 10.0, 7.0};
int indexRef = 0;
float32_t PrevEnable = 0;

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
	CURRENT_A_UNFILT = ((float32_t)(ADC_MEASUREMENT_ADV_GetResult(&ADC_CURRENT_A) - CURRENT_SENS_A_OFFSET)) * CURRENT_SENS_A_GAIN + CURRENT_SENS_A_BIAS;
	CURRENT_B_UNFILT = ((float32_t)(ADC_MEASUREMENT_ADV_GetResult(&ADC_CURRENT_B) - CURRENT_SENS_B_OFFSET)) * CURRENT_SENS_B_GAIN + CURRENT_SENS_B_BIAS;
	CURRENT_C_UNFILT = ((float32_t)(ADC_MEASUREMENT_ADV_GetResult(&ADC_CURRENT_C) - CURRENT_SENS_C_OFFSET)) * CURRENT_SENS_C_GAIN + CURRENT_SENS_C_BIAS;
	V_DC_UNFILT = ((float32_t)(ADC_MEASUREMENT_ADV_GetResult(&ADC_V_DC) - V_DC_SENS_OFFSET)) * V_DC_SENS_GAIN + V_DC_SENS_BIAS;
	//TEMP_SENS_MOT_UNFILT =	((float)(ADC_MEASUREMENT_ADV_GetResult(&ADC_TEMP_SENS_MOT) - TEMP_MOT_SENS_OFFSET)) * TEMP_MOT_SENS_GAIN + TEMP_MOT_SENS_BIAS;
	//TEMP_SENS_IGBT_UNFILT =	((float)(ADC_MEASUREMENT_ADV_GetResult(&ADC_TEMP_SENS_IGBT) - TEMP_IGBT_SENS_OFFSET)) * TEMP_IGBT_SENS_GAIN + TEMP_IGBT_SENS_BIAS;
	//RPM_COS_UNFILT = 			((float)(ADC_MEASUREMENT_ADV_GetResult(&ADC_RPM_COS) - RPM_COS_SENS_OFFSET)) * RPM_COS_SENS_GAIN + RPM_COS_SENS_BIAS;
	//RPM_SIN_UNFILT = 			((float)(ADC_MEASUREMENT_ADV_GetResult(&ADC_RPM_SIN) - RPM_SIN_SENS_OFFSET)) * RPM_SIN_SENS_GAIN + RPM_SIN_SENS_BIAS;

	Ib = IIRFilter(CURRENT_A_UNFILT, &Ia_Current_Filt);
	Ia = IIRFilter(CURRENT_B_UNFILT, &Ib_Current_Filt);
	Ic = IIRFilter(CURRENT_C_UNFILT, &Ic_Current_Filt);

	V_DC = IIRFilter(V_DC_UNFILT, &V_DC_Filt);

	//Ia_Unfilt = CURRENT_A_UNFILT;
	//Ib_Unfilt = CURRENT_B_UNFILT;
	//Ic_Unfilt = CURRENT_C_UNFILT;

	Stop = CheckValues(Ia, Ib, Ic, VAL_RPM, V_DC, &WarningValue);

	ProbeScope_Sampling();
}


int main(void)
{
	DAVE_STATUS_t status;
	status = DAVE_Init();
	/* Initialization of DAVE APPs  */

	if (status != DAVE_STATUS_SUCCESS)
	{
		XMC_DEBUG("DAVE APPs initialization failed\n");
		while (1U)
		{
		}
	}
	//Place all enables at 1
	BUS_IO_Write(&ENABLE_BUS, 0x000f);

	//Clear all faults (maybe removed)
	BUS_IO_Write(&FAULT_BUS, 0x0000);

	//Disable all discrete outputs
	BUS_IO_Write(&OUTPUT_DISCRETE, 0x0000);


	//Disable all discrete inputs (i.e. place pins at 0)
	BUS_IO_Write(&INPUT_DISCRETE, 0x0000);

	//Clear all faults
	DIGITAL_IO_SetOutputLow(&RESET_FAULTS);

	//uC Probe Init
	ProbeScope_Init(SystemCoreClock / TICKS_PER_SECOND);

	INTERRUPT_Enable(&ACQUISITION_LOOP);

	while (Stop == 0)
	{
		if (PrevEnable != Enable && Enable == 0)
		{
			PrevEnable = Enable;
			DIGITAL_IO_SetOutputLow(&DIGITAL_IO_0);
			BUS_IO_Write(&OUTPUT_DISCRETE, 0x0000);
		}
		else if (PrevEnable != Enable && Enable == 1)
		{
			PrevEnable = Enable;
			DIGITAL_IO_SetOutputHigh(&DIGITAL_IO_0);
			BUS_IO_Write(&OUTPUT_DISCRETE, 0x0001);
		}
	}
	DIGITAL_IO_SetOutputLow(&DIGITAL_IO_0);
	Enable = 0;
	while (1)
	{
	}
	INTERRUPT_Disable(&ACQUISITION_LOOP);
}


