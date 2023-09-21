/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_IPC_LL_H_
#define __AL_IPC_LL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "al_core.h"
#include "al_ipc_hw.h"

static inline AL_VOID AlIpc_ll_SpinLockInit(AL_VOID)
{
    for (AL_U32 i = 0; i < IPC_SPIN_LOCK_NUM; i++) {
        AL_REG32_WRITE(AL_IPC_BASE_ADDR + IPC_SPIN_LOCK_OFFSET + (i << 2), 0x1);
    }
}

#ifdef __cplusplus
}
#endif

#endif
