/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_core.h"
#include "dr1x90_crg.h"
#include "dr1x90_pinctrl.h"
#include "soc_plat.h"

#if (defined ENABLE_PINMUX_MODE1 || defined ENABLE_PINMUX_MODE2)
#else
#define ENABLE_PINMUX_MODE1 1
#endif

extern void Enablepinmux1(void);
extern void Enablepinmux1_mode2(void);
extern int  demo_ddr_init();

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

    const pin_uart_t pin_uart0 = {27, 26};
    pinmux_config_uart0(&pin_uart0);
}

/*
 * generate by FD
*/
int Soc_PlatGetDeviceVersion()
{
    return AL_OK;
}


int Soc_PlatInit()
{
	demo_ddr_init();
    return AL_OK;
}
