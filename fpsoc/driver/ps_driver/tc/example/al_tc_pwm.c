/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    al_tc_pwm.c
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2023-09-01
 * @brief   tc pwm output example
 */

/***************************** Include Files *********************************/
#include <string.h>
#include <stdlib.h>
#include "al_tc_hal.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/
#define AL_TC_DEVICE_ID             (3)
 /* pwm frequency = clock frequency / pow(2, PrescaleVal + 1) / (AL_TC_INTERVAL_MAX_VAL + 1) */
#define AL_TC_INTERVAL_MAX_VAL      (999)
/* pwm duty cycle = (AL_TC_MATCH1_VAL + 1) / (AL_TC_INTERVAL_MAX_VAL + 1) */
#define AL_TC_MATCH1_VAL            (249)

/************************** Variable Definitions *****************************/
static AL_TC_TimerInitStruct PwmInitConfigs = {
    .CountDec            = AL_TC_CountUp,
    .ClkSrc              = AL_TC_PCLK,
    .EnablePrescale      = AL_TRUE,
    .PrescaleVal         = 2,
};

/************************** Function Prototypes ******************************/
static AL_S32 AlTc_Test_PwmOutput(AL_VOID);

/************************** Function Definitions ******************************/
/**
 * This function serves as the entry point for the program. It initializes the application,
 * runs the PWM output test, and handles the return status. If the test fails, it logs an error
 * message and returns the error code. Otherwise, it returns AL_OK indicating success.
 *
 * @return AL_OK on success, or an error code on failure.
 */
AL_S32 main(AL_VOID)
{
    AL_S32 Ret = AL_OK;

    AL_LOG(AL_LOG_LEVEL_INFO, "Tc pwm output test\r\n");

    Ret = AlTc_Test_PwmOutput();
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Tc pwm output test failed\r\n");
        return Ret;
    }

    return Ret;
}

/**
 * This function initializes the TC device with specific configurations for PWM output,
 * sets up the interval and match values to generate PWM signal, and enables the waveform output.
 * It configures the device to generate a PWM signal with a specific frequency and duty cycle,
 * based on predefined values for the interval max value and match value.
 *
 * @return AL_OK on successful initialization and configuration, or an error code on failure.
 */
static AL_S32 AlTc_Test_PwmOutput(AL_VOID)
{
    AL_U32 Ret = AL_OK;
    AL_TC_HalStruct *TcHandle;

    Ret = AlTc_Hal_Init(&TcHandle, AL_TC_DEVICE_ID, &PwmInitConfigs, AL_NULL);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hal Init error:0x%x\r\n", Ret);
        return Ret;
    }

    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    AlTc_Hal_EnableIntervalMode(TcHandle);
    AlTc_Hal_SetIntervalMaxVal(TcHandle, AL_TC_INTERVAL_MAX_VAL);

    AlTc_Hal_EnableMatchMode(TcHandle, AL_TRUE);
    AlTc_Hal_SetMatchVal(TcHandle, AL_TC_Match1, AL_TC_MATCH1_VAL);

    AlTc_Hal_SetWaveformPolarity(TcHandle, AL_TC_Negedge);

    AlTc_Hal_EnableIntr(TcHandle, AL_TC_IntrInterval, AL_TRUE);
    AlTc_Hal_EnableIntr(TcHandle, AL_TC_IntrMatch1, AL_TRUE);

    AlTc_Hal_EnableWaveOutput(TcHandle, AL_TRUE);
    AlTc_Hal_EnableCounter(TcHandle, AL_TRUE);


    return Ret;
}