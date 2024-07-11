/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    al_adc_single_channel.c
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2023-09-01
 * @brief   adc in single channel mode read data through polled
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
    .ConvMode                   = AL_ADC_SINGLE_CHANNEL_MODE,
    .ConvChanNum                = 5,
    .ClkSource                  = AL_ADC_CLK_PS_50MHz,
    .AdcClkDiv                  = 50,
    .PlIntrCfg.IntrDoneMask     = AL_FALSE,
    .PlIntrCfg.IntrGthMask      = AL_TRUE,
    .PlIntrCfg.IntrLthMask      = AL_TRUE,
    .PlIntrCfg.IntrErrorMask    = AL_TRUE,
};

static AL_ADC_ChanCfg ChanInitCfg = {
    .ChanNum            = AL_ADC_CHAN5,
    .MuxForChan         = AL_ADC_MUX_VPVN,
    .LthVal             = 0x0,
    .GthVal             = 0xFFF,
};


/************************** Function Prototypes ******************************/
static AL_S32 AlAdc_SingleChannelMode_Test(AL_VOID);

/************************** Function Definitions ******************************/

/**
 *
 * This function logs the start of the ADC single channel mode read data test, enters an infinite loop
 * where it continuously calls AlAdc_SingleChannelMode_Test(), and logs the result of the test. If the test
 * fails, it logs an error message and returns the error code. Otherwise, it logs a success message.
 *
 * Note: The infinite loop makes this function never return under normal operation, which might not be
 * intended. Consider reviewing the logic related to the infinite loop.
 *
 * @param  None.
 *
 * @return AL_S32 Returns AL_OK if the test is successful, or an error code if the test fails.
 */
AL_S32 main(AL_VOID)
{
    AL_S32 Ret = AL_OK;

    AL_LOG(AL_LOG_LEVEL_INFO, "Adc single channel mode read data test \r\n");

    while (1) {
        AlAdc_SingleChannelMode_Test();
    }

    Ret = AlAdc_SingleChannelMode_Test();
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Adc single channel mode read data test failed\r\n");
        return Ret;
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "Adc single channel mode read data test success\r\n");

    return AL_OK;
}

/**
 *
 * This function initializes the ADC hardware with specified configurations for single channel mode,
 * starts the ADC, reads data from the configured channel, stops the ADC, and cleans up. It logs
 * the ADC value read from the specified channel. If any step fails, it logs an error message and
 * returns the error code.
 *
 * @param  None.
 *
 * @return AL_S32 Returns AL_OK if the ADC single channel mode operates correctly, or an error code
 * if initialization, starting, or stopping the ADC fails.
 */
static AL_S32 AlAdc_SingleChannelMode_Test(AL_VOID)
{
    AL_S32 Ret = AL_OK;
    AL_ADC_HalStruct *Handle;

    Ret = AlAdc_Hal_Init(&Handle, AL_ADC_DEVICE_ID, &AdcInitCfg, &ChanInitCfg, AL_NULL);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hal Init error:0x%x\r\n", Ret);
        return Ret;
    }

    Ret = AlAdc_Hal_AdcStart(Handle);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Adc start error:0x%x\r\n", Ret);
        return Ret;
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "Adc channel %d vc is %x \r\n", ChanInitCfg.ChanNum,
                AlAdc_Hal_GetAdcData(Handle, ChanInitCfg.ChanNum));

    Ret = AlAdc_Hal_AdcStop(Handle);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Adc stop error:0x%x\r\n", Ret);
        return Ret;
    }

    return AL_OK;
}
