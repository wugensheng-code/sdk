/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


#ifndef __AL_USB_HAL_H_
#define __AL_USB_HAL_H_

#ifdef __cplusplus
extern "C" {
#endif

/********************************* Include  Files ********************************/
#include "al_usb_ll.h"

/********************************* Exported Constant *****************************/
#define AL_USB_ERR_ILLEGAL_PARAM    (AL_DEF_ERR(AL_USB, AL_LOG_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM))

/**************************** Exported Typedef ******************************/
typedef struct
{
    AL_USB_HwConfigStruct   HwConfig;
} AL_USB_HalStruct;

/************************** Exported Function *****************************/
AL_S32 AlUsb_Hal_Init(AL_USB_HalStruct **Handle, AL_U32 DevId, AL_VOID *CallBack);

#ifdef __cplusplus
}
#endif

#endif
