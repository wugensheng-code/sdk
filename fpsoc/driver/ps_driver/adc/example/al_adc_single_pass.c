/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    al_adc_single_pass.c
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2023-09-01
 * @brief   adc in single pass mode read data through polled
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
    .Resolution                 = AL_ADC_10BIT_MODE,
    .RefVoltag                  = AL_ADC_REF_VREF,
    .ConvMode                   = AL_ADC_SINGLE_PASS_MODE,
    .ConvChanNum                = AL_ADC_CHAN2,
    .ClkSource                  = AL_ADC_CLK_PS_50MHz,
    .AdcClkDiv                  = 50,
    .PlIntrCfg.IntrDoneMask     = AL_FALSE,
    .PlIntrCfg.IntrGthMask      = AL_TRUE,
    .PlIntrCfg.IntrLthMask      = AL_TRUE,
    .PlIntrCfg.IntrErrorMask    = AL_TRUE,
};

static AL_ADC_ChanCfg ChanInitCfg[3] = {
    {   .ChanNum            = AL_ADC_CHAN0,
        .MuxForChan         = AL_ADC_MUX_VPVN,
        .LthVal             = 0x0,
        .GthVal             = 0xFFF,
    },
    {   .ChanNum            = AL_ADC_CHAN1,
        .MuxForChan         = AL_ADC_MUX_0,
        .LthVal             = 0x0,
        .GthVal             = 0xFFF,
    },
    {   .ChanNum            = AL_ADC_CHAN2,
        .MuxForChan         = AL_ADC_MUX_8,
        .LthVal             = 0x0,
        .GthVal             = 0xFFF,
    },
};

static AL_U32 EVENT_FLAG;


/************************** Function Prototypes ******************************/
static AL_S32 AlAdc_SinglePassMode_Test(AL_VOID);
static AL_VOID AlAdc_Handler(AL_ADC_EventStruct AdcEvent, AL_VOID *CallbackRef);

/************************** Function Definitions ******************************/

/**
 *
 * This function initializes logging, performs the ADC single pass mode read data test by calling
 * AlAdc_SinglePassMode_Test(), and logs the outcome of the test. If the test fails, it logs an error
 * message and returns the error code. Otherwise, it logs a success message and returns AL_OK.
 *
 * @param  None.
 *
 * @return AL_S32 Returns AL_OK if the test is successful, or an error code if the test fails.
 *
 */
AL_S32 main(AL_VOID)
{
    AL_S32 Ret = AL_OK;

    AL_LOG(AL_LOG_LEVEL_INFO, "Adc in single pass mode read data through polled test\r\n");

    Ret = AlAdc_SinglePassMode_Test();
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Adc in single pass mode read data through polled test failed\r\n");
        return Ret;
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "Adc in single pass mode read data through polled test success\r\n");

    return AL_OK;
}

/**
 *
 * This function initializes the ADC hardware with the specified configurations, starts the ADC,
 * reads data from all configured channels, and then stops the ADC. It logs the value read from
 * each channel. If any step fails, it logs an error message and returns the error code.
 *
 * @param  None.
 *
 * @return AL_S32 Returns AL_OK if the ADC operates correctly in single pass mode, or an error code
 * if initialization, starting, or stopping the ADC fails.
 *
 */
static AL_S32 AlAdc_SinglePassMode_Test(AL_VOID)
{
    AL_S32 Ret = AL_OK;
    AL_ADC_HalStruct *Handle;
    AL_S32 Index = 0;
    AL_U32 PlAdcFunc = AL_ADC_DATA_DONE;

    Ret = AlAdc_Hal_Init(&Handle, AL_ADC_DEVICE_ID, &AdcInitCfg, ChanInitCfg, AL_NULL);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hal Init error:0x%x\r\n", Ret);
        return Ret;
    }

    Ret = AlAdc_Hal_AdcStart(Handle);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Adc Start error:0x%x\r\n", Ret);
        return Ret;
    }

    for (Index = 0; Index <= Handle->Dev.Configs.ConvChanNum; Index++) {
        AL_LOG(AL_LOG_LEVEL_INFO, "Adc channel %d vc is %x \r\n", Index,
                AlAdc_Hal_GetAdcData(Handle, ChanInitCfg[Index].ChanNum));
    }

    AlAdc_Hal_AdcStopIntr(Handle);
    return AL_OK;
}