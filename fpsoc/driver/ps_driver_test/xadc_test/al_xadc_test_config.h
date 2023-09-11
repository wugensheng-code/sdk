/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    al_xadc_test_config.h
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date
 * @brief   xadc test config file
 */

#ifndef AL_XADC_TEST_CONFIG_H
#define AL_XADC_TEST_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#include "al_xadc_hal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/************************** Constant Definitions *****************************/
#define AL_XADC_DEVICE_ID 0


/***************** Macros (Inline Functions) Definitions *********************/

/**************************** Type Definitions *******************************/

/************************** Variable Definitions *****************************/
static AL_XADC_InitStruct XadcInitConfigs = {

    .InputSingal             = AL_XADC_SINGLE_ENDED,
    .Resolution              = AL_XADC_12BIT_MODE,
    .RefVoltag               = AL_XADC_REF_INTER_REF,
    .ClkSource               = AL_XADC_CLK_PL_70MHz,
    .ConvMode                = AL_XADC_CONTINUE_MODE,
    .ExternalMux             = AL_XADC_NORMAL_MODE,
    .MaxConvChannelNum       = 7,
};

static AL_XADC_ChannelCfg XadcInitChannelCfg[8] = {
    {   .ChannelNum           = AL_XADC_CHANNEL0,
        .ChannelIomux         = AL_XADC_IO_MUX_8,
        .LthValue             = 0xFFF,
        .GthValue             = 0x0,
    },
    {   .ChannelNum           = AL_XADC_CHANNEL1,
        .ChannelIomux         = AL_XADC_IO_MUX_9,
        .LthValue             = 0xFFF,
        .GthValue             = 0x0,
    },
    {   .ChannelNum           = AL_XADC_CHANNEL2,
        .ChannelIomux         = AL_XADC_IO_MUX_10,
        .LthValue             = 0xFFF,
        .GthValue             = 0x0,
    },
    {   .ChannelNum           = AL_XADC_CHANNEL3,
        .ChannelIomux         = AL_XADC_IO_MUX_11,
        .LthValue             = 0xFFF,
        .GthValue             = 0x0,
    },
    {   .ChannelNum           = AL_XADC_CHANNEL4,
        .ChannelIomux         = AL_XADC_IO_MUX_12,
        .LthValue             = 0xFFF,
        .GthValue             = 0x0,
    },
    {   .ChannelNum           = AL_XADC_CHANNEL5,
        .ChannelIomux         = AL_XADC_IO_MUX_13,
        .LthValue             = 0xFFF,
        .GthValue             = 0x0,
    },
    {   .ChannelNum           = AL_XADC_CHANNEL6,
        .ChannelIomux         = AL_XADC_IO_MUX_14,
        .LthValue             = 0xFFF,
        .GthValue             = 0x0,
    },
    {   .ChannelNum           = AL_XADC_CHANNEL7,
        .ChannelIomux         = AL_XADC_IO_MUX_15,
        .LthValue             = 0xFFF,
        .GthValue             = 0x0,
    }
};


/************************** Function Prototypes ******************************/

#ifdef __cplusplus
}
#endif

#endif
