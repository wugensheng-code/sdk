/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_ttc_test_config.h"

static AL_S32 AlTtc_MeasurePulseWidth(AL_VOID);
static AL_S32 AlTtc_PwmOutput(AL_VOID);


AL_VOID main()
{
#if CONFIG_AlTtc_MeasurePulseWidth_TEST
    AL_LOG(AL_LOG_LEVEL_INFO, "AL_TTC_Measure_PulseWidth.\r\n");
    AL_LOG(AL_LOG_LEVEL_INFO, "Please input the signal to be measured.\r\n");
    AlTtc_MeasurePulseWidth();

#endif

#if CONFIG_AlTtc_PwmOutput_TEST
    AL_LOG(AL_LOG_LEVEL_INFO, "AL_TTC_Pwn_Otput.\r\n");
    AlTtc_PwmOutput();

#endif
}

AL_S32 AlTtc_MeasurePulseWidth(AL_VOID)
{
    AL_TTC_HalStruct *TtcHandle;

    AlTtc_Hal_Init(&TtcHandle, AL_TTC_DEVICE_ID, &EventInitConfigs, AL_NULL);
    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);
    AlTtc_Hal_SetExtClkEdge(TtcHandle, AL_TTC_Negedge);
    AlTtc_Hal_SetEventTimerOvState(TtcHandle, AL_TRUE);
    AlTtc_Hal_SetEventTimerLevel(TtcHandle, AL_TTC_HighLevel);
    AlTtc_Hal_EnableEventTimerMode(TtcHandle, AL_TRUE);
    AlTtc_Hal_EnableIntr(TtcHandle, AL_TTC_IntrEventTimer, AL_TRUE);
    AlTtc_Hal_EnableCounter(TtcHandle, AL_TRUE);

    while (1) {
        AL_U16 Val = AltTtc_Hal_GetEventTimerVal(TtcHandle);
        AL_LOG(AL_LOG_LEVEL_INFO, "Event val is %x\r\n", Val);
    }

    return AL_OK;
}

AL_S32 AlTtc_PwmOutput(AL_VOID)
{
    AL_TTC_HalStruct *TtcHandle;
    AlTtc_Hal_Init(&TtcHandle, AL_TTC_DEVICE_ID, &PwmInitConfigs, AL_NULL);

    AlTtc_Hal_EnableIntervalMode(TtcHandle);
    AlTtc_Hal_SetIntervalMaxVal(TtcHandle, AL_TTC_INTERVAL_MAX_VAL);

    AlTtc_Hal_EnableMatchMode(TtcHandle, AL_TRUE);
    AlTtc_Hal_SetMatchVal(TtcHandle, AL_TTC_Match1, AL_TTC_MATCH1_VAL);

    AlTtc_Hal_SetWaveformPolarity(TtcHandle, AL_TTC_Posedge);

    AlTtc_Hal_EnableIntr(TtcHandle, AL_TTC_IntrInterval, AL_TRUE);
    AlTtc_Hal_EnableIntr(TtcHandle, AL_TTC_IntrMatch1, AL_TRUE);

    AlTtc_Hal_EnableWaveOutput(TtcHandle, AL_TRUE);
    AlTtc_Hal_EnableCounter(TtcHandle, AL_TRUE);

    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    return AL_OK;
}