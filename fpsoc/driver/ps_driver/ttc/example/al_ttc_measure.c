/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    al_ttc_measure.c
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2023-09-01
 * @brief   ttc measure pulse width example
 */

/***************************** Include Files *********************************/
#include <string.h>
#include <stdlib.h>
#include "al_ttc_hal.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/
#define AL_TTC_DEVICE_ID             (3)

/************************** Variable Definitions *****************************/
static AL_TTC_TimerInitStruct EventInitConfigs = {
    .CountDec            = AL_TTC_CountUp,
    .ClkSrc              = AL_TTC_EXTCLK,
    .EnablePrescale      = AL_TRUE,
    .PrescaleVal         = 2,
};

/************************** Function Prototypes ******************************/
static AL_S32 AlTtc_Test_MeasurePulseWidth(AL_VOID);

/************************** Function Definitions ******************************/

AL_S32 main(AL_VOID)
{
    AL_S32 Ret = AL_OK;

    AL_LOG(AL_LOG_LEVEL_INFO, "Ttc measure pulse width test\r\n");

    Ret = AlTtc_Test_MeasurePulseWidth();
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Ttc measure pulse width test failed\r\n");
        return Ret;
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "Ttc measure pulse width test success\r\n");

    return Ret;
}

static AL_S32 AlTtc_Test_MeasurePulseWidth(AL_VOID)
{
    AL_U32 Ret = AL_OK;
    AL_TTC_HalStruct *TtcHandle;

    Ret = AlTtc_Hal_Init(&TtcHandle, AL_TTC_DEVICE_ID, &EventInitConfigs, AL_NULL);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hal Init error:0x%x\r\n", Ret);
        return Ret;
    }
    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);
    AlTtc_Hal_SetExtClkEdge(TtcHandle, AL_TTC_Negedge);
    AlTtc_Hal_SetEventTimerOvState(TtcHandle, AL_TRUE);
    AlTtc_Hal_SetEventTimerLevel(TtcHandle, AL_TTC_LowLevel);
    AlTtc_Hal_EnableEventTimerMode(TtcHandle, AL_TRUE);
    AlTtc_Hal_EnableIntr(TtcHandle, AL_TTC_IntrEventTimer, AL_TRUE);
    AlTtc_Hal_EnableCounter(TtcHandle, AL_TRUE);

    while (1) {
        AL_U16 Val = AltTtc_Hal_GetEventTimerVal(TtcHandle);
        AL_LOG(AL_LOG_LEVEL_INFO, "Event val is %x\r\n", Val);
    }

    return AL_OK;
}