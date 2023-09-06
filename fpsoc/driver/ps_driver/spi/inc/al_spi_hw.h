/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef AL_SPI_HW_H
#define AL_SPI_HW_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files ********************************/
#include "al_core.h"

/************************** Constant Definitions ****************************/

/**************************** Type Definitions ******************************/

/***************** Macros (Inline Functions Definitions ********************/
// #define SPI_DEBUG 1

///////////////////////////////////////////////////////
// Register: CFG_CTRL_SPI
//
///////////////////////////////////////////////////////

#define TOP_NS_CFG_CTRL_SPI_OFFSET 0x160UL
#define TOP_NS_CFG_CTRL_SPI_NUM  0x1

#define TOP_NS_CFG_CTRL_SPI_CTRL_SPI0_SHIFT    0
#define TOP_NS_CFG_CTRL_SPI_CTRL_SPI1_SHIFT    8
#define TOP_NS_CFG_CTRL_SPI_RESERVED_31_16_SHIFT    16
#define TOP_NS_CFG_CTRL_SPI_CTRL_SPI0_SIZE 8
#define TOP_NS_CFG_CTRL_SPI_CTRL_SPI1_SIZE 8
#define TOP_NS_CFG_CTRL_SPI_RESERVED_31_16_SIZE 16

#define TOP_NS_CFG_CTRL_SPI_ADDR (TOP_NS_BASE_ADDR + TOP_NS_CFG_CTRL_SPI_OFFSET)


///////////////////////////////////////////////////////
// Register: CTRLR0_MST
// This register controls the serial data transfer. It is impossible to write to this register when the DW_apb_ssi is enabled. The DW_apb_ssi is enabled and disabled by writing to the SSIENR register.
///////////////////////////////////////////////////////

#define SPI_CTRLR0_MST_OFFSET 0x0UL
#define SPI_CTRLR0_MST_NUM  0x1
#define SPI_CTRLR0_MST_DFS_SHIFT    0
#define SPI_CTRLR0_MST_FRF_SHIFT    4
#define SPI_CTRLR0_MST_SCPH_SHIFT    6
#define SPI_CTRLR0_MST_SCPOL_SHIFT    7
#define SPI_CTRLR0_MST_TMOD_SHIFT    8
#define SPI_CTRLR0_MST_SLV_OE_SHIFT    10
#define SPI_CTRLR0_MST_SRL_SHIFT    11
#define SPI_CTRLR0_MST_CFS_SHIFT    12
#define SPI_CTRLR0_MST_DFS_32_SHIFT    16
#define SPI_CTRLR0_MST_SPI_FRF_SHIFT    21
#define SPI_CTRLR0_MST_RESERVED_23_SHIFT    23
#define SPI_CTRLR0_MST_SSTE_SHIFT    24
#define SPI_CTRLR0_MST_SECONV_SHIFT    25
#define SPI_CTRLR0_MST_RESERVED_31_26_SHIFT    26

#define SPI_CTRLR0_MST_DFS_SIZE 4
#define SPI_CTRLR0_MST_FRF_SIZE 2
#define SPI_CTRLR0_MST_SCPH_SIZE 1
#define SPI_CTRLR0_MST_SCPOL_SIZE 1
#define SPI_CTRLR0_MST_TMOD_SIZE 2
#define SPI_CTRLR0_MST_SLV_OE_SIZE 1
#define SPI_CTRLR0_MST_SRL_SIZE 1
#define SPI_CTRLR0_MST_CFS_SIZE 4
#define SPI_CTRLR0_MST_DFS_32_SIZE 5
#define SPI_CTRLR0_MST_SPI_FRF_SIZE 2
#define SPI_CTRLR0_MST_RESERVED_23_SIZE 1
#define SPI_CTRLR0_MST_SSTE_SIZE 1
#define SPI_CTRLR0_MST_SECONV_SIZE 1
#define SPI_CTRLR0_MST_RESERVED_31_26_SIZE 6

///////////////////////////////////////////////////////
// Register: CTRLR1_MST
// Control register 1 controls the end of serial transfers when in receive-only mode. It is impossible to write to this register when the DW_apb_ssi is enabled
///////////////////////////////////////////////////////

#define SPI_CTRLR1_MST_OFFSET 0x4UL
#define SPI_CTRLR1_MST_NUM  0x1

#define SPI_CTRLR1_MST_NDF_SHIFT    0
#define SPI_CTRLR1_MST_RESERVED_31_16_SHIFT    16
#define SPI_CTRLR1_MST_NDF_SIZE 16
#define SPI_CTRLR1_MST_RESERVED_31_16_SIZE 16


///////////////////////////////////////////////////////
// Register: SSIENR_MST
// This register enables and disables the DW_apb_ssi
///////////////////////////////////////////////////////

#define SPI_SSIENR_MST_OFFSET 0x8UL
#define SPI_SSIENR_MST_NUM  0x1
#define SPI_SSIENR_MST_SSI_EN_SHIFT    0
#define SPI_SSIENR_MST_RESERVED_31_1_SHIFT    1

#define SPI_SSIENR_MST_SSI_EN_SIZE 1
#define SPI_SSIENR_MST_RESERVED_31_1_SIZE 31

///////////////////////////////////////////////////////
// Register: MWCR_MST
// This register controls the direction of the data word for the half-duplex Microwire serial protocol. It is impossible to write to this register when the DW_apb_ssi is enabled
///////////////////////////////////////////////////////

#define SPI_MWCR_MST_OFFSET 0xCUL
#define SPI_MWCR_MST_NUM  0x1

#define SPI_MWCR_MST_MWMOD_SHIFT    0
#define SPI_MWCR_MST_MDD_SHIFT    1
#define SPI_MWCR_MST_MHS_SHIFT    2
#define SPI_MWCR_MST_RESERVED_31_3_SHIFT    3
#define SPI_MWCR_MST_MWMOD_SIZE 1
#define SPI_MWCR_MST_MDD_SIZE 1
#define SPI_MWCR_MST_MHS_SIZE 1
#define SPI_MWCR_MST_RESERVED_31_3_SIZE 29


///////////////////////////////////////////////////////
// Register: SER_MST
// The register enables the individual slave select output lines from the DW_apb_ssi master. Register bits can be set or cleared when SSI_EN=0
///////////////////////////////////////////////////////

#define SPI_SER_MST_OFFSET 0x10UL
#define SPI_SER_MST_NUM  0x1
#define SPI_SER_MST_SER_SHIFT    0
#define SPI_SER_MST_RESERVED_31_3_SHIFT    3

#define SPI_SER_MST_SER_SIZE 3
#define SPI_SER_MST_RESERVED_31_3_SIZE 29

///////////////////////////////////////////////////////
// Register: BAUDR_MST
// The register derives the frequency of the serial clock that regulates the data transfer. The 16-bit field in this register defines the ssi_clk divider value. It is impossible to write to this register when the DW_apb_ssi is enabled.
///////////////////////////////////////////////////////

#define SPI_BAUDR_MST_OFFSET 0x14UL
#define SPI_BAUDR_MST_NUM  0x1

#define SPI_BAUDR_MST_RESERVED_0_SHIFT    0
#define SPI_BAUDR_MST_SCKDV_SHIFT    1
#define SPI_BAUDR_MST_RESERVED_31_16_SHIFT    16
#define SPI_BAUDR_MST_RESERVED_0_SIZE 1
#define SPI_BAUDR_MST_SCKDV_SIZE 15
#define SPI_BAUDR_MST_RESERVED_31_16_SIZE 16


///////////////////////////////////////////////////////
// Register: TXFTLR_MST
// This register controls the threshold value for the transmit FIFO memory.
///////////////////////////////////////////////////////

#define SPI_TXFTLR_MST_OFFSET 0x18UL
#define SPI_TXFTLR_MST_NUM  0x1
#define SPI_TXFTLR_MST_TFT_SHIFT    0
#define SPI_TXFTLR_MST_RESERVED_31_5_SHIFT    5

#define SPI_TXFTLR_MST_TFT_SIZE 5
#define SPI_TXFTLR_MST_RESERVED_31_5_SIZE 27

///////////////////////////////////////////////////////
// Register: RXFTLR_MST
// This register controls the threshold value for the receive FIFO memory.
///////////////////////////////////////////////////////

#define SPI_RXFTLR_MST_OFFSET 0x1CUL
#define SPI_RXFTLR_MST_NUM  0x1

#define SPI_RXFTLR_MST_RFT_SHIFT    0
#define SPI_RXFTLR_MST_RESERVED_31_5_SHIFT    5
#define SPI_RXFTLR_MST_RFT_SIZE 5
#define SPI_RXFTLR_MST_RESERVED_31_5_SIZE 27


///////////////////////////////////////////////////////
// Register: TXFLR_MST
// This register contains the number of valid data entries in the transmit FIFO memory.
///////////////////////////////////////////////////////

#define SPI_TXFLR_MST_OFFSET 0x20UL
#define SPI_TXFLR_MST_NUM  0x1
#define SPI_TXFLR_MST_TXTFL_SHIFT    0
#define SPI_TXFLR_MST_RESERVED_31_6_SHIFT    6

#define SPI_TXFLR_MST_TXTFL_SIZE 6
#define SPI_TXFLR_MST_RESERVED_31_6_SIZE 26

///////////////////////////////////////////////////////
// Register: RXFLR_MST
// This register contains the number of valid data entries in the receive FIFO memory.
///////////////////////////////////////////////////////

#define SPI_RXFLR_MST_OFFSET 0x24UL
#define SPI_RXFLR_MST_NUM  0x1

#define SPI_RXFLR_MST_RXTFL_SHIFT    0
#define SPI_RXFLR_MST_RESERVED_31_6_SHIFT    6
#define SPI_RXFLR_MST_RXTFL_SIZE 6
#define SPI_RXFLR_MST_RESERVED_31_6_SIZE 26


///////////////////////////////////////////////////////
// Register: SR_MST
// This is a read-only register used to indicate the current transfer status, FIFO status, and any transmission/reception errors that may have occurred.
///////////////////////////////////////////////////////

#define SPI_SR_MST_OFFSET 0x28UL
#define SPI_SR_MST_NUM  0x1
#define SPI_SR_MST_BUSY_SHIFT    0
#define SPI_SR_MST_TFNF_SHIFT    1
#define SPI_SR_MST_TFE_SHIFT    2
#define SPI_SR_MST_RFNE_SHIFT    3
#define SPI_SR_MST_RFF_SHIFT    4
#define SPI_SR_MST_RESERVED_5_SHIFT    5
#define SPI_SR_MST_DCOL_SHIFT    6
#define SPI_SR_MST_RESERVED_31_7_SHIFT    7

#define SPI_SR_MST_BUSY_SIZE 1
#define SPI_SR_MST_TFNF_SIZE 1
#define SPI_SR_MST_TFE_SIZE 1
#define SPI_SR_MST_RFNE_SIZE 1
#define SPI_SR_MST_RFF_SIZE 1
#define SPI_SR_MST_RESERVED_5_SIZE 1
#define SPI_SR_MST_DCOL_SIZE 1
#define SPI_SR_MST_RESERVED_31_7_SIZE 25

///////////////////////////////////////////////////////
// Register: IMR_MST
// This read/write reigster masks or enables all interrupts generated by the DW_apb_ssi
///////////////////////////////////////////////////////

#define SPI_IMR_MST_OFFSET 0x2CUL
#define SPI_IMR_MST_NUM  0x1

#define SPI_IMR_MST_TXEIM_SHIFT    0
#define SPI_IMR_MST_TXOIM_SHIFT    1
#define SPI_IMR_MST_RXUIM_SHIFT    2
#define SPI_IMR_MST_RXOIM_SHIFT    3
#define SPI_IMR_MST_RXFIM_SHIFT    4
#define SPI_IMR_MST_MSTIM_SHIFT    5
#define SPI_IMR_MST_RESERVED_31_6_SHIFT    6
#define SPI_IMR_MST_TXEIM_SIZE 1
#define SPI_IMR_MST_TXOIM_SIZE 1
#define SPI_IMR_MST_RXUIM_SIZE 1
#define SPI_IMR_MST_RXOIM_SIZE 1
#define SPI_IMR_MST_RXFIM_SIZE 1
#define SPI_IMR_MST_MSTIM_SIZE 1
#define SPI_IMR_MST_RESERVED_31_6_SIZE 26


///////////////////////////////////////////////////////
// Register: ISR_MST
// This register reports the status of the DW_apb_ssi interrupts after they have been masked.
///////////////////////////////////////////////////////

#define SPI_ISR_MST_OFFSET 0x30UL
#define SPI_ISR_MST_NUM  0x1
#define SPI_ISR_MST_TXEIS_SHIFT    0
#define SPI_ISR_MST_TXOIS_SHIFT    1
#define SPI_ISR_MST_RXUIS_SHIFT    2
#define SPI_ISR_MST_RXOIS_SHIFT    3
#define SPI_ISR_MST_RXFIS_SHIFT    4
#define SPI_ISR_MST_MSTIS_SHIFT    5
#define SPI_ISR_MST_RESERVED_31_6_SHIFT    6

#define SPI_ISR_MST_TXEIS_SIZE 1
#define SPI_ISR_MST_TXOIS_SIZE 1
#define SPI_ISR_MST_RXUIS_SIZE 1
#define SPI_ISR_MST_RXOIS_SIZE 1
#define SPI_ISR_MST_RXFIS_SIZE 1
#define SPI_ISR_MST_MSTIS_SIZE 1
#define SPI_ISR_MST_RESERVED_31_6_SIZE 26

///////////////////////////////////////////////////////
// Register: RISR_MST
// This read-only register reports the status of the DW_apb_ssi interrupts prior to masking.
///////////////////////////////////////////////////////

#define SPI_RISR_MST_OFFSET 0x34UL
#define SPI_RISR_MST_NUM  0x1

#define SPI_RISR_MST_TXEIR_SHIFT    0
#define SPI_RISR_MST_TXOIR_SHIFT    1
#define SPI_RISR_MST_RXUIR_SHIFT    2
#define SPI_RISR_MST_RXOIR_SHIFT    3
#define SPI_RISR_MST_RXFIR_SHIFT    4
#define SPI_RISR_MST_MSTIR_SHIFT    5
#define SPI_RISR_MST_RESERVED_31_6_SHIFT    6
#define SPI_RISR_MST_TXEIR_SIZE 1
#define SPI_RISR_MST_TXOIR_SIZE 1
#define SPI_RISR_MST_RXUIR_SIZE 1
#define SPI_RISR_MST_RXOIR_SIZE 1
#define SPI_RISR_MST_RXFIR_SIZE 1
#define SPI_RISR_MST_MSTIR_SIZE 1
#define SPI_RISR_MST_RESERVED_31_6_SIZE 26


///////////////////////////////////////////////////////
// Register: TXOICR_MST
// Transmit FIFO Overflow Interrupt Clear Register.
///////////////////////////////////////////////////////

#define SPI_TXOICR_MST_OFFSET 0x38UL
#define SPI_TXOICR_MST_NUM  0x1
#define SPI_TXOICR_MST_TXOICR_SHIFT    0
#define SPI_TXOICR_MST_RESERVED_31_1_SHIFT    1

#define SPI_TXOICR_MST_TXOICR_SIZE 1
#define SPI_TXOICR_MST_RESERVED_31_1_SIZE 31

///////////////////////////////////////////////////////
// Register: RXOICR_MST
// Receive FIFO Overflow Interrupt Clear Register.
///////////////////////////////////////////////////////

#define SPI_RXOICR_MST_OFFSET 0x3CUL
#define SPI_RXOICR_MST_NUM  0x1

#define SPI_RXOICR_MST_RXOICR_SHIFT    0
#define SPI_RXOICR_MST_RESERVED_31_1_SHIFT    1
#define SPI_RXOICR_MST_RXOICR_SIZE 1
#define SPI_RXOICR_MST_RESERVED_31_1_SIZE 31


///////////////////////////////////////////////////////
// Register: RXUICR_MST
// Receive FIFO Underflow Interrupt Clear Register.
///////////////////////////////////////////////////////

#define SPI_RXUICR_MST_OFFSET 0x40UL
#define SPI_RXUICR_MST_NUM  0x1
#define SPI_RXUICR_MST_RXUICR_SHIFT    0
#define SPI_RXUICR_MST_RESERVED_31_1_SHIFT    1

#define SPI_RXUICR_MST_RXUICR_SIZE 1
#define SPI_RXUICR_MST_RESERVED_31_1_SIZE 31

///////////////////////////////////////////////////////
// Register: MSTICR_MST
// Multi-Master Interrupt Clear Register.
///////////////////////////////////////////////////////

#define SPI_MSTICR_MST_OFFSET 0x44UL
#define SPI_MSTICR_MST_NUM  0x1

#define SPI_MSTICR_MST_MSTICR_SHIFT    0
#define SPI_MSTICR_MST_RESERVED_31_1_SHIFT    1
#define SPI_MSTICR_MST_MSTICR_SIZE 1
#define SPI_MSTICR_MST_RESERVED_31_1_SIZE 31


///////////////////////////////////////////////////////
// Register: ICR_MST
// Interrupt Clear Register.
///////////////////////////////////////////////////////

#define SPI_ICR_MST_OFFSET 0x48UL
#define SPI_ICR_MST_NUM  0x1
#define SPI_ICR_MST_ICR_SHIFT    0
#define SPI_ICR_MST_RESERVED_31_1_SHIFT    1

#define SPI_ICR_MST_ICR_SIZE 1
#define SPI_ICR_MST_RESERVED_31_1_SIZE 31

///////////////////////////////////////////////////////
// Register: DMACR_MST
// The register is used to enable the DMA Controller interface operation.
///////////////////////////////////////////////////////

#define SPI_DMACR_MST_OFFSET 0x4CUL
#define SPI_DMACR_MST_NUM  0x1

#define SPI_DMACR_MST_RDMAE_SHIFT    0
#define SPI_DMACR_MST_TDMAE_SHIFT    1
#define SPI_DMACR_MST_RESERVED_31_2_SHIFT    2
#define SPI_DMACR_MST_RDMAE_SIZE 1
#define SPI_DMACR_MST_TDMAE_SIZE 1
#define SPI_DMACR_MST_RESERVED_31_2_SIZE 30


///////////////////////////////////////////////////////
// Register: DMATDLR_MST
// DMA Transmit Data Level
///////////////////////////////////////////////////////

#define SPI_DMATDLR_MST_OFFSET 0x50UL
#define SPI_DMATDLR_MST_NUM  0x1
#define SPI_DMATDLR_MST_DMATDL_SHIFT    0
#define SPI_DMATDLR_MST_RESERVED_31_5_SHIFT    5

#define SPI_DMATDLR_MST_DMATDL_SIZE 5
#define SPI_DMATDLR_MST_RESERVED_31_5_SIZE 27

///////////////////////////////////////////////////////
// Register: DMARDLR_MST
// DMA Receive Data Level
///////////////////////////////////////////////////////

#define SPI_DMARDLR_MST_OFFSET 0x54UL
#define SPI_DMARDLR_MST_NUM  0x1

#define SPI_DMARDLR_MST_DMARDL_SHIFT    0
#define SPI_DMARDLR_MST_RESERVED_31_5_SHIFT    5
#define SPI_DMARDLR_MST_DMARDL_SIZE 5
#define SPI_DMARDLR_MST_RESERVED_31_5_SIZE 27


///////////////////////////////////////////////////////
// Register: IDR_MST
// This register contains the peripherals identification code
///////////////////////////////////////////////////////

#define SPI_IDR_MST_OFFSET 0x58UL
#define SPI_IDR_MST_NUM  0x1
#define SPI_IDR_MST_IDCODE_SHIFT    0

#define SPI_IDR_MST_IDCODE_SIZE 32

///////////////////////////////////////////////////////
// Register: SSI_VERSION_ID_MST
// This read-only register stores the specific DW_apb_ssi component version.
///////////////////////////////////////////////////////

#define SPI_SSI_VERSION_ID_MST_OFFSET 0x5CUL
#define SPI_SSI_VERSION_ID_MST_NUM  0x1

#define SPI_SSI_VERSION_ID_MST_SSI_COMP_VERSION_SHIFT    0
#define SPI_SSI_VERSION_ID_MST_SSI_COMP_VERSION_SIZE 32


///////////////////////////////////////////////////////
// Register: DR0_MST
// Data Register 1. When the register is read, data in the receive FIFO buffer is accessed. When it is written to, data are moved into the transmit FIFO buffer
///////////////////////////////////////////////////////

#define SPI_DR0_MST_OFFSET 0x60UL
#define SPI_DR0_MST_NUM  0x1
#define SPI_DR0_MST_DR_SHIFT    0

#define SPI_DR0_MST_DR_SIZE 32


///////////////////////////////////////////////////////
// Register: RX_SAMPLE_DLY_MST
// This register control the number of ssi_clk cycles that are delayed (from the default sample time) before the actual sample of the rxd input occurs.
///////////////////////////////////////////////////////

#define SPI_RX_SAMPLE_DLY_MST_OFFSET 0xF0UL
#define SPI_RX_SAMPLE_DLY_MST_NUM  0x1
#define SPI_RX_SAMPLE_DLY_MST_RSD_SHIFT    0
#define SPI_RX_SAMPLE_DLY_MST_RESERVED_31_8_SHIFT    8

#define SPI_RX_SAMPLE_DLY_MST_RSD_SIZE 8
#define SPI_RX_SAMPLE_DLY_MST_RESERVED_31_8_SIZE 24


/**************************** Type Definitions ******************************/
typedef struct {
    AL_U16                     DeviceId;        /**< Unique ID  of device */
    AL_UINTPTR                 BaseAddress;     /**< Base address of the device */
    AL_U16                     IntrId;          /**< Unique ID  of interrupt */
    AL_U16                     FifoLen;         /**< Fifo length */
    AL_U16                     CsSel;           /**< Cs select */
} AL_SPI_HwConfigStruct;

#ifdef __cplusplus
}
#endif

#endif
