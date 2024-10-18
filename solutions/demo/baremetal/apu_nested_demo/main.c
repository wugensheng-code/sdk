/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***************************** Include Files *********************************/
#include <string.h>
#include <stdlib.h>
#include "al_hal.h"
#include "al_aarch64_core.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/
#define AL_TC_DEVICE_ID             (3)
 /* pwm frequency = clock frequency / pow(2, PrescaleVal + 1) / (AL_TC_INTERVAL_MAX_VAL + 1) */
#define AL_TC_INTERVAL_MAX_VAL      (48828 - 1)
/* pwm duty cycle = (AL_TC_MATCH1_VAL + 1) / (AL_TC_INTERVAL_MAX_VAL + 1) */
#define AL_TC_MATCH1_VAL            (499)  //500ms timer

#define AL_GPIO_DEVICE_ID  0
/* use SD_CD pin as key */
#define PS_KEY_1 0
/************************** Variable Definitions *****************************/
static AL_TC_TimerInitStruct PwmInitConfigs = {
    .CountDec            = AL_TC_CountUp,
    .ClkSrc              = AL_TC_PCLK,
    .EnablePrescale      = AL_TRUE,
    .PrescaleVal         = 10,
};

static AL_U32 GpioFlag = 0;
AL_TC_HalStruct *TcHandle;
AL_GPIO_HalStruct *GPIO;
/************************** Function Prototypes ******************************/
static AL_VOID Nest_Tc_EventHandler(AL_TC_EventStruct TcEvent, AL_VOID *CallbackRef);
static AL_VOID Nest_Gpio_EventCallBack(AL_GPIO_EventStruct GpioEvent, AL_VOID *CallbackRef);

/************************** Function Definitions ******************************/

AL_S32 main(AL_VOID)
{
    AL_U32 Ret = AL_OK;
    AL_U32 CpuId = AlCore_GetCoreId();
    AL_INTR_AttrStrct AttrTc = {.Priority = 0x30, .TrigMode = LEVEL_HIGH_TRIGGER};

	AlIntr_SetPreemptionBitsCount(0);

    Ret = AlTc_Hal_Init(&TcHandle, AL_TC_DEVICE_ID, &PwmInitConfigs, Nest_Tc_EventHandler);
    if (Ret == AL_OK) {
        AL_LOG(AL_LOG_LEVEL_INFO, "[TEST] APU TC initsuccess\r\n");
    } else {
        AL_LOG(AL_LOG_LEVEL_INFO, "[TEST] APU TC init failed\r\n");
    }

    AlGicv3_SetInterruptPriority(TcHandle->Dev.IntrNum, CpuId, AttrTc.Priority);

    AlTc_Hal_EnableIntervalMode(TcHandle);

    AlTc_Hal_SetIntervalMaxVal(TcHandle, AL_TC_INTERVAL_MAX_VAL);

    AlTc_Hal_EnableIntr(TcHandle, AL_TC_IntrInterval, AL_TRUE);

    AlTc_Hal_EnableCounter(TcHandle, AL_TRUE);

    Ret = AlGpio_Hal_Init(&GPIO, AL_GPIO_DEVICE_ID, Nest_Gpio_EventCallBack);
    if (Ret == AL_OK) {
        AL_LOG(AL_LOG_LEVEL_INFO, "[TEST] APU AlGpio_Hal_Init success\r\n");
    } else {
        AL_LOG(AL_LOG_LEVEL_INFO, "[TEST] APU AlGpio_Hal_Init failed\r\n");
    }

#if 0
    AlGpio_Hal_IntrPinCfg(GPIO, PS_KEY_1, GPIO_INTR_TYPE_EDGE_FALLING);
    AL_INTR_AttrStrct Attr = {.Priority = 0x20, .TrigMode = LEVEL_HIGH_TRIGGER};
    (AL_VOID)AlIntr_RegHandler(Handle->HwConfig.IntrId, &Attr, AlGpio_Hal_IntrHandler, Handle);
#endif

    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    while (1) {}

    return 0;
}

static AL_VOID Nest_Tc_EventHandler(AL_TC_EventStruct TcEvent, AL_VOID *CallbackRef)
{
    switch (TcEvent.Events)
    {
    case AL_TC_EVENT_Interval:
        AL_LOG(AL_LOG_LEVEL_INFO, "Al tc generated a interval interrupt\r\n");
        //AlTc_Hal_EnableIntr(TcHandle, AL_TC_IntrInterval, AL_FALSE);
        // AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);
        while (GpioFlag != 1);
        GpioFlag = 0;
        //AlTc_Hal_EnableIntr(TcHandle, AL_TC_IntrInterval, AL_TRUE);
        break;
    case AL_TC_EVENT_Match1:
        AL_LOG(AL_LOG_LEVEL_INFO, "Al tc generated a match1 interrupt\r\n");
        break;
    case AL_TC_EVENT_Match2:
        AL_LOG(AL_LOG_LEVEL_INFO, "Al tc generated a match2 interrupt\r\n");
        break;
    case AL_TC_EVENT_Match3:
        AL_LOG(AL_LOG_LEVEL_INFO, "Al tc generated a match3 interrupt\r\n");
        break;
    case AL_TC_EVENT_Overflow:
        AL_LOG(AL_LOG_LEVEL_INFO, "Al tc generated a overflow interrupt\r\n");
        break;
    case AL_TC_EVENT_EventTimer:
        AL_LOG(AL_LOG_LEVEL_INFO, "Al tc generated a event timer interrupt\r\n");
        break;
    default:
        break;
    }
}

static AL_VOID Nest_Gpio_EventCallBack(AL_GPIO_EventStruct GpioEvent, AL_VOID *CallbackRef)
{
    AL_LOG(AL_LOG_LEVEL_INFO, "Bank 0x%x: IntrStatus is 0x%x\r\n", GpioEvent.Bank, GpioEvent.EventData);
    GpioFlag = 1;
}