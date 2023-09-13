/**
 * @file    al_wdt_test_config.h
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2023-05-17
 * @brief   wdt test config file
 */

#ifndef AL_WDT_TEST_CONFIG_H
#define AL_WDT_TEST_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#include "al_wdt_hal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/************************** Constant Definitions *****************************/

/***************** Macros (Inline Functions) Definitions *********************/

/**************************** Type Definitions *******************************/

/************************** Variable Definitions *****************************/
static AL_WDT_InitStruct WDT_InitStruct = {
    .ResetPuaseLength = WDT_RPL_PCLK_CYCLES_8,
    .ResponseMode = WDT_INTR_MODE,
    .TimeOutValue = WDT_TIMEOUT_PERIOD_32M_CLOCKS,
};

/************************** Function Prototypes ******************************/

#ifdef __cplusplus
}
#endif

#endif
