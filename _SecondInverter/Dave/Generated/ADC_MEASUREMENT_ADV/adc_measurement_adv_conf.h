/*********************************************************************************************************************
* DAVE APP Name : ADC_MEASUREMENT_ADV       APP Version: 4.0.20
*
* NOTE:
* This file is generated by DAVE. Any manual modification done to this file will be lost when the code is regenerated.
*********************************************************************************************************************/

/* @cond
 ***********************************************************************************************************************
 *
 * Copyright (c) 2015-2020, Infineon Technologies AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,are permitted provided that the
 * following conditions are met:
 *
 *   Redistributions of source code must retain the above copyright notice, this list of conditions and the  following
 *   disclaimer.
 *
 *   Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *   following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *   Neither the name of the copyright holders nor the names of its contributors may be used to endorse or promote
 *   products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT  OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * To improve the quality of the software, users are encouraged to share modifications, enhancements or bug fixes
 * with Infineon Technologies AG (dave@infineon.com).
 ***********************************************************************************************************************
 *
 * Change History
 * --------------
 *
 * 2015-10-09:
 *     - Initial version for DAVEv4.<BR>
 *
 * 2015-10-20:
 *     - Documentation updated.<BR>
 *
 * 2015-12-15:
 *     - Added support for XMC4300 devices.<BR>
 *
 * 2016-01-18:
 *     - 1.Internal consumption of request source.<BR>
 *     - 2.Support for synchronized conversion reduced from 8 master channels to 4 .<BR>
 *     - 3.Converted the SetBoundary() API from public API to private API .<BR>
 *     - 4.Updated all APIs for the Internal consumption of request source.<BR>
 * 
 *
 * 2016-02-05:
 *     - Documentation updated.<BR>
 * 
 * 2016-03-18:
 *     - Added consumption of the GLOBAL ICLASS -1 for Sync. Conversions.<BR>
 *     - Added consumption of the result register-0 for subtraction mode.<BR>
 *     - Removed ADC_MEASUREMENT_ADV_SetUniformConversion().<BR>
 *     - New API ADC_MEASUREMENT_ADV_SetIclass() added to configure the GLOBAL ICLASS for Slaves. <BR>
 *
 * 2016-04-26:
 *     - The synchronized conversion in a master slave configuration is currently not supported.<BR>
 *
 * 2016-06-17:
 *     - The synchronized conversion is supported.<BR>
 *     - Modified the Sync initialization sequence to configure the EVAL bits in the slave groups.<BR>
 *
 * 2016-08-18:
 *     - Minimum sample time at low frequencies changed to incorporate Errata ADC_AI.H006 and ADC_AI.H007.
 *
 * @endcond
 *
 *
 */

#ifndef ADC_MEASUREMENT_ADV_CONF_H
#define ADC_MEASUREMENT_ADV_CONF_H

/***********************************************************************************************************************
 * HEADER FILES
 **********************************************************************************************************************/

#include "ADC_QUEUE/adc_queue.h" 

/**********************************************************************************************************************
 * MACROS
 **********************************************************************************************************************/


/* If a queue request source is used by any of the ADC_MEASUREMENT_ADV APP 
 * Instances this macro is generated*/
#define ADC_MEASUREMENT_ADV_QUEUE_USED (1U)

/* If SHS gain apart from 1:1 is needed and an alias channel is used by one of 
 * the ADC_MEASUREMENT_ADV APP Instances this macro is generated. Needed for correct configuration of the gain value.*/
#define ADC_MEASUREMENT_ADV_SHS_GAIN_ALIAS (1U)

/* If FIFO is not used by any of the ADC_MEASUREMENT_ADV APP
 * Instances this macro is generated. The Result register configuration holds only one config structure.*/
#define ADC_MEASUREMENT_ADV_RESULT_REG (1U)

/* If queue request source is need by using ADC_QUEUE APP in any of the ADC_MEASUREMENT_ADV APP
 * Instances this macro is generated.*/
#define ADC_MEASUREMENT_ADV_ADC_QUEUE_USED (1U)

/* If Start after initialization is used by any of the ADC_MEASUREMENT_ADV APP
 * Instances this macro is generated.*/
#define ADC_MEASUREMENT_ADV_START_ADC (1U)


 /* Channel and result related macros for the instance ADC*/
#define ADC_GROUP_PTR  ((XMC_VADC_GROUP_t*)(void*) VADC_G0)

#define ADC_CURRENT_A  (ADC_CURRENT_A_handle)
#define ADC_CURRENT_A_RES  (VADC_G0->RES[15])

#define ADC_CURRENT_B  (ADC_CURRENT_B_handle)
#define ADC_CURRENT_B_RES  (VADC_G0->RES[4])

#define ADC_CURRENT_C  (ADC_CURRENT_C_handle)
#define ADC_CURRENT_C_RES  (VADC_G0->RES[5])

#define ADC_V_DC  (ADC_V_DC_handle)
#define ADC_V_DC_RES  (VADC_G0->RES[8])

#define ADC_RPM_COS  (ADC_RPM_COS_handle)
#define ADC_RPM_COS_RES  (VADC_G0->RES[9])

#define ADC_RPM_SIN  (ADC_RPM_SIN_handle)
#define ADC_RPM_SIN_RES  (VADC_G0->RES[6])

#define ADC_TEMP_SENS_MOT  (ADC_TEMP_SENS_MOT_handle)
#define ADC_TEMP_SENS_MOT_RES  (VADC_G0->RES[11])

#define ADC_TEMP_SENS_IGBT  (ADC_TEMP_SENS_IGBT_handle)
#define ADC_TEMP_SENS_IGBT_RES  (VADC_G0->RES[12])

#ifndef VADC_QUEUE_GROUP_0_POSITION_0
#define VADC_QUEUE_GROUP_0_POSITION_0 (0U)
#else
#error "Redefined the same Queue position. Please change the queue position to a different value in the UI editor of the APP"
#endif

#ifndef VADC_QUEUE_GROUP_0_POSITION_1
#define VADC_QUEUE_GROUP_0_POSITION_1 (1U)
#else
#error "Redefined the same Queue position. Please change the queue position to a different value in the UI editor of the APP"
#endif

#ifndef VADC_QUEUE_GROUP_0_POSITION_2
#define VADC_QUEUE_GROUP_0_POSITION_2 (2U)
#else
#error "Redefined the same Queue position. Please change the queue position to a different value in the UI editor of the APP"
#endif

#ifndef VADC_QUEUE_GROUP_0_POSITION_3
#define VADC_QUEUE_GROUP_0_POSITION_3 (3U)
#else
#error "Redefined the same Queue position. Please change the queue position to a different value in the UI editor of the APP"
#endif

#ifndef VADC_QUEUE_GROUP_0_POSITION_4
#define VADC_QUEUE_GROUP_0_POSITION_4 (4U)
#else
#error "Redefined the same Queue position. Please change the queue position to a different value in the UI editor of the APP"
#endif

#ifndef VADC_QUEUE_GROUP_0_POSITION_5
#define VADC_QUEUE_GROUP_0_POSITION_5 (5U)
#else
#error "Redefined the same Queue position. Please change the queue position to a different value in the UI editor of the APP"
#endif

#ifndef VADC_QUEUE_GROUP_0_POSITION_6
#define VADC_QUEUE_GROUP_0_POSITION_6 (6U)
#else
#error "Redefined the same Queue position. Please change the queue position to a different value in the UI editor of the APP"
#endif

#ifndef VADC_QUEUE_GROUP_0_POSITION_7
#define VADC_QUEUE_GROUP_0_POSITION_7 (7U)
#else
#error "Redefined the same Queue position. Please change the queue position to a different value in the UI editor of the APP"
#endif


#define ADC_MEASUREMENT_ADV_MAJOR_VERSION (4U) /**< Major version number of ADC_MEASUREMENT_ADV APP*/
#define ADC_MEASUREMENT_ADV_MINOR_VERSION (0U) /**< Minor version number of ADC_MEASUREMENT_ADV APP*/
#define ADC_MEASUREMENT_ADV_PATCH_VERSION (20U) /**< Patch version number of ADC_MEASUREMENT_ADV APP*/

  
#endif /* _ADC_MEASUREMENT_ADV_CONF_H_ */

