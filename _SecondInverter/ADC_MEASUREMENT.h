#include <Dave.h>
#ifndef _ADC_MEASUREMENT_H_
#define _ADC_MEASUREMENT_H_
/*
 * Sensors gain and bias definition please change
 * accordingly to sensors used.
 */
#define CURRENT_SENS_A_GAIN 0.0408000000000000f
#define CURRENT_SENS_B_GAIN 0.0408000000000000f
#define CURRENT_SENS_C_GAIN 0.0297500000000000f

#define CURRENT_SENS_A_BIAS -84.111000000000000f
#define CURRENT_SENS_B_BIAS -83.811000000000000f
#define CURRENT_SENS_C_BIAS -61.009999999999999f

#define CURRENT_SENS_A_OFFSET 0.0f
#define CURRENT_SENS_B_OFFSET 0.0f
#define CURRENT_SENS_C_OFFSET 0.0f

#define V_DC_SENS_GAIN 1.757505429f  /// VS3000B
#define V_DC_SENS_BIAS -218.6629394f /// VS3000B
#define V_DC_SENS_OFFSET 0

#define RPM_COS_SENS_GAIN 0.0f
#define RPM_SIN_SENS_GAIN 0.0f

#define RPM_COS_SENS_BIAS 0.0f
#define RPM_SIN_SENS_BIAS 0.0f

#define RPM_COS_SENS_OFFSET 0
#define RPM_SIN_SENS_OFFSET 0

/*
 * Not Currently in use
 */
#define TEMP_MOT_SENS_GAIN 0.0f
#define TEMP_MOT_SENS_BIAS 0.0f
#define TEMP_MOT_SENS_OFFSET 0

#define TEMP_IGBT_SENS_GAIN 0.0f
#define TEMP_IGBT_SENS_BIAS 0.0f
#define TEMP_IGBT_SENS_OFFSET 0

/*
 * Un-filtered buffer size and format
 */

float32_t UN_FILT_VALS[8];

#define CURRENT_A_UNFILT UN_FILT_VALS[0]
#define CURRENT_B_UNFILT UN_FILT_VALS[1]
#define CURRENT_C_UNFILT UN_FILT_VALS[2]
#define V_DC_UNFILT UN_FILT_VALS[3]
#define TEMP_SENS_MOT_UNFILT UN_FILT_VALS[4]
#define TEMP_SENS_IGBT_UNFILT UN_FILT_VALS[5]
#define RPM_COS_UNFILT UN_FILT_VALS[6]
#define RPM_SIN_UNFILT UN_FILT_VALS[7]

/*
 * Filtered buffer size and format
 */

float32_t FILT_VALS[8];

#define CURRENT_A FILT_VALS[0]
#define CURRENT_B FILT_VALS[1]
#define CURRENT_C FILT_VALS[2]
#define V_DC_REAL FILT_VALS[3]
#define TEMP_SENS_MOT FILT_VALS[4]
#define TEMP_SENS_IGBT FILT_VALS[5]
#define RPM_COS FILT_VALS[6]
#define RPM_SIN FILT_VALS[7]

/*
 * Filter instances and corresponding coefficients
 */
/*
//Current Sensors Filters
arm_fir_instance_f32 FIR_CURRENT_A;
arm_fir_instance_f32 FIR_CURRENT_B;
arm_fir_instance_f32 FIR_CURRENT_C;

#define FIR_CURRENT_numTaps	23
#define	FIR_CURRENT_blockSize 1

static float32_t FIR_CURRENT_A_pState[FIR_CURRENT_numTaps + FIR_CURRENT_blockSize - 1 ];
static float32_t FIR_CURRENT_B_pState[FIR_CURRENT_numTaps + FIR_CURRENT_blockSize - 1 ];
static float32_t FIR_CURRENT_C_pState[FIR_CURRENT_numTaps + FIR_CURRENT_blockSize - 1 ];

//Speed Sensor Filters
arm_fir_instance_f32 FIR_RPM_COS;
arm_fir_instance_f32 FIR_RPM_SIN;

#define FIR_RPM_numTaps	101
#define	FIR_RPM_blockSize 1

float32_t FIR_RPM_COS_pState[FIR_RPM_numTaps + FIR_RPM_blockSize - 1 ];
float32_t FIR_RPM_SIN_pState[FIR_RPM_numTaps + FIR_RPM_blockSize - 1 ];

//Voltage Sensor Filter
arm_fir_instance_f32 FIR_V_DC;

#define FIR_V_DC_numTaps	103
#define	FIR_V_DC_blockSize 1

float32_t FIR_V_DC_pState[FIR_V_DC_numTaps + FIR_V_DC_blockSize - 1 ];

/*
 * Filter coefficients
 */
/*
const float32_t FIR_V_DC_pCoeffs[FIR_V_DC_numTaps] =
	{

};

const float32_t FIR_RPM_pCoeffs[FIR_RPM_numTaps] =
	{

};

const float32_t FIR_CURRENT_pCoeffs[FIR_CURRENT_numTaps] =
	{
		0.005450753093095797,
		0.017411033520375457,
		0.017487739846213914,
		0.009444597835027853,
		-0.014232463896468903,
		-0.03890720894242469,
		-0.04517002774491089,
		-0.013319761849134302,
		0.0595150695172939,
		0.1539078380254452,
		0.23394362067460858,
		0.26532766218886455,
		0.23394362067460858,
		0.1539078380254452,
		0.0595150695172939,
		-0.013319761849134302,
		-0.04517002774491089,
		-0.03890720894242469,
		-0.014232463896468903,
		0.009444597835027853,
		0.017487739846213914,
		0.017411033520375457,
		0.005450753093095797};
*/

#endif
