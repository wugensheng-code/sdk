/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_gpio_hal.h"

#define AL_GPIO_DEVICE_ID  0
#define LED                14
#define PS_KEY_1           10
#define PS_KEY_2           11

static AL_S32 AlGpio_Test_EventCallBack(AL_GPIO_EventStruct Event, AL_VOID *CallBackRef)
{
    AL_LOG(AL_LOG_LEVEL_INFO, "Bank 0x%x: IntrStatus is 0x%x", Event.Bank, Event.EventData);
    AL_GPIO_HalStruct *Handle = (AL_GPIO_HalStruct *)CallBackRef;
    AlGpio_Hal_MaskBankIntr(Handle, Event.Bank, Event.EventData);
}

int AlGpio_Hal_Test()
{
    AL_GPIO_HalStruct *GPIO;
    AL_U32 RawVal = 0;

    /* 1、Test AlGpio_Hal_Init */
    AL_S32 ret = AlGpio_Hal_Init(&GPIO, AL_GPIO_DEVICE_ID, AlGpio_Test_EventCallBack);

    if (ret == AL_OK) {
        AL_LOG(AL_LOG_LEVEL_INFO, "[TEST] APU AlGpio_Hal_Init success");
    }
    else {
        AL_LOG(AL_LOG_LEVEL_INFO, "[TEST] APU AlGpio_Hal_Init failed");
    }

    /* 2、Test LED function */
    AlGpio_Hal_WritePin(GPIO, LED, 0x0);
    AlGpio_Hal_ReadPinOutput(GPIO, LED);
    AlGpio_Hal_WritePin(GPIO, LED, 0x1);
    AlGpio_Hal_ReadPinOutput(GPIO, LED);

    /* 3、Test InputRead EOI register */
    for(int i = 0;i < 10;i++) {
        AL_LOG(AL_LOG_LEVEL_INFO, "The Input Data Value of GPIO Pin %d is 0x%x", PS_KEY_1, AlGpio_Hal_ReadPinInput(GPIO,PS_KEY_1));
    }

    /* 4、Test intr */
    AlGpio_Hal_IntrPinCfg(GPIO, PS_KEY_1, GPIO_INTR_TYPE_LEVEL_HIGH);
    AlGpio_Hal_IntrPinCfg(GPIO, PS_KEY_2, GPIO_INTR_TYPE_EDGE_FALLING);

    AlSys_MDelay(5000);
    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    RawVal = AlGpio_Hal_GetBankRawIntrStatus(GPIO, AL_GPIO_BANK0);
    AL_LOG(AL_LOG_LEVEL_INFO, "The IntrRaw Value of GPIO Bank 0x%x  is 0x%x", AL_GPIO_BANK0, RawVal);
    while(1);

    return 0;
}


int main(void) {
    AL_LOG(AL_LOG_LEVEL_INFO, "[TEST]AlGpio_Hal_Test start");
    AlGpio_Hal_Test();

    return 0;
}
