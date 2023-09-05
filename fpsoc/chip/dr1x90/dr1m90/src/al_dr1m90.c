/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_type.h"
#include "al_chip.h"
#include "al_intr.h"
#include "al_systimer.h"

AL_VOID AlChip_Dr1m90Init(AL_VOID)
{
	#if (defined SUPPORT_NONSECURE || defined SWITCH_TO_EL0_FROM_EL3)
		//if SUPPORT_NONSECURE, gic-v3 init in EL3
		//if SWITCH_TO_EL0_FROM_EL3, boot to EL0, only to test
	#else
		AlGic_Init();
	#endif

    #ifndef SWITCH_TO_EL0_FROM_EL3
        AlSys_StartTimer();
    #endif
}