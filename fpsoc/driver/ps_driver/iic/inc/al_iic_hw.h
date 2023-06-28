#ifndef AL_IIC_HW_H
#define AL_IIC_HW_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files ********************************/
#include "al_core.h"

/************************** Constant Definitions ****************************/

/**************************** Type Definitions ******************************/

/***************** Macros (Inline Functions) Definitions ********************/

///////////////////////////////////////////////////////
// Register: IC_CON
// I2C Control Register
///////////////////////////////////////////////////////
#define I2C__IC_CON__OFFSET   0x0ULL
#define I2C__IC_CON__NUM      0x1

#define I2C__IC_CON__MASTER_MODE__SHIFT    0
#define I2C__IC_CON__SPEED__SHIFT    1
#define I2C__IC_CON__IC_10BITADDR_SLAVE__SHIFT    3
#define I2C__IC_CON__IC_10BITADDR_MASTER__SHIFT    4
#define I2C__IC_CON__IC_RESTART_EN__SHIFT    5
#define I2C__IC_CON__IC_SLAVE_DISABLE__SHIFT    6
#define I2C__IC_CON__STOP_DET_IFADDRESSED__SHIFT    7
#define I2C__IC_CON__TX_EMPTY_CTRL__SHIFT    8
#define I2C__IC_CON__RX_FIFO_FULL_HLD_CTRL__SHIFT    9
#define I2C__IC_CON__STOP_DET_IF_MASTER_ACTIVE__SHIFT    10
#define I2C__IC_CON__BUS_CLEAR_FEATURE_CTRL__SHIFT    11
#define I2C__IC_CON__RESERVED_31_12__SHIFT    12

#define I2C__IC_CON__SPEED__SIZE    2

///////////////////////////////////////////////////////
// Register: IC_TAR
// I2C Target Address Register
///////////////////////////////////////////////////////
#define I2C__IC_TAR__OFFSET 0x4ULL
#define I2C__IC_TAR__NUM  0x1

#define I2C__IC_TAR__IC_TAR__SHIFT    0
#define I2C__IC_TAR__GC_OR_START__SHIFT    10
#define I2C__IC_TAR__SPECIAL__SHIFT    11
#define I2C__IC_TAR__RESERVED_31_12__SHIFT    12

#define I2C__IC_TAR__IC_TAR__SIZE    10

///////////////////////////////////////////////////////
// Register: IC_SAR
// I2C Slave Address Register
///////////////////////////////////////////////////////
#define I2C__IC_SAR__OFFSET 0x8ULL
#define I2C__IC_SAR__NUM  0x1

#define I2C__IC_SAR__IC_SAR__SHIFT    0
#define I2C__IC_SAR__RESERVED_31_10__SHIFT    10

#define I2C__IC_SAR__IC_SAR__SIZE    10

///////////////////////////////////////////////////////
// Register: IC_DATA_CMD
// I2C Rx/Tx Data Buffer and Command Register; this is the register the CPU writes to when filling the TX FIFO and the CPU reads from when retrieving bytes from RX FIFO.
///////////////////////////////////////////////////////

#define I2C__IC_DATA_CMD__OFFSET 0x10ULL
#define I2C__IC_DATA_CMD__NUM  0x1

#define I2C__IC_DATA_CMD__DAT__SHIFT    0
#define I2C__IC_DATA_CMD__CMD__SHIFT    8
#define I2C__IC_DATA_CMD__STOP__SHIFT    9
#define I2C__IC_DATA_CMD__RESTART__SHIFT    10
#define I2C__IC_DATA_CMD__RESERVED_31_11__SHIFT    11

#define I2C__IC_DATA_CMD__DAT__SIZE    8
#define I2C__IC_DATA_CMD__SIZE         11

///////////////////////////////////////////////////////
// Register: IC_SS_SCL_HCNT
// Standard Speed I2C Clock SCL High Count Register
///////////////////////////////////////////////////////

#define I2C__IC_SS_SCL_HCNT__OFFSET 0x14ULL
#define I2C__IC_SS_SCL_HCNT__NUM  0x1

#define I2C__IC_SS_SCL_HCNT__IC_SS_SCL_HCNT__SHIFT    0
#define I2C__IC_SS_SCL_HCNT__RESERVED_31_16__SHIFT    16

#define I2C__IC_SS_SCL_HCNT__IC_SS_SCL_HCNT__SIZE  16

///////////////////////////////////////////////////////
// Register: IC_SS_SCL_LCNT
// Standard Speed I2C Clock SCL Low Count Register 
///////////////////////////////////////////////////////

#define I2C__IC_SS_SCL_LCNT__OFFSET 0x18ULL
#define I2C__IC_SS_SCL_LCNT__NUM  0x1

#define I2C__IC_SS_SCL_LCNT__IC_SS_SCL_LCNT__SHIFT    0
#define I2C__IC_SS_SCL_LCNT__RESERVED_31_16__SHIFT    16

#define I2C__IC_SS_SCL_LCNT__IC_SS_SCL_LCNT__SIZE    16

///////////////////////////////////////////////////////
// Register: IC_FS_SCL_HCNT
// Fast Mode or Fast Mode Plus I2C Clock SCL High Count Register
///////////////////////////////////////////////////////

#define I2C__IC_FS_SCL_HCNT__OFFSET 0x1CULL
#define I2C__IC_FS_SCL_HCNT__NUM  0x1

#define I2C__IC_FS_SCL_HCNT__IC_FS_SCL_HCNT__SHIFT    0
#define I2C__IC_FS_SCL_HCNT__RESERVED_31_16__SHIFT    16

#define I2C__IC_FS_SCL_HCNT__IC_FS_SCL_HCNT__SIZE    16

///////////////////////////////////////////////////////
// Register: IC_FS_SCL_LCNT
// Fast Mode or Fast Mode Plus I2C Clock SCL Low Count Register
///////////////////////////////////////////////////////

#define I2C__IC_FS_SCL_LCNT__OFFSET 0x20ULL
#define I2C__IC_FS_SCL_LCNT__NUM  0x1

#define I2C__IC_FS_SCL_LCNT__IC_FS_SCL_LCNT__SHIFT    0
#define I2C__IC_FS_SCL_LCNT__RESERVED_31_16__SHIFT    16

#define I2C__IC_FS_SCL_LCNT__IC_FS_SCL_LCNT__SIZE    16

///////////////////////////////////////////////////////
// Register: IC_HS_SCL_HCNT
// Fast Mode or Fast Mode Plus I2C Clock SCL High Count Register
///////////////////////////////////////////////////////

#define I2C__IC_HS_SCL_HCNT__OFFSET 0x24ULL
#define I2C__IC_HS_SCL_HCNT__NUM  0x1

#define I2C__IC_HS_SCL_HCNT__IC_FS_SCL_HCNT__SHIFT    0
#define I2C__IC_HS_SCL_HCNT__RESERVED_31_16__SHIFT    16

#define I2C__IC_HS_SCL_HCNT__IC_FS_SCL_HCNT__SIZE    16

///////////////////////////////////////////////////////
// Register: IC_FS_SCL_LCNT
// Fast Mode or Fast Mode Plus I2C Clock SCL Low Count Register
///////////////////////////////////////////////////////

#define I2C__IC_HS_SCL_LCNT__OFFSET 0x28ULL
#define I2C__IC_HS_SCL_LCNT__NUM  0x1

#define I2C__IC_HS_SCL_LCNT__IC_FS_SCL_LCNT__SHIFT    0
#define I2C__IC_HS_SCL_LCNT__RESERVED_31_16__SHIFT    16

#define I2C__IC_HS_SCL_LCNT__IC_FS_SCL_LCNT__SIZE    16

///////////////////////////////////////////////////////
// Register: IC_INTR_STAT
// I2C Interrupt Status Register
///////////////////////////////////////////////////////

#define I2C__IC_INTR_STAT__OFFSET 0x2CULL
#define I2C__IC_INTR_STAT__NUM  0x1

#define I2C__IC_INTR_STAT__R_RX_UNDER__SHIFT    0
#define I2C__IC_INTR_STAT__R_RX_OVER__SHIFT    1
#define I2C__IC_INTR_STAT__R_RX_FULL__SHIFT    2
#define I2C__IC_INTR_STAT__R_TX_OVER__SHIFT    3
#define I2C__IC_INTR_STAT__R_TX_EMPTY__SHIFT    4
#define I2C__IC_INTR_STAT__R_RD_REQ__SHIFT    5
#define I2C__IC_INTR_STAT__R_TX_ABRT__SHIFT    6
#define I2C__IC_INTR_STAT__R_RX_DONE__SHIFT    7
#define I2C__IC_INTR_STAT__R_ACTIVITY__SHIFT    8
#define I2C__IC_INTR_STAT__R_STOP_DET__SHIFT    9
#define I2C__IC_INTR_STAT__R_START_DET__SHIFT    10
#define I2C__IC_INTR_STAT__R_GEN_CALL__SHIFT    11
#define I2C__IC_INTR_STAT__R_RESTART_DET__SHIFT    12
#define I2C__IC_INTR_STAT__R_MASTER_ON_HOLD__SHIFT    13
#define I2C__IC_INTR_STAT__R_SCL_STUCK_AT_LOW__SHIFT    14
#define I2C__IC_INTR_STAT__RESERVED_31_15__SHIFT    15


///////////////////////////////////////////////////////
// Register: IC_INTR_MASK
// I2C Interrupt Mask Register
///////////////////////////////////////////////////////

#define I2C__IC_INTR_MASK__OFFSET 0x30ULL
#define I2C__IC_INTR_MASK__NUM  0x1

#define I2C__IC_INTR_MASK__M_RX_UNDER__SHIFT    0
#define I2C__IC_INTR_MASK__M_RX_OVER__SHIFT    1
#define I2C__IC_INTR_MASK__M_RX_FULL__SHIFT    2
#define I2C__IC_INTR_MASK__M_TX_OVER__SHIFT    3
#define I2C__IC_INTR_MASK__M_TX_EMPTY__SHIFT    4
#define I2C__IC_INTR_MASK__M_RD_REQ__SHIFT    5
#define I2C__IC_INTR_MASK__M_TX_ABRT__SHIFT    6
#define I2C__IC_INTR_MASK__M_RX_DONE__SHIFT    7
#define I2C__IC_INTR_MASK__M_ACTIVITY__SHIFT    8
#define I2C__IC_INTR_MASK__M_STOP_DET__SHIFT    9
#define I2C__IC_INTR_MASK__M_START_DET__SHIFT    10
#define I2C__IC_INTR_MASK__M_GEN_CALL__SHIFT    11
#define I2C__IC_INTR_MASK__M_RESTART_DET__SHIFT    12
#define I2C__IC_INTR_MASK__M_MASTER_ON_HOLD_READ_ONLY__SHIFT    13
#define I2C__IC_INTR_MASK__M_SCL_STUCK_AT_LOW__SHIFT    14
#define I2C__IC_INTR_MASK__RESERVED_31_15__SHIFT    15

#define I2C__IC_INTR_MASK__SIZE                 15

///////////////////////////////////////////////////////
// Register: IC_RAW_INTR_STAT
//  I2C Raw Interrupt Status Register
///////////////////////////////////////////////////////

#define I2C__IC_RAW_INTR_STAT__OFFSET 0x34ULL
#define I2C__IC_RAW_INTR_STAT__NUM  0x1

#define I2C__IC_RAW_INTR_STAT__RX_UNDER__SHIFT    0
#define I2C__IC_RAW_INTR_STAT__RX_OVER__SHIFT    1
#define I2C__IC_RAW_INTR_STAT__RX_FULL__SHIFT    2
#define I2C__IC_RAW_INTR_STAT__TX_OVER__SHIFT    3
#define I2C__IC_RAW_INTR_STAT__TX_EMPTY__SHIFT    4
#define I2C__IC_RAW_INTR_STAT__RD_REQ__SHIFT    5
#define I2C__IC_RAW_INTR_STAT__TX_ABRT__SHIFT    6
#define I2C__IC_RAW_INTR_STAT__RX_DONE__SHIFT    7
#define I2C__IC_RAW_INTR_STAT__ACTIVITY__SHIFT    8
#define I2C__IC_RAW_INTR_STAT__STOP_DET__SHIFT    9
#define I2C__IC_RAW_INTR_STAT__START_DET__SHIFT    10
#define I2C__IC_RAW_INTR_STAT__GEN_CALL__SHIFT    11
#define I2C__IC_RAW_INTR_STAT__RESTART_DET__SHIFT    12
#define I2C__IC_RAW_INTR_STAT__MASTER_ON_HOLD__SHIFT    13
#define I2C__IC_RAW_INTR_STAT__SCL_STUCK_AT_LOW__SHIFT    14
#define I2C__IC_RAW_INTR_STAT__RESERVED_31_15__SHIFT    15


///////////////////////////////////////////////////////
// Register: IC_RX_TL
// I2C Receive FIFO Threshold Register
///////////////////////////////////////////////////////

#define I2C__IC_RX_TL__OFFSET 0x38ULL
#define I2C__IC_RX_TL__NUM  0x1

#define I2C__IC_RX_TL__RX_TL__SHIFT    0
#define I2C__IC_RX_TL__RESERVED_31_8__SHIFT    8

#define I2C__IC_RX_TL__RX_TL__SIZE    8

///////////////////////////////////////////////////////
// Register: IC_TX_TL
// I2C Transmit FIFO Threshold Register
///////////////////////////////////////////////////////

#define I2C__IC_TX_TL__OFFSET 0x3CULL
#define I2C__IC_TX_TL__NUM  0x1

#define I2C__IC_TX_TL__TX_TL__SHIFT    0
#define I2C__IC_TX_TL__RESERVED_31_8__SHIFT    8

#define I2C__IC_TX_TL__TX_TL__SIZE    8

///////////////////////////////////////////////////////
// Register: IC_CLR_INTR
// Clear Combined and Individual Interrupt Register
///////////////////////////////////////////////////////

#define I2C__IC_CLR_INTR__OFFSET 0x40ULL
#define I2C__IC_CLR_INTR__NUM  0x1

#define I2C__IC_CLR_INTR__CLR_INTR__SHIFT    0
#define I2C__IC_CLR_INTR__RESERVED_31_1__SHIFT    1

///////////////////////////////////////////////////////
// Register: IC_CLR_RX_UNDER
// Clear RX_UNDER Interrupt Register
///////////////////////////////////////////////////////

#define I2C__IC_CLR_RX_UNDER__OFFSET 0x44ULL
#define I2C__IC_CLR_RX_UNDER__NUM  0x1

#define I2C__IC_CLR_RX_UNDER__CLR_RX_UNDER__SHIFT    0
#define I2C__IC_CLR_RX_UNDER__RESERVED_31_1__SHIFT    1

///////////////////////////////////////////////////////
// Register: IC_CLR_RX_OVER
// Clear RX_OVER Interrupt Register
///////////////////////////////////////////////////////

#define I2C__IC_CLR_RX_OVER__OFFSET 0x48ULL
#define I2C__IC_CLR_RX_OVER__NUM  0x1

#define I2C__IC_CLR_RX_OVER__CLR_RX_OVER__SHIFT    0
#define I2C__IC_CLR_RX_OVER__RESERVED_31_1__SHIFT    1

///////////////////////////////////////////////////////
// Register: IC_CLR_TX_OVER
// Clear TX_OVER Interrupt Register
///////////////////////////////////////////////////////

#define I2C__IC_CLR_TX_OVER__OFFSET 0x4CULL
#define I2C__IC_CLR_TX_OVER__NUM  0x1

#define I2C__IC_CLR_TX_OVER__CLR_TX_OVER__SHIFT    0
#define I2C__IC_CLR_TX_OVER__RESERVED_31_1__SHIFT    1

///////////////////////////////////////////////////////
// Register: IC_CLR_RD_REQ
// Clear RD_REQ Interrupt Register
///////////////////////////////////////////////////////

#define I2C__IC_CLR_RD_REQ__OFFSET 0x50ULL
#define I2C__IC_CLR_RD_REQ__NUM  0x1

#define I2C__IC_CLR_RD_REQ__CLR_RD_REQ__SHIFT    0
#define I2C__IC_CLR_RD_REQ__RESERVED_31_1__SHIFT    1

///////////////////////////////////////////////////////
// Register: IC_CLR_TX_ABRT
// Clear TX_ABRT Interrupt Register
///////////////////////////////////////////////////////
#define I2C__IC_CLR_TX_ABRT__OFFSET 0x54ULL

#define I2C__IC_CLR_TX_ABRT__NUM  0x1

#define I2C__IC_CLR_TX_ABRT__CLR_TX_ABRT__SHIFT    0
#define I2C__IC_CLR_TX_ABRT__RESERVED_31_1__SHIFT    1

///////////////////////////////////////////////////////
// Register: IC_CLR_RX_DONE
// Clear RX_DONE Interrupt Register
///////////////////////////////////////////////////////

#define I2C__IC_CLR_RX_DONE__OFFSET 0x58ULL
#define I2C__IC_CLR_RX_DONE__NUM  0x1

#define I2C__IC_CLR_RX_DONE__CLR_RX_DONE__SHIFT    0
#define I2C__IC_CLR_RX_DONE__RESERVED_31_1__SHIFT    1

///////////////////////////////////////////////////////
// Register: IC_CLR_ACTIVITY
// Clear ACTIVITY Interrupt Register
///////////////////////////////////////////////////////

#define I2C__IC_CLR_ACTIVITY__OFFSET 0x5CULL
#define I2C__IC_CLR_ACTIVITY__NUM  0x1

#define I2C__IC_CLR_ACTIVITY__CLR_ACTIVITY__SHIFT    0
#define I2C__IC_CLR_ACTIVITY__RESERVED_31_1__SHIFT    1

///////////////////////////////////////////////////////
// Register: IC_CLR_STOP_DET
// Clear STOP_DET Interrupt Register
///////////////////////////////////////////////////////

#define I2C__IC_CLR_STOP_DET__OFFSET 0x60ULL
#define I2C__IC_CLR_STOP_DET__NUM  0x1

#define I2C__IC_CLR_STOP_DET__CLR_STOP_DET__SHIFT    0
#define I2C__IC_CLR_STOP_DET__RESERVED_31_1__SHIFT    1

///////////////////////////////////////////////////////
// Register: IC_CLR_START_DET
// Clear START_DET Interrupt Register
///////////////////////////////////////////////////////
#define I2C__IC_CLR_START_DET__OFFSET 0x64ULL
#define I2C__IC_CLR_START_DET__NUM  0x1

#define I2C__IC_CLR_START_DET__CLR_START_DET__SHIFT    0
#define I2C__IC_CLR_START_DET__RESERVED_31_1__SHIFT    1

///////////////////////////////////////////////////////
// Register: IC_CLR_GEN_CALL
// Clear GEN_CALL Interrupt Register
///////////////////////////////////////////////////////

#define I2C__IC_CLR_GEN_CALL__OFFSET  0x68ULL
#define I2C__IC_CLR_GEN_CALL__NUM  0x1

#define I2C__IC_CLR_GEN_CALL__CLR_GEN_CALL__SHIFT    0
#define I2C__IC_CLR_GEN_CALL__RESERVED_31_1__SHIFT    1

///////////////////////////////////////////////////////
// Register: IC_ENABLE
// I2C Enable Register
///////////////////////////////////////////////////////

#define I2C__IC_ENABLE__OFFSET 0x6CULL
#define I2C__IC_ENABLE__NUM  0x1

#define I2C__IC_ENABLE__ENABLE__SHIFT    0
#define I2C__IC_ENABLE__ABORT__SHIFT    1
#define I2C__IC_ENABLE__TX_CMD_BLOCK__SHIFT    2
#define I2C__IC_ENABLE__SDA_STUCK_RECOVERY_ENABLE__SHIFT    3
#define I2C__IC_ENABLE__RESERVED_31_4__SHIFT    4

///////////////////////////////////////////////////////
// Register: IC_STATUS
// I2C Status Register
///////////////////////////////////////////////////////

#define I2C__IC_STATUS__OFFSET 0x70ULL
#define I2C__IC_STATUS__NUM  0x1

#define I2C__IC_STATUS__ACTIVITY__SHIFT    0
#define I2C__IC_STATUS__TFNF__SHIFT    1
#define I2C__IC_STATUS__TFE__SHIFT    2
#define I2C__IC_STATUS__RFNE__SHIFT    3
#define I2C__IC_STATUS__RFF__SHIFT    4
#define I2C__IC_STATUS__MST_ACTIVITY__SHIFT    5
#define I2C__IC_STATUS__SLV_ACTIVITY__SHIFT    6
#define I2C__IC_STATUS__MST_HOLD_TX_FIFO_EMPTY__SHIFT    7
#define I2C__IC_STATUS__MST_HOLD_RX_FIFO_FULL__SHIFT    8
#define I2C__IC_STATUS__SLV_HOLD_TX_FIFO_EMPTY__SHIFT    9
#define I2C__IC_STATUS__SLV_HOLD_RX_FIFO_FULL__SHIFT    10
#define I2C__IC_STATUS__SDA_STUCK_NOT_RECOVERED__SHIFT    11
#define I2C__IC_STATUS__RESERVED_31_12__SHIFT    12

///////////////////////////////////////////////////////
// Register: IC_TXFLR
// I2C Transmit FIFO Level Register
///////////////////////////////////////////////////////

#define I2C__IC_TXFLR__OFFSET 0x74ULL
#define I2C__IC_TXFLR__NUM  0x1

#define I2C__IC_TXFLR__TXFLR__SHIFT    0
#define I2C__IC_TXFLR__RESERVED_31_9__SHIFT    9

///////////////////////////////////////////////////////
// Register: IC_RXFLR
// I2C Receive FIFO Level Register
///////////////////////////////////////////////////////

#define I2C__IC_RXFLR__OFFSET 0x78ULL
#define I2C__IC_RXFLR__NUM  0x1

#define I2C__IC_RXFLR__RXFLR__SHIFT    0
#define I2C__IC_RXFLR__RESERVED_31_9__SHIFT    9

#define I2C__IC_RXFLR__RXFLR__SIZE 9

///////////////////////////////////////////////////////
// Register: IC_SDA_HOLD
// I2C SDA Hold Time Length Register
///////////////////////////////////////////////////////

#define I2C__IC_SDA_HOLD__OFFSET  0x7CULL
#define I2C__IC_SDA_HOLD__NUM  0x1

#define I2C__IC_SDA_HOLD__IC_SDA_TX_HOLD__SHIFT    0
#define I2C__IC_SDA_HOLD__IC_SDA_RX_HOLD__SHIFT    16
#define I2C__IC_SDA_HOLD__RESERVED_31_24__SHIFT    24


#define I2C__IC_SDA_HOLD__IC_SDA_TX_HOLD__SIZE    16
#define I2C__IC_SDA_HOLD__IC_SDA_RX_HOLD__SIZE    8

///////////////////////////////////////////////////////
// Register: IC_TX_ABRT_SOURCE
// I2C Transmit Abort Source Register
///////////////////////////////////////////////////////

#define I2C__IC_TX_ABRT_SOURCE__OFFSET 0x80ULL
#define I2C__IC_TX_ABRT_SOURCE__NUM  0x1

#define I2C__IC_TX_ABRT_SOURCE__ABRT_7B_ADDR_NOACK__SHIFT    0
#define I2C__IC_TX_ABRT_SOURCE__ABRT_10ADDR1_NOACK__SHIFT    1
#define I2C__IC_TX_ABRT_SOURCE__ABRT_10ADDR2_NOACK__SHIFT    2
#define I2C__IC_TX_ABRT_SOURCE__ABRT_TXDATA_NOACK__SHIFT    3
#define I2C__IC_TX_ABRT_SOURCE__ABRT_GCALL_NOACK__SHIFT    4
#define I2C__IC_TX_ABRT_SOURCE__ABRT_GCALL_READ__SHIFT    5
#define I2C__IC_TX_ABRT_SOURCE__ABRT_HS_ACKDET__SHIFT    6
#define I2C__IC_TX_ABRT_SOURCE__ABRT_SBYTE_ACKDET__SHIFT    7
#define I2C__IC_TX_ABRT_SOURCE__ABRT_HS_NORSTRT__SHIFT    8
#define I2C__IC_TX_ABRT_SOURCE__ABRT_SBYTE_NORSTRT__SHIFT    9
#define I2C__IC_TX_ABRT_SOURCE__ABRT_10B_RD_NORSTRT__SHIFT    10
#define I2C__IC_TX_ABRT_SOURCE__ABRT_MASTER_DIS__SHIFT    11
#define I2C__IC_TX_ABRT_SOURCE__ARB_LOST__SHIFT    12
#define I2C__IC_TX_ABRT_SOURCE__ABRT_SLVFLUSH_TXFIFO__SHIFT    13
#define I2C__IC_TX_ABRT_SOURCE__ABRT_SLV_ARBLOST__SHIFT    14
#define I2C__IC_TX_ABRT_SOURCE__ABRT_SLVRD_INTX__SHIFT    15
#define I2C__IC_TX_ABRT_SOURCE__ABRT_USER_ABRT__SHIFT    16
#define I2C__IC_TX_ABRT_SOURCE__ABRT_SDA_STUCK_AT_LOW__SHIFT    17
#define I2C__IC_TX_ABRT_SOURCE__RESERVED_22_18__SHIFT    18
#define I2C__IC_TX_ABRT_SOURCE__TX_FLUSH_CNT__SHIFT    23

#define I2C__IC_TX_ABRT_SOURCE__SIZE                    18

///////////////////////////////////////////////////////
// Register: IC_SLV_DATA_NACK_ONLY
// Generate Slave Data NACK Register
///////////////////////////////////////////////////////

#define I2C__IC_SLV_DATA_NACK_ONLY__OFFSET 0x84ULL
#define I2C__IC_SLV_DATA_NACK_ONLY__NUM  0x1

#define I2C__IC_SLV_DATA_NACK_ONLY__NACK__SHIFT    0
#define I2C__IC_SLV_DATA_NACK_ONLY__RESERVED_31_1__SHIFT    1


///////////////////////////////////////////////////////
// Register: IC_DMA_CR
// DMA Control Register
///////////////////////////////////////////////////////

#define I2C__IC_DMA_CR__OFFSET 0x88ULL
#define I2C__IC_DMA_CR__NUM  0x1

#define I2C__IC_DMA_CR__RDMAE__SHIFT    0
#define I2C__IC_DMA_CR__TDMAE__SHIFT    1
#define I2C__IC_DMA_CR__RESERVED_31_2__SHIFT    2

///////////////////////////////////////////////////////
// Register: IC_DMA_TDLR
// DMA Transmit Data Level Register
///////////////////////////////////////////////////////

#define I2C__IC_DMA_TDLR__OFFSET 0x8CULL
#define I2C__IC_DMA_TDLR__NUM  0x1

#define I2C__IC_DMA_TDLR__DMATDL__SHIFT    0
#define I2C__IC_DMA_TDLR__RESERVED_31_8__SHIFT    8

#define I2C__IC_DMA_TDLR__DMATDL__SIZE    8

///////////////////////////////////////////////////////
// Register: IC_DMA_RDLR
// DMA Receive Data Level Register
///////////////////////////////////////////////////////

#define I2C__IC_DMA_RDLR__ADDR 0x90ULL
#define I2C__IC_DMA_RDLR__NUM  0x1

#define I2C__IC_DMA_RDLR__DMARDL__SHIFT    0
#define I2C__IC_DMA_RDLR__RESERVED_31_8__SHIFT    8

#define I2C__IC_DMA_RDLR__DMARDL__SIZE    8

///////////////////////////////////////////////////////
// Register: IC_SDA_SETUP
// I2C SDA Setup Register
///////////////////////////////////////////////////////

#define I2C__IC_SDA_SETUP__OFFSET 0x94ULL
#define I2C__IC_SDA_SETUP__NUM  0x1

#define I2C__IC_SDA_SETUP__SDA_SETUP__SHIFT    0
#define I2C__IC_SDA_SETUP__RESERVED_31_8__SHIFT    8

#define I2C__IC_SDA_SETUP__SDA_SETUP__SIZE    8

///////////////////////////////////////////////////////
// Register: IC_ACK_GENERAL_CALL
// I2C ACK General Call Register
///////////////////////////////////////////////////////

#define I2C__IC_ACK_GENERAL_CALL__OFFSET 0x98ULL
#define I2C__IC_ACK_GENERAL_CALL__NUM  0x1

#define I2C__IC_ACK_GENERAL_CALL__ACK_GEN_CALL__SHIFT    0
#define I2C__IC_ACK_GENERAL_CALL__RESERVED_31_1__SHIFT    1

///////////////////////////////////////////////////////
// Register: IC_ENABLE_STATUS
// I2C Enable Status Register
///////////////////////////////////////////////////////

#define I2C__IC_ENABLE_STATUS__OFFSET 0x9CULL
#define I2C__IC_ENABLE_STATUS__NUM  0x1

#define I2C__IC_ENABLE_STATUS__IC_EN__SHIFT    0
#define I2C__IC_ENABLE_STATUS__SLV_DISABLED_WHILE_BUSY__SHIFT    1
#define I2C__IC_ENABLE_STATUS__SLV_RX_DATA_LOST__SHIFT    2
#define I2C__IC_ENABLE_STATUS__RESERVED_31_3__SHIFT    3


///////////////////////////////////////////////////////
// Register: IC_FS_SPKLEN
// I2C SS, FS or FM+ spike suppression limit
///////////////////////////////////////////////////////

#define I2C__IC_FS_SPKLEN__OFFSET 0xA0ULL
#define I2C__IC_FS_SPKLEN__NUM  0x1

#define I2C__IC_FS_SPKLEN__IC_FS_SPKLEN__SHIFT    0
#define I2C__IC_FS_SPKLEN__RESERVED_31_8__SHIFT    8

#define I2C__IC_FS_SPKLEN__IC_FS_SPKLEN__SIZE    8

///////////////////////////////////////////////////////
// Register: IC_CLR_RESTART_DET
// Clear RESTART_DET Interrupt Register
///////////////////////////////////////////////////////

#define I2C__IC_CLR_RESTART_DET__OFFSET 0xA8ULL
#define I2C__IC_CLR_RESTART_DET__NUM  0x1

#define I2C__IC_CLR_RESTART_DET__CLR_RESTART_DET__SHIFT    0
#define I2C__IC_CLR_RESTART_DET__RESERVED_31_1__SHIFT    1

///////////////////////////////////////////////////////
// Register: IC_SCL_STUCK_AT_LOW_TIMEOUT
// I2C SCL Stuck at Low Timeout
///////////////////////////////////////////////////////

#define I2C__IC_SCL_STUCK_AT_LOW_TIMEOUT__OFFSET I2C__BASE_ADDR + 0xACULL
#define I2C__IC_SCL_STUCK_AT_LOW_TIMEOUT__NUM  0x1

#define I2C__IC_SCL_STUCK_AT_LOW_TIMEOUT__IC_SCL_STUCK_LOW_TIMEOUT__SHIFT    0

///////////////////////////////////////////////////////
// Register: IC_SDA_STUCK_AT_LOW_TIMEOUT
// I2C SDA Stuck at Low Timeout
///////////////////////////////////////////////////////

#define I2C__IC_SDA_STUCK_AT_LOW_TIMEOUT__OFFSET 0xB0ULL
#define I2C__IC_SDA_STUCK_AT_LOW_TIMEOUT__NUM  0x1

#define I2C__IC_SDA_STUCK_AT_LOW_TIMEOUT__IC_SDA_STUCK_LOW_TIMEOUT__SHIFT    0

///////////////////////////////////////////////////////
// Register: IC_CLR_SCL_STUCK_DET
// Clear SCL Stuck at Low Detect Interrupt Register
///////////////////////////////////////////////////////

#define I2C__IC_CLR_SCL_STUCK_DET__OFFSET 0xB4ULL
#define I2C__IC_CLR_SCL_STUCK_DET__NUM  0x1

#define I2C__IC_CLR_SCL_STUCK_DET__CLR_SCL_STUCK_DET__SHIFT    0
#define I2C__IC_CLR_SCL_STUCK_DET__RESERVED_31_1__SHIFT    1

///////////////////////////////////////////////////////
// Register: IC_COMP_PARAM_1
// Component Parameter Register 1
///////////////////////////////////////////////////////

#define I2C__IC_COMP_PARAM_1__OFFSET 0xF4ULL
#define I2C__IC_COMP_PARAM_1__NUM  0x1

#define I2C__IC_COMP_PARAM_1__APB_DATA_WIDTH__SHIFT    0
#define I2C__IC_COMP_PARAM_1__MAX_SPEED_MODE__SHIFT    2
#define I2C__IC_COMP_PARAM_1__HC_COUNT_VALUES__SHIFT    4
#define I2C__IC_COMP_PARAM_1__INTR_IO__SHIFT    5
#define I2C__IC_COMP_PARAM_1__HAS_DMA__SHIFT    6
#define I2C__IC_COMP_PARAM_1__ADD_ENCODED_PARAMS__SHIFT    7
#define I2C__IC_COMP_PARAM_1__RX_BUFFER_DEPTH__SHIFT    8
#define I2C__IC_COMP_PARAM_1__TX_BUFFER_DEPTH__SHIFT    16
#define I2C__IC_COMP_PARAM_1__RESERVED_31_24__SHIFT    24

#define I2C__IC_COMP_PARAM_1__APB_DATA_WIDTH__SIZE    2
#define I2C__IC_COMP_PARAM_1__MAX_SPEED_MODE__SIZE    2
#define I2C__IC_COMP_PARAM_1__RX_BUFFER_DEPTH__SIZE   8
#define I2C__IC_COMP_PARAM_1__TX_BUFFER_DEPTH__SIZE   8

///////////////////////////////////////////////////////
// Register: IC_COMP_VERSION
// I2C Component Version Register
///////////////////////////////////////////////////////

#define I2C__IC_COMP_VERSION__OFFSET 0xF8ULL
#define I2C__IC_COMP_VERSION__NUM  0x1

#define I2C__IC_COMP_VERSION__IC_COMP_VERSION__SHIFT    0


///////////////////////////////////////////////////////
// Register: IC_COMP_TYPE
// I2C Component Type Register
///////////////////////////////////////////////////////

#define I2C__IC_COMP_TYPE__OFFSET 0xFCULL
#define I2C__IC_COMP_TYPE__NUM  0x1

#define I2C__IC_COMP_TYPE__IC_COMP_TYPE__SHIFT    0


typedef struct {
    AL_U32      DeviceId;
    AL_U32      BaseAddress;
    AL_U32      ClockHz;
    AL_U32      IntrId;
} AL_IIC_HwConfigStruct;

#ifdef __cplusplus
}
#endif

#endif /* AL_IIC_HW_H */