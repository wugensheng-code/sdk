/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    al_wdt.c
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2023-09-01
 * @brief   wdt interrupt example
 */

/***************************** Include Files *********************************/
#include <string.h>
#include <stdlib.h>
#include "al_wdt_hal.h"


static AL_S32 AlWdt_Test_Interrupt_Mode(AL_VOID);


static AL_WDT_InitStruct WDT_InitStruct = {
    .ResetPuaseLength = WDT_RPL_PCLK_CYCLES_8,
    .ResponseMode = WDT_INTR_MODE,
    .TimeOutValue = WDT_TIMEOUT_PERIOD_32M_CLOCKS,
};

/**
 *
 * This function serves as the starting point for the application. It begins by logging a message indicating
 * the start of the WDT interrupt mode example test. It then calls the AlWdt_Test_Interrupt_Mode function to
 * initialize and test the WDT in interrupt mode. If the test encounters an error, it logs an error message
 * and returns the error code.
 *
 * @return Returns AL_OK if the WDT interrupt mode example test completes successfully, or an error code if it fails.
 */
AL_S32 main(AL_VOID)
{
    AL_S32 Ret = AL_OK;

    AL_LOG(AL_LOG_LEVEL_INFO, "AlWdt Interrupt mode example testing...\r\n");
    Ret = AlWdt_Test_Interrupt_Mode();
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_INFO, "AlWdt Interrupt mode examplee test error\r\n");
        return Ret;
    }

}

/**
 *
 * This function is designed to be called when a WDT interrupt occurs. It logs a message indicating that the
 * WDT interrupt has occurred. This function is registered as a callback in the WDT initialization process and
 * demonstrates handling of WDT interrupts in a system.
 *
 * @param CallbackRef A pointer to user-defined data, in this case, an AL_WDT_HalStruct structure representing the WDT device.
 */
AL_VOID AlWdt_Hal_CustomEventHandler(AL_VOID *CallbackRef)
{
    AL_WDT_HalStruct *Handle = (AL_WDT_HalStruct *)CallbackRef;

    AL_LOG(AL_LOG_LEVEL_INFO, "WDT interrupt occurred\r\n");

}

/**
 *
 * This function performs the initialization of a WDT device in interrupt mode using predefined settings. It
 * logs an error message and returns if the initialization fails. After successful initialization, it enables
 * local interrupts and waits for a specified duration before entering an infinite loop where it continuously
 * "kicks" or "feeds" the WDT to prevent it from resetting the system. This demonstrates the use of a WDT to
 * monitor system health and prevent system lock-up.
 *
 * @return Returns AL_OK on successful initialization and operation, or an error code on failure.
 */
static AL_S32 AlWdt_Test_Interrupt_Mode(AL_VOID)
{
    AL_WDT_HalStruct *Wdt0;

    AL_S32 Ret = AlWdt_Hal_Init(&Wdt0, 0, &WDT_InitStruct, AlWdt_Hal_CustomEventHandler);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_INFO, "AlWdt_Hal_Init error\r\n");
        return Ret;
    }

    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);


    AlSys_MDelay(5000);


    AL_LOG(AL_LOG_LEVEL_INFO, "kicking the dog\r\n");
    while (1)
    {
        AlWdt_Hal_Feed(Wdt0);
    }

}
