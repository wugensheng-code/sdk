/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_core.h"
#include "al_rv64_core.h"
#include "al_chip.h"
#include "al_rv64_systimer.h"
#include "al_ipc_ll.h"

extern void Exception_Init(void);
extern void ECLIC_Init(void);

AL_VOID AlChip_Init(AL_VOID) __attribute__((alias ("AlChip_Dr1V90Init")));

AL_VOID AlChip_Dr1V90Init(AL_VOID)
{
    Exception_Init();
    ECLIC_Init();

//#ifndef RTOS_RTTHREAD
    // SysTimer_clk_sel();
//#endif

    AlIpc_ll_SpinLockInit();
}
