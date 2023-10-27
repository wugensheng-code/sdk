/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_adc_test_config.h"

static AL_S32 AlAdc_AlarmFunc_Test(AL_VOID);
static AL_S32 AlAdc_ContinuousMode_Test(AL_VOID);
static AL_VOID AlAdc_Handler(AL_ADC_EventStruct AdcEvent, AL_VOID *CallbackRef);

AL_VOID main()
{
#if CONFIG_AlAdc_AlarmFunc_Test
    AL_S32 Ret = AL_OK;

    AL_LOG(AL_LOG_LEVEL_INFO, "Adc alarm function through interrupts test\r\n");

    Ret = AlAdc_AlarmFunc_Test();
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Adc alarm function interrupts test failed\r\n");
        return Ret;
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "Adc alarm function through interrupts test success\r\n");

    return AL_OK;

#endif

#if CONFIG_AlAdc_ContinuousMode_Test
    AL_S32 Ret = AL_OK;

    AL_LOG(AL_LOG_LEVEL_INFO, "Adc in continuous mode read data through polled test\r\n");

    Ret = AlAdc_ContinuousMode_Test();
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Adc in continuous mode read data through polled test failed\r\n");
        return Ret;
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "Adc in continuous mode read data through polled test success\r\n");

    return AL_OK;
#endif
}

static AL_S32 AlAdc_AlarmFunc_Test(AL_VOID)
{
    AL_S32 Ret = AL_OK;
    AL_ADC_HalStruct *Handle;
    AL_S32 Index = 0;
    AL_U32 PlAdcFunc = AL_ADC_DATA_GTH;

    Ret = AlAdc_Hal_Init(&Handle, AL_ADC_DEVICE_ID, &AdcInitCfg, ChanInitCfg, AlAdc_Handler);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hal Init error:0x%x\r\n", Ret);
        return Ret;
    }

    Ret = AlAdc_Hal_AdcStartIntr(Handle, PlAdcFunc);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Adc Start error:0x%x\r\n", Ret);
        return Ret;
    }

    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    while (EVENT_FLAG != AL_TRUE);

    AlAdc_Hal_AdcStopIntr(Handle);
    return AL_OK;
}

static AL_VOID AlAdc_Handler(AL_ADC_EventStruct AdcEvent, AL_VOID *CallbackRef)
{
    AL_S32 Index;
    AL_ADC_HalStruct *Handle = (AL_ADC_HalStruct *)CallbackRef;

    if (AdcEvent.Events == AL_ADC_EVENT_DATA_GTH) {
        for (Index = 0; Index < 8; Index++) {
            if(AdcEvent.EventData & (1 << Index)) {
                AL_LOG(AL_LOG_LEVEL_INFO, "Adc Data Gth channel %d vc is %x \r\n",
                       Index, Handle->Dev.AdcData[Index].ChanData);
            }
        }
    }

    EVENT_FLAG = AL_TRUE;
}

static AL_S32 AlAdc_ContinuousMode_Test(AL_VOID)
{
    AL_ADC_HalStruct *Handle;
    AL_S32 Ret = AL_OK;
    AL_U32 Index;
    AL_U32 Counts = 0;
    AL_U32 Iterations = 1000;
    AL_U32 PlAdcFunc = AL_ADC_DATA_DONE;

    Ret = AlAdc_Hal_Init(&Handle, AL_ADC_DEVICE_ID, &AdcInitCfg, ChanInitCfg, AL_NULL);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hal Init error:0x%x\r\n", Ret);
        return Ret;
    }

    Ret = AlAdc_Hal_AdcStart(Handle, PlAdcFunc);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Adc Start error:0x%x\r\n", Ret);
        return Ret;
    }

    while (Counts < Iterations) {
        for (Index = 0; Index <= Handle->Dev.Configs.ConvChanNum; Index++) {
            AL_LOG(AL_LOG_LEVEL_INFO, "Adc channel %d vc is %x \r\n", Index,
                    AlAdc_Hal_GetAdcData(Handle, ChanInitCfg[Index].ChanNum));
        }
        AlAdc_Hal_ClrPlAdcIntr(Handle, AL_ADC_INTR_DONE_PL);
        Counts++;
    }

    AlAdc_Hal_AdcStop(Handle);
    return AL_OK;
}
