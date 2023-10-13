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
static AL_ADC_InitStruct AdcInitConfigs = {

    .InputSingal             = AL_ADC_UNIPOLAR,
    .Resolution              = AL_ADC_12BIT_MODE,
    .RefVoltag               = AL_ADC_REF_INTER_REF,
    .ClkSource               = AL_ADC_CLK_PL_70MHz,
    .ConvMode                = AL_ADC_CONTINUE_MODE,
    .ExternalMux             = AL_ADC_NORMAL_MODE,
    .MaxConvChanNum          = 7,
};

static AL_ADC_ChannelCfg AdcInitChanCfg[8] = {
    {   .ChanNum            = AL_ADC_CHAN0,
        .ChanIomux          = AL_ADC_IO_MUX_16,
        .LthVal             = 0xFFF,
        .GthVal             = 0x0,
    },
    {   .ChanNum            = AL_ADC_CHAN1,
        .ChanIomux          = AL_ADC_IO_MUX_0,
        .LthVal             = 0xFFF,
        .GthVal             = 0x0,
    },
    {   .ChanNum            = AL_ADC_CHAN2,
        .ChanIomux          = AL_ADC_IO_MUX_8,
        .LthVal             = 0xFFF,
        .GthVal             = 0x0,
    },
    {   .ChanNum            = AL_ADC_CHAN3,
        .ChanIomux          = AL_ADC_IO_MUX_11,
        .LthVal             = 0xFFF,
        .GthVal             = 0x0,
    },
    {   .ChanNum            = AL_ADC_CHAN4,
        .ChanIomux          = AL_ADC_IO_MUX_12,
        .LthVal             = 0xFFF,
        .GthVal             = 0x0,
    },
    {   .ChanNum            = AL_ADC_CHAN5,
        .ChanIomux          = AL_ADC_IO_MUX_13,
        .LthVal             = 0xFFF,
        .GthVal             = 0x0,
    },
    {   .ChanNum            = AL_ADC_CHAN6,
        .ChanIomux          = AL_ADC_IO_MUX_14,
        .LthVal             = 0xFFF,
        .GthVal             = 0x0,
    },
    {   .ChanNum            = AL_ADC_CHAN7,
        .ChanIomux          = AL_ADC_IO_MUX_15,
        .LthVal             = 0xFFF,
        .GthVal             = 0x0,
    }
};


/************************** Function Prototypes ******************************/

#ifdef __cplusplus
}
#endif

#endif
