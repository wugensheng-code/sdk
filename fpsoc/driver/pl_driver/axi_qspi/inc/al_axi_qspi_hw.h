/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    al_axi_qspi_hw.h
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2024/08/27
 * @brief   axi-qspi device driver
 */

#ifndef __AL_AXI_QSPI_HW_H__
#define __AL_AXI_QSPI_HW_H__

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files ********************************/
#include "al_type.h"
#include "al_core.h"
#include "soc_plat.h"

/************************** Constant Definitions ****************************/

/***************** Macros (Inline Functions) Definitions ********************/
typedef enum {
    AXI_QSPI_STANDARD = 0,
    AXI_QSPI_DUAL     = 1,
    AXI_QSPI_QUAD     = 2,
} AlAxiQspi_ModeEnum;

typedef enum {
    AXI_QSPI_FLASH_MIXED       = 0,
    AXI_QSPI_FLASH_WINBOND     = 1,
    AXI_QSPI_FLASH_MICRON      = 2,
    AXI_QSPI_FLASH_SPANSION    = 3,
    AXI_QSPI_FLASH_MACRONIX    = 4,
} AlAxiQspi_FlashTypeEnum;

typedef enum {
    AXI_QSPI_FLASH_ADDR_24     = 24,
    AXI_QSPI_FLASH_ADDR_32     = 32,
} AlAxiQspi_FlashAddrLenEnum;

typedef enum {
    AXI_QSPI_TRANS_BITS_8      = 8,
    AXI_QSPI_TRANS_BITS_16     = 16,
    AXI_QSPI_TRANS_BITS_32     = 32,
} AlAxiQspi_TransBitsEnum;

typedef enum {
    AXI_QSPI_FIFO_DEPTH_0      = 0,
    AXI_QSPI_FIFO_DEPTH_16     = 16,
    AXI_QSPI_FIFO_DEPTH_256    = 256,
} AlAxiQspi_FifoDepthEnum;


#ifndef AL_AXI_QSPI_NUM_INSTANCE
#define AL_AXI_QSPI_NUM_INSTANCE      1
#endif

#define AXI_QSPI_XIP_MODE             0

#if AXI_QSPI_XIP_MODE == 0

///////////////////////////////////////////////////////
// Register: sw_rst_register
// Software reset register
///////////////////////////////////////////////////////

#define AXI_QSPI_SW_RST_OFFSET                0x40ULL
#define AXI_QSPI_SW_RST_KEY_VAL            0x0000000A


///////////////////////////////////////////////////////
// Register: spi_ctl_register
// Spi Controller register
///////////////////////////////////////////////////////

#define AXI_QSPI_SPI_CTL_OFFSET                  0x60ULL
#define AXI_QSPI_SPI_CTL_LOOP_SHIFT                    0
#define AXI_QSPI_SPI_CTL_SPE_SHIFT                     1
#define AXI_QSPI_SPI_CTL_MASTER_SHIFT                  2
#define AXI_QSPI_SPI_CTL_CPOL_SHIFT                    3
#define AXI_QSPI_SPI_CTL_CPHA_SHIFT                    4
#define AXI_QSPI_SPI_CTL_TX_FIFO_RST_SHIFT             5
#define AXI_QSPI_SPI_CTL_RX_FIFO_RST_SHIFT             6
#define AXI_QSPI_SPI_CTL_MANUAL_SLAVE_EN_SHIFT         7
#define AXI_QSPI_SPI_CTL_MASTER_TRANS_INHIBIT_SHIFT    8
#define AXI_QSPI_SPI_CTL_LSB_FIRST_SHIFT               9

#define AXI_QSPI_SPI_CTL_LOOP_SIZE                     1
#define AXI_QSPI_SPI_CTL_SPE_SIZE                      1
#define AXI_QSPI_SPI_CTL_MASTER_SIZE                   1
#define AXI_QSPI_SPI_CTL_CPOL_SIZE                     1
#define AXI_QSPI_SPI_CTL_CPHA_SIZE                     1
#define AXI_QSPI_SPI_CTL_TX_FIFO_RST_SIZE              1
#define AXI_QSPI_SPI_CTL_RX_FIFO_RST_SIZE              1
#define AXI_QSPI_SPI_CTL_MANUAL_SLAVE_EN_SIZE          1
#define AXI_QSPI_SPI_CTL_MASTER_TRANS_INHIBIT_SIZE     1
#define AXI_QSPI_SPI_CTL_LSB_FIRST_SIZE                1


///////////////////////////////////////////////////////
// Register: spi_sr_register
// Spi Status register
///////////////////////////////////////////////////////

#define AXI_QSPI_SPI_STATUS_OFFSET               0x64ULL
#define AXI_QSPI_SPI_STATUS_RX_EMPTY_SHIFT             0
#define AXI_QSPI_SPI_STATUS_RX_FULL_SHIFT              1
#define AXI_QSPI_SPI_STATUS_TX_EMPTY_SHIFT             2
#define AXI_QSPI_SPI_STATUS_TX_FULL_SHIFT              3
#define AXI_QSPI_SPI_STATUS_MODF_SHIFT                 4
#define AXI_QSPI_SPI_STATUS_SLAVE_SEL_SHIFT            5
#define AXI_QSPI_SPI_STATUS_CPOL_CPHA_ERR_SHIFT        6
#define AXI_QSPI_SPI_STATUS_SLAVE_MODE_ERR_SHIFT       7
#define AXI_QSPI_SPI_STATUS_MSB_ERR_SHIFT              8
#define AXI_QSPI_SPI_STATUS_LOOPBACK_ERR_SHIFT         9
#define AXI_QSPI_SPI_STATUS_CMD_ERR_SHIFT             10

#define AXI_QSPI_SPI_STATUS_RX_EMPTY_SIZE              0
#define AXI_QSPI_SPI_STATUS_RX_FULL_SIZE               1
#define AXI_QSPI_SPI_STATUS_TX_EMPTY_SIZE              2
#define AXI_QSPI_SPI_STATUS_TX_FULL_SIZE               3
#define AXI_QSPI_SPI_STATUS_MODF_SIZE                  4
#define AXI_QSPI_SPI_STATUS_SLAVE_SEL_SIZE             5
#define AXI_QSPI_SPI_STATUS_CPOL_CPHA_ERR_SIZE         6
#define AXI_QSPI_SPI_STATUS_SLAVE_MODE_ERR_SIZE        7
#define AXI_QSPI_SPI_STATUS_MSB_ERR_SIZE               8
#define AXI_QSPI_SPI_STATUS_LOOPBACK_ERR_SIZE          9
#define AXI_QSPI_SPI_STATUS_CMD_ERR_SIZE              10


///////////////////////////////////////////////////////
// Register: spi_dtr_register
// Spi Data Transmit register
///////////////////////////////////////////////////////

#define AXI_QSPI_SPI_DTR_OFFSET               0x68ULL
#define AXI_QSPI_SPI_DTR_TX_DATA_SHIFT              0

#define AXI_QSPI_SPI_DTR_TX_DATA_SIZE              32


///////////////////////////////////////////////////////
// Register: spi_drr_register
// Spi Data Receive register
///////////////////////////////////////////////////////

#define AXI_QSPI_SPI_DRR_OFFSET               0x6CULL
#define AXI_QSPI_SPI_DRR_RX_DATA_SHIFT              0

#define AXI_QSPI_SPI_DRR_RX_DATA_SIZE              32


///////////////////////////////////////////////////////
// Register: spi_slv_sel_register
// Spi Slave Select register
///////////////////////////////////////////////////////

#define AXI_QSPI_SPI_SLV_SEL_OFFSET               0x70ULL
#define AXI_QSPI_SPI_SLV_SEL_SLAVE_SEL_SHIFT            0

#define AXI_QSPI_SPI_SLV_SEL_SLAVE_SEL_SIZE            32


///////////////////////////////////////////////////////
// Register: spi_tx_fifo_ocy_register
// Spi Transmit FIFO Occupancy register
///////////////////////////////////////////////////////

#define AXI_QSPI_SPI_TX_FIFO_OCY_OFFSET           0x74ULL
#define AXI_QSPI_SPI_TX_FIFO_OCY_VAL_SHIFT              0

#define AXI_QSPI_SPI_TX_FIFO_OCY_VAL_SIZE               8


///////////////////////////////////////////////////////
// Register: spi_rx_fifo_ocy_register
// Spi Receive FIFO Occupancy register
///////////////////////////////////////////////////////

#define AXI_QSPI_SPI_RX_FIFO_OCY_OFFSET           0x78ULL
#define AXI_QSPI_SPI_RX_FIFO_OCY_VAL_SHIFT              0

#define AXI_QSPI_SPI_RX_FIFO_OCY_VAL_SIZE               8


///////////////////////////////////////////////////////
// Register: spi_dev_glb_intr_en_register
// Spi Device Global Interrupt Enable register
///////////////////////////////////////////////////////

#define AXI_QSPI_SPI_DEV_GLB_INTR_EN_OFFSET           0x1CULL
#define AXI_QSPI_SPI_DEV_GLB_INTR_EN_SHIFT                 31

#define AXI_QSPI_SPI_DEV_GLB_INTR_EN_SIZE                   1


///////////////////////////////////////////////////////
// Register: spi_intr_sta_register
// Spi Interrupt Status register
///////////////////////////////////////////////////////

#define AXI_QSPI_SPI_INTR_STA_OFFSET                  0x20ULL
#define AXI_QSPI_SPI_INTR_STA_MODF_SHIFT                    0
#define AXI_QSPI_SPI_INTR_STA_SLAVE_MODF_SHIFT              1
#define AXI_QSPI_SPI_INTR_STA_DTR_EMPTY_SHIFT               2
#define AXI_QSPI_SPI_INTR_STA_DTR_UNDER_SHIFT               3
#define AXI_QSPI_SPI_INTR_STA_DRR_FULL_SHIFT                4
#define AXI_QSPI_SPI_INTR_STA_DRR_OVER_SHIFT                5
#define AXI_QSPI_SPI_INTR_STA_TX_FIFO_HALF_SHIFT            6
#define AXI_QSPI_SPI_INTR_STA_SLV_SEL_SHIFT                 7
#define AXI_QSPI_SPI_INTR_STA_DRR_NOT_EMPTY_SHIFT           8
#define AXI_QSPI_SPI_INTR_STA_CPOL_CPHA_ERR_SHIFT           9
#define AXI_QSPI_SPI_INTR_STA_SLAVE_MODE_ERR_SHIFT         10
#define AXI_QSPI_SPI_INTR_STA_MSB_ERR_SHIFT                11
#define AXI_QSPI_SPI_INTR_STA_LOOPBACK_ERR_SHIFT           12
#define AXI_QSPI_SPI_INTR_STA_CMD_ERR_SHIFT                13

#define AXI_QSPI_SPI_INTR_STA_MODF_SIZE                     1
#define AXI_QSPI_SPI_INTR_STA_SLAVE_MODF_SIZE               1
#define AXI_QSPI_SPI_INTR_STA_DTR_EMPTY_SIZE                1
#define AXI_QSPI_SPI_INTR_STA_DTR_UNDER_SIZE                1
#define AXI_QSPI_SPI_INTR_STA_DRR_FULL_SIZE                 1
#define AXI_QSPI_SPI_INTR_STA_DRR_OVER_SIZE                 1
#define AXI_QSPI_SPI_INTR_STA_TX_FIFO_HALF_SIZE             1
#define AXI_QSPI_SPI_INTR_STA_SLV_SEL_SIZE                  1
#define AXI_QSPI_SPI_INTR_STA_DRR_NOT_EMPTY_SIZE            1
#define AXI_QSPI_SPI_INTR_STA_CPOL_CPHA_ERR_SIZE            1
#define AXI_QSPI_SPI_INTR_STA_SLAVE_MODE_ERR_SIZE           1
#define AXI_QSPI_SPI_INTR_STA_MSB_ERR_SIZE                  1
#define AXI_QSPI_SPI_INTR_STA_LOOPBACK_ERR_SIZE             1
#define AXI_QSPI_SPI_INTR_STA_CMD_ERR_SIZE                  1


///////////////////////////////////////////////////////
// Register: spi_intr_en_register
// Spi Interrupt Enable register
///////////////////////////////////////////////////////

#define AXI_QSPI_SPI_INTR_EN_OFFSET                   0x28ULL
#define AXI_QSPI_SPI_INTR_EN_MODF_SHIFT                     0
#define AXI_QSPI_SPI_INTR_EN_SLAVE_MODF_SHIFT               1
#define AXI_QSPI_SPI_INTR_EN_DTR_EMPTY_SHIFT                2
#define AXI_QSPI_SPI_INTR_EN_DTR_UNDER_SHIFT                3
#define AXI_QSPI_SPI_INTR_EN_DRR_FULL_SHIFT                 4
#define AXI_QSPI_SPI_INTR_EN_DRR_OVER_SHIFT                 5
#define AXI_QSPI_SPI_INTR_EN_TX_FIFO_HALF_SHIFT             6
#define AXI_QSPI_SPI_INTR_EN_SLV_SEL_SHIFT                  7
#define AXI_QSPI_SPI_INTR_EN_DRR_NOT_EMPTY_SHIFT            8
#define AXI_QSPI_SPI_INTR_EN_CPOL_CPHA_ERR_SHIFT            9
#define AXI_QSPI_SPI_INTR_EN_SLAVE_MODE_ERR_SHIFT          10
#define AXI_QSPI_SPI_INTR_EN_MSB_ERR_SHIFT                 11
#define AXI_QSPI_SPI_INTR_EN_LOOPBACK_ERR_SHIFT            12
#define AXI_QSPI_SPI_INTR_EN_CMD_ERR_SHIFT                 13

#define AXI_QSPI_SPI_INTR_EN_MODF_SIZE                      1
#define AXI_QSPI_SPI_INTR_EN_SLAVE_MODF_SIZE                1
#define AXI_QSPI_SPI_INTR_EN_DTR_EMPTY_SIZE                 1
#define AXI_QSPI_SPI_INTR_EN_DTR_UNDER_SIZE                 1
#define AXI_QSPI_SPI_INTR_EN_DRR_FULL_SIZE                  1
#define AXI_QSPI_SPI_INTR_EN_DRR_OVER_SIZE                  1
#define AXI_QSPI_SPI_INTR_EN_TX_FIFO_HALF_SIZE              1
#define AXI_QSPI_SPI_INTR_EN_SLV_SEL_SIZE                   1
#define AXI_QSPI_SPI_INTR_EN_DRR_NOT_EMPTY_SIZE             1
#define AXI_QSPI_SPI_INTR_EN_CPOL_CPHA_ERR_SIZE             1
#define AXI_QSPI_SPI_INTR_EN_SLAVE_MODE_ERR_SIZE            1
#define AXI_QSPI_SPI_INTR_EN_MSB_ERR_SIZE                   1
#define AXI_QSPI_SPI_INTR_EN_LOOPBACK_ERR_SIZE              1
#define AXI_QSPI_SPI_INTR_EN_CMD_ERR_SIZE                   1

#else

///////////////////////////////////////////////////////
// Register: spi_xip_cr_register
// Spi XIP Control register
///////////////////////////////////////////////////////

#define AXI_QSPI_SPI_XIP_CR_OFFSET                    0x60ULL
#define AXI_QSPI_SPI_XIP_CR_CPHA_SHIFT                      0
#define AXI_QSPI_SPI_XIP_CR_CPOL_SHIFT                      1

#define AXI_QSPI_SPI_XIP_CR_CPHA_SIZE                       1
#define AXI_QSPI_SPI_XIP_CR_CPOL_SIZE                       1


///////////////////////////////////////////////////////
// Register: spi_xip_cr_register
// Spi XIP Control register
///////////////////////////////////////////////////////

#define AXI_QSPI_SPI_XIP_SR_OFFSET                    0x64ULL
#define AXI_QSPI_SPI_XIP_SR_RX_EMPTY_SHIFT                  0
#define AXI_QSPI_SPI_XIP_SR_RX_FULL_SHIFT                   1
#define AXI_QSPI_SPI_XIP_SR_MASTER_MODF_SHIFT               2
#define AXI_QSPI_SPI_XIP_SR_CPOL_CPHA_ERR_SHIFT             3
#define AXI_QSPI_SPI_XIP_SR_AXI_ERR_SHIFT                   4

#define AXI_QSPI_SPI_XIP_SR_RX_EMPTY_SIZE                   1
#define AXI_QSPI_SPI_XIP_SR_RX_FULL_SIZE                    1
#define AXI_QSPI_SPI_XIP_SR_MASTER_MODF_SIZE                1
#define AXI_QSPI_SPI_XIP_SR_CPOL_CPHA_ERR_SIZE              1
#define AXI_QSPI_SPI_XIP_SR_AXI_ERR_SIZE                    1

#endif

/**************************** Type Definitions ******************************/
typedef struct {
    AL_U32 DeviceId;
    AL_U32 IntrId;
    AL_U64 BaseAddress;
    AL_U32 InputClk;

    AL_U32 IsAsyncClk;
    AL_U32 AxiType;
    AL_U32 AxiIdWidth;
    AL_U32 XipPerfMode;
    AL_U32 IsXipMode;
    AL_U32 SlaveNums;
    AL_U32 SCK_RATIO;
    AlAxiQspi_ModeEnum SpiMode;
    AlAxiQspi_FlashTypeEnum FlashType;
    AlAxiQspi_TransBitsEnum TransBits;
    AlAxiQspi_FifoDepthEnum FifoDepth;
    AlAxiQspi_FlashAddrLenEnum FlashAddrLen;
} AlAxiQspi_HwConfigStruct;

#ifdef __cplusplus
}
#endif

#endif /* __AL_AXI_QSPI_HW_H__ */