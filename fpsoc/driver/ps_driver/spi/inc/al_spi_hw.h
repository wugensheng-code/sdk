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
#define TOP_NS_BASE_ADDR 0xF8800000ULL

///////////////////////////////////////////////////////
// Register: CFG_CTRL_SPI
//
///////////////////////////////////////////////////////
#define TOP_NS_CFG_CTRL_SPI_ADDR (TOP_NS_BASE_ADDR + 0x160ULL)
#define TOP_NS_CFG_CTRL_SPI_NUM  0x1

#define TOP_NS_CFG_CTRL_SPI_CTRL_SPI0_SHIFT    0
#define TOP_NS_CFG_CTRL_SPI_CTRL_SPI1_SHIFT    8
#define TOP_NS_CFG_CTRL_SPI_RESERVED_31_16_SHIFT    16

#define TOP_NS_CFG_CTRL_SPI_CTRL_SPI0_MASK    0x000000ff
#define TOP_NS_CFG_CTRL_SPI_CTRL_SPI1_MASK    0x0000ff00
#define TOP_NS_CFG_CTRL_SPI_RESERVED_31_16_MASK    0xffff0000

#define TOP_NS_CFG_CTRL_SPI_CTRL_SPI0_POR_VALUE    0x00
#define TOP_NS_CFG_CTRL_SPI_CTRL_SPI1_POR_VALUE    0x00
#define TOP_NS_CFG_CTRL_SPI_RESERVED_31_16_POR_VALUE    0x0000

#define AL_SPI_NUM_INSTANCE	(2)

#define SPI0_BASE_ADDR 0xF8404000UL
#define SPI1_BASE_ADDR 0xF8405000UL

#define SPI_CLOCK  50000000

///////////////////////////////////////////////////////
// Register: CTRLR0_MST
// This register controls the serial data transfer. It is impossible to write to this register when the DW_apb_ssi is enabled. The DW_apb_ssi is enabled and disabled by writing to the SSIENR register.
///////////////////////////////////////////////////////
#define SPI_CTRLR0_MST_OFFSET 0x0ULL
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

#define SPI_CTRLR0_MST_DFS_MASK    0x0000000f
#define SPI_CTRLR0_MST_FRF_MASK    0x00000030
#define SPI_CTRLR0_MST_SCPH_MASK    0x00000040
#define SPI_CTRLR0_MST_SCPOL_MASK    0x00000080
#define SPI_CTRLR0_MST_TMOD_MASK    0x00000300
#define SPI_CTRLR0_MST_SLV_OE_MASK    0x00000400
#define SPI_CTRLR0_MST_SRL_MASK    0x00000800
#define SPI_CTRLR0_MST_CFS_MASK    0x0000f000
#define SPI_CTRLR0_MST_DFS_32_MASK    0x001f0000
#define SPI_CTRLR0_MST_SPI_FRF_MASK    0x00600000
#define SPI_CTRLR0_MST_RESERVED_23_MASK    0x00800000
#define SPI_CTRLR0_MST_SSTE_MASK    0x01000000
#define SPI_CTRLR0_MST_SECONV_MASK    0x02000000
#define SPI_CTRLR0_MST_RESERVED_31_26_MASK    0xfc000000

#define SPI_CTRLR0_MST_DFS_POR_VALUE    0x0
#define SPI_CTRLR0_MST_FRF_POR_VALUE    0x0
#define SPI_CTRLR0_MST_SCPH_POR_VALUE    0x0
#define SPI_CTRLR0_MST_SCPOL_POR_VALUE    0x0
#define SPI_CTRLR0_MST_TMOD_POR_VALUE    0x0
#define SPI_CTRLR0_MST_SLV_OE_POR_VALUE    0x0
#define SPI_CTRLR0_MST_SRL_POR_VALUE    0x0
#define SPI_CTRLR0_MST_CFS_POR_VALUE    0x0
#define SPI_CTRLR0_MST_DFS_32_POR_VALUE    0x7
#define SPI_CTRLR0_MST_SPI_FRF_POR_VALUE    0x0
#define SPI_CTRLR0_MST_RESERVED_23_POR_VALUE    0x0
#define SPI_CTRLR0_MST_SSTE_POR_VALUE    0x1
#define SPI_CTRLR0_MST_SECONV_POR_VALUE    0x0
#define SPI_CTRLR0_MST_RESERVED_31_26_POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: CTRLR1_MST
// Control register 1 controls the end of serial transfers when in receive-only mode. It is impossible to write to this register when the DW_apb_ssi is enabled
///////////////////////////////////////////////////////
#define SPI_CTRLR1_MST_OFFSET 0x4ULL
#define SPI_CTRLR1_MST_NUM  0x1

#define SPI_CTRLR1_MST_NDF_SHIFT    0
#define SPI_CTRLR1_MST_RESERVED_31_16_SHIFT    16

#define SPI_CTRLR1_MST_NDF_MASK    0x0000ffff
#define SPI_CTRLR1_MST_RESERVED_31_16_MASK    0xffff0000

#define SPI_CTRLR1_MST_NDF_POR_VALUE    0x0
#define SPI_CTRLR1_MST_RESERVED_31_16_POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: SSIENR_MST
// This register enables and disables the DW_apb_ssi
///////////////////////////////////////////////////////
#define SPI_SSIENR_MST_OFFSET 0x8ULL
#define SPI_SSIENR_MST_NUM  0x1

#define SPI_SSIENR_MST_SSI_EN_SHIFT    0
#define SPI_SSIENR_MST_RESERVED_31_1_SHIFT    1

#define SPI_SSIENR_MST_SSI_EN_MASK    0x00000001
#define SPI_SSIENR_MST_RESERVED_31_1_MASK    0xfffffffe

#define SPI_SSIENR_MST_SSI_EN_POR_VALUE    0x0
#define SPI_SSIENR_MST_RESERVED_31_1_POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MWCR_MST
// This register controls the direction of the data word for the half-duplex Microwire serial protocol. It is impossible to write to this register when the DW_apb_ssi is enabled
///////////////////////////////////////////////////////
#define SPI_MWCR_MST_OFFSET 0xCULL
#define SPI_MWCR_MST_NUM  0x1

#define SPI_MWCR_MST_MWMOD_SHIFT    0
#define SPI_MWCR_MST_MDD_SHIFT    1
#define SPI_MWCR_MST_MHS_SHIFT    2
#define SPI_MWCR_MST_RESERVED_31_3_SHIFT    3

#define SPI_MWCR_MST_MWMOD_MASK    0x00000001
#define SPI_MWCR_MST_MDD_MASK    0x00000002
#define SPI_MWCR_MST_MHS_MASK    0x00000004
#define SPI_MWCR_MST_RESERVED_31_3_MASK    0xfffffff8

#define SPI_MWCR_MST_MWMOD_POR_VALUE    0x0
#define SPI_MWCR_MST_MDD_POR_VALUE    0x0
#define SPI_MWCR_MST_MHS_POR_VALUE    0x0
#define SPI_MWCR_MST_RESERVED_31_3_POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: SER_MST
// The register enables the individual slave select output lines from the DW_apb_ssi master. Register bits can be set or cleared when SSI_EN=0
///////////////////////////////////////////////////////
#define SPI_SER_MST_OFFSET 0x10ULL
#define SPI_SER_MST_NUM  0x1

#define SPI_SER_MST_SER_SHIFT    0
#define SPI_SER_MST_RESERVED_31_3_SHIFT    3

#define SPI_SER_MST_SER_MASK    0x00000007
#define SPI_SER_MST_RESERVED_31_3_MASK    0xfffffff8

#define SPI_SER_MST_SER_POR_VALUE    0x0
#define SPI_SER_MST_RESERVED_31_3_POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: BAUDR_MST
// The register derives the frequency of the serial clock that regulates the data transfer. The 16-bit field in this register defines the ssi_clk divider value. It is impossible to write to this register when the DW_apb_ssi is enabled.
///////////////////////////////////////////////////////
#define SPI_BAUDR_MST_OFFSET 0x14ULL
#define SPI_BAUDR_MST_NUM  0x1

#define SPI_BAUDR_MST_RESERVED_0_SHIFT    0
#define SPI_BAUDR_MST_SCKDV_SHIFT    1
#define SPI_BAUDR_MST_RESERVED_31_16_SHIFT    16

#define SPI_BAUDR_MST_RESERVED_0_MASK    0x00000001
#define SPI_BAUDR_MST_SCKDV_MASK    0x0000fffe
#define SPI_BAUDR_MST_RESERVED_31_16_MASK    0xffff0000

#define SPI_BAUDR_MST_RESERVED_0_POR_VALUE    0x0
#define SPI_BAUDR_MST_SCKDV_POR_VALUE    0x0
#define SPI_BAUDR_MST_RESERVED_31_16_POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: TXFTLR_MST
// This register controls the threshold value for the transmit FIFO memory.
///////////////////////////////////////////////////////
#define SPI_TXFTLR_MST_OFFSET 0x18ULL
#define SPI_TXFTLR_MST_NUM  0x1

#define SPI_TXFTLR_MST_TFT_SHIFT    0
#define SPI_TXFTLR_MST_RESERVED_31_8_SHIFT    8

#define SPI_TXFTLR_MST_TFT_MASK    0x000000ff
#define SPI_TXFTLR_MST_RESERVED_31_8_MASK    0xffffff00

#define SPI_TXFTLR_MST_TFT_POR_VALUE    0x0
#define SPI_TXFTLR_MST_RESERVED_31_8_POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: RXFTLR_MST
// This register controls the threshold value for the receive FIFO memory.
///////////////////////////////////////////////////////
#define SPI_RXFTLR_MST_OFFSET 0x1CULL
#define SPI_RXFTLR_MST_NUM  0x1

#define SPI_RXFTLR_MST_RFT_SHIFT    0
#define SPI_RXFTLR_MST_RESERVED_31_8_SHIFT    8

#define SPI_RXFTLR_MST_RFT_MASK    0x000000ff
#define SPI_RXFTLR_MST_RESERVED_31_8_MASK    0xffffff00

#define SPI_RXFTLR_MST_RFT_POR_VALUE    0x0
#define SPI_RXFTLR_MST_RESERVED_31_8_POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: TXFLR_MST
// This register contains the number of valid data entries in the transmit FIFO memory.
///////////////////////////////////////////////////////
#define SPI_TXFLR_MST_OFFSET 0x20ULL
#define SPI_TXFLR_MST_NUM  0x1

#define SPI_TXFLR_MST_TXTFL_SHIFT    0
#define SPI_TXFLR_MST_RESERVED_31_9_SHIFT    9

#define SPI_TXFLR_MST_TXTFL_MASK    0x000001ff
#define SPI_TXFLR_MST_RESERVED_31_9_MASK    0xfffffe00

#define SPI_TXFLR_MST_TXTFL_POR_VALUE    0x0
#define SPI_TXFLR_MST_RESERVED_31_9_POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: RXFLR_MST
// This register contains the number of valid data entries in the receive FIFO memory.
///////////////////////////////////////////////////////
#define SPI_RXFLR_MST_OFFSET 0x24ULL
#define SPI_RXFLR_MST_NUM  0x1

#define SPI_RXFLR_MST_RXTFL_SHIFT    0
#define SPI_RXFLR_MST_RESERVED_31_9_SHIFT    9

#define SPI_RXFLR_MST_RXTFL_MASK    0x000001ff
#define SPI_RXFLR_MST_RESERVED_31_9_MASK    0xfffffe00

#define SPI_RXFLR_MST_RXTFL_POR_VALUE    0x0
#define SPI_RXFLR_MST_RESERVED_31_9_POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: SR_MST
// This is a read-only register used to indicate the current transfer status, FIFO status, and any transmission/reception errors that may have occurred.
///////////////////////////////////////////////////////
#define SPI_SR_MST_OFFSET 0x28ULL
#define SPI_SR_MST_NUM  0x1

#define SPI_SR_MST_BUSY_SHIFT    0
#define SPI_SR_MST_TFNF_SHIFT    1
#define SPI_SR_MST_TFE_SHIFT    2
#define SPI_SR_MST_RFNE_SHIFT    3
#define SPI_SR_MST_RFF_SHIFT    4
#define SPI_SR_MST_RESERVED_5_SHIFT    5
#define SPI_SR_MST_DCOL_SHIFT    6
#define SPI_SR_MST_RESERVED_31_7_SHIFT    7

#define SPI_SR_MST_BUSY_MASK    0x00000001
#define SPI_SR_MST_TFNF_MASK    0x00000002
#define SPI_SR_MST_TFE_MASK    0x00000004
#define SPI_SR_MST_RFNE_MASK    0x00000008
#define SPI_SR_MST_RFF_MASK    0x00000010
#define SPI_SR_MST_RESERVED_5_MASK    0x00000020
#define SPI_SR_MST_DCOL_MASK    0x00000040
#define SPI_SR_MST_RESERVED_31_7_MASK    0xffffff80

#define SPI_SR_MST_BUSY_POR_VALUE    0x0
#define SPI_SR_MST_TFNF_POR_VALUE    0x1
#define SPI_SR_MST_TFE_POR_VALUE    0x1
#define SPI_SR_MST_RFNE_POR_VALUE    0x0
#define SPI_SR_MST_RFF_POR_VALUE    0x0
#define SPI_SR_MST_RESERVED_5_POR_VALUE    0x0
#define SPI_SR_MST_DCOL_POR_VALUE    0x0
#define SPI_SR_MST_RESERVED_31_7_POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: IMR_MST
// This read/write reigster masks or enables all interrupts generated by the DW_apb_ssi
///////////////////////////////////////////////////////
#define SPI_IMR_MST_OFFSET 0x2CULL
#define SPI_IMR_MST_NUM  0x1

#define SPI_IMR_MST_TXEIM_SHIFT    0
#define SPI_IMR_MST_TXOIM_SHIFT    1
#define SPI_IMR_MST_RXUIM_SHIFT    2
#define SPI_IMR_MST_RXOIM_SHIFT    3
#define SPI_IMR_MST_RXFIM_SHIFT    4
#define SPI_IMR_MST_MSTIM_SHIFT    5
#define SPI_IMR_MST_RESERVED_31_6_SHIFT    6

#define SPI_IMR_MST_TXEIM_MASK    0x00000001
#define SPI_IMR_MST_TXOIM_MASK    0x00000002
#define SPI_IMR_MST_RXUIM_MASK    0x00000004
#define SPI_IMR_MST_RXOIM_MASK    0x00000008
#define SPI_IMR_MST_RXFIM_MASK    0x00000010
#define SPI_IMR_MST_MSTIM_MASK    0x00000020
#define SPI_IMR_MST_RESERVED_31_6_MASK    0xffffffc0

#define SPI_IMR_MST_TXEIM_POR_VALUE    0x1
#define SPI_IMR_MST_TXOIM_POR_VALUE    0x1
#define SPI_IMR_MST_RXUIM_POR_VALUE    0x1
#define SPI_IMR_MST_RXOIM_POR_VALUE    0x1
#define SPI_IMR_MST_RXFIM_POR_VALUE    0x1
#define SPI_IMR_MST_MSTIM_POR_VALUE    0x1
#define SPI_IMR_MST_RESERVED_31_6_POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: ISR_MST
// This register reports the status of the DW_apb_ssi interrupts after they have been masked.
///////////////////////////////////////////////////////
#define SPI_ISR_MST_OFFSET 0x30ULL
#define SPI_ISR_MST_NUM  0x1

#define SPI_ISR_MST_TXEIS_SHIFT    0
#define SPI_ISR_MST_TXOIS_SHIFT    1
#define SPI_ISR_MST_RXUIS_SHIFT    2
#define SPI_ISR_MST_RXOIS_SHIFT    3
#define SPI_ISR_MST_RXFIS_SHIFT    4
#define SPI_ISR_MST_MSTIS_SHIFT    5
#define SPI_ISR_MST_RESERVED_31_6_SHIFT    6

#define SPI_ISR_MST_TXEIS_MASK    0x00000001
#define SPI_ISR_MST_TXOIS_MASK    0x00000002
#define SPI_ISR_MST_RXUIS_MASK    0x00000004
#define SPI_ISR_MST_RXOIS_MASK    0x00000008
#define SPI_ISR_MST_RXFIS_MASK    0x00000010
#define SPI_ISR_MST_MSTIS_MASK    0x00000020
#define SPI_ISR_MST_RESERVED_31_6_MASK    0xffffffc0

#define SPI_ISR_MST_TXEIS_POR_VALUE    0x0
#define SPI_ISR_MST_TXOIS_POR_VALUE    0x0
#define SPI_ISR_MST_RXUIS_POR_VALUE    0x0
#define SPI_ISR_MST_RXOIS_POR_VALUE    0x0
#define SPI_ISR_MST_RXFIS_POR_VALUE    0x0
#define SPI_ISR_MST_MSTIS_POR_VALUE    0x0
#define SPI_ISR_MST_RESERVED_31_6_POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: RISR_MST
// This read-only register reports the status of the DW_apb_ssi interrupts prior to masking.
///////////////////////////////////////////////////////
#define SPI_RISR_MST_OFFSET 0x34ULL
#define SPI_RISR_MST_NUM  0x1

#define SPI_RISR_MST_TXEIR_SHIFT    0
#define SPI_RISR_MST_TXOIR_SHIFT    1
#define SPI_RISR_MST_RXUIR_SHIFT    2
#define SPI_RISR_MST_RXOIR_SHIFT    3
#define SPI_RISR_MST_RXFIR_SHIFT    4
#define SPI_RISR_MST_MSTIR_SHIFT    5
#define SPI_RISR_MST_RESERVED_31_6_SHIFT    6

#define SPI_RISR_MST_TXEIR_MASK    0x00000001
#define SPI_RISR_MST_TXOIR_MASK    0x00000002
#define SPI_RISR_MST_RXUIR_MASK    0x00000004
#define SPI_RISR_MST_RXOIR_MASK    0x00000008
#define SPI_RISR_MST_RXFIR_MASK    0x00000010
#define SPI_RISR_MST_MSTIR_MASK    0x00000020
#define SPI_RISR_MST_RESERVED_31_6_MASK    0xffffffc0

#define SPI_RISR_MST_TXEIR_POR_VALUE    0x0
#define SPI_RISR_MST_TXOIR_POR_VALUE    0x0
#define SPI_RISR_MST_RXUIR_POR_VALUE    0x0
#define SPI_RISR_MST_RXOIR_POR_VALUE    0x0
#define SPI_RISR_MST_RXFIR_POR_VALUE    0x0
#define SPI_RISR_MST_MSTIR_POR_VALUE    0x0
#define SPI_RISR_MST_RESERVED_31_6_POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: TXOICR_MST
// Transmit FIFO Overflow Interrupt Clear Register.
///////////////////////////////////////////////////////
#define SPI_TXOICR_MST_OFFSET 0x38ULL
#define SPI_TXOICR_MST_NUM  0x1

#define SPI_TXOICR_MST_TXOICR_SHIFT    0
#define SPI_TXOICR_MST_RESERVED_31_1_SHIFT    1

#define SPI_TXOICR_MST_TXOICR_MASK    0x00000001
#define SPI_TXOICR_MST_RESERVED_31_1_MASK    0xfffffffe

#define SPI_TXOICR_MST_TXOICR_POR_VALUE    0x0
#define SPI_TXOICR_MST_RESERVED_31_1_POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: RXOICR_MST
// Receive FIFO Overflow Interrupt Clear Register.
///////////////////////////////////////////////////////
#define SPI_RXOICR_MST_OFFSET 0x3CULL
#define SPI_RXOICR_MST_NUM  0x1

#define SPI_RXOICR_MST_RXOICR_SHIFT    0
#define SPI_RXOICR_MST_RESERVED_31_1_SHIFT    1

#define SPI_RXOICR_MST_RXOICR_MASK    0x00000001
#define SPI_RXOICR_MST_RESERVED_31_1_MASK    0xfffffffe

#define SPI_RXOICR_MST_RXOICR_POR_VALUE    0x0
#define SPI_RXOICR_MST_RESERVED_31_1_POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: RXUICR_MST
// Receive FIFO Underflow Interrupt Clear Register.
///////////////////////////////////////////////////////
#define SPI_RXUICR_MST_OFFSET 0x40ULL
#define SPI_RXUICR_MST_NUM  0x1

#define SPI_RXUICR_MST_RXUICR_SHIFT    0
#define SPI_RXUICR_MST_RESERVED_31_1_SHIFT    1

#define SPI_RXUICR_MST_RXUICR_MASK    0x00000001
#define SPI_RXUICR_MST_RESERVED_31_1_MASK    0xfffffffe

#define SPI_RXUICR_MST_RXUICR_POR_VALUE    0x0
#define SPI_RXUICR_MST_RESERVED_31_1_POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: MSTICR_MST
// Multi-Master Interrupt Clear Register.
///////////////////////////////////////////////////////
#define SPI_MSTICR_MST_OFFSET 0x44ULL
#define SPI_MSTICR_MST_NUM  0x1

#define SPI_MSTICR_MST_MSTICR_SHIFT    0
#define SPI_MSTICR_MST_RESERVED_31_1_SHIFT    1

#define SPI_MSTICR_MST_MSTICR_MASK    0x00000001
#define SPI_MSTICR_MST_RESERVED_31_1_MASK    0xfffffffe

#define SPI_MSTICR_MST_MSTICR_POR_VALUE    0x0
#define SPI_MSTICR_MST_RESERVED_31_1_POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: ICR_MST
// Interrupt Clear Register.
///////////////////////////////////////////////////////
#define SPI_ICR_MST_OFFSET 0x48ULL
#define SPI_ICR_MST_NUM  0x1

#define SPI_ICR_MST_ICR_SHIFT    0
#define SPI_ICR_MST_RESERVED_31_1_SHIFT    1

#define SPI_ICR_MST_ICR_MASK    0x00000001
#define SPI_ICR_MST_RESERVED_31_1_MASK    0xfffffffe

#define SPI_ICR_MST_ICR_POR_VALUE    0x0
#define SPI_ICR_MST_RESERVED_31_1_POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: DMACR_MST
// The register is used to enable the DMA Controller interface operation.
///////////////////////////////////////////////////////
#define SPI_DMACR_MST_OFFSET 0x4CULL
#define SPI_DMACR_MST_NUM  0x1

#define SPI_DMACR_MST_RDMAE_SHIFT    0
#define SPI_DMACR_MST_TDMAE_SHIFT    1
#define SPI_DMACR_MST_RESERVED_31_2_SHIFT    2

#define SPI_DMACR_MST_RDMAE_MASK    0x00000001
#define SPI_DMACR_MST_TDMAE_MASK    0x00000002
#define SPI_DMACR_MST_RESERVED_31_2_MASK    0xfffffffc

#define SPI_DMACR_MST_RDMAE_POR_VALUE    0x0
#define SPI_DMACR_MST_TDMAE_POR_VALUE    0x0
#define SPI_DMACR_MST_RESERVED_31_2_POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: DMATDLR_MST
// DMA Transmit Data Level
///////////////////////////////////////////////////////
#define SPI_DMATDLR_MST_OFFSET 0x50ULL
#define SPI_DMATDLR_MST_NUM  0x1

#define SPI_DMATDLR_MST_DMATDL_SHIFT    0
#define SPI_DMATDLR_MST_RESERVED_31_8_SHIFT    8

#define SPI_DMATDLR_MST_DMATDL_MASK    0x000000ff
#define SPI_DMATDLR_MST_RESERVED_31_8_MASK    0xffffff00

#define SPI_DMATDLR_MST_DMATDL_POR_VALUE    0x0
#define SPI_DMATDLR_MST_RESERVED_31_8_POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: DMARDLR_MST
// DMA Receive Data Level
///////////////////////////////////////////////////////
#define SPI_DMARDLR_MST_OFFSET 0x54ULL
#define SPI_DMARDLR_MST_NUM  0x1

#define SPI_DMARDLR_MST_DMARDL_SHIFT    0
#define SPI_DMARDLR_MST_RESERVED_31_8_SHIFT    8

#define SPI_DMARDLR_MST_DMARDL_MASK    0x000000ff
#define SPI_DMARDLR_MST_RESERVED_31_8_MASK    0xffffff00

#define SPI_DMARDLR_MST_DMARDL_POR_VALUE    0x0
#define SPI_DMARDLR_MST_RESERVED_31_8_POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: IDR_MST
// This register contains the peripherals identification code
///////////////////////////////////////////////////////
#define SPI_IDR_MST_OFFSET 0x58ULL
#define SPI_IDR_MST_NUM  0x1

#define SPI_IDR_MST_IDCODE_SHIFT    0

#define SPI_IDR_MST_IDCODE_MASK    0xffffffff

#define SPI_IDR_MST_IDCODE_POR_VALUE    0x000a9000


///////////////////////////////////////////////////////
// Register: SSI_VERSION_ID_MST
// This read-only register stores the specific DW_apb_ssi component version.
///////////////////////////////////////////////////////
#define SPI_SSI_VERSION_ID_MST_OFFSET 0x5CULL
#define SPI_SSI_VERSION_ID_MST_NUM  0x1

#define SPI_SSI_VERSION_ID_MST_SSI_COMP_VERSION_SHIFT    0

#define SPI_SSI_VERSION_ID_MST_SSI_COMP_VERSION_MASK    0xffffffff

#define SPI_SSI_VERSION_ID_MST_SSI_COMP_VERSION_POR_VALUE    0x3430332a


///////////////////////////////////////////////////////
// Register: DR0_MST
// Data Register 1. When the register is read, data in the receive FIFO buffer is accessed. When it is written to, data are moved into the transmit FIFO buffer
///////////////////////////////////////////////////////
#define SPI_DR0_MST_OFFSET 0x60ULL
#define SPI_DR0_MST_NUM  0x1

#define SPI_DR0_MST_DR_SHIFT    0

#define SPI_DR0_MST_DR_MASK    0xffffffff

#define SPI_DR0_MST_DR_POR_VALUE    0x0


///////////////////////////////////////////////////////
// Register: RX_SAMPLE_DLY_MST
// This register control the number of ssi_clk cycles that are delayed (from the default sample time before the actual sample of the rxd input occurs.
///////////////////////////////////////////////////////
#define SPI_RX_SAMPLE_DLY_MST_OFFSET 0xF0ULL
#define SPI_RX_SAMPLE_DLY_MST_NUM  0x1

#define SPI_RX_SAMPLE_DLY_MST_RSD_SHIFT    0
#define SPI_RX_SAMPLE_DLY_MST_RESERVED_31_8_SHIFT    8

#define SPI_RX_SAMPLE_DLY_MST_RSD_MASK    0x000000ff
#define SPI_RX_SAMPLE_DLY_MST_RESERVED_31_8_MASK    0xffffff00

#define SPI_RX_SAMPLE_DLY_MST_RSD_POR_VALUE    0x0
#define SPI_RX_SAMPLE_DLY_MST_RESERVED_31_8_POR_VALUE    0x0


/**************************** Type Definitions ******************************/
typedef struct {
	AL_U16 					DeviceId;		/**< Unique ID  of device */
	AL_UINTPTR 				BaseAddress;	/**< Base address of the device */
	AL_U16 					InterrupId;		/**< Unique ID  of interrupt */
	AL_U16 					FifoLen;		/**< Fifo length */
} AL_SPI_HwConfigStruct;

#ifdef __cplusplus
}
#endif

#endif
