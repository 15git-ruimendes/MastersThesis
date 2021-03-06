/*********************************************************************************************************************
* DAVE APP Name : ADC_QUEUE       APP Version: 4.0.22
*
* NOTE:
* This file is generated by DAVE. Any manual modification done to this file will be lost when the code is regenerated.
*********************************************************************************************************************/

/**
 * @cond
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
 * 2015-02-16:
 *     - Initial version for DAVEv4.<BR>
 *
 * 2015-04-27:
 *     - Optimized the ADC_QUEUE_AllEntriesInserted.<BR>
 *
 * 2015-06-20:
 *     - Updated the copyright section.<BR>
 *    
 * 2015-06-26:
 *     - Added flushing of all the queue entries before inserting to the Hardware buffer.<BR>
 *
 * 2015-07-09:
 *     - Documentation updated.<BR>
 *
 * 2015-07-31:
 *     - Updated the APP to support new devices.<BR>
 *
 * 2015-10-01:
 *     - Iclass signal added to the APP.<BR>
 *
 * 2015-10-08:
 *     - Added support for XMC4700/XMC4800 devices.<BR>
 *
 * 2015-12-15:     
 *          - Added support for XMC4300 devices.<BR>
 *          - Updated the APP to optimize the request source callback.<BR>
 *          - APIs touched are ADC_QUEUE_InsertQueueEntry(), ADC_QUEUE_ResetListContents(), 
 *            ADC_QUEUE_ReqSrcEventHandler().<BR>       
 *
 * 2016-01-19:
 *          - Added new APIs ADC_QUEUE_EnableArbitration() and ADC_QUEUE_DisableArbitration().<BR>
 *
 * 2016-04-03:
 *          - Total conversion time now includes post calibration time as well for XMC1x.<BR>
 *
 * 2016-08-18:
 *     - Minimum sample time at low frequencies changed to incorporate Errata ADC_AI.H006 and ADC_AI.H007.
 *
 * @endcond
 *
 */

/***********************************************************************************************************************
 * HEADER FILES                                                                                                      
 **********************************************************************************************************************/
#include "adc_queue.h"

/**********************************************************************************************************************
 * DATA STRUCTURES
 **********************************************************************************************************************/
/**
 * @ingroup ADC_QUEUE_datastructures
 * @{
 */

/**
 * ADC_QUEUE ISR Handle for ADC_QUEUE_0
 */
const ADC_QUEUE_ISR_t ADC_QUEUE_0_reqsrc_config_0 =
{
  .node_id          = 0U,
  .priority         = 0U,
  .sub_priority     = 0U,
  .interrupt_enable = (bool)false 
};

/**
 * ADC QUEUE LLD Handle for configuring the ADC_QUEUE_0
 */
const XMC_VADC_QUEUE_CONFIG_t ADC_QUEUE_0_init_config_0 =
{
  .conv_start_mode  = (uint32_t) XMC_VADC_STARTMODE_WFS, 		/* Conversion start mode WFS/CIR/CNR*/
  .req_src_priority = (uint32_t) XMC_VADC_GROUP_RS_PRIORITY_0, /*The queue request source priority */
  .trigger_signal   = (uint32_t) XMC_VADC_REQ_TR_A, 			/*If trigger needed the signal input*/
  .trigger_edge     = (uint32_t) XMC_VADC_TRIGGER_EDGE_NONE,   /*Trigger edge needed if trigger enabled*/
  .gate_signal      = (uint32_t) XMC_VADC_REQ_GT_A,            /*If gating needed the signal input*/
  .timer_mode       = (uint32_t) 0,                            /* Disabled equidistant sampling*/
  .external_trigger = (uint32_t) 0 						/*External trigger Enabled/Disabled*/  
};

/**
 * ADC_QUEUE Handle for configuring the ADC_QUEUE_0
 */
ADC_QUEUE_t ADC_QUEUE_0 =
{
  .iclass_config_handle = {
                           .sample_time_std_conv= (uint32_t) 0, /*The required STCS value.*/
                           .conversion_mode_standard        = (uint32_t) XMC_VADC_CONVMODE_12BIT, /* Conversion mode for ADC_QUEUE_0*/
                          },
  .group_handle         = (XMC_VADC_GROUP_t*)(void*)VADC_G0, /* Group pointer for ADC_QUEUE_0*/
  .queue_config_handle  = (XMC_VADC_QUEUE_CONFIG_t*) &ADC_QUEUE_0_init_config_0, /* Queue configuration structure for ADC_QUEUE_0*/
  .global_handle        = (GLOBAL_ADC_t *) &GLOBAL_ADC_0, /*The GLOBAL_ADC App handle*/ 
  .rs_intr_handle       = (ADC_QUEUE_ISR_t*) &ADC_QUEUE_0_reqsrc_config_0, /* The request source interrupt config handler*/
  .gating_mode          = XMC_VADC_GATEMODE_IGNORE,     /*Gating edge needed if gate enabled*/
  .srv_req_node         = XMC_VADC_SR_GROUP_SR0, /*Service Request line for Request source Event */
  .init_status			= ADC_QUEUE_UNINITIALIZED, /* ADC_QUEUE_0 INIT status*/
  .instance_number      = (uint8_t) 0,   /* APP Instance number for ADC_QUEUE_0*/
  .iclass_num           = (uint8_t) 0,   /* The selected ICLASS configuration for ADC_QUEUE_0*/
  .group_index          = (uint8_t) 0    /* The Group index for ADC_QUEUE_0*/
};

ADC_QUEUE_ENTRY_t adc_queue_list_ch_index_0[ADC_QUEUE_NUM_CHANNELS];
ADC_QUEUE_ENTRY_t adc_queue_list_pos_index_0[ADC_QUEUE_NUM_CHANNELS];
/**
 *@}
 */


/**
 * This function invokes the Request source handler for ADC_QUEUE_0
 */
void ADC_QUEUE_0_ReqSrcEventHandler(void)
{
  ADC_QUEUE_ReqSrcEventHandler(&ADC_QUEUE_0);
}



/* API generated to initialise list pointers and clear its contents*/
void ADC_QUEUE_lInitializeListToDefaults(uint8_t instance_num)
{

  /* Initialise list pointers of APP Instance- ADC_QUEUE_0 */
  adc_queue_list_ptrs[0][0] = adc_queue_list_ch_index_0;
  adc_queue_list_ptrs[0][1] = adc_queue_list_pos_index_0;
 
  /* Reset channels */
  ADC_QUEUE_ResetListContents(instance_num);
}
/**
 * @}
 */

