/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_QSPI_HW_H_
#define __AL_QSPI_HW_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files ********************************/
#include "al_core.h"

/************************** Constant Definitions ****************************/

/**************************** Type Definitions ******************************/

/***************** Macros (Inline Functions Definitions ********************/
// #define QSPI_DEBUG 1

///////////////////////////////////////////////////////
// Register: CFG_CTRL_QSPI
//
///////////////////////////////////////////////////////

#define TOP_NS_CFG_CTRL_QSPI_OFFSET 0x16CUL
#define TOP_NS_CFG_CTRL_QSPI_NUM  0x1
#define TOP_NS_CFG_CTRL_QSPI_CTRL_QSPI_SHIFT    0

#define TOP_NS_CFG_CTRL_QSPI_CTRL_QSPI_SIZE 32

#define TOP_NS_CFG_CTRL_QSPI_ADDR (TOP_NS_BASE_ADDR + TOP_NS_CFG_CTRL_QSPI_OFFSET)

///////////////////////////////////////////////////////
// Register: CTRLR0
// This register controls the serial data transfer. It is impossible to write to this register when the DWC_ssi is enabled.
///////////////////////////////////////////////////////

#define QSPI_CTRLR0_OFFSET 0x0UL
#define QSPI_CTRLR0_NUM  0x1

#define QSPI_CTRLR0_DFS_SHIFT    0
#define QSPI_CTRLR0_RESERVED_5_SHIFT    5
#define QSPI_CTRLR0_FRF_SHIFT    6
#define QSPI_CTRLR0_SCPH_SHIFT    8
#define QSPI_CTRLR0_SCPOL_SHIFT    9
#define QSPI_CTRLR0_TMOD_SHIFT    10
#define QSPI_CTRLR0_SLV_OE_SHIFT    12
#define QSPI_CTRLR0_SRL_SHIFT    13
#define QSPI_CTRLR0_SSTE_SHIFT    14
#define QSPI_CTRLR0_RESERVED_15_SHIFT    15
#define QSPI_CTRLR0_CFS_SHIFT    16
#define QSPI_CTRLR0_RESERVED_21_20_SHIFT    20
#define QSPI_CTRLR0_SPI_FRF_SHIFT    22
#define QSPI_CTRLR0_SPI_HYPERBUS_EN_SHIFT    24
#define QSPI_CTRLR0_RESERVED_30_25_SHIFT    25
#define QSPI_CTRLR0_SSI_IS_MST_SHIFT    31
#define QSPI_CTRLR0_DFS_SIZE 5
#define QSPI_CTRLR0_RESERVED_5_SIZE 1
#define QSPI_CTRLR0_FRF_SIZE 2
#define QSPI_CTRLR0_SCPH_SIZE 1
#define QSPI_CTRLR0_SCPOL_SIZE 1
#define QSPI_CTRLR0_TMOD_SIZE 2
#define QSPI_CTRLR0_SLV_OE_SIZE 1
#define QSPI_CTRLR0_SRL_SIZE 1
#define QSPI_CTRLR0_SSTE_SIZE 1
#define QSPI_CTRLR0_RESERVED_15_SIZE 1
#define QSPI_CTRLR0_CFS_SIZE 4
#define QSPI_CTRLR0_RESERVED_21_20_SIZE 2
#define QSPI_CTRLR0_SPI_FRF_SIZE 2
#define QSPI_CTRLR0_SPI_HYPERBUS_EN_SIZE 1
#define QSPI_CTRLR0_RESERVED_30_25_SIZE 6
#define QSPI_CTRLR0_SSI_IS_MST_SIZE 1


///////////////////////////////////////////////////////
// Register: CTRLR1
// Control register 1 controls the end of serial transfers when in receive-only mode. It is impossible to write to this register when the DWC_ssi is enabled.
///////////////////////////////////////////////////////

#define QSPI_CTRLR1_OFFSET 0x4UL
#define QSPI_CTRLR1_NUM  0x1
#define QSPI_CTRLR1_NDF_SHIFT    0
#define QSPI_CTRLR1_RESERVED_31_16_SHIFT    16

#define QSPI_CTRLR1_NDF_SIZE 16
#define QSPI_CTRLR1_RESERVED_31_16_SIZE 16

///////////////////////////////////////////////////////
// Register: SSIENR
// This register enables and disables the DWC_ssi.
///////////////////////////////////////////////////////

#define QSPI_SSIENR_OFFSET 0x8UL
#define QSPI_SSIENR_NUM  0x1

#define QSPI_SSIENR_SSIC_EN_SHIFT    0
#define QSPI_SSIENR_RESERVED_31_1_SHIFT    1
#define QSPI_SSIENR_SSIC_EN_SIZE 1
#define QSPI_SSIENR_RESERVED_31_1_SIZE 31


///////////////////////////////////////////////////////
// Register: MWCR
// This register controls the direction of the data word for the half-duplex Microwire serial protocol. It is impossible to write to this register when the DWC_ssi is enabled.
///////////////////////////////////////////////////////

#define QSPI_MWCR_OFFSET 0xCUL
#define QSPI_MWCR_NUM  0x1
#define QSPI_MWCR_MWMOD_SHIFT    0
#define QSPI_MWCR_MDD_SHIFT    1
#define QSPI_MWCR_MHS_SHIFT    2
#define QSPI_MWCR_RESERVED_31_3_SHIFT    3

#define QSPI_MWCR_MWMOD_SIZE 1
#define QSPI_MWCR_MDD_SIZE 1
#define QSPI_MWCR_MHS_SIZE 1
#define QSPI_MWCR_RESERVED_31_3_SIZE 29

///////////////////////////////////////////////////////
// Register: SER
// The register enables the individual slave select output lines from the DWC_ssi master.You cannot write to this register when DWC_ssi is busy and when SSIC_EN = 1.
///////////////////////////////////////////////////////

#define QSPI_SER_OFFSET 0x10UL
#define QSPI_SER_NUM  0x1

#define QSPI_SER_SER_SHIFT    0
#define QSPI_SER_RESERVED_31_2_SHIFT    2
#define QSPI_SER_SER_SIZE 2
#define QSPI_SER_RESERVED_31_2_SIZE 30


///////////////////////////////////////////////////////
// Register: BAUDR
// The register derives the frequency of the serial clock that regulates the data transfer.It is impossible to write to this register when the DWC_ssi is enabled.
///////////////////////////////////////////////////////

#define QSPI_BAUDR_OFFSET 0x14UL
#define QSPI_BAUDR_NUM  0x1
#define QSPI_BAUDR_RESERVED_0_SHIFT    0
#define QSPI_BAUDR_SCKDV_SHIFT    1
#define QSPI_BAUDR_RESERVED_31_16_SHIFT    16

#define QSPI_BAUDR_RESERVED_0_SIZE 1
#define QSPI_BAUDR_SCKDV_SIZE 15
#define QSPI_BAUDR_RESERVED_31_16_SIZE 16

///////////////////////////////////////////////////////
// Register: TXFTLR
// This register controls the threshold value for the transmit FIFO memory.
///////////////////////////////////////////////////////

#define QSPI_TXFTLR_OFFSET 0x18UL
#define QSPI_TXFTLR_NUM  0x1

#define QSPI_TXFTLR_TFT_SHIFT    0
#define QSPI_TXFTLR_RESERVED_15_8_SHIFT    8
#define QSPI_TXFTLR_TXFTHR_SHIFT    16
#define QSPI_TXFTLR_RESERVED_31_24_SHIFT    24
#define QSPI_TXFTLR_TFT_SIZE 8
#define QSPI_TXFTLR_RESERVED_15_8_SIZE 8
#define QSPI_TXFTLR_TXFTHR_SIZE 8
#define QSPI_TXFTLR_RESERVED_31_24_SIZE 8


///////////////////////////////////////////////////////
// Register: RXFTLR
// This register controls the threshold value for the receive FIFO memory.
///////////////////////////////////////////////////////

#define QSPI_RXFTLR_OFFSET 0x1CUL
#define QSPI_RXFTLR_NUM  0x1
#define QSPI_RXFTLR_RFT_SHIFT    0
#define QSPI_RXFTLR_RESERVED_31_8_SHIFT    8

#define QSPI_RXFTLR_RFT_SIZE 8
#define QSPI_RXFTLR_RESERVED_31_8_SIZE 24

///////////////////////////////////////////////////////
// Register: TXFLR
// This register contains the number of valid data entries in the transmit FIFO memory.
///////////////////////////////////////////////////////

#define QSPI_TXFLR_OFFSET 0x20UL
#define QSPI_TXFLR_NUM  0x1

#define QSPI_TXFLR_TXTFL_SHIFT    0
#define QSPI_TXFLR_RESERVED_31_9_SHIFT    9
#define QSPI_TXFLR_TXTFL_SIZE 9
#define QSPI_TXFLR_RESERVED_31_9_SIZE 23


///////////////////////////////////////////////////////
// Register: RXFLR
// This register contains the number of valid data entries in the receive FIFO memory.
///////////////////////////////////////////////////////

#define QSPI_RXFLR_OFFSET 0x24UL
#define QSPI_RXFLR_NUM  0x1
#define QSPI_RXFLR_RXTFL_SHIFT    0
#define QSPI_RXFLR_RESERVED_31_9_SHIFT    9

#define QSPI_RXFLR_RXTFL_SIZE 9
#define QSPI_RXFLR_RESERVED_31_9_SIZE 23

///////////////////////////////////////////////////////
// Register: SR
// This is a read-only register used to indicate the current transfer status, FIFO status, and any transmission/reception errors that may have occurred.
///////////////////////////////////////////////////////

#define QSPI_SR_OFFSET 0x28UL
#define QSPI_SR_NUM  0x1

#define QSPI_SR_BUSY_SHIFT    0
#define QSPI_SR_TFNF_SHIFT    1
#define QSPI_SR_TFE_SHIFT    2
#define QSPI_SR_RFNE_SHIFT    3
#define QSPI_SR_RFF_SHIFT    4
#define QSPI_SR_TXE_SHIFT    5
#define QSPI_SR_DCOL_SHIFT    6
#define QSPI_SR_RESERVED_31_7_SHIFT    7
#define QSPI_SR_BUSY_SIZE 1
#define QSPI_SR_TFNF_SIZE 1
#define QSPI_SR_TFE_SIZE 1
#define QSPI_SR_RFNE_SIZE 1
#define QSPI_SR_RFF_SIZE 1
#define QSPI_SR_TXE_SIZE 1
#define QSPI_SR_DCOL_SIZE 1
#define QSPI_SR_RESERVED_31_7_SIZE 25


///////////////////////////////////////////////////////
// Register: IMR
// This read/write register masks or enables all interrupts generated by the DWC_ssi.
///////////////////////////////////////////////////////

#define QSPI_IMR_OFFSET 0x2CUL
#define QSPI_IMR_NUM  0x1
#define QSPI_IMR_TXEIM_SHIFT    0
#define QSPI_IMR_TXOIM_SHIFT    1
#define QSPI_IMR_RXUIM_SHIFT    2
#define QSPI_IMR_RXOIM_SHIFT    3
#define QSPI_IMR_RXFIM_SHIFT    4
#define QSPI_IMR_MSTIM_SHIFT    5
#define QSPI_IMR_XRXOIM_SHIFT    6
#define QSPI_IMR_RESERVED_31_7_SHIFT    7

#define QSPI_IMR_TXEIM_SIZE 1
#define QSPI_IMR_TXOIM_SIZE 1
#define QSPI_IMR_RXUIM_SIZE 1
#define QSPI_IMR_RXOIM_SIZE 1
#define QSPI_IMR_RXFIM_SIZE 1
#define QSPI_IMR_MSTIM_SIZE 1
#define QSPI_IMR_XRXOIM_SIZE 1
#define QSPI_IMR_RESERVED_31_7_SIZE 25

///////////////////////////////////////////////////////
// Register: ISR
// This register reports the status of the DWC_ssi interrupts after they have been masked.
///////////////////////////////////////////////////////

#define QSPI_ISR_OFFSET 0x30UL
#define QSPI_ISR_NUM  0x1

#define QSPI_ISR_TXEIS_SHIFT    0
#define QSPI_ISR_TXOIS_SHIFT    1
#define QSPI_ISR_RXUIS_SHIFT    2
#define QSPI_ISR_RXOIS_SHIFT    3
#define QSPI_ISR_RXFIS_SHIFT    4
#define QSPI_ISR_MSTIS_SHIFT    5
#define QSPI_ISR_XRXOIS_SHIFT    6
#define QSPI_ISR_RESERVED_31_7_SHIFT    7
#define QSPI_ISR_TXEIS_SIZE 1
#define QSPI_ISR_TXOIS_SIZE 1
#define QSPI_ISR_RXUIS_SIZE 1
#define QSPI_ISR_RXOIS_SIZE 1
#define QSPI_ISR_RXFIS_SIZE 1
#define QSPI_ISR_MSTIS_SIZE 1
#define QSPI_ISR_XRXOIS_SIZE 1
#define QSPI_ISR_RESERVED_31_7_SIZE 25


///////////////////////////////////////////////////////
// Register: RISR
// Raw Interrupt Status Register
///////////////////////////////////////////////////////

#define QSPI_RISR_OFFSET 0x34UL
#define QSPI_RISR_NUM  0x1
#define QSPI_RISR_TXEIR_SHIFT    0
#define QSPI_RISR_TXOIR_SHIFT    1
#define QSPI_RISR_RXUIR_SHIFT    2
#define QSPI_RISR_RXOIR_SHIFT    3
#define QSPI_RISR_RXFIR_SHIFT    4
#define QSPI_RISR_MSTIR_SHIFT    5
#define QSPI_RISR_XRXOIR_SHIFT    6
#define QSPI_RISR_RESERVED_31_7_SHIFT    7

#define QSPI_RISR_TXEIR_SIZE 1
#define QSPI_RISR_TXOIR_SIZE 1
#define QSPI_RISR_RXUIR_SIZE 1
#define QSPI_RISR_RXOIR_SIZE 1
#define QSPI_RISR_RXFIR_SIZE 1
#define QSPI_RISR_MSTIR_SIZE 1
#define QSPI_RISR_XRXOIR_SIZE 1
#define QSPI_RISR_RESERVED_31_7_SIZE 25

///////////////////////////////////////////////////////
// Register: TXOICR
// Transmit FIFO Overflow Interrupt Clear Register
///////////////////////////////////////////////////////

#define QSPI_TXOICR_OFFSET 0x38UL
#define QSPI_TXOICR_NUM  0x1

#define QSPI_TXOICR_TXOICR_SHIFT    0
#define QSPI_TXOICR_RESERVED_31_1_SHIFT    1
#define QSPI_TXOICR_TXOICR_SIZE 1
#define QSPI_TXOICR_RESERVED_31_1_SIZE 31


///////////////////////////////////////////////////////
// Register: RXOICR
// Receive FIFO Overflow Interrupt Clear Register
///////////////////////////////////////////////////////

#define QSPI_RXOICR_OFFSET 0x3CUL
#define QSPI_RXOICR_NUM  0x1
#define QSPI_RXOICR_RXOICR_SHIFT    0
#define QSPI_RXOICR_RESERVED_31_1_SHIFT    1

#define QSPI_RXOICR_RXOICR_SIZE 1
#define QSPI_RXOICR_RESERVED_31_1_SIZE 31

///////////////////////////////////////////////////////
// Register: RXUICR
// Receive FIFO Underflow Interrupt Clear Register
///////////////////////////////////////////////////////

#define QSPI_RXUICR_OFFSET 0x40UL
#define QSPI_RXUICR_NUM  0x1

#define QSPI_RXUICR_RXUICR_SHIFT    0
#define QSPI_RXUICR_RESERVED_31_1_SHIFT    1
#define QSPI_RXUICR_RXUICR_SIZE 1
#define QSPI_RXUICR_RESERVED_31_1_SIZE 31


///////////////////////////////////////////////////////
// Register: MSTICR
// Multi-Master Interrupt Clear Register
///////////////////////////////////////////////////////

#define QSPI_MSTICR_OFFSET 0x44UL
#define QSPI_MSTICR_NUM  0x1
#define QSPI_MSTICR_MSTICR_SHIFT    0
#define QSPI_MSTICR_RESERVED_31_1_SHIFT    1

#define QSPI_MSTICR_MSTICR_SIZE 1
#define QSPI_MSTICR_RESERVED_31_1_SIZE 31

///////////////////////////////////////////////////////
// Register: ICR
// Interrupt Clear Register
///////////////////////////////////////////////////////

#define QSPI_ICR_OFFSET 0x48UL
#define QSPI_ICR_NUM  0x1

#define QSPI_ICR_ICR_SHIFT    0
#define QSPI_ICR_RESERVED_31_1_SHIFT    1
#define QSPI_ICR_ICR_SIZE 1
#define QSPI_ICR_RESERVED_31_1_SIZE 31


///////////////////////////////////////////////////////
// Register: DMACR
// DMA Control Register
///////////////////////////////////////////////////////

#define QSPI_DMACR_OFFSET 0x4CUL
#define QSPI_DMACR_NUM  0x1
#define QSPI_DMACR_RDMAE_SHIFT    0
#define QSPI_DMACR_TDMAE_SHIFT    1
#define QSPI_DMACR_RESERVED_31_2_SHIFT    2

#define QSPI_DMACR_RDMAE_SIZE 1
#define QSPI_DMACR_TDMAE_SIZE 1
#define QSPI_DMACR_RESERVED_31_2_SIZE 30

///////////////////////////////////////////////////////
// Register: DMATDLR
// DMA Transmit Data Level
///////////////////////////////////////////////////////

#define QSPI_DMATDLR_OFFSET 0x50UL
#define QSPI_DMATDLR_NUM  0x1

#define QSPI_DMATDLR_DMATDL_SHIFT    0
#define QSPI_DMATDLR_RESERVED_31_8_SHIFT    8
#define QSPI_DMATDLR_DMATDL_SIZE 8
#define QSPI_DMATDLR_RESERVED_31_8_SIZE 24


///////////////////////////////////////////////////////
// Register: DMARDLR
// DMA Receive Data Level
///////////////////////////////////////////////////////

#define QSPI_DMARDLR_OFFSET 0x54UL
#define QSPI_DMARDLR_NUM  0x1
#define QSPI_DMARDLR_DMARDL_SHIFT    0
#define QSPI_DMARDLR_RESERVED_31_8_SHIFT    8

#define QSPI_DMARDLR_DMARDL_SIZE 8
#define QSPI_DMARDLR_RESERVED_31_8_SIZE 24

///////////////////////////////////////////////////////
// Register: IDR
// This register contains the peripherals identification code
///////////////////////////////////////////////////////

#define QSPI_IDR_OFFSET 0x58UL
#define QSPI_IDR_NUM  0x1

#define QSPI_IDR_IDCODE_SHIFT    0
#define QSPI_IDR_IDCODE_SIZE 32


///////////////////////////////////////////////////////
// Register: SSIC_VERSION_ID
// This read-only register stores the specific DWC_ssi component version.
///////////////////////////////////////////////////////

#define QSPI_SSIC_VERSION_ID_OFFSET 0x5CUL
#define QSPI_SSIC_VERSION_ID_NUM  0x1
#define QSPI_SSIC_VERSION_ID_SSIC_COMP_VERSION_SHIFT    0

#define QSPI_SSIC_VERSION_ID_SSIC_COMP_VERSION_SIZE 32

///////////////////////////////////////////////////////
// Register: DR0
// Data Register 1. When the register is read, data in the receive FIFO buffer is accessed. When it is written to, data are moved into the transmit FIFO buffer
///////////////////////////////////////////////////////

#define QSPI_DR0_OFFSET 0x60UL
#define QSPI_DR0_NUM  0x1

#define QSPI_DR0_DR_SHIFT    0
#define QSPI_DR0_DR_SIZE 32


///////////////////////////////////////////////////////
// Register: RX_SAMPLE_DELAY
// This register control the number of ssi_clk cycles that are delayed (from the default sample time) before the actual sample of the rxd input occurs
///////////////////////////////////////////////////////

#define QSPI_RX_SAMPLE_DELAY_OFFSET 0xF0UL
#define QSPI_RX_SAMPLE_DELAY_NUM  0x1

#define QSPI_RX_SAMPLE_DELAY_RSD_SHIFT    0
#define QSPI_RX_SAMPLE_DELAY_RESERVED_15_8_SHIFT    8
#define QSPI_RX_SAMPLE_DELAY_SE_SHIFT    16
#define QSPI_RX_SAMPLE_DELAY_RESERVED_31_17_SHIFT    17

#define QSPI_RX_SAMPLE_DELAY_RSD_SIZE 8
#define QSPI_RX_SAMPLE_DELAY_RESERVED_15_8_SIZE 8
#define QSPI_RX_SAMPLE_DELAY_SE_SIZE 1
#define QSPI_RX_SAMPLE_DELAY_RESERVED_31_17_SIZE 15


///////////////////////////////////////////////////////
// Register: SPI_CTRLR0
// This register is used to control the serial data transfer in enhanced SPI mode of operation
///////////////////////////////////////////////////////

#define QSPI_SPI_CTRLR0_OFFSET 0xF4UL
#define QSPI_SPI_CTRLR0_NUM  0x1
#define QSPI_SPI_CTRLR0_TRANS_TYPE_SHIFT    0
#define QSPI_SPI_CTRLR0_ADDR_L_SHIFT    2
#define QSPI_SPI_CTRLR0_RESERVED_6_SHIFT    6
#define QSPI_SPI_CTRLR0_XIP_MD_BIT_EN_SHIFT    7
#define QSPI_SPI_CTRLR0_INST_L_SHIFT    8
#define QSPI_SPI_CTRLR0_RESERVED_10_SHIFT    10
#define QSPI_SPI_CTRLR0_WAIT_CYCLES_SHIFT    11
#define QSPI_SPI_CTRLR0_SPI_DDR_EN_SHIFT    16
#define QSPI_SPI_CTRLR0_INST_DDR_EN_SHIFT    17
#define QSPI_SPI_CTRLR0_SPI_RXDS_EN_SHIFT    18
#define QSPI_SPI_CTRLR0_XIP_DFS_HC_SHIFT    19
#define QSPI_SPI_CTRLR0_XIP_INST_EN_SHIFT    20
#define QSPI_SPI_CTRLR0_SSIC_XIP_CONT_XFER_EN_SHIFT    21
#define QSPI_SPI_CTRLR0_RESERVED_23_22_SHIFT    22
#define QSPI_SPI_CTRLR0_SPI_DM_EN_SHIFT    24
#define QSPI_SPI_CTRLR0_SPI_RXDS_SIG_EN_SHIFT    25
#define QSPI_SPI_CTRLR0_XIP_MBL_SHIFT    26
#define QSPI_SPI_CTRLR0_RESERVED_28_SHIFT    28
#define QSPI_SPI_CTRLR0_XIP_PREFETCH_EN_SHIFT    29
#define QSPI_SPI_CTRLR0_CLK_STRETCH_EN_SHIFT    30
#define QSPI_SPI_CTRLR0_RESERVED_31_SHIFT    31

#define QSPI_SPI_CTRLR0_TRANS_TYPE_SIZE 2
#define QSPI_SPI_CTRLR0_ADDR_L_SIZE 4
#define QSPI_SPI_CTRLR0_RESERVED_6_SIZE 1
#define QSPI_SPI_CTRLR0_XIP_MD_BIT_EN_SIZE 1
#define QSPI_SPI_CTRLR0_INST_L_SIZE 2
#define QSPI_SPI_CTRLR0_RESERVED_10_SIZE 1
#define QSPI_SPI_CTRLR0_WAIT_CYCLES_SIZE 5
#define QSPI_SPI_CTRLR0_SPI_DDR_EN_SIZE 1
#define QSPI_SPI_CTRLR0_INST_DDR_EN_SIZE 1
#define QSPI_SPI_CTRLR0_SPI_RXDS_EN_SIZE 1
#define QSPI_SPI_CTRLR0_XIP_DFS_HC_SIZE 1
#define QSPI_SPI_CTRLR0_XIP_INST_EN_SIZE 1
#define QSPI_SPI_CTRLR0_SSIC_XIP_CONT_XFER_EN_SIZE 1
#define QSPI_SPI_CTRLR0_RESERVED_23_22_SIZE 2
#define QSPI_SPI_CTRLR0_SPI_DM_EN_SIZE 1
#define QSPI_SPI_CTRLR0_SPI_RXDS_SIG_EN_SIZE 1
#define QSPI_SPI_CTRLR0_XIP_MBL_SIZE 2
#define QSPI_SPI_CTRLR0_RESERVED_28_SIZE 1
#define QSPI_SPI_CTRLR0_XIP_PREFETCH_EN_SIZE 1
#define QSPI_SPI_CTRLR0_CLK_STRETCH_EN_SIZE 1
#define QSPI_SPI_CTRLR0_RESERVED_31_SIZE 1

///////////////////////////////////////////////////////
// Register: XIP_MODE_BITS
// This register carries the mode bits which are sent in the XIP mode of operation after address phase
///////////////////////////////////////////////////////

#define QSPI_XIP_MODE_BITS_OFFSET 0xFCUL
#define QSPI_XIP_MODE_BITS_NUM  0x1

#define QSPI_XIP_MODE_BITS_XIP_MD_BITS_SHIFT    0
#define QSPI_XIP_MODE_BITS_RESERVED_31_16_SHIFT    16
#define QSPI_XIP_MODE_BITS_XIP_MD_BITS_SIZE 16
#define QSPI_XIP_MODE_BITS_RESERVED_31_16_SIZE 16


///////////////////////////////////////////////////////
// Register: XIP_INCR_INST
// This register is used to store the instruction op-code to be used in INCR transactions when the same is requested on AHB interface
///////////////////////////////////////////////////////

#define QSPI_XIP_INCR_INST_OFFSET 0x100UL
#define QSPI_XIP_INCR_INST_NUM  0x1
#define QSPI_XIP_INCR_INST_INCR_INST_SHIFT    0
#define QSPI_XIP_INCR_INST_RESERVED_31_16_SHIFT    16

#define QSPI_XIP_INCR_INST_INCR_INST_SIZE 16
#define QSPI_XIP_INCR_INST_RESERVED_31_16_SIZE 16

///////////////////////////////////////////////////////
// Register: XIP_WRAP_INST
// This register is used to store the instruction op-code to be used in WRAP transactions when the same is requested on AHB interface
///////////////////////////////////////////////////////

#define QSPI_XIP_WRAP_INST_OFFSET 0x104UL
#define QSPI_XIP_WRAP_INST_NUM  0x1

#define QSPI_XIP_WRAP_INST_WRAP_INST_SHIFT    0
#define QSPI_XIP_WRAP_INST_RESERVED_31_16_SHIFT    16
#define QSPI_XIP_WRAP_INST_WRAP_INST_SIZE 16
#define QSPI_XIP_WRAP_INST_RESERVED_31_16_SIZE 16


///////////////////////////////////////////////////////
// Register: XIP_CNT_TIME_OUT
// XIP count down register for continuous mode. The counter is used to de-select the slave during continuous transfer mode.
///////////////////////////////////////////////////////

#define QSPI_XIP_CNT_TIME_OUT_OFFSET 0x114UL
#define QSPI_XIP_CNT_TIME_OUT_NUM  0x1
#define QSPI_XIP_CNT_TIME_OUT_XTOC_SHIFT    0
#define QSPI_XIP_CNT_TIME_OUT_RESERVED_31_8_SHIFT    8

#define QSPI_XIP_CNT_TIME_OUT_XTOC_SIZE 8
#define QSPI_XIP_CNT_TIME_OUT_RESERVED_31_8_SIZE 24


/**************************** Type Definitions ******************************/
typedef struct {
	AL_U16 					DeviceId;		/**< Unique ID  of device */
	AL_UINTPTR 				BaseAddress;	/**< Base address of the device */
	AL_U16 					InterrupId;		/**< Unique ID  of interrupt */
	AL_U16 					FifoLen;		/**< Fifo length */
	AL_U16 					NumSlvSel;		/**< Num slave select */
} AL_QSPI_HwConfigStruct;

#ifdef __cplusplus
}
#endif

#endif
