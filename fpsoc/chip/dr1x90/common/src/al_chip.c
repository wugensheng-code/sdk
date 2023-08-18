/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>

#include "al_log.h"
#include "al_core.h"
#include "al_chip.h"


AL_U64 SystemCoreClock = SYSTEM_CLOCK;  /* System Clock Frequency (Core Clock) */

void _premain_init(void)
{
    AlChip_Init();
   
}

void components_init(void)
{
    AlLog_Init();
}
