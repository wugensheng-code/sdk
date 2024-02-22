/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_IPC_HW_H_
#define __AL_IPC_HW_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files ********************************/
#include "al_core.h"

/************************** Constant Definitions ****************************/

/**************************** Type Definitions ******************************/

/***************** Macros (Inline Functions) Definitions ********************/
#define IPC_MAILBOX_LOCK_OFFSET (0x000)
#define IPC_MAILBOX_NUM         (16)
#define IPC_INTR_OFFSET         (0x400)
#define IPC_INTR_NUM            (12)
#define IPC_SPINLOCK_OFFSET     (0x800)
#define IPC_SPINLOCK_NUM        (4)
#define IPC_INTR_APU2RPU_REQ    (0x420)
#define IPC_INTR_APU2RPU_ACK    (0x424)
#define IPC_INTR_RPU2APU_REQ    (0x428)
#define IPC_INTR_RPU2APU_ACK    (0x42C)
/**************************** Type Definitions ******************************/

typedef enum
{
    AL_IPC_APU2RPU_REQ                 = 0,
    AL_IPC_APU2RPU_ACK                 = 1,
    AL_IPC_RPU2APU_REQ                 = 2,
    AL_IPC_RPU2APU_ACK                 = 3
} AL_IPC_IntrEnum;

typedef struct
{
    AL_IPC_IntrEnum         Type;
    AL_U32                  IntrNum;
} AL_IPCINTR_HwConfigStruct;

#ifdef __cplusplus
}
#endif

#endif
