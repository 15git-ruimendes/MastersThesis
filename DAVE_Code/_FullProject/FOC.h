#ifndef _FOC_H_
#define _FOC_H_

#include <math.h>
#include <stdlib.h>

#define TICKS_PER_SECOND 12000
#define SECOND_PER_TICKS 0.001f
#define PI 3.14159265359
#define SQRT_3 1.7320508075688772935274463415059f
#define SQRT_2 1.4142135623730950488016887242097f
#define SQRT_3_2 1.2247448713915890490986420373529f
#define SQRT_1_2 0.70710678118654752440084436210485f
#define SQRT_1_3 0.57735026918962576450914878050196f

/*
 * Initial Motor Parameters
 */
#define Rs 0.03552
#define Ls 0.000335
#define Rr 0.02092
#define Lr 0.000335
#define Lm 0.0151
#define P 4
#define J 1.2
#define RPM_NOMINAL 1500
#define ID_NOMINAL 62.3
#define IQ_MAX 315
#define ID_MIN 30
/*
 * @brief FOC Struct: Structure used to define current controller gain,
 * and current and voltage transform values
 */
typedef struct FOC{
	float Kp;
	float Ki;
	float T_PI;
	float T_Theta;
	float Theta;
	float Ial;
	float Ibet;
	float Val;
	float Vbet;
	float Id;
	float Iq;
	float Id_Ref;
	float Iq_Ref;
	float Vd_Ref;
	float Vq_Ref;
	float V_Ref;
	float Theta_Ref;
}FOC;

/*
 * @brief Calculation of electrical angle
 * given current references and RPM value
 * @param RPM Instant mechanical speed in RPM
 * @param Controller FOC type structure containing relevant parameters (requires initialization)
 */
float ThetaCalculation(float RPM,FOC* Controller);
/*
 * @brief Adaptation of speed sensor signals
 * to return current speed value in RPM
 * @param inA First Sinusoidal Signal
 * @param inB Second sinusoidal signal (90 degree delay)
 * @param Controller FOC type structure containing relevant parameters (requires initialization)
 */
float SpeedSensor(float inA, float inB);

/*
 * @brief Calulating D and Q voltage references using PI controller
 * @param Controller FOC type structure containing relevant parameters (requires initialization)
 */
void CurrentController(FOC* Controller);

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
void ClarkeTransform(int opt,float Ia,float Ib,float Ic,float Va,float Vb,float Vc,FOC* Controller);

/*
 * @brief Park transform. Applied to Ial, Ibet currents already present in Controller structure
 * (! Needs to follow ClarkeTransform)
 * @params Controller FOC type structure containing relevant parameters (requires initialization)
 */
void ParkTransform(FOC* Controller);

/*
 * @brief Inverse Park transform. Applied only to VdRef and VqRef already within Controller structure (must be used after PI controllers)
 * also calulates V_Ref and Theta to be used in SVMPWM app.
 * @params Controller FOC type structure containing relevant parameters (requires initialization)
 */
void InvParkTransform(FOC* Controller,float* Val,float* Vbet);


#endif
