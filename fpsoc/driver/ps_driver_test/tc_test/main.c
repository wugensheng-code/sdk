/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_tc_test_config.h"

static AL_S32 AlTc_MeasurePulseWidth(AL_VOID);
static AL_S32 AlTc_PwmOutput(AL_VOID);


AL_VOID main()
{
#if CONFIG_AlTc_MeasurePulseWidth_TEST
    AL_LOG(AL_LOG_LEVEL_INFO, "AL_TC_Measure_PulseWidth.\r\n");
    AL_LOG(AL_LOG_LEVEL_INFO, "Please input the signal to be measured.\r\n");
    AlTc_MeasurePulseWidth();

#endif

#if CONFIG_AlTc_PwmOutput_TEST
    AL_LOG(AL_LOG_LEVEL_INFO, "AL_TC_Pwn_Otput.\r\n");
    AlTc_PwmOutput();

#endif
}

AL_S32 AlTc_MeasurePulseWidth(AL_VOID)
{
    AL_TC_HalStruct *TcHandle;

    AlTc_Hal_Init(&TcHandle, AL_TC_DEVICE_ID, &EventInitConfigs, AL_NULL);
    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);
    AlTc_Hal_SetExtClkEdge(TcHandle, AL_TC_Negedge);
    AlTc_Hal_SetEventTimerOvState(TcHandle, AL_TRUE);
    AlTc_Hal_SetEventTimerLevel(TcHandle, AL_TC_HighLevel);
    AlTc_Hal_EnableEventTimerMode(TcHandle, AL_TRUE);
    AlTc_Hal_EnableIntr(TcHandle, AL_TC_IntrEventTimer, AL_TRUE);
    AlTc_Hal_EnableCounter(TcHandle, AL_TRUE);

    while (1) {
        AL_U16 Val = AlTc_Hal_GetEventTimerVal(TcHandle);
        AL_LOG(AL_LOG_LEVEL_INFO, "Event val is %x\r\n", Val);
    }

    return AL_OK;
}

AL_S32 AlTc_PwmOutput(AL_VOID)
{
    AL_TC_HalStruct *TcHandle;
    AlTc_Hal_Init(&TcHandle, AL_TC_DEVICE_ID, &PwmInitConfigs, AL_NULL);

    AlTc_Hal_EnableIntervalMode(TcHandle);
    AlTc_Hal_SetIntervalMaxVal(TcHandle, AL_TC_INTERVAL_MAX_VAL);

    AlTc_Hal_EnableMatchMode(TcHandle, AL_TRUE);
    AlTc_Hal_SetMatchVal(TcHandle, AL_TC_Match1, AL_TC_MATCH1_VAL);

    AlTc_Hal_SetWaveformPolarity(TcHandle, AL_TC_Posedge);

    AlTc_Hal_EnableIntr(TcHandle, AL_TC_IntrInterval, AL_TRUE);
    AlTc_Hal_EnableIntr(TcHandle, AL_TC_IntrMatch1, AL_TRUE);

    AlTc_Hal_EnableWaveOutput(TcHandle, AL_TRUE);
    AlTc_Hal_EnableCounter(TcHandle, AL_TRUE);

    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    return AL_OK;
}