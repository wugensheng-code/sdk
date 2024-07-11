/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    al_adc_alarm.c
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2023-09-01
 * @brief   If the adc exceeds the waterline,  an alarm will be triggered through interruption.
 */

/***************************** Include Files *********************************/
#include <string.h>
#include <stdlib.h>
#include "al_adc_hal.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/
#define AL_ADC_DEVICE_ID             (0)

/************************** Variable Definitions *****************************/
static AL_ADC_InitStruct AdcInitCfg = {
    .InputSingal                = AL_ADC_UNIPOLAR,
    .Resolution                 = AL_ADC_12BIT_MODE,
    .RefVoltag                  = AL_ADC_REF_VREF,
    .ConvMode                   = AL_ADC_CONTINUOUS_MODE,
    .ConvChanNum                = AL_ADC_CHAN0,
    .ClkSource                  = AL_ADC_CLK_PS_50MHz,
    .AdcClkDiv                  = 50,
    .PlIntrCfg.IntrDoneMask     = AL_TRUE,
    .PlIntrCfg.IntrGthMask      = AL_FALSE,
    .PlIntrCfg.IntrLthMask      = AL_FALSE,
    .PlIntrCfg.IntrErrorMask    = AL_TRUE,
};

static AL_ADC_ChanCfg ChanInitCfg[1] = {
    {   .ChanNum            = AL_ADC_CHAN0,
        .MuxForChan         = AL_ADC_MUX_VPVN,
        .LthVal             = 0x7b6, /*12bit mode, 0x7b6 = 0.3V*/
        .GthVal             = 0xa3c, /*12bit mode, 0xa3c = 0.8V*/
    },
};


/************************** Function Prototypes ******************************/
static AL_S32 AlAdc_AlarmFunc_Test(AL_VOID);
static AL_VOID AlAdc_EventHandler(AL_ADC_EventStruct AdcEvent, AL_VOID *CallbackRef);

/************************** Function Definitions ******************************/

/**
 *
 * Initializes the ADC and starts the ADC alarm function test. It logs the test start,
 * calls AlAdc_AlarmFunc_Test to perform the test, and logs the result. If the test fails,
 * it logs an error message and returns the error code.
 *
 * @param  None.
 *
 * @return AL_S32 Returns AL_OK if the test is successful, or an error code if the test fails.
 *
 */
AL_S32 main(AL_VOID)
{
    AL_S32 Ret = AL_OK;

    AL_LOG(AL_LOG_LEVEL_INFO, "Adc alarm function through interrupts test\r\n");

    Ret = AlAdc_AlarmFunc_Test();
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Adc alarm function interrupts test failed\r\n");
        return Ret;
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "Adc alarm function through interrupts test success\r\n");

    return AL_OK;
}

/**
 *
 * This function initializes the ADC hardware with specified configurations, starts the ADC
 * with interrupt mode, and enters an infinite loop to simulate continuous operation. It
 * stops the ADC interrupt mode before returning. If any step fails, it logs an error message
 * and returns the error code.
 *
 * @param  None.
 *
 * @return AL_S32 Returns AL_OK if the ADC alarm functionality works as expected, or an error
 * code if initialization or starting the ADC fails.
 *
 */
static AL_S32 AlAdc_AlarmFunc_Test(AL_VOID)
{
    AL_S32 Ret = AL_OK;
    AL_ADC_HalStruct *Handle;

    Ret = AlAdc_Hal_Init(&Handle, AL_ADC_DEVICE_ID, &AdcInitCfg, ChanInitCfg, AlAdc_EventHandler);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hal Init error:0x%x\r\n", Ret);
        return Ret;
    }

    Ret = AlAdc_Hal_AdcStartIntr(Handle);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Adc Start error:0x%x\r\n", Ret);
        return Ret;
    }

    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    while(1);

    AlAdc_Hal_AdcStopIntr(Handle);

    return AL_OK;
}

/**
 *
 * This function is called when an ADC event occurs. It checks the type of event (data greater
 * than high threshold or data less than low threshold) and logs the channel number and value
 * for each active channel. The function is designed to be used as a callback with ADC interrupts.
 *
 * @param AdcEvent The ADC event structure containing details about the event.
 * @param CallbackRef A pointer to user-defined data, in this case, an AL_ADC_HalStruct object.
 *
 * @return None.
 *
 */
static AL_VOID AlAdc_EventHandler(AL_ADC_EventStruct AdcEvent, AL_VOID *CallbackRef)
{
    AL_S32 Index;
    AL_ADC_HalStruct *Handle = (AL_ADC_HalStruct *)CallbackRef;

    if (AdcEvent.Events == AL_ADC_EVENT_DATA_GTH) {
        for (Index = 0; Index <= Handle->Dev.Configs.ConvChanNum; Index++) {
            if (AdcEvent.EventData & BIT(Index)) {
                AL_LOG(AL_LOG_LEVEL_INFO, "Adc Data Gth channel %d vc is %x \r\n",
                       Index, Handle->Dev.AdcData[Index].ChanData);
            }
        }
    }

    if (AdcEvent.Events == AL_ADC_EVENT_DATA_LTH) {
        for (Index = 0; Index <= Handle->Dev.Configs.ConvChanNum; Index++) {
            if (AdcEvent.EventData & BIT(Index)) {
                AL_LOG(AL_LOG_LEVEL_INFO, "Adc Data Lth channel %d vc is %x \r\n",
                       Index, Handle->Dev.AdcData[Index].ChanData);
            }
        }
    }
}
