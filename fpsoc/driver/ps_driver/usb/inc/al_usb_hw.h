/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


#ifndef __AL_USB_HW_H_
#define __AL_USB_HW_H_

#ifdef __cplusplus
extern "C" {
#endif

/********************************** Include Files ********************************/
#include "al_core.h"

/******************************** Exported Typedef *******************************/
typedef struct {
    AL_U32                  DeviceId;
    AL_U32                  BaseAddress;
    AL_U32                  IntrId;
} AL_USB_HwConfigStruct;

#ifdef __cplusplus
}
#endif

#endif
