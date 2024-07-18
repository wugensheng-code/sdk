/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    al_tc_measure.c
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2023-09-01
 * @brief   tc measure pulse width example
 */

/***************************** Include Files *********************************/
#include <string.h>
#include <stdlib.h>
#include "al_tc_hal.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/
#define AL_TC_DEVICE_ID             (3)

/************************** Variable Definitions *****************************/
static AL_TC_TimerInitStruct EventInitConfigs = {
    .CountDec            = AL_TC_CountUp,
    .ClkSrc              = AL_TC_EXTCLK,
    .EnablePrescale      = AL_TRUE,
    .PrescaleVal         = 2,
};

/************************** Function Prototypes ******************************/
static AL_S32 AlTc_Test_MeasurePulseWidth(AL_VOID);

/************************** Function Definitions ******************************/
/**
 * This function initializes the application, runs the pulse width measurement test,
 * and reports the test result. If the test fails, it logs an error message and returns
 * an error code. On success, it logs a success message and returns AL_OK.
 *
 * @return AL_OK if the test succeeds, error code otherwise.
 */
AL_S32 main(AL_VOID)
{
    AL_S32 Ret = AL_OK;

    AL_LOG(AL_LOG_LEVEL_INFO, "Tc measure pulse width test\r\n");

    Ret = AlTc_Test_MeasurePulseWidth();
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Tc measure pulse width test failed\r\n");
        return Ret;
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "Tc measure pulse width test success\r\n");

    return Ret;
}

/**
 * This function initializes the TC hardware with predefined configurations, sets up the
 * event timer for pulse width measurement, and continuously reads and logs the event timer value.
 * The function runs indefinitely, logging the value of the event timer.
 *
 * @return AL_OK if the TC hardware is successfully initialized, error code otherwise.
 */
static AL_S32 AlTc_Test_MeasurePulseWidth(AL_VOID)
{
    AL_U32 Ret = AL_OK;
    AL_TC_HalStruct *TcHandle;

    Ret = AlTc_Hal_Init(&TcHandle, AL_TC_DEVICE_ID, &EventInitConfigs, AL_NULL);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hal Init error:0x%x\r\n", Ret);
        return Ret;
    }
    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);
    AlTc_Hal_SetExtClkEdge(TcHandle, AL_TC_Negedge);
    AlTc_Hal_SetEventTimerOvState(TcHandle, AL_TRUE);
    AlTc_Hal_SetEventTimerLevel(TcHandle, AL_TC_LowLevel);
    AlTc_Hal_EnableEventTimerMode(TcHandle, AL_TRUE);
    AlTc_Hal_EnableIntr(TcHandle, AL_TC_IntrEventTimer, AL_TRUE);
    AlTc_Hal_EnableCounter(TcHandle, AL_TRUE);

    while (1) {
        AL_U16 Val = AlTc_Hal_GetEventTimerVal(TcHandle);
        AL_LOG(AL_LOG_LEVEL_INFO, "Event val is %x\r\n", Val);
    }

    return AL_OK;
}