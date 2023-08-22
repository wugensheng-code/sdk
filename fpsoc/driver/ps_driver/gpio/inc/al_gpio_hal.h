/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


#ifndef AL_AL_GPIO_H
#define AL_AL_GPIO_H


#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include  Files ********************************/
// #include "al_errno.h"
// #include "al_core.h"
#include "al_gpio_dev.h"

/**************************** Exported Typedef ******************************/
typedef struct {
    AL_GPIO_DevStruct            *Dev;
} AL_GPIO_HalStruct;

/************************** Exported Function *****************************/
AL_S32 AlGpio_Hal_Init(AL_GPIO_HalStruct *Handle, AL_U32 DevId, AL_GPIO_EventCallBack CallBack);
AL_S32 AlGpio_Hal_WritePin(AL_GPIO_HalStruct *Handle, AL_U32 Pin, AL_U32 Data);
AL_S32 AlGpio_Hal_OutputReadPin(AL_GPIO_HalStruct *Handle, AL_U32 Pin);
AL_S32 AlGpio_Hal_InputReadPin(AL_GPIO_HalStruct *Handle, AL_U8 Pin);
AL_S32 AlGpio_Hal_IntrCfg(AL_GPIO_HalStruct *Handle, AL_U32 Pin, AL_GPIO_IntrEnum IntrType);

#ifdef __cplusplus
}
#endif

#endif