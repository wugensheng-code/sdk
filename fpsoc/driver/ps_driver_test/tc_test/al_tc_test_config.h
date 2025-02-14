/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    al_tc_test_config.h
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date
 * @brief   tc test config file
 */

#ifndef AL_TC_TEST_CONFIG_H
#define AL_TC_TEST_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#include "al_tc_hal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/************************** Constant Definitions *****************************/
#define AL_TC_DEVICE_ID             (0)
 /* pwm frequency = clock frequency / (pow(2, PrescaleVal+1) / (AL_TC_INTERVAL_MAX_VAL + 1) */
#define AL_TC_INTERVAL_MAX_VAL      (499)
/* pwm duty cycle = (AL_TC_MATCH1_VAL + 1) / (AL_TC_INTERVAL_MAX_VAL + 1) */
#define AL_TC_MATCH1_VAL            (249)


/***************** Macros (Inline Functions) Definitions *********************/

/**************************** Type Definitions *******************************/

/************************** Variable Definitions *****************************/
static AL_TC_TimerInitStruct EventInitConfigs = {
    .CountDec            = AL_TC_CountUp,
    .ClkSrc              = AL_TC_PCLK,
    .EnablePrescale      = AL_TRUE,
    .PrescaleVal         = 2,
};

static AL_TC_TimerInitStruct PwmInitConfigs = {
    .CountDec            = AL_TC_CountUp,
    .ClkSrc              = AL_TC_EXTCLK,
    .EnablePrescale      = AL_TRUE,
    .PrescaleVal         = 2,
};


/************************** Function Prototypes ******************************/

#ifdef __cplusplus
}
#endif

#endif
