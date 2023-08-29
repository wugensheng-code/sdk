/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef AL_CIPHER_HW_H
#define AL_CIPHER_HW_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files ********************************/
#include "al_core.h"

/************************** Constant Definitions ****************************/

/**************************** Type Definitions ******************************/

/***************** Macros (Inline Functions) Definitions ********************/
#define CIPHER_MSG_OFFSET   0x00UL
#define CIPHER_ACK_OFFSET   0x40UL

/**************************** Type Definitions ******************************/
typedef struct {
    AL_U32      DeviceId;
    AL_U32      BaseAddress;
    AL_U32      ReqIntrAddr;
    AL_U32      AckIntrId;
} AL_CIPHER_HwConfigStruct;

#ifdef __cplusplus
}
#endif

#endif
