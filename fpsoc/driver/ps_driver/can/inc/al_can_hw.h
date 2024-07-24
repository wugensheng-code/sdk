/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_CAN_HW_H_
#define __AL_CAN_HW_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files ********************************/
#include "al_core.h"

/************************** Constant Definitions ****************************/

/**************************** Type Definitions ******************************/

/***************** Macros (Inline Functions) Definitions ********************/

///////////////////////////////////////////////////////
// Register: CAN_TOP_CFG_CTRL_ADDR
// Top Nonsecure Config Contrl
///////////////////////////////////////////////////////
#define CAN_TOP_CFG_CTRL_ADDR_CAN0_FD_ENABLE_SHIFT    0
#define CAN_TOP_CFG_CTRL_ADDR_CAN0_DMA_ENABLE_SHIFT   1
#define CAN_TOP_CFG_CTRL_ADDR_CAN1_FD_ENABLE_SHIFT    8
#define CAN_TOP_CFG_CTRL_ADDR_CAN1_DMA_ENABLE_SHIFT   9

#define CAN_TOP_CFG_CTRL_ADDR_CAN0_FD_ENABLE_SIZE     1
#define CAN_TOP_CFG_CTRL_ADDR_CAN0_DMA_ENABLE_SIZE    1
#define CAN_TOP_CFG_CTRL_ADDR_CAN1_FD_ENABLE_SIZE     1
#define CAN_TOP_CFG_CTRL_ADDR_CAN1_DMA_ENABLE_SIZE    1

///////////////////////////////////////////////////////
// Register: RBUF_0_3
// Receive Buffer Registers
///////////////////////////////////////////////////////
#define CAN_RBUF_0_3_OFFSET     0x00UL
#define CAN_RBUF_0_3_NUM        0x1

#define CAN_RBUF_0_3_ID_7_0_SHIFT       0
#define CAN_RBUF_0_3_ID_15_8_SHIFT      8
#define CAN_RBUF_0_3_ID_23_16_SHIFT     16
#define CAN_RBUF_0_3_ID_28_24_SHIFT     24
#define CAN_RBUF_0_3_RSDV_29_30_SHIFT   29
#define CAN_RBUF_0_3_ESI_SHIFT          31

#define CAN_RBUF_0_3_ID_7_0_SIZE        8
#define CAN_RBUF_0_3_ID_15_8_SIZE       8
#define CAN_RBUF_0_3_ID_23_16_SIZE      8
#define CAN_RBUF_0_3_ID_28_24_SIZE      5
#define CAN_RBUF_0_3_RSDV_29_30_SIZE    2
#define CAN_RBUF_0_3_ESI_SIZE           1

#define CAN_RBUF_0_3_ID_EXT_SIZE        29
#define CAN_RBUF_0_3_ID_STD_SIZE        11

///////////////////////////////////////////////////////
// Register: RBUF_4_7
// TTCAN Stauts and control register
///////////////////////////////////////////////////////
#define CAN_RBUF_4_7_OFFSET     0x04UL
#define CAN_RBUF_4_7_NUM        0x1

#define CAN_RBUF_4_7_DLC_SHIFT          0
#define CAN_RBUF_4_7_BRS_SHIFT          4
#define CAN_RBUF_4_7_FDF_SHIFT          5
#define CAN_RBUF_4_7_RTR_SHIFT          6
#define CAN_RBUF_4_7_IDE_SHIFT          7
#define CAN_RBUF_4_7_RSDV_8_11_SHIFT    8
#define CAN_RBUF_4_7_TX_SHIFT           12
#define CAN_RBUF_4_7_KOER_SHIFT         13
#define CAN_RBUF_4_7_CYCLE_TIME_SHIFT   16

#define CAN_RBUF_4_7_DLC_SIZE           4
#define CAN_RBUF_4_7_BRS_SIZE           1
#define CAN_RBUF_4_7_FDF_SIZE           1
#define CAN_RBUF_4_7_RTR_SIZE           1
#define CAN_RBUF_4_7_IDE_SIZE           1
#define CAN_RBUF_4_7_RSDV_8_11_SIZE     4
#define CAN_RBUF_4_7_TX_SIZE            1
#define CAN_RBUF_4_7_KOER_SIZE          3
#define CAN_RBUF_4_7_CYCLE_TIME_SIZE    16

///////////////////////////////////////////////////////
// Register: RBUF_8_11
// Data register
///////////////////////////////////////////////////////
#define CAN_RBUF_8_11_OFFSET    0x08UL
#define CAN_RBUF_8_11_NUM       0x1

#define CAN_RBUF_8_11_D1_D4_SHIFT   0

#define CAN_RBUF_8_11_D1_D4_SIZE    32

///////////////////////////////////////////////////////
// Register: RBUF_12_15
// Data register
///////////////////////////////////////////////////////
#define CAN_RBUF_12_15_OFFSET   0x0CUL
#define CAN_RBUF_12_15_NUM      0x1

#define CAN_RBUF_12_15_D5_D8_SHIFT  0

#define CAN_RBUF_12_15_D5_D8_SIZE   32

///////////////////////////////////////////////////////
// Register: RBUF_16_19
// Data register
///////////////////////////////////////////////////////
#define CAN_RBUF_16_19_OFFSET   0x10UL
#define CAN_RBUF_16_19_NUM      0x1

#define CAN_RBUF_16_19_D9_D12_SHIFT        0

#define CAN_RBUF_16_19_D9_D12_SIZE         32

///////////////////////////////////////////////////////
// Register: RBUF_20_23
// Data register
///////////////////////////////////////////////////////
#define CAN_RBUF_20_23_OFFSET   0x14UL
#define CAN_RBUF_20_23_NUM      0x1

#define CAN_RBUF_20_23_D13_D16_SHIFT    0

#define CAN_RBUF_20_23_D13_D16_SIZE     32

///////////////////////////////////////////////////////
// Register: RBUF_24_27
// Data register
///////////////////////////////////////////////////////
#define CAN_RBUF_24_27_OFFSET   0x18UL
#define CAN_RBUF_24_27_NUM      0x1

#define CAN_RBUF_24_27_D17_D20_SHIFT    0

#define CAN_RBUF_24_27_D17_D20_SIZE     32

///////////////////////////////////////////////////////
// Register: RBUF_28_31
// Data register
///////////////////////////////////////////////////////
#define CAN_RBUF_28_31_OFFSET   0x1CUL
#define CAN_RBUF_28_31_NUM      0x1

#define CAN_RBUF_28_31_D21_D24_SHIFT    0

#define CAN_RBUF_28_31_D21_D24_SIZE     32

///////////////////////////////////////////////////////
// Register: RBUF_32_35
// Data register
///////////////////////////////////////////////////////
#define CAN_RBUF_32_35_OFFSET   0x20UL
#define CAN_RBUF_32_35_NUM      0x1

#define CAN_RBUF_32_35_D25_D28_SHIFT    0

#define CAN_RBUF_32_35_D25_D28_SIZE     32

///////////////////////////////////////////////////////
// Register: RBUF_36_39
// Data register
///////////////////////////////////////////////////////
#define CAN_RBUF_36_39_OFFSET   0x24UL
#define CAN_RBUF_36_39_NUM      0x1

#define CAN_RBUF_36_39_D29_D32_SHIFT    0

#define CAN_RBUF_36_39_D29_D32_SIZE     32

///////////////////////////////////////////////////////
// Register: RBUF_40_43
// Data register
///////////////////////////////////////////////////////
#define CAN_RBUF_40_43_OFFSET   0x28UL
#define CAN_RBUF_40_43_NUM      0x1

#define CAN_RBUF_40_43_D33_D36_SHIFT    0

#define CAN_RBUF_40_43_D33_D36_SIZE     32

///////////////////////////////////////////////////////
// Register: RBUF_44_47
// Data register
///////////////////////////////////////////////////////
#define CAN_RBUF_44_47_OFFSET   0x2CUL
#define CAN_RBUF_44_47_NUM      0x1

#define CAN_RBUF_44_47_D37_D40_SHIFT    0

#define CAN_RBUF_44_47_D37_D40_SIZE     32

///////////////////////////////////////////////////////
// Register: RBUF_48_51
// Data register
///////////////////////////////////////////////////////
#define CAN_RBUF_48_51_OFFSET   0x30UL
#define CAN_RBUF_48_51_NUM      0x1

#define CAN_RBUF_48_51_D41_D44_SHIFT    0

#define CAN_RBUF_48_51_D41_D44_SIZE     32

///////////////////////////////////////////////////////
// Register: RBUF_52_55
// Data register
///////////////////////////////////////////////////////
#define CAN_RBUF_52_55_OFFSET   0x34UL
#define CAN_RBUF_52_55_NUM      0x1

#define CAN_RBUF_52_55_D45_D48_SHIFT    0

#define CAN_RBUF_52_55_D45_D48_SIZE     32

///////////////////////////////////////////////////////
// Register: RBUF_56_59
// Data register
///////////////////////////////////////////////////////
#define CAN_RBUF_56_59_OFFSET   0x38UL
#define CAN_RBUF_56_59_NUM      0x1

#define CAN_RBUF_56_59_D49_D52_SHIFT    0

#define CAN_RBUF_56_59_D49_D52_SIZE     32

///////////////////////////////////////////////////////
// Register: RBUF_60_63
// Data register
///////////////////////////////////////////////////////
#define CAN_RBUF_60_63_OFFSET   0x3CUL
#define CAN_RBUF_60_63_NUM      0x1

#define CAN_RBUF_60_63_D53_D56_SHIFT    0

#define CAN_RBUF_60_63_D53_D56_SIZE     32

///////////////////////////////////////////////////////
// Register: RBUF_64_67
// Data register
///////////////////////////////////////////////////////
#define CAN_RBUF_64_67_OFFSET   0x40UL
#define CAN_RBUF_64_67_NUM      0x1

#define CAN_RBUF_64_67_D57_D60_SHIFT    0

#define CAN_RBUF_64_67_D57_D60_SIZE     32

///////////////////////////////////////////////////////
// Register: RBUF_68_71
// Data register
///////////////////////////////////////////////////////
#define CAN_RBUF_68_71_OFFSET   0x44UL
#define CAN_RBUF_68_71_NUM      0x1

#define CAN_RBUF_68_71_D61_D64_SHIFT    0

#define CAN_RBUF_68_71_D61_D64_SIZE     32

///////////////////////////////////////////////////////
// Register: RBUF_72_75
// The Reception Time Stamp
// s (RTS) for CiA 603 time
///////////////////////////////////////////////////////
#define CAN_RBUF_72_75_OFFSET   0x48UL
#define CAN_RBUF_72_75_NUM      0x1

#define CAN_RBUF_72_75_RTS_31_0_SHIFT   0

#define CAN_RBUF_72_75_RTS_31_0_SIZE    32

///////////////////////////////////////////////////////
// Register: RBUF_76_79
// The Reception Time Stamp
// s (RTS) for CiA 603 time
///////////////////////////////////////////////////////
#define CAN_RBUF_76_79_OFFSET   0x4CUL
#define CAN_RBUF_76_79_NUM      0x1

#define CAN_RBUF_76_79_RTS_63_32_SHIFT  0

#define CAN_RBUF_76_79_RTS_63_32_SIZE   32

///////////////////////////////////////////////////////
// Register: TBUF_0_3
// Transmit Buffer Registers
///////////////////////////////////////////////////////
#define CAN_TBUF_0_3_OFFSET     0x50UL
#define CAN_TBUF_0_3_NUM        0x1

#define CAN_TBUF_0_3_ID_7_0_SHIFT       0
#define CAN_TBUF_0_3_ID_15_8_SHIFT      8
#define CAN_TBUF_0_3_ID_23_16_SHIFT     16
#define CAN_TBUF_0_3_ID_28_24_SHIFT     24
#define CAN_TBUF_0_3_RSDV_29_30_SHIFT   29
#define CAN_TBUF_0_3_TTSEN_SHIFT        31

#define CAN_TBUF_0_3_ID_7_0_SIZE        8
#define CAN_TBUF_0_3_ID_15_8_SIZE       8
#define CAN_TBUF_0_3_ID_23_16_SIZE      8
#define CAN_TBUF_0_3_ID_28_24_SIZE      5
#define CAN_TBUF_0_3_RSDV_29_30_SIZE    2
#define CAN_TBUF_0_3_TTSEN_SIZE         1

///////////////////////////////////////////////////////
// Register: TBUF_4_7
// Transmit Buffer Registers
///////////////////////////////////////////////////////
#define CAN_TBUF_4_7_OFFSET     0x54UL
#define CAN_TBUF_4_7_NUM        0x1

#define CAN_TBUF_4_7_DLC_SHIFT          0
#define CAN_TBUF_4_7_BRS_SHIFT          4
#define CAN_TBUF_4_7_FDF_SHIFT          5
#define CAN_TBUF_4_7_RTR_SHIFT          6
#define CAN_TBUF_4_7_IDE_SHIFT          7
#define CAN_TBUF_4_7_RSDV_8_31_SHIFT    8

#define CAN_TBUF_4_7_DLC_SIZE           4
#define CAN_TBUF_4_7_BRS_SIZE           1
#define CAN_TBUF_4_7_FDF_SIZE           1
#define CAN_TBUF_4_7_RTR_SIZE           1
#define CAN_TBUF_4_7_IDE_SIZE           1
#define CAN_TBUF_4_7_RSDV_8_31_SIZE     24

///////////////////////////////////////////////////////
// Register: TBUF_8_11
// Data register
///////////////////////////////////////////////////////
#define CAN_TBUF_8_11_OFFSET    0x58UL
#define CAN_TBUF_8_11_NUM       0x1

#define CAN_TBUF_8_11_D1_D4_SHIFT   0

#define CAN_TBUF_8_11_D1_D4_SIZE    32

///////////////////////////////////////////////////////
// Register: TBUF_12_15
// Data register
///////////////////////////////////////////////////////
#define CAN_TBUF_12_15_OFFSET   0x5CUL
#define CAN_TBUF_12_15_NUM      0x1

#define CAN_TBUF_12_15_D5_D8_SHIFT  0

#define CAN_TBUF_12_15_D5_D8_SIZE   32

///////////////////////////////////////////////////////
// Register: TBUF_16_19
// Data register
///////////////////////////////////////////////////////
#define CAN_TBUF_16_19_OFFSET   0x60UL
#define CAN_TBUF_16_19_NUM      0x1

#define CAN_TBUF_16_19_D9_D12_SHIFT     0

#define CAN_TBUF_16_19_D9_D12_SIZE      32

///////////////////////////////////////////////////////
// Register: TBUF_20_23
// Data register
///////////////////////////////////////////////////////
#define CAN_TBUF_20_23_OFFSET   0x64UL
#define CAN_TBUF_20_23_NUM      0x1

#define CAN_TBUF_20_23_D13_D16_SHIFT    0

#define CAN_TBUF_20_23_D13_D16_SIZE     32

///////////////////////////////////////////////////////
// Register: TBUF_24_27
// Data register
///////////////////////////////////////////////////////
#define CAN_TBUF_24_27_OFFSET   0x68UL
#define CAN_TBUF_24_27_NUM      0x1

#define CAN_TBUF_24_27_D17_D20_SHIFT    0

#define CAN_TBUF_24_27_D17_D20_SIZE     32

///////////////////////////////////////////////////////
// Register: TBUF_28_31
// Data register
///////////////////////////////////////////////////////
#define CAN_TBUF_28_31_OFFSET   0x6CUL
#define CAN_TBUF_28_31_NUM      0x1

#define CAN_TBUF_28_31_D21_D24_SHIFT    0

#define CAN_TBUF_28_31_D21_D24_SIZE     32

///////////////////////////////////////////////////////
// Register: TBUF_32_35
// Data register
///////////////////////////////////////////////////////
#define CAN_TBUF_32_35_OFFSET   0x70UL
#define CAN_TBUF_32_35_NUM      0x1

#define CAN_TBUF_32_35_D25_D28_SHIFT    0

#define CAN_TBUF_32_35_D25_D28_SIZE     32

///////////////////////////////////////////////////////
// Register: TBUF_36_39
// Data register
///////////////////////////////////////////////////////
#define CAN_TBUF_36_39_OFFSET   0x74UL
#define CAN_TBUF_36_39_NUM      0x1

#define CAN_TBUF_36_39_D29_D32_SHIFT    0

#define CAN_TBUF_36_39_D29_D32_SIZE     32

///////////////////////////////////////////////////////
// Register: TBUF_40_43
// Data register
///////////////////////////////////////////////////////
#define CAN_TBUF_40_43_OFFSET   0x78UL
#define CAN_TBUF_40_43_NUM      0x1

#define CAN_TBUF_40_43_D33_D36_SHIFT    0

#define CAN_TBUF_40_43_D33_D36_SIZE     32

///////////////////////////////////////////////////////
// Register: TBUF_44_47
// Data register
///////////////////////////////////////////////////////
#define CAN_TBUF_44_47_OFFSET   0x7CUL
#define CAN_TBUF_44_47_NUM      0x1

#define CAN_TBUF_44_47_D37_D40_SHIFT    0

#define CAN_TBUF_44_47_D37_D40_SIZE     32

///////////////////////////////////////////////////////
// Register: TBUF_48_51
// Data register
///////////////////////////////////////////////////////
#define CAN_TBUF_48_51_OFFSET   0x80UL
#define CAN_TBUF_48_51_NUM      0x1

#define CAN_TBUF_48_51_D41_D44_SHIFT    0

#define CAN_TBUF_48_51_D41_D44_SIZE     32

///////////////////////////////////////////////////////
// Register: TBUF_52_55
// Data register
///////////////////////////////////////////////////////
#define CAN_TBUF_52_55_OFFSET     0x84UL
#define CAN_TBUF_52_55_NUM        0x1

#define CAN_TBUF_52_55_D45_D48_SHIFT    0

#define CAN_TBUF_52_55_D45_D48_SIZE     32

///////////////////////////////////////////////////////
// Register: TBUF_56_59
// Data register
///////////////////////////////////////////////////////
#define CAN_TBUF_56_59_OFFSET   0x88UL
#define CAN_TBUF_56_59_NUM      0x1

#define CAN_TBUF_56_59_D49_D52_SHIFT    0

#define CAN_TBUF_56_59_D49_D52_SIZE     32

///////////////////////////////////////////////////////
// Register: TBUF_60_63
// Data register
///////////////////////////////////////////////////////
#define CAN_TBUF_60_63_OFFSET   0x8CUL
#define CAN_TBUF_60_63_NUM      0x1

#define CAN_TBUF_60_63_D53_D56_SHIFT    0

#define CAN_TBUF_60_63_D53_D56_SIZE     32

///////////////////////////////////////////////////////
// Register: TBUF_64_67
// Data register
///////////////////////////////////////////////////////
#define CAN_TBUF_64_67_OFFSET   0x90UL
#define CAN_TBUF_64_67_NUM      0x1

#define CAN_TBUF_64_67_D57_D60_SHIFT    0

#define CAN_TBUF_64_67_D57_D60_SIZE     32

///////////////////////////////////////////////////////
// Register: TBUF_68_71
// Data register
///////////////////////////////////////////////////////
#define CAN_TBUF_68_71_OFFSET   0x94UL
#define CAN_TBUF_68_71_NUM      0x1

#define CAN_TBUF_68_71_D61_D64_SHIFT    0

#define CAN_TBUF_68_71_D61_D64_SIZE     32

///////////////////////////////////////////////////////
// Register: TTS
// Transmission Time Stamp
///////////////////////////////////////////////////////
#define CAN_TTS_OFFSET      0x98UL
#define CAN_TTS_NUM         0x2

#define CAN_TTS_TTS_SHIFT   0

#define CAN_TTS_TTS_SIZE    32

///////////////////////////////////////////////////////
// Register: RCTRL_TCTRL_TCMD_CFG_STAT
// (Receive Control Register RCTRL)
// (Transmit Control Register)
// (Command Register)
// (Configuration and stuatus register)
///////////////////////////////////////////////////////
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_OFFSET    0xA0UL
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_NUM       0x1

#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_BUSOFF_SHIFT      0
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_RSDV_1_SHIFT      1
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_RSDV_2_SHIFT      2
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_TSSS_SHIFT        3
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_TPSS_SHIFT        4
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_LBMI_SHIFT        5
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_LBME_SHIFT        6
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_RESET_SHIFT       7
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_TSA_SHIFT         8
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_TSALL_SHIFT       9
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_TSONE_SHIFT       10
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_TPA_SHIFT         11
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_TPE_SHIFT         12
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_STBY_SHIFT        13
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_LOM_SHIFT         14
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_TBSEL_SHIFT       15
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_TSSTAT_SHIFT      16
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_RSDV_18_19_SHIFT  18
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_TTTBM_SHIFT       20
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_TSMODE_SHIFT      21
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_TSNEXT_SHIFT      22
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_FD_ISO_SHIFT      23
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_RSTAT_SHIFT       24
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_RSDV_26_SHIFT     26
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_RBALL_SHIFT       27
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_RREL_SHIFT        28
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_ROV_SHIFT         29
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_ROM_SHIFT         30
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_SACK_SHIFT        31

#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_BUSOFF_SIZE       1
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_RSDV_1_SIZE       1
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_RSDV_2_SIZE       1
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_TSSS_SIZE         1
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_TPSS_SIZE         1
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_LBMI_SIZE         1
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_LBME_SIZE         1
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_RESET_SIZE        1
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_TSA_SIZE          1
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_TSALL_SIZE        1
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_TSONE_SIZE        1
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_TPA_SIZE          1
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_TPE_SIZE          1
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_STBY_SIZE         1
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_LOM_SIZE          1
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_TBSEL_SIZE        1
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_TSSTAT_SIZE       2
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_RSDV_18_19_SIZE   2
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_TTTBM_SIZE        1
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_TSMODE_SIZE       1
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_TSNEXT_SIZE       1
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_FD_ISO_SIZE       1
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_RSTAT_SIZE        2
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_RSDV_26_SIZE      1
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_RBALL_SIZE        1
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_RREL_SIZE         1
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_ROV_SIZE          1
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_ROM_SIZE          1
#define CAN_RCTRL_TCTRL_TCMD_CFG_STAT_SACK_SIZE         1

///////////////////////////////////////////////////////
// Register: LIMIT_ERRINT_RTIF_RTIE
// (Warning Limits Register)
// (ERRor INTerrupt Enable and Flag Register )
// (Receive and Transmit Interrupt Flag Register RTIF)
// (Receive and Transmit Interrupt Enable Register)
///////////////////////////////////////////////////////
#define CAN_LIMIT_ERRINT_RTIF_RTIE_OFFSET   0xA4UL
#define CAN_LIMIT_ERRINT_RTIF_RTIE_NUM      0x1

#define CAN_LIMIT_ERRINT_RTIF_RTIE_TSFF_SHIFT       0
#define CAN_LIMIT_ERRINT_RTIF_RTIE_EIE_SHIFT        1
#define CAN_LIMIT_ERRINT_RTIF_RTIE_TSIE_SHIFT       2
#define CAN_LIMIT_ERRINT_RTIF_RTIE_TPIE_SHIFT       3
#define CAN_LIMIT_ERRINT_RTIF_RTIE_RAFIE_SHIFT      4
#define CAN_LIMIT_ERRINT_RTIF_RTIE_RFIE_SHIFT       5
#define CAN_LIMIT_ERRINT_RTIF_RTIE_ROIE_SHIFT       6
#define CAN_LIMIT_ERRINT_RTIF_RTIE_RIE_SHIFT        7
#define CAN_LIMIT_ERRINT_RTIF_RTIE_AIF_SHIFT        8
#define CAN_LIMIT_ERRINT_RTIF_RTIE_EIF_SHIFT        9
#define CAN_LIMIT_ERRINT_RTIF_RTIE_TSIF_SHIFT       10
#define CAN_LIMIT_ERRINT_RTIF_RTIE_TPIF_SHIFT       11
#define CAN_LIMIT_ERRINT_RTIF_RTIE_RAFIF_SHIFT      12
#define CAN_LIMIT_ERRINT_RTIF_RTIE_RFIF_SHIFT       13
#define CAN_LIMIT_ERRINT_RTIF_RTIE_ROIF_SHIFT       14
#define CAN_LIMIT_ERRINT_RTIF_RTIE_RIF_SHIFT        15
#define CAN_LIMIT_ERRINT_RTIF_RTIE_BEIF_SHIFT       16
#define CAN_LIMIT_ERRINT_RTIF_RTIE_BEIE_SHIFT       17
#define CAN_LIMIT_ERRINT_RTIF_RTIE_ALIF_SHIFT       18
#define CAN_LIMIT_ERRINT_RTIF_RTIE_ALIE_SHIFT       19
#define CAN_LIMIT_ERRINT_RTIF_RTIE_EPIF_SHIFT       20
#define CAN_LIMIT_ERRINT_RTIF_RTIE_EPIE_SHIFT       21
#define CAN_LIMIT_ERRINT_RTIF_RTIE_EPASS_SHIFT      22
#define CAN_LIMIT_ERRINT_RTIF_RTIE_EWARN_SHIFT      23
#define CAN_LIMIT_ERRINT_RTIF_RTIE_EWL_SHIFT        24
#define CAN_LIMIT_ERRINT_RTIF_RTIE_AFWL_SHIFT       28

#define CAN_LIMIT_ERRINT_RTIF_RTIE_TSFF_SIZE        1
#define CAN_LIMIT_ERRINT_RTIF_RTIE_EIE_SIZE         1
#define CAN_LIMIT_ERRINT_RTIF_RTIE_TSIE_SIZE        1
#define CAN_LIMIT_ERRINT_RTIF_RTIE_TPIE_SIZE        1
#define CAN_LIMIT_ERRINT_RTIF_RTIE_RAFIE_SIZE       1
#define CAN_LIMIT_ERRINT_RTIF_RTIE_RFIE_SIZE        1
#define CAN_LIMIT_ERRINT_RTIF_RTIE_ROIE_SIZE        1
#define CAN_LIMIT_ERRINT_RTIF_RTIE_RIE_SIZE         1
#define CAN_LIMIT_ERRINT_RTIF_RTIE_AIF_SIZE         1
#define CAN_LIMIT_ERRINT_RTIF_RTIE_EIF_SIZE         1
#define CAN_LIMIT_ERRINT_RTIF_RTIE_TSIF_SIZE        1
#define CAN_LIMIT_ERRINT_RTIF_RTIE_TPIF_SIZE        1
#define CAN_LIMIT_ERRINT_RTIF_RTIE_RAFIF_SIZE       1
#define CAN_LIMIT_ERRINT_RTIF_RTIE_RFIF_SIZE        1
#define CAN_LIMIT_ERRINT_RTIF_RTIE_ROIF_SIZE        1
#define CAN_LIMIT_ERRINT_RTIF_RTIE_RIF_SIZE         1
#define CAN_LIMIT_ERRINT_RTIF_RTIE_BEIF_SIZE        1
#define CAN_LIMIT_ERRINT_RTIF_RTIE_BEIE_SIZE        1
#define CAN_LIMIT_ERRINT_RTIF_RTIE_ALIF_SIZE        1
#define CAN_LIMIT_ERRINT_RTIF_RTIE_ALIE_SIZE        1
#define CAN_LIMIT_ERRINT_RTIF_RTIE_EPIF_SIZE        1
#define CAN_LIMIT_ERRINT_RTIF_RTIE_EPIE_SIZE        1
#define CAN_LIMIT_ERRINT_RTIF_RTIE_EPASS_SIZE       1
#define CAN_LIMIT_ERRINT_RTIF_RTIE_EWARN_SIZE       1
#define CAN_LIMIT_ERRINT_RTIF_RTIE_EWL_SIZE         4
#define CAN_LIMIT_ERRINT_RTIF_RTIE_AFWL_SIZE    4

#define CAN_LIMIT_ERRINT_RTIF_RTIE_ALLINTR_MASK     0x0015FF00

///////////////////////////////////////////////////////
// Register: S_PRESC_S_SJW_S_Seg_2_S_Seg_1
// (Prescaler Registers)
// (Bit Timing Register)
// (Bit Timing Register 2)
// (Bit Timing Register 1)
///////////////////////////////////////////////////////
#define CAN_S_PRESC_S_SJW_S_SEG_2_S_SEG_1_OFFSET    0xA8UL
#define CAN_S_PRESC_S_SJW_S_SEG_2_S_SEG_1_NUM       0x1

#define CAN_S_PRESC_S_SJW_S_SEG_2_S_SEG_1_S_SEG_1_SHIFT     0
#define CAN_S_PRESC_S_SJW_S_SEG_2_S_SEG_1_S_SEG_2_SHIFT     8
#define CAN_S_PRESC_S_SJW_S_SEG_2_S_SEG_1_REDV_15_SHIFT     15
#define CAN_S_PRESC_S_SJW_S_SEG_2_S_SEG_1_S_SJW_SHIFT       16
#define CAN_S_PRESC_S_SJW_S_SEG_2_S_SEG_1_REDV_23_SHIFT     23
#define CAN_S_PRESC_S_SJW_S_SEG_2_S_SEG_1_S_PRESC_SHIFT     24

#define CAN_S_PRESC_S_SJW_S_SEG_2_S_SEG_1_S_SEG_1_SIZE      8
#define CAN_S_PRESC_S_SJW_S_SEG_2_S_SEG_1_S_SEG_2_SIZE      7
#define CAN_S_PRESC_S_SJW_S_SEG_2_S_SEG_1_REDV_15_SIZE      1
#define CAN_S_PRESC_S_SJW_S_SEG_2_S_SEG_1_S_SJW_SIZE        7
#define CAN_S_PRESC_S_SJW_S_SEG_2_S_SEG_1_REDV_23_SIZE      1
#define CAN_S_PRESC_S_SJW_S_SEG_2_S_SEG_1_S_PRESC_SIZE      8

///////////////////////////////////////////////////////
// Register: F_PRESC_F_SJW_F_Seg_2_F_Seg_1
// (Prescaler Registers)
// (Bit Timing Register)
// (Bit Timing Register 2)
// (Bit Timing Register 1)
///////////////////////////////////////////////////////
#define CAN_F_PRESC_F_SJW_F_SEG_2_F_SEG_1_OFFSET    0xACUL
#define CAN_F_PRESC_F_SJW_F_SEG_2_F_SEG_1_NUM       0x1

#define CAN_F_PRESC_F_SJW_F_SEG_2_F_SEG_1_F_SEG_1_SHIFT         0
#define CAN_F_PRESC_F_SJW_F_SEG_2_F_SEG_1_REDV_5_7_SHIFT        5
#define CAN_F_PRESC_F_SJW_F_SEG_2_F_SEG_1_F_SEG_2_SHIFT         8
#define CAN_F_PRESC_F_SJW_F_SEG_2_F_SEG_1_REDV_12_15_SHIFT      12
#define CAN_F_PRESC_F_SJW_F_SEG_2_F_SEG_1_F_SJW_SHIFT           16
#define CAN_F_PRESC_F_SJW_F_SEG_2_F_SEG_1_REDV_20_23_SHIFT      20
#define CAN_F_PRESC_F_SJW_F_SEG_2_F_SEG_1_F_PRESC_SHIFT         24

#define CAN_F_PRESC_F_SJW_F_SEG_2_F_SEG_1_F_SEG_1_SIZE          5
#define CAN_F_PRESC_F_SJW_F_SEG_2_F_SEG_1_REDV_5_7_SIZE         3
#define CAN_F_PRESC_F_SJW_F_SEG_2_F_SEG_1_F_SEG_2_SIZE          4
#define CAN_F_PRESC_F_SJW_F_SEG_2_F_SEG_1_REDV_12_15_SIZE       4
#define CAN_F_PRESC_F_SJW_F_SEG_2_F_SEG_1_F_SJW_SIZE            4
#define CAN_F_PRESC_F_SJW_F_SEG_2_F_SEG_1_REDV_20_23_SIZE       4
#define CAN_F_PRESC_F_SJW_F_SEG_2_F_SEG_1_F_PRESC_SIZE          8

///////////////////////////////////////////////////////
// Register: TECNT_REVNT_TDC_EALCAP
// (Error Counter Registers)
// (Error Counter Registers)
// (Transmitter Delay Compensation Register)
// (Error and Arbitration Lost Capture Register EALCAP)
///////////////////////////////////////////////////////
#define CAN_TECNT_REVNT_TDC_EALCAP_OFFSET   0xB0UL
#define CAN_TECNT_REVNT_TDC_EALCAP_NUM      0x1

#define CAN_TECNT_REVNT_TDC_EALCAP_ALC_SHIFT        0
#define CAN_TECNT_REVNT_TDC_EALCAP_KOER_SHIFT       5
#define CAN_TECNT_REVNT_TDC_EALCAP_SSPOFF_SHIFT     8
#define CAN_TECNT_REVNT_TDC_EALCAP_TDCEN_SHIFT      15
#define CAN_TECNT_REVNT_TDC_EALCAP_RECNT_SHIFT      16
#define CAN_TECNT_REVNT_TDC_EALCAP_TECNT_SHIFT      24

#define CAN_TECNT_REVNT_TDC_EALCAP_ALC_SIZE         5
#define CAN_TECNT_REVNT_TDC_EALCAP_KOER_SIZE        3
#define CAN_TECNT_REVNT_TDC_EALCAP_SSPOFF_SIZE      7
#define CAN_TECNT_REVNT_TDC_EALCAP_TDCEN_SIZE       1
#define CAN_TECNT_REVNT_TDC_EALCAP_RECNT_SIZE       8
#define CAN_TECNT_REVNT_TDC_EALCAP_TECNT_SIZE       8

///////////////////////////////////////////////////////
// Register: ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL
// (Acceptance Filter Enable)
// (Acceptance filter Enable)
// (CiA 603 Time Stamping TIMECFG)
// (Acceptance Filter Control Register)
///////////////////////////////////////////////////////
#define CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_OFFSET    0xB4UL
#define CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_NUM       0x1

#define CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_ACFADR_SHIFT      0
#define CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_RSDV_4_SHIFT      4
#define CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_SELMASK_SHIFT     5
#define CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_RSDV_6_7_SHIFT    6
#define CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_TIMEEN_SHIFT      8
#define CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_TIMEPOS_SHIFT     9
#define CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_RSDV_10_15_SHIFT  10
#define CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_AE_0_SHIFT        16
#define CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_AE_1_SHIFT        17
#define CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_AE_2_SHIFT        18
#define CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_AE_3_SHIFT        19
#define CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_AE_4_SHIFT        20
#define CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_AE_5_SHIFT        21
#define CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_AE_6_SHIFT        22
#define CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_AE_7_SHIFT        23
#define CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_AE_8_SHIFT        24
#define CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_AE_9_SHIFT        25
#define CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_AE_10_SHIFT       26
#define CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_AE_11_SHIFT       27
#define CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_AE_12_SHIFT       28
#define CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_AE_13_SHIFT       29
#define CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_AE_14_SHIFT       30
#define CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_AE_15_SHIFT       31

#define CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_ACFADR_SIZE       4
#define CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_RSDV_4_SIZE       1
#define CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_SELSIZE           1
#define CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_RSDV_6_7_SIZE     2
#define CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_TIMEEN_SIZE       1
#define CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_TIMEPOS_SIZE      1
#define CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_RSDV_10_15_SIZE   6
#define CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_AE_0_SIZE         1
#define CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_AE_1_SIZE         1
#define CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_AE_2_SIZE         1
#define CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_AE_3_SIZE         1
#define CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_AE_4_SIZE         1
#define CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_AE_5_SIZE         1
#define CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_AE_6_SIZE         1
#define CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_AE_7_SIZE         1
#define CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_AE_8_SIZE         1
#define CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_AE_9_SIZE         1
#define CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_AE_10_SIZE        1
#define CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_AE_11_SIZE        1
#define CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_AE_12_SIZE        1
#define CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_AE_13_SIZE        1
#define CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_AE_14_SIZE        1
#define CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_AE_15_SIZE        1

#define CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_AE_X_SIZE         3

///////////////////////////////////////////////////////
// Register: ACF_3_ACF_2_ACF_1_ACF_0
// (Bits in Register AC F_3 , if SELMASK=1)
// Acceptance CODE ACODE_x
// Acceptance MASK AMASK_x
///////////////////////////////////////////////////////
#define CAN_ACF_3_ACF_2_ACF_1_ACF_0_OFFSET  0xB8UL
#define CAN_ACF_3_ACF_2_ACF_1_ACF_0_NUM     0x1

#define CAN_ACF_3_ACF_2_ACF_1_ACF_0_ACODE_AMASK_7_0_SHIFT       0
#define CAN_ACF_3_ACF_2_ACF_1_ACF_0_ACODE_AMASK_15_8_SHIFT      8
#define CAN_ACF_3_ACF_2_ACF_1_ACF_0_ACODE_AMASK_23_16_SHIFT     16
#define CAN_ACF_3_ACF_2_ACF_1_ACF_0_ACODE_AMASK_28_24_SHIFT     24
#define CAN_ACF_3_ACF_2_ACF_1_ACF_0_AIDE_SHIFT                  29
#define CAN_ACF_3_ACF_2_ACF_1_ACF_0_AIDEE_SHIFT                 30
#define CAN_ACF_3_ACF_2_ACF_1_ACF_0_RSDV_31_SHIFT               31

#define CAN_ACF_3_ACF_2_ACF_1_ACF_0_ACODE_AMASK_7_0_SIZE        8
#define CAN_ACF_3_ACF_2_ACF_1_ACF_0_ACODE_AMASK_15_8_SIZE       8
#define CAN_ACF_3_ACF_2_ACF_1_ACF_0_ACODE_AMASK_23_16_SIZE      8
#define CAN_ACF_3_ACF_2_ACF_1_ACF_0_ACODE_AMASK_28_24_SIZE      5
#define CAN_ACF_3_ACF_2_ACF_1_ACF_0_AIDE_SIZE                   1
#define CAN_ACF_3_ACF_2_ACF_1_ACF_0_AIDEE_SIZE                  1
#define CAN_ACF_3_ACF_2_ACF_1_ACF_0_RSDV_31_SIZE                1

#define CAN_ACF_3_ACF_2_ACF_1_ACF_0_ACODE_AMASK_VALUE_MASK      0x1FFFFFFF
#define CAN_ACF_3_ACF_2_ACF_1_ACF_0_ACODE_AMASK_TYPE_MASK       0x60000000

///////////////////////////////////////////////////////
// Register: TTCFG_TBSLOT_VER_1_VER_0
// (Time Trigger Configuration)
// (TT CAN: TB Slot Pointer TBSLOT)
// (Version Information VER_1)
// (Version Information VER_0)
///////////////////////////////////////////////////////
#define CAN_TTCFG_TBSLOT_VER_1_VER_0_OFFSET     0xBCUL
#define CAN_TTCFG_TBSLOT_VER_1_VER_0_NUM        0x1

#define CAN_TTCFG_TBSLOT_VER_1_VER_0_VER_1_VER_0_SHIFT  0
#define CAN_TTCFG_TBSLOT_VER_1_VER_0_TBPTR_SHIFT        16
#define CAN_TTCFG_TBSLOT_VER_1_VER_0_TBF_SHIFT          22
#define CAN_TTCFG_TBSLOT_VER_1_VER_0_TBE_SHIFT          23
#define CAN_TTCFG_TBSLOT_VER_1_VER_0_TTEN_SHIFT         24
#define CAN_TTCFG_TBSLOT_VER_1_VER_0_T_PRESC_SHIFT      25
#define CAN_TTCFG_TBSLOT_VER_1_VER_0_TTIF_SHIFT         27
#define CAN_TTCFG_TBSLOT_VER_1_VER_0_TTIE_SHIFT         28
#define CAN_TTCFG_TBSLOT_VER_1_VER_0_TEIF_SHIFT         29
#define CAN_TTCFG_TBSLOT_VER_1_VER_0_WTIF_SHIFT         30
#define CAN_TTCFG_TBSLOT_VER_1_VER_0_WTIE_SHIFT         31

#define CAN_TTCFG_TBSLOT_VER_1_VER_0_VER_1_VER_0_SIZE   16
#define CAN_TTCFG_TBSLOT_VER_1_VER_0_TBPTR_SIZE         6
#define CAN_TTCFG_TBSLOT_VER_1_VER_0_TBF_SIZE           1
#define CAN_TTCFG_TBSLOT_VER_1_VER_0_TBE_SIZE           1
#define CAN_TTCFG_TBSLOT_VER_1_VER_0_TTEN_SIZE          1
#define CAN_TTCFG_TBSLOT_VER_1_VER_0_T_PRESC_SIZE       2
#define CAN_TTCFG_TBSLOT_VER_1_VER_0_TTIF_SIZE          1
#define CAN_TTCFG_TBSLOT_VER_1_VER_0_TTIE_SIZE          1
#define CAN_TTCFG_TBSLOT_VER_1_VER_0_TEIF_SIZE          1
#define CAN_TTCFG_TBSLOT_VER_1_VER_0_WTIF_SIZE          1
#define CAN_TTCFG_TBSLOT_VER_1_VER_0_WTIE_SIZE          1

///////////////////////////////////////////////////////
// Register: REF_MSG
// TTCAN: Reference Mes sage REF_MSG_0 to REF_MSG_ 3
///////////////////////////////////////////////////////
#define CAN_REF_MSG_OFFSET  0xC0UL
#define CAN_REF_MSG_NUM     0x1

#define CAN_REF_MSG_REF_ID_SHIFT        0
#define CAN_REF_MSG_RSDV_29_30_SHIFT    29
#define CAN_REF_MSG_REF_IDE_SHIFT       31

#define CAN_REF_MSG_REF_ID_SIZE         29
#define CAN_REF_MSG_RSDV_29_30_SIZE     2
#define CAN_REF_MSG_REF_IDE_SIZE        1

///////////////////////////////////////////////////////
// Register: TT_TRIG_TRIG_CFG
// (TTCAN: Trigger Time TT_TRIG_0 and TT_TRIG_1 )
// (TTCAN: Trigger Configuration TRIG_CFG_1)
///////////////////////////////////////////////////////
#define CAN_TT_TRIG_TRIG_CFG_OFFSET     0xC4UL
#define CAN_TT_TRIG_TRIG_CFG_NUM        0x1

#define CAN_TT_TRIG_TRIG_CFG_TTPTR_SHIFT        0
#define CAN_TT_TRIG_TRIG_CFG_RSDV_6_7_SHIFT     6
#define CAN_TT_TRIG_TRIG_CFG_TTYPE_SHIFT        8
#define CAN_TT_TRIG_TRIG_CFG_RSDV_11_SHIFT      11
#define CAN_TT_TRIG_TRIG_CFG_TEW_3_0_SHIFT      12
#define CAN_TT_TRIG_TRIG_CFG_TT_TRIG_SHIFT      16

#define CAN_TT_TRIG_TRIG_CFG_TTPTR_SIZE         6
#define CAN_TT_TRIG_TRIG_CFG_RSDV_6_7_SIZE      2
#define CAN_TT_TRIG_TRIG_CFG_TTYPE_SIZE         3
#define CAN_TT_TRIG_TRIG_CFG_RSDV_11_SIZE       1
#define CAN_TT_TRIG_TRIG_CFG_TEW_3_0_SIZE       4
#define CAN_TT_TRIG_TRIG_CFG_TT_TRIG_SIZE       16

///////////////////////////////////////////////////////
// Register: TT_WTRIG
// TTCAN: Watch Trigger Time TT_WTRIG_0 and TT_WTRIG_ 1
///////////////////////////////////////////////////////
#define CAN_TT_WTRIG_OFFSET     0xC8UL
#define CAN_TT_WTRIG_NUM        0x1

#define CAN_TT_WTRIG_TT_WTRIG_SHIFT     0
#define CAN_TT_WTRIG_RSDV_16_32_SHIFT   16

#define CAN_TT_WTRIG_TT_WTRIG_SIZE      16
#define CAN_TT_WTRIG_RSDV_16_32_SIZE    16

/**************************** Type Definitions ******************************/
typedef struct {
    AL_U32      DeviceId;
    AL_U32      BaseAddress;
    AL_U32      IntrId;
    AL_U32      InputClk;
    AL_U32      Type;
    AL_U32      SlowBitRate;
    AL_U32      FastBitRate;
} AL_CAN_HwConfigStruct;

#ifdef __cplusplus
}
#endif

#endif
