/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_core.h"
#include "dr1x90_crg.h"
#include "al_reg_io.h"


extern void Enablepinmux1(void);
extern void Enablepinmux1_mode2(void);

#if (defined ENABLE_PINMUX_MODE1 || defined ENABLE_PINMUX_MODE2)
#else
#define ENABLE_PINMUX_MODE1 1
#endif


__attribute__((__used__)) void board_init()
{
#ifdef AL_CLK_CONFIG
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

int Soc_PsPlInit()
{
    /// open pl-ps bus connections, hp and gpm bus
	AL_REG32_SET_BITS(0xF8801078UL, 0, 2, 3);
	AL_REG32_SET_BITS(0xF8801078UL, 4, 2, 3);

	/// open pl-ps bus connections, fahb and gps bus
	AL_REG32_SET_BITS(0xF8800080UL, 0, 2, 0);

	/// open apu acp bus connections
	AL_REG32_SET_BITS(0xF8801070UL, 8, 1, 1);
	AL_REG32_SET_BITS(0xF8802004UL, 0, 1, 0);
    return 0;
}