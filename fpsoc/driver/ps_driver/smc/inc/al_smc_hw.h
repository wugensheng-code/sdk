#ifndef AL_SMC_HW_H
#define AL_SMC_HW_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files ********************************/
#include "al_type.h"

/************************** Constant Definitions ****************************/

/**************************** Type Definitions ******************************/

/***************** Macros (Inline Functions Definitions ********************/
#define TOP_NS_BASE_ADDR 0xF8800000ULL



#define SMC_BASE_ADDR 	0xF841A000ULL
#define NAND_BASE_ADDR 	0x64000000ULL

#define AL_SMC_NUM_INSTANCE	(1)

///////////////////////////////////////////////////////
// Register: memc_status
// Memory Controller Status Register
///////////////////////////////////////////////////////

#define SMC_MEMC_STATUS_OFFSET 0x0UL
#define SMC_MEMC_STATUS_NUM  0x1
#define SMC_MEMC_STATUS_STATE_SHIFT    0
#define SMC_MEMC_STATUS_INT_EN0_SHIFT    1
#define SMC_MEMC_STATUS_INT_EN1_SHIFT    2
#define SMC_MEMC_STATUS_INT_STATUS0_SHIFT    3
#define SMC_MEMC_STATUS_INT_STATUS1_SHIFT    4
#define SMC_MEMC_STATUS_RAW_INT_STATUS0_SHIFT    5
#define SMC_MEMC_STATUS_RAW_INT_STATUS1_SHIFT    6
#define SMC_MEMC_STATUS_ECC_INT_EN0_SHIFT    7
#define SMC_MEMC_STATUS_ECC_INT_EN1_SHIFT    8
#define SMC_MEMC_STATUS_ECC_INT0_SHIFT    9
#define SMC_MEMC_STATUS_ECC_INT1_SHIFT    10
#define SMC_MEMC_STATUS_RAW_ECC_INT0_SHIFT    11
#define SMC_MEMC_STATUS_RAW_ECC_INT1_SHIFT    12
#define SMC_MEMC_STATUS_RESERVED_31_13_SHIFT    13

#define SMC_MEMC_STATUS_STATE_SIZE 1
#define SMC_MEMC_STATUS_INT_EN0_SIZE 1
#define SMC_MEMC_STATUS_INT_EN1_SIZE 1
#define SMC_MEMC_STATUS_INT_STATUS0_SIZE 1
#define SMC_MEMC_STATUS_INT_STATUS1_SIZE 1
#define SMC_MEMC_STATUS_RAW_INT_STATUS0_SIZE 1
#define SMC_MEMC_STATUS_RAW_INT_STATUS1_SIZE 1
#define SMC_MEMC_STATUS_ECC_INT_EN0_SIZE 1
#define SMC_MEMC_STATUS_ECC_INT_EN1_SIZE 1
#define SMC_MEMC_STATUS_ECC_INT0_SIZE 1
#define SMC_MEMC_STATUS_ECC_INT1_SIZE 1
#define SMC_MEMC_STATUS_RAW_ECC_INT0_SIZE 1
#define SMC_MEMC_STATUS_RAW_ECC_INT1_SIZE 1
#define SMC_MEMC_STATUS_RESERVED_31_13_SIZE 19

///////////////////////////////////////////////////////
// Register: memif_cfg
// Memory Interface Configuration Register
///////////////////////////////////////////////////////

#define SMC_MEMIF_CFG_OFFSET 0x4UL
#define SMC_MEMIF_CFG_NUM  0x1

#define SMC_MEMIF_CFG_MEMORY0_TYPE_SHIFT    0
#define SMC_MEMIF_CFG_MEMORY0_CHIPS_SHIFT    2
#define SMC_MEMIF_CFG_MEMORY0_BYTES_SHIFT    4
#define SMC_MEMIF_CFG_REMAP0_SHIFT    6
#define SMC_MEMIF_CFG_RESERVED_7_SHIFT    7
#define SMC_MEMIF_CFG_MEMORY1_TYPE_SHIFT    8
#define SMC_MEMIF_CFG_MEMORY1_CHIPS_SHIFT    10
#define SMC_MEMIF_CFG_MEMORY1_BYTES_SHIFT    12
#define SMC_MEMIF_CFG_REMAP1_SHIFT    14
#define SMC_MEMIF_CFG_RESERVED_15_SHIFT    15
#define SMC_MEMIF_CFG_EXCLUSIVE_MONITORS_SHIFT    16
#define SMC_MEMIF_CFG_RESERVED_31_18_SHIFT    18
#define SMC_MEMIF_CFG_MEMORY0_TYPE_SIZE 2
#define SMC_MEMIF_CFG_MEMORY0_CHIPS_SIZE 2
#define SMC_MEMIF_CFG_MEMORY0_BYTES_SIZE 2
#define SMC_MEMIF_CFG_REMAP0_SIZE 1
#define SMC_MEMIF_CFG_RESERVED_7_SIZE 1
#define SMC_MEMIF_CFG_MEMORY1_TYPE_SIZE 2
#define SMC_MEMIF_CFG_MEMORY1_CHIPS_SIZE 2
#define SMC_MEMIF_CFG_MEMORY1_BYTES_SIZE 2
#define SMC_MEMIF_CFG_REMAP1_SIZE 1
#define SMC_MEMIF_CFG_RESERVED_15_SIZE 1
#define SMC_MEMIF_CFG_EXCLUSIVE_MONITORS_SIZE 2
#define SMC_MEMIF_CFG_RESERVED_31_18_SIZE 14


///////////////////////////////////////////////////////
// Register: mem_cfg_set
// Set Configuration Register
///////////////////////////////////////////////////////

#define SMC_MEM_CFG_SET_OFFSET 0x8UL
#define SMC_MEM_CFG_SET_NUM  0x1
#define SMC_MEM_CFG_SET_INT_ENABLE0_SHIFT    0
#define SMC_MEM_CFG_SET_INT_ENABLE1_SHIFT    1
#define SMC_MEM_CFG_SET_LOW_POWER_REQ_SHIFT    2
#define SMC_MEM_CFG_SET_RESERVED_4_3_SHIFT    3
#define SMC_MEM_CFG_SET_ECC_INT_ENABLE0_SHIFT    5
#define SMC_MEM_CFG_SET_ECC_INT_ENABLE1_SHIFT    6
#define SMC_MEM_CFG_SET_RESERVED_31_7_SHIFT    7

#define SMC_MEM_CFG_SET_INT_ENABLE0_SIZE 1
#define SMC_MEM_CFG_SET_INT_ENABLE1_SIZE 1
#define SMC_MEM_CFG_SET_LOW_POWER_REQ_SIZE 1
#define SMC_MEM_CFG_SET_RESERVED_4_3_SIZE 2
#define SMC_MEM_CFG_SET_ECC_INT_ENABLE0_SIZE 1
#define SMC_MEM_CFG_SET_ECC_INT_ENABLE1_SIZE 1
#define SMC_MEM_CFG_SET_RESERVED_31_7_SIZE 25

///////////////////////////////////////////////////////
// Register: mem_cfg_clr
// Clear Configuration Register
///////////////////////////////////////////////////////

#define SMC_MEM_CFG_CLR_OFFSET 0xCUL
#define SMC_MEM_CFG_CLR_NUM  0x1

#define SMC_MEM_CFG_CLR_INT_DISABLE0_SHIFT    0
#define SMC_MEM_CFG_CLR_INT_DISABLE1_SHIFT    1
#define SMC_MEM_CFG_CLR_LOW_POWER_EXIT_SHIFT    2
#define SMC_MEM_CFG_CLR_INT_CLEAR0_SHIFT    3
#define SMC_MEM_CFG_CLR_INT_CLEAR1_SHIFT    4
#define SMC_MEM_CFG_CLR_ECC_INT_DISABLE0_SHIFT    5
#define SMC_MEM_CFG_CLR_ECC_INT_DISABLE1_SHIFT    6
#define SMC_MEM_CFG_CLR_RESERVED_31_7_SHIFT    7

#define SMC_MEM_CFG_CLR_INT_DISABLE0_SIZE 1
#define SMC_MEM_CFG_CLR_INT_DISABLE1_SIZE 1
#define SMC_MEM_CFG_CLR_LOW_POWER_EXIT_SIZE 1
#define SMC_MEM_CFG_CLR_INT_CLEAR0_SIZE 1
#define SMC_MEM_CFG_CLR_INT_CLEAR1_SIZE 1
#define SMC_MEM_CFG_CLR_ECC_INT_DISABLE0_SIZE 1
#define SMC_MEM_CFG_CLR_ECC_INT_DISABLE1_SIZE 1
#define SMC_MEM_CFG_CLR_RESERVED_31_7_SIZE 25


///////////////////////////////////////////////////////
// Register: direct_cmd
// Direct Command Register
///////////////////////////////////////////////////////

#define SMC_DIRECT_CMD_OFFSET 0x10UL
#define SMC_DIRECT_CMD_NUM  0x1
#define SMC_DIRECT_CMD_ADDR_19_TO_0_SHIFT    0
#define SMC_DIRECT_CMD_SET_CRE_SHIFT    20
#define SMC_DIRECT_CMD_CMD_TYPE_SHIFT    21
#define SMC_DIRECT_CMD_CHIP_NMBR_SHIFT    23
#define SMC_DIRECT_CMD_RESERVED_31_26_SHIFT    26

#define SMC_DIRECT_CMD_ADDR_19_TO_0_SIZE 20
#define SMC_DIRECT_CMD_SET_CRE_SIZE 1
#define SMC_DIRECT_CMD_CMD_TYPE_SIZE 2
#define SMC_DIRECT_CMD_CHIP_NMBR_SIZE 3
#define SMC_DIRECT_CMD_RESERVED_31_26_SIZE 6

///////////////////////////////////////////////////////
// Register: set_cycles
// Set Cycles Register
///////////////////////////////////////////////////////

#define SMC_SET_CYCLES_OFFSET 0x14UL
#define SMC_SET_CYCLES_NUM  0x1

#define SMC_SET_CYCLES_SET_T0_SHIFT    0
#define SMC_SET_CYCLES_SET_T1_SHIFT    4
#define SMC_SET_CYCLES_SET_T2_SHIFT    8
#define SMC_SET_CYCLES_SET_T3_SHIFT    11
#define SMC_SET_CYCLES_SET_T4_SHIFT    14
#define SMC_SET_CYCLES_SET_T5_SHIFT    17
#define SMC_SET_CYCLES_SET_T6_SHIFT    20
#define SMC_SET_CYCLES_RESERVED_31_24_SHIFT    24
#define SMC_SET_CYCLES_SET_T0_SIZE 4
#define SMC_SET_CYCLES_SET_T1_SIZE 4
#define SMC_SET_CYCLES_SET_T2_SIZE 3
#define SMC_SET_CYCLES_SET_T3_SIZE 3
#define SMC_SET_CYCLES_SET_T4_SIZE 3
#define SMC_SET_CYCLES_SET_T5_SIZE 3
#define SMC_SET_CYCLES_SET_T6_SIZE 4
#define SMC_SET_CYCLES_RESERVED_31_24_SIZE 8


///////////////////////////////////////////////////////
// Register: set_opmode
// Set Operating Mode Register
///////////////////////////////////////////////////////

#define SMC_SET_OPMODE_OFFSET 0x18UL
#define SMC_SET_OPMODE_NUM  0x1
#define SMC_SET_OPMODE_SET_MW_SHIFT    0
#define SMC_SET_OPMODE_SET_RD_SYNC_SHIFT    2
#define SMC_SET_OPMODE_SET_RD_BL_SHIFT    3
#define SMC_SET_OPMODE_SET_WR_SYNC_SHIFT    6
#define SMC_SET_OPMODE_SET_WR_BL_SHIFT    7
#define SMC_SET_OPMODE_SET_BAA_SHIFT    10
#define SMC_SET_OPMODE_SET_ADV_SHIFT    11
#define SMC_SET_OPMODE_SET_BLS_TIME_SHIFT    12
#define SMC_SET_OPMODE_SET_BURST_ALIGN_SHIFT    13
#define SMC_SET_OPMODE_RESERVED_31_16_SHIFT    16

#define SMC_SET_OPMODE_SET_MW_SIZE 2
#define SMC_SET_OPMODE_SET_RD_SYNC_SIZE 1
#define SMC_SET_OPMODE_SET_RD_BL_SIZE 3
#define SMC_SET_OPMODE_SET_WR_SYNC_SIZE 1
#define SMC_SET_OPMODE_SET_WR_BL_SIZE 3
#define SMC_SET_OPMODE_SET_BAA_SIZE 1
#define SMC_SET_OPMODE_SET_ADV_SIZE 1
#define SMC_SET_OPMODE_SET_BLS_TIME_SIZE 1
#define SMC_SET_OPMODE_SET_BURST_ALIGN_SIZE 3
#define SMC_SET_OPMODE_RESERVED_31_16_SIZE 16

///////////////////////////////////////////////////////
// Register: refresh_0
// Refresh Period 0 Register
///////////////////////////////////////////////////////

#define SMC_REFRESH_0_OFFSET 0x20UL
#define SMC_REFRESH_0_NUM  0x1

#define SMC_REFRESH_0_REF_PERIOD0_SHIFT    0
#define SMC_REFRESH_0_RESERVED_31_4_SHIFT    4
#define SMC_REFRESH_0_REF_PERIOD0_SIZE 4
#define SMC_REFRESH_0_RESERVED_31_4_SIZE 28


///////////////////////////////////////////////////////
// Register: opmode0_0
// Operating Mode Status Register
///////////////////////////////////////////////////////

#define SMC_OPMODE0_0_OFFSET 0x104UL
#define SMC_OPMODE0_0_NUM  0x1
#define SMC_OPMODE0_0_MW0_SHIFT    0
#define SMC_OPMODE0_0_RD_SYNC0_SHIFT    2
#define SMC_OPMODE0_0_RD_BL0_SHIFT    3
#define SMC_OPMODE0_0_WR_SYNC0_SHIFT    6
#define SMC_OPMODE0_0_WR_BL0_SHIFT    7
#define SMC_OPMODE0_0_BAA0_SHIFT    10
#define SMC_OPMODE0_0_ADV0_SHIFT    11
#define SMC_OPMODE0_0_BLS_TIME0_SHIFT    12
#define SMC_OPMODE0_0_BURST_ALIGN0_SHIFT    13
#define SMC_OPMODE0_0_ADDR_MASK0_SHIFT    16
#define SMC_OPMODE0_0_ADDR_MATCH0_SHIFT    24

#define SMC_OPMODE0_0_MW0_SIZE 2
#define SMC_OPMODE0_0_RD_SYNC0_SIZE 1
#define SMC_OPMODE0_0_RD_BL0_SIZE 3
#define SMC_OPMODE0_0_WR_SYNC0_SIZE 1
#define SMC_OPMODE0_0_WR_BL0_SIZE 3
#define SMC_OPMODE0_0_BAA0_SIZE 1
#define SMC_OPMODE0_0_ADV0_SIZE 1
#define SMC_OPMODE0_0_BLS_TIME0_SIZE 1
#define SMC_OPMODE0_0_BURST_ALIGN0_SIZE 3
#define SMC_OPMODE0_0_ADDR_MASK0_SIZE 8
#define SMC_OPMODE0_0_ADDR_MATCH0_SIZE 8

///////////////////////////////////////////////////////
// Register: opmode0_1
// Operating Mode Status Register
///////////////////////////////////////////////////////

#define SMC_OPMODE0_1_OFFSET 0x124UL
#define SMC_OPMODE0_1_NUM  0x1

#define SMC_OPMODE0_1_MW0_SHIFT    0
#define SMC_OPMODE0_1_RD_SYNC0_SHIFT    2
#define SMC_OPMODE0_1_RD_BL0_SHIFT    3
#define SMC_OPMODE0_1_WR_SYNC0_SHIFT    6
#define SMC_OPMODE0_1_WR_BL0_SHIFT    7
#define SMC_OPMODE0_1_BAA0_SHIFT    10
#define SMC_OPMODE0_1_ADV0_SHIFT    11
#define SMC_OPMODE0_1_BLS_TIME0_SHIFT    12
#define SMC_OPMODE0_1_BURST_ALIGN0_SHIFT    13
#define SMC_OPMODE0_1_ADDR_MASK0_SHIFT    16
#define SMC_OPMODE0_1_ADDR_MATCH0_SHIFT    24
#define SMC_OPMODE0_1_MW0_SIZE 2
#define SMC_OPMODE0_1_RD_SYNC0_SIZE 1
#define SMC_OPMODE0_1_RD_BL0_SIZE 3
#define SMC_OPMODE0_1_WR_SYNC0_SIZE 1
#define SMC_OPMODE0_1_WR_BL0_SIZE 3
#define SMC_OPMODE0_1_BAA0_SIZE 1
#define SMC_OPMODE0_1_ADV0_SIZE 1
#define SMC_OPMODE0_1_BLS_TIME0_SIZE 1
#define SMC_OPMODE0_1_BURST_ALIGN0_SIZE 3
#define SMC_OPMODE0_1_ADDR_MASK0_SIZE 8
#define SMC_OPMODE0_1_ADDR_MATCH0_SIZE 8


///////////////////////////////////////////////////////
// Register: nand_cycles1_0
// NAND Cycles Register
///////////////////////////////////////////////////////

#define SMC_NAND_CYCLES1_0_OFFSET 0x180UL
#define SMC_NAND_CYCLES1_0_NUM  0x1
#define SMC_NAND_CYCLES1_0_T_RC1_SHIFT    0
#define SMC_NAND_CYCLES1_0_T_WC1_SHIFT    4
#define SMC_NAND_CYCLES1_0_T_REA1_SHIFT    8
#define SMC_NAND_CYCLES1_0_T_WP1_SHIFT    11
#define SMC_NAND_CYCLES1_0_T_CLR1_SHIFT    14
#define SMC_NAND_CYCLES1_0_T_AR1_SHIFT    17
#define SMC_NAND_CYCLES1_0_T_RR1_SHIFT    20
#define SMC_NAND_CYCLES1_0_RESERVED_31_24_SHIFT    24

#define SMC_NAND_CYCLES1_0_T_RC1_SIZE 4
#define SMC_NAND_CYCLES1_0_T_WC1_SIZE 4
#define SMC_NAND_CYCLES1_0_T_REA1_SIZE 3
#define SMC_NAND_CYCLES1_0_T_WP1_SIZE 3
#define SMC_NAND_CYCLES1_0_T_CLR1_SIZE 3
#define SMC_NAND_CYCLES1_0_T_AR1_SIZE 3
#define SMC_NAND_CYCLES1_0_T_RR1_SIZE 4
#define SMC_NAND_CYCLES1_0_RESERVED_31_24_SIZE 8

///////////////////////////////////////////////////////
// Register: opmode1_0
// Operating Mode Status Register
///////////////////////////////////////////////////////

#define SMC_OPMODE1_0_OFFSET 0x184UL
#define SMC_OPMODE1_0_NUM  0x1

#define SMC_OPMODE1_0_MW0_SHIFT    0
#define SMC_OPMODE1_0_RD_SYNC0_SHIFT    2
#define SMC_OPMODE1_0_RD_BL0_SHIFT    3
#define SMC_OPMODE1_0_WR_SYNC0_SHIFT    6
#define SMC_OPMODE1_0_WR_BL0_SHIFT    7
#define SMC_OPMODE1_0_BAA0_SHIFT    10
#define SMC_OPMODE1_0_ADV0_SHIFT    11
#define SMC_OPMODE1_0_BLS_TIME0_SHIFT    12
#define SMC_OPMODE1_0_BURST_ALIGN0_SHIFT    13
#define SMC_OPMODE1_0_ADDR_MASK0_SHIFT    16
#define SMC_OPMODE1_0_ADDR_MATCH0_SHIFT    24
#define SMC_OPMODE1_0_MW0_SIZE 2
#define SMC_OPMODE1_0_RD_SYNC0_SIZE 1
#define SMC_OPMODE1_0_RD_BL0_SIZE 3
#define SMC_OPMODE1_0_WR_SYNC0_SIZE 1
#define SMC_OPMODE1_0_WR_BL0_SIZE 3
#define SMC_OPMODE1_0_BAA0_SIZE 1
#define SMC_OPMODE1_0_ADV0_SIZE 1
#define SMC_OPMODE1_0_BLS_TIME0_SIZE 1
#define SMC_OPMODE1_0_BURST_ALIGN0_SIZE 3
#define SMC_OPMODE1_0_ADDR_MASK0_SIZE 8
#define SMC_OPMODE1_0_ADDR_MATCH0_SIZE 8


///////////////////////////////////////////////////////
// Register: user_status
// User Status Register
///////////////////////////////////////////////////////

#define SMC_USER_STATUS_OFFSET 0x200UL
#define SMC_USER_STATUS_NUM  0x1
#define SMC_USER_STATUS_USER_STATUS_SHIFT    0
#define SMC_USER_STATUS_RESERVED_31_8_SHIFT    8

#define SMC_USER_STATUS_USER_STATUS_SIZE 8
#define SMC_USER_STATUS_RESERVED_31_8_SIZE 24

///////////////////////////////////////////////////////
// Register: user_config
// User Config Register
///////////////////////////////////////////////////////

#define SMC_USER_CONFIG_OFFSET 0x204UL
#define SMC_USER_CONFIG_NUM  0x1

#define SMC_USER_CONFIG_USER_CONFIG_SHIFT    0
#define SMC_USER_CONFIG_RESERVED_31_8_SHIFT    8
#define SMC_USER_CONFIG_USER_CONFIG_SIZE 8
#define SMC_USER_CONFIG_RESERVED_31_8_SIZE 24


///////////////////////////////////////////////////////
// Register: ecc1_status
// ECC Status Register
///////////////////////////////////////////////////////

#define SMC_ECC1_STATUS_OFFSET 0x400UL
#define SMC_ECC1_STATUS_NUM  0x1
#define SMC_ECC1_STATUS_ECC_INT_STATUS_SHIFT    0
#define SMC_ECC1_STATUS_ECC_STATUS_SHIFT    6
#define SMC_ECC1_STATUS_ECC_LAST_STATUS_SHIFT    7
#define SMC_ECC1_STATUS_ECC_RD_N_WR_SHIFT    9
#define SMC_ECC1_STATUS_ECC_VALUE_VALID_SHIFT    10
#define SMC_ECC1_STATUS_ECC_FAIL_SHIFT    15
#define SMC_ECC1_STATUS_ECC_CAN_CORRECT_SHIFT    20
#define SMC_ECC1_STATUS_ECC_READ_SHIFT    25
#define SMC_ECC1_STATUS_RESERVED_31_30_SHIFT    30

#define SMC_ECC1_STATUS_ECC_INT_STATUS_SIZE 6
#define SMC_ECC1_STATUS_ECC_STATUS_SIZE 1
#define SMC_ECC1_STATUS_ECC_LAST_STATUS_SIZE 2
#define SMC_ECC1_STATUS_ECC_RD_N_WR_SIZE 1
#define SMC_ECC1_STATUS_ECC_VALUE_VALID_SIZE 5
#define SMC_ECC1_STATUS_ECC_FAIL_SIZE 5
#define SMC_ECC1_STATUS_ECC_CAN_CORRECT_SIZE 5
#define SMC_ECC1_STATUS_ECC_READ_SIZE 5
#define SMC_ECC1_STATUS_RESERVED_31_30_SIZE 2

///////////////////////////////////////////////////////
// Register: ecc1_cfg
// ECC Configuration Register
///////////////////////////////////////////////////////

#define SMC_ECC1_CFG_OFFSET 0x404UL
#define SMC_ECC1_CFG_NUM  0x1

#define SMC_ECC1_CFG_PAGE_SIZE_SHIFT    0
#define SMC_ECC1_CFG_ECC_MODE_SHIFT    2
#define SMC_ECC1_CFG_ECC_READ_END_SHIFT    4
#define SMC_ECC1_CFG_ECC_JUMP_SHIFT    5
#define SMC_ECC1_CFG_ECC_IGNORE_ADD_EIGHT_SHIFT    7
#define SMC_ECC1_CFG_ECC_INT_PASS_SHIFT    8
#define SMC_ECC1_CFG_ECC_INT_ABORT_SHIFT    9
#define SMC_ECC1_CFG_ECC_EXTRA_BLOCK_SHIFT    10
#define SMC_ECC1_CFG_ECC_EXTRA_BLOCK_SIZE_SHIFT    11
#define SMC_ECC1_CFG_RESERVED_31_13_SHIFT    13
#define SMC_ECC1_CFG_PAGE_SIZE_SIZE 2
#define SMC_ECC1_CFG_ECC_MODE_SIZE 2
#define SMC_ECC1_CFG_ECC_READ_END_SIZE 1
#define SMC_ECC1_CFG_ECC_JUMP_SIZE 2
#define SMC_ECC1_CFG_ECC_IGNORE_ADD_EIGHT_SIZE 1
#define SMC_ECC1_CFG_ECC_INT_PASS_SIZE 1
#define SMC_ECC1_CFG_ECC_INT_ABORT_SIZE 1
#define SMC_ECC1_CFG_ECC_EXTRA_BLOCK_SIZE 1
#define SMC_ECC1_CFG_ECC_EXTRA_BLOCK_SIZE_SIZE 2
#define SMC_ECC1_CFG_RESERVED_31_13_SIZE 19


///////////////////////////////////////////////////////
// Register: ecc1_memcmd0
// ECC Command 0 Register
///////////////////////////////////////////////////////

#define SMC_ECC1_MEMCMD0_OFFSET 0x408UL
#define SMC_ECC1_MEMCMD0_NUM  0x1
#define SMC_ECC1_MEMCMD0_NAND1_WR_CMD_SHIFT    0
#define SMC_ECC1_MEMCMD0_NAND1_RD_CMD_SHIFT    8
#define SMC_ECC1_MEMCMD0_NAND1_RD_END_CMD_SHIFT    16
#define SMC_ECC1_MEMCMD0_NAND1_RD_END_CMD_V_SHIFT    24
#define SMC_ECC1_MEMCMD0_RESERVED_31_25_SHIFT    25

#define SMC_ECC1_MEMCMD0_NAND1_WR_CMD_SIZE 8
#define SMC_ECC1_MEMCMD0_NAND1_RD_CMD_SIZE 8
#define SMC_ECC1_MEMCMD0_NAND1_RD_END_CMD_SIZE 8
#define SMC_ECC1_MEMCMD0_NAND1_RD_END_CMD_V_SIZE 1
#define SMC_ECC1_MEMCMD0_RESERVED_31_25_SIZE 7

///////////////////////////////////////////////////////
// Register: ecc1_memcmd1
// ECC Command 1 Register
///////////////////////////////////////////////////////

#define SMC_ECC1_MEMCMD1_OFFSET 0x40CUL
#define SMC_ECC1_MEMCMD1_NUM  0x1

#define SMC_ECC1_MEMCMD1_NAND1_WR_COL_CHANGE_SHIFT    0
#define SMC_ECC1_MEMCMD1_NAND1_RD_COL_CHANGE_SHIFT    8
#define SMC_ECC1_MEMCMD1_NAND1_RD_END_COL_CHANGE_SHIFT    16
#define SMC_ECC1_MEMCMD1_NAND1_RD_END_COL_CHANGE_V_SHIFT    24
#define SMC_ECC1_MEMCMD1_RESERVED_31_25_SHIFT    25
#define SMC_ECC1_MEMCMD1_NAND1_WR_COL_CHANGE_SIZE 8
#define SMC_ECC1_MEMCMD1_NAND1_RD_COL_CHANGE_SIZE 8
#define SMC_ECC1_MEMCMD1_NAND1_RD_END_COL_CHANGE_SIZE 8
#define SMC_ECC1_MEMCMD1_NAND1_RD_END_COL_CHANGE_V_SIZE 1
#define SMC_ECC1_MEMCMD1_RESERVED_31_25_SIZE 7


///////////////////////////////////////////////////////
// Register: ecc1_addr0
// ECC Address 0 Register
///////////////////////////////////////////////////////

#define SMC_ECC1_ADDR0_OFFSET 0x410UL
#define SMC_ECC1_ADDR0_NUM  0x1
#define SMC_ECC1_ADDR0_ECC0_ADD_LOW_SHIFT    0

#define SMC_ECC1_ADDR0_ECC0_ADD_LOW_SIZE 32

///////////////////////////////////////////////////////
// Register: ecc1_addr1
// ECC Address 1 Register
///////////////////////////////////////////////////////

#define SMC_ECC1_ADDR1_OFFSET 0x414UL
#define SMC_ECC1_ADDR1_NUM  0x1

#define SMC_ECC1_ADDR1_ECC1_ADD_HIGH_SHIFT    0
#define SMC_ECC1_ADDR1_RESERVED_31_24_SHIFT    24
#define SMC_ECC1_ADDR1_ECC1_ADD_HIGH_SIZE 24
#define SMC_ECC1_ADDR1_RESERVED_31_24_SIZE 8


///////////////////////////////////////////////////////
// Register: ecc1_block0
// ECC Block Registers
///////////////////////////////////////////////////////

#define SMC_ECC1_BLOCK0_OFFSET 0x418UL
#define SMC_ECC1_BLOCK0_NUM  0x1
#define SMC_ECC1_BLOCK0_ECC1_BLOCK0_VAL_SHIFT    0
#define SMC_ECC1_BLOCK0_RESERVED_26_24_SHIFT    24
#define SMC_ECC1_BLOCK0_ECC1_0_CORRECT_SHIFT    27
#define SMC_ECC1_BLOCK0_ECC1_0_FAIL_SHIFT    28
#define SMC_ECC1_BLOCK0_ECC1_0_READ_SHIFT    29
#define SMC_ECC1_BLOCK0_ECC1_0_VALID_SHIFT    30
#define SMC_ECC1_BLOCK0_ECC1_0_INT_SHIFT    31

#define SMC_ECC1_BLOCK0_ECC1_BLOCK0_VAL_SIZE 24
#define SMC_ECC1_BLOCK0_RESERVED_26_24_SIZE 3
#define SMC_ECC1_BLOCK0_ECC1_0_CORRECT_SIZE 1
#define SMC_ECC1_BLOCK0_ECC1_0_FAIL_SIZE 1
#define SMC_ECC1_BLOCK0_ECC1_0_READ_SIZE 1
#define SMC_ECC1_BLOCK0_ECC1_0_VALID_SIZE 1
#define SMC_ECC1_BLOCK0_ECC1_0_INT_SIZE 1

///////////////////////////////////////////////////////
// Register: ecc1_block1
// ECC Block Registers
///////////////////////////////////////////////////////

#define SMC_ECC1_BLOCK1_OFFSET 0x41CUL
#define SMC_ECC1_BLOCK1_NUM  0x1

#define SMC_ECC1_BLOCK1_ECC1_BLOCK1_VAL_SHIFT    0
#define SMC_ECC1_BLOCK1_RESERVED_26_24_SHIFT    24
#define SMC_ECC1_BLOCK1_ECC1_1_CORRECT_SHIFT    27
#define SMC_ECC1_BLOCK1_ECC1_1_FAIL_SHIFT    28
#define SMC_ECC1_BLOCK1_ECC1_1_READ_SHIFT    29
#define SMC_ECC1_BLOCK1_ECC1_1_VALID_SHIFT    30
#define SMC_ECC1_BLOCK1_ECC1_1_INT_SHIFT    31
#define SMC_ECC1_BLOCK1_ECC1_BLOCK1_VAL_SIZE 24
#define SMC_ECC1_BLOCK1_RESERVED_26_24_SIZE 3
#define SMC_ECC1_BLOCK1_ECC1_1_CORRECT_SIZE 1
#define SMC_ECC1_BLOCK1_ECC1_1_FAIL_SIZE 1
#define SMC_ECC1_BLOCK1_ECC1_1_READ_SIZE 1
#define SMC_ECC1_BLOCK1_ECC1_1_VALID_SIZE 1
#define SMC_ECC1_BLOCK1_ECC1_1_INT_SIZE 1


///////////////////////////////////////////////////////
// Register: ecc1_block2
// ECC Block Registers
///////////////////////////////////////////////////////

#define SMC_ECC1_BLOCK2_OFFSET 0x420UL
#define SMC_ECC1_BLOCK2_NUM  0x1
#define SMC_ECC1_BLOCK2_ECC1_BLOCK2_VAL_SHIFT    0
#define SMC_ECC1_BLOCK2_RESERVED_26_24_SHIFT    24
#define SMC_ECC1_BLOCK2_ECC1_2_CORRECT_SHIFT    27
#define SMC_ECC1_BLOCK2_ECC1_2_FAIL_SHIFT    28
#define SMC_ECC1_BLOCK2_ECC1_2_READ_SHIFT    29
#define SMC_ECC1_BLOCK2_ECC1_2_VALID_SHIFT    30
#define SMC_ECC1_BLOCK2_ECC1_2_INT_SHIFT    31

#define SMC_ECC1_BLOCK2_ECC1_BLOCK2_VAL_SIZE 24
#define SMC_ECC1_BLOCK2_RESERVED_26_24_SIZE 3
#define SMC_ECC1_BLOCK2_ECC1_2_CORRECT_SIZE 1
#define SMC_ECC1_BLOCK2_ECC1_2_FAIL_SIZE 1
#define SMC_ECC1_BLOCK2_ECC1_2_READ_SIZE 1
#define SMC_ECC1_BLOCK2_ECC1_2_VALID_SIZE 1
#define SMC_ECC1_BLOCK2_ECC1_2_INT_SIZE 1

///////////////////////////////////////////////////////
// Register: ecc1_block3
// ECC Block Registers
///////////////////////////////////////////////////////

#define SMC_ECC1_BLOCK3_OFFSET 0x424UL
#define SMC_ECC1_BLOCK3_NUM  0x1

#define SMC_ECC1_BLOCK3_ECC1_BLOCK3_VAL_SHIFT    0
#define SMC_ECC1_BLOCK3_RESERVED_26_24_SHIFT    24
#define SMC_ECC1_BLOCK3_ECC1_3_CORRECT_SHIFT    27
#define SMC_ECC1_BLOCK3_ECC1_3_FAIL_SHIFT    28
#define SMC_ECC1_BLOCK3_ECC1_3_READ_SHIFT    29
#define SMC_ECC1_BLOCK3_ECC1_3_VALID_SHIFT    30
#define SMC_ECC1_BLOCK3_ECC1_3_INT_SHIFT    31
#define SMC_ECC1_BLOCK3_ECC1_BLOCK3_VAL_SIZE 24
#define SMC_ECC1_BLOCK3_RESERVED_26_24_SIZE 3
#define SMC_ECC1_BLOCK3_ECC1_3_CORRECT_SIZE 1
#define SMC_ECC1_BLOCK3_ECC1_3_FAIL_SIZE 1
#define SMC_ECC1_BLOCK3_ECC1_3_READ_SIZE 1
#define SMC_ECC1_BLOCK3_ECC1_3_VALID_SIZE 1
#define SMC_ECC1_BLOCK3_ECC1_3_INT_SIZE 1


///////////////////////////////////////////////////////
// Register: ecc1_extra_block
// ECC Extra Block Register
///////////////////////////////////////////////////////

#define SMC_ECC1_EXTRA_BLOCK_OFFSET 0x428UL
#define SMC_ECC1_EXTRA_BLOCK_NUM  0x1
#define SMC_ECC1_EXTRA_BLOCK_ECC1_EXTRA_VAL_SHIFT    0
#define SMC_ECC1_EXTRA_BLOCK_RESERVED_26_24_SHIFT    24
#define SMC_ECC1_EXTRA_BLOCK_ECC1_E_CORRECT_SHIFT    27
#define SMC_ECC1_EXTRA_BLOCK_ECC1_E_FAIL_SHIFT    28
#define SMC_ECC1_EXTRA_BLOCK_ECC1_E_READ_SHIFT    29
#define SMC_ECC1_EXTRA_BLOCK_ECC1_E_VALID_SHIFT    30
#define SMC_ECC1_EXTRA_BLOCK_ECC1_E_INT_SHIFT    31

#define SMC_ECC1_EXTRA_BLOCK_ECC1_EXTRA_VAL_SIZE 24
#define SMC_ECC1_EXTRA_BLOCK_RESERVED_26_24_SIZE 3
#define SMC_ECC1_EXTRA_BLOCK_ECC1_E_CORRECT_SIZE 1
#define SMC_ECC1_EXTRA_BLOCK_ECC1_E_FAIL_SIZE 1
#define SMC_ECC1_EXTRA_BLOCK_ECC1_E_READ_SIZE 1
#define SMC_ECC1_EXTRA_BLOCK_ECC1_E_VALID_SIZE 1
#define SMC_ECC1_EXTRA_BLOCK_ECC1_E_INT_SIZE 1

///////////////////////////////////////////////////////
// Register: int_cfg
// Integration Configuration Register
///////////////////////////////////////////////////////

#define SMC_INT_CFG_OFFSET 0xE00UL
#define SMC_INT_CFG_NUM  0x1

#define SMC_INT_CFG_INT_TEST_EN_SHIFT    0
#define SMC_INT_CFG_RESERVED_31_1_SHIFT    1
#define SMC_INT_CFG_INT_TEST_EN_SIZE 1
#define SMC_INT_CFG_RESERVED_31_1_SIZE 31


///////////////////////////////////////////////////////
// Register: int_inputs
// Integration Inputs Register
///////////////////////////////////////////////////////

#define SMC_INT_INPUTS_OFFSET 0xE04UL
#define SMC_INT_INPUTS_NUM  0x1
#define SMC_INT_INPUTS_CSYSREQ_SHIFT    0
#define SMC_INT_INPUTS_USE_EBI_SHIFT    1
#define SMC_INT_INPUTS_EBIGRANT0_SHIFT    2
#define SMC_INT_INPUTS_EBIBACKOFF0_SHIFT    3
#define SMC_INT_INPUTS_ASYNC0_SHIFT    4
#define SMC_INT_INPUTS_MSYNC0_SHIFT    5
#define SMC_INT_INPUTS_EBIGRANT1_SHIFT    6
#define SMC_INT_INPUTS_EBIBACKOFF1_SHIFT    7
#define SMC_INT_INPUTS_ASYNC1_SHIFT    8
#define SMC_INT_INPUTS_MSYNC1_SHIFT    9
#define SMC_INT_INPUTS_RESERVED_31_10_SHIFT    10

#define SMC_INT_INPUTS_CSYSREQ_SIZE 1
#define SMC_INT_INPUTS_USE_EBI_SIZE 1
#define SMC_INT_INPUTS_EBIGRANT0_SIZE 1
#define SMC_INT_INPUTS_EBIBACKOFF0_SIZE 1
#define SMC_INT_INPUTS_ASYNC0_SIZE 1
#define SMC_INT_INPUTS_MSYNC0_SIZE 1
#define SMC_INT_INPUTS_EBIGRANT1_SIZE 1
#define SMC_INT_INPUTS_EBIBACKOFF1_SIZE 1
#define SMC_INT_INPUTS_ASYNC1_SIZE 1
#define SMC_INT_INPUTS_MSYNC1_SIZE 1
#define SMC_INT_INPUTS_RESERVED_31_10_SIZE 22

///////////////////////////////////////////////////////
// Register: int_outputs
// Integration Outputs Register
///////////////////////////////////////////////////////

#define SMC_INT_OUTPUTS_OFFSET 0xE08UL
#define SMC_INT_OUTPUTS_NUM  0x1

#define SMC_INT_OUTPUTS_CACTIVE_INT_SHIFT    0
#define SMC_INT_OUTPUTS_CSYSACK_INT_SHIFT    1
#define SMC_INT_OUTPUTS_EBIREQ0_INT_SHIFT    2
#define SMC_INT_OUTPUTS_EBIREQ1_INT_SHIFT    3
#define SMC_INT_OUTPUTS_SMC_INT_INT_SHIFT    4
#define SMC_INT_OUTPUTS_SMC_INT0_INT_SHIFT    5
#define SMC_INT_OUTPUTS_SMC_INT1_INT_SHIFT    6
#define SMC_INT_OUTPUTS_ECC_INT0_INT_SHIFT    7
#define SMC_INT_OUTPUTS_ECC_INT1_INT_SHIFT    8
#define SMC_INT_OUTPUTS_RESERVED_31_9_SHIFT    9
#define SMC_INT_OUTPUTS_CACTIVE_INT_SIZE 1
#define SMC_INT_OUTPUTS_CSYSACK_INT_SIZE 1
#define SMC_INT_OUTPUTS_EBIREQ0_INT_SIZE 1
#define SMC_INT_OUTPUTS_EBIREQ1_INT_SIZE 1
#define SMC_INT_OUTPUTS_SMC_INT_INT_SIZE 1
#define SMC_INT_OUTPUTS_SMC_INT0_INT_SIZE 1
#define SMC_INT_OUTPUTS_SMC_INT1_INT_SIZE 1
#define SMC_INT_OUTPUTS_ECC_INT0_INT_SIZE 1
#define SMC_INT_OUTPUTS_ECC_INT1_INT_SIZE 1
#define SMC_INT_OUTPUTS_RESERVED_31_9_SIZE 23


///////////////////////////////////////////////////////
// Register: periph_id_0
// Peripheral Identification Registers 0
///////////////////////////////////////////////////////

#define SMC_PERIPH_ID_0_OFFSET 0xFE0UL
#define SMC_PERIPH_ID_0_NUM  0x1
#define SMC_PERIPH_ID_0_PART_NUMBER_0_SHIFT    0
#define SMC_PERIPH_ID_0_RESERVED_31_8_SHIFT    8

#define SMC_PERIPH_ID_0_PART_NUMBER_0_SIZE 8
#define SMC_PERIPH_ID_0_RESERVED_31_8_SIZE 24

///////////////////////////////////////////////////////
// Register: periph_id_1
// Peripheral Identification Registers 1
///////////////////////////////////////////////////////

#define SMC_PERIPH_ID_1_OFFSET 0xFE4UL
#define SMC_PERIPH_ID_1_NUM  0x1

#define SMC_PERIPH_ID_1_PART_NUMBER_1_SHIFT    0
#define SMC_PERIPH_ID_1_DESIGNER_0_SHIFT    4
#define SMC_PERIPH_ID_1_RESERVED_31_8_SHIFT    8
#define SMC_PERIPH_ID_1_PART_NUMBER_1_SIZE 4
#define SMC_PERIPH_ID_1_DESIGNER_0_SIZE 4
#define SMC_PERIPH_ID_1_RESERVED_31_8_SIZE 24


///////////////////////////////////////////////////////
// Register: periph_id_2
// Peripheral Identification Registers 2
///////////////////////////////////////////////////////

#define SMC_PERIPH_ID_2_OFFSET 0xFE8UL
#define SMC_PERIPH_ID_2_NUM  0x1
#define SMC_PERIPH_ID_2_DESIGNER_1_SHIFT    0
#define SMC_PERIPH_ID_2_REVISION_SHIFT    4
#define SMC_PERIPH_ID_2_RESERVED_31_8_SHIFT    8

#define SMC_PERIPH_ID_2_DESIGNER_1_SIZE 4
#define SMC_PERIPH_ID_2_REVISION_SIZE 4
#define SMC_PERIPH_ID_2_RESERVED_31_8_SIZE 24

///////////////////////////////////////////////////////
// Register: periph_id_3
// Peripheral Identification Registers 3
///////////////////////////////////////////////////////

#define SMC_PERIPH_ID_3_OFFSET 0xFECUL
#define SMC_PERIPH_ID_3_NUM  0x1

#define SMC_PERIPH_ID_3_RESERVED_31_0_SHIFT    0
#define SMC_PERIPH_ID_3_RESERVED_31_0_SIZE 32


///////////////////////////////////////////////////////
// Register: pcell_id_0
// CoreLink Identification Register 0
///////////////////////////////////////////////////////

#define SMC_PCELL_ID_0_OFFSET 0xFF0UL
#define SMC_PCELL_ID_0_NUM  0x1
#define SMC_PCELL_ID_0_PCELL_ID_0_SHIFT    0
#define SMC_PCELL_ID_0_RESERVED_31_8_SHIFT    8

#define SMC_PCELL_ID_0_PCELL_ID_0_SIZE 8
#define SMC_PCELL_ID_0_RESERVED_31_8_SIZE 24

///////////////////////////////////////////////////////
// Register: pcell_id_1
// CoreLink Identification Register 1
///////////////////////////////////////////////////////

#define SMC_PCELL_ID_1_OFFSET 0xFF4UL
#define SMC_PCELL_ID_1_NUM  0x1

#define SMC_PCELL_ID_1_PCELL_ID_1_SHIFT    0
#define SMC_PCELL_ID_1_RESERVED_31_8_SHIFT    8
#define SMC_PCELL_ID_1_PCELL_ID_1_SIZE 8
#define SMC_PCELL_ID_1_RESERVED_31_8_SIZE 24


///////////////////////////////////////////////////////
// Register: pcell_id_2
// CoreLink Identification Register 2
///////////////////////////////////////////////////////

#define SMC_PCELL_ID_2_OFFSET 0xFF8UL
#define SMC_PCELL_ID_2_NUM  0x1
#define SMC_PCELL_ID_2_PCELL_ID_2_SHIFT    0
#define SMC_PCELL_ID_2_RESERVED_31_8_SHIFT    8

#define SMC_PCELL_ID_2_PCELL_ID_2_SIZE 8
#define SMC_PCELL_ID_2_RESERVED_31_8_SIZE 24

///////////////////////////////////////////////////////
// Register: pcell_id_3
// CoreLink Identification Register 3
///////////////////////////////////////////////////////

#define SMC_PCELL_ID_3_OFFSET 0xFFCUL
#define SMC_PCELL_ID_3_NUM  0x1

#define SMC_PCELL_ID_3_PCELL_ID_3_SHIFT    0
#define SMC_PCELL_ID_3_RESERVED_31_8_SHIFT    8
#define SMC_PCELL_ID_3_PCELL_ID_3_SIZE 8
#define SMC_PCELL_ID_3_RESERVED_31_8_SIZE 24


/**************************** Type Definitions ******************************/
typedef struct {
	AL_U16 					DeviceId;		/**< Unique ID  of device */
	AL_UINTPTR 				NandBaseAddr;	/**< Base address of the device */
	AL_UINTPTR 				SmcBaseAddr;	/**< Base address of the device */
} AL_SMC_HwConfigStruct;

#ifdef __cplusplus
}
#endif

#endif
