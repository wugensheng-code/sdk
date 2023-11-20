/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    al_ttc_pwm.c
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2023-09-01
 * @brief   ttc pwm output example
 */

/***************************** Include Files *********************************/
#include <string.h>
#include <stdlib.h>
#include "al_ttc_hal.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/
#define AL_TTC_DEVICE_ID             (3)
 /* pwm frequency = clock frequency / pow(2, PrescaleVal + 1) / (AL_TTC_INTERVAL_MAX_VAL + 1) */
#define AL_TTC_INTERVAL_MAX_VAL      (999)
/* pwm duty cycle = (AL_TTC_MATCH1_VAL + 1) / (AL_TTC_INTERVAL_MAX_VAL + 1) */
#define AL_TTC_MATCH1_VAL            (249)

/************************** Variable Definitions *****************************/
static AL_TTC_TimerInitStruct PwmInitConfigs = {
    .CountDec            = AL_TTC_CountUp,
    .ClkSrc              = AL_TTC_PCLK,
    .EnablePrescale      = AL_TRUE,
    .PrescaleVal         = 2,
};

/************************** Function Prototypes ******************************/
static AL_S32 AlTtc_Test_PwmOutput(AL_VOID);

/************************** Function Definitions ******************************/

AL_S32 main(AL_VOID)
{
    AL_S32 Ret = AL_OK;

    AL_LOG(AL_LOG_LEVEL_INFO, "Ttc pwm output test\r\n");

    Ret = AlTtc_Test_PwmOutput();
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Ttc pwm output test failed\r\n");
        return Ret;
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "Ttc pwm output test success\r\n");

    return Ret;
}

static AL_S32 AlTtc_Test_PwmOutput(AL_VOID)
{
    AL_U32 Ret = AL_OK;
    AL_TTC_HalStruct *TtcHandle;

    Ret = AlTtc_Hal_Init(&TtcHandle, AL_TTC_DEVICE_ID, &PwmInitConfigs, AL_NULL);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hal Init error:0x%x\r\n", Ret);
        return Ret;
    }

    AlTtc_Hal_EnableIntervalMode(TtcHandle);
    AlTtc_Hal_SetIntervalMaxVal(TtcHandle, AL_TTC_INTERVAL_MAX_VAL);

    AlTtc_Hal_EnableMatchMode(TtcHandle, AL_TRUE);
    AlTtc_Hal_SetMatchVal(TtcHandle, AL_TTC_Match1, AL_TTC_MATCH1_VAL);

    AlTtc_Hal_SetWaveformPolarity(TtcHandle, AL_TTC_Negedge);

    AlTtc_Hal_EnableIntr(TtcHandle, AL_TTC_IntrInterval, AL_TRUE);
    AlTtc_Hal_EnableIntr(TtcHandle, AL_TTC_IntrMatch1, AL_TRUE);

    AlTtc_Hal_EnableWaveOutput(TtcHandle, AL_TRUE);
    AlTtc_Hal_EnableCounter(TtcHandle, AL_TRUE);

    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    AL_LOG(AL_LOG_LEVEL_INFO, "Ttc pwm output in progress\r\n");

    return Ret;
}