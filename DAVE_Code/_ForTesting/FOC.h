#ifndef _FOC_H_
#define _FOC_H_

#include <Dave.h>
#include <math.h>
#include <stdlib.h>

#define TICKS_PER_SECOND 2000
#define ANGLE_PER_ZERO 0.01f
#define SQRT_3 1.7320508075688772935274463415059f
#define SQRT_2 1.4142135623730950488016887242097f
#define SQRT_3_2 1.2247448713915890490986420373529f
#define SQRT_1_2 0.70710678118654752440084436210485f
#define SQRT_1_3 0.57735026918962576450914878050196f

#define ACQUISITION_TIME 0.000050
#define CONTROL_TIME 0.00083
#define FAULT_CHECK_TIME 0.001

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
#define IQ_MAX 315f
#define ID_MIN 30f

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
	float32_t V_Ref;
	float32_t Theta_Ref;
} FOC;

/*
 * @brief Diff instance corresponding to an auxiliary
 * structure to implement differential calculation using function
 * Derivative. In initialization time_step must be defined.
 */
typedef struct
{
	float32_t previous_in;
	float32_t time_step;
} Diff;

/*
 *
 */
typedef struct
{
	float32_t accm_val;
	bool reset;
	float32_t lim_max;
	float32_t lim_min;
	float32_t prev_val;
	float32_t time_step;
} Integ;

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
void CurrentController(FOC *Controller);

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
int32_t SignVal(float32_t sign);

/*
 * @brief Derivative calculation formula calculated at time_step
 * @params input New value to calculate derivative value
 * @params deriv_inst Instance of derivative calculation
 */
float32_t Derivative(float32_t input, Diff *deriv_inst);

/*
 *
 */
float32_t Integrator(float32_t input, Integ *integ_inst);
#endif
