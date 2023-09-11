/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    al_xadc_intr.c
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2023-09-01
 * @brief   xadc read data with intr example
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
static AL_S32 AlXadc_Test_ReadDataWithIntr(AL_VOID);
static AL_VOID AlXadc_Handler(AL_XADC_EventStruct XadcEvent, AL_VOID *CallbackRef);

/************************** Function Definitions ******************************/

AL_S32 main(AL_VOID)
{
    AL_S32 Ret = AL_OK;

    AL_LOG(AL_LOG_LEVEL_INFO, "Xadc read data with intr test\r\n");

    Ret = AlXadc_Test_ReadDataWithIntr();
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Xadc read data with intr test failed\r\n");
        return Ret;
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "Xadc read data with intr test success\r\n");

    return Ret;
}

static AL_S32 AlXadc_Test_ReadDataWithIntr(AL_VOID)
{
    AL_S32 Ret = AL_OK;
    AL_XADC_HalStruct *Handle;
    AL_U8 IntrData;
    IntrData = AL_XADC_INTR_GTH_BIT;

    Ret = AlXadc_Hal_Init(&Handle, AL_XADC_DEVICE_ID, &XadcInitConfigs, XadcInitChannelCfg, AlXadc_Handler);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hal Init error:0x%x\r\n", Ret);
        return Ret;
    }

    Ret = AlXadc_Hal_XadcStartIntr(Handle, IntrData);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Xadc Start error:0x%x\r\n", Ret);
        return Ret;
    }
    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    while (1)
    {

    }
}

static AL_VOID AlXadc_Handler(AL_XADC_EventStruct XadcEvent, AL_VOID *CallbackRef)
{
    AL_S32 Index;
    AL_XADC_HalStruct *Handle = (AL_XADC_HalStruct *)CallbackRef;

    if (XadcEvent.Events == AL_XADC_EVENT_GETDATA_DONE) {
        for (Index = 0; Index < 8; Index++) {
            if(XadcEvent.EventData & (1 << Index)) {
                AL_LOG(AL_LOG_LEVEL_INFO, "Xadc Data Done channel %d vc is %x \r\n",
                       Index, Handle->Dev.XadcData[Index].ChannelData);
            }
        }
    }

    if (XadcEvent.Events == AL_XADC_EVENT_GETDATA_GTH) {
        for (Index = 0; Index < 8; Index++) {
            if(XadcEvent.EventData & (1 << Index)) {
                AL_LOG(AL_LOG_LEVEL_INFO, "Xadc Data Gth channel %d vc is %x \r\n",
                       Index, Handle->Dev.XadcData[Index].ChannelData);
            }
        }
    }

    if (XadcEvent.Events == AL_XADC_EVENT_GETDATA_LTH) {
        for (Index = 0; Index < 8; Index++) {
            if(XadcEvent.EventData & (1 << Index)) {
                AL_LOG(AL_LOG_LEVEL_INFO, "Xadc Data Lth channel %d vc is %x \r\n",
                       Index, Handle->Dev.XadcData[Index].ChannelData);
            }
        }
    }

}
