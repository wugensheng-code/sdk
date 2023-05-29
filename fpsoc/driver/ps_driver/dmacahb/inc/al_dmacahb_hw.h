#ifndef AL_DMACAHB_HW_H
#define AL_DMACAHB_HW_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files ********************************/
#include "al_core.h"

/************************** Constant Definitions ****************************/

/**************************** Type Definitions ******************************/

/***************** Macros (Inline Functions) Definitions ********************/

#define AL_DMACAHB_NUM_INSTANCE     (1)

#define DMAC_AHB_BASE_ADDR 0xF804D000ULL

///////////////////////////////////////////////////////
// Register: SAR
// Source Address for Channel 0
///////////////////////////////////////////////////////

#define DMAC_AHB_SAR_OFFSET 0x0UL
#define DMAC_AHB_SAR_NUM  0x1
#define DMAC_AHB_SAR_SAR_SHIFT    0

#define DMAC_AHB_SAR_SAR_SIZE 32

///////////////////////////////////////////////////////
// Register: DAR
// Destination Address Register for Channel 0
///////////////////////////////////////////////////////

#define DMAC_AHB_DAR_OFFSET 0x8UL
#define DMAC_AHB_DAR_NUM  0x1

#define DMAC_AHB_DAR_DAR_SHIFT    0
#define DMAC_AHB_DAR_DAR_SIZE 32


///////////////////////////////////////////////////////
// Register: LLP
// Linked List Pointer Register for Channel 0
///////////////////////////////////////////////////////

#define DMAC_AHB_LLP_OFFSET 0x10UL
#define DMAC_AHB_LLP_NUM  0x1
#define DMAC_AHB_LLP_LMS_SHIFT    0
#define DMAC_AHB_LLP_LOC_SHIFT    2

#define DMAC_AHB_LLP_LMS_SIZE 2
#define DMAC_AHB_LLP_LOC_SIZE 30

///////////////////////////////////////////////////////
// Register: CTL_LOWER
// Lower 32-bit Control Register for Channel 0. This register contains fields that control the DMA transfer.
///////////////////////////////////////////////////////

#define DMAC_AHB_CTL0_LOWER_OFFSET 0x18UL
#define DMAC_AHB_CTL0_LOWER_NUM  0x1

#define DMAC_AHB_CTL0_LOWER_INT_EN_SHIFT    0
#define DMAC_AHB_CTL0_LOWER_DST_TR_WIDTH_SHIFT    1
#define DMAC_AHB_CTL0_LOWER_SRC_TR_WIDTH_SHIFT    4
#define DMAC_AHB_CTL0_LOWER_DINC_SHIFT    7
#define DMAC_AHB_CTL0_LOWER_SINC_SHIFT    9
#define DMAC_AHB_CTL0_LOWER_DEST_MSIZE_SHIFT    11
#define DMAC_AHB_CTL0_LOWER_SRC_MSIZE_SHIFT    14
#define DMAC_AHB_CTL0_LOWER_SRC_GATHER_EN_SHIFT    17
#define DMAC_AHB_CTL0_LOWER_DST_SCATTER_EN_SHIFT    18
#define DMAC_AHB_CTL0_LOWER_RESERVED_19_SHIFT    19
#define DMAC_AHB_CTL0_LOWER_TT_FC_SHIFT    20
#define DMAC_AHB_CTL0_LOWER_DMS_SHIFT    23
#define DMAC_AHB_CTL0_LOWER_SMS_SHIFT    25
#define DMAC_AHB_CTL0_LOWER_LLP_DST_EN_SHIFT    27
#define DMAC_AHB_CTL0_LOWER_LLP_SRC_EN_SHIFT    28
#define DMAC_AHB_CTL0_LOWER_RESERVED_31_29_SHIFT    29
#define DMAC_AHB_CTL0_LOWER_INT_EN_SIZE 1
#define DMAC_AHB_CTL0_LOWER_DST_TR_WIDTH_SIZE 3
#define DMAC_AHB_CTL0_LOWER_SRC_TR_WIDTH_SIZE 3
#define DMAC_AHB_CTL0_LOWER_DINC_SIZE 2
#define DMAC_AHB_CTL0_LOWER_SINC_SIZE 2
#define DMAC_AHB_CTL0_LOWER_DEST_MSIZE_SIZE 3
#define DMAC_AHB_CTL0_LOWER_SRC_MSIZE_SIZE 3
#define DMAC_AHB_CTL0_LOWER_SRC_GATHER_EN_SIZE 1
#define DMAC_AHB_CTL0_LOWER_DST_SCATTER_EN_SIZE 1
#define DMAC_AHB_CTL0_LOWER_RESERVED_19_SIZE 1
#define DMAC_AHB_CTL0_LOWER_TT_FC_SIZE 3
#define DMAC_AHB_CTL0_LOWER_DMS_SIZE 2
#define DMAC_AHB_CTL0_LOWER_SMS_SIZE 2
#define DMAC_AHB_CTL0_LOWER_LLP_DST_EN_SIZE 1
#define DMAC_AHB_CTL0_LOWER_LLP_SRC_EN_SIZE 1
#define DMAC_AHB_CTL0_LOWER_RESERVED_31_29_SIZE 3


///////////////////////////////////////////////////////
// Register: CTL_UPPER
// Upper 32-bit Control Register for Channel 0. This register contains fields that control the DMA transfer.
///////////////////////////////////////////////////////

#define DMAC_AHB_CTL0_UPPER_OFFSET 0x1CUL
#define DMAC_AHB_CTL0_UPPER_NUM  0x1
#define DMAC_AHB_CTL0_UPPER_BLOCK_TS_SHIFT    0
#define DMAC_AHB_CTL0_UPPER_DONE_SHIFT    12
#define DMAC_AHB_CTL0_UPPER_RESERVED_31_13_SHIFT    13

#define DMAC_AHB_CTL0_UPPER_BLOCK_TS_SIZE 12
#define DMAC_AHB_CTL0_UPPER_DONE_SIZE 1
#define DMAC_AHB_CTL0_UPPER_RESERVED_31_13_SIZE 19

///////////////////////////////////////////////////////
// Register: CFG_LOWER
// Lower 32-bit Configuration Register for Channel 0. This register contains fields that configure the DMA transfer.
///////////////////////////////////////////////////////

#define DMAC_AHB_CFG0_LOWER_OFFSET 0x40UL
#define DMAC_AHB_CFG0_LOWER_NUM  0x1

#define DMAC_AHB_CFG0_LOWER_RESERVED_4_0_SHIFT    0
#define DMAC_AHB_CFG0_LOWER_CH_PRIOR_SHIFT    5
#define DMAC_AHB_CFG0_LOWER_CH_SUSP_SHIFT    8
#define DMAC_AHB_CFG0_LOWER_FIFO_EMPTY_SHIFT    9
#define DMAC_AHB_CFG0_LOWER_HS_SEL_DST_SHIFT    10
#define DMAC_AHB_CFG0_LOWER_HS_SEL_SRC_SHIFT    11
#define DMAC_AHB_CFG0_LOWER_RESERVED_17_12_SHIFT    12
#define DMAC_AHB_CFG0_LOWER_DST_HS_POL_SHIFT    18
#define DMAC_AHB_CFG0_LOWER_SRC_HS_POL_SHIFT    19
#define DMAC_AHB_CFG0_LOWER_RESERVED_29_20_SHIFT    20
#define DMAC_AHB_CFG0_LOWER_RELOAD_SRC_SHIFT    30
#define DMAC_AHB_CFG0_LOWER_RELOAD_DST_SHIFT    31
#define DMAC_AHB_CFG0_LOWER_RESERVED_4_0_SIZE 5
#define DMAC_AHB_CFG0_LOWER_CH_PRIOR_SIZE 3
#define DMAC_AHB_CFG0_LOWER_CH_SUSP_SIZE 1
#define DMAC_AHB_CFG0_LOWER_FIFO_EMPTY_SIZE 1
#define DMAC_AHB_CFG0_LOWER_HS_SEL_DST_SIZE 1
#define DMAC_AHB_CFG0_LOWER_HS_SEL_SRC_SIZE 1
#define DMAC_AHB_CFG0_LOWER_RESERVED_17_12_SIZE 6
#define DMAC_AHB_CFG0_LOWER_DST_HS_POL_SIZE 1
#define DMAC_AHB_CFG0_LOWER_SRC_HS_POL_SIZE 1
#define DMAC_AHB_CFG0_LOWER_RESERVED_29_20_SIZE 10
#define DMAC_AHB_CFG0_LOWER_RELOAD_SRC_SIZE 1
#define DMAC_AHB_CFG0_LOWER_RELOAD_DST_SIZE 1


///////////////////////////////////////////////////////
// Register: CFG_UPPER
// Upper 32-bit Configuration Register for Channel 0. This register contains fields that configure the DMA transfer.
///////////////////////////////////////////////////////

#define DMAC_AHB_CFG0_UPPER_OFFSET 0x44UL
#define DMAC_AHB_CFG0_UPPER_NUM  0x1
#define DMAC_AHB_CFG0_UPPER_FCMODE_SHIFT    0
#define DMAC_AHB_CFG0_UPPER_FIFO_MODE_SHIFT    1
#define DMAC_AHB_CFG0_UPPER_PROTCTL_SHIFT    2
#define DMAC_AHB_CFG0_UPPER_RESERVED_6_5_SHIFT    5
#define DMAC_AHB_CFG0_UPPER_SRC_PER_SHIFT    7
#define DMAC_AHB_CFG0_UPPER_DEST_PER_SHIFT    11
#define DMAC_AHB_CFG0_UPPER_RESERVED_31_15_SHIFT    15

#define DMAC_AHB_CFG0_UPPER_FCMODE_SIZE 1
#define DMAC_AHB_CFG0_UPPER_FIFO_MODE_SIZE 1
#define DMAC_AHB_CFG0_UPPER_PROTCTL_SIZE 3
#define DMAC_AHB_CFG0_UPPER_RESERVED_6_5_SIZE 2
#define DMAC_AHB_CFG0_UPPER_SRC_PER_SIZE 4
#define DMAC_AHB_CFG0_UPPER_DEST_PER_SIZE 4
#define DMAC_AHB_CFG0_UPPER_RESERVED_31_15_SIZE 17

///////////////////////////////////////////////////////
// Register: SGR
// Source Gather Register for Channel 0
///////////////////////////////////////////////////////

#define DMAC_AHB_SGR_OFFSET 0x48UL
#define DMAC_AHB_SGR_NUM  0x1

#define DMAC_AHB_SGR_SGI_SHIFT    0
#define DMAC_AHB_SGR_SGC_SHIFT    20
#define DMAC_AHB_SGR_SGI_SIZE 20
#define DMAC_AHB_SGR_SGC_SIZE 12


///////////////////////////////////////////////////////
// Register: DSR
// Destination Scatter Register for Channel 0
///////////////////////////////////////////////////////

#define DMAC_AHB_DSR_OFFSET 0x50UL
#define DMAC_AHB_DSR_NUM  0x1
#define DMAC_AHB_DSR_DSI_SHIFT    0
#define DMAC_AHB_DSR_DSC_SHIFT    20

#define DMAC_AHB_DSR_DSI_SIZE 20
#define DMAC_AHB_DSR_DSC_SIZE 12

///////////////////////////////////////////////////////
// Register: RawTfr
///////////////////////////////////////////////////////

#define DMAC_AHB_RAWTFR_OFFSET 0x2C0UL
#define DMAC_AHB_RAWTFR_NUM  0x1
#define DMAC_AHB_RAWTFR_RAW_SHIFT    0
#define DMAC_AHB_RAWTFR_RESERVED_31_8_SHIFT    8

#define DMAC_AHB_RAWTFR_RAW_SIZE 8
#define DMAC_AHB_RAWTFR_RESERVED_31_8_SIZE 24

///////////////////////////////////////////////////////
// Register: RawBlock
///////////////////////////////////////////////////////

#define DMAC_AHB_RAWBLOCK_OFFSET 0x2C8UL
#define DMAC_AHB_RAWBLOCK_NUM  0x1

#define DMAC_AHB_RAWBLOCK_RAW_SHIFT    0
#define DMAC_AHB_RAWBLOCK_RESERVED_31_8_SHIFT    8
#define DMAC_AHB_RAWBLOCK_RAW_SIZE 8
#define DMAC_AHB_RAWBLOCK_RESERVED_31_8_SIZE 24


///////////////////////////////////////////////////////
// Register: RawSrcTran
///////////////////////////////////////////////////////

#define DMAC_AHB_RAWSRCTRAN_OFFSET 0x2D0UL
#define DMAC_AHB_RAWSRCTRAN_NUM  0x1
#define DMAC_AHB_RAWSRCTRAN_RAW_SHIFT    0
#define DMAC_AHB_RAWSRCTRAN_RESERVED_31_8_SHIFT    8

#define DMAC_AHB_RAWSRCTRAN_RAW_SIZE 8
#define DMAC_AHB_RAWSRCTRAN_RESERVED_31_8_SIZE 24

///////////////////////////////////////////////////////
// Register: RawDstTran
///////////////////////////////////////////////////////

#define DMAC_AHB_RAWDSTTRAN_OFFSET 0x2D8UL
#define DMAC_AHB_RAWDSTTRAN_NUM  0x1

#define DMAC_AHB_RAWDSTTRAN_RAW_SHIFT    0
#define DMAC_AHB_RAWDSTTRAN_RESERVED_31_8_SHIFT    8
#define DMAC_AHB_RAWDSTTRAN_RAW_SIZE 8
#define DMAC_AHB_RAWDSTTRAN_RESERVED_31_8_SIZE 24


///////////////////////////////////////////////////////
// Register: RawErr
///////////////////////////////////////////////////////

#define DMAC_AHB_RAWERR_OFFSET 0x2E0UL
#define DMAC_AHB_RAWERR_NUM  0x1
#define DMAC_AHB_RAWERR_RAW_SHIFT    0
#define DMAC_AHB_RAWERR_RESERVED_31_8_SHIFT    8

#define DMAC_AHB_RAWERR_RAW_SIZE 8
#define DMAC_AHB_RAWERR_RESERVED_31_8_SIZE 24

///////////////////////////////////////////////////////
// Register: StatusTfr
// Status for IntTfr Interrupt. Channel DMA Transfer complete interrupt event from all channels is stored in this Interrupt Status register after masking. This register has a bit allocated per channel.
///////////////////////////////////////////////////////

#define DMAC_AHB_STATUSTFR_OFFSET 0x2E8UL
#define DMAC_AHB_STATUSTFR_NUM  0x1

#define DMAC_AHB_STATUSTFR_STATUS_SHIFT    0
#define DMAC_AHB_STATUSTFR_RESERVED_31_8_SHIFT    8
#define DMAC_AHB_STATUSTFR_STATUS_SIZE 8
#define DMAC_AHB_STATUSTFR_RESERVED_31_8_SIZE 24


///////////////////////////////////////////////////////
// Register: StatusBlock
// Status for IntBlock Interrupt. Channel Block complete interrupt event from all channels is stored in this Interrupt Status register after masking. This register has a bit allocated per channel.
///////////////////////////////////////////////////////

#define DMAC_AHB_STATUSBLOCK_OFFSET 0x2F0UL
#define DMAC_AHB_STATUSBLOCK_NUM  0x1
#define DMAC_AHB_STATUSBLOCK_STATUS_SHIFT    0
#define DMAC_AHB_STATUSBLOCK_RESERVED_31_8_SHIFT    8

#define DMAC_AHB_STATUSBLOCK_STATUS_SIZE 8
#define DMAC_AHB_STATUSBLOCK_RESERVED_31_8_SIZE 24

///////////////////////////////////////////////////////
// Register: StatusSrcTran
// Status for IntSrcTran Interrupt. Channel Source Transaction complete interrupt event from all channels is stored in this Interrupt Status register after masking. This register has a bit allocated per channel.
///////////////////////////////////////////////////////

#define DMAC_AHB_STATUSSRCTRAN_OFFSET 0x2F8UL
#define DMAC_AHB_STATUSSRCTRAN_NUM  0x1

#define DMAC_AHB_STATUSSRCTRAN_STATUS_SHIFT    0
#define DMAC_AHB_STATUSSRCTRAN_RESERVED_31_8_SHIFT    8
#define DMAC_AHB_STATUSSRCTRAN_STATUS_SIZE 8
#define DMAC_AHB_STATUSSRCTRAN_RESERVED_31_8_SIZE 24


///////////////////////////////////////////////////////
// Register: StatusDstTran
// Status for IntDstTran Interrupt. Channel destination transaction complete interrupt event from all channels is stored in this Interrupt Status register after masking. This register has a bit allocated per channel.
///////////////////////////////////////////////////////

#define DMAC_AHB_STATUSDSTTRAN_OFFSET 0x300UL
#define DMAC_AHB_STATUSDSTTRAN_NUM  0x1
#define DMAC_AHB_STATUSDSTTRAN_STATUS_SHIFT    0
#define DMAC_AHB_STATUSDSTTRAN_RESERVED_31_8_SHIFT    8

#define DMAC_AHB_STATUSDSTTRAN_STATUS_SIZE 8
#define DMAC_AHB_STATUSDSTTRAN_RESERVED_31_8_SIZE 24

///////////////////////////////////////////////////////
// Register: StatusErr
// Status for IntErr Interrupt. Channel Error interrupt event from all channels is stored in this Interrupt Status register after masking. This register has a bit allocated per channel.
///////////////////////////////////////////////////////

#define DMAC_AHB_STATUSERR_OFFSET 0x308UL
#define DMAC_AHB_STATUSERR_NUM  0x1

#define DMAC_AHB_STATUSERR_STATUS_SHIFT    0
#define DMAC_AHB_STATUSERR_RESERVED_31_8_SHIFT    8
#define DMAC_AHB_STATUSERR_STATUS_SIZE 8
#define DMAC_AHB_STATUSERR_RESERVED_31_8_SIZE 24


///////////////////////////////////////////////////////
// Register: MaskTfr
// Mask for IntTfr Interrupt. Each bit of register is allocated per channel.
///////////////////////////////////////////////////////

#define DMAC_AHB_MASKTFR_OFFSET 0x310UL
#define DMAC_AHB_MASKTFR_NUM  0x1
#define DMAC_AHB_MASKTFR_INT_MASK_SHIFT    0
#define DMAC_AHB_MASKTFR_INT_MASK_WE_SHIFT    8
#define DMAC_AHB_MASKTFR_RESERVED_31_16_SHIFT    16

#define DMAC_AHB_MASKTFR_INT_MASK_SIZE 8
#define DMAC_AHB_MASKTFR_INT_MASK_WE_SIZE 8
#define DMAC_AHB_MASKTFR_RESERVED_31_16_SIZE 16

///////////////////////////////////////////////////////
// Register: MaskBlock
// Mask for IntBlock Interrupt. Each bit of register is allocated per channel.
///////////////////////////////////////////////////////

#define DMAC_AHB_MASKBLOCK_OFFSET 0x318UL
#define DMAC_AHB_MASKBLOCK_NUM  0x1

#define DMAC_AHB_MASKBLOCK_INT_MASK_SHIFT    0
#define DMAC_AHB_MASKBLOCK_INT_MASK_WE_SHIFT    8
#define DMAC_AHB_MASKBLOCK_RESERVED_31_16_SHIFT    16
#define DMAC_AHB_MASKBLOCK_INT_MASK_SIZE 8
#define DMAC_AHB_MASKBLOCK_INT_MASK_WE_SIZE 8
#define DMAC_AHB_MASKBLOCK_RESERVED_31_16_SIZE 16


///////////////////////////////////////////////////////
// Register: MaskSrcTran
// Mask for IntSrcTran Interrupt. Each bit of register is allocated per channel.
///////////////////////////////////////////////////////

#define DMAC_AHB_MASKSRCTRAN_OFFSET 0x320UL
#define DMAC_AHB_MASKSRCTRAN_NUM  0x1
#define DMAC_AHB_MASKSRCTRAN_INT_MASK_SHIFT    0
#define DMAC_AHB_MASKSRCTRAN_INT_MASK_WE_SHIFT    8
#define DMAC_AHB_MASKSRCTRAN_RESERVED_31_16_SHIFT    16

#define DMAC_AHB_MASKSRCTRAN_INT_MASK_SIZE 8
#define DMAC_AHB_MASKSRCTRAN_INT_MASK_WE_SIZE 8
#define DMAC_AHB_MASKSRCTRAN_RESERVED_31_16_SIZE 16

///////////////////////////////////////////////////////
// Register: MaskDstTran
// Mask for IntDstTran Interrupt. Each bit of register is allocated per channel.
///////////////////////////////////////////////////////

#define DMAC_AHB_MASKDSTTRAN_OFFSET 0x328UL
#define DMAC_AHB_MASKDSTTRAN_NUM  0x1

#define DMAC_AHB_MASKDSTTRAN_INT_MASK_SHIFT    0
#define DMAC_AHB_MASKDSTTRAN_INT_MASK_WE_SHIFT    8
#define DMAC_AHB_MASKDSTTRAN_RESERVED_31_16_SHIFT    16
#define DMAC_AHB_MASKDSTTRAN_INT_MASK_SIZE 8
#define DMAC_AHB_MASKDSTTRAN_INT_MASK_WE_SIZE 8
#define DMAC_AHB_MASKDSTTRAN_RESERVED_31_16_SIZE 16


///////////////////////////////////////////////////////
// Register: MaskErr
// Mask for IntErr Interrupt. Each bit of register is allocated per channel.
///////////////////////////////////////////////////////

#define DMAC_AHB_MASKERR_OFFSET 0x330UL
#define DMAC_AHB_MASKERR_NUM  0x1
#define DMAC_AHB_MASKERR_INT_MASK_SHIFT    0
#define DMAC_AHB_MASKERR_INT_MASK_WE_SHIFT    8
#define DMAC_AHB_MASKERR_RESERVED_31_16_SHIFT    16

#define DMAC_AHB_MASKERR_INT_MASK_SIZE 8
#define DMAC_AHB_MASKERR_INT_MASK_WE_SIZE 8
#define DMAC_AHB_MASKERR_RESERVED_31_16_SIZE 16

///////////////////////////////////////////////////////
// Register: ClearTfr
// Clear for IntTfr Interrupt. Each bit of register is allocated per channel.
///////////////////////////////////////////////////////

#define DMAC_AHB_CLEARTFR_OFFSET 0x338UL
#define DMAC_AHB_CLEARTFR_NUM  0x1

#define DMAC_AHB_CLEARTFR_CLEAR_SHIFT    0
#define DMAC_AHB_CLEARTFR_RESERVED_31_8_SHIFT    8
#define DMAC_AHB_CLEARTFR_CLEAR_SIZE 8
#define DMAC_AHB_CLEARTFR_RESERVED_31_8_SIZE 24


///////////////////////////////////////////////////////
// Register: ClearBlock
// Clear for IntBlock Interrupt. Each bit of register is allocated per channel.
///////////////////////////////////////////////////////

#define DMAC_AHB_CLEARBLOCK_OFFSET 0x340UL
#define DMAC_AHB_CLEARBLOCK_NUM  0x1
#define DMAC_AHB_CLEARBLOCK_CLEAR_SHIFT    0
#define DMAC_AHB_CLEARBLOCK_RESERVED_31_8_SHIFT    8

#define DMAC_AHB_CLEARBLOCK_CLEAR_SIZE 8
#define DMAC_AHB_CLEARBLOCK_RESERVED_31_8_SIZE 24

///////////////////////////////////////////////////////
// Register: ClearSrcTran
// Clear for IntSrcTran Interrupt. Each bit of register is allocated per channel.
///////////////////////////////////////////////////////

#define DMAC_AHB_CLEARSRCTRAN_OFFSET 0x348UL
#define DMAC_AHB_CLEARSRCTRAN_NUM  0x1

#define DMAC_AHB_CLEARSRCTRAN_CLEAR_SHIFT    0
#define DMAC_AHB_CLEARSRCTRAN_RESERVED_31_8_SHIFT    8
#define DMAC_AHB_CLEARSRCTRAN_CLEAR_SIZE 8
#define DMAC_AHB_CLEARSRCTRAN_RESERVED_31_8_SIZE 24


///////////////////////////////////////////////////////
// Register: ClearDstTran
// Clear for IntDstTran Interrupt. Each bit of register is allocated per channel.
///////////////////////////////////////////////////////

#define DMAC_AHB_CLEARDSTTRAN_OFFSET 0x350UL
#define DMAC_AHB_CLEARDSTTRAN_NUM  0x1
#define DMAC_AHB_CLEARDSTTRAN_CLEAR_SHIFT    0
#define DMAC_AHB_CLEARDSTTRAN_RESERVED_31_8_SHIFT    8

#define DMAC_AHB_CLEARDSTTRAN_CLEAR_SIZE 8
#define DMAC_AHB_CLEARDSTTRAN_RESERVED_31_8_SIZE 24

///////////////////////////////////////////////////////
// Register: ClearErr
// Clear for IntErr Interrupt. Each bit of register is allocated per channel.
///////////////////////////////////////////////////////

#define DMAC_AHB_CLEARERR_OFFSET 0x358UL
#define DMAC_AHB_CLEARERR_NUM  0x1

#define DMAC_AHB_CLEARERR_CLEAR_SHIFT    0
#define DMAC_AHB_CLEARERR_RESERVED_31_8_SHIFT    8
#define DMAC_AHB_CLEARERR_CLEAR_SIZE 8
#define DMAC_AHB_CLEARERR_RESERVED_31_8_SIZE 24


///////////////////////////////////////////////////////
// Register: StatusInt
// Status for each Interrupt type. The contents of each of the five Status registers StatusTfr, StatusBlock, StatusSrcTran, StatusDstTran, StatusErr is ORed to produce a single bit for each interrupt type in the Combined Status register (StatusInt).
///////////////////////////////////////////////////////

#define DMAC_AHB_STATUSINT_OFFSET 0x360UL
#define DMAC_AHB_STATUSINT_NUM  0x1
#define DMAC_AHB_STATUSINT_TFR_SHIFT    0
#define DMAC_AHB_STATUSINT_BLOCK_SHIFT    1
#define DMAC_AHB_STATUSINT_SRCT_SHIFT    2
#define DMAC_AHB_STATUSINT_DSTT_SHIFT    3
#define DMAC_AHB_STATUSINT_ERR_SHIFT    4
#define DMAC_AHB_STATUSINT_RESERVED_31_5_SHIFT    5

#define DMAC_AHB_STATUSINT_TFR_SIZE 1
#define DMAC_AHB_STATUSINT_BLOCK_SIZE 1
#define DMAC_AHB_STATUSINT_SRCT_SIZE 1
#define DMAC_AHB_STATUSINT_DSTT_SIZE 1
#define DMAC_AHB_STATUSINT_ERR_SIZE 1
#define DMAC_AHB_STATUSINT_RESERVED_31_5_SIZE 27

///////////////////////////////////////////////////////
// Register: ReqSrcReg
// Source Software Transaction Request register. A bit is assigned for each channel in this register.
///////////////////////////////////////////////////////

#define DMAC_AHB_REQSRCREG_OFFSET 0x368UL
#define DMAC_AHB_REQSRCREG_NUM  0x1

#define DMAC_AHB_REQSRCREG_SRC_REQ_SHIFT    0
#define DMAC_AHB_REQSRCREG_SRC_REQ_WE_SHIFT    8
#define DMAC_AHB_REQSRCREG_RESERVED_31_16_SHIFT    16
#define DMAC_AHB_REQSRCREG_SRC_REQ_SIZE 8
#define DMAC_AHB_REQSRCREG_SRC_REQ_WE_SIZE 8
#define DMAC_AHB_REQSRCREG_RESERVED_31_16_SIZE 16


///////////////////////////////////////////////////////
// Register: ReqDstReg
// Destination Software Transaction Request register. A bit is assigned for each channel in this register.
///////////////////////////////////////////////////////

#define DMAC_AHB_REQDSTREG_OFFSET 0x370UL
#define DMAC_AHB_REQDSTREG_NUM  0x1
#define DMAC_AHB_REQDSTREG_DST_REQ_SHIFT    0
#define DMAC_AHB_REQDSTREG_DST_REQ_WE_SHIFT    8
#define DMAC_AHB_REQDSTREG_RESERVED_31_16_SHIFT    16

#define DMAC_AHB_REQDSTREG_DST_REQ_SIZE 8
#define DMAC_AHB_REQDSTREG_DST_REQ_WE_SIZE 8
#define DMAC_AHB_REQDSTREG_RESERVED_31_16_SIZE 16

///////////////////////////////////////////////////////
// Register: SglRqSrcReg
// Source Single Transaction Request register. A bit is assigned for each channel in this register.
///////////////////////////////////////////////////////

#define DMAC_AHB_SGLRQSRCREG_OFFSET 0x378UL
#define DMAC_AHB_SGLRQSRCREG_NUM  0x1

#define DMAC_AHB_SGLRQSRCREG_SRC_SGLREQ_SHIFT    0
#define DMAC_AHB_SGLRQSRCREG_SRC_SGLREQ_WE_SHIFT    8
#define DMAC_AHB_SGLRQSRCREG_RESERVED_31_16_SHIFT    16
#define DMAC_AHB_SGLRQSRCREG_SRC_SGLREQ_SIZE 8
#define DMAC_AHB_SGLRQSRCREG_SRC_SGLREQ_WE_SIZE 8
#define DMAC_AHB_SGLRQSRCREG_RESERVED_31_16_SIZE 16


///////////////////////////////////////////////////////
// Register: SglRqDstReg
// Destination Single Transaction Request register. A bit is assigned for each channel in this register.
///////////////////////////////////////////////////////

#define DMAC_AHB_SGLRQDSTREG_OFFSET 0x380UL
#define DMAC_AHB_SGLRQDSTREG_NUM  0x1
#define DMAC_AHB_SGLRQDSTREG_DST_SGLREQ_SHIFT    0
#define DMAC_AHB_SGLRQDSTREG_DST_SGLREQ_WE_SHIFT    8
#define DMAC_AHB_SGLRQDSTREG_RESERVED_31_16_SHIFT    16

#define DMAC_AHB_SGLRQDSTREG_DST_SGLREQ_SIZE 8
#define DMAC_AHB_SGLRQDSTREG_DST_SGLREQ_WE_SIZE 8
#define DMAC_AHB_SGLRQDSTREG_RESERVED_31_16_SIZE 16

///////////////////////////////////////////////////////
// Register: LstSrcReg
// Source Last Transaction Request register. A bit is assigned for each channel in this register.
///////////////////////////////////////////////////////

#define DMAC_AHB_LSTSRCREG_OFFSET 0x388UL
#define DMAC_AHB_LSTSRCREG_NUM  0x1

#define DMAC_AHB_LSTSRCREG_LSTSRC_SHIFT    0
#define DMAC_AHB_LSTSRCREG_LSTSRC_WE_SHIFT    8
#define DMAC_AHB_LSTSRCREG_RESERVED_31_16_SHIFT    16
#define DMAC_AHB_LSTSRCREG_LSTSRC_SIZE 8
#define DMAC_AHB_LSTSRCREG_LSTSRC_WE_SIZE 8
#define DMAC_AHB_LSTSRCREG_RESERVED_31_16_SIZE 16


///////////////////////////////////////////////////////
// Register: LstDstReg
// Destination Last Transaction Request register. A bit is assigned for each channel in this register.
///////////////////////////////////////////////////////

#define DMAC_AHB_LSTDSTREG_OFFSET 0x390UL
#define DMAC_AHB_LSTDSTREG_NUM  0x1
#define DMAC_AHB_LSTDSTREG_LSTDST_SHIFT    0
#define DMAC_AHB_LSTDSTREG_LSTDST_WE_SHIFT    8
#define DMAC_AHB_LSTDSTREG_RESERVED_31_16_SHIFT    16

#define DMAC_AHB_LSTDSTREG_LSTDST_SIZE 8
#define DMAC_AHB_LSTDSTREG_LSTDST_WE_SIZE 8
#define DMAC_AHB_LSTDSTREG_RESERVED_31_16_SIZE 16

///////////////////////////////////////////////////////
// Register: DmaCfgReg
// DW_ahb_dmac Configuration Register. This register is used to enable the DW_ahb_dmac, which must be done before any channel activity can begin.
///////////////////////////////////////////////////////

#define DMAC_AHB_DMACFGREG_OFFSET 0x398UL
#define DMAC_AHB_DMACFGREG_NUM  0x1

#define DMAC_AHB_DMACFGREG_DMA_EN_SHIFT    0
#define DMAC_AHB_DMACFGREG_RESERVED_31_1_SHIFT    1
#define DMAC_AHB_DMACFGREG_DMA_EN_SIZE 1
#define DMAC_AHB_DMACFGREG_RESERVED_31_1_SIZE 31


///////////////////////////////////////////////////////
// Register: ChEnReg
// DW_ahb_dmac Channel Enable Register.
///////////////////////////////////////////////////////

#define DMAC_AHB_CHENREG_OFFSET 0x3A0UL
#define DMAC_AHB_CHENREG_NUM  0x1
#define DMAC_AHB_CHENREG_CH_EN_SHIFT    0
#define DMAC_AHB_CHENREG_CH_EN_WE_SHIFT    8
#define DMAC_AHB_CHENREG_RESERVED_31_16_SHIFT    16

#define DMAC_AHB_CHENREG_CH_EN_SIZE 8
#define DMAC_AHB_CHENREG_CH_EN_WE_SIZE 8
#define DMAC_AHB_CHENREG_RESERVED_31_16_SIZE 16

///////////////////////////////////////////////////////
// Register: DmaIdReg
// DW_ahb_dmac ID registe. which is a read-only register that reads back the coreConsultant-configured hardcoded ID number.
///////////////////////////////////////////////////////

#define DMAC_AHB_DMAIDREG_OFFSET 0x3A8UL
#define DMAC_AHB_DMAIDREG_NUM  0x1

#define DMAC_AHB_DMAIDREG_DMA_ID_SHIFT    0
#define DMAC_AHB_DMAIDREG_DMA_ID_SIZE 32


///////////////////////////////////////////////////////
// Register: DmaTestReg
// DMA Test registers. This register is used to put the AHB slave interface into test mode, during which the readback value of the writable registers match the value written.
///////////////////////////////////////////////////////

#define DMAC_AHB_DMATESTREG_OFFSET 0x3B0UL
#define DMAC_AHB_DMATESTREG_NUM  0x1
#define DMAC_AHB_DMATESTREG_TEST_SLV_IF_SHIFT    0
#define DMAC_AHB_DMATESTREG_RESERVED_31_1_SHIFT    1

#define DMAC_AHB_DMATESTREG_TEST_SLV_IF_SIZE 1
#define DMAC_AHB_DMATESTREG_RESERVED_31_1_SIZE 31

///////////////////////////////////////////////////////
// Register: DMA_COMP_PARAMS_6
// DW_ahb_dmac Component Parameters Register 6. This register contains encoded information about the component parameter settings.
///////////////////////////////////////////////////////

#define DMAC_AHB_DMA_COMP_PARAMS_6_OFFSET 0x3CCUL
#define DMAC_AHB_DMA_COMP_PARAMS_6_NUM  0x1

#define DMAC_AHB_DMA_COMP_PARAMS_6_CH7_DTW_SHIFT    0
#define DMAC_AHB_DMA_COMP_PARAMS_6_CH7_STW_SHIFT    3
#define DMAC_AHB_DMA_COMP_PARAMS_6_CH7_STAT_DST_SHIFT    6
#define DMAC_AHB_DMA_COMP_PARAMS_6_CH7_STAT_SRC_SHIFT    7
#define DMAC_AHB_DMA_COMP_PARAMS_6_CH7_DST_SCA_EN_SHIFT    8
#define DMAC_AHB_DMA_COMP_PARAMS_6_CH7_SRC_GAT_EN_SHIFT    9
#define DMAC_AHB_DMA_COMP_PARAMS_6_CH7_LOCK_EN_SHIFT    10
#define DMAC_AHB_DMA_COMP_PARAMS_6_CH7_MULTI_BLK_EN_SHIFT    11
#define DMAC_AHB_DMA_COMP_PARAMS_6_CH7_CTL_WB_EN_SHIFT    12
#define DMAC_AHB_DMA_COMP_PARAMS_6_CH7_HC_LLP_SHIFT    13
#define DMAC_AHB_DMA_COMP_PARAMS_6_CH7_FC_SHIFT    14
#define DMAC_AHB_DMA_COMP_PARAMS_6_CH7_MAX_MULT_SIZE_SHIFT    16
#define DMAC_AHB_DMA_COMP_PARAMS_6_CH7_DMS_SHIFT    19
#define DMAC_AHB_DMA_COMP_PARAMS_6_CH7_LMS_SHIFT    22
#define DMAC_AHB_DMA_COMP_PARAMS_6_CH7_SMS_SHIFT    25
#define DMAC_AHB_DMA_COMP_PARAMS_6_CH7_FIFO_DEPTH_SHIFT    28
#define DMAC_AHB_DMA_COMP_PARAMS_6_RESERVED_31_SHIFT    31
#define DMAC_AHB_DMA_COMP_PARAMS_6_CH7_DTW_SIZE 3
#define DMAC_AHB_DMA_COMP_PARAMS_6_CH7_STW_SIZE 3
#define DMAC_AHB_DMA_COMP_PARAMS_6_CH7_STAT_DST_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_6_CH7_STAT_SRC_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_6_CH7_DST_SCA_EN_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_6_CH7_SRC_GAT_EN_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_6_CH7_LOCK_EN_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_6_CH7_MULTI_BLK_EN_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_6_CH7_CTL_WB_EN_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_6_CH7_HC_LLP_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_6_CH7_FC_SIZE 2
#define DMAC_AHB_DMA_COMP_PARAMS_6_CH7_MAX_MULT_SIZE_SIZE 3
#define DMAC_AHB_DMA_COMP_PARAMS_6_CH7_DMS_SIZE 3
#define DMAC_AHB_DMA_COMP_PARAMS_6_CH7_LMS_SIZE 3
#define DMAC_AHB_DMA_COMP_PARAMS_6_CH7_SMS_SIZE 3
#define DMAC_AHB_DMA_COMP_PARAMS_6_CH7_FIFO_DEPTH_SIZE 3
#define DMAC_AHB_DMA_COMP_PARAMS_6_RESERVED_31_SIZE 1


///////////////////////////////////////////////////////
// Register: DMA_COMP_PARAMS_5_LOWER
// Lower 32-bit DW_ahb_dmac Component Parameters Register 5. This register contains encoded information about the component parameter settings.
///////////////////////////////////////////////////////

#define DMAC_AHB_DMA_COMP_PARAMS_5_LOWER_OFFSET 0x3D0UL
#define DMAC_AHB_DMA_COMP_PARAMS_5_LOWER_NUM  0x1
#define DMAC_AHB_DMA_COMP_PARAMS_5_LOWER_CH6_DTW_SHIFT    0
#define DMAC_AHB_DMA_COMP_PARAMS_5_LOWER_CH6_STW_SHIFT    3
#define DMAC_AHB_DMA_COMP_PARAMS_5_LOWER_CH6_STAT_DST_SHIFT    6
#define DMAC_AHB_DMA_COMP_PARAMS_5_LOWER_CH6_STAT_SRC_SHIFT    7
#define DMAC_AHB_DMA_COMP_PARAMS_5_LOWER_CH6_DST_SCA_EN_SHIFT    8
#define DMAC_AHB_DMA_COMP_PARAMS_5_LOWER_CH6_SRC_GAT_EN_SHIFT    9
#define DMAC_AHB_DMA_COMP_PARAMS_5_LOWER_CH6_LOCK_EN_SHIFT    10
#define DMAC_AHB_DMA_COMP_PARAMS_5_LOWER_CH6_MULTI_BLK_EN_SHIFT    11
#define DMAC_AHB_DMA_COMP_PARAMS_5_LOWER_CH6_CTL_WB_EN_SHIFT    12
#define DMAC_AHB_DMA_COMP_PARAMS_5_LOWER_CH6_HC_LLP_SHIFT    13
#define DMAC_AHB_DMA_COMP_PARAMS_5_LOWER_CH6_FC_SHIFT    14
#define DMAC_AHB_DMA_COMP_PARAMS_5_LOWER_CH6_MAX_MULT_SIZE_SHIFT    16
#define DMAC_AHB_DMA_COMP_PARAMS_5_LOWER_CH6_DMS_SHIFT    19
#define DMAC_AHB_DMA_COMP_PARAMS_5_LOWER_CH6_LMS_SHIFT    22
#define DMAC_AHB_DMA_COMP_PARAMS_5_LOWER_CH6_SMS_SHIFT    25
#define DMAC_AHB_DMA_COMP_PARAMS_5_LOWER_CH6_FIFO_DEPTH_SHIFT    28
#define DMAC_AHB_DMA_COMP_PARAMS_5_LOWER_RESERVED_31_SHIFT    31

#define DMAC_AHB_DMA_COMP_PARAMS_5_LOWER_CH6_DTW_SIZE 3
#define DMAC_AHB_DMA_COMP_PARAMS_5_LOWER_CH6_STW_SIZE 3
#define DMAC_AHB_DMA_COMP_PARAMS_5_LOWER_CH6_STAT_DST_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_5_LOWER_CH6_STAT_SRC_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_5_LOWER_CH6_DST_SCA_EN_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_5_LOWER_CH6_SRC_GAT_EN_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_5_LOWER_CH6_LOCK_EN_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_5_LOWER_CH6_MULTI_BLK_EN_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_5_LOWER_CH6_CTL_WB_EN_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_5_LOWER_CH6_HC_LLP_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_5_LOWER_CH6_FC_SIZE 2
#define DMAC_AHB_DMA_COMP_PARAMS_5_LOWER_CH6_MAX_MULT_SIZE_SIZE 3
#define DMAC_AHB_DMA_COMP_PARAMS_5_LOWER_CH6_DMS_SIZE 3
#define DMAC_AHB_DMA_COMP_PARAMS_5_LOWER_CH6_LMS_SIZE 3
#define DMAC_AHB_DMA_COMP_PARAMS_5_LOWER_CH6_SMS_SIZE 3
#define DMAC_AHB_DMA_COMP_PARAMS_5_LOWER_CH6_FIFO_DEPTH_SIZE 3
#define DMAC_AHB_DMA_COMP_PARAMS_5_LOWER_RESERVED_31_SIZE 1

///////////////////////////////////////////////////////
// Register: DMA_COMP_PARAMS_5_UPPER
// Upper 32-bit DW_ahb_dmac Component Parameters Register 5. This register contains encoded information about the component parameter settings.
///////////////////////////////////////////////////////

#define DMAC_AHB_DMA_COMP_PARAMS_5_UPPER_OFFSET 0x3D4UL
#define DMAC_AHB_DMA_COMP_PARAMS_5_UPPER_NUM  0x1

#define DMAC_AHB_DMA_COMP_PARAMS_5_UPPER_CH5_DTW_SHIFT    0
#define DMAC_AHB_DMA_COMP_PARAMS_5_UPPER_CH5_STW_SHIFT    3
#define DMAC_AHB_DMA_COMP_PARAMS_5_UPPER_CH5_STAT_DST_SHIFT    6
#define DMAC_AHB_DMA_COMP_PARAMS_5_UPPER_CH5_STAT_SRC_SHIFT    7
#define DMAC_AHB_DMA_COMP_PARAMS_5_UPPER_CH5_DST_SCA_EN_SHIFT    8
#define DMAC_AHB_DMA_COMP_PARAMS_5_UPPER_CH5_SRC_GAT_EN_SHIFT    9
#define DMAC_AHB_DMA_COMP_PARAMS_5_UPPER_CH5_LOCK_EN_SHIFT    10
#define DMAC_AHB_DMA_COMP_PARAMS_5_UPPER_CH5_MULTI_BLK_EN_SHIFT    11
#define DMAC_AHB_DMA_COMP_PARAMS_5_UPPER_CH5_CTL_WB_EN_SHIFT    12
#define DMAC_AHB_DMA_COMP_PARAMS_5_UPPER_CH5_HC_LLP_SHIFT    13
#define DMAC_AHB_DMA_COMP_PARAMS_5_UPPER_CH5_FC_SHIFT    14
#define DMAC_AHB_DMA_COMP_PARAMS_5_UPPER_CH5_MAX_MULT_SIZE_SHIFT    16
#define DMAC_AHB_DMA_COMP_PARAMS_5_UPPER_CH5_DMS_SHIFT    19
#define DMAC_AHB_DMA_COMP_PARAMS_5_UPPER_CH5_LMS_SHIFT    22
#define DMAC_AHB_DMA_COMP_PARAMS_5_UPPER_CH5_SMS_SHIFT    25
#define DMAC_AHB_DMA_COMP_PARAMS_5_UPPER_CH5_FIFO_DEPTH_SHIFT    28
#define DMAC_AHB_DMA_COMP_PARAMS_5_UPPER_RESERVED_31_SHIFT    31
#define DMAC_AHB_DMA_COMP_PARAMS_5_UPPER_CH5_DTW_SIZE 3
#define DMAC_AHB_DMA_COMP_PARAMS_5_UPPER_CH5_STW_SIZE 3
#define DMAC_AHB_DMA_COMP_PARAMS_5_UPPER_CH5_STAT_DST_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_5_UPPER_CH5_STAT_SRC_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_5_UPPER_CH5_DST_SCA_EN_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_5_UPPER_CH5_SRC_GAT_EN_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_5_UPPER_CH5_LOCK_EN_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_5_UPPER_CH5_MULTI_BLK_EN_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_5_UPPER_CH5_CTL_WB_EN_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_5_UPPER_CH5_HC_LLP_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_5_UPPER_CH5_FC_SIZE 2
#define DMAC_AHB_DMA_COMP_PARAMS_5_UPPER_CH5_MAX_MULT_SIZE_SIZE 3
#define DMAC_AHB_DMA_COMP_PARAMS_5_UPPER_CH5_DMS_SIZE 3
#define DMAC_AHB_DMA_COMP_PARAMS_5_UPPER_CH5_LMS_SIZE 3
#define DMAC_AHB_DMA_COMP_PARAMS_5_UPPER_CH5_SMS_SIZE 3
#define DMAC_AHB_DMA_COMP_PARAMS_5_UPPER_CH5_FIFO_DEPTH_SIZE 3
#define DMAC_AHB_DMA_COMP_PARAMS_5_UPPER_RESERVED_31_SIZE 1


///////////////////////////////////////////////////////
// Register: DMA_COMP_PARAMS_4_LOWER
// Lower 32-bit DW_ahb_dmac Component Parameters Register 4. This register contains encoded information about the component parameter settings.
///////////////////////////////////////////////////////

#define DMAC_AHB_DMA_COMP_PARAMS_4_LOWER_OFFSET 0x3D8UL
#define DMAC_AHB_DMA_COMP_PARAMS_4_LOWER_NUM  0x1
#define DMAC_AHB_DMA_COMP_PARAMS_4_LOWER_CH4_DTW_SHIFT    0
#define DMAC_AHB_DMA_COMP_PARAMS_4_LOWER_CH4_STW_SHIFT    3
#define DMAC_AHB_DMA_COMP_PARAMS_4_LOWER_CH4_STAT_DST_SHIFT    6
#define DMAC_AHB_DMA_COMP_PARAMS_4_LOWER_CH4_STAT_SRC_SHIFT    7
#define DMAC_AHB_DMA_COMP_PARAMS_4_LOWER_CH4_DST_SCA_EN_SHIFT    8
#define DMAC_AHB_DMA_COMP_PARAMS_4_LOWER_CH4_SRC_GAT_EN_SHIFT    9
#define DMAC_AHB_DMA_COMP_PARAMS_4_LOWER_CH4_LOCK_EN_SHIFT    10
#define DMAC_AHB_DMA_COMP_PARAMS_4_LOWER_CH4_MULTI_BLK_EN_SHIFT    11
#define DMAC_AHB_DMA_COMP_PARAMS_4_LOWER_CH4_CTL_WB_EN_SHIFT    12
#define DMAC_AHB_DMA_COMP_PARAMS_4_LOWER_CH4_HC_LLP_SHIFT    13
#define DMAC_AHB_DMA_COMP_PARAMS_4_LOWER_CH4_FC_SHIFT    14
#define DMAC_AHB_DMA_COMP_PARAMS_4_LOWER_CH4_MAX_MULT_SIZE_SHIFT    16
#define DMAC_AHB_DMA_COMP_PARAMS_4_LOWER_CH4_DMS_SHIFT    19
#define DMAC_AHB_DMA_COMP_PARAMS_4_LOWER_CH4_LMS_SHIFT    22
#define DMAC_AHB_DMA_COMP_PARAMS_4_LOWER_CH4_SMS_SHIFT    25
#define DMAC_AHB_DMA_COMP_PARAMS_4_LOWER_CH4_FIFO_DEPTH_SHIFT    28
#define DMAC_AHB_DMA_COMP_PARAMS_4_LOWER_RESERVED_31_SHIFT    31

#define DMAC_AHB_DMA_COMP_PARAMS_4_LOWER_CH4_DTW_SIZE 3
#define DMAC_AHB_DMA_COMP_PARAMS_4_LOWER_CH4_STW_SIZE 3
#define DMAC_AHB_DMA_COMP_PARAMS_4_LOWER_CH4_STAT_DST_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_4_LOWER_CH4_STAT_SRC_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_4_LOWER_CH4_DST_SCA_EN_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_4_LOWER_CH4_SRC_GAT_EN_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_4_LOWER_CH4_LOCK_EN_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_4_LOWER_CH4_MULTI_BLK_EN_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_4_LOWER_CH4_CTL_WB_EN_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_4_LOWER_CH4_HC_LLP_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_4_LOWER_CH4_FC_SIZE 2
#define DMAC_AHB_DMA_COMP_PARAMS_4_LOWER_CH4_MAX_MULT_SIZE_SIZE 3
#define DMAC_AHB_DMA_COMP_PARAMS_4_LOWER_CH4_DMS_SIZE 3
#define DMAC_AHB_DMA_COMP_PARAMS_4_LOWER_CH4_LMS_SIZE 3
#define DMAC_AHB_DMA_COMP_PARAMS_4_LOWER_CH4_SMS_SIZE 3
#define DMAC_AHB_DMA_COMP_PARAMS_4_LOWER_CH4_FIFO_DEPTH_SIZE 3
#define DMAC_AHB_DMA_COMP_PARAMS_4_LOWER_RESERVED_31_SIZE 1

///////////////////////////////////////////////////////
// Register: DMA_COMP_PARAMS_4_UPPER
// Upper 32-bit DW_ahb_dmac Component Parameters Register 4. This register contains encoded information about the component parameter settings.
///////////////////////////////////////////////////////

#define DMAC_AHB_DMA_COMP_PARAMS_4_UPPER_OFFSET 0x3DCUL
#define DMAC_AHB_DMA_COMP_PARAMS_4_UPPER_NUM  0x1

#define DMAC_AHB_DMA_COMP_PARAMS_4_UPPER_CH3_DTW_SHIFT    0
#define DMAC_AHB_DMA_COMP_PARAMS_4_UPPER_CH3_STW_SHIFT    3
#define DMAC_AHB_DMA_COMP_PARAMS_4_UPPER_CH3_STAT_DST_SHIFT    6
#define DMAC_AHB_DMA_COMP_PARAMS_4_UPPER_CH3_STAT_SRC_SHIFT    7
#define DMAC_AHB_DMA_COMP_PARAMS_4_UPPER_CH3_DST_SCA_EN_SHIFT    8
#define DMAC_AHB_DMA_COMP_PARAMS_4_UPPER_CH3_SRC_GAT_EN_SHIFT    9
#define DMAC_AHB_DMA_COMP_PARAMS_4_UPPER_CH3_LOCK_EN_SHIFT    10
#define DMAC_AHB_DMA_COMP_PARAMS_4_UPPER_CH3_MULTI_BLK_EN_SHIFT    11
#define DMAC_AHB_DMA_COMP_PARAMS_4_UPPER_CH3_CTL_WB_EN_SHIFT    12
#define DMAC_AHB_DMA_COMP_PARAMS_4_UPPER_CH3_HC_LLP_SHIFT    13
#define DMAC_AHB_DMA_COMP_PARAMS_4_UPPER_CH3_FC_SHIFT    14
#define DMAC_AHB_DMA_COMP_PARAMS_4_UPPER_CH3_MAX_MULT_SIZE_SHIFT    16
#define DMAC_AHB_DMA_COMP_PARAMS_4_UPPER_CH3_DMS_SHIFT    19
#define DMAC_AHB_DMA_COMP_PARAMS_4_UPPER_CH3_LMS_SHIFT    22
#define DMAC_AHB_DMA_COMP_PARAMS_4_UPPER_CH3_SMS_SHIFT    25
#define DMAC_AHB_DMA_COMP_PARAMS_4_UPPER_CH3_FIFO_DEPTH_SHIFT    28
#define DMAC_AHB_DMA_COMP_PARAMS_4_UPPER_RESERVED_31_SHIFT    31
#define DMAC_AHB_DMA_COMP_PARAMS_4_UPPER_CH3_DTW_SIZE 3
#define DMAC_AHB_DMA_COMP_PARAMS_4_UPPER_CH3_STW_SIZE 3
#define DMAC_AHB_DMA_COMP_PARAMS_4_UPPER_CH3_STAT_DST_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_4_UPPER_CH3_STAT_SRC_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_4_UPPER_CH3_DST_SCA_EN_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_4_UPPER_CH3_SRC_GAT_EN_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_4_UPPER_CH3_LOCK_EN_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_4_UPPER_CH3_MULTI_BLK_EN_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_4_UPPER_CH3_CTL_WB_EN_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_4_UPPER_CH3_HC_LLP_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_4_UPPER_CH3_FC_SIZE 2
#define DMAC_AHB_DMA_COMP_PARAMS_4_UPPER_CH3_MAX_MULT_SIZE_SIZE 3
#define DMAC_AHB_DMA_COMP_PARAMS_4_UPPER_CH3_DMS_SIZE 3
#define DMAC_AHB_DMA_COMP_PARAMS_4_UPPER_CH3_LMS_SIZE 3
#define DMAC_AHB_DMA_COMP_PARAMS_4_UPPER_CH3_SMS_SIZE 3
#define DMAC_AHB_DMA_COMP_PARAMS_4_UPPER_CH3_FIFO_DEPTH_SIZE 3
#define DMAC_AHB_DMA_COMP_PARAMS_4_UPPER_RESERVED_31_SIZE 1


///////////////////////////////////////////////////////
// Register: DMA_COMP_PARAMS_3_LOWER
// Lower 32-bit DW_ahb_dmac Component Parameters Register 3. This register contains encoded information about the component parameter settings.
///////////////////////////////////////////////////////

#define DMAC_AHB_DMA_COMP_PARAMS_3_LOWER_OFFSET 0x3E0UL
#define DMAC_AHB_DMA_COMP_PARAMS_3_LOWER_NUM  0x1
#define DMAC_AHB_DMA_COMP_PARAMS_3_LOWER_CH2_DTW_SHIFT    0
#define DMAC_AHB_DMA_COMP_PARAMS_3_LOWER_CH2_STW_SHIFT    3
#define DMAC_AHB_DMA_COMP_PARAMS_3_LOWER_CH2_STAT_DST_SHIFT    6
#define DMAC_AHB_DMA_COMP_PARAMS_3_LOWER_CH2_STAT_SRC_SHIFT    7
#define DMAC_AHB_DMA_COMP_PARAMS_3_LOWER_CH2_DST_SCA_EN_SHIFT    8
#define DMAC_AHB_DMA_COMP_PARAMS_3_LOWER_CH2_SRC_GAT_EN_SHIFT    9
#define DMAC_AHB_DMA_COMP_PARAMS_3_LOWER_CH2_LOCK_EN_SHIFT    10
#define DMAC_AHB_DMA_COMP_PARAMS_3_LOWER_CH2_MULTI_BLK_EN_SHIFT    11
#define DMAC_AHB_DMA_COMP_PARAMS_3_LOWER_CH2_CTL_WB_EN_SHIFT    12
#define DMAC_AHB_DMA_COMP_PARAMS_3_LOWER_CH2_HC_LLP_SHIFT    13
#define DMAC_AHB_DMA_COMP_PARAMS_3_LOWER_CH2_FC_SHIFT    14
#define DMAC_AHB_DMA_COMP_PARAMS_3_LOWER_CH2_MAX_MULT_SIZE_SHIFT    16
#define DMAC_AHB_DMA_COMP_PARAMS_3_LOWER_CH2_DMS_SHIFT    19
#define DMAC_AHB_DMA_COMP_PARAMS_3_LOWER_CH2_LMS_SHIFT    22
#define DMAC_AHB_DMA_COMP_PARAMS_3_LOWER_CH2_SMS_SHIFT    25
#define DMAC_AHB_DMA_COMP_PARAMS_3_LOWER_CH2_FIFO_DEPTH_SHIFT    28
#define DMAC_AHB_DMA_COMP_PARAMS_3_LOWER_RESERVED_31_SHIFT    31

#define DMAC_AHB_DMA_COMP_PARAMS_3_LOWER_CH2_DTW_SIZE 3
#define DMAC_AHB_DMA_COMP_PARAMS_3_LOWER_CH2_STW_SIZE 3
#define DMAC_AHB_DMA_COMP_PARAMS_3_LOWER_CH2_STAT_DST_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_3_LOWER_CH2_STAT_SRC_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_3_LOWER_CH2_DST_SCA_EN_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_3_LOWER_CH2_SRC_GAT_EN_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_3_LOWER_CH2_LOCK_EN_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_3_LOWER_CH2_MULTI_BLK_EN_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_3_LOWER_CH2_CTL_WB_EN_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_3_LOWER_CH2_HC_LLP_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_3_LOWER_CH2_FC_SIZE 2
#define DMAC_AHB_DMA_COMP_PARAMS_3_LOWER_CH2_MAX_MULT_SIZE_SIZE 3
#define DMAC_AHB_DMA_COMP_PARAMS_3_LOWER_CH2_DMS_SIZE 3
#define DMAC_AHB_DMA_COMP_PARAMS_3_LOWER_CH2_LMS_SIZE 3
#define DMAC_AHB_DMA_COMP_PARAMS_3_LOWER_CH2_SMS_SIZE 3
#define DMAC_AHB_DMA_COMP_PARAMS_3_LOWER_CH2_FIFO_DEPTH_SIZE 3
#define DMAC_AHB_DMA_COMP_PARAMS_3_LOWER_RESERVED_31_SIZE 1

///////////////////////////////////////////////////////
// Register: DMA_COMP_PARAMS_3_UPPER
// Upper 32-bit DW_ahb_dmac Component Parameters Register 3. This register contains encoded information about the component parameter settings.
///////////////////////////////////////////////////////

#define DMAC_AHB_DMA_COMP_PARAMS_3_UPPER_OFFSET 0x3E4UL
#define DMAC_AHB_DMA_COMP_PARAMS_3_UPPER_NUM  0x1

#define DMAC_AHB_DMA_COMP_PARAMS_3_UPPER_CH1_DTW_SHIFT    0
#define DMAC_AHB_DMA_COMP_PARAMS_3_UPPER_CH1_STW_SHIFT    3
#define DMAC_AHB_DMA_COMP_PARAMS_3_UPPER_CH1_STAT_DST_SHIFT    6
#define DMAC_AHB_DMA_COMP_PARAMS_3_UPPER_CH1_STAT_SRC_SHIFT    7
#define DMAC_AHB_DMA_COMP_PARAMS_3_UPPER_CH1_DST_SCA_EN_SHIFT    8
#define DMAC_AHB_DMA_COMP_PARAMS_3_UPPER_CH1_SRC_GAT_EN_SHIFT    9
#define DMAC_AHB_DMA_COMP_PARAMS_3_UPPER_CH1_LOCK_EN_SHIFT    10
#define DMAC_AHB_DMA_COMP_PARAMS_3_UPPER_CH1_MULTI_BLK_EN_SHIFT    11
#define DMAC_AHB_DMA_COMP_PARAMS_3_UPPER_CH1_CTL_WB_EN_SHIFT    12
#define DMAC_AHB_DMA_COMP_PARAMS_3_UPPER_CH1_HC_LLP_SHIFT    13
#define DMAC_AHB_DMA_COMP_PARAMS_3_UPPER_CH1_FC_SHIFT    14
#define DMAC_AHB_DMA_COMP_PARAMS_3_UPPER_CH1_MAX_MULT_SIZE_SHIFT    16
#define DMAC_AHB_DMA_COMP_PARAMS_3_UPPER_CH1_DMS_SHIFT    19
#define DMAC_AHB_DMA_COMP_PARAMS_3_UPPER_CH1_LMS_SHIFT    22
#define DMAC_AHB_DMA_COMP_PARAMS_3_UPPER_CH1_SMS_SHIFT    25
#define DMAC_AHB_DMA_COMP_PARAMS_3_UPPER_CH1_FIFO_DEPTH_SHIFT    28
#define DMAC_AHB_DMA_COMP_PARAMS_3_UPPER_RESERVED_31_SHIFT    31
#define DMAC_AHB_DMA_COMP_PARAMS_3_UPPER_CH1_DTW_SIZE 3
#define DMAC_AHB_DMA_COMP_PARAMS_3_UPPER_CH1_STW_SIZE 3
#define DMAC_AHB_DMA_COMP_PARAMS_3_UPPER_CH1_STAT_DST_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_3_UPPER_CH1_STAT_SRC_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_3_UPPER_CH1_DST_SCA_EN_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_3_UPPER_CH1_SRC_GAT_EN_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_3_UPPER_CH1_LOCK_EN_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_3_UPPER_CH1_MULTI_BLK_EN_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_3_UPPER_CH1_CTL_WB_EN_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_3_UPPER_CH1_HC_LLP_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_3_UPPER_CH1_FC_SIZE 2
#define DMAC_AHB_DMA_COMP_PARAMS_3_UPPER_CH1_MAX_MULT_SIZE_SIZE 3
#define DMAC_AHB_DMA_COMP_PARAMS_3_UPPER_CH1_DMS_SIZE 3
#define DMAC_AHB_DMA_COMP_PARAMS_3_UPPER_CH1_LMS_SIZE 3
#define DMAC_AHB_DMA_COMP_PARAMS_3_UPPER_CH1_SMS_SIZE 3
#define DMAC_AHB_DMA_COMP_PARAMS_3_UPPER_CH1_FIFO_DEPTH_SIZE 3
#define DMAC_AHB_DMA_COMP_PARAMS_3_UPPER_RESERVED_31_SIZE 1


///////////////////////////////////////////////////////
// Register: DMA_COMP_PARAMS_2_LOWER
// Lower 32-bit DW_ahb_dmac Component Parameters Register 2. This register contains encoded information about the component parameter settings.
///////////////////////////////////////////////////////

#define DMAC_AHB_DMA_COMP_PARAMS_2_LOWER_OFFSET 0x3E8UL
#define DMAC_AHB_DMA_COMP_PARAMS_2_LOWER_NUM  0x1
#define DMAC_AHB_DMA_COMP_PARAMS_2_LOWER_CH0_DTW_SHIFT    0
#define DMAC_AHB_DMA_COMP_PARAMS_2_LOWER_CH0_STW_SHIFT    3
#define DMAC_AHB_DMA_COMP_PARAMS_2_LOWER_CH0_STAT_DST_SHIFT    6
#define DMAC_AHB_DMA_COMP_PARAMS_2_LOWER_CH0_STAT_SRC_SHIFT    7
#define DMAC_AHB_DMA_COMP_PARAMS_2_LOWER_CH0_DST_SCA_EN_SHIFT    8
#define DMAC_AHB_DMA_COMP_PARAMS_2_LOWER_CH0_SRC_GAT_EN_SHIFT    9
#define DMAC_AHB_DMA_COMP_PARAMS_2_LOWER_CH0_LOCK_EN_SHIFT    10
#define DMAC_AHB_DMA_COMP_PARAMS_2_LOWER_CH0_MULTI_BLK_EN_SHIFT    11
#define DMAC_AHB_DMA_COMP_PARAMS_2_LOWER_CH0_CTL_WB_EN_SHIFT    12
#define DMAC_AHB_DMA_COMP_PARAMS_2_LOWER_CH0_HC_LLP_SHIFT    13
#define DMAC_AHB_DMA_COMP_PARAMS_2_LOWER_CH0_FC_SHIFT    14
#define DMAC_AHB_DMA_COMP_PARAMS_2_LOWER_CH0_MAX_MULT_SIZE_SHIFT    16
#define DMAC_AHB_DMA_COMP_PARAMS_2_LOWER_CH0_DMS_SHIFT    19
#define DMAC_AHB_DMA_COMP_PARAMS_2_LOWER_CH0_LMS_SHIFT    22
#define DMAC_AHB_DMA_COMP_PARAMS_2_LOWER_CH0_SMS_SHIFT    25
#define DMAC_AHB_DMA_COMP_PARAMS_2_LOWER_CH0_FIFO_DEPTH_SHIFT    28
#define DMAC_AHB_DMA_COMP_PARAMS_2_LOWER_RESERVED_31_SHIFT    31

#define DMAC_AHB_DMA_COMP_PARAMS_2_LOWER_CH0_DTW_SIZE 3
#define DMAC_AHB_DMA_COMP_PARAMS_2_LOWER_CH0_STW_SIZE 3
#define DMAC_AHB_DMA_COMP_PARAMS_2_LOWER_CH0_STAT_DST_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_2_LOWER_CH0_STAT_SRC_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_2_LOWER_CH0_DST_SCA_EN_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_2_LOWER_CH0_SRC_GAT_EN_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_2_LOWER_CH0_LOCK_EN_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_2_LOWER_CH0_MULTI_BLK_EN_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_2_LOWER_CH0_CTL_WB_EN_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_2_LOWER_CH0_HC_LLP_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_2_LOWER_CH0_FC_SIZE 2
#define DMAC_AHB_DMA_COMP_PARAMS_2_LOWER_CH0_MAX_MULT_SIZE_SIZE 3
#define DMAC_AHB_DMA_COMP_PARAMS_2_LOWER_CH0_DMS_SIZE 3
#define DMAC_AHB_DMA_COMP_PARAMS_2_LOWER_CH0_LMS_SIZE 3
#define DMAC_AHB_DMA_COMP_PARAMS_2_LOWER_CH0_SMS_SIZE 3
#define DMAC_AHB_DMA_COMP_PARAMS_2_LOWER_CH0_FIFO_DEPTH_SIZE 3
#define DMAC_AHB_DMA_COMP_PARAMS_2_LOWER_RESERVED_31_SIZE 1

///////////////////////////////////////////////////////
// Register: DMA_COMP_PARAMS_2_UPPER
// Upper 32-bit DW_ahb_dmac Component Parameters Register 2. This register contains encoded information about the component parameter settings.
///////////////////////////////////////////////////////

#define DMAC_AHB_DMA_COMP_PARAMS_2_UPPER_OFFSET 0x3ECUL
#define DMAC_AHB_DMA_COMP_PARAMS_2_UPPER_NUM  0x1

#define DMAC_AHB_DMA_COMP_PARAMS_2_UPPER_CH0_MULTI_BLK_TYPE_SHIFT    0
#define DMAC_AHB_DMA_COMP_PARAMS_2_UPPER_CH1_MULTI_BLK_TYPE_SHIFT    4
#define DMAC_AHB_DMA_COMP_PARAMS_2_UPPER_CH2_MULTI_BLK_TYPE_SHIFT    8
#define DMAC_AHB_DMA_COMP_PARAMS_2_UPPER_CH3_MULTI_BLK_TYPE_SHIFT    12
#define DMAC_AHB_DMA_COMP_PARAMS_2_UPPER_CH4_MULTI_BLK_TYPE_SHIFT    16
#define DMAC_AHB_DMA_COMP_PARAMS_2_UPPER_CH5_MULTI_BLK_TYPE_SHIFT    20
#define DMAC_AHB_DMA_COMP_PARAMS_2_UPPER_CH6_MULTI_BLK_TYPE_SHIFT    24
#define DMAC_AHB_DMA_COMP_PARAMS_2_UPPER_CH7_MULTI_BLK_TYPE_SHIFT    28
#define DMAC_AHB_DMA_COMP_PARAMS_2_UPPER_CH0_MULTI_BLK_TYPE_SIZE 4
#define DMAC_AHB_DMA_COMP_PARAMS_2_UPPER_CH1_MULTI_BLK_TYPE_SIZE 4
#define DMAC_AHB_DMA_COMP_PARAMS_2_UPPER_CH2_MULTI_BLK_TYPE_SIZE 4
#define DMAC_AHB_DMA_COMP_PARAMS_2_UPPER_CH3_MULTI_BLK_TYPE_SIZE 4
#define DMAC_AHB_DMA_COMP_PARAMS_2_UPPER_CH4_MULTI_BLK_TYPE_SIZE 4
#define DMAC_AHB_DMA_COMP_PARAMS_2_UPPER_CH5_MULTI_BLK_TYPE_SIZE 4
#define DMAC_AHB_DMA_COMP_PARAMS_2_UPPER_CH6_MULTI_BLK_TYPE_SIZE 4
#define DMAC_AHB_DMA_COMP_PARAMS_2_UPPER_CH7_MULTI_BLK_TYPE_SIZE 4


///////////////////////////////////////////////////////
// Register: DMA_COMP_PARAMS_1_LOWER
// Lower 32-bit DW_ahb_dmac Component Parameters Register 1. This register contains encoded information about the component parameter settings.
///////////////////////////////////////////////////////

#define DMAC_AHB_DMA_COMP_PARAMS_1_LOWER_OFFSET 0x3F0UL
#define DMAC_AHB_DMA_COMP_PARAMS_1_LOWER_NUM  0x1
#define DMAC_AHB_DMA_COMP_PARAMS_1_LOWER_CH0_MAX_BLK_SIZE_SHIFT    0
#define DMAC_AHB_DMA_COMP_PARAMS_1_LOWER_CH1_MAX_BLK_SIZE_SHIFT    4
#define DMAC_AHB_DMA_COMP_PARAMS_1_LOWER_CH2_MAX_BLK_SIZE_SHIFT    8
#define DMAC_AHB_DMA_COMP_PARAMS_1_LOWER_CH3_MAX_BLK_SIZE_SHIFT    12
#define DMAC_AHB_DMA_COMP_PARAMS_1_LOWER_CH4_MAX_BLK_SIZE_SHIFT    16
#define DMAC_AHB_DMA_COMP_PARAMS_1_LOWER_CH5_MAX_BLK_SIZE_SHIFT    20
#define DMAC_AHB_DMA_COMP_PARAMS_1_LOWER_CH6_MAX_BLK_SIZE_SHIFT    24
#define DMAC_AHB_DMA_COMP_PARAMS_1_LOWER_CH7_MAX_BLK_SIZE_SHIFT    28

#define DMAC_AHB_DMA_COMP_PARAMS_1_LOWER_CH0_MAX_BLK_SIZE_SIZE 4
#define DMAC_AHB_DMA_COMP_PARAMS_1_LOWER_CH1_MAX_BLK_SIZE_SIZE 4
#define DMAC_AHB_DMA_COMP_PARAMS_1_LOWER_CH2_MAX_BLK_SIZE_SIZE 4
#define DMAC_AHB_DMA_COMP_PARAMS_1_LOWER_CH3_MAX_BLK_SIZE_SIZE 4
#define DMAC_AHB_DMA_COMP_PARAMS_1_LOWER_CH4_MAX_BLK_SIZE_SIZE 4
#define DMAC_AHB_DMA_COMP_PARAMS_1_LOWER_CH5_MAX_BLK_SIZE_SIZE 4
#define DMAC_AHB_DMA_COMP_PARAMS_1_LOWER_CH6_MAX_BLK_SIZE_SIZE 4
#define DMAC_AHB_DMA_COMP_PARAMS_1_LOWER_CH7_MAX_BLK_SIZE_SIZE 4

///////////////////////////////////////////////////////
// Register: DMA_COMP_PARAMS_1_UPPER
// Upper 32-bit DW_ahb_dmac Component Parameters Register 1. This register contains encoded information about the component parameter settings.
///////////////////////////////////////////////////////

#define DMAC_AHB_DMA_COMP_PARAMS_1_UPPER_OFFSET 0x3F4UL
#define DMAC_AHB_DMA_COMP_PARAMS_1_UPPER_NUM  0x1

#define DMAC_AHB_DMA_COMP_PARAMS_1_UPPER_BIG_ENDIAN_SHIFT    0
#define DMAC_AHB_DMA_COMP_PARAMS_1_UPPER_INTR_IO_SHIFT    1
#define DMAC_AHB_DMA_COMP_PARAMS_1_UPPER_MAX_ABRST_SHIFT    3
#define DMAC_AHB_DMA_COMP_PARAMS_1_UPPER_RESERVED_7_4_SHIFT    4
#define DMAC_AHB_DMA_COMP_PARAMS_1_UPPER_NUM_CHANNELS_SHIFT    8
#define DMAC_AHB_DMA_COMP_PARAMS_1_UPPER_NUM_MASTER_INT_SHIFT    11
#define DMAC_AHB_DMA_COMP_PARAMS_1_UPPER_S_HDATA_WIDTH_SHIFT    13
#define DMAC_AHB_DMA_COMP_PARAMS_1_UPPER_M4_HDATA_WIDTH_SHIFT    15
#define DMAC_AHB_DMA_COMP_PARAMS_1_UPPER_M3_HDATA_WIDTH_SHIFT    17
#define DMAC_AHB_DMA_COMP_PARAMS_1_UPPER_M2_HDATA_WIDTH_SHIFT    19
#define DMAC_AHB_DMA_COMP_PARAMS_1_UPPER_M1_HDATA_WIDTH_SHIFT    21
#define DMAC_AHB_DMA_COMP_PARAMS_1_UPPER_NUM_HS_INT_SHIFT    23
#define DMAC_AHB_DMA_COMP_PARAMS_1_UPPER_ADD_ENCODED_PARAMS_SHIFT    28
#define DMAC_AHB_DMA_COMP_PARAMS_1_UPPER_STATIC_ENDIAN_SELECT_SHIFT    29
#define DMAC_AHB_DMA_COMP_PARAMS_1_UPPER_RESERVED_31_30_SHIFT    30
#define DMAC_AHB_DMA_COMP_PARAMS_1_UPPER_BIG_ENDIAN_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_1_UPPER_INTR_IO_SIZE 2
#define DMAC_AHB_DMA_COMP_PARAMS_1_UPPER_MAX_ABRST_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_1_UPPER_RESERVED_7_4_SIZE 4
#define DMAC_AHB_DMA_COMP_PARAMS_1_UPPER_NUM_CHANNELS_SIZE 3
#define DMAC_AHB_DMA_COMP_PARAMS_1_UPPER_NUM_MASTER_INT_SIZE 2
#define DMAC_AHB_DMA_COMP_PARAMS_1_UPPER_S_HDATA_WIDTH_SIZE 2
#define DMAC_AHB_DMA_COMP_PARAMS_1_UPPER_M4_HDATA_WIDTH_SIZE 2
#define DMAC_AHB_DMA_COMP_PARAMS_1_UPPER_M3_HDATA_WIDTH_SIZE 2
#define DMAC_AHB_DMA_COMP_PARAMS_1_UPPER_M2_HDATA_WIDTH_SIZE 2
#define DMAC_AHB_DMA_COMP_PARAMS_1_UPPER_M1_HDATA_WIDTH_SIZE 2
#define DMAC_AHB_DMA_COMP_PARAMS_1_UPPER_NUM_HS_INT_SIZE 5
#define DMAC_AHB_DMA_COMP_PARAMS_1_UPPER_ADD_ENCODED_PARAMS_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_1_UPPER_STATIC_ENDIAN_SELECT_SIZE 1
#define DMAC_AHB_DMA_COMP_PARAMS_1_UPPER_RESERVED_31_30_SIZE 2


///////////////////////////////////////////////////////
// Register: DmaCompsID_LOWER
// Lower 32-bit DMA Component ID register
///////////////////////////////////////////////////////

#define DMAC_AHB_DMACOMPSID_LOWER_OFFSET 0x3F8UL
#define DMAC_AHB_DMACOMPSID_LOWER_NUM  0x1
#define DMAC_AHB_DMACOMPSID_LOWER_DMA_COMP_TYPE_SHIFT    0

#define DMAC_AHB_DMACOMPSID_LOWER_DMA_COMP_TYPE_SIZE 32

///////////////////////////////////////////////////////
// Register: DmaCompsID_UPPER
// Upper 32-bit DMA Component ID register
///////////////////////////////////////////////////////

#define DMAC_AHB_DMACOMPSID_UPPER_OFFSET 0x3FCUL
#define DMAC_AHB_DMACOMPSID_UPPER_NUM  0x1

#define DMAC_AHB_DMACOMPSID_UPPER_DMA_COMP_VERSION_SHIFT    0
#define DMAC_AHB_DMACOMPSID_UPPER_DMA_COMP_VERSION_SIZE 32


/**************************** Type Definitions ******************************/
typedef struct {
    AL_U32      DeviceId;
    AL_U32      BaseAddress;
    AL_U32      ChannelNum;
    AL_U32      IntrId;
} AL_DMACAHB_HwConfigStruct;

#ifdef __cplusplus
}
#endif

#endif
