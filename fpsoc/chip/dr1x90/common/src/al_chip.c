/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>

#include "al_log.h"
#include "al_printf.h"
#include "al_core.h"
#include "al_chip.h"


AL_U64 SystemCoreClock = SYSTEM_CLOCK;  /* System Clock Frequency (Core Clock) */

void components_init(void)
{
    AL_S32 Ret;

    AlLog_Init();

#ifdef AL_PRINT_ASYNC
    AlPrint_Init();
#endif

#ifdef ENABLE_MMU
    /* defined in the link script */
    extern AL_U32 _no_cache_section_start, _no_cache_section_end;
    if (&(_no_cache_section_start) != &(_no_cache_section_end)) {
        Ret = AlCache_SetMemoryAttr((AL_UINTPTR) &(_no_cache_section_start), (AL_UINTPTR) &(_no_cache_section_end), Al_MEM_DMA);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlCache_SetMemoryAttr failed ret = 0x%x\r\n", Ret);
        }
    }
#endif
}
