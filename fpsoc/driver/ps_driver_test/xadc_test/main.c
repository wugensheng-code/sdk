/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <stdio.h>
#include <string.h>

#include "al_xadc_hal.h"

AL_XADC_InitStruct XadcInitConfigs = {

    .InputSingal             = AL_XADC_SINGLE_ENDED,
    .Resolution              = AL_XADC_12BIT_MODE,
    .RefVoltag               = AL_XADC_REF_INTER_REF,
    .ClkSource               = AL_XADC_CLK_PL_70MHz,
    .ConvMode                = AL_XADC_CONTINUE_MODE,
    .ExternalMux             = AL_XADC_NORMAL_MODE,
    .MaxConvChannelNum       = 7,
};

AL_XADC_ChannelCfg XadcInitChannelCfg[8] = {
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

static AL_VOID AlXadc_Handler(AL_XADC_EventStruct XadcEvent, AL_VOID *CallbackRef);

AL_VOID main()
{
#if CONFIG_AlXadc_PollingReadXadcData_TEST
    printf("AlXadc polling read data test example ...\r\n");
    AlXadc_PollingReadXadcData();
#endif

#if CONFIG_AlXadc_IntrReadXadcData_TEST
    printf("AlXadc Intr read data test example ...\r\n");
    AlXadc_IntrReadXadcData();
#endif

}

AL_VOID AlXadc_PollingReadXadcData()
{
    AL_XADC_HalStruct Handle;
    AL_S32 Index = 0;
    AlXadc_Hal_Init(&Handle, 0, &XadcInitConfigs, XadcInitChannelCfg, AL_NULL);
    AlXadc_Hal_XadcStart(&Handle);

    while (1)
    {
        for (Index = 0; Index < 8; Index++) {
            printf("channel %d vc is %x \r\n", Index, AlXadc_Hal_GetAdcData(&Handle, XadcInitChannelCfg[Index].ChannelNum));
        }
    }
}

AL_VOID AlXadc_IntrReadXadcData()
{
    AL_XADC_HalStruct Handle;
    AL_U8 IntrData;
    IntrData = AL_XADC_INTR_GTH_BIT;

    AlXadc_Hal_Init(&Handle, 0, &XadcInitConfigs, XadcInitChannelCfg, AlXadc_Handler);
    AlXadc_Hal_XadcStartIntr(&Handle, IntrData);
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
                printf("Data Done channel %d vc is %x \r\n", Index, Handle->Dev->XadcData[Index].ChannelData);
            }
        }
    }

    if (XadcEvent.Events == AL_XADC_EVENT_GETDATA_GTH) {
        for (Index = 0; Index < 8; Index++) {
            if(XadcEvent.EventData & (1 << Index)) {
                printf("Data Gth channel %d vc is %x \r\n", Index, Handle->Dev->XadcData[Index].ChannelData);
            }
        }
    }

    if (XadcEvent.Events == AL_XADC_EVENT_GETDATA_LTH) {
        for (Index = 0; Index < 8; Index++) {
            if(XadcEvent.EventData & (1 << Index)) {
                printf("Data Lth channel %d vc is %x \r\n", Index, Handle->Dev->XadcData[Index].ChannelData);
            }
        }
    }

}