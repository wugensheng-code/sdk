/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    al_gpio_intr.c
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2023-09-08
 * @brief   gpio intrrupt example
 */
#include "al_gpio_hal.h"

#define PS_KEY_1 10
#define PS_KEY_2 11
#define PL_KEY   54
#define AL_GPIO_DEVICE_ID  0
#define BANK_INTR_VALUE    0xc00
#define BANK_TYPE_REG      0xc00
#define BANK_POLARUTY_REG  0x0
#define BANK_BOTHEDGE_REG  0x0


/**
 *
 * This function demonstrates how to trigger an interrupt for a specific GPIO bank or a specific pin.
 *
 * @return AL_OK if successful.
 *
 */
AL_S32 AlGpio_Hal_Intr_Example()
{
    AL_GPIO_HalStruct *GPIO;
    AL_S32 i = 0;

    /* 1、Test AlGpio_Hal_Init */
    AL_S32 ret = AlGpio_Hal_Init(&GPIO, AL_GPIO_DEVICE_ID, AL_NULL);

    if (ret == AL_OK) {
        AL_LOG(AL_LOG_LEVEL_INFO, "[TEST] APU AlGpio_Hal_Init success");
    }
    else {
        AL_LOG(AL_LOG_LEVEL_INFO, "[TEST] APU AlGpio_Hal_Init failed");
    }

    /* 2、Test InputRead EXT register */
    for(i = 0; i < 6; i++) {
        AL_LOG(AL_LOG_LEVEL_INFO, "GPIO Pin %d input data value is 0x%x", PS_KEY_1, AlGpio_Hal_ReadPinInput(GPIO,PS_KEY_1));
    }

    /* 3、Test intr */
    AL_INTR_AttrStrct AttrGpio = {.Priority = 0x20, .TrigMode = LEVEL_HIGH_TRIGGER};
    AlGpio_Hal_IntrPinCfg(GPIO, PS_KEY_1, GPIO_INTR_TYPE_EDGE_RISING, &AttrGpio);
    AlGpio_Hal_IntrPinCfg(GPIO, PS_KEY_2, GPIO_INTR_TYPE_EDGE_RISING, &AttrGpio);
    AlGpio_Hal_IntrPinCfg(GPIO, PL_KEY, GPIO_INTR_TYPE_EDGE_FALLING, &AttrGpio);
    AlSys_MDelay(5000);

    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    while(1);

    return AL_OK;
}


AL_S32 main(void) {
    *(AL_U32 *)(0xf8803028u) =0x4;    //GPIO MIO10
    *(AL_U32 *)(0xf880302cu) =0x4;    //GPIO MIO11
    AL_LOG(AL_LOG_LEVEL_INFO, "[TEST]AlGpio_Hal_Test start");
    AlGpio_Hal_Intr_Example();

    return AL_OK;
}
