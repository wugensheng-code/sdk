/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */



/***************************** Include Files *********************************/
#include "al_gpio_hal.h"

/************************** Variable Definitions *****************************/
static AL_GPIO_DevStruct AL_GPIO_DevInstance[AL_GPIO_NUM_INSTANCE];

/**
 * @brief  This function actions when interrupt done.
 * @param  GpioEvent
 * @param  CallbackRef
 * @return AL_VOID
 */
static AL_VOID AlGpio_Hal_DefEventCallBack(AL_GPIO_EventStruct GpioEvent, AL_VOID *CallbackRef)
{
    if(GpioEvent.Events == AL_GPIO_Event)
    {
        AL_LOG(AL_LOG_LEVEL_INFO, "Get AL_GPIO_INTR \r\n");
    }
}


/**************************** Function Prototypes ******************************/
/**
 * @brief  This function initializes a AL_GPIO driver.
 * @param  Handle is a pointer to the AL_GPIO instance.
 * @param  DevId
 * @return
 *         - AL_OK for function success
 *         - Other for function failuregit
 */
AL_S32 AlGpio_Hal_Init(AL_GPIO_HalStruct *Handle, AL_U32 DevId, AL_GPIO_EventCallBack CallBack)
{
    AL_S32 Ret = AL_OK;
    AL_GPIO_HwConfigStruct *HwConfig = AL_NULL;

    if (Handle == AL_NULL)
    {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }

    /* 1. look up hardware config */
    HwConfig = AlGpio_Dev_LookupConfig(DevId);
    if(HwConfig != AL_NULL){
        Handle->Dev = &AL_GPIO_DevInstance[DevId];
    } else {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }

    /* 2. Init */
    Ret = AlGpio_Dev_Init(Handle->Dev, HwConfig);
    if(Ret != AL_OK) {
        return Ret;
    }

    /* 3. register intr */
    if(CallBack) {
        Ret = AlGpio_Dev_RegisterEventCallBack(Handle->Dev, CallBack, (void *)Handle);
    } else {
        Ret = AlGpio_Dev_RegisterEventCallBack(Handle->Dev, AlGpio_Hal_DefEventCallBack, (void *)Handle);
    }
    if(Ret != AL_OK) {
        return Ret;
    }

    (AL_VOID)AlIntr_RegHandler(Handle->Dev->HwConfig.IntrId, AL_NULL, AlGpio_Dev_IntrHandler, Handle->Dev);

    return Ret;
}

/**
 * @brief  Write data to the specified pin.
 * @param  Handle is a pointer to the AL_GPIO instance.
 * @param  Pin is the pin number to which the Data is to be written.
 * @param  Data is the data to be written to the specified pin (0 or 1).
 * @return AL_S32
 */
AL_S32 AlGpio_Hal_WritePin(AL_GPIO_HalStruct *Handle, AL_U32 Pin, AL_U32 Data)
{
    if (Handle == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }

    AlGpio_Dev_SetDirectionPin(Handle->Dev, Pin, GPIO_OUTPUT);
    AlGpio_Dev_WritePin(Handle->Dev, Pin, Data);

    return AL_OK;
}

/**
 * @brief  Read Data from the specified pin in DR register.
 * @param  Handle is a pointer to the AL_GPIO instance.
 * @param  Pin is the pin number to which the Data is to be written.
 * @return AL_S32
 */
AL_S32 AlGpio_Hal_OutputReadPin(AL_GPIO_HalStruct *Handle, AL_U32 Pin)
{
    if (Handle == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }

    return AlGpio_Dev_OutputReadPin(Handle->Dev, Pin);
}

/**
 * @brief  Read Data from the specified pin in EXT register.
 * @param  Handle is a pointer to the AL_GPIO instance.
 * @param  Pin is the pin number to which the Data is to be written.
 * @return AL_S32
 */
AL_S32 AlGpio_Hal_InputReadPin(AL_GPIO_HalStruct *Handle, AL_U8 Pin)
{
    if (Handle == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }
    AlGpio_Dev_SetDirectionPin(Handle->Dev, Pin, GPIO_INPUT);
    return AlGpio_Dev_InputReadPin(Handle->Dev, Pin);
}

/**
 * @brief  This function configure the interrupt, including direction, type and enable register.
 * @param  Handle
 * @param  Pin
 * @param  IntrType
 * @return AL_S32
 */
AL_S32 AlGpio_Hal_IntrCfg(AL_GPIO_HalStruct *Handle, AL_U32 Pin, AL_GPIO_IntrEnum IntrType)
{
    if (Handle == AL_NULL) {
        return AL_GPIO_ERR_ILLEGAL_PARAM;
    }

    AlGpio_Dev_SetDirectionPin(Handle->Dev, Pin, GPIO_INPUT);
    AlGpio_Dev_IntrSetTypePin(Handle->Dev, Pin, IntrType);
    AlGpio_Dev_IntrEnablePin(Handle->Dev, Pin);

    return AL_OK;
}
