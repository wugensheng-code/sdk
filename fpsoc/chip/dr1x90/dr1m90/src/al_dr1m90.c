/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_type.h"
#include "al_chip.h"
#include "al_intr.h"
#include "al_systimer.h"

extern void AlGic_Init(void);

AL_VOID AlChip_Dr1m90Init(AL_VOID)
{
#ifndef SUPPORT_NONSECURE
	AlGic_Init();
	AlSys_StartTimer();
#endif
}