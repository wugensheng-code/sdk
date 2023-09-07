/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef AL_PMU_HW_H
#define AL_PMU_HW_H

#ifdef _cplusplus
extern "C" {
#endif

/***************************** Include Files ********************************/

/************************** Constant Definitions ****************************/

/**************************** Type Definitions ******************************/

/***************** Macros (Inline Functions) Definitions ********************/

#define PMU_CTRL_ADDR                   TOP_S_BASE_ADDR

/* PMU ERR REF REGISTER OFFSET */
#define PMU_HW_EN0_SET_ADDR_OFFSET      0x000UL
#define PMU_HW_EN0_CLR_ADDR_OFFSET      0x004UL
#define PMU_HW_EN1_SET_ADDR_OFFSET      0x008UL
#define PMU_HW_EN1_CLR_ADDR_OFFSET      0x00CUL
#define PMU_INT_EN0_SET_ADDR_OFFSET     0x010UL
#define PMU_INT_EN0_CLR_ADDR_OFFSET     0x014UL
#define PMU_INT_EN1_SET_ADDR_OFFSET     0x018UL
#define PMU_INT_EN1_CLR_ADDR_OFFSET     0x01CUL
#define PMU_INT_MSK0_SET_ADDR_OFFSET    0x020UL
#define PMU_INT_MSK0_CLR_ADDR_OFFSET    0x024UL
#define PMU_INT_MSK1_SET_ADDR_OFFSET    0x028UL
#define PMU_INT_MSK1_CLR_ADDR_OFFSET    0x02CUL
#define PMU_INT_CLR0_ADDR_OFFSET        0x030UL
#define PMU_INT_CLR1_ADDR_OFFSET        0x034UL
#define PMU_INT_STATE0_ADDR_OFFSET      0x040UL
#define PMU_INT_STATE1_ADDR_OFFSET      0x044UL
#define PMU_RAW_HIS0_ADDR_OFFSET        0x048UL
#define PMU_RAW_HIS1_ADDR_OFFSET        0x04CUL

/* PMU ERR SHIFT */
#define PMU_DDR_ECC_SHIFT       0
#define PMU_OCM_ECC_SHIFT       1
#define PMU_RPU_FATAL_SHIFT     2
#define PMU_RPU_NORM_SHIFT      3
#define PMU_APU_ECC_SHIFT       4
#define PMU_WDT0_OT_SHIFT       8
#define PMU_WDT1_OT_SHIFT       9
#define PMU_WDT2_OT_SHIFT       10
#define PMU_CLK_DETEC_SHIFT     18
#define PMU_BUS_TIMEOUT_SHIFT   20
#define PMU_CPU_PLL_LOCK_SHIFT  0
#define PMU_IO_PLL_LOCK_SHIFT   1
#define PMU_DDR_PLL_LOCK_SHIFT  2

/* PMU ERR BIT SIZE */
#define PMU_DDR_ECC_SIZE        1
#define PMU_OCM_ECC_SIZE        1
#define PMU_RPU_FATAL_SIZE      1
#define PMU_RPU_NORM_SIZE       1
#define PMU_APU_ECC_SIZE        1
#define PMU_WDT0_OT_SIZE        1
#define PMU_WDT1_OT_SIZE        1
#define PMU_WDT2_OT_SIZE        1
#define PMU_CLK_DETEC_SIZE      1
#define PMU_BUS_TIMEOUT_SIZE    1
#define PMU_CPU_PLL_LOCK_SIZE   1
#define PMU_IO_PLL_LOCK_SIZE    1
#define PMU_DDR_PLL_LOCK_SIZE   1

/* PMU NMI CTRL REGISTER */
#define PMU_NMI_EN_ADDR_OFFSET          0x050UL

#define PMU_NMI_EN_RPU_SHIFT    1

#define PMU_NMI_EN_RPU_SIZE     1

/* PMU SHACK REGISTER OFFSET */
#define PMU_SW_PMU_SHACK_ADDR_OFFSET    0x080UL

#define PMU_SW_PMU_SHACK_PROC_DONE_SHIFT    0
#define PMU_SW_PMU_SHACK_FIQ_ACK_SHIFT      1
#define PMU_SW_PMU_SHACK_HDRSTCNT_CLR_SHIFT 2

#define PMU_SW_PMU_SHACK_PROC_DONE_SIZE     1
#define PMU_SW_PMU_SHACK_FIQ_ACK_SIZE       1
#define PMU_SW_PMU_SHACK_HDRSTCNT_CLR_SIZE  1

/* PMU FSM REGISTER OFFSET */
#define PMU_PMU_STAT_ADDR_OFFSET        0x084UL

#define PMU_PMU_STAT_PMU_FSM_SHIFT  0

#define PMU_PMU_STAT_PMU_FSM_SIZE   2

#ifdef _cplusplus
}
#endif

#endif
