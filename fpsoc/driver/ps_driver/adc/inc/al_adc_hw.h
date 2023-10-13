/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef __AL_ADC_HW_H_
#define __AL_ADC_HW_H_

#ifdef _cplusplus
extern "C" {
#endif

/***************************** Include Files ********************************/
#include "al_core.h"

/************************** Constant Definitions ****************************/

/**************************** Type Definitions ******************************/

/***************** Macros (Inline Functions Definitions ********************/

/* ADC GP0 define interface */
#define PLS_PROT (0xF8800080ULL)

#define GP0_BASE_ADDR (0x80004000ULL)

#define G0_ADC_VC0           (GP0_BASE_ADDR+0x40)
#define G0_ADC_VC1           (GP0_BASE_ADDR+0x42)
#define G0_ADC_VC2           (GP0_BASE_ADDR+0x44)
#define G0_ADC_VC3           (GP0_BASE_ADDR+0x46)
#define G0_ADC_VC4           (GP0_BASE_ADDR+0x48)
#define G0_ADC_VC5           (GP0_BASE_ADDR+0x4a)
#define G0_ADC_VC6           (GP0_BASE_ADDR+0x4c)
#define G0_ADC_VC7           (GP0_BASE_ADDR+0x4e)

#define G0_ADC_VER0          (GP0_BASE_ADDR+0x50)
#define G0_ADC_VER1          (GP0_BASE_ADDR+0x52)

#define G0_ADC_FLAG          (GP0_BASE_ADDR+0x5e)

#define G0_ADC_CHANNEL10     (GP0_BASE_ADDR+0x60)
#define G0_ADC_CHANNEL32     (GP0_BASE_ADDR+0x62)
#define G0_ADC_CHANNEL54     (GP0_BASE_ADDR+0x64)
#define G0_ADC_CHANNEL76     (GP0_BASE_ADDR+0x66)

#define G0_ADC_CONFIG0       (GP0_BASE_ADDR+0x68)
#define G0_ADC_CONFIG1       (GP0_BASE_ADDR+0x6a)
#define G0_ADC_CONFIG2       (GP0_BASE_ADDR+0x6c)
#define G0_ADC_CONFIG3       (GP0_BASE_ADDR+0x6e)

#define G0_ADC_CH0_LTH       (GP0_BASE_ADDR+0x80)
#define G0_ADC_CH1_LTH       (GP0_BASE_ADDR+0x82)
#define G0_ADC_CH2_LTH       (GP0_BASE_ADDR+0x84)
#define G0_ADC_CH3_LTH       (GP0_BASE_ADDR+0x86)
#define G0_ADC_CH4_LTH       (GP0_BASE_ADDR+0x88)
#define G0_ADC_CH5_LTH       (GP0_BASE_ADDR+0x8a)
#define G0_ADC_CH6_LTH       (GP0_BASE_ADDR+0x8c)
#define G0_ADC_CH7_LTH       (GP0_BASE_ADDR+0x8e)

#define G0_ADC_CH0_GTH       (GP0_BASE_ADDR+0x90)
#define G0_ADC_CH1_GTH       (GP0_BASE_ADDR+0x92)
#define G0_ADC_CH2_GTH       (GP0_BASE_ADDR+0x94)
#define G0_ADC_CH3_GTH       (GP0_BASE_ADDR+0x96)
#define G0_ADC_CH4_GTH       (GP0_BASE_ADDR+0x98)
#define G0_ADC_CH5_GTH       (GP0_BASE_ADDR+0x9a)
#define G0_ADC_CH6_GTH       (GP0_BASE_ADDR+0x9c)
#define G0_ADC_CH7_GTH       (GP0_BASE_ADDR+0x9e)


///////////////////////////////////////////////////////
// Register: ADC_CFG_R
//
///////////////////////////////////////////////////////

#define ADC_CFG_OFFSET 0x0UL
#define ADC_CFG_NUM  0x1

#define ADC_CFG_IGAP_SHIFT    0
#define ADC_CFG_READ_IGAP_SHIFT    5
#define ADC_CFG_TCK_RATE_SHIFT    10
#define ADC_CFG_DATA_FIFO_LVL_L_SHIFT    12
#define ADC_CFG_CMD_FIFO_LVL_L_SHIFT    16
#define ADC_CFG_DATA_FIFO_LVL_H_SHIFT    20
#define ADC_CFG_CMD_FIFO_LVL_H_SHIFT    24
#define ADC_CFG_RESERVED_28_SHIFT    28
#define ADC_CFG_JTAG_REDGE_SHIFT    29
#define ADC_CFG_JTAG_WEDGE_SHIFT    30
#define ADC_CFG_ENABLE_SHIFT    31

#define ADC_CFG_IGAP_SIZE 5
#define ADC_CFG_READ_IGAP_SIZE 5
#define ADC_CFG_TCK_RATE_SIZE 2
#define ADC_CFG_DATA_FIFO_LVL_L_SIZE 4
#define ADC_CFG_CMD_FIFO_LVL_L_SIZE 4
#define ADC_CFG_DATA_FIFO_LVL_H_SIZE 4
#define ADC_CFG_CMD_FIFO_LVL_H_SIZE 4
#define ADC_CFG_RESERVED_28_SIZE 1
#define ADC_CFG_JTAG_REDGE_SIZE 1
#define ADC_CFG_JTAG_WEDGE_SIZE 1
#define ADC_CFG_ENABLE_SIZE 1

#define ADC_CFG_R_IGAP_MASK    0x0000001f
#define ADC_CFG_R_READ_IGAP_MASK    0x000003e0
#define ADC_CFG_R_TCK_RATE_MASK    0x00000c00
#define ADC_CFG_R_DATA_FIFO_LVL_L_MASK    0x0000f000
#define ADC_CFG_R_CMD_FIFO_LVL_L_MASK    0x000f0000
#define ADC_CFG_R_DATA_FIFO_LVL_H_MASK    0x00f00000
#define ADC_CFG_R_CMD_FIFO_LVL_H_MASK    0x0f000000
#define ADC_CFG_R_RESERVED_28_MASK    0x10000000
#define ADC_CFG_R_JTAG_REDGE_MASK    0x20000000
#define ADC_CFG_R_JTAG_WEDGE_MASK    0x40000000
#define ADC_CFG_R_ENABLE_MASK    0x80000000


///////////////////////////////////////////////////////
// Register: ADC_INTR_R
//
///////////////////////////////////////////////////////

#define ADC_INTR_OFFSET 0x4UL
#define ADC_INTR_NUM  0x1

#define ADC_INTR_INTR_SHIFT    0
#define ADC_INTR_RESERVED_7_1_SHIFT    1
#define ADC_INTR_DATA_FIFO_LTH_SHIFT    8
#define ADC_INTR_CMD_FIFO_LTH_SHIFT    9
#define ADC_INTR_DATA_FIFO_GTH_SHIFT    10
#define ADC_INTR_CMD_FIFO_GTH_SHIFT    11
#define ADC_INTR_RESERVED_31_12_SHIFT    12

#define ADC_INTR_INTR_SIZE 1
#define ADC_INTR_RESERVED_7_1_SIZE 7
#define ADC_INTR_DATA_FIFO_LTH_SIZE 1
#define ADC_INTR_CMD_FIFO_LTH_SIZE 1
#define ADC_INTR_DATA_FIFO_GTH_SIZE 1
#define ADC_INTR_CMD_FIFO_GTH_SIZE 1
#define ADC_INTR_RESERVED_31_12_SIZE 20

#define ADC_INTR_R_INTR_MASK    0x00000001
#define ADC_INTR_R_RESERVED_7_1_MASK    0x000000fe
#define ADC_INTR_R_DATA_FIFO_LTH_MASK    0x00000100
#define ADC_INTR_R_CMD_FIFO_LTH_MASK    0x00000200
#define ADC_INTR_R_DATA_FIFO_GTH_MASK    0x00000400
#define ADC_INTR_R_CMD_FIFO_GTH_MASK    0x00000800
#define ADC_INTR_R_RESERVED_31_12_MASK    0xfffff000


///////////////////////////////////////////////////////
// Register: ADC_INTR_MASK_R
//
///////////////////////////////////////////////////////

#define ADC_INTR_MASK_OFFSET 0x8UL
#define ADC_INTR_MASK_NUM  0x1

#define ADC_INTR_MASK_M_INTR_SHIFT    0
#define ADC_INTR_MASK_RESERVED_7_1_SHIFT    1
#define ADC_INTR_MASK_M_DATA_FIFO_LTH_SHIFT    8
#define ADC_INTR_MASK_M_CMD_FIFO_LTH_SHIFT    9
#define ADC_INTR_MASK_M_DATA_FIFO_GTH_SHIFT    10
#define ADC_INTR_MASK_M_CMD_FIFO_GTH_SHIFT    11
#define ADC_INTR_MASK_RESERVED_31_12_SHIFT    12

#define ADC_INTR_MASK_M_INTR_SIZE 1
#define ADC_INTR_MASK_RESERVED_7_1_SIZE 7
#define ADC_INTR_MASK_M_DATA_FIFO_LTH_SIZE 1
#define ADC_INTR_MASK_M_CMD_FIFO_LTH_SIZE 1
#define ADC_INTR_MASK_M_DATA_FIFO_GTH_SIZE 1
#define ADC_INTR_MASK_M_CMD_FIFO_GTH_SIZE 1
#define ADC_INTR_MASK_RESERVED_31_12_SIZE 20

#define ADC_INTR_MASK_R_M_INTR_MASK    0x00000001
#define ADC_INTR_MASK_R_RESERVED_7_1_MASK    0x000000fe
#define ADC_INTR_MASK_R_M_DATA_FIFO_LTH_MASK    0x00000100
#define ADC_INTR_MASK_R_M_CMD_FIFO_LTH_MASK    0x00000200
#define ADC_INTR_MASK_R_M_DAYA_FIFO_GTH_MASK    0x00000400
#define ADC_INTR_MASK_R_M_CMD_FIFO_GTH_MASK    0x00000800
#define ADC_INTR_MASK_R_RESERVED_31_12_MASK    0xfffff000


///////////////////////////////////////////////////////
// Register: ADC_MSTS_R
//
///////////////////////////////////////////////////////

#define ADC_MSTS_OFFSET 0xCUL
#define ADC_MSTS_NUM  0x1

#define ADC_MSTS_INTR_SHIFT    0
#define ADC_MSTS_RESERVED_7_1_SHIFT    1
#define ADC_MSTS_DATA_FIFO_E_SHIFT    8
#define ADC_MSTS_DATA_FIFO_F_SHIFT    9
#define ADC_MSTS_CMD_FIFO_E_SHIFT    10
#define ADC_MSTS_CMD_FIFO_F_SHIFT    11
#define ADC_MSTS_DATA_FIFO_LVL_SHIFT    12
#define ADC_MSTS_CMD_FIFO_LVL_SHIFT    16
#define ADC_MSTS_RESERVED_31_12_SHIFT    20

#define ADC_MSTS_INTR_SIZE 1
#define ADC_MSTS_RESERVED_7_1_SIZE 7
#define ADC_MSTS_DATA_FIFO_E_SIZE 1
#define ADC_MSTS_DATA_FIFO_F_SIZE 1
#define ADC_MSTS_CMD_FIFO_E_SIZE 1
#define ADC_MSTS_CMD_FIFO_F_SIZE 1
#define ADC_MSTS_DATA_FIFO_LVL_SIZE 4
#define ADC_MSTS_CMD_FIFO_LVL_SIZE 4
#define ADC_MSTS_RESERVED_31_12_SIZE 12

#define ADC_MSTS_R_INTR_MASK    0x00000001
#define ADC_MSTS_R_RESERVED_7_1_MASK    0x000000fe
#define ADC_MSTS_R_DATA_FIFO_E_MASK    0x00000100
#define ADC_MSTS_R_DATA_FIFO_F_MASK    0x00000200
#define ADC_MSTS_R_CMD_FIFO_E_MASK    0x00000400
#define ADC_MSTS_R_CMD_FIFO_F_MASK    0x00000800
#define ADC_MSTS_R_DATA_FIFO_LVL_MASK    0x0000f000
#define ADC_MSTS_R_CMD_FIFO_LVL_MASK    0x000f0000
#define ADC_MSTS_R_RESERVED_31_12_MASK    0xfff00000


///////////////////////////////////////////////////////
// Register: ADC_CMD_FIFO_R
//
///////////////////////////////////////////////////////

#define ADC_CMD_FIFO_OFFSET 0x10UL
#define ADC_CMD_FIFO_NUM  0x1

#define ADC_CMD_FIFO_CMD_SHIFT    0

#define ADC_CMD_FIFO_CMD_SIZE 32

#define ADC_CMD_FIFO_R_CMD_MASK    0xffffffff


///////////////////////////////////////////////////////
// Register: ADC_DATA_FIFO_R
//
///////////////////////////////////////////////////////

#define ADC_DATA_FIFO_OFFSET 0x14UL
#define ADC_DATA_FIFO_NUM  0x1

#define ADC_DATA_FIFO_DATA_SHIFT    0

#define ADC_DATA_FIFO_DATA_SIZE 32

#define ADC_DATA_FIFO_R_DATA_MASK    0xffffffff


///////////////////////////////////////////////////////
// Register: ADC_MCTRL_R
//
///////////////////////////////////////////////////////

#define ADC_MCTRL_OFFSET 0x18UL
#define ADC_MCTRL_NUM  0x1

#define ADC_MCTRL_CMD_FIFO_FLUSH_SHIFT    0
#define ADC_MCTRL_DATA_FIFO_FLUSH_SHIFT    1
#define ADC_MCTRL_RESERVED_3_2_SHIFT    2
#define ADC_MCTRL_RESET_SHIFT    4
#define ADC_MCTRL_RESERVED_31_5_SHIFT    5

#define ADC_MCTRL_CMD_FIFO_FLUSH_SIZE 1
#define ADC_MCTRL_DATA_FIFO_FLUSH_SIZE 1
#define ADC_MCTRL_RESERVED_3_2_SIZE 2
#define ADC_MCTRL_RESET_SIZE 1
#define ADC_MCTRL_RESERVED_31_5_SIZE 27

#define ADC_MCTRL_R_CMD_FIFO_FLUSH_MASK    0x00000001
#define ADC_MCTRL_R_DATA_FIFO_FLUSH_MASK    0x00000002
#define ADC_MCTRL_R_RESERVED_3_2_MASK    0x0000000c
#define ADC_MCTRL_R_RESET_MASK    0x00000010
#define ADC_MCTRL_R_RESERVED_31_5_MASK    0xffffffe0


///////////////////////////////////////////////////////
// Register: ADC_VC0
//
///////////////////////////////////////////////////////

#define ADC_VC0_OFFSET 0x40UL
#define ADC_VC0_NUM  0x1

#define ADC_VC0_RESERVED_3_0_SHIFT    0
#define ADC_VC0_VC0_SHIFT    4

#define ADC_VC0_RESERVED_3_0_SIZE 4
#define ADC_VC0_VC0_SIZE 12

#define ADC_VC0_RESERVED_3_0_MASK    0x000f
#define ADC_VC0_ADC_VC0_MASK    0xfff0


///////////////////////////////////////////////////////
// Register: ADC_VC1
//
///////////////////////////////////////////////////////

#define ADC_VC1_OFFSET 0x42UL
#define ADC_VC1_NUM  0x1

#define ADC_VC1_RESERVED_3_0_SHIFT    0
#define ADC_VC1_VC1_SHIFT    4

#define ADC_VC1_RESERVED_3_0_SIZE 4
#define ADC_VC1_VC1_SIZE 12

#define ADC_VC1_RESERVED_3_0_MASK    0x000f
#define ADC_VC1_ADC_VC1_MASK    0xfff0


///////////////////////////////////////////////////////
// Register: ADC_VC2
//
///////////////////////////////////////////////////////

#define ADC_VC2_OFFSET 0x44UL
#define ADC_VC2_NUM  0x1

#define ADC_VC2_RESERVED_3_0_SHIFT    0
#define ADC_VC2_VC2_SHIFT    4

#define ADC_VC2_RESERVED_3_0_SIZE 4
#define ADC_VC2_VC2_SIZE 12

#define ADC_VC2_RESERVED_3_0_MASK    0x000f
#define ADC_VC2_ADC_VC2_MASK    0xfff0


///////////////////////////////////////////////////////
// Register: ADC_VC3
//
///////////////////////////////////////////////////////

#define ADC_VC3_OFFSET 0x46UL
#define ADC_VC3_NUM  0x1

#define ADC_VC3_RESERVED_3_0_SHIFT    0
#define ADC_VC3_VC3_SHIFT    4

#define ADC_VC3_RESERVED_3_0_SIZE 4
#define ADC_VC3_VC3_SIZE 12

#define ADC_VC3_RESERVED_3_0_MASK    0x000f
#define ADC_VC3_ADC_VC3_MASK    0xfff0


///////////////////////////////////////////////////////
// Register: ADC_VC4
//
///////////////////////////////////////////////////////

#define ADC_VC4_OFFSET 0x48UL
#define ADC_VC4_NUM  0x1

#define ADC_VC4_RESERVED_3_0_SHIFT    0
#define ADC_VC4_VC4_SHIFT    4

#define ADC_VC4_RESERVED_3_0_SIZE 4
#define ADC_VC4_VC4_SIZE 12

#define ADC_VC4_RESERVED_3_0_MASK    0x000f
#define ADC_VC4_ADC_VC4_MASK    0xfff0


///////////////////////////////////////////////////////
// Register: ADC_VC5
//
///////////////////////////////////////////////////////

#define ADC_VC5_OFFSET 0x4AUL
#define ADC_VC5_NUM  0x1

#define ADC_VC5_RESERVED_3_0_SHIFT    0
#define ADC_VC5_VC5_SHIFT    4

#define ADC_VC5_RESERVED_3_0_SIZE 4
#define ADC_VC5_VC5_SIZE 12

#define ADC_VC5_RESERVED_3_0_MASK    0x000f
#define ADC_VC5_ADC_VC5_MASK    0xfff0


///////////////////////////////////////////////////////
// Register: ADC_VC6
//
///////////////////////////////////////////////////////

#define ADC_VC6_OFFSET 0x4CUL
#define ADC_VC6_NUM  0x1

#define ADC_VC6_RESERVED_3_0_SHIFT    0
#define ADC_VC6_VC6_SHIFT    4

#define ADC_VC6_RESERVED_3_0_SIZE 4
#define ADC_VC6_VC6_SIZE 12

#define ADC_VC6_RESERVED_3_0_MASK    0x000f
#define ADC_VC6_ADC_VC6_MASK    0xfff0


///////////////////////////////////////////////////////
// Register: ADC_VC7
//
///////////////////////////////////////////////////////

#define ADC_VC7_OFFSET 0x4EUL
#define ADC_VC7_NUM  0x1

#define ADC_VC7_RESERVED_3_0_SHIFT    0
#define ADC_VC7_VC7_SHIFT    4

#define ADC_VC7_RESERVED_3_0_SIZE 4
#define ADC_VC7_VC7_SIZE 12

#define ADC_VC7_RESERVED_3_0_MASK    0x000f
#define ADC_VC7_ADC_VC7_MASK    0xfff0


///////////////////////////////////////////////////////
// Register: ADC_VER0
//
///////////////////////////////////////////////////////

#define ADC_VER0_OFFSET 0x50UL
#define ADC_VER0_NUM  0x1

#define ADC_VER0_VERSION0_SHIFT    0

#define ADC_VER0_VERSION0_SIZE 16

#define ADC_VER0_VERSION0_MASK    0xffff


///////////////////////////////////////////////////////
// Register: ADC_VER1
//
///////////////////////////////////////////////////////

#define ADC_VER1_OFFSET 0x52UL
#define ADC_VER1_NUM  0x1

#define ADC_VER1_VERSION1_SHIFT    0

#define ADC_VER1_VERSION1_SIZE 16

#define ADC_VER1_VERSION1_MASK    0xffff


///////////////////////////////////////////////////////
// Register: ADC_FLAG
//
///////////////////////////////////////////////////////

#define ADC_FLAG_OFFSET 0x5EUL
#define ADC_FLAG_NUM  0x1

#define ADC_FLAG_FLAG_DONE_SHIFT    0
#define ADC_FLAG_FLAG_GTH_SHIFT    1
#define ADC_FLAG_FLAG_LTH_SHIFT    2
#define ADC_FLAG_FLAG_ERROR_SHIFT    3
#define ADC_FLAG_RESERVED_7_4_SHIFT    4
#define ADC_FLAG_FLAG_CO_VC0_SHIFT    8
#define ADC_FLAG_FLAG_CO_VC1_SHIFT    9
#define ADC_FLAG_FLAG_CO_VC2_SHIFT    10
#define ADC_FLAG_FLAG_CO_VC3_SHIFT    11
#define ADC_FLAG_FLAG_CO_VC4_SHIFT    12
#define ADC_FLAG_FLAG_CO_VC5_SHIFT    13
#define ADC_FLAG_FLAG_CO_VC6_SHIFT    14
#define ADC_FLAG_FLAG_CO_VC7_SHIFT    15

#define ADC_FLAG_FLAG_DONE_SIZE 1
#define ADC_FLAG_FLAG_GTH_SIZE 1
#define ADC_FLAG_FLAG_LTH_SIZE 1
#define ADC_FLAG_FLAG_ERROR_SIZE 1
#define ADC_FLAG_RESERVED_7_4_SIZE 4
#define ADC_FLAG_FLAG_CO_VC0_SIZE 1
#define ADC_FLAG_FLAG_CO_VC1_SIZE 1
#define ADC_FLAG_FLAG_CO_VC2_SIZE 1
#define ADC_FLAG_FLAG_CO_VC3_SIZE 1
#define ADC_FLAG_FLAG_CO_VC4_SIZE 1
#define ADC_FLAG_FLAG_CO_VC5_SIZE 1
#define ADC_FLAG_FLAG_CO_VC6_SIZE 1
#define ADC_FLAG_FLAG_CO_VC7_SIZE 1

#define ADC_FLAG_FLAG_DONE_MASK    0x0001
#define ADC_FLAG_FLAG_GTH_MASK    0x0002
#define ADC_FLAG_FLAG_LTH_MASK    0x0004
#define ADC_FLAG_FLAG_ERROR_MASK    0x0008
#define ADC_FLAG_RESERVED_7_4_MASK    0x00f0
#define ADC_FLAG_FLAG_CO_VC0_MASK    0x0100
#define ADC_FLAG_FLAG_CO_VC1_MASK    0x0200
#define ADC_FLAG_FLAG_CO_VC2_MASK    0x0400
#define ADC_FLAG_FLAG_CO_VC3_MASK    0x0800
#define ADC_FLAG_FLAG_CO_VC4_MASK    0x1000
#define ADC_FLAG_FLAG_CO_VC5_MASK    0x2000
#define ADC_FLAG_FLAG_CO_VC6_MASK    0x4000
#define ADC_FLAG_FLAG_CO_VC7_MASK    0x8000


///////////////////////////////////////////////////////
// Register: ADC_CHANNEL_0_1
//
///////////////////////////////////////////////////////

#define ADC_CHANNEL_0_1_OFFSET 0x60UL
#define ADC_CHANNEL_0_1_NUM  0x1

#define ADC_CHANNEL_0_1_CHANNEL0_SEL_SHIFT    0
#define ADC_CHANNEL_0_1_RESERVED_7_5_SHIFT    5
#define ADC_CHANNEL_0_1_CHANNEL1_SEL_SHIFT    8
#define ADC_CHANNEL_0_1_RESERVED_15_13_SHIFT    13

#define ADC_CHANNEL_0_1_CHANNEL0_SEL_SIZE 5
#define ADC_CHANNEL_0_1_RESERVED_7_5_SIZE 3
#define ADC_CHANNEL_0_1_CHANNEL1_SEL_SIZE 5
#define ADC_CHANNEL_0_1_RESERVED_15_13_SIZE 3

#define ADC_CHANNEL_0_1_ADC_CHANNEL0_SEL_MASK    0x001f
#define ADC_CHANNEL_0_1_RESERVED_7_5_MASK    0x00e0
#define ADC_CHANNEL_0_1_ADC_CHANNEL1_SEL_MASK    0x1f00
#define ADC_CHANNEL_0_1_RESERVED_15_13_MASK    0xe000


///////////////////////////////////////////////////////
// Register: ADC_CHANNEL_2_3
//
///////////////////////////////////////////////////////

#define ADC_CHANNEL_2_3_OFFSET 0x62UL
#define ADC_CHANNEL_2_3_NUM  0x1

#define ADC_CHANNEL_2_3_CHANNEL2_SEL_SHIFT    0
#define ADC_CHANNEL_2_3_RESERVED_7_5_SHIFT    5
#define ADC_CHANNEL_2_3_CHANNEL3_SEL_SHIFT    8
#define ADC_CHANNEL_2_3_RESERVED_15_13_SHIFT    13

#define ADC_CHANNEL_2_3_CHANNEL2_SEL_SIZE 5
#define ADC_CHANNEL_2_3_RESERVED_7_5_SIZE 3
#define ADC_CHANNEL_2_3_CHANNEL3_SEL_SIZE 5
#define ADC_CHANNEL_2_3_RESERVED_15_13_SIZE 3

#define ADC_CHANNEL_2_3_ADC_CHANNEL2_SEL_MASK    0x001f
#define ADC_CHANNEL_2_3_RESERVED_7_5_MASK    0x00e0
#define ADC_CHANNEL_2_3_ADC_CHANNEL3_SEL_MASK    0x1f00
#define ADC_CHANNEL_2_3_RESERVED_15_13_MASK    0xe000


///////////////////////////////////////////////////////
// Register: ADC_CHANNEL_4_5
//
///////////////////////////////////////////////////////

#define ADC_CHANNEL_4_5_OFFSET 0x64UL
#define ADC_CHANNEL_4_5_NUM  0x1

#define ADC_CHANNEL_4_5_CHANNEL4_SEL_SHIFT    0
#define ADC_CHANNEL_4_5_RESERVED_7_5_SHIFT    5
#define ADC_CHANNEL_4_5_CHANNEL5_SEL_SHIFT    8
#define ADC_CHANNEL_4_5_RESERVED_15_13_SHIFT    13

#define ADC_CHANNEL_4_5_CHANNEL4_SEL_SIZE 5
#define ADC_CHANNEL_4_5_RESERVED_7_5_SIZE 3
#define ADC_CHANNEL_4_5_CHANNEL5_SEL_SIZE 5
#define ADC_CHANNEL_4_5_RESERVED_15_13_SIZE 3

#define ADC_CHANNEL_4_5_ADC_CHANNEL4_SEL_MASK    0x001f
#define ADC_CHANNEL_4_5_RESERVED_7_5_MASK    0x00e0
#define ADC_CHANNEL_4_5_ADC_CHANNEL5_SEL_MASK    0x1f00
#define ADC_CHANNEL_4_5_RESERVED_15_13_MASK    0xe000


///////////////////////////////////////////////////////
// Register: ADC_CHANNEL_6_7
//
///////////////////////////////////////////////////////

#define ADC_CHANNEL_6_7_OFFSET 0x66UL
#define ADC_CHANNEL_6_7_NUM  0x1

#define ADC_CHANNEL_6_7_CHANNEL6_SEL_SHIFT    0
#define ADC_CHANNEL_6_7_RESERVED_7_5_SHIFT    5
#define ADC_CHANNEL_6_7_CHANNEL7_SEL_SHIFT    8
#define ADC_CHANNEL_6_7_RESERVED_15_13_SHIFT    13

#define ADC_CHANNEL_6_7_CHANNEL6_SEL_SIZE 5
#define ADC_CHANNEL_6_7_RESERVED_7_5_SIZE 3
#define ADC_CHANNEL_6_7_CHANNEL7_SEL_SIZE 5
#define ADC_CHANNEL_6_7_RESERVED_15_13_SIZE 3

#define ADC_CHANNEL_6_7_ADC_CHANNEL6_SEL_MASK    0x001f
#define ADC_CHANNEL_6_7_RESERVED_7_5_MASK    0x00e0
#define ADC_CHANNEL_6_7_ADC_CHANNEL7_SEL_MASK    0x1f00
#define ADC_CHANNEL_6_7_RESERVED_15_13_MASK    0xe000


///////////////////////////////////////////////////////
// Register: ADC_CONFIG0
//
///////////////////////////////////////////////////////

#define ADC_CONFIG0_OFFSET 0x68UL
#define ADC_CONFIG0_NUM  0x1

#define ADC_CONFIG0_SW_RESET_SHIFT    0
#define ADC_CONFIG0_ENCTR_SHIFT    1
#define ADC_CONFIG0_RESERVED_4_SHIFT    4
#define ADC_CONFIG0_REF_SEL_SHIFT    5
#define ADC_CONFIG0_RES_SEL_SHIFT    6
#define ADC_CONFIG0_DIFF_ENABLE_SHIFT    8
#define ADC_CONFIG0_PHY_EXTERNAL_MUX_SHIFT    9
#define ADC_CONFIG0_ANALOG_MUX_EN_SHIFT    10
#define ADC_CONFIG0_REG_SOC_SEL_SHIFT    11
#define ADC_CONFIG0_PHY_EXTERNAL_MUX_GAP_SHIFT    12

#define ADC_CONFIG0_SW_RESET_SIZE 1
#define ADC_CONFIG0_ENCTR_SIZE 3
#define ADC_CONFIG0_RESERVED_4_SIZE 1
#define ADC_CONFIG0_REF_SEL_SIZE 1
#define ADC_CONFIG0_RES_SEL_SIZE 2
#define ADC_CONFIG0_DIFF_ENABLE_SIZE 1
#define ADC_CONFIG0_PHY_EXTERNAL_MUX_SIZE 1
#define ADC_CONFIG0_ANALOG_MUX_EN_SIZE 1
#define ADC_CONFIG0_REG_SOC_SEL_SIZE 1
#define ADC_CONFIG0_PHY_EXTERNAL_MUX_GAP_SIZE 4

#define ADC_CONFIG0_ADC_SW_RESET_MASK    0x0001
#define ADC_CONFIG0_ENCTR_MASK    0x000e
#define ADC_CONFIG0_RESERVED_4_MASK    0x0010
#define ADC_CONFIG0_REF_SEL_MASK    0x0020
#define ADC_CONFIG0_RES_SEL_MASK    0x00c0
#define ADC_CONFIG0_DIFF_ENABLE_MASK    0x0100
#define ADC_CONFIG0_PHY_EXTERNAL_MUX_MASK    0x0200
#define ADC_CONFIG0_ANALOG_MUX_EN_MASK    0x0400
#define ADC_CONFIG0_REG_ADC_SOC_SEL_MASK    0x0800
#define ADC_CONFIG0_PHY_EXTERNAL_MUX_GAP_MASK    0xf000


///////////////////////////////////////////////////////
// Register: ADC_CONFIG1
//
///////////////////////////////////////////////////////

#define ADC_CONFIG1_OFFSET 0x6AUL
#define ADC_CONFIG1_NUM  0x1

#define ADC_CONFIG1_INTR_DONE_MASK_SHIFT    0
#define ADC_CONFIG1_INTR_GTH_MASK_SHIFT    1
#define ADC_CONFIG1_INTR_LTH_MASK_SHIFT    2
#define ADC_CONFIG1_INTR_ERROR_MASK_SHIFT    3
#define ADC_CONFIG1_RESERVED_4_SHIFT    4
#define ADC_CONFIG1_INTR_DONE_SHIFT    8
#define ADC_CONFIG1_INTR_GTH_SHIFT    9
#define ADC_CONFIG1_INTR_LTH_SHIFT    10
#define ADC_CONFIG1_INTR_ERROR_SHIFT    11
#define ADC_CONFIG1_RESERVED_15_12_SHIFT    12

#define ADC_CONFIG1_INTR_DONE_MASK_SIZE 1
#define ADC_CONFIG1_INTR_GTH_MASK_SIZE 1
#define ADC_CONFIG1_INTR_LTH_MASK_SIZE 1
#define ADC_CONFIG1_INTR_ERROR_MASK_SIZE 1
#define ADC_CONFIG1_RESERVED_4_SIZE 4
#define ADC_CONFIG1_INTR_DONE_SIZE 1
#define ADC_CONFIG1_INTR_GTH_SIZE 1
#define ADC_CONFIG1_INTR_LTH_SIZE 1
#define ADC_CONFIG1_INTR_ERROR_SIZE 1
#define ADC_CONFIG1_RESERVED_15_12_SIZE 4

#define ADC_CONFIG1_INTR_DONE_MASK_MASK    0x0001
#define ADC_CONFIG1_INTR_GTH_MASK_MASK    0x0002
#define ADC_CONFIG1_INTR_LTH_MASK_MASK    0x0004
#define ADC_CONFIG1_INTR_ERROR_MASK_MASK    0x0008
#define ADC_CONFIG1_RESERVED_4_MASK    0x00f0
#define ADC_CONFIG1_INTR_DONE_MASK    0x0100
#define ADC_CONFIG1_INTR_GTH_MASK    0x0200
#define ADC_CONFIG1_INTR_LTH_MASK    0x0400
#define ADC_CONFIG1_INTR_ERROR_MASK    0x0800
#define ADC_CONFIG1_RESERVED_15_12_MASK    0xf000


///////////////////////////////////////////////////////
// Register: ADC_CONFIG2
//
///////////////////////////////////////////////////////

#define ADC_CONFIG2_OFFSET 0x6CUL
#define ADC_CONFIG2_NUM  0x1

#define ADC_CONFIG2_REG_ENABLE_SHIFT    0
#define ADC_CONFIG2_REG_DISLVL_SHIFT    1
#define ADC_CONFIG2_RESERVED_3_2_SHIFT    2
#define ADC_CONFIG2_CLK_SEL_SHIFT    4
#define ADC_CONFIG2_CLK_GATE_SHIFT    6
#define ADC_CONFIG2_RESERVED_7_SHIFT    7
#define ADC_CONFIG2_CLK_DIV_SHIFT    8

#define ADC_CONFIG2_REG_ENABLE_SIZE 1
#define ADC_CONFIG2_REG_DISLVL_SIZE 1
#define ADC_CONFIG2_RESERVED_3_2_SIZE 2
#define ADC_CONFIG2_CLK_SEL_SIZE 2
#define ADC_CONFIG2_CLK_GATE_SIZE 1
#define ADC_CONFIG2_RESERVED_7_SIZE 1
#define ADC_CONFIG2_CLK_DIV_SIZE 8

#define ADC_CONFIG2_REG_ADC_ENABLE_MASK    0x0001
#define ADC_CONFIG2_REG_ADC_DISLVL_MASK    0x0002
#define ADC_CONFIG2_RESERVED_3_2_MASK    0x000c
#define ADC_CONFIG2_ADC_CLK_SEL_MASK    0x0030
#define ADC_CONFIG2_ADC_CLK_GATE_MASK    0x0040
#define ADC_CONFIG2_RESERVED_7_MASK    0x0080
#define ADC_CONFIG2_ADC_CLK_DIV_MASK    0xff00


///////////////////////////////////////////////////////
// Register: ADC_CONFIG3
//
///////////////////////////////////////////////////////

#define ADC_CONFIG3_OFFSET 0x6EUL
#define ADC_CONFIG3_NUM  0x1

#define ADC_CONFIG3_MODE_SEL_SHIFT    0
#define ADC_CONFIG3_RESERVED_3_SHIFT    3
#define ADC_CONFIG3_CHANNEL_SEL_SHIFT    4
#define ADC_CONFIG3_RESERVED_7_SHIFT    7
#define ADC_CONFIG3_EXTERNAL_CHANNEL_SEL_SHIFT    8
#define ADC_CONFIG3_RESERVED_11_SHIFT    11
#define ADC_CONFIG3_REG_SOC_SHIFT    12
#define ADC_CONFIG3_RESERVED_15_13_SHIFT    13

#define ADC_CONFIG3_MODE_SEL_SIZE 3
#define ADC_CONFIG3_RESERVED_3_SIZE 1
#define ADC_CONFIG3_CHANNEL_SEL_SIZE 3
#define ADC_CONFIG3_RESERVED_7_SIZE 1
#define ADC_CONFIG3_EXTERNAL_CHANNEL_SEL_SIZE 3
#define ADC_CONFIG3_RESERVED_11_SIZE 1
#define ADC_CONFIG3_REG_SOC_SIZE 1
#define ADC_CONFIG3_RESERVED_15_13_SIZE 3

#define ADC_CONFIG3_MODE_SEL_MASK    0x0007
#define ADC_CONFIG3_RESERVED_3_MASK    0x0008
#define ADC_CONFIG3_CHANNEL_SEL_MASK    0x0070
#define ADC_CONFIG3_RESERVED_7_MASK    0x0080
#define ADC_CONFIG3_EXTERNAL_CHANNEL_SEL_MASK    0x0700
#define ADC_CONFIG3_RESERVED_11_MASK    0x0800
#define ADC_CONFIG3_REG_ADC_SOC_MASK    0x1000
#define ADC_CONFIG3_RESERVED_15_13_MASK    0xe000


///////////////////////////////////////////////////////
// Register: ADC_TEST0
//
///////////////////////////////////////////////////////

#define ADC_TEST0_OFFSET 0x70UL
#define ADC_TEST0_NUM  0x1

#define ADC_TEST0_RESERVED_SHIFT    0

#define ADC_TEST0_RESERVED_SIZE 16


///////////////////////////////////////////////////////
// Register: ADC_TEST1
//
///////////////////////////////////////////////////////

#define ADC_TEST1_OFFSET 0x72UL
#define ADC_TEST1_NUM  0x1

#define ADC_TEST1_RESERVED_SHIFT    0

#define ADC_TEST1_RESERVED_SIZE 16


///////////////////////////////////////////////////////
// Register: ADC_TEST2
//
///////////////////////////////////////////////////////

#define ADC_TEST2_OFFSET 0x74UL
#define ADC_TEST2_NUM  0x1

#define ADC_TEST2_RESERVED_SHIFT    0

#define ADC_TEST2_RESERVED_SIZE 16


///////////////////////////////////////////////////////
// Register: ADC_TEST3
//
///////////////////////////////////////////////////////

#define ADC_TEST3_OFFSET 0x76UL
#define ADC_TEST3_NUM  0x1

#define ADC_TEST3_RESERVED_SHIFT    0

#define ADC_TEST3_RESERVED_SIZE 16


///////////////////////////////////////////////////////
// Register: ADC_CH0_LTH
//
///////////////////////////////////////////////////////

#define ADC_CH0_LTH_OFFSET 0x80UL
#define ADC_CH0_LTH_NUM  0x1

#define ADC_CH0_LTH_RESERVED_3_0_SHIFT    0
#define ADC_CH0_LTH_CH0_LTH_SHIFT    4

#define ADC_CH0_LTH_RESERVED_3_0_SIZE 4
#define ADC_CH0_LTH_CH0_LTH_SIZE 12

#define ADC_CH0_LTH_RESERVED_3_0_MASK    0x000f
#define ADC_CH0_LTH_CH0_LTH_MASK    0xfff0


///////////////////////////////////////////////////////
// Register: ADC_CH1_LTH
//
///////////////////////////////////////////////////////

#define ADC_CH1_LTH_OFFSET 0x82UL
#define ADC_CH1_LTH_NUM  0x1

#define ADC_CH1_LTH_RESERVED_3_0_SHIFT    0
#define ADC_CH1_LTH_CH1_LTH_SHIFT    4

#define ADC_CH1_LTH_RESERVED_3_0_SIZE 4
#define ADC_CH1_LTH_CH1_LTH_SIZE 12

#define ADC_CH1_LTH_RESERVED_3_0_MASK    0x000f
#define ADC_CH1_LTH_CH1_LTH_MASK    0xfff0


///////////////////////////////////////////////////////
// Register: ADC_CH2_LTH
//
///////////////////////////////////////////////////////

#define ADC_CH2_LTH_OFFSET 0x84UL
#define ADC_CH2_LTH_NUM  0x1

#define ADC_CH2_LTH_RESERVED_3_0_SHIFT    0
#define ADC_CH2_LTH_CH2_LTH_SHIFT    4

#define ADC_CH2_LTH_RESERVED_3_0_SIZE 4
#define ADC_CH2_LTH_CH2_LTH_SIZE 12

#define ADC_CH2_LTH_RESERVED_3_0_MASK    0x000f
#define ADC_CH2_LTH_CH2_LTH_MASK    0xfff0


///////////////////////////////////////////////////////
// Register: ADC_CH3_LTH
//
///////////////////////////////////////////////////////

#define ADC_CH3_LTH_OFFSET 0x86UL
#define ADC_CH3_LTH_NUM  0x1

#define ADC_CH3_LTH_RESERVED_3_0_SHIFT    0
#define ADC_CH3_LTH_CH3_LTH_SHIFT    4

#define ADC_CH3_LTH_RESERVED_3_0_SIZE 4
#define ADC_CH3_LTH_CH3_LTH_SIZE 12

#define ADC_CH3_LTH_RESERVED_3_0_MASK    0x000f
#define ADC_CH3_LTH_CH3_LTH_MASK    0xfff0


///////////////////////////////////////////////////////
// Register: ADC_CH4_LTH
//
///////////////////////////////////////////////////////

#define ADC_CH4_LTH_OFFSET 0x88UL
#define ADC_CH4_LTH_NUM  0x1

#define ADC_CH4_LTH_RESERVED_3_0_SHIFT    0
#define ADC_CH4_LTH_CH4_LTH_SHIFT    4

#define ADC_CH4_LTH_RESERVED_3_0_SIZE 4
#define ADC_CH4_LTH_CH4_LTH_SIZE 12

#define ADC_CH4_LTH_RESERVED_3_0_MASK    0x000f
#define ADC_CH4_LTH_CH4_LTH_MASK    0xfff0


///////////////////////////////////////////////////////
// Register: ADC_CH5_LTH
//
///////////////////////////////////////////////////////

#define ADC_CH5_LTH_OFFSET 0x8AUL
#define ADC_CH5_LTH_NUM  0x1

#define ADC_CH5_LTH_RESERVED_3_0_SHIFT    0
#define ADC_CH5_LTH_CH5_LTH_SHIFT    4

#define ADC_CH5_LTH_RESERVED_3_0_SIZE 4
#define ADC_CH5_LTH_CH5_LTH_SIZE 12

#define ADC_CH5_LTH_RESERVED_3_0_MASK    0x000f
#define ADC_CH5_LTH_CH5_LTH_MASK    0xfff0


///////////////////////////////////////////////////////
// Register: ADC_CH6_LTH
//
///////////////////////////////////////////////////////

#define ADC_CH6_LTH_OFFSET 0x8CUL
#define ADC_CH6_LTH_NUM  0x1

#define ADC_CH6_LTH_RESERVED_3_0_SHIFT    0
#define ADC_CH6_LTH_CH6_LTH_SHIFT    4

#define ADC_CH6_LTH_RESERVED_3_0_SIZE 4
#define ADC_CH6_LTH_CH6_LTH_SIZE 12

#define ADC_CH6_LTH_RESERVED_3_0_MASK    0x000f
#define ADC_CH6_LTH_CH6_LTH_MASK    0xfff0


///////////////////////////////////////////////////////
// Register: ADC_CH7_LTH
//
///////////////////////////////////////////////////////

#define ADC_CH7_LTH_OFFSET 0x8EUL
#define ADC_CH7_LTH_NUM  0x1

#define ADC_CH7_LTH_RESERVED_3_0_SHIFT    0
#define ADC_CH7_LTH_CH7_LTH_SHIFT    4

#define ADC_CH7_LTH_RESERVED_3_0_SIZE 4
#define ADC_CH7_LTH_CH7_LTH_SIZE 12

#define ADC_CH7_LTH_RESERVED_3_0_MASK    0x000f
#define ADC_CH7_LTH_CH7_LTH_MASK    0xfff0


///////////////////////////////////////////////////////
// Register: ADC_CH0_GTH
//
///////////////////////////////////////////////////////

#define ADC_CH0_GTH_OFFSET 0x90UL
#define ADC_CH0_GTH_NUM  0x1

#define ADC_CH0_GTH_RESERVED_3_0_SHIFT    0
#define ADC_CH0_GTH_CH0_GTH_SHIFT    4

#define ADC_CH0_GTH_RESERVED_3_0_SIZE 4
#define ADC_CH0_GTH_CH0_GTH_SIZE 12

#define ADC_CH0_GTH_RESERVED_3_0_MASK    0x000f
#define ADC_CH0_GTH_CH0_GTH_MASK    0xfff0


///////////////////////////////////////////////////////
// Register: ADC_CH1_GTH
//
///////////////////////////////////////////////////////

#define ADC_CH1_GTH_OFFSET 0x92UL
#define ADC_CH1_GTH_NUM  0x1

#define ADC_CH1_GTH_RESERVED_3_0_SHIFT    0
#define ADC_CH1_GTH_CH1_GTH_SHIFT    4

#define ADC_CH1_GTH_RESERVED_3_0_SIZE 4
#define ADC_CH1_GTH_CH1_GTH_SIZE 12

#define ADC_CH1_GTH_RESERVED_3_0_MASK    0x000f
#define ADC_CH1_GTH_CH1_GTH_MASK    0xfff0


///////////////////////////////////////////////////////
// Register: ADC_CH2_GTH
//
///////////////////////////////////////////////////////

#define ADC_CH2_GTH_OFFSET 0x94UL
#define ADC_CH2_GTH_NUM  0x1

#define ADC_CH2_GTH_RESERVED_3_0_SHIFT    0
#define ADC_CH2_GTH_CH2_GTH_SHIFT    4

#define ADC_CH2_GTH_RESERVED_3_0_SIZE 4
#define ADC_CH2_GTH_CH2_GTH_SIZE 12

#define ADC_CH2_GTH_RESERVED_3_0_MASK    0x000f
#define ADC_CH2_GTH_CH2_GTH_MASK    0xfff0


///////////////////////////////////////////////////////
// Register: ADC_CH3_GTH
//
///////////////////////////////////////////////////////

#define ADC_CH3_GTH_OFFSET 0x96UL
#define ADC_CH3_GTH_NUM  0x1

#define ADC_CH3_GTH_RESERVED_3_0_SHIFT    0
#define ADC_CH3_GTH_CH3_GTH_SHIFT    4

#define ADC_CH3_GTH_RESERVED_3_0_SIZE 4
#define ADC_CH3_GTH_CH3_GTH_SIZE 12

#define ADC_CH3_GTH_RESERVED_3_0_MASK    0x000f
#define ADC_CH3_GTH_CH3_GTH_MASK    0xfff0


///////////////////////////////////////////////////////
// Register: ADC_CH4_GTH
//
///////////////////////////////////////////////////////

#define ADC_CH4_GTH_OFFSET 0x98UL
#define ADC_CH4_GTH_NUM  0x1

#define ADC_CH4_GTH_RESERVED_3_0_SHIFT    0
#define ADC_CH4_GTH_CH4_GTH_SHIFT    4

#define ADC_CH4_GTH_RESERVED_3_0_SIZE 4
#define ADC_CH4_GTH_CH4_GTH_SIZE 12

#define ADC_CH4_GTH_RESERVED_3_0_MASK    0x000f
#define ADC_CH4_GTH_CH4_GTH_MASK    0xfff0


///////////////////////////////////////////////////////
// Register: ADC_CH5_GTH
//
///////////////////////////////////////////////////////

#define ADC_CH5_GTH_OFFSET 0x9AUL
#define ADC_CH5_GTH_NUM  0x1

#define ADC_CH5_GTH_RESERVED_3_0_SHIFT    0
#define ADC_CH5_GTH_CH5_GTH_SHIFT    4

#define ADC_CH5_GTH_RESERVED_3_0_SIZE 4
#define ADC_CH5_GTH_CH5_GTH_SIZE 12

#define ADC_CH5_GTH_RESERVED_3_0_MASK    0x000f
#define ADC_CH5_GTH_CH5_GTH_MASK    0xfff0


///////////////////////////////////////////////////////
// Register: ADC_CH6_GTH
//
///////////////////////////////////////////////////////

#define ADC_CH6_GTH_OFFSET 0x9CUL
#define ADC_CH6_GTH_NUM  0x1

#define ADC_CH6_GTH_RESERVED_3_0_SHIFT    0
#define ADC_CH6_GTH_CH6_GTH_SHIFT    4

#define ADC_CH6_GTH_RESERVED_3_0_SIZE 4
#define ADC_CH6_GTH_CH6_GTH_SIZE 12

#define ADC_CH6_GTH_RESERVED_3_0_MASK    0x000f
#define ADC_CH6_GTH_CH6_GTH_MASK    0xfff0


///////////////////////////////////////////////////////
// Register: ADC_CH7_GTH
//
///////////////////////////////////////////////////////

#define ADC_CH7_GTH_OFFSET 0x9EUL
#define ADC_CH7_GTH_NUM  0x1

#define ADC_CH7_GTH_RESERVED_3_0_SHIFT    0
#define ADC_CH7_GTH_CH7_GTH_SHIFT    4

#define ADC_CH7_GTH_RESERVED_3_0_SIZE 4
#define ADC_CH7_GTH_CH7_GTH_SIZE 12

#define ADC_CH7_GTH_RESERVED_3_0_MASK    0x000f
#define ADC_CH7_GTH_CH7_GTH_MASK    0xfff0


/**************************** Type Definitions ******************************/
typedef struct
{
    AL_U32                  DeviceId;
    AL_U32                  AdcBaseAddress;
    AL_U32                  GpBaseAddress;
    AL_U32                  IntrNum;
} AL_ADC_HwConfigStruct;

#ifdef _cplusplus
}
#endif

#endif
