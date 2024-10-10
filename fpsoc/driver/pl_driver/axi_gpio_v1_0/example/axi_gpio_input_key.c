/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_axi_gpio_hal.h"


static AL_VOID ReleasePorts(AL_VOID)
{
    AL_REG32_SET_BIT(0xF8800080, 1, 0);
    AL_REG32_SET_BIT(0xF8801078, 0, 1);
    AL_REG32_SET_BIT(0xF8801078, 4, 1);
}

AL_VOID Callback(AlAxiGpio_EventStruct Event, AL_VOID *CallbackRef)
{
    AlAxiGpio_Hal_Struct *Handle = (AlAxiGpio_Hal_Struct *)CallbackRef;
    AL_U32 ReadInChannel1 = 0;
    AL_U32 ReadInChannel2 = 0;

    if(Event.Channel == BIT(AL_AXI_GPIO_CHANNEL1))
    {
        ReadInChannel1 = AlAxiGpio_Hal_ReadPin(Handle, AL_AXI_GPIO_CHANNEL1, 0);
        AL_LOG(AL_LOG_LEVEL_INFO, "Channel = 0x%x, IntrStatus = 0x%x, ReadIn = 0x%x", Event.Channel, Event.EventData, ReadInChannel1);
    } else if(Event.Channel == BIT(AL_AXI_GPIO_CHANNEL2)) {
        ReadInChannel2 = AlAxiGpio_Hal_ReadPin(Handle, AL_AXI_GPIO_CHANNEL2, 0);
        AL_LOG(AL_LOG_LEVEL_INFO, "Channel = 0x%x, IntrStatus = 0x%x, ReadIn = 0x%x", Event.Channel, Event.EventData, ReadInChannel2);
    } else if(Event.Channel == BIT(AL_AXI_GPIO_CHANNEL1) | BIT(AL_AXI_GPIO_CHANNEL2))
    {
        ReadInChannel1 = AlAxiGpio_Hal_ReadPin(Handle, AL_AXI_GPIO_CHANNEL1, 0);
        ReadInChannel2 = AlAxiGpio_Hal_ReadPin(Handle, AL_AXI_GPIO_CHANNEL2, 0);
        AL_LOG(AL_LOG_LEVEL_INFO, "Channel = 0x%x, IntrStatus = 0x%x, ReadInChannel1 = 0x%x, ReadInChannel2 = 0x%x", Event.Channel, Event.EventData, ReadInChannel1, ReadInChannel2);
    }
}

AL_S32 main()
{
    AL_S32 ret = AL_OK;

    ReleasePorts();

    AL_LOG(AL_LOG_LEVEL_INFO, "Axi gpio input key interrupt test!");

    /* Instantiate a axi gpio and init it */
    AlAxiGpio_Hal_Struct *Gpio0;
    AlAxiGpio_Hal_Struct *Gpio1;

    ret = AlAxiGpio_Hal_Init(&Gpio0, AXI_GPIO0_DEVICE_ID, Callback);
    if (ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Axi gpio0 init failed!");
        return ret;
    }

    ret = AlAxiGpio_Hal_Init(&Gpio1, AXI_GPIO1_DEVICE_ID, Callback);
    if (ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Axi gpio1 init failed!");
        return ret;
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "Read Channel in = 0x%x", AlAxiGpio_Hal_ReadChannel(Gpio0, AL_AXI_GPIO_CHANNEL2));


    /* Enable axi gpio channel1 interrupt */
    ret = AlAxiGpio_Hal_IntInit(Gpio0, AL_AXI_GPIO_CHANNEL1);
    if(ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_INFO, "Axi gpio0 interrupt init failed, ret = 0x%x\r\n!", ret);
        return ret;
    }

    /* Enable axi gpio channel2 interrupt */
    ret = AlAxiGpio_Hal_IntInit(Gpio1, AL_AXI_GPIO_CHANNEL2);
    if(ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_INFO, "Axi gpio1 interrupt init failed, ret = 0x%x\r\n!", ret);
        return ret;
    }

    AlSys_MDelay(5000);
    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    while (1)
    {

    }

    return 0;
}