/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_adc_test_config.h"

static AL_S32 AlAdc_ReadDataWithPolled(AL_VOID);
static AL_S32 AlAdc_Test_ReadDataWithIntr(AL_VOID);
static AL_VOID AlAdc_Handler(AL_ADC_EventStruct AdcEvent, AL_VOID *CallbackRef);

AL_VOID main()
{
#if CONFIG_AlAdc_ReadDataWithPolled_TEST
    AL_LOG(AL_LOG_LEVEL_INFO, "AlAdc polling read data test example ...\r\n");
    AL_S32 Ret = AlAdc_ReadDataWithPolled();
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Adc read data with polling test failed\r\n");
        return;
    }

#endif

#if CONFIG_AlAdc_Test_ReadDataWithIntr_TEST
    AL_LOG(AL_LOG_LEVEL_INFO, "AlAdc Intr read data test example ...\r\n");
    AL_S32 Ret = AlAdc_Test_ReadDataWithIntr();
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Adc read data with intr test failed\r\n");
        return;
    }
#endif

}

AL_S32 AlAdc_ReadDataWithPolled(AL_VOID)
{
    AL_S32 Ret = AL_OK;
    AL_ADC_HalStruct *Handle;
    AL_S32 Index = 0;

    Ret = AlAdc_Hal_Init(&Handle, AL_ADC_DEVICE_ID, &AdcInitConfigs, AdcInitChanCfg, AL_NULL);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hal Init error:0x%x\r\n", Ret);
        return Ret;
    }

    Ret = AlAdc_Hal_AdcStart(Handle);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Adc Start error:0x%x\r\n", Ret);
        return Ret;
    }

    while (1) {
        for (Index = 0; Index < 8; Index++) {
            AL_LOG(AL_LOG_LEVEL_INFO, "Adc channel %d vc is %x \r\n", Index,
                   AlAdc_Hal_GetAdcData(Handle, AdcInitChanCfg[Index].ChanNum));
        }
    }
}

AL_S32 AlAdc_Test_ReadDataWithIntr(AL_VOID)
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