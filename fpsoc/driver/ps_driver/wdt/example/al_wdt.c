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

AL_VOID AlWdt_Hal_CustomEventHandler(AL_VOID *CallbackRef)
{
    AL_WDT_HalStruct *Handle = (AL_WDT_HalStruct *)CallbackRef;

    AL_LOG(AL_LOG_LEVEL_INFO, "WDT interrupt occurred\r\n");

}

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
