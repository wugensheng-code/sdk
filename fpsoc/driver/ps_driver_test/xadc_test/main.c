/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_xadc_test_config.h"

static AL_S32 AlXadc_ReadDataWithPolled(AL_VOID);
static AL_S32 AlXadc_Test_ReadDataWithIntr(AL_VOID);
static AL_VOID AlXadc_Handler(AL_XADC_EventStruct XadcEvent, AL_VOID *CallbackRef);

AL_VOID main()
{
#if CONFIG_AlXadc_ReadDataWithPolled_TEST
    AL_LOG(AL_LOG_LEVEL_INFO, "AlXadc polling read data test example ...\r\n");
    AL_S32 Ret = AlXadc_ReadDataWithPolled();
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Xadc read data with polling test failed\r\n");
        return Ret;
    }

#endif

#if CONFIG_AlXadc_Test_ReadDataWithIntr_TEST
    AL_LOG(AL_LOG_LEVEL_INFO, "AlXadc Intr read data test example ...\r\n");
    AL_S32 Ret = AlXadc_Test_ReadDataWithIntr();
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Xadc read data with intr test failed\r\n");
        return Ret;
    }
#endif

}

AL_S32 AlXadc_ReadDataWithPolled(AL_VOID)
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

AL_S32 AlXadc_Test_ReadDataWithIntr(AL_VOID)
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