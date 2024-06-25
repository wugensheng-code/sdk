/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_type.h"
#include "al_chip.h"
#include "al_intr.h"
#include "al_systimer.h"
#include "al_ipc_ll.h"

extern void AlGic_Init(void);

AL_VOID AlChip_Init(AL_VOID) __attribute__((alias ("AlChip_Dr1m90Init")));

AL_VOID AlChip_Dr1m90Init(AL_VOID)
{
#ifndef SUPPORT_NONSECURE
	AlGic_Init();
	AlSys_StartTimer();
#endif

#ifndef ARM_CORE_SLAVE
	// AlIpc_ll_SpinLockInit();
#endif
}
