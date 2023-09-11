/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    al_xadc_polled.c
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2023-09-01
 * @brief   xadc read data with polled example
 */

/***************************** Include Files *********************************/
#include <string.h>
#include <stdlib.h>
#include "al_xadc_hal.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/
#define AL_XADC_DEVICE_ID             (0)

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
static AL_S32 AlXadc_Test_ReadDataWithPolled(AL_VOID);
static AL_VOID AlXadc_Handler(AL_XADC_EventStruct XadcEvent, AL_VOID *CallbackRef);

/************************** Function Definitions ******************************/

AL_S32 main(AL_VOID)
{
    AL_S32 Ret = AL_OK;

    AL_LOG(AL_LOG_LEVEL_INFO, "Xadc read data with polled test\r\n");

    Ret = AlXadc_Test_ReadDataWithPolled();
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Xadc read data with polled test failed\r\n");
        return Ret;
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "Xadc read data with polled test success\r\n");

    return Ret;
}

static AL_S32 AlXadc_Test_ReadDataWithPolled(AL_VOID)
{
    AL_S32 Ret = AL_OK;
    AL_XADC_HalStruct *Handle;
    AL_S32 Index = 0;

    Ret = AlXadc_Hal_Init(&Handle, AL_XADC_DEVICE_ID, &XadcInitConfigs, XadcInitChannelCfg, AL_NULL);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hal Init error:0x%x\r\n", Ret);
        return Ret;
    }

    Ret = AlXadc_Hal_XadcStart(Handle);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Xadc Start error:0x%x\r\n", Ret);
        return Ret;
    }

    while (1) {
        for (Index = 0; Index < 8; Index++) {
            AL_LOG(AL_LOG_LEVEL_INFO, "Xadc channel %d vc is %x \r\n", Index,
                   AlXadc_Hal_GetAdcData(Handle, XadcInitChannelCfg[Index].ChannelNum));
        }
    }
}
