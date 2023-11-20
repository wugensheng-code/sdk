/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    al_adc_continuous.c
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2023-09-01
 * @brief   adc in continuous mode read data through polled
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
    .PlIntrCfg.IntrDoneMask     = AL_FALSE,
    .PlIntrCfg.IntrGthMask      = AL_TRUE,
    .PlIntrCfg.IntrLthMask      = AL_TRUE,
    .PlIntrCfg.IntrErrorMask    = AL_TRUE,
};

static AL_ADC_ChanCfg ChanInitCfg[1] = {
    {   .ChanNum            = AL_ADC_CHAN0,
        .MuxForChan         = AL_ADC_MUX_VPVN,
        .LthVal             = 0x0,
        .GthVal             = 0xFFF,
    },
};


/************************** Function Prototypes ******************************/
static AL_S32 AlAdc_ContinuousMode_Test(AL_VOID);

/************************** Function Definitions ******************************/

AL_S32 main(AL_VOID)
{
    AL_S32 Ret = AL_OK;

    AL_LOG(AL_LOG_LEVEL_INFO, "Adc in continuous mode read data through polled test\r\n");

    Ret = AlAdc_ContinuousMode_Test();
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Adc in continuous mode read data through polled test failed\r\n");
        return Ret;
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "Adc in continuous mode read data through polled test success\r\n");

    return AL_OK;
}

static AL_S32 AlAdc_ContinuousMode_Test(AL_VOID)
{
    AL_ADC_HalStruct *Handle;
    AL_S32 Ret = AL_OK;
    AL_U32 Index;
    AL_U32 Counts = 0;
    AL_U32 Iterations = 10000;

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

    while (Counts < Iterations) {
        for (Index = 0; Index <= Handle->Dev.Configs.ConvChanNum; Index++) {
            AL_LOG(AL_LOG_LEVEL_INFO, "Adc channel %d vc is %x \r\n", Index,
                    AlAdc_Hal_GetAdcData(Handle, ChanInitCfg[Index].ChanNum));
        }
        Counts++;
    }

    AlAdc_Hal_AdcStop(Handle);
    return AL_OK;
}