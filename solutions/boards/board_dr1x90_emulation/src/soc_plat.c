/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_core.h"
#include "al9000_crg.h"

extern void Enablepinmux1(void);
extern void Enablepinmux1_mode2(void);

#if (defined ENABLE_PINMUX_MODE1 || defined ENABLE_PINMUX_MODE2)
#else
#define ENABLE_PINMUX_MODE1 1
#endif


__attribute__((__used__)) void board_init()
{
#ifdef AL9000_CLK_CONFIG
    clk_simple_config();
#endif

#if ENABLE_PINMUX_MODE1 == 1
    Enablepinmux1();
#endif

#if ENABLE_PINMUX_MODE2 == 1
    Enablepinmux1_mode2();
#endif
}

/*
 * generate by FD
*/
int Soc_PlatGetDeviceVersion ()
{
    return AL_OK;
}


int Soc_PlatInit()
{
    return AL_OK;
}
