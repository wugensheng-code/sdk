/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    al_adc_intr.c
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2023-09-01
 * @brief   adc read data with intr example
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
static AL_ADC_InitStruct AdcInitConfigs = {
    .InputSingal             = AL_ADC_UNIPOLAR,
    .Resolution              = AL_ADC_10BIT_MODE,
    .RefVoltag               = AL_ADC_REF_INTER_REF,
    .ClkSource               = AL_ADC_CLK_OSC_44MHz,
    .ConvMode                = AL_ADC_CONTINUE_MODE,
    .ExternalMux             = AL_ADC_NORMAL_MODE,
    .MaxConvChanNum          = 0,
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
static AL_S32 AlAdc_Test_ReadDataWithIntr(AL_VOID);
static AL_VOID AlAdc_Handler(AL_ADC_EventStruct AdcEvent, AL_VOID *CallbackRef);

/************************** Function Definitions ******************************/

AL_S32 main(AL_VOID)
{
    AL_S32 Ret = AL_OK;

    AL_LOG(AL_LOG_LEVEL_INFO, "Adc read data with intr test\r\n");

    Ret = AlAdc_Test_ReadDataWithIntr();
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Adc read data with intr test failed\r\n");
        return Ret;
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "Adc read data with intr test success\r\n");

    return Ret;
}

static AL_S32 AlAdc_Test_ReadDataWithIntr(AL_VOID)
{
    AL_S32 Ret = AL_OK;
    AL_ADC_HalStruct *Handle;
    AL_U8 IntrData;
    IntrData = AL_ADC_INTR_GTH_BIT;

    Ret = AlAdc_Hal_Init(&Handle, AL_ADC_DEVICE_ID, &AdcInitConfigs, AdcInitChanCfg, AlAdc_Handler);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hal Init error:0x%x\r\n", Ret);
        return Ret;
    }

    Ret = AlAdc_Hal_AdcStartIntr(Handle, IntrData);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Adc Start error:0x%x\r\n", Ret);
        return Ret;
    }
    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    while (1)
    {

    }
}

static AL_VOID AlAdc_Handler(AL_ADC_EventStruct AdcEvent, AL_VOID *CallbackRef)
{
    AL_S32 Index;
    AL_ADC_HalStruct *Handle = (AL_ADC_HalStruct *)CallbackRef;

    if (AdcEvent.Events == AL_ADC_EVENT_GETDATA_DONE) {
        for (Index = 0; Index < 8; Index++) {
            if(AdcEvent.EventData & (1 << Index)) {
                AL_LOG(AL_LOG_LEVEL_INFO, "Adc Data Done channel %d vc is %x \r\n",
                       Index, Handle->Dev.AdcData[Index].ChanData);
            }
        }
    }

    if (AdcEvent.Events == AL_ADC_EVENT_GETDATA_GTH) {
        for (Index = 0; Index < 8; Index++) {
            if(AdcEvent.EventData & (1 << Index)) {
                AL_LOG(AL_LOG_LEVEL_INFO, "Adc Data Gth channel %d vc is %x \r\n",
                       Index, Handle->Dev.AdcData[Index].ChanData);
            }
        }
    }

    if (AdcEvent.Events == AL_ADC_EVENT_GETDATA_LTH) {
        for (Index = 0; Index < 8; Index++) {
            if(AdcEvent.EventData & (1 << Index)) {
                AL_LOG(AL_LOG_LEVEL_INFO, "Adc Data Lth channel %d vc is %x \r\n",
                       Index, Handle->Dev.AdcData[Index].ChanData);
            }
        }
    }
}
