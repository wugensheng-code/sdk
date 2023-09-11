/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_TTC_HW_H_
#define __AL_TTC_HW_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files ********************************/
#include "al_core.h"

/************************** Constant Definitions ****************************/

/**************************** Type Definitions ******************************/

/***************** Macros (Inline Functions) Definitions ********************/


///////////////////////////////////////////////////////
// Register: EVENT_REG_1
// pclk cycle count for event
///////////////////////////////////////////////////////

#define TTC__EVENT_REG_1__OFFSET 0x0ULL
#define TTC__EVENT_REG_1__NUM  0x1

#define TTC__EVENT_REG_1__EVENT__SHIFT    0
#define TTC__EVENT_REG_1__RESERVED_31_16__SHIFT    16

#define TTC__EVENT_REG_1__EVENT__SIZE    16
#define TTC__EVENT_REG_1__RESERVED_31_16__SIZE    16

#define TTC__EVENT_REG_1__EVENT__MASK    0x0000ffff
#define TTC__EVENT_REG_1__RESERVED_31_16__MASK    0xffff0000

#define TTC__EVENT_REG_1__EVENT__POR_VALUE    0x0
#define TTC__EVENT_REG_1__RESERVED_31_16__POR_VALUE    0x0

///////////////////////////////////////////////////////
// Register: EVENT_REG_2
// pclk cycle count for event
///////////////////////////////////////////////////////

#define TTC__EVENT_REG_2__OFFSET   0x4ULL
#define TTC__EVENT_REG_2__NUM  0x1

#define TTC__EVENT_REG_2__EVENT__SHIFT    0
#define TTC__EVENT_REG_2__RESERVED_31_16__SHIFT    16

#define TTC__EVENT_REG_2__EVENT__SIZE    16
#define TTC__EVENT_REG_2__RESERVED_31_16__SIZE    16

#define TTC__EVENT_REG_2__EVENT__MASK    0x0000ffff
#define TTC__EVENT_REG_2__RESERVED_31_16__MASK    0xffff0000

#define TTC__EVENT_REG_2__EVENT__POR_VALUE    0x0
#define TTC__EVENT_REG_2__RESERVED_31_16__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: EVENT_REG_3
// pclk cycle count for event
///////////////////////////////////////////////////////

#define TTC__EVENT_REG_3__OFFSET 0x8ULL
#define TTC__EVENT_REG_3__NUM  0x1

#define TTC__EVENT_REG_3__EVENT__SHIFT    0
#define TTC__EVENT_REG_3__RESERVED_31_16__SHIFT    16

#define TTC__EVENT_REG_3__EVENT__SIZE    16
#define TTC__EVENT_REG_3__RESERVED_31_16__SIZE    16

#define TTC__EVENT_REG_3__EVENT__MASK    0x0000ffff
#define TTC__EVENT_REG_3__RESERVED_31_16__MASK    0xffff0000

#define TTC__EVENT_REG_3__EVENT__POR_VALUE    0x0
#define TTC__EVENT_REG_3__RESERVED_31_16__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: EVENT_CTRL_1
// Enable, pulse and overflow
///////////////////////////////////////////////////////

#define TTC__EVENT_CTRL_1__OFFSET 0xCULL
#define TTC__EVENT_CTRL_1__NUM  0x1

#define TTC__EVENT_CTRL_1__E_EN__SHIFT    0
#define TTC__EVENT_CTRL_1__E_LO__SHIFT    1
#define TTC__EVENT_CTRL_1__E_OV__SHIFT    2
#define TTC__EVENT_CTRL_1__RESERVED_31_3__SHIFT    3

#define TTC__EVENT_CTRL_1__E_EN__SIZE    1
#define TTC__EVENT_CTRL_1__E_LO__SIZE    1
#define TTC__EVENT_CTRL_1__E_OV__SIZE    1
#define TTC__EVENT_CTRL_1__RESERVED_31_3__SIZE    29

#define TTC__EVENT_CTRL_1__E_EN__MASK    0x00000001
#define TTC__EVENT_CTRL_1__E_LO__MASK    0x00000002
#define TTC__EVENT_CTRL_1__E_OV__MASK    0x00000004
#define TTC__EVENT_CTRL_1__RESERVED_31_3__MASK    0xfffffff8

#define TTC__EVENT_CTRL_1__E_EN__POR_VALUE    0x0
#define TTC__EVENT_CTRL_1__E_LO__POR_VALUE    0x0
#define TTC__EVENT_CTRL_1__E_OV__POR_VALUE    0x0
#define TTC__EVENT_CTRL_1__RESERVED_31_3__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: EVENT_CTRL_2
// Enable, pulse and overflow
///////////////////////////////////////////////////////

#define TTC__EVENT_CTRL_2__OFFSET 0x10ULL
#define TTC__EVENT_CTRL_2__NUM  0x1

#define TTC__EVENT_CTRL_2__E_EN__SHIFT    0
#define TTC__EVENT_CTRL_2__E_LO__SHIFT    1
#define TTC__EVENT_CTRL_2__E_OV__SHIFT    2
#define TTC__EVENT_CTRL_2__RESERVED_31_3__SHIFT    3

#define TTC__EVENT_CTRL_2__E_EN__SIZE    1
#define TTC__EVENT_CTRL_2__E_LO__SIZE    1
#define TTC__EVENT_CTRL_2__E_OV__SIZE    1
#define TTC__EVENT_CTRL_2__RESERVED_31_3__SIZE    29

#define TTC__EVENT_CTRL_2__E_EN__MASK    0x00000001
#define TTC__EVENT_CTRL_2__E_LO__MASK    0x00000002
#define TTC__EVENT_CTRL_2__E_OV__MASK    0x00000004
#define TTC__EVENT_CTRL_2__RESERVED_31_3__MASK    0xfffffff8

#define TTC__EVENT_CTRL_2__E_EN__POR_VALUE    0x0
#define TTC__EVENT_CTRL_2__E_LO__POR_VALUE    0x0
#define TTC__EVENT_CTRL_2__E_OV__POR_VALUE    0x0
#define TTC__EVENT_CTRL_2__RESERVED_31_3__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: EVENT_CTRL_3
// Enable, pulse and overflow
///////////////////////////////////////////////////////

#define TTC__EVENT_CTRL_3__OFFSET 0x14ULL
#define TTC__EVENT_CTRL_3__NUM  0x1

#define TTC__EVENT_CTRL_3__E_EN__SHIFT    0
#define TTC__EVENT_CTRL_3__E_LO__SHIFT    1
#define TTC__EVENT_CTRL_3__E_OV__SHIFT    2
#define TTC__EVENT_CTRL_3__RESERVED_31_3__SHIFT    3

#define TTC__EVENT_CTRL_3__E_EN__SIZE    1
#define TTC__EVENT_CTRL_3__E_LO__SIZE    1
#define TTC__EVENT_CTRL_3__E_OV__SIZE    1
#define TTC__EVENT_CTRL_3__RESERVED_31_3__SIZE    29

#define TTC__EVENT_CTRL_3__E_EN__MASK    0x00000001
#define TTC__EVENT_CTRL_3__E_LO__MASK    0x00000002
#define TTC__EVENT_CTRL_3__E_OV__MASK    0x00000004
#define TTC__EVENT_CTRL_3__RESERVED_31_3__MASK    0xfffffff8

#define TTC__EVENT_CTRL_3__E_EN__POR_VALUE    0x0
#define TTC__EVENT_CTRL_3__E_LO__POR_VALUE    0x0
#define TTC__EVENT_CTRL_3__E_OV__POR_VALUE    0x0
#define TTC__EVENT_CTRL_3__RESERVED_31_3__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: CNTR_CTRL_REG_1
// Operational mode and reset
///////////////////////////////////////////////////////

#define TTC__CNTR_CTRL_REG_1__ADDR 0x18ULL
#define TTC__CNTR_CTRL_REG_1__NUM  0x1

#define TTC__CNTR_CTRL_REG_1__DIS__SHIFT    0
#define TTC__CNTR_CTRL_REG_1__INT__SHIFT    1
#define TTC__CNTR_CTRL_REG_1__DEC__SHIFT    2
#define TTC__CNTR_CTRL_REG_1__MATCH__SHIFT    3
#define TTC__CNTR_CTRL_REG_1__RST__SHIFT    4
#define TTC__CNTR_CTRL_REG_1__WAVE_EN__SHIFT    5
#define TTC__CNTR_CTRL_REG_1__WAVE_POL__SHIFT    6
#define TTC__CNTR_CTRL_REG_1__RESERVED_31_7__SHIFT    7

#define TTC__CNTR_CTRL_REG_1__DIS__SIZE    1
#define TTC__CNTR_CTRL_REG_1__INT__SIZE    1
#define TTC__CNTR_CTRL_REG_1__DEC__SIZE    1
#define TTC__CNTR_CTRL_REG_1__MATCH__SIZE    1
#define TTC__CNTR_CTRL_REG_1__RST__SIZE    1
#define TTC__CNTR_CTRL_REG_1__WAVE_EN__SIZE    1
#define TTC__CNTR_CTRL_REG_1__WAVE_POL__SIZE    1
#define TTC__CNTR_CTRL_REG_1__RESERVED_31_7__SIZE    25

#define TTC__CNTR_CTRL_REG_1__DIS__MASK    0x00000001
#define TTC__CNTR_CTRL_REG_1__INT__MASK    0x00000002
#define TTC__CNTR_CTRL_REG_1__DEC__MASK    0x00000004
#define TTC__CNTR_CTRL_REG_1__MATCH__MASK    0x00000008
#define TTC__CNTR_CTRL_REG_1__RST__MASK    0x00000010
#define TTC__CNTR_CTRL_REG_1__WAVE_EN__MASK    0x00000020
#define TTC__CNTR_CTRL_REG_1__WAVE_POL__MASK    0x00000040
#define TTC__CNTR_CTRL_REG_1__RESERVED_31_7__MASK    0xffffff80

#define TTC__CNTR_CTRL_REG_1__DIS__POR_VALUE    0x1
#define TTC__CNTR_CTRL_REG_1__INT__POR_VALUE    0x0
#define TTC__CNTR_CTRL_REG_1__DEC__POR_VALUE    0x0
#define TTC__CNTR_CTRL_REG_1__MATCH__POR_VALUE    0x0
#define TTC__CNTR_CTRL_REG_1__RST__POR_VALUE    0x0
#define TTC__CNTR_CTRL_REG_1__WAVE_EN__POR_VALUE    0x1
#define TTC__CNTR_CTRL_REG_1__WAVE_POL__POR_VALUE    0x0
#define TTC__CNTR_CTRL_REG_1__RESERVED_31_7__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: CNTR_CTRL_REG_2
// Operational mode and reset
///////////////////////////////////////////////////////

#define TTC__CNTR_CTRL_REG_2__OFFSET 0x1CULL
#define TTC__CNTR_CTRL_REG_2__NUM  0x1

#define TTC__CNTR_CTRL_REG_2__DIS__SHIFT    0
#define TTC__CNTR_CTRL_REG_2__INT__SHIFT    1
#define TTC__CNTR_CTRL_REG_2__DEC__SHIFT    2
#define TTC__CNTR_CTRL_REG_2__MATCH__SHIFT    3
#define TTC__CNTR_CTRL_REG_2__RST__SHIFT    4
#define TTC__CNTR_CTRL_REG_2__WAVE_EN__SHIFT    5
#define TTC__CNTR_CTRL_REG_2__WAVE_POL__SHIFT    6
#define TTC__CNTR_CTRL_REG_2__RESERVED_31_7__SHIFT    7

#define TTC__CNTR_CTRL_REG_2__DIS__SIZE    1
#define TTC__CNTR_CTRL_REG_2__INT__SIZE    1
#define TTC__CNTR_CTRL_REG_2__DEC__SIZE    1
#define TTC__CNTR_CTRL_REG_2__MATCH__SIZE    1
#define TTC__CNTR_CTRL_REG_2__RST__SIZE    1
#define TTC__CNTR_CTRL_REG_2__WAVE_EN__SIZE    1
#define TTC__CNTR_CTRL_REG_2__WAVE_POL__SIZE    1
#define TTC__CNTR_CTRL_REG_2__RESERVED_31_7__SIZE    25

#define TTC__CNTR_CTRL_REG_2__DIS__MASK    0x00000001
#define TTC__CNTR_CTRL_REG_2__INT__MASK    0x00000002
#define TTC__CNTR_CTRL_REG_2__DEC__MASK    0x00000004
#define TTC__CNTR_CTRL_REG_2__MATCH__MASK    0x00000008
#define TTC__CNTR_CTRL_REG_2__RST__MASK    0x00000010
#define TTC__CNTR_CTRL_REG_2__WAVE_EN__MASK    0x00000020
#define TTC__CNTR_CTRL_REG_2__WAVE_POL__MASK    0x00000040
#define TTC__CNTR_CTRL_REG_2__RESERVED_31_7__MASK    0xffffff80

#define TTC__CNTR_CTRL_REG_2__DIS__POR_VALUE    0x1
#define TTC__CNTR_CTRL_REG_2__INT__POR_VALUE    0x0
#define TTC__CNTR_CTRL_REG_2__DEC__POR_VALUE    0x0
#define TTC__CNTR_CTRL_REG_2__MATCH__POR_VALUE    0x0
#define TTC__CNTR_CTRL_REG_2__RST__POR_VALUE    0x0
#define TTC__CNTR_CTRL_REG_2__WAVE_EN__POR_VALUE    0x1
#define TTC__CNTR_CTRL_REG_2__WAVE_POL__POR_VALUE    0x0
#define TTC__CNTR_CTRL_REG_2__RESERVED_31_7__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: CNTR_CTRL_REG_3
// Operational mode and reset
///////////////////////////////////////////////////////

#define TTC__CNTR_CTRL_REG_3__ADDR 0x20ULL
#define TTC__CNTR_CTRL_REG_3__NUM  0x1

#define TTC__CNTR_CTRL_REG_3__DIS__SHIFT    0
#define TTC__CNTR_CTRL_REG_3__INT__SHIFT    1
#define TTC__CNTR_CTRL_REG_3__DEC__SHIFT    2
#define TTC__CNTR_CTRL_REG_3__MATCH__SHIFT    3
#define TTC__CNTR_CTRL_REG_3__RST__SHIFT    4
#define TTC__CNTR_CTRL_REG_3__WAVE_EN__SHIFT    5
#define TTC__CNTR_CTRL_REG_3__WAVE_POL__SHIFT    6
#define TTC__CNTR_CTRL_REG_3__RESERVED_31_7__SHIFT    7

#define TTC__CNTR_CTRL_REG_3__DIS__SIZE    1
#define TTC__CNTR_CTRL_REG_3__INT__SIZE    1
#define TTC__CNTR_CTRL_REG_3__DEC__SIZE    1
#define TTC__CNTR_CTRL_REG_3__MATCH__SIZE    1
#define TTC__CNTR_CTRL_REG_3__RST__SIZE    1
#define TTC__CNTR_CTRL_REG_3__WAVE_EN__SIZE    1
#define TTC__CNTR_CTRL_REG_3__WAVE_POL__SIZE    1
#define TTC__CNTR_CTRL_REG_3__RESERVED_31_7__SIZE    25

#define TTC__CNTR_CTRL_REG_3__DIS__MASK    0x00000001
#define TTC__CNTR_CTRL_REG_3__INT__MASK    0x00000002
#define TTC__CNTR_CTRL_REG_3__DEC__MASK    0x00000004
#define TTC__CNTR_CTRL_REG_3__MATCH__MASK    0x00000008
#define TTC__CNTR_CTRL_REG_3__RST__MASK    0x00000010
#define TTC__CNTR_CTRL_REG_3__WAVE_EN__MASK    0x00000020
#define TTC__CNTR_CTRL_REG_3__WAVE_POL__MASK    0x00000040
#define TTC__CNTR_CTRL_REG_3__RESERVED_31_7__MASK    0xffffff80

#define TTC__CNTR_CTRL_REG_3__DIS__POR_VALUE    0x1
#define TTC__CNTR_CTRL_REG_3__INT__POR_VALUE    0x0
#define TTC__CNTR_CTRL_REG_3__DEC__POR_VALUE    0x0
#define TTC__CNTR_CTRL_REG_3__MATCH__POR_VALUE    0x0
#define TTC__CNTR_CTRL_REG_3__RST__POR_VALUE    0x0
#define TTC__CNTR_CTRL_REG_3__WAVE_EN__POR_VALUE    0x1
#define TTC__CNTR_CTRL_REG_3__WAVE_POL__POR_VALUE    0x0
#define TTC__CNTR_CTRL_REG_3__RESERVED_31_7__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: CLK_CTRL_REG_1
// Clock control register
///////////////////////////////////////////////////////

#define TTC__CLK_CTRL_REG_1__OFFSET 0x24ULL
#define TTC__CLK_CTRL_REG_1__NUM  0x1

#define TTC__CLK_CTRL_REG_1__PS_EN__SHIFT    0
#define TTC__CLK_CTRL_REG_1__PS_V__SHIFT    1
#define TTC__CLK_CTRL_REG_1__C_SRC__SHIFT    5
#define TTC__CLK_CTRL_REG_1__EX_E__SHIFT    6
#define TTC__CLK_CTRL_REG_1__RESERVED_31_7__SHIFT    7

#define TTC__CLK_CTRL_REG_1__PS_EN__SIZE    1
#define TTC__CLK_CTRL_REG_1__PS_V__SIZE     4
#define TTC__CLK_CTRL_REG_1__C_SRC__SIZE    1
#define TTC__CLK_CTRL_REG_1__EX_E__SIZE     1
#define TTC__CLK_CTRL_REG_1__RESERVED_31_7__SIZE    25

#define TTC__CLK_CTRL_REG_1__PS_EN__MASK    0x00000001
#define TTC__CLK_CTRL_REG_1__PS_V__MASK    0x0000001e
#define TTC__CLK_CTRL_REG_1__C_SRC__MASK    0x00000020
#define TTC__CLK_CTRL_REG_1__EX_E__MASK    0x00000040
#define TTC__CLK_CTRL_REG_1__RESERVED_31_7__MASK    0xffffff80

#define TTC__CLK_CTRL_REG_1__PS_EN__POR_VALUE    0x0
#define TTC__CLK_CTRL_REG_1__PS_V__POR_VALUE    0x0
#define TTC__CLK_CTRL_REG_1__C_SRC__POR_VALUE    0x0
#define TTC__CLK_CTRL_REG_1__EX_E__POR_VALUE    0x0
#define TTC__CLK_CTRL_REG_1__RESERVED_31_7__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: CLK_CTRL_REG_2
// Clock control register
///////////////////////////////////////////////////////



#define TTC__CLK_CTRL_REG_2__OFFSET 0x28ULL
#define TTC__CLK_CTRL_REG_2__NUM  0x1

#define TTC__CLK_CTRL_REG_2__PS_EN__SHIFT    0
#define TTC__CLK_CTRL_REG_2__PS_V__SHIFT    1
#define TTC__CLK_CTRL_REG_2__C_SRC__SHIFT    5
#define TTC__CLK_CTRL_REG_2__EX_E__SHIFT    6
#define TTC__CLK_CTRL_REG_2__RESERVED_31_7__SHIFT    7

#define TTC__CLK_CTRL_REG_1__PS_EN__SIZE    1
#define TTC__CLK_CTRL_REG_1__PS_V__SIZE     4
#define TTC__CLK_CTRL_REG_1__C_SRC__SIZE    1
#define TTC__CLK_CTRL_REG_1__EX_E__SIZE     1
#define TTC__CLK_CTRL_REG_1__RESERVED_31_7__SIZE    25

#define TTC__CLK_CTRL_REG_2__PS_EN__MASK    0x00000001
#define TTC__CLK_CTRL_REG_2__PS_V__MASK    0x0000001e
#define TTC__CLK_CTRL_REG_2__C_SRC__MASK    0x00000020
#define TTC__CLK_CTRL_REG_2__EX_E__MASK    0x00000040
#define TTC__CLK_CTRL_REG_2__RESERVED_31_7__MASK    0xffffff80

#define TTC__CLK_CTRL_REG_2__PS_EN__POR_VALUE    0x0
#define TTC__CLK_CTRL_REG_2__PS_V__POR_VALUE    0x0
#define TTC__CLK_CTRL_REG_2__C_SRC__POR_VALUE    0x0
#define TTC__CLK_CTRL_REG_2__EX_E__POR_VALUE    0x0
#define TTC__CLK_CTRL_REG_2__RESERVED_31_7__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: CLK_CTRL_REG_3
// Clock control register
///////////////////////////////////////////////////////

#define TTC__CLK_CTRL_REG_3__ADDR 0x2CULL
#define TTC__CLK_CTRL_REG_3__NUM  0x1

#define TTC__CLK_CTRL_REG_3__PS_EN__SHIFT    0
#define TTC__CLK_CTRL_REG_3__PS_V__SHIFT    1
#define TTC__CLK_CTRL_REG_3__C_SRC__SHIFT    5
#define TTC__CLK_CTRL_REG_3__EX_E__SHIFT    6
#define TTC__CLK_CTRL_REG_3__RESERVED_31_7__SHIFT    7

#define TTC__CLK_CTRL_REG_3__PS_EN__SIZE    1
#define TTC__CLK_CTRL_REG_3__PS_V__SIZE     4
#define TTC__CLK_CTRL_REG_3__C_SRC__SIZE    1
#define TTC__CLK_CTRL_REG_3__EX_E__SIZE     1
#define TTC__CLK_CTRL_REG_3__RESERVED_31_7__SIZE    25

#define TTC__CLK_CTRL_REG_3__PS_EN__MASK    0x00000001
#define TTC__CLK_CTRL_REG_3__PS_V__MASK    0x0000001e
#define TTC__CLK_CTRL_REG_3__C_SRC__MASK    0x00000020
#define TTC__CLK_CTRL_REG_3__EX_E__MASK    0x00000040
#define TTC__CLK_CTRL_REG_3__RESERVED_31_7__MASK    0xffffff80

#define TTC__CLK_CTRL_REG_3__PS_EN__POR_VALUE    0x0
#define TTC__CLK_CTRL_REG_3__PS_V__POR_VALUE    0x0
#define TTC__CLK_CTRL_REG_3__C_SRC__POR_VALUE    0x0
#define TTC__CLK_CTRL_REG_3__EX_E__POR_VALUE    0x0
#define TTC__CLK_CTRL_REG_3__RESERVED_31_7__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: INTERVAL_REG_1
// Interval value
///////////////////////////////////////////////////////

#define TTC__INTERVAL_REG_1__OFFSET 0x30ULL
#define TTC__INTERVAL_REG_1__NUM  0x1

#define TTC__INTERVAL_REG_1__INTERVAL__SHIFT    0
#define TTC__INTERVAL_REG_1__RESERVED_31_16__SHIFT    16

#define TTC__INTERVAL_REG_1__INTERVAL__SIZE    16
#define TTC__INTERVAL_REG_1__RESERVED_31_16__SIZE    16

#define TTC__INTERVAL_REG_1__INTERVAL__MASK    0x0000ffff
#define TTC__INTERVAL_REG_1__RESERVED_31_16__MASK    0xffff0000

#define TTC__INTERVAL_REG_1__INTERVAL__POR_VALUE    0x0
#define TTC__INTERVAL_REG_1__RESERVED_31_16__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: INTERVAL_REG_2
// Interval value
///////////////////////////////////////////////////////

#define TTC__INTERVAL_REG_2__OFFSET 0x34ULL
#define TTC__INTERVAL_REG_2__NUM  0x1

#define TTC__INTERVAL_REG_2__INTERVAL__SHIFT    0
#define TTC__INTERVAL_REG_2__RESERVED_31_16__SHIFT    16

#define TTC__INTERVAL_REG_2__INTERVAL__SIZE    16
#define TTC__INTERVAL_REG_2__RESERVED_31_16__SIZE    16

#define TTC__INTERVAL_REG_2__INTERVAL__MASK    0x0000ffff
#define TTC__INTERVAL_REG_2__RESERVED_31_16__MASK    0xffff0000

#define TTC__INTERVAL_REG_2__INTERVAL__POR_VALUE    0x0
#define TTC__INTERVAL_REG_2__RESERVED_31_16__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: INTERVAL_REG_3
// Interval value
///////////////////////////////////////////////////////

#define TTC__INTERVAL_REG_3__OFFSET 0x38ULL
#define TTC__INTERVAL_REG_3__NUM  0x1

#define TTC__INTERVAL_REG_3__INTERVAL__SHIFT    0
#define TTC__INTERVAL_REG_3__RESERVED_31_16__SHIFT    16

#define TTC__INTERVAL_REG_3__INTERVAL__SIZE    16
#define TTC__INTERVAL_REG_3__RESERVED_31_16__SIZE    16

#define TTC__INTERVAL_REG_3__INTERVAL__MASK    0x0000ffff
#define TTC__INTERVAL_REG_3__RESERVED_31_16__MASK    0xffff0000

#define TTC__INTERVAL_REG_3__INTERVAL__POR_VALUE    0x0
#define TTC__INTERVAL_REG_3__RESERVED_31_16__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: CNTR_VAL_REG_1
// Current counter value
///////////////////////////////////////////////////////

#define TTC__CNTR_VAL_REG_1__OFFSET 0x3CULL
#define TTC__CNTR_VAL_REG_1__NUM  0x1

#define TTC__CNTR_VAL_REG_1__VALUE__SHIFT    0
#define TTC__CNTR_VAL_REG_1__RESERVED_31_16__SHIFT    16

#define TTC__CNTR_VAL_REG_1__VALUE__SIZE    16
#define TTC__CNTR_VAL_REG_1__RESERVED_31_16__SIZE    16

#define TTC__CNTR_VAL_REG_1__VALUE__MASK    0x0000ffff
#define TTC__CNTR_VAL_REG_1__RESERVED_31_16__MASK    0xffff0000

#define TTC__CNTR_VAL_REG_1__VALUE__POR_VALUE    0x0
#define TTC__CNTR_VAL_REG_1__RESERVED_31_16__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: CNTR_VAL_REG_2
// Current counter value
///////////////////////////////////////////////////////

#define TTC__CNTR_VAL_REG_2__OFFSET 0x40ULL
#define TTC__CNTR_VAL_REG_2__NUM  0x1

#define TTC__CNTR_VAL_REG_2__VALUE__SHIFT    0
#define TTC__CNTR_VAL_REG_2__RESERVED_31_16__SHIFT    16

#define TTC__CNTR_VAL_REG_2__VALUE__SIZE    16
#define TTC__CNTR_VAL_REG_2__RESERVED_31_16__SIZE    16

#define TTC__CNTR_VAL_REG_2__VALUE__MASK    0x0000ffff
#define TTC__CNTR_VAL_REG_2__RESERVED_31_16__MASK    0xffff0000

#define TTC__CNTR_VAL_REG_2__VALUE__POR_VALUE    0x0
#define TTC__CNTR_VAL_REG_2__RESERVED_31_16__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: CNTR_VAL_REG_3
// Current counter value
///////////////////////////////////////////////////////

#define TTC__CNTR_VAL_REG_3__OFFSET 0x44ULL
#define TTC__CNTR_VAL_REG_3__NUM  0x1

#define TTC__CNTR_VAL_REG_3__VALUE__SHIFT    0
#define TTC__CNTR_VAL_REG_3__RESERVED_31_16__SHIFT    16

#define TTC__CNTR_VAL_REG_3__VALUE__SIZE    16
#define TTC__CNTR_VAL_REG_3__RESERVED_31_16__SIZE    16

#define TTC__CNTR_VAL_REG_3__VALUE__MASK    0x0000ffff
#define TTC__CNTR_VAL_REG_3__RESERVED_31_16__MASK    0xffff0000

#define TTC__CNTR_VAL_REG_3__VALUE__POR_VALUE    0x0
#define TTC__CNTR_VAL_REG_3__RESERVED_31_16__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MATCH_1_REG_1
// Match value
///////////////////////////////////////////////////////

#define TTC__MATCH_1_REG_1__OFFSET 0x48ULL
#define TTC__MATCH_1_REG_1__NUM  0x1

#define TTC__MATCH_1_REG_1__MATCH__SHIFT    0
#define TTC__MATCH_1_REG_1__RESERVED_31_16__SHIFT    16

#define TTC__MATCH_1_REG_1__MATCH__SIZE    16
#define TTC__MATCH_1_REG_1__RESERVED_31_16__SIZE    16

#define TTC__MATCH_1_REG_1__MATCH__MASK    0x0000ffff
#define TTC__MATCH_1_REG_1__RESERVED_31_16__MASK    0xffff0000

#define TTC__MATCH_1_REG_1__MATCH__POR_VALUE    0x0
#define TTC__MATCH_1_REG_1__RESERVED_31_16__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MATCH_2_REG_1
// Match value
///////////////////////////////////////////////////////

#define TTC__MATCH_2_REG_1__OFFSET 0x4CULL
#define TTC__MATCH_2_REG_1__NUM  0x1

#define TTC__MATCH_2_REG_1__MATCH__SHIFT    0
#define TTC__MATCH_2_REG_1__RESERVED_31_16__SHIFT    16

#define TTC__MATCH_2_REG_1__MATCH__SIZE    16
#define TTC__MATCH_2_REG_1__RESERVED_31_16__SIZE    16

#define TTC__MATCH_2_REG_1__MATCH__MASK    0x0000ffff
#define TTC__MATCH_2_REG_1__RESERVED_31_16__MASK    0xffff0000

#define TTC__MATCH_2_REG_1__MATCH__POR_VALUE    0x0
#define TTC__MATCH_2_REG_1__RESERVED_31_16__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MATCH_3_REG_1
// Match value
///////////////////////////////////////////////////////

#define TTC__MATCH_3_REG_1__OFFSET 0x50ULL
#define TTC__MATCH_3_REG_1__NUM  0x1

#define TTC__MATCH_3_REG_1__MATCH__SHIFT    0
#define TTC__MATCH_3_REG_1__RESERVED_31_16__SHIFT    16

#define TTC__MATCH_3_REG_1__MATCH__SIZE    16
#define TTC__MATCH_3_REG_1__RESERVED_31_16__SIZE    16

#define TTC__MATCH_3_REG_1__MATCH__MASK    0x0000ffff
#define TTC__MATCH_3_REG_1__RESERVED_31_16__MASK    0xffff0000

#define TTC__MATCH_3_REG_1__MATCH__POR_VALUE    0x0
#define TTC__MATCH_3_REG_1__RESERVED_31_16__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MATCH_1_REG_2
// Match value
///////////////////////////////////////////////////////

#define TTC__MATCH_1_REG_2__OFFSET 0x54ULL
#define TTC__MATCH_1_REG_2__NUM  0x1

#define TTC__MATCH_1_REG_2__MATCH__SHIFT    0
#define TTC__MATCH_1_REG_2__RESERVED_31_16__SHIFT    16

#define TTC__MATCH_1_REG_2__MATCH__SIZE    16
#define TTC__MATCH_1_REG_2__RESERVED_31_16__SIZE    16

#define TTC__MATCH_1_REG_2__MATCH__MASK    0x0000ffff
#define TTC__MATCH_1_REG_2__RESERVED_31_16__MASK    0xffff0000

#define TTC__MATCH_1_REG_2__MATCH__POR_VALUE    0x0
#define TTC__MATCH_1_REG_2__RESERVED_31_16__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MATCH_2_REG_2
// Match value
///////////////////////////////////////////////////////

#define TTC__MATCH_2_REG_2__OFFSET 0x58ULL
#define TTC__MATCH_2_REG_2__NUM  0x1

#define TTC__MATCH_2_REG_2__MATCH__SHIFT    0
#define TTC__MATCH_2_REG_2__RESERVED_31_16__SHIFT    16

#define TTC__MATCH_2_REG_2__MATCH__SIZE    16
#define TTC__MATCH_2_REG_2__RESERVED_31_16__SHIFT    16

#define TTC__MATCH_2_REG_2__MATCH__MASK    0x0000ffff
#define TTC__MATCH_2_REG_2__RESERVED_31_16__MASK    0xffff0000

#define TTC__MATCH_2_REG_2__MATCH__POR_VALUE    0x0
#define TTC__MATCH_2_REG_2__RESERVED_31_16__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MATCH_3_REG_2
// Match value
///////////////////////////////////////////////////////

#define TTC__MATCH_3_REG_2__OFFSET 0x5CULL
#define TTC__MATCH_3_REG_2__NUM  0x1

#define TTC__MATCH_3_REG_2__MATCH__SHIFT    0
#define TTC__MATCH_3_REG_2__RESERVED_31_16__SHIFT    16

#define TTC__MATCH_3_REG_2__MATCH__SIZE    16
#define TTC__MATCH_3_REG_2__RESERVED_31_16__SIZE    16

#define TTC__MATCH_3_REG_2__MATCH__MASK    0x0000ffff
#define TTC__MATCH_3_REG_2__RESERVED_31_16__MASK    0xffff0000

#define TTC__MATCH_3_REG_2__MATCH__POR_VALUE    0x0
#define TTC__MATCH_3_REG_2__RESERVED_31_16__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MATCH_1_REG_3
// Match value
///////////////////////////////////////////////////////

#define TTC__MATCH_1_REG_3__OFFSET 0x60ULL
#define TTC__MATCH_1_REG_3__NUM  0x1

#define TTC__MATCH_1_REG_3__MATCH__SHIFT    0
#define TTC__MATCH_1_REG_3__RESERVED_31_16__SHIFT    16

#define TTC__MATCH_1_REG_3__MATCH__SIZE    16
#define TTC__MATCH_1_REG_3__RESERVED_31_16__SIZE    16

#define TTC__MATCH_1_REG_3__MATCH__MASK    0x0000ffff
#define TTC__MATCH_1_REG_3__RESERVED_31_16__MASK    0xffff0000

#define TTC__MATCH_1_REG_3__MATCH__POR_VALUE    0x0
#define TTC__MATCH_1_REG_3__RESERVED_31_16__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MATCH_2_REG_3
// Match value
///////////////////////////////////////////////////////

#define TTC__MATCH_2_REG_3__OFFSET 0x64ULL
#define TTC__MATCH_2_REG_3__NUM  0x1

#define TTC__MATCH_2_REG_3__MATCH__SHIFT    0
#define TTC__MATCH_2_REG_3__RESERVED_31_16__SHIFT    16

#define TTC__MATCH_2_REG_3__MATCH__SIZE    16
#define TTC__MATCH_2_REG_3__RESERVED_31_16__SIZE    16

#define TTC__MATCH_2_REG_3__MATCH__MASK    0x0000ffff
#define TTC__MATCH_2_REG_3__RESERVED_31_16__MASK    0xffff0000

#define TTC__MATCH_2_REG_3__MATCH__POR_VALUE    0x0
#define TTC__MATCH_2_REG_3__RESERVED_31_16__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MATCH_3_REG_3
// Match value
///////////////////////////////////////////////////////

#define TTC__MATCH_3_REG_3__OFFSET 0x68ULL
#define TTC__MATCH_3_REG_3__NUM  0x1

#define TTC__MATCH_3_REG_3__MATCH__SHIFT    0
#define TTC__MATCH_3_REG_3__RESERVED_31_16__SHIFT    16

#define TTC__MATCH_3_REG_3__MATCH__SIZE    16
#define TTC__MATCH_3_REG_3__RESERVED_31_16__SIZE    16

#define TTC__MATCH_3_REG_3__MATCH__MASK    0x0000ffff
#define TTC__MATCH_3_REG_3__RESERVED_31_16__MASK    0xffff0000

#define TTC__MATCH_3_REG_3__MATCH__POR_VALUE    0x0
#define TTC__MATCH_3_REG_3__RESERVED_31_16__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: IRQ_EN_REG_1
// ANDed with corresponding Interrupt Register
///////////////////////////////////////////////////////

#define TTC__IRQ_EN_REG_1__OFFSET 0x6CULL
#define TTC__IRQ_EN_REG_1__NUM  0x1

#define TTC__IRQ_EN_REG_1__IEN__SHIFT    0
#define TTC__IRQ_EN_REG_1__RESERVED_31_6__SHIFT    6

#define TTC__IRQ_EN_REG_1__IEN__SIZE    6
#define TTC__IRQ_EN_REG_1__RESERVED_31_6__SIZE    26

#define TTC__IRQ_EN_REG_1__IEN__MASK    0x0000003f
#define TTC__IRQ_EN_REG_1__RESERVED_31_6__MASK    0xffffffc0

#define TTC__IRQ_EN_REG_1__IEN__POR_VALUE    0x0
#define TTC__IRQ_EN_REG_1__RESERVED_31_6__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: IRQ_EN_REG_2
// ANDed with corresponding Interrupt Register
///////////////////////////////////////////////////////

#define TTC__IRQ_EN_REG_2__OFFSET 0x70ULL
#define TTC__IRQ_EN_REG_2__NUM  0x1

#define TTC__IRQ_EN_REG_2__IEN__SHIFT    0
#define TTC__IRQ_EN_REG_2__RESERVED_31_6__SHIFT    6

#define TTC__IRQ_EN_REG_2__IEN__SIZE    6
#define TTC__IRQ_EN_REG_2__RESERVED_31_6__SIZE    26

#define TTC__IRQ_EN_REG_2__IEN__MASK    0x0000003f
#define TTC__IRQ_EN_REG_2__RESERVED_31_6__MASK    0xffffffc0

#define TTC__IRQ_EN_REG_2__IEN__POR_VALUE    0x0
#define TTC__IRQ_EN_REG_2__RESERVED_31_6__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: IRQ_EN_REG_3
// ANDed with corresponding Interrupt Register
///////////////////////////////////////////////////////

#define TTC__IRQ_EN_REG_3__OFFSET 0x74ULL
#define TTC__IRQ_EN_REG_3__NUM  0x1

#define TTC__IRQ_EN_REG_3__IEN__SHIFT    0
#define TTC__IRQ_EN_REG_3__RESERVED_31_6__SHIFT    6

#define TTC__IRQ_EN_REG_3__IEN__SIZE    6
#define TTC__IRQ_EN_REG_3__RESERVED_31_6__SIZE    26

#define TTC__IRQ_EN_REG_3__IEN__MASK    0x0000003f
#define TTC__IRQ_EN_REG_3__RESERVED_31_6__MASK    0xffffffc0

#define TTC__IRQ_EN_REG_3__IEN__POR_VALUE    0x0
#define TTC__IRQ_EN_REG_3__RESERVED_31_6__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: IRQ_REG_1
// Counter 1 Interval, Match, Overflow and Event interrupts
///////////////////////////////////////////////////////

#define TTC__IRQ_REG_1__OFFSET 0x78ULL
#define TTC__IRQ_REG_1__NUM  0x1

#define TTC__IRQ_REG_1__IV__SHIFT    0
#define TTC__IRQ_REG_1__M1__SHIFT    1
#define TTC__IRQ_REG_1__M2__SHIFT    2
#define TTC__IRQ_REG_1__M3__SHIFT    3
#define TTC__IRQ_REG_1__OV__SHIFT    4
#define TTC__IRQ_REG_1__EV__SHIFT    5
#define TTC__IRQ_REG_1__RESERVED_31_6__SHIFT    6

#define TTC__IRQ_REG_1__IV__SIZE    1
#define TTC__IRQ_REG_1__M1__SIZE    1
#define TTC__IRQ_REG_1__M2__SIZE    1
#define TTC__IRQ_REG_1__M3__SIZE    1
#define TTC__IRQ_REG_1__OV__SIZE    1
#define TTC__IRQ_REG_1__EV__SIZE    1
#define TTC__IRQ_REG_1__RESERVED_31_6__SIZE    26

#define TTC__IRQ_REG_1__IV__MASK    0x00000001
#define TTC__IRQ_REG_1__M1__MASK    0x00000002
#define TTC__IRQ_REG_1__M2__MASK    0x00000004
#define TTC__IRQ_REG_1__M3__MASK    0x00000008
#define TTC__IRQ_REG_1__OV__MASK    0x00000010
#define TTC__IRQ_REG_1__EV__MASK    0x00000020
#define TTC__IRQ_REG_1__RESERVED_31_6__MASK    0xffffffc0

#define TTC__IRQ_REG_1__IV__POR_VALUE    0x0
#define TTC__IRQ_REG_1__M1__POR_VALUE    0x0
#define TTC__IRQ_REG_1__M2__POR_VALUE    0x0
#define TTC__IRQ_REG_1__M3__POR_VALUE    0x0
#define TTC__IRQ_REG_1__OV__POR_VALUE    0x0
#define TTC__IRQ_REG_1__EV__POR_VALUE    0x0
#define TTC__IRQ_REG_1__RESERVED_31_6__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: IRQ_REG_2
// Counter 2 Interval, Match, Overflow and Event interrupts
///////////////////////////////////////////////////////

#define TTC__IRQ_REG_2__OFFSET 0x7CULL
#define TTC__IRQ_REG_2__NUM  0x1

#define TTC__IRQ_REG_2__IV__SHIFT    0
#define TTC__IRQ_REG_2__M1__SHIFT    1
#define TTC__IRQ_REG_2__M2__SHIFT    2
#define TTC__IRQ_REG_2__M3__SHIFT    3
#define TTC__IRQ_REG_2__OV__SHIFT    4
#define TTC__IRQ_REG_2__EV__SHIFT    5
#define TTC__IRQ_REG_2__RESERVED_31_6__SHIFT    6

#define TTC__IRQ_REG_2__IV__SIZE    1
#define TTC__IRQ_REG_2__M1__SIZE    1
#define TTC__IRQ_REG_2__M2__SIZE    1
#define TTC__IRQ_REG_2__M3__SIZE    1
#define TTC__IRQ_REG_2__OV__SIZE    1
#define TTC__IRQ_REG_2__EV__SIZE    1
#define TTC__IRQ_REG_2__RESERVED_31_6__SIZE    26

#define TTC__IRQ_REG_2__IV__MASK    0x00000001
#define TTC__IRQ_REG_2__M1__MASK    0x00000002
#define TTC__IRQ_REG_2__M2__MASK    0x00000004
#define TTC__IRQ_REG_2__M3__MASK    0x00000008
#define TTC__IRQ_REG_2__OV__MASK    0x00000010
#define TTC__IRQ_REG_2__EV__MASK    0x00000020
#define TTC__IRQ_REG_2__RESERVED_31_6__MASK    0xffffffc0

#define TTC__IRQ_REG_2__IV__POR_VALUE    0x0
#define TTC__IRQ_REG_2__M1__POR_VALUE    0x0
#define TTC__IRQ_REG_2__M2__POR_VALUE    0x0
#define TTC__IRQ_REG_2__M3__POR_VALUE    0x0
#define TTC__IRQ_REG_2__OV__POR_VALUE    0x0
#define TTC__IRQ_REG_2__EV__POR_VALUE    0x0
#define TTC__IRQ_REG_2__RESERVED_31_6__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: IRQ_REG_3
// Counter 3 Interval, Match, Overflow and Event interrupts
///////////////////////////////////////////////////////

#define TTC__IRQ_REG_3__OFFSET 0x80ULL
#define TTC__IRQ_REG_3__NUM  0x1

#define TTC__IRQ_REG_3__IV__SHIFT    0
#define TTC__IRQ_REG_3__M1__SHIFT    1
#define TTC__IRQ_REG_3__M2__SHIFT    2
#define TTC__IRQ_REG_3__M3__SHIFT    3
#define TTC__IRQ_REG_3__OV__SHIFT    4
#define TTC__IRQ_REG_3__EV__SHIFT    5
#define TTC__IRQ_REG_3__RESERVED_31_6__SHIFT    6

#define TTC__IRQ_REG_3__IV__SIZE    1
#define TTC__IRQ_REG_3__M1__SIZE    1
#define TTC__IRQ_REG_3__M2__SIZE    1
#define TTC__IRQ_REG_3__M3__SIZE    1
#define TTC__IRQ_REG_3__OV__SIZE    1
#define TTC__IRQ_REG_3__EV__SIZE    1
#define TTC__IRQ_REG_3__RESERVED_31_6__SIZE    26

#define TTC__IRQ_REG_3__IV__MASK    0x00000001
#define TTC__IRQ_REG_3__M1__MASK    0x00000002
#define TTC__IRQ_REG_3__M2__MASK    0x00000004
#define TTC__IRQ_REG_3__M3__MASK    0x00000008
#define TTC__IRQ_REG_3__OV__MASK    0x00000010
#define TTC__IRQ_REG_3__EV__MASK    0x00000020
#define TTC__IRQ_REG_3__RESERVED_31_6__MASK    0xffffffc0

#define TTC__IRQ_REG_3__IV__POR_VALUE    0x0
#define TTC__IRQ_REG_3__M1__POR_VALUE    0x0
#define TTC__IRQ_REG_3__M2__POR_VALUE    0x0
#define TTC__IRQ_REG_3__M3__POR_VALUE    0x0
#define TTC__IRQ_REG_3__OV__POR_VALUE    0x0
#define TTC__IRQ_REG_3__EV__POR_VALUE    0x0
#define TTC__IRQ_REG_3__RESERVED_31_6__POR_VALUE    0x0


/**************************** Type Definitions ******************************/
typedef struct
{
    AL_U32                  DevId;
    AL_U32                  BaseAddress;
    AL_U32                  InputClockHz;
    AL_U32                  IntrNum;
} AL_TTC_HwConfigStruct;

#ifdef __cplusplus
}
#endif

#endif
