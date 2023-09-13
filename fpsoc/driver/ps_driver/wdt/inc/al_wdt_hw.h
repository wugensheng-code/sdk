/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef AL_WDT_HW_H
#define AL_WDT_HW_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files ********************************/
#include "al_core.h"

/************************** Constant Definitions ****************************/

/**************************** Type Definitions ******************************/

/***************** Macros (Inline Functions) Definitions ********************/

///////////////////////////////////////////////////////
// Register: WDT_TOP_CFG_CTRL_ADDR
// Top Nonsecure Config Contrl
///////////////////////////////////////////////////////
#define WDT_TOP_CFG_CTRL_ADDR_WDT0_PAUSE_SHIFT    0
#define WDT_TOP_CFG_CTRL_ADDR_WDT1_PAUSE_SHIFT    1


///////////////////////////////////////////////////////
// Register: WDT_CR
// Control register
///////////////////////////////////////////////////////

#define WDT__WDT_CR__ADDR__OFFSET    0x0ULL
#define WDT__WDT_CR__NUM  0x1

#define WDT__WDT_CR__WDT_EN__SHIFT    0
#define WDT__WDT_CR__RMOD__SHIFT    1
#define WDT__WDT_CR__RPL__SHIFT    2
#define WDT__WDT_CR__RPL__SIZE     3
#define WDT__WDT_CR__NO_NAME__SHIFT    5
#define WDT__WDT_CR__RSVD_WDT_CR__SHIFT    6

#define WDT__WDT_CR__WDT_EN__MASK    0x00000001
#define WDT__WDT_CR__RMOD__MASK    0x00000002
#define WDT__WDT_CR__RPL__MASK    0x0000001c
#define WDT__WDT_CR__NO_NAME__MASK    0x00000020
#define WDT__WDT_CR__RSVD_WDT_CR__MASK    0xffffffc0

#define WDT__WDT_CR__WDT_EN__POR_VALUE    0x1
#define WDT__WDT_CR__RMOD__POR_VALUE    0x0
#define WDT__WDT_CR__RPL__POR_VALUE    0x4
#define WDT__WDT_CR__NO_NAME__POR_VALUE    0x0
#define WDT__WDT_CR__RSVD_WDT_CR__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: WDT_TORR
// Timeout range register
///////////////////////////////////////////////////////

#define WDT__WDT_TORR__ADDR__OFFSET    0x4ULL
#define WDT__WDT_TORR__NUM  0x1

#define WDT__WDT_TORR__TOP__SHIFT    0
#define WDT__WDT_TORR__TOP__SIZE     4
#define WDT__WDT_TORR__RSVD_TOP_INIT__SHIFT    4
#define WDT__WDT_TORR__RESERVED__SHIFT    8

#define WDT__WDT_TORR__TOP__MASK    0x0000000f
#define WDT__WDT_TORR__RSVD_TOP_INIT__MASK    0x000000f0
#define WDT__WDT_TORR__RESERVED__MASK    0xffffff00

#define WDT__WDT_TORR__TOP__POR_VALUE    0xc
#define WDT__WDT_TORR__RSVD_TOP_INIT__POR_VALUE    0x0
#define WDT__WDT_TORR__RESERVED__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: WDT_CCVR
// Current counter value register
///////////////////////////////////////////////////////

#define WDT__WDT_CCVR__ADDR__OFFSET    0x8ULL
#define WDT__WDT_CCVR__NUM  0x1

#define WDT__WDT_CCVR__WDT_CCVR__SHIFT    0

#define WDT__WDT_CCVR__WDT_CCVR__MASK    0xffffffff

#define WDT__WDT_CCVR__WDT_CCVR__POR_VALUE    0xfffffff


///////////////////////////////////////////////////////
// Register: WDT_CRR
// Counter restart register
///////////////////////////////////////////////////////

#define WDT__WDT_CRR__ADDR__OFFSET    0xCULL
#define WDT__WDT_CRR__NUM  0x1

#define WDT__WDT_CRR__WDT_CRR__SHIFT    0
#define WDT__WDT_CRR__WDT_CRR__SIZE     8
#define WDT__WDT_CRR__RSVD_WDT_CRR__SHIFT    8

#define WDT__WDT_CRR__WDT_CRR__MASK    0x000000ff
#define WDT__WDT_CRR__RSVD_WDT_CRR__MASK    0xffffff00

#define WDT__WDT_CRR__WDT_CRR__POR_VALUE    0x0
#define WDT__WDT_CRR__RSVD_WDT_CRR__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: WDT_STAT
// Interrupt status register
///////////////////////////////////////////////////////

#define WDT__WDT_STAT__ADDR__OFFSET    0x10ULL
#define WDT__WDT_STAT__NUM  0x1

#define WDT__WDT_STAT__WDT_STAT__SHIFT    0
#define WDT__WDT_STAT__RSVD_WDT_STAT__SHIFT    1

#define WDT__WDT_STAT__WDT_STAT__MASK    0x00000001
#define WDT__WDT_STAT__RSVD_WDT_STAT__MASK    0xfffffffe

#define WDT__WDT_STAT__WDT_STAT__POR_VALUE    0x0
#define WDT__WDT_STAT__RSVD_WDT_STAT__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: WDT_EOI
// Interrupt clear register
///////////////////////////////////////////////////////

#define WDT__WDT_EOI__ADDR__OFFSET    0x14ULL
#define WDT__WDT_EOI__NUM  0x1

#define WDT__WDT_EOI__WDT_EOI__SHIFT    0
#define WDT__WDT_EOI__RSVD_WDT_EOI__SHIFT    1

#define WDT__WDT_EOI__WDT_EOI__MASK    0x00000001
#define WDT__WDT_EOI__RSVD_WDT_EOI__MASK    0xfffffffe

#define WDT__WDT_EOI__WDT_EOI__POR_VALUE    0x0
#define WDT__WDT_EOI__RSVD_WDT_EOI__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: WDT_COMP_PARAMS_5
// Component Parameters Register 5
///////////////////////////////////////////////////////

#define WDT__WDT_COMP_PARAMS_5__ADDR__OFFSET    0xE4ULL
#define WDT__WDT_COMP_PARAMS_5__NUM  0x1

#define WDT__WDT_COMP_PARAMS_5__CP_WDT_USER_TOP_MAX__SHIFT    0

#define WDT__WDT_COMP_PARAMS_5__CP_WDT_USER_TOP_MAX__MASK    0xffffffff

#define WDT__WDT_COMP_PARAMS_5__CP_WDT_USER_TOP_MAX__POR_VALUE    0x7fffffff


///////////////////////////////////////////////////////
// Register: WDT_COMP_PARAMS_4
// Component Parameters Register 4
///////////////////////////////////////////////////////

#define WDT__WDT_COMP_PARAMS_4__ADDR__OFFSET    0xE8ULL
#define WDT__WDT_COMP_PARAMS_4__NUM  0x1

#define WDT__WDT_COMP_PARAMS_4__CP_WDT_USER_TOP_INIT_MAX__SHIFT    0

#define WDT__WDT_COMP_PARAMS_4__CP_WDT_USER_TOP_INIT_MAX__MASK    0xffffffff

#define WDT__WDT_COMP_PARAMS_4__CP_WDT_USER_TOP_INIT_MAX__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: WDT_COMP_PARAMS_3
// Component Parameters Register 3
///////////////////////////////////////////////////////

#define WDT__WDT_COMP_PARAMS_3__ADDR__OFFSET    0xECULL
#define WDT__WDT_COMP_PARAMS_3__NUM  0x1

#define WDT__WDT_COMP_PARAMS_3__CD_WDT_TOP_RST__SHIFT    0

#define WDT__WDT_COMP_PARAMS_3__CD_WDT_TOP_RST__MASK    0xffffffff

#define WDT__WDT_COMP_PARAMS_3__CD_WDT_TOP_RST__POR_VALUE    0xc


///////////////////////////////////////////////////////
// Register: WDT_COMP_PARAMS_2
// Component Parameters Register 2
///////////////////////////////////////////////////////

#define WDT__WDT_COMP_PARAMS_2__ADDR__OFFSET    0xF0ULL
#define WDT__WDT_COMP_PARAMS_2__NUM  0x1

#define WDT__WDT_COMP_PARAMS_2__CP_WDT_CNT_RST__SHIFT    0

#define WDT__WDT_COMP_PARAMS_2__CP_WDT_CNT_RST__MASK    0xffffffff

#define WDT__WDT_COMP_PARAMS_2__CP_WDT_CNT_RST__POR_VALUE    0xfffffff


///////////////////////////////////////////////////////
// Register: WDT_COMP_PARAMS_1
// Component Parameters Register 1
///////////////////////////////////////////////////////

#define WDT__WDT_COMP_PARAMS_1__ADDR__OFFSET    0xF4ULL
#define WDT__WDT_COMP_PARAMS_1__NUM  0x1

#define WDT__WDT_COMP_PARAMS_1__WDT_ALWAYS_EN__SHIFT    0
#define WDT__WDT_COMP_PARAMS_1__WDT_DFLT_RMOD__SHIFT    1
#define WDT__WDT_COMP_PARAMS_1__WDT_DUAL_TOP__SHIFT    2
#define WDT__WDT_COMP_PARAMS_1__WDT_HC_RMOD__SHIFT    3
#define WDT__WDT_COMP_PARAMS_1__WDT_HC_RPL__SHIFT    4
#define WDT__WDT_COMP_PARAMS_1__WDT_HC_TOP__SHIFT    5
#define WDT__WDT_COMP_PARAMS_1__WDT_USE_FIX_TOP__SHIFT    6
#define WDT__WDT_COMP_PARAMS_1__WDT_PAUSE__SHIFT    7
#define WDT__WDT_COMP_PARAMS_1__APB_DATA_WIDTH__SHIFT    8
#define WDT__WDT_COMP_PARAMS_1__WDT_DFLT_RPL__SHIFT    10
#define WDT__WDT_COMP_PARAMS_1__RSVD_15_13__SHIFT    13
#define WDT__WDT_COMP_PARAMS_1__WDT_DFLT_TOP__SHIFT    16
#define WDT__WDT_COMP_PARAMS_1__WDT_DFLT_TOP_INIT__SHIFT    20
#define WDT__WDT_COMP_PARAMS_1__WDT_CNT_WIDTH__SHIFT    24
#define WDT__WDT_COMP_PARAMS_1__RSVD_31_29__SHIFT    29

#define WDT__WDT_COMP_PARAMS_1__WDT_ALWAYS_EN__MASK    0x00000001
#define WDT__WDT_COMP_PARAMS_1__WDT_DFLT_RMOD__MASK    0x00000002
#define WDT__WDT_COMP_PARAMS_1__WDT_DUAL_TOP__MASK    0x00000004
#define WDT__WDT_COMP_PARAMS_1__WDT_HC_RMOD__MASK    0x00000008
#define WDT__WDT_COMP_PARAMS_1__WDT_HC_RPL__MASK    0x00000010
#define WDT__WDT_COMP_PARAMS_1__WDT_HC_TOP__MASK    0x00000020
#define WDT__WDT_COMP_PARAMS_1__WDT_USE_FIX_TOP__MASK    0x00000040
#define WDT__WDT_COMP_PARAMS_1__WDT_PAUSE__MASK    0x00000080
#define WDT__WDT_COMP_PARAMS_1__APB_DATA_WIDTH__MASK    0x00000300
#define WDT__WDT_COMP_PARAMS_1__WDT_DFLT_RPL__MASK    0x00001c00
#define WDT__WDT_COMP_PARAMS_1__RSVD_15_13__MASK    0x0000e000
#define WDT__WDT_COMP_PARAMS_1__WDT_DFLT_TOP__MASK    0x000f0000
#define WDT__WDT_COMP_PARAMS_1__WDT_DFLT_TOP_INIT__MASK    0x00f00000
#define WDT__WDT_COMP_PARAMS_1__WDT_CNT_WIDTH__MASK    0x1f000000
#define WDT__WDT_COMP_PARAMS_1__RSVD_31_29__MASK    0xe0000000

#define WDT__WDT_COMP_PARAMS_1__WDT_ALWAYS_EN__POR_VALUE    0x1
#define WDT__WDT_COMP_PARAMS_1__WDT_DFLT_RMOD__POR_VALUE    0x0
#define WDT__WDT_COMP_PARAMS_1__WDT_DUAL_TOP__POR_VALUE    0x0
#define WDT__WDT_COMP_PARAMS_1__WDT_HC_RMOD__POR_VALUE    0x0
#define WDT__WDT_COMP_PARAMS_1__WDT_HC_RPL__POR_VALUE    0x0
#define WDT__WDT_COMP_PARAMS_1__WDT_HC_TOP__POR_VALUE    0x0
#define WDT__WDT_COMP_PARAMS_1__WDT_USE_FIX_TOP__POR_VALUE    0x0
#define WDT__WDT_COMP_PARAMS_1__WDT_PAUSE__POR_VALUE    0x1
#define WDT__WDT_COMP_PARAMS_1__APB_DATA_WIDTH__POR_VALUE    0x2
#define WDT__WDT_COMP_PARAMS_1__WDT_DFLT_RPL__POR_VALUE    0x4
#define WDT__WDT_COMP_PARAMS_1__RSVD_15_13__POR_VALUE    0x0
#define WDT__WDT_COMP_PARAMS_1__WDT_DFLT_TOP__POR_VALUE    0xc
#define WDT__WDT_COMP_PARAMS_1__WDT_DFLT_TOP_INIT__POR_VALUE    0x0
#define WDT__WDT_COMP_PARAMS_1__WDT_CNT_WIDTH__POR_VALUE    0x10
#define WDT__WDT_COMP_PARAMS_1__RSVD_31_29__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: WDT_COMP_VERSION
// Component Version register
///////////////////////////////////////////////////////

#define WDT__WDT_COMP_VERSION__ADDR__OFFSET    0xF8ULL
#define WDT__WDT_COMP_VERSION__NUM  0x1

#define WDT__WDT_COMP_VERSION__WDT_COMP_VERSION__SHIFT    0

#define WDT__WDT_COMP_VERSION__WDT_COMP_VERSION__MASK    0xffffffff

#define WDT__WDT_COMP_VERSION__WDT_COMP_VERSION__POR_VALUE    0x3131322a


///////////////////////////////////////////////////////
// Register: WDT_COMP_TYPE
// Component Type register
///////////////////////////////////////////////////////

#define WDT__WDT_COMP_TYPE__ADDR__OFFSET    0xFCULL
#define WDT__WDT_COMP_TYPE__NUM  0x1

#define WDT__WDT_COMP_TYPE__WDT_COMP_TYPE__SHIFT    0

#define WDT__WDT_COMP_TYPE__WDT_COMP_TYPE__MASK    0xffffffff

#define WDT__WDT_COMP_TYPE__WDT_COMP_TYPE__POR_VALUE    0x44570120


///////////////////////////////////////////////////////
// Register: WDT_CR_BLK1
// Control register
///////////////////////////////////////////////////////

#define WDT__WDT_CR_BLK1__ADDR__OFFSET    0x100ULL
#define WDT__WDT_CR_BLK1__NUM  0x1

#define WDT__WDT_CR_BLK1__WDT_EN__SHIFT    0
#define WDT__WDT_CR_BLK1__RMOD__SHIFT    1
#define WDT__WDT_CR_BLK1__RPL__SHIFT    2
#define WDT__WDT_CR_BLK1__NO_NAME__SHIFT    5
#define WDT__WDT_CR_BLK1__RSVD_WDT_CR__SHIFT    6

#define WDT__WDT_CR_BLK1__WDT_EN__MASK    0x00000001
#define WDT__WDT_CR_BLK1__RMOD__MASK    0x00000002
#define WDT__WDT_CR_BLK1__RPL__MASK    0x0000001c
#define WDT__WDT_CR_BLK1__NO_NAME__MASK    0x00000020
#define WDT__WDT_CR_BLK1__RSVD_WDT_CR__MASK    0xffffffc0

#define WDT__WDT_CR_BLK1__WDT_EN__POR_VALUE    0x1
#define WDT__WDT_CR_BLK1__RMOD__POR_VALUE    0x0
#define WDT__WDT_CR_BLK1__RPL__POR_VALUE    0x4
#define WDT__WDT_CR_BLK1__NO_NAME__POR_VALUE    0x0
#define WDT__WDT_CR_BLK1__RSVD_WDT_CR__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: WDT_TORR_BLK1
// Timeout range register
///////////////////////////////////////////////////////

#define WDT__WDT_TORR_BLK1__ADDR__OFFSET    0x104ULL
#define WDT__WDT_TORR_BLK1__NUM  0x1

#define WDT__WDT_TORR_BLK1__TOP__SHIFT    0
#define WDT__WDT_TORR_BLK1__RSVD_TOP_INIT__SHIFT    4
#define WDT__WDT_TORR_BLK1__RESERVED__SHIFT    8

#define WDT__WDT_TORR_BLK1__TOP__MASK    0x0000000f
#define WDT__WDT_TORR_BLK1__RSVD_TOP_INIT__MASK    0x000000f0
#define WDT__WDT_TORR_BLK1__RESERVED__MASK    0xffffff00

#define WDT__WDT_TORR_BLK1__TOP__POR_VALUE    0xc
#define WDT__WDT_TORR_BLK1__RSVD_TOP_INIT__POR_VALUE    0x0
#define WDT__WDT_TORR_BLK1__RESERVED__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: WDT_CCVR_BLK1
// Current counter value register
///////////////////////////////////////////////////////

#define WDT__WDT_CCVR_BLK1__ADDR__OFFSET    0x108ULL
#define WDT__WDT_CCVR_BLK1__NUM  0x1

#define WDT__WDT_CCVR_BLK1__WDT_CCVR__SHIFT    0

#define WDT__WDT_CCVR_BLK1__WDT_CCVR__MASK    0xffffffff

#define WDT__WDT_CCVR_BLK1__WDT_CCVR__POR_VALUE    0xfffffff


///////////////////////////////////////////////////////
// Register: WDT_CRR_BLK1
// Counter restart register
///////////////////////////////////////////////////////

#define WDT__WDT_CRR_BLK1__ADDR__OFFSET    0x10CULL
#define WDT__WDT_CRR_BLK1__NUM  0x1

#define WDT__WDT_CRR_BLK1__WDT_CRR__SHIFT    0
#define WDT__WDT_CRR_BLK1__RSVD_WDT_CRR__SHIFT    8

#define WDT__WDT_CRR_BLK1__WDT_CRR__MASK    0x000000ff
#define WDT__WDT_CRR_BLK1__RSVD_WDT_CRR__MASK    0xffffff00

#define WDT__WDT_CRR_BLK1__WDT_CRR__POR_VALUE    0x0
#define WDT__WDT_CRR_BLK1__RSVD_WDT_CRR__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: WDT_STAT_BLK1
// Interrupt status register
///////////////////////////////////////////////////////

#define WDT__WDT_STAT_BLK1__ADDR__OFFSET    0x110ULL
#define WDT__WDT_STAT_BLK1__NUM  0x1

#define WDT__WDT_STAT_BLK1__WDT_STAT__SHIFT    0
#define WDT__WDT_STAT_BLK1__RSVD_WDT_STAT__SHIFT    1

#define WDT__WDT_STAT_BLK1__WDT_STAT__MASK    0x00000001
#define WDT__WDT_STAT_BLK1__RSVD_WDT_STAT__MASK    0xfffffffe

#define WDT__WDT_STAT_BLK1__WDT_STAT__POR_VALUE    0x0
#define WDT__WDT_STAT_BLK1__RSVD_WDT_STAT__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: WDT_EOI_BLK1
// Interrupt clear register
///////////////////////////////////////////////////////

#define WDT__WDT_EOI_BLK1__ADDR__OFFSET    0x114ULL
#define WDT__WDT_EOI_BLK1__NUM  0x1

#define WDT__WDT_EOI_BLK1__WDT_EOI__SHIFT    0
#define WDT__WDT_EOI_BLK1__RSVD_WDT_EOI__SHIFT    1

#define WDT__WDT_EOI_BLK1__WDT_EOI__MASK    0x00000001
#define WDT__WDT_EOI_BLK1__RSVD_WDT_EOI__MASK    0xfffffffe

#define WDT__WDT_EOI_BLK1__WDT_EOI__POR_VALUE    0x0
#define WDT__WDT_EOI_BLK1__RSVD_WDT_EOI__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: WDT_COMP_PARAMS_5_BLK1
// Component Parameters Register 5
///////////////////////////////////////////////////////

#define WDT__WDT_COMP_PARAMS_5_BLK1__ADDR__OFFSET    0x1E4ULL
#define WDT__WDT_COMP_PARAMS_5_BLK1__NUM  0x1

#define WDT__WDT_COMP_PARAMS_5_BLK1__CP_WDT_USER_TOP_MAX__SHIFT    0

#define WDT__WDT_COMP_PARAMS_5_BLK1__CP_WDT_USER_TOP_MAX__MASK    0xffffffff

#define WDT__WDT_COMP_PARAMS_5_BLK1__CP_WDT_USER_TOP_MAX__POR_VALUE    0x7fffffff


///////////////////////////////////////////////////////
// Register: WDT_COMP_PARAMS_4_BLK1
// Component Parameters Register 4
///////////////////////////////////////////////////////

#define WDT__WDT_COMP_PARAMS_4_BLK1__ADDR__OFFSET    0x1E8ULL
#define WDT__WDT_COMP_PARAMS_4_BLK1__NUM  0x1

#define WDT__WDT_COMP_PARAMS_4_BLK1__CP_WDT_USER_TOP_INIT_MAX__SHIFT    0

#define WDT__WDT_COMP_PARAMS_4_BLK1__CP_WDT_USER_TOP_INIT_MAX__MASK    0xffffffff

#define WDT__WDT_COMP_PARAMS_4_BLK1__CP_WDT_USER_TOP_INIT_MAX__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: WDT_COMP_PARAMS_3_BLK1
// Component Parameters Register 3
///////////////////////////////////////////////////////

#define WDT__WDT_COMP_PARAMS_3_BLK1__ADDR__OFFSET    0x1ECULL
#define WDT__WDT_COMP_PARAMS_3_BLK1__NUM  0x1

#define WDT__WDT_COMP_PARAMS_3_BLK1__CD_WDT_TOP_RST__SHIFT    0

#define WDT__WDT_COMP_PARAMS_3_BLK1__CD_WDT_TOP_RST__MASK    0xffffffff

#define WDT__WDT_COMP_PARAMS_3_BLK1__CD_WDT_TOP_RST__POR_VALUE    0xc


///////////////////////////////////////////////////////
// Register: WDT_COMP_PARAMS_2_BLK1
// Component Parameters Register 2
///////////////////////////////////////////////////////

#define WDT__WDT_COMP_PARAMS_2_BLK1__ADDR__OFFSET    0x1F0ULL
#define WDT__WDT_COMP_PARAMS_2_BLK1__NUM  0x1

#define WDT__WDT_COMP_PARAMS_2_BLK1__CP_WDT_CNT_RST__SHIFT    0

#define WDT__WDT_COMP_PARAMS_2_BLK1__CP_WDT_CNT_RST__MASK    0xffffffff

#define WDT__WDT_COMP_PARAMS_2_BLK1__CP_WDT_CNT_RST__POR_VALUE    0xfffffff


///////////////////////////////////////////////////////
// Register: WDT_COMP_PARAMS_1_BLK1
// Component Parameters Register 1
///////////////////////////////////////////////////////

#define WDT__WDT_COMP_PARAMS_1_BLK1__ADDR__OFFSET     0x1F4ULL
#define WDT__WDT_COMP_PARAMS_1_BLK1__NUM  0x1

#define WDT__WDT_COMP_PARAMS_1_BLK1__WDT_ALWAYS_EN__SHIFT    0
#define WDT__WDT_COMP_PARAMS_1_BLK1__WDT_DFLT_RMOD__SHIFT    1
#define WDT__WDT_COMP_PARAMS_1_BLK1__WDT_DUAL_TOP__SHIFT    2
#define WDT__WDT_COMP_PARAMS_1_BLK1__WDT_HC_RMOD__SHIFT    3
#define WDT__WDT_COMP_PARAMS_1_BLK1__WDT_HC_RPL__SHIFT    4
#define WDT__WDT_COMP_PARAMS_1_BLK1__WDT_HC_TOP__SHIFT    5
#define WDT__WDT_COMP_PARAMS_1_BLK1__WDT_USE_FIX_TOP__SHIFT    6
#define WDT__WDT_COMP_PARAMS_1_BLK1__WDT_PAUSE__SHIFT    7
#define WDT__WDT_COMP_PARAMS_1_BLK1__APB_DATA_WIDTH__SHIFT    8
#define WDT__WDT_COMP_PARAMS_1_BLK1__WDT_DFLT_RPL__SHIFT    10
#define WDT__WDT_COMP_PARAMS_1_BLK1__RSVD_15_13__SHIFT    13
#define WDT__WDT_COMP_PARAMS_1_BLK1__WDT_DFLT_TOP__SHIFT    16
#define WDT__WDT_COMP_PARAMS_1_BLK1__WDT_DFLT_TOP_INIT__SHIFT    20
#define WDT__WDT_COMP_PARAMS_1_BLK1__WDT_CNT_WIDTH__SHIFT    24
#define WDT__WDT_COMP_PARAMS_1_BLK1__RSVD_31_29__SHIFT    29

#define WDT__WDT_COMP_PARAMS_1_BLK1__WDT_ALWAYS_EN__MASK    0x00000001
#define WDT__WDT_COMP_PARAMS_1_BLK1__WDT_DFLT_RMOD__MASK    0x00000002
#define WDT__WDT_COMP_PARAMS_1_BLK1__WDT_DUAL_TOP__MASK    0x00000004
#define WDT__WDT_COMP_PARAMS_1_BLK1__WDT_HC_RMOD__MASK    0x00000008
#define WDT__WDT_COMP_PARAMS_1_BLK1__WDT_HC_RPL__MASK    0x00000010
#define WDT__WDT_COMP_PARAMS_1_BLK1__WDT_HC_TOP__MASK    0x00000020
#define WDT__WDT_COMP_PARAMS_1_BLK1__WDT_USE_FIX_TOP__MASK    0x00000040
#define WDT__WDT_COMP_PARAMS_1_BLK1__WDT_PAUSE__MASK    0x00000080
#define WDT__WDT_COMP_PARAMS_1_BLK1__APB_DATA_WIDTH__MASK    0x00000300
#define WDT__WDT_COMP_PARAMS_1_BLK1__WDT_DFLT_RPL__MASK    0x00001c00
#define WDT__WDT_COMP_PARAMS_1_BLK1__RSVD_15_13__MASK    0x0000e000
#define WDT__WDT_COMP_PARAMS_1_BLK1__WDT_DFLT_TOP__MASK    0x000f0000
#define WDT__WDT_COMP_PARAMS_1_BLK1__WDT_DFLT_TOP_INIT__MASK    0x00f00000
#define WDT__WDT_COMP_PARAMS_1_BLK1__WDT_CNT_WIDTH__MASK    0x1f000000
#define WDT__WDT_COMP_PARAMS_1_BLK1__RSVD_31_29__MASK    0xe0000000

#define WDT__WDT_COMP_PARAMS_1_BLK1__WDT_ALWAYS_EN__POR_VALUE    0x1
#define WDT__WDT_COMP_PARAMS_1_BLK1__WDT_DFLT_RMOD__POR_VALUE    0x0
#define WDT__WDT_COMP_PARAMS_1_BLK1__WDT_DUAL_TOP__POR_VALUE    0x0
#define WDT__WDT_COMP_PARAMS_1_BLK1__WDT_HC_RMOD__POR_VALUE    0x0
#define WDT__WDT_COMP_PARAMS_1_BLK1__WDT_HC_RPL__POR_VALUE    0x0
#define WDT__WDT_COMP_PARAMS_1_BLK1__WDT_HC_TOP__POR_VALUE    0x0
#define WDT__WDT_COMP_PARAMS_1_BLK1__WDT_USE_FIX_TOP__POR_VALUE    0x0
#define WDT__WDT_COMP_PARAMS_1_BLK1__WDT_PAUSE__POR_VALUE    0x1
#define WDT__WDT_COMP_PARAMS_1_BLK1__APB_DATA_WIDTH__POR_VALUE    0x2
#define WDT__WDT_COMP_PARAMS_1_BLK1__WDT_DFLT_RPL__POR_VALUE    0x4
#define WDT__WDT_COMP_PARAMS_1_BLK1__RSVD_15_13__POR_VALUE    0x0
#define WDT__WDT_COMP_PARAMS_1_BLK1__WDT_DFLT_TOP__POR_VALUE    0xc
#define WDT__WDT_COMP_PARAMS_1_BLK1__WDT_DFLT_TOP_INIT__POR_VALUE    0x0
#define WDT__WDT_COMP_PARAMS_1_BLK1__WDT_CNT_WIDTH__POR_VALUE    0x10
#define WDT__WDT_COMP_PARAMS_1_BLK1__RSVD_31_29__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: WDT_COMP_VERSION_BLK1
// Component Version register
///////////////////////////////////////////////////////

#define WDT__WDT_COMP_VERSION_BLK1__ADDR__OFFSET    0x1F8ULL
#define WDT__WDT_COMP_VERSION_BLK1__NUM  0x1

#define WDT__WDT_COMP_VERSION_BLK1__WDT_COMP_VERSION__SHIFT    0

#define WDT__WDT_COMP_VERSION_BLK1__WDT_COMP_VERSION__MASK    0xffffffff

#define WDT__WDT_COMP_VERSION_BLK1__WDT_COMP_VERSION__POR_VALUE    0x3131322a


///////////////////////////////////////////////////////
// Register: WDT_COMP_TYPE_BLK1
// Component Type register
///////////////////////////////////////////////////////

#define WDT__WDT_COMP_TYPE_BLK1__ADDR__OFFSET     0x1FCULL
#define WDT__WDT_COMP_TYPE_BLK1__NUM  0x1

#define WDT__WDT_COMP_TYPE_BLK1__WDT_COMP_TYPE__SHIFT    0

#define WDT__WDT_COMP_TYPE_BLK1__WDT_COMP_TYPE__MASK    0xffffffff

#define WDT__WDT_COMP_TYPE_BLK1__WDT_COMP_TYPE__POR_VALUE    0x44570120


///////////////////////////////////////////////////////
// Register: WDT_CR_BLK2
// Control register
///////////////////////////////////////////////////////

#define WDT__WDT_CR_BLK2__ADDR__OFFSET 0x200ULL
#define WDT__WDT_CR_BLK2__NUM  0x1

#define WDT__WDT_CR_BLK2__WDT_EN__SHIFT    0
#define WDT__WDT_CR_BLK2__RMOD__SHIFT    1
#define WDT__WDT_CR_BLK2__RPL__SHIFT    2
#define WDT__WDT_CR_BLK2__NO_NAME__SHIFT    5
#define WDT__WDT_CR_BLK2__RSVD_WDT_CR__SHIFT    6

#define WDT__WDT_CR_BLK2__WDT_EN__MASK    0x00000001
#define WDT__WDT_CR_BLK2__RMOD__MASK    0x00000002
#define WDT__WDT_CR_BLK2__RPL__MASK    0x0000001c
#define WDT__WDT_CR_BLK2__NO_NAME__MASK    0x00000020
#define WDT__WDT_CR_BLK2__RSVD_WDT_CR__MASK    0xffffffc0

#define WDT__WDT_CR_BLK2__WDT_EN__POR_VALUE    0x1
#define WDT__WDT_CR_BLK2__RMOD__POR_VALUE    0x0
#define WDT__WDT_CR_BLK2__RPL__POR_VALUE    0x4
#define WDT__WDT_CR_BLK2__NO_NAME__POR_VALUE    0x0
#define WDT__WDT_CR_BLK2__RSVD_WDT_CR__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: WDT_TORR_BLK2
// Timeout range register
///////////////////////////////////////////////////////

#define WDT__WDT_TORR_BLK2__ADDR__OFFSET   0x204ULL
#define WDT__WDT_TORR_BLK2__NUM  0x1

#define WDT__WDT_TORR_BLK2__TOP__SHIFT    0
#define WDT__WDT_TORR_BLK2__RSVD_TOP_INIT__SHIFT    4
#define WDT__WDT_TORR_BLK2__RESERVED__SHIFT    8

#define WDT__WDT_TORR_BLK2__TOP__MASK    0x0000000f
#define WDT__WDT_TORR_BLK2__RSVD_TOP_INIT__MASK    0x000000f0
#define WDT__WDT_TORR_BLK2__RESERVED__MASK    0xffffff00

#define WDT__WDT_TORR_BLK2__TOP__POR_VALUE    0xc
#define WDT__WDT_TORR_BLK2__RSVD_TOP_INIT__POR_VALUE    0x0
#define WDT__WDT_TORR_BLK2__RESERVED__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: WDT_CCVR_BLK2
// Current counter value register
///////////////////////////////////////////////////////

#define WDT__WDT_CCVR_BLK2__ADDR__OFFSET    0x208ULL
#define WDT__WDT_CCVR_BLK2__NUM  0x1

#define WDT__WDT_CCVR_BLK2__WDT_CCVR__SHIFT    0

#define WDT__WDT_CCVR_BLK2__WDT_CCVR__MASK    0xffffffff

#define WDT__WDT_CCVR_BLK2__WDT_CCVR__POR_VALUE    0xfffffff


///////////////////////////////////////////////////////
// Register: WDT_CRR_BLK2
// Counter restart register
///////////////////////////////////////////////////////

#define WDT__WDT_CRR_BLK2__ADDR__OFFSET   0x20CULL
#define WDT__WDT_CRR_BLK2__NUM  0x1

#define WDT__WDT_CRR_BLK2__WDT_CRR__SHIFT    0
#define WDT__WDT_CRR_BLK2__RSVD_WDT_CRR__SHIFT    8

#define WDT__WDT_CRR_BLK2__WDT_CRR__MASK    0x000000ff
#define WDT__WDT_CRR_BLK2__RSVD_WDT_CRR__MASK    0xffffff00

#define WDT__WDT_CRR_BLK2__WDT_CRR__POR_VALUE    0x0
#define WDT__WDT_CRR_BLK2__RSVD_WDT_CRR__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: WDT_STAT_BLK2
// Interrupt status register
///////////////////////////////////////////////////////

#define WDT__WDT_STAT_BLK2__ADDR__OFFSET    0x210ULL
#define WDT__WDT_STAT_BLK2__NUM  0x1

#define WDT__WDT_STAT_BLK2__WDT_STAT__SHIFT    0
#define WDT__WDT_STAT_BLK2__RSVD_WDT_STAT__SHIFT    1

#define WDT__WDT_STAT_BLK2__WDT_STAT__MASK    0x00000001
#define WDT__WDT_STAT_BLK2__RSVD_WDT_STAT__MASK    0xfffffffe

#define WDT__WDT_STAT_BLK2__WDT_STAT__POR_VALUE    0x0
#define WDT__WDT_STAT_BLK2__RSVD_WDT_STAT__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: WDT_EOI_BLK2
// Interrupt clear register
///////////////////////////////////////////////////////

#define WDT__WDT_EOI_BLK2__ADDR__OFFSET   0x214ULL
#define WDT__WDT_EOI_BLK2__NUM  0x1

#define WDT__WDT_EOI_BLK2__WDT_EOI__SHIFT    0
#define WDT__WDT_EOI_BLK2__RSVD_WDT_EOI__SHIFT    1

#define WDT__WDT_EOI_BLK2__WDT_EOI__MASK    0x00000001
#define WDT__WDT_EOI_BLK2__RSVD_WDT_EOI__MASK    0xfffffffe

#define WDT__WDT_EOI_BLK2__WDT_EOI__POR_VALUE    0x0
#define WDT__WDT_EOI_BLK2__RSVD_WDT_EOI__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: WDT_COMP_PARAMS_5_BLK2
// Component Parameters Register 5
///////////////////////////////////////////////////////

#define WDT__WDT_COMP_PARAMS_5_BLK2__ADDR__OFFSET    0x2E4ULL
#define WDT__WDT_COMP_PARAMS_5_BLK2__NUM  0x1

#define WDT__WDT_COMP_PARAMS_5_BLK2__CP_WDT_USER_TOP_MAX__SHIFT    0

#define WDT__WDT_COMP_PARAMS_5_BLK2__CP_WDT_USER_TOP_MAX__MASK    0xffffffff

#define WDT__WDT_COMP_PARAMS_5_BLK2__CP_WDT_USER_TOP_MAX__POR_VALUE    0x7fffffff


///////////////////////////////////////////////////////
// Register: WDT_COMP_PARAMS_4_BLK2
// Component Parameters Register 4
///////////////////////////////////////////////////////

#define WDT__WDT_COMP_PARAMS_4_BLK2__ADDR__OFFSET    0x2E8ULL
#define WDT__WDT_COMP_PARAMS_4_BLK2__NUM  0x1

#define WDT__WDT_COMP_PARAMS_4_BLK2__CP_WDT_USER_TOP_INIT_MAX__SHIFT    0

#define WDT__WDT_COMP_PARAMS_4_BLK2__CP_WDT_USER_TOP_INIT_MAX__MASK    0xffffffff

#define WDT__WDT_COMP_PARAMS_4_BLK2__CP_WDT_USER_TOP_INIT_MAX__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: WDT_COMP_PARAMS_3_BLK2
// Component Parameters Register 3
///////////////////////////////////////////////////////

#define WDT__WDT_COMP_PARAMS_3_BLK2__ADDR__OFFSET    0x2ECULL
#define WDT__WDT_COMP_PARAMS_3_BLK2__NUM  0x1

#define WDT__WDT_COMP_PARAMS_3_BLK2__CD_WDT_TOP_RST__SHIFT    0

#define WDT__WDT_COMP_PARAMS_3_BLK2__CD_WDT_TOP_RST__MASK    0xffffffff

#define WDT__WDT_COMP_PARAMS_3_BLK2__CD_WDT_TOP_RST__POR_VALUE    0xc


///////////////////////////////////////////////////////
// Register: WDT_COMP_PARAMS_2_BLK2
// Component Parameters Register 2
///////////////////////////////////////////////////////

#define WDT__WDT_COMP_PARAMS_2_BLK2__ADDR__OFFSET   0x2F0ULL
#define WDT__WDT_COMP_PARAMS_2_BLK2__NUM  0x1

#define WDT__WDT_COMP_PARAMS_2_BLK2__CP_WDT_CNT_RST__SHIFT    0

#define WDT__WDT_COMP_PARAMS_2_BLK2__CP_WDT_CNT_RST__MASK    0xffffffff

#define WDT__WDT_COMP_PARAMS_2_BLK2__CP_WDT_CNT_RST__POR_VALUE    0xfffffff


///////////////////////////////////////////////////////
// Register: WDT_COMP_PARAMS_1_BLK2
// Component Parameters Register 1
///////////////////////////////////////////////////////

#define WDT__WDT_COMP_PARAMS_1_BLK2__ADDR__OFFSET    0x2F4ULL
#define WDT__WDT_COMP_PARAMS_1_BLK2__NUM  0x1

#define WDT__WDT_COMP_PARAMS_1_BLK2__WDT_ALWAYS_EN__SHIFT    0
#define WDT__WDT_COMP_PARAMS_1_BLK2__WDT_DFLT_RMOD__SHIFT    1
#define WDT__WDT_COMP_PARAMS_1_BLK2__WDT_DUAL_TOP__SHIFT    2
#define WDT__WDT_COMP_PARAMS_1_BLK2__WDT_HC_RMOD__SHIFT    3
#define WDT__WDT_COMP_PARAMS_1_BLK2__WDT_HC_RPL__SHIFT    4
#define WDT__WDT_COMP_PARAMS_1_BLK2__WDT_HC_TOP__SHIFT    5
#define WDT__WDT_COMP_PARAMS_1_BLK2__WDT_USE_FIX_TOP__SHIFT    6
#define WDT__WDT_COMP_PARAMS_1_BLK2__WDT_PAUSE__SHIFT    7
#define WDT__WDT_COMP_PARAMS_1_BLK2__APB_DATA_WIDTH__SHIFT    8
#define WDT__WDT_COMP_PARAMS_1_BLK2__WDT_DFLT_RPL__SHIFT    10
#define WDT
#define WDT__WDT_COMP_PARAMS_1_BLK2__RSVD_15_13__SHIFT    13
#define WDT__WDT_COMP_PARAMS_1_BLK2__WDT_DFLT_TOP__SHIFT    16
#define WDT__WDT_COMP_PARAMS_1_BLK2__WDT_DFLT_TOP_INIT__SHIFT    20
#define WDT__WDT_COMP_PARAMS_1_BLK2__WDT_CNT_WIDTH__SHIFT    24
#define WDT__WDT_COMP_PARAMS_1_BLK2__RSVD_31_29__SHIFT    29

#define WDT__WDT_COMP_PARAMS_1_BLK2__WDT_ALWAYS_EN__MASK    0x00000001
#define WDT__WDT_COMP_PARAMS_1_BLK2__WDT_DFLT_RMOD__MASK    0x00000002
#define WDT__WDT_COMP_PARAMS_1_BLK2__WDT_DUAL_TOP__MASK    0x00000004
#define WDT__WDT_COMP_PARAMS_1_BLK2__WDT_HC_RMOD__MASK    0x00000008
#define WDT__WDT_COMP_PARAMS_1_BLK2__WDT_HC_RPL__MASK    0x00000010
#define WDT__WDT_COMP_PARAMS_1_BLK2__WDT_HC_TOP__MASK    0x00000020
#define WDT__WDT_COMP_PARAMS_1_BLK2__WDT_USE_FIX_TOP__MASK    0x00000040
#define WDT__WDT_COMP_PARAMS_1_BLK2__WDT_PAUSE__MASK    0x00000080
#define WDT__WDT_COMP_PARAMS_1_BLK2__APB_DATA_WIDTH__MASK    0x00000300
#define WDT__WDT_COMP_PARAMS_1_BLK2__WDT_DFLT_RPL__MASK    0x00001c00
#define WDT__WDT_COMP_PARAMS_1_BLK2__RSVD_15_13__MASK    0x0000e000
#define WDT__WDT_COMP_PARAMS_1_BLK2__WDT_DFLT_TOP__MASK    0x000f0000
#define WDT__WDT_COMP_PARAMS_1_BLK2__WDT_DFLT_TOP_INIT__MASK    0x00f00000
#define WDT__WDT_COMP_PARAMS_1_BLK2__WDT_CNT_WIDTH__MASK    0x1f000000
#define WDT__WDT_COMP_PARAMS_1_BLK2__RSVD_31_29__MASK    0xe0000000

#define WDT__WDT_COMP_PARAMS_1_BLK2__WDT_ALWAYS_EN__POR_VALUE    0x1
#define WDT__WDT_COMP_PARAMS_1_BLK2__WDT_DFLT_RMOD__POR_VALUE    0x0
#define WDT__WDT_COMP_PARAMS_1_BLK2__WDT_DUAL_TOP__POR_VALUE    0x0
#define WDT__WDT_COMP_PARAMS_1_BLK2__WDT_HC_RMOD__POR_VALUE    0x0
#define WDT__WDT_COMP_PARAMS_1_BLK2__WDT_HC_RPL__POR_VALUE    0x0
#define WDT__WDT_COMP_PARAMS_1_BLK2__WDT_HC_TOP__POR_VALUE    0x0
#define WDT__WDT_COMP_PARAMS_1_BLK2__WDT_USE_FIX_TOP__POR_VALUE    0x0
#define WDT__WDT_COMP_PARAMS_1_BLK2__WDT_PAUSE__POR_VALUE    0x1
#define WDT__WDT_COMP_PARAMS_1_BLK2__APB_DATA_WIDTH__POR_VALUE    0x2
#define WDT__WDT_COMP_PARAMS_1_BLK2__WDT_DFLT_RPL__POR_VALUE    0x4
#define WDT__WDT_COMP_PARAMS_1_BLK2__RSVD_15_13__POR_VALUE    0x0
#define WDT__WDT_COMP_PARAMS_1_BLK2__WDT_DFLT_TOP__POR_VALUE    0xc
#define WDT__WDT_COMP_PARAMS_1_BLK2__WDT_DFLT_TOP_INIT__POR_VALUE    0x0
#define WDT__WDT_COMP_PARAMS_1_BLK2__WDT_CNT_WIDTH__POR_VALUE    0x10
#define WDT__WDT_COMP_PARAMS_1_BLK2__RSVD_31_29__POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: WDT_COMP_VERSION_BLK2
// Component Version register
///////////////////////////////////////////////////////

#define WDT__WDT_COMP_VERSION_BLK2__ADDR__OFFSET    0x2F8ULL
#define WDT__WDT_COMP_VERSION_BLK2__NUM  0x1

#define WDT__WDT_COMP_VERSION_BLK2__WDT_COMP_VERSION__SHIFT    0

#define WDT__WDT_COMP_VERSION_BLK2__WDT_COMP_VERSION__MASK    0xffffffff

#define WDT__WDT_COMP_VERSION_BLK2__WDT_COMP_VERSION__POR_VALUE    0x3131322a


///////////////////////////////////////////////////////
// Register: WDT_COMP_TYPE_BLK2
// Component Type register
///////////////////////////////////////////////////////

#define WDT__WDT_COMP_TYPE_BLK2__ADDR__OFFSET    0x2FCULL
#define WDT__WDT_COMP_TYPE_BLK2__NUM  0x1

#define WDT__WDT_COMP_TYPE_BLK2__WDT_COMP_TYPE__SHIFT    0

#define WDT__WDT_COMP_TYPE_BLK2__WDT_COMP_TYPE__MASK    0xffffffff

#define WDT__WDT_COMP_TYPE_BLK2__WDT_COMP_TYPE__POR_VALUE    0x44570120

/**************************** Type Definitions ******************************/
typedef struct
{
    AL_U32                  DeviceId;
    AL_U32                  BaseAddress;
    AL_U32                  IntrNum;
    AL_U32                  InputClockHz;
} AL_WDT_HwConfigStruct;

#ifdef __cplusplus
}
#endif

#endif
