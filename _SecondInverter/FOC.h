#ifndef _FOC_H_
#define _FOC_H_

#include <Dave.h>
#include <math.h>
#include <stdlib.h>

/*
* System variables
*/
#define TICKS_PER_SECOND 25000
#define ANGLE_PER_ZERO 0.01f
#define SQRT_3 1.7320508075688772935274463415059f
#define SQRT_2 1.4142135623730950488016887242097f
#define SQRT_3_2 1.2247448713915890490986420373529f
#define SQRT_1_2 0.7071067811865475244008443621048f
#define SQRT_1_3 0.5773502691896257645091487805019f

#define ACQUISITION_TIME 0.000040f
#define CONTROL_TIME 0.00008333333f
#define FREQ_1k2_Hz 0.0008333333333333333333333333333f
#define FREQ_500_Hz 0.02f
#define MAX_SVM_VAL 24999.9f
/*
 * Initial Motor Parameters
 */
#define Rs 0.03552f
#define Ls 0.000335f
#define Rr 0.02092f
#define Lr 0.000335f
#define Lm 0.0151f
#define P 4.0f
#define J 1.2f
#define RPM_NOMINAL 1500.0f
#define ID_NOMINAL 62.3f
#define IQ_MAX 315.0f
#define ID_MIN 30.0f

/*
* Filter variables
*/
#define FILTER_ORDER 2u

/*
 * Safe Operating Values
 */
#define CURRENT_LIM				 19.0f
#define CURRENT_LIM_BALANCE		 10000.0f
#define MAX_RPM					 14000.0f
#define MAX_VDC					 150.0f

#define CURRENT_LIM_WARN		 15.0f
#define CURRENT_LIM_BALANCE_WARN 1000.0f
#define MAX_RPM_WARN			 10000.0f
#define MAX_VDC_WARN			 160.0f

/*
 * ATAN2 Fast
 */
#define SizeAc 4096u
/*
 * @brief FOC Struct: Structure used to define current controller gain,
 * and current and voltage transform values
 */
typedef struct FOC
{
	float32_t Kp;
	float32_t Ki;
	float32_t T_PI;
	float32_t T_Theta;
	float32_t Theta;
	float32_t Ial;
	float32_t Ibet;
	float32_t Val;
	float32_t Vbet;
	float32_t Id;
	float32_t Iq;
	float32_t Id_Ref;
	float32_t Iq_Ref;
	float32_t Vd_Ref;
	float32_t Vq_Ref;
	uint16_t V_Ref;
	uint32_t Theta_Ref;
} FOC;

typedef struct
{
	float32_t previous_in;
	float32_t time_step;
} derivative;

typedef struct
{
	float32_t buff[FILTER_ORDER + 1];
	float32_t buff_out[FILTER_ORDER + 1];
	float32_t A[FILTER_ORDER + 1];
	float32_t B[FILTER_ORDER + 1];
} Butter;

/*
 * @brief Calculation of electrical angle
 * given current references and RPM value
 * @param RPM Instant mechanical speed in RPM
 * @param Controller FOC type structure containing relevant parameters (requires initialization)
 */
void ThetaEstimation(float32_t RPM, FOC *Controller);

/*
 * @brief Adaptation of speed sensor signals
 * to return current speed value in RPM using
 * zero passage detection (coarse signals).
 * @param RPM_COS First Sinusoidal Signal
 * @param RPM_SIN Second sinusoidal signal (90 degree delay)
 * @param Controller FOC type structure containing relevant parameters (requires initialization)
 */
float32_t SpeedSensorCoarse(float32_t RPM_COS, float32_t RPM_SIN);

/*
 * @brief Adaptation of speed sensor signals
 * to return current speed value in RPM using
 * atan2 calculation (fine resolution)(! Not Implemented !).
 * @param RPM_COS First Sinusoidal Signal
 * @param RPM_SIN Second sinusoidal signal (90 degree delay)
 * @param Controller FOC type structure containing relevant parameters (requires initialization)
 */
float32_t SpeedSensorFine(float32_t RPM_COS, float32_t RPM_SIN);

/*
 * @brief Calulating D and Q voltage references using PI controller
 * @param Controller FOC type structure containing relevant parameters (requires initialization)
 */
void CurrentController(FOC *Controller,uint32_t Reset,float32_t V_DC);

/*
 * @brief Clake transform. If opt = 0 then transform will be applied to Ia, Ib, Ic currents,
 * if opt = 1 then it will be performed to Va, Vb, Vc.
 * @params opt Option for current or voltage transform
 * @params Ia
 * @params Ib
 * @params Ic
 * @params Va
 * @params Vb
 * @params Vc
 * @params Controller FOC type structure containing relevant parameters (requires initialization)
 */
void ClarkeTransform(int opt, float32_t Ia, float32_t Ib, float32_t Ic, float32_t Va, float32_t Vb, float32_t Vc, FOC *Controller);

/*
 * @brief Park transform. Applied to Ial, Ibet currents already present in Controller structure
 * (! Needs to follow ClarkeTransform)
 * @params Controller FOC type structure containing relevant parameters (requires initialization)
 */
void ParkTransform(FOC *Controller);

/*
 * @brief Inverse Park transform. Applied only to VdRef and VqRef already within Controller structure (must be used after PI controllers)
 * also calulates V_Ref and Theta to be used in SVMPWM app.
 * @params Controller FOC type structure containing relevant parameters (requires initialization)
 */
void InvParkTransform(FOC *Controller, float32_t *Val, float32_t *Vbet);

/*
 * @brief Determine the signal of the input value
 * returns 1 if positive, -1 if negative and 0 if zero
 * @params sign input signal
 */
uint32_t SignVal(float32_t sign);

/*
 * @brief Derivative calculation formula calculated at time_step
 * @params input New value to calculate derivative value
 * @params deriv_inst Instance of derivative calculation
 */
float32_t Derivative(float32_t input, derivative *deriv_inst);

/*
 * @brief Butterworth filter implementation. Order and coeffs
 * must be defined previously before running Filter function.
 * @params input New value to add to buffer
 * @params Filter Butter structure variable
 */
float32_t ButterworthFilter(float32_t input, Butter *Filter);
float32_t IIRFilter(float32_t input, Butter *Filter);

/*
 * @brief Butterworth filter implementation. Order and coeffs
 * must be defined previously before running Filter function.
 * @params input New value to add to buffer
 * @params Filter Butter structure variable
 */
float32_t FirstOrderFilter(float32_t input, float32_t *PrevValue);


/*
 * @brief CheckValues function to check limit values and turn on the shutdown procedure
 * @params Ia_Not_Filt Not filtered current (but with gain and balance)
 * @params Ib_Not_Filt Not filtered current (but with gain and balance)
 * @params Ic_Not_Filt Not filtered current (but with gain and balance)
 * @params RPM_VAL Speed values
 */
uint32_t CheckValues(float32_t Ia_Not_Filt, float32_t Ib_Not_Filt, float32_t Ic_Not_Filt, float32_t RPM_VAL, float32_t V_DC_UNFILT,uint32_t* WarnValue);

/*
 * @brief icecoreATAN2 is a memory map to the atan values of multiple reference numbers needs to be run only after running initializeATAN2
 * @params y Numerator value for the calculation of atan(y/x)
 * @params x Denominator value for the calculation of atan(y/x)
 */
float32_t icecoreATAN2(float32_t y, float32_t x);

/*
 * @brief initializeATAN2 performs the preset work of calculating SizeAc number of atan2 values and filling out ATAN2 vector values
 */
void initializeATAN2();

/*
 * Auxiliary angle calculation function fixed frequency
 */
float32_t auxAngleCalc(float32_t Frequency, float32_t TimeStep, float32_t CurrentAngle ,uint32_t* CurrentAngleU, uint32_t option);
#endif
