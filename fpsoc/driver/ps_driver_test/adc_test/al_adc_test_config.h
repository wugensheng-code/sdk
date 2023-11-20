/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    al_adc_test_config.h
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date
 * @brief   adc test config file
 */

#ifndef AL_ADC_TEST_CONFIG_H
#define AL_ADC_TEST_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#include "al_adc_hal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/************************** Constant Definitions *****************************/
#define AL_ADC_DEVICE_ID 0


/***************** Macros (Inline Functions) Definitions *********************/

/**************************** Type Definitions *******************************/

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
    .PlIntrCfg.IntrGthMask      = AL_FALSE,
    .PlIntrCfg.IntrLthMask      = AL_FALSE,
    .PlIntrCfg.IntrErrorMask    = AL_TRUE,
};

static AL_ADC_ChanCfg ChanInitCfg[8] = {
    {   .ChanNum            = AL_ADC_CHAN0,
        .MuxForChan         = AL_ADC_MUX_VPVN,
        .LthVal             = 0x7b6, /*12bit mode, 0x7b6 = 0.3V*/
        .GthVal             = 0xa3c, /*12bit mode, 0xa3c = 0.8V*/
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
    {   .ChanNum            = AL_ADC_CHAN3,
        .MuxForChan         = AL_ADC_MUX_11,
        .LthVal             = 0x0,
        .GthVal             = 0xFFF,
    },
    {   .ChanNum            = AL_ADC_CHAN4,
        .MuxForChan         = AL_ADC_MUX_12,
        .LthVal             = 0x0,
        .GthVal             = 0xFFF,
    },
    {   .ChanNum            = AL_ADC_CHAN5,
        .MuxForChan         = AL_ADC_MUX_13,
        .LthVal             = 0x0,
        .GthVal             = 0xFFF,
    },
    {   .ChanNum            = AL_ADC_CHAN6,
        .MuxForChan         = AL_ADC_MUX_14,
        .LthVal             = 0x0,
        .GthVal             = 0xFFF,
    },
    {   .ChanNum            = AL_ADC_CHAN7,
        .MuxForChan         = AL_ADC_MUX_15,
        .LthVal             = 0x0,
        .GthVal             = 0xFFF,
    }
};


/************************** Function Prototypes ******************************/

#ifdef __cplusplus
}
#endif

#endif
