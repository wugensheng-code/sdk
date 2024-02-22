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
    for (AL_U32 i = 0; i < IPC_SPINLOCK_NUM; i++) {
        AL_REG32_WRITE(AL_IPC_BASE_ADDR + IPC_SPINLOCK_OFFSET + (i << 2), 0x1);
    }
}

static inline AL_VOID AlIpc_ll_SpinLockInitThis(AL_U32 Number)
{
    AL_REG32_WRITE(AL_IPC_BASE_ADDR + IPC_SPINLOCK_OFFSET + (Number << 2), 0x1);
}

static inline AL_U32 AlIpc_ll_SpinLockTryTake(AL_U32 Number)
{
    return AL_REG32_READ(AL_IPC_BASE_ADDR + IPC_SPINLOCK_OFFSET + (Number << 2));
}

static inline AL_VOID AlIpc_ll_SpinLockRelease(AL_U32 Number)
{
    AL_REG32_WRITE(AL_IPC_BASE_ADDR + IPC_SPINLOCK_OFFSET + (Number << 2), 0x1);
}

static inline AL_U32 AlIpc_ll_MboxRead(AL_U32 Number)
{
    return AL_REG32_READ(AL_IPC_BASE_ADDR + IPC_MAILBOX_LOCK_OFFSET + Number * 4);
}

static inline AL_VOID AlIpc_ll_MboxWrite(AL_U32 Number, AL_U32 Value)
{
    AL_REG32_WRITE(AL_IPC_BASE_ADDR + IPC_MAILBOX_LOCK_OFFSET + Number * 4, Value);
}

static inline AL_VOID AlIpc_ll_SetInter(AL_IPC_IntrEnum Type, AL_FUNCTION State)
{
    AL_REG32_WRITE(AL_IPC_BASE_ADDR + IPC_INTR_APU2RPU_REQ + Type * 4, State);
}

#ifdef __cplusplus
}
#endif

#endif
