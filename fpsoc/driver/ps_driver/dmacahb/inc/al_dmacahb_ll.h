#ifndef AL_DMACAHB_LL_H
#define AL_DMACAHB_LL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "al_reg_io.h"
#include "al_dmacahb_hw.h"

/* Transfer Type and Flow Control(TT_FC) */
typedef enum
{
    AL_DMACAHB_TT_FC_MEM2MEM, /* Transfer type is Memory to Memory and Flow Controller is DW_ahb_dmac */
    AL_DMACAHB_TT_FC_MEM2PER, /* Transfer type is Memory to Peripheral and Flow Controller is DW_ahb_dmac */
    AL_DMACAHB_TT_FC_PER2MEM, /* Transfer type is Peripheral to Memory and Flow Controller is DW_ahb_dmac */
    AL_DMACAHB_TT_FC_PER2PER  /* Not support */
} AL_DMACAHB_TtFcEnum;

typedef enum
{
    AL_DMACAHB_MSIZE_1,
    AL_DMACAHB_MSIZE_4,
    AL_DMACAHB_MSIZE_8,
    AL_DMACAHB_MSIZE_16,
    AL_DMACAHB_MSIZE_32,
    AL_DMACAHB_MSIZE_64,
    AL_DMACAHB_MSIZE_128,
    AL_DMACAHB_MSIZE_256
} AL_DMACAHB_MsizeEnum;

typedef enum
{
    AL_DMACAHB_ADDR_INC_INC,
    AL_DMACAHB_ADDR_INC_DEC,
    AL_DMACAHB_ADDR_INC_NO0,
    AL_DMACAHB_ADDR_INC_NO1
} AL_DMACAHB_AddrIncEnum;

typedef enum
{
    AL_DMACAHB_TRANS_WIDTH_8    = 0,
    AL_DMACAHB_TRANS_WIDTH_16,
    AL_DMACAHB_TRANS_WIDTH_32,
    AL_DMACAHB_TRANS_WIDTH_64,
    AL_DMACAHB_TRANS_WIDTH_128,
    AL_DMACAHB_TRANS_WIDTH_256_0,
    AL_DMACAHB_TRANS_WIDTH_256_1,
    AL_DMACAHB_TRANS_WIDTH_256_2
} AL_DMACAHB_TransWidthEnum;

typedef enum
{
    AL_DMACAHB_CH_PRIOR_0,
    AL_DMACAHB_CH_PRIOR_1,
    AL_DMACAHB_CH_PRIOR_2,
    AL_DMACAHB_CH_PRIOR_3,
    AL_DMACAHB_CH_PRIOR_4,
    AL_DMACAHB_CH_PRIOR_5,
    AL_DMACAHB_CH_PRIOR_6,
    AL_DMACAHB_CH_PRIOR_7
} AL_DMACAHB_ChPriorEnum;

typedef enum
{
    AL_DMACAHB_HAND_SHAKING_HARDWARE,
    AL_DMACAHB_HAND_SHAKING_SOFTWARE
} AL_DMACAHB_HsSelEnum;

typedef enum
{
    AL_DMACAHB_HS_POL_ACTIVE_HI,
    AL_DMACAHB_HS_POL_ACTIVE_LO
} AL_DMACAHB_HsPolSelEnum;

typedef enum
{
    AL_DMACAHB_FCMODE_0,    /* Source data pre-fetching */
    AL_DMACAHB_FCMODE_1     /* Source data transfer wait for destination request */
} AL_DMACAHB_FlowCtlModeEnum;

typedef enum
{
    AL_DMACAHB_FIFO_MODE_0,  /* Space/data available for single AHB transfer */
    AL_DMACAHB_FIFO_MODE_1   /* Space/data available is greater than or equal to half the FIFO depth */
} AL_DMACAHB_FifoModeEnum;

/**
 * CFGx.PROTCTL[1] to HPROT[1]: Privileged
 * CFGx.PROTCTL[2] to HPROT[2]: Bufferable
 * CFGx.PROTCTL[3] to HPROT[3]: Modifiable
*/
typedef enum
{
    AL_DMACAHB_PROT_0,
    AL_DMACAHB_PROT_1,
    AL_DMACAHB_PROT_2,
    AL_DMACAHB_PROT_3,
    AL_DMACAHB_PROT_4,
    AL_DMACAHB_PROT_5,
    AL_DMACAHB_PROT_6,
    AL_DMACAHB_PROT_7
} AL_DMACAHB_ProtCtlSelEnum;

typedef enum
{
    AL_DMACAHB_PER_I2C0_TX,
    AL_DMACAHB_PER_I2C0_RX,
    AL_DMACAHB_PER_I2C1_TX,
    AL_DMACAHB_PER_I2C1_RX,
    AL_DMACAHB_PER_SPI0_TX,
    AL_DMACAHB_PER_SPI0_RX,
    AL_DMACAHB_PER_SPI1_TX,
    AL_DMACAHB_PER_SPI1_RX,
    AL_DMACAHB_PER_UART0_TX,
    AL_DMACAHB_PER_UART0_RX,
    AL_DMACAHB_PER_UART1_TX,
    AL_DMACAHB_PER_UART1_RX,
    AL_DMACAHB_PER_QSPI_TX,
    AL_DMACAHB_PER_QSPI_RX,
    AL_DMACAHB_PER_CAN0_RX,
    AL_DMACAHB_PER_CAN1_RX
} AL_DMACAHB_PerSelEnum;

/* --------------------DMAC/Channel_x_Registers-------------------- */



static inline AL_U32 AlDmacAhb_ll_ReadSrcAddr(AL_REG BaseAddr, AL_REG ChannelOffset)
{
    return AL_REG32_READ(BaseAddr + DMAC_AHB_SAR_OFFSET + ChannelOffset);
}

static inline AL_VOID AlDmacAhb_ll_WriteSrcAddr(AL_REG BaseAddr, AL_REG ChannelOffset, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + DMAC_AHB_SAR_OFFSET + ChannelOffset, Value);
}

static inline AL_U32 AlDmacAhb_ll_ReadDstAddr(AL_REG BaseAddr, AL_REG ChannelOffset)
{
    return AL_REG32_READ(BaseAddr + DMAC_AHB_DAR_OFFSET + ChannelOffset);
}

static inline AL_VOID AlDmacAhb_ll_WriteDstAddr(AL_REG BaseAddr, AL_REG ChannelOffset, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + DMAC_AHB_DAR_OFFSET + ChannelOffset, Value);
}

/* List Master Select(LMS), hardware config 2 */
static inline AL_U32 AlDmacAhb_ll_GetLms(AL_REG BaseAddr, AL_REG ChannelOffset)
{
    return AL_REG32_GET_BITS(BaseAddr + DMAC_AHB_LLP_OFFSET + ChannelOffset, DMAC_AHB_LLP_LMS_SHIFT,
                    DMAC_AHB_LLP_LOC_SHIFT);
}

static inline AL_VOID AlDmacAhb_ll_SetLms(AL_REG BaseAddr, AL_REG ChannelOffset, AL_U32 Value)
{
    AL_REG32_SET_BITS(BaseAddr + DMAC_AHB_LLP_OFFSET + ChannelOffset, DMAC_AHB_LLP_LMS_SHIFT, DMAC_AHB_LLP_LOC_SHIFT,
             Value);
}

/* Starting Address In Memory of next LLI if block chaining is enabled */
static inline AL_U32 AlDmacAhb_ll_GetLinkStartAddr(AL_REG BaseAddr, AL_REG ChannelOffset)
{
    return AL_REG32_READ(BaseAddr + DMAC_AHB_LLP_OFFSET + ChannelOffset) & 0xFFFFFFFC;
}

static inline AL_VOID AlDmacAhb_ll_SetLinkStartAddr(AL_REG BaseAddr, AL_REG ChannelOffset, AL_U32 Value)
{
    AL_REG32_SET_BITS(BaseAddr + DMAC_AHB_LLP_OFFSET + ChannelOffset, DMAC_AHB_LLP_LOC_SHIFT, DMAC_AHB_LLP_LOC_SIZE,
             (Value >> 2));
}

/* Interrupt Enable Bit */
static inline AL_BOOL AlDmacAhb_ll_IsIntrEn(AL_REG BaseAddr, AL_REG ChannelOffset)
{
    return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + DMAC_AHB_CTL0_LOWER_OFFSET + ChannelOffset,
                            DMAC_AHB_CTL0_LOWER_INT_EN_SHIFT);
}

static inline AL_VOID AlDmacAhb_ll_SetIntrEn(AL_REG BaseAddr, AL_REG ChannelOffset, AL_BOOL IsEnabled)
{
    AL_REG32_SET_BIT(BaseAddr + DMAC_AHB_CTL0_LOWER_OFFSET + ChannelOffset,
            DMAC_AHB_CTL0_LOWER_INT_EN_SHIFT, IsEnabled);
}

/* Destination Transfer Width(DST_TR_WIDTH) */
static inline AL_U32 AlDmacAhb_ll_GetDstTransWidth(AL_REG BaseAddr, AL_REG ChannelOffset)
{
    return AL_REG32_GET_BITS(BaseAddr + DMAC_AHB_CTL0_UPPER_OFFSET + ChannelOffset,
                    DMAC_AHB_CTL0_LOWER_DST_TR_WIDTH_SHIFT, DMAC_AHB_CTL0_LOWER_DST_TR_WIDTH_SIZE);
}

static inline AL_VOID AlDmacAhb_ll_SetDstTransWidth(AL_REG BaseAddr, AL_REG ChannelOffset,
                                                    AL_DMACAHB_TransWidthEnum DsttTransWidth)
{
    AL_REG32_SET_BITS(BaseAddr + DMAC_AHB_CTL0_UPPER_OFFSET + ChannelOffset, DMAC_AHB_CTL0_LOWER_DST_TR_WIDTH_SHIFT,
             DMAC_AHB_CTL0_LOWER_DST_TR_WIDTH_SIZE, DsttTransWidth);
}

/* Source Transfer Width(SRC_TR_WIDTH) */
static inline AL_U32 AlDmacAhb_ll_GetSrcTransWidth(AL_REG BaseAddr, AL_REG ChannelOffset)
{
    return AL_REG32_GET_BITS(BaseAddr + DMAC_AHB_CTL0_UPPER_OFFSET + ChannelOffset, DMAC_AHB_CTL0_LOWER_SRC_TR_WIDTH_SHIFT,
                    DMAC_AHB_CTL0_LOWER_SRC_TR_WIDTH_SIZE);
}

static inline AL_VOID AlDmacAhb_ll_SetSrcTransWidth(AL_REG BaseAddr, AL_REG ChannelOffset,
                                                    AL_DMACAHB_TransWidthEnum SrcTransWidth)
{
    AL_REG32_SET_BITS(BaseAddr + DMAC_AHB_CTL0_UPPER_OFFSET + ChannelOffset, DMAC_AHB_CTL0_LOWER_SRC_TR_WIDTH_SHIFT,
             DMAC_AHB_CTL0_LOWER_SRC_TR_WIDTH_SIZE, SrcTransWidth);
}

/* Destination Address Increment */
static inline AL_U32 AlDmacAhb_ll_GetDstIncMode(AL_REG BaseAddr, AL_REG ChannelOffset)
{
    return AL_REG32_GET_BITS(BaseAddr + DMAC_AHB_CTL0_UPPER_OFFSET + ChannelOffset, DMAC_AHB_CTL0_LOWER_DEST_MSIZE_SHIFT,
                    DMAC_AHB_CTL0_LOWER_DINC_SIZE);
}

static inline AL_VOID AlDmacAhb_ll_SetDstIncMode(AL_REG BaseAddr, AL_REG ChannelOffset,
    AL_DMACAHB_AddrIncEnum DstInc)
{
    AL_REG32_SET_BITS(BaseAddr + DMAC_AHB_CTL0_UPPER_OFFSET + ChannelOffset, DMAC_AHB_CTL0_LOWER_DEST_MSIZE_SHIFT,
             DMAC_AHB_CTL0_LOWER_DINC_SIZE, DstInc);
}

/* Source Address Increment(SINC) */
static inline AL_U32 AlDmacAhb_ll_GetSrcIncMode(AL_REG BaseAddr, AL_REG ChannelOffset)
{
    return AL_REG32_GET_BITS(BaseAddr + DMAC_AHB_CTL0_UPPER_OFFSET + ChannelOffset, DMAC_AHB_CTL0_LOWER_SINC_SHIFT,
                    DMAC_AHB_CTL0_LOWER_SINC_SIZE);
}

static inline AL_VOID AlDmacAhb_ll_SetSrcIncModee(AL_REG BaseAddr, AL_REG ChannelOffset,
                                                  AL_DMACAHB_AddrIncEnum SrcInc)
{
    AL_REG32_SET_BITS(BaseAddr + DMAC_AHB_CTL0_UPPER_OFFSET + ChannelOffset, DMAC_AHB_CTL0_LOWER_SINC_SHIFT,
             DMAC_AHB_CTL0_LOWER_SINC_SIZE, SrcInc);
}

/* Destination Burst Transaction Length(DST_MSIZE) */
static inline AL_U32 AlDmacAhb_ll_GetDstMsize(AL_REG BaseAddr, AL_REG ChannelOffset)
{
    return AL_REG32_GET_BITS(BaseAddr + DMAC_AHB_CTL0_UPPER_OFFSET + ChannelOffset, DMAC_AHB_CTL0_LOWER_DEST_MSIZE_SHIFT,
                    DMAC_AHB_CTL0_LOWER_DEST_MSIZE_SIZE);
}

static inline AL_VOID AlDmacAhb_ll_SetDstMsize(AL_REG BaseAddr, AL_REG ChannelOffset, AL_DMACAHB_MsizeEnum Msize)
{
    AL_REG32_SET_BITS(BaseAddr + DMAC_AHB_CTL0_UPPER_OFFSET + ChannelOffset, DMAC_AHB_CTL0_LOWER_DEST_MSIZE_SHIFT,
             DMAC_AHB_CTL0_LOWER_DEST_MSIZE_SIZE, Msize);
}

/* Source Burst Transaction Length(SRC_MSIZE) */
static inline AL_U32 AlDmacAhb_ll_GetSrcMsize(AL_REG BaseAddr, AL_REG ChannelOffset)
{
    return AL_REG32_GET_BITS(BaseAddr + DMAC_AHB_CTL0_UPPER_OFFSET + ChannelOffset, DMAC_AHB_CTL0_LOWER_SRC_MSIZE_SHIFT,
                    DMAC_AHB_CTL0_LOWER_SRC_MSIZE_SIZE);
}

static inline AL_VOID AlDmacAhb_ll_SetSrcMsize(AL_REG BaseAddr, AL_REG ChannelOffset, AL_DMACAHB_MsizeEnum Msize)
{
    AL_REG32_SET_BITS(BaseAddr + DMAC_AHB_CTL0_UPPER_OFFSET + ChannelOffset, DMAC_AHB_CTL0_LOWER_SRC_MSIZE_SHIFT,
             DMAC_AHB_CTL0_LOWER_SRC_MSIZE_SIZE, Msize);
}

/* Source gather enable */
static inline AL_BOOL AlDmacAhb_ll_IsSrcGatEn(AL_REG BaseAddr, AL_REG ChannelOffset)
{
    return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + DMAC_AHB_CTL0_LOWER_OFFSET + ChannelOffset,
                            DMAC_AHB_CTL0_LOWER_SRC_GATHER_EN_SHIFT);
}

static inline AL_VOID AlDmacAhb_ll_SetSrcGatEn(AL_REG BaseAddr, AL_REG ChannelOffset, AL_BOOL IsEnabled)
{
    AL_REG32_SET_BIT(BaseAddr + DMAC_AHB_CTL0_LOWER_OFFSET + ChannelOffset, DMAC_AHB_CTL0_LOWER_SRC_GATHER_EN_SHIFT,
            IsEnabled);
}

/* Destination scatter enable */
static inline AL_BOOL AlDmacAhb_ll_IsDstScaEn(AL_REG BaseAddr, AL_REG ChannelOffset)
{
    return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + DMAC_AHB_CTL0_LOWER_OFFSET + ChannelOffset,
                            DMAC_AHB_CTL0_LOWER_DST_SCATTER_EN_SHIFT);
}

static inline AL_VOID AlDmacAhb_ll_SetDstScaEn(AL_REG BaseAddr, AL_REG ChannelOffset, AL_BOOL IsEnabled)
{
    AL_REG32_SET_BIT(BaseAddr + DMAC_AHB_CTL0_LOWER_OFFSET + ChannelOffset, DMAC_AHB_CTL0_LOWER_DST_SCATTER_EN_SHIFT,
            IsEnabled);
}

/* Transfer Type and Flow Control(TT_FC), hardware config DMA_FC_ONLY, bit[2] always reads back 0 */
static inline AL_U32 AlDmacAhb_ll_GetTtFc(AL_REG BaseAddr, AL_REG ChannelOffset)
{
    return AL_REG32_GET_BITS(BaseAddr + DMAC_AHB_CTL0_UPPER_OFFSET + ChannelOffset, DMAC_AHB_CTL0_LOWER_TT_FC_SHIFT,
                    DMAC_AHB_CTL0_LOWER_DMS_SIZE);
}

static inline AL_VOID AlDmacAhb_ll_SetTtFc(AL_REG BaseAddr, AL_REG ChannelOffset, AL_DMACAHB_TtFcEnum TtFc)
{
    AL_REG32_SET_BITS(BaseAddr + DMAC_AHB_CTL0_UPPER_OFFSET + ChannelOffset, DMAC_AHB_CTL0_LOWER_TT_FC_SHIFT,
             DMAC_AHB_CTL0_LOWER_DMS_SIZE, TtFc);
}

/* Destination Master Select(DMS), hardware config 2 */
static inline AL_U32 AlDmacAhb_ll_GetDms(AL_REG BaseAddr, AL_REG ChannelOffset)
{
    return AL_REG32_GET_BITS(BaseAddr + DMAC_AHB_CTL0_UPPER_OFFSET + ChannelOffset, DMAC_AHB_CTL0_LOWER_DMS_SHIFT,
                    DMAC_AHB_CTL0_LOWER_DMS_SIZE);
}

static inline AL_VOID AlDmacAhb_ll_SetDms(AL_REG BaseAddr, AL_REG ChannelOffset,
    AL_U32 Value)
{
    AL_REG32_SET_BITS(BaseAddr + DMAC_AHB_CTL0_UPPER_OFFSET + ChannelOffset, DMAC_AHB_CTL0_LOWER_DMS_SHIFT,
             DMAC_AHB_CTL0_LOWER_DMS_SIZE, Value);
}

/* Source Master Select(SMS), hardware config 2 */
static inline AL_U32 AlDmacAhb_ll_GetSms(AL_REG BaseAddr, AL_REG ChannelOffset)
{
    return AL_REG32_GET_BITS(BaseAddr + DMAC_AHB_CTL0_UPPER_OFFSET + ChannelOffset, DMAC_AHB_CTL0_LOWER_SMS_SHIFT,
                    DMAC_AHB_CTL0_LOWER_SMS_SIZE);
}

static inline AL_VOID AlDmacAhb_ll_SetSms(AL_REG BaseAddr, AL_REG ChannelOffset,
    AL_U32 Value)
{
    AL_REG32_SET_BITS(BaseAddr + DMAC_AHB_CTL0_UPPER_OFFSET + ChannelOffset, DMAC_AHB_CTL0_LOWER_SMS_SHIFT,
             DMAC_AHB_CTL0_LOWER_SMS_SIZE, Value);
}

static inline AL_BOOL AlDmacAhb_ll_IsLlpDstEn(AL_REG BaseAddr, AL_REG ChannelOffset)
{
    return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + DMAC_AHB_CTL0_LOWER_OFFSET + ChannelOffset,
                            DMAC_AHB_CTL0_LOWER_LLP_DST_EN_SHIFT);
}

static inline AL_VOID AlDmacAhb_ll_SetLlpDstEn(AL_REG BaseAddr, AL_REG ChannelOffset, AL_BOOL IsEnabled)
{
    AL_REG32_SET_BIT(BaseAddr + DMAC_AHB_CTL0_LOWER_OFFSET + ChannelOffset, DMAC_AHB_CTL0_LOWER_LLP_DST_EN_SHIFT, IsEnabled);
}

static inline AL_BOOL AlDmacAhb_ll_IsLlpSrcEn(AL_REG BaseAddr, AL_REG ChannelOffset)
{
    return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + DMAC_AHB_CTL0_LOWER_OFFSET + ChannelOffset,
                            DMAC_AHB_CTL0_LOWER_LLP_SRC_EN_SHIFT);
}

static inline AL_VOID AlDmacAhb_ll_SetLlpSrcEn(AL_REG BaseAddr, AL_REG ChannelOffset, AL_BOOL IsEnabled)
{
    AL_REG32_SET_BIT(BaseAddr + DMAC_AHB_CTL0_LOWER_OFFSET + ChannelOffset, DMAC_AHB_CTL0_LOWER_LLP_SRC_EN_SHIFT, IsEnabled);
}

/* Block transfer size(BLOCK_TS), hardware config width 12 */
static inline AL_U32 AlDmacAhb_ll_GetBlkTransSize(AL_REG BaseAddr, AL_REG ChannelOffset)
{
    return AL_REG32_GET_BITS(BaseAddr + DMAC_AHB_CTL0_UPPER_OFFSET + ChannelOffset, DMAC_AHB_CTL0_UPPER_BLOCK_TS_SHIFT,
                    DMAC_AHB_CTL0_UPPER_BLOCK_TS_SIZE);
}

static inline AL_VOID AlDmacAhb_ll_SetBlkTransSize(AL_REG BaseAddr, AL_REG ChannelOffset, AL_U32 Value)
{
    AL_REG32_SET_BITS(BaseAddr + DMAC_AHB_CTL0_UPPER_OFFSET + ChannelOffset, DMAC_AHB_CTL0_UPPER_BLOCK_TS_SHIFT,
             DMAC_AHB_CTL0_UPPER_BLOCK_TS_SIZE, Value);
}

static inline AL_BOOL AlDmacAhb_ll_GetDoneBit(AL_REG BaseAddr, AL_REG ChannelOffset)
{
    return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + DMAC_AHB_CTL0_UPPER_OFFSET + ChannelOffset, DMAC_AHB_CTL0_UPPER_DONE_SHIFT);
}

static inline AL_VOID AlDmacAhb_ll_SetDoneBit(AL_REG BaseAddr, AL_REG ChannelOffset, AL_BOOL IsSet)
{
    AL_REG32_SET_BIT(BaseAddr + DMAC_AHB_CTL0_UPPER_OFFSET + ChannelOffset, DMAC_AHB_CTL0_UPPER_DONE_SHIFT, IsSet);
}

/* Channel Priority */
static inline AL_U32 AlDmacAhb_ll_GetChPrior(AL_REG BaseAddr, AL_REG ChannelOffset)
{
    return AL_REG32_GET_BITS(BaseAddr + DMAC_AHB_CFG0_LOWER_OFFSET + ChannelOffset, DMAC_AHB_CFG0_LOWER_CH_PRIOR_SHIFT,
                    DMAC_AHB_CFG0_LOWER_CH_PRIOR_SIZE);
}

static inline AL_VOID AlDmacAhb_ll_SetChPrior(AL_REG BaseAddr, AL_REG ChannelOffset, AL_DMACAHB_ChPriorEnum Prior)
{
    AL_REG32_SET_BITS(BaseAddr + DMAC_AHB_CFG0_LOWER_OFFSET + ChannelOffset, DMAC_AHB_CFG0_LOWER_CH_PRIOR_SHIFT,
             DMAC_AHB_CFG0_LOWER_CH_PRIOR_SIZE, Prior);
}

/* Channel Suspend */
static inline AL_BOOL AlDmacAhb_ll_IsChSusp(AL_REG BaseAddr, AL_REG ChannelOffset)
{
    return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + DMAC_AHB_CFG0_LOWER_OFFSET + ChannelOffset,
                            DMAC_AHB_CFG0_LOWER_CH_SUSP_SHIFT);
}

static inline AL_VOID AlDmacAhb_ll_SetChSusp(AL_REG BaseAddr, AL_REG ChannelOffset, AL_BOOL IsSet)
{
    AL_REG32_SET_BIT(BaseAddr + DMAC_AHB_CFG0_LOWER_OFFSET + ChannelOffset, DMAC_AHB_CFG0_LOWER_CH_SUSP_SHIFT, IsSet);
}

/* Channel FIFO status */
static inline AL_BOOL AlDmacAhb_ll_IsChFifoEmpty(AL_REG BaseAddr, AL_REG ChannelOffset)
{
    return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + DMAC_AHB_CFG0_LOWER_OFFSET + ChannelOffset,
                            DMAC_AHB_CFG0_LOWER_FIFO_EMPTY_SHIFT);
}

/* Destination Software or Hardware Handshaking Select */
static inline AL_DMACAHB_HsSelEnum AlDmacAhb_ll_GetDstHsSel(AL_REG BaseAddr, AL_REG ChannelOffset)
{
    return (AL_DMACAHB_HsSelEnum)AL_REG32_GET_BIT(BaseAddr + DMAC_AHB_CFG0_LOWER_OFFSET + ChannelOffset,
                                         DMAC_AHB_CFG0_LOWER_HS_SEL_DST_SHIFT);
}

static inline AL_VOID AlDmacAhb_ll_SetDstHsSel(AL_REG BaseAddr, AL_REG ChannelOffset, AL_DMACAHB_HsSelEnum HsSel)
{
    AL_REG32_SET_BIT(BaseAddr + DMAC_AHB_CFG0_LOWER_OFFSET + ChannelOffset, DMAC_AHB_CFG0_LOWER_HS_SEL_DST_SHIFT, HsSel);
}

/* Source Software or Hardware Handshaking Select */
static inline AL_DMACAHB_HsSelEnum AlDmacAhb_ll_GetSrcHsSel(AL_REG BaseAddr, AL_REG ChannelOffset)
{
    return (AL_DMACAHB_HsSelEnum)AL_REG32_GET_BIT(BaseAddr + DMAC_AHB_CFG0_LOWER_OFFSET + ChannelOffset,
                                         DMAC_AHB_CFG0_LOWER_HS_SEL_SRC_SHIFT);
}

static inline AL_VOID AlDmacAhb_ll_SetSrcHsSel(AL_REG BaseAddr, AL_REG ChannelOffset, AL_DMACAHB_HsSelEnum HsSel)
{
    AL_REG32_SET_BIT(BaseAddr + DMAC_AHB_CFG0_LOWER_OFFSET + ChannelOffset, DMAC_AHB_CFG0_LOWER_HS_SEL_SRC_SHIFT, HsSel);
}

/* Destination Handshaking Interface Polarity */
static inline AL_DMACAHB_HsPolSelEnum AlDmacAhb_ll_GetDstHsPol(AL_REG BaseAddr, AL_REG ChannelOffset)
{
    return (AL_DMACAHB_HsPolSelEnum)AL_REG32_GET_BIT(BaseAddr + DMAC_AHB_CFG0_LOWER_OFFSET + ChannelOffset,
                                            DMAC_AHB_CFG0_LOWER_DST_HS_POL_SHIFT);
}

static inline AL_VOID AlDmacAhb_ll_SetDstHsPol(AL_REG BaseAddr, AL_REG ChannelOffset,
                                               AL_DMACAHB_HsPolSelEnum HsPolSel)
{
    AL_REG32_SET_BIT(BaseAddr + DMAC_AHB_CFG0_LOWER_OFFSET + ChannelOffset, DMAC_AHB_CFG0_LOWER_DST_HS_POL_SHIFT, HsPolSel);
}

/* Source Handshaking Interface Polarity */
static inline AL_DMACAHB_HsPolSelEnum AlDmacAhb_ll_GetSrcHsPol(AL_REG BaseAddr, AL_REG ChannelOffset)
{
    return (AL_DMACAHB_HsPolSelEnum)AL_REG32_GET_BIT(BaseAddr + DMAC_AHB_CFG0_LOWER_OFFSET + ChannelOffset,
                                            DMAC_AHB_CFG0_LOWER_SRC_HS_POL_SHIFT);
}

static inline AL_VOID AlDmacAhb_ll_SetSrcHsPol(AL_REG BaseAddr, AL_REG ChannelOffset,
                                               AL_DMACAHB_HsPolSelEnum HsPolSel)
{
    AL_REG32_SET_BIT(BaseAddr + DMAC_AHB_CFG0_LOWER_OFFSET + ChannelOffset, DMAC_AHB_CFG0_LOWER_SRC_HS_POL_SHIFT, HsPolSel);
}

/* Automatic Source Reload */
static inline AL_BOOL AlDmacAhb_ll_IsSrcAutoReload(AL_REG BaseAddr, AL_REG ChannelOffset)
{
    return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + DMAC_AHB_CFG0_LOWER_OFFSET + ChannelOffset,
                            DMAC_AHB_CFG0_LOWER_RELOAD_SRC_SHIFT);
}

static inline AL_VOID AlDmacAhb_ll_SetSrcAutoReload(AL_REG BaseAddr, AL_REG ChannelOffset, AL_BOOL IsEnabled)
{
    AL_REG32_SET_BIT(BaseAddr + DMAC_AHB_CFG0_LOWER_OFFSET + ChannelOffset, DMAC_AHB_CFG0_LOWER_RELOAD_SRC_SHIFT, IsEnabled);
}

/* Automatic Destination Reload */
static inline AL_BOOL AlDmacAhb_ll_IsDstAutoReload(AL_REG BaseAddr, AL_REG ChannelOffset)
{
    return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + DMAC_AHB_CFG0_LOWER_OFFSET + ChannelOffset,
                            DMAC_AHB_CFG0_LOWER_RELOAD_DST_SHIFT);
}

static inline AL_VOID AlDmacAhb_ll_SetDstAutoReload(AL_REG BaseAddr, AL_REG ChannelOffset, AL_BOOL IsEnabled)
{
    AL_REG32_SET_BIT(BaseAddr + DMAC_AHB_CFG0_LOWER_OFFSET + ChannelOffset, DMAC_AHB_CFG0_LOWER_RELOAD_DST_SHIFT, IsEnabled);
}

/* Flow Control Mode */
static inline AL_DMACAHB_FlowCtlModeEnum AlDmacAhb_ll_GetFlowCtlMode(AL_REG BaseAddr, AL_REG ChannelOffset)
{
    return (AL_DMACAHB_FlowCtlModeEnum)AL_REG32_GET_BIT(BaseAddr + DMAC_AHB_CFG0_UPPER_OFFSET + ChannelOffset,
                                               DMAC_AHB_CFG0_UPPER_FCMODE_SHIFT);
}

static inline AL_VOID AlDmacAhb_ll_SetFlowCtlMode(AL_REG BaseAddr, AL_REG ChannelOffset,
                                                  AL_DMACAHB_FlowCtlModeEnum Mode)
{
    AL_REG32_SET_BIT(BaseAddr + DMAC_AHB_CFG0_UPPER_OFFSET + ChannelOffset, DMAC_AHB_CFG0_UPPER_FCMODE_SHIFT, Mode);
}

/* FIFO Mode Select */
static inline AL_DMACAHB_FifoModeEnum AlDmacAhb_ll_GetFifoMode(AL_REG BaseAddr, AL_REG ChannelOffset)
{
    return (AL_DMACAHB_FifoModeEnum)AL_REG32_GET_BIT(BaseAddr + DMAC_AHB_CFG0_UPPER_OFFSET + ChannelOffset,
                                            DMAC_AHB_CFG0_UPPER_FIFO_MODE_SHIFT);
}

static inline AL_VOID AlDmacAhb_ll_SetFifoMode(AL_REG BaseAddr, AL_REG ChannelOffset, AL_DMACAHB_FifoModeEnum Mode)
{
    AL_REG32_SET_BIT(BaseAddr + DMAC_AHB_CFG0_UPPER_OFFSET + ChannelOffset, DMAC_AHB_CFG0_UPPER_FIFO_MODE_SHIFT, Mode);
}

/* Protection Control */
static inline AL_DMACAHB_ProtCtlSelEnum AlDmacAhb_ll_GetProtCtl(AL_REG BaseAddr, AL_REG ChannelOffset)
{
    return (AL_DMACAHB_ProtCtlSelEnum)AL_REG32_GET_BITS(BaseAddr + DMAC_AHB_CFG0_UPPER_OFFSET + ChannelOffset,
                                               DMAC_AHB_CFG0_UPPER_PROTCTL_SHIFT,
                                               DMAC_AHB_CFG0_UPPER_PROTCTL_SIZE);
}

static inline AL_VOID AlDmacAhb_ll_SetProtCtl(AL_REG BaseAddr, AL_REG ChannelOffset, AL_DMACAHB_ProtCtlSelEnum Sel)
{
    AL_REG32_SET_BITS(BaseAddr + DMAC_AHB_CFG0_UPPER_OFFSET + ChannelOffset, DMAC_AHB_CFG0_UPPER_PROTCTL_SHIFT,
             DMAC_AHB_CFG0_UPPER_PROTCTL_SIZE, Sel);
}

/* Source Hardware Interface */
static inline AL_DMACAHB_PerSelEnum AlDmacAhb_ll_GetSrcPer(AL_REG BaseAddr, AL_REG ChannelOffset)
{
    return (AL_DMACAHB_PerSelEnum)AL_REG32_GET_BITS(BaseAddr + DMAC_AHB_CFG0_UPPER_OFFSET + ChannelOffset,
                                           DMAC_AHB_CFG0_UPPER_SRC_PER_SHIFT, DMAC_AHB_CFG0_UPPER_SRC_PER_SIZE);
}

static inline AL_VOID AlDmacAhb_ll_SetSrcPer(AL_REG BaseAddr, AL_REG ChannelOffset, AL_DMACAHB_PerSelEnum Sel)
{
    AL_REG32_SET_BITS(BaseAddr + DMAC_AHB_CFG0_UPPER_OFFSET + ChannelOffset, DMAC_AHB_CFG0_UPPER_SRC_PER_SHIFT,
             DMAC_AHB_CFG0_UPPER_SRC_PER_SIZE, Sel);
}

/* Destination Hardware Interface */
static inline AL_DMACAHB_PerSelEnum AlDmacAhb_ll_GetDstPer(AL_REG BaseAddr, AL_REG ChannelOffset)
{
    return (AL_DMACAHB_PerSelEnum)AL_REG32_GET_BITS(BaseAddr + DMAC_AHB_CFG0_UPPER_OFFSET + ChannelOffset,
                                           DMAC_AHB_CFG0_UPPER_DEST_PER_SHIFT, DMAC_AHB_CFG0_UPPER_DEST_PER_SIZE);
}

static inline AL_VOID AlDmacAhb_ll_SetDstPer(AL_REG BaseAddr, AL_REG ChannelOffset, AL_DMACAHB_PerSelEnum Sel)
{
    AL_REG32_SET_BITS(BaseAddr + DMAC_AHB_CFG0_UPPER_OFFSET + ChannelOffset, DMAC_AHB_CFG0_UPPER_DEST_PER_SHIFT,
             DMAC_AHB_CFG0_UPPER_DEST_PER_SIZE, Sel);
}

/* Source Gather Interval(SGI) */
static inline AL_U32 AlDmacAhb_ll_GetSgi(AL_REG BaseAddr, AL_REG ChannelOffset)
{
    return AL_REG32_GET_BITS(BaseAddr + DMAC_AHB_SGR_OFFSET + ChannelOffset, DMAC_AHB_SGR_SGI_SHIFT,
                    DMAC_AHB_SGR_SGC_SHIFT);
}

static inline AL_VOID AlDmacAhb_ll_SetSgi(AL_REG BaseAddr, AL_REG ChannelOffset, AL_U32 Value)
{
    AL_REG32_SET_BITS(BaseAddr + DMAC_AHB_SGR_OFFSET + ChannelOffset, DMAC_AHB_SGR_SGI_SHIFT, DMAC_AHB_SGR_SGC_SHIFT,
             Value);
}

/* Source Gather Count(SGC) */
static inline AL_U32 AlDmacAhb_ll_GetSgc(AL_REG BaseAddr, AL_REG ChannelOffset)
{
    return AL_REG32_GET_BITS(BaseAddr + DMAC_AHB_SGR_OFFSET + ChannelOffset, DMAC_AHB_SGR_SGC_SHIFT,
                    DMAC_AHB_SGR_SGC_SIZE);
}

static inline AL_VOID AlDmacAhb_ll_SetSgc(AL_REG BaseAddr, AL_REG ChannelOffset, AL_U32 Value)
{
    AL_REG32_SET_BITS(BaseAddr + DMAC_AHB_SGR_OFFSET + ChannelOffset, DMAC_AHB_SGR_SGC_SHIFT, DMAC_AHB_SGR_SGC_SHIFT,
             Value);
}

/* Destination Scatter Interval(DSI) */
static inline AL_U32 AlDmacAhb_ll_GetDsi(AL_REG BaseAddr, AL_REG ChannelOffset)
{
    return AL_REG32_GET_BITS(BaseAddr + DMAC_AHB_DSR_OFFSET + ChannelOffset, DMAC_AHB_DSR_DSI_SHIFT,
                    DMAC_AHB_DSR_DSI_SIZE);
}

static inline AL_VOID AlDmacAhb_ll_SetDsi(AL_REG BaseAddr, AL_REG ChannelOffset, AL_U32 Value)
{
    AL_REG32_SET_BITS(BaseAddr + DMAC_AHB_DSR_OFFSET + ChannelOffset, DMAC_AHB_DSR_DSI_SHIFT, DMAC_AHB_DSR_DSI_SIZE,
             Value);
}

/* Destination Scatter Count(DSC) */
static inline AL_U32 AlDmacAhb_ll_GetDsc(AL_REG BaseAddr, AL_REG ChannelOffset)
{
    return AL_REG32_GET_BITS(BaseAddr + DMAC_AHB_DSR_OFFSET + ChannelOffset, DMAC_AHB_DSR_DSI_SHIFT,
                    DMAC_AHB_DSR_DSI_SIZE);
}

static inline AL_VOID AlDmacAhb_ll_SetDsc(AL_REG BaseAddr, AL_REG ChannelOffset, AL_U32 Value)
{
    AL_REG32_SET_BITS(BaseAddr + DMAC_AHB_DSR_OFFSET + ChannelOffset, DMAC_AHB_DSR_DSI_SHIFT, DMAC_AHB_DSR_DSI_SIZE,
             Value);
}

/* Control Register for Channelx */
static inline AL_U32 AlDmacAhb_ll_ReadCtlLo(AL_REG BaseAddr, AL_REG ChannelOffset)
{
    return AL_REG32_READ(BaseAddr + DMAC_AHB_CTL0_LOWER_OFFSET + ChannelOffset);
}

static inline AL_VOID AlDmacAhb_ll_WriteCtlLo(AL_REG BaseAddr, AL_REG ChannelOffset, AL_U32 CtlConfig)
{
    AL_REG32_WRITE(BaseAddr + DMAC_AHB_CTL0_LOWER_OFFSET + ChannelOffset, CtlConfig);
}

/* Control Register for Channelx */
static inline AL_U32 AlDmacAhb_ll_ReadCtlHi(AL_REG BaseAddr, AL_REG ChannelOffset)
{
    return AL_REG32_READ(BaseAddr + DMAC_AHB_CTL0_UPPER_OFFSET + ChannelOffset);
}

static inline AL_VOID AlDmacAhb_ll_WriteCtlHi(AL_REG BaseAddr, AL_REG ChannelOffset, AL_U32 CtlConfig)
{
    AL_REG32_WRITE(BaseAddr + DMAC_AHB_CTL0_UPPER_OFFSET + ChannelOffset, CtlConfig);
}

/* Configuration Register for Channel x */
static inline AL_U32 AlDmacAhb_ll_ReadCfgLo(AL_REG BaseAddr, AL_REG ChannelOffset)
{
    return AL_REG32_READ(BaseAddr + DMAC_AHB_CFG0_LOWER_OFFSET + ChannelOffset);
}

static inline AL_VOID AlDmacAhb_ll_WriteCfgLo(AL_REG BaseAddr, AL_REG ChannelOffset, AL_U32 Cfg)
{
    AL_REG32_WRITE(BaseAddr + DMAC_AHB_CFG0_LOWER_OFFSET + ChannelOffset, Cfg);
}

/* Configuration Register for Channel x */
static inline AL_U32 AlDmacAhb_ll_ReadCfgHi(AL_REG BaseAddr, AL_REG ChannelOffset)
{
    return AL_REG32_READ(BaseAddr + DMAC_AHB_CFG0_UPPER_OFFSET + ChannelOffset);
}

static inline AL_VOID AlDmacAhb_ll_WriteCfgHi(AL_REG BaseAddr, AL_REG ChannelOffset, AL_U32 Cfg)
{
    AL_REG32_WRITE(BaseAddr + DMAC_AHB_CFG0_UPPER_OFFSET + ChannelOffset, Cfg);
}

/* Source Gather Register for Channel x */
static inline AL_U32 AlDmacAhb_ll_ReadSrcGatherReg(AL_REG BaseAddr, AL_REG ChannelOffset)
{
    return AL_REG64_READ(BaseAddr + DMAC_AHB_SGR_OFFSET + ChannelOffset);
}

static inline AL_VOID AlDmacAhb_ll_WriteSrcGatherReg(AL_REG BaseAddr, AL_REG ChannelOffset, AL_U32 Value)
{
    AL_REG64_WRITE(BaseAddr + DMAC_AHB_SGR_OFFSET + ChannelOffset, Value);
}

/* Destination Scatter Register for Channel x */
static inline AL_U32 AlDmacAhb_ll_ReadDstGatherReg(AL_REG BaseAddr, AL_REG ChannelOffset)
{
    return AL_REG64_READ(BaseAddr + DMAC_AHB_DSR_OFFSET + ChannelOffset);
}

static inline AL_VOID AlDmacAhb_ll_WriteDstGatherReg(AL_REG BaseAddr, AL_REG ChannelOffset, AL_U32 Value)
{
    AL_REG64_WRITE(BaseAddr + DMAC_AHB_DSR_OFFSET + ChannelOffset, Value);
}



/* --------------------DMAC/Interrupt Registers-------------------- */



/* Raw Status for IntTfr Interrupt(RawTfr) */
static inline AL_U32 AlDmacAhb_ll_ReadRawTfr(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + DMAC_AHB_RAWTFR_OFFSET);
}

static inline AL_VOID AlDmacAhb_ll_WriteRawTfr(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + DMAC_AHB_RAWTFR_OFFSET, Value);
}

/* Raw Status for IntBlock Interrupt(RawBlock) */
static inline AL_U32 AlDmacAhb_ll_ReadRawBlock(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + DMAC_AHB_RAWBLOCK_OFFSET);
}

static inline AL_VOID AlDmacAhb_ll_WriteRawBlock(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + DMAC_AHB_RAWBLOCK_OFFSET, Value);
}

/* Raw Status for IntSrcTran Interrupt(RawSrcTran) */
static inline AL_U32 AlDmacAhb_ll_ReadRawSrcTran(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + DMAC_AHB_RAWSRCTRAN_OFFSET);
}

static inline AL_VOID AlDmacAhb_ll_WriteRawSrcTran(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + DMAC_AHB_RAWSRCTRAN_OFFSET, Value);
}

/* Raw Status for IntDstTran Interrupt(RawDstTran) */
static inline AL_U32 AlDmacAhb_ll_ReadRawDstTran(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + DMAC_AHB_RAWDSTTRAN_OFFSET);
}

static inline AL_VOID AlDmacAhb_ll_WriteRawDstTran(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + DMAC_AHB_RAWDSTTRAN_OFFSET, Value);
}

/* Raw Status for IntErr Interrupt(RawErr) */
static inline AL_U32 AlDmacAhb_ll_ReadRawErr(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + DMAC_AHB_RAWERR_OFFSET);
}

static inline AL_VOID AlDmacAhb_ll_WriteRawErr(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + DMAC_AHB_RAWERR_OFFSET, Value);
}

/* Status for IntTfr Interrupt(StatusTfr) */
static inline AL_U32 AlDmacAhb_ll_ReadStatusTfr(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + DMAC_AHB_STATUSTFR_OFFSET);
}

static inline AL_VOID AlDmacAhb_ll_WriteStatusTfr(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + DMAC_AHB_STATUSTFR_OFFSET, Value);
}

/* Status for IntBlock Interrupt(StatusBlock) */
static inline AL_U32 AlDmacAhb_ll_ReadStatusBlock(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + DMAC_AHB_STATUSBLOCK_OFFSET);
}

static inline AL_VOID AlDmacAhb_ll_WriteStatusBlock(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + DMAC_AHB_STATUSBLOCK_OFFSET, Value);
}

/* Status for IntSrcTran Interrupt(StatusSrcTran) */
static inline AL_U32 AlDmacAhb_ll_ReadStatusSrcTran(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + DMAC_AHB_STATUSSRCTRAN_OFFSET);
}

static inline AL_VOID AlDmacAhb_ll_WriteStatusSrcTran(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + DMAC_AHB_STATUSSRCTRAN_OFFSET, Value);
}

/* Status for IntDstTran Interrupt(StatusDstTran) */
static inline AL_U32 AlDmacAhb_ll_ReadStatusDstTran(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + DMAC_AHB_STATUSDSTTRAN_OFFSET);
}

static inline AL_VOID AlDmacAhb_ll_WriteStatusDstTran(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + DMAC_AHB_STATUSDSTTRAN_OFFSET, Value);
}

/* Status for IntErr Interrupt(StatusErr) */
static inline AL_U32 AlDmacAhb_ll_ReadStatusErr(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + DMAC_AHB_STATUSERR_OFFSET);
}

static inline AL_VOID AlDmacAhb_ll_WriteStatusErr(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + DMAC_AHB_STATUSERR_OFFSET, Value);
}

/* Mask for IntTfr Interrupt(MaskTfr) */
static inline AL_U32 AlDmacAhb_ll_GetMaskTfr(AL_REG BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr + DMAC_AHB_MASKTFR_OFFSET, DMAC_AHB_MASKTFR_INT_MASK_SHIFT,
                    DMAC_AHB_MASKTFR_INT_MASK_SIZE);
}

static inline AL_VOID AlDmacAhb_ll_SetMaskTfr(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + DMAC_AHB_MASKTFR_OFFSET, (Value | (Value << DMAC_AHB_MASKTFR_INT_MASK_WE_SHIFT)));
}

/* Mask for IntBlock Interrupt(MaskBlock) */
static inline AL_U32 AlDmacAhb_ll_GetMaskBlock(AL_REG BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr + DMAC_AHB_MASKBLOCK_OFFSET, DMAC_AHB_MASKBLOCK_INT_MASK_SHIFT,
                    DMAC_AHB_MASKBLOCK_INT_MASK_SIZE);
}

static inline AL_VOID AlDmacAhb_ll_SetMaskBlock(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + DMAC_AHB_MASKBLOCK_OFFSET, (Value | (Value << DMAC_AHB_MASKBLOCK_INT_MASK_WE_SHIFT)));
}

/* Mask for IntSrcTran Interrupt(MaskSrcTran) */
static inline AL_U32 AlDmacAhb_ll_GetMaskSrcTran(AL_REG BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr + DMAC_AHB_MASKSRCTRAN_OFFSET, DMAC_AHB_MASKSRCTRAN_INT_MASK_SHIFT,
                    DMAC_AHB_MASKSRCTRAN_INT_MASK_SIZE);
}

static inline AL_VOID AlDmacAhb_ll_SetMaskSrcTran(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + DMAC_AHB_MASKSRCTRAN_OFFSET, (Value | (Value << DMAC_AHB_MASKSRCTRAN_INT_MASK_WE_SHIFT)));
}

/* Mask for IntDstTran Interrupt(MaskDstTran) */
static inline AL_U32 AlDmacAhb_ll_GetMaskDstTran(AL_REG BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr + DMAC_AHB_MASKDSTTRAN_OFFSET, DMAC_AHB_MASKDSTTRAN_INT_MASK_SHIFT,
                    DMAC_AHB_MASKDSTTRAN_INT_MASK_SIZE);
}

static inline AL_VOID AlDmacAhb_ll_SetMaskDstTran(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + DMAC_AHB_MASKDSTTRAN_OFFSET, (Value | (Value << DMAC_AHB_MASKDSTTRAN_INT_MASK_WE_SHIFT)));
}

/* Mask for IntErr Interrupt(MaskErr) */
static inline AL_U32 AlDmacAhb_ll_GetMaskErr(AL_REG BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr + DMAC_AHB_MASKERR_OFFSET, DMAC_AHB_MASKERR_INT_MASK_SHIFT,
                    DMAC_AHB_MASKERR_INT_MASK_SIZE);
}

static inline AL_VOID AlDmacAhb_ll_SetMaskErr(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + DMAC_AHB_MASKERR_OFFSET, (Value | Value << DMAC_AHB_MASKERR_INT_MASK_WE_SHIFT));
}

static inline AL_VOID AlDmacAhb_ll_WriteClearTfr(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + DMAC_AHB_CLEARTFR_OFFSET, Value);
}

static inline AL_VOID AlDmacAhb_ll_WriteClearBlock(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + DMAC_AHB_CLEARBLOCK_OFFSET, Value);
}

static inline AL_VOID AlDmacAhb_ll_WriteClearSrcTran(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + DMAC_AHB_CLEARSRCTRAN_OFFSET, Value);
}

static inline AL_VOID AlDmacAhb_ll_WriteClearDstTran(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + DMAC_AHB_CLEARDSTTRAN_OFFSET, Value);
}

static inline AL_VOID AlDmacAhb_ll_WriteClearErr(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + DMAC_AHB_CLEARERR_OFFSET, Value);
}

/* Status for each Interrupt type(StatusInt) */
static inline AL_U32 AlDmacAhb_ll_ReadStatusIntr(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + DMAC_AHB_STATUSINT_OFFSET);
}



/* --------------------DMAC/Software_Handshake Registers-------------------- */



/* Source Software Transaction Request(SRC_REQ) */
static inline AL_U32 AlDmacAhb_ll_GetSrcReq(AL_REG BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr + DMAC_AHB_REQSRCREG_OFFSET, DMAC_AHB_REQSRCREG_SRC_REQ_SHIFT,
                    DMAC_AHB_REQSRCREG_SRC_REQ_SIZE);
}

static inline AL_VOID AlDmacAhb_ll_SetSrcReqAct(AL_REG BaseAddr, AL_U32 ChannelMask, AL_BOOL IsEnable)
{
    AL_REG32_WRITE(BaseAddr + DMAC_AHB_REQSRCREG_OFFSET,
                   ((IsEnable ? ChannelMask : 0) | (ChannelMask << DMAC_AHB_REQSRCREG_SRC_REQ_WE_SHIFT)));
}

/* Destination Software Transaction Request(DST_REQ) */
static inline AL_U32 AlDmacAhb_ll_GetDstReq(AL_REG BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr + DMAC_AHB_REQDSTREG_OFFSET, DMAC_AHB_REQDSTREG_DST_REQ_SHIFT,
                    DMAC_AHB_REQDSTREG_DST_REQ_SIZE);
}

static inline AL_VOID AlDmacAhb_ll_SetDstReqAct(AL_REG BaseAddr, AL_U32 ChannelMask, AL_BOOL IsEnable)
{
    AL_REG32_WRITE(BaseAddr + DMAC_AHB_REQDSTREG_OFFSET,
                   ((IsEnable ? ChannelMask : 0) | (ChannelMask << DMAC_AHB_REQDSTREG_DST_REQ_WE_SHIFT)));
}

/* Source Single Transaction Request(SRC_SGLREQ) */
static inline AL_U32 AlDmacAhb_ll_GetSrcSglReq(AL_REG BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr + DMAC_AHB_SGLRQSRCREG_OFFSET, DMAC_AHB_SGLRQSRCREG_SRC_SGLREQ_SHIFT,
                    DMAC_AHB_SGLRQSRCREG_SRC_SGLREQ_SIZE);
}

static inline AL_VOID AlDmacAhb_ll_SetSrcSglReqAct(AL_REG BaseAddr, AL_U32 ChannelMask, AL_BOOL IsEnable)
{
    AL_REG32_WRITE(BaseAddr + DMAC_AHB_SGLRQSRCREG_OFFSET,
                   ((IsEnable ? ChannelMask : 0) | (ChannelMask << DMAC_AHB_SGLRQSRCREG_SRC_SGLREQ_WE_SHIFT)));
}

/* Destination Single Transaction Request(DST_SGLREQ) */
static inline AL_U32 AlDmacAhb_ll_GetDstSglReq(AL_REG BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr + DMAC_AHB_SGLRQDSTREG_OFFSET, DMAC_AHB_SGLRQDSTREG_DST_SGLREQ_SHIFT,
                    DMAC_AHB_SGLRQDSTREG_DST_SGLREQ_SIZE);
}

static inline AL_VOID AlDmacAhb_ll_SetDstSglReqAct(AL_REG BaseAddr, AL_U32 ChannelMask, AL_BOOL IsEnable)
{
    AL_REG32_WRITE(BaseAddr + DMAC_AHB_SGLRQDSTREG_OFFSET,
                ((IsEnable ? ChannelMask : 0) | (ChannelMask << DMAC_AHB_SGLRQDSTREG_DST_SGLREQ_WE_SHIFT)));
}

/* Source Last Transaction Request register(LSTSRC) */
static inline AL_U32 AlDmacAhb_ll_GetLstSrc(AL_REG BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr + DMAC_AHB_LSTSRCREG_OFFSET, DMAC_AHB_LSTSRCREG_LSTSRC_SHIFT,
                    DMAC_AHB_LSTSRCREG_LSTSRC_SIZE);
}

static inline AL_VOID AlDmacAhb_ll_SetLstSrcAct(AL_REG BaseAddr, AL_U32 ChannelMask, AL_BOOL IsEnable)
{
    AL_REG32_WRITE(BaseAddr + DMAC_AHB_LSTSRCREG_OFFSET,
                ((IsEnable ? ChannelMask : 0) | (ChannelMask << DMAC_AHB_LSTSRCREG_LSTSRC_WE_SHIFT)));
}

/* Destination Last Transaction Request(LSTDST) */
static inline AL_U32 AlDmacAhb_ll_GetLstDstReq(AL_REG BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr + DMAC_AHB_LSTDSTREG_OFFSET, DMAC_AHB_LSTDSTREG_LSTDST_SHIFT,
                    DMAC_AHB_LSTDSTREG_LSTDST_SIZE);
}

static inline AL_VOID AlDmacAhb_ll_SetLstDstReqAct(AL_REG BaseAddr, AL_U32 ChannelMask, AL_BOOL IsEnable)
{
    AL_REG32_WRITE(BaseAddr + DMAC_AHB_LSTDSTREG_OFFSET,
                ((IsEnable ? ChannelMask : 0) | (ChannelMask << DMAC_AHB_LSTDSTREG_LSTDST_WE_SHIFT)));
}


/* --------------------DMAC/Miscellaneous Registers-------------------- */



/* DW_ahb_dmac Configuration Register(DmaCfgReg) */
static inline AL_BOOL AlDmacAhb_ll_IsDmaCfgEn(AL_REG BaseAddr)
{
    return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + DMAC_AHB_DMACFGREG_OFFSET, DMAC_AHB_DMACFGREG_DMA_EN_SHIFT);
}

static inline AL_VOID AlDmacAhb_ll_SetDmaCfgEn(AL_REG BaseAddr, AL_BOOL IsEnable)
{
    AL_REG32_SET_BIT(BaseAddr + DMAC_AHB_DMACFGREG_OFFSET, DMAC_AHB_DMACFGREG_DMA_EN_SHIFT, IsEnable);
}

/* Channel Enable(CH_EN) */
static inline AL_U32 AlDmacAhb_ll_GetChannelEn(AL_REG BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr + DMAC_AHB_CHENREG_OFFSET, DMAC_AHB_CHENREG_CH_EN_SHIFT,
                    DMAC_AHB_CHENREG_CH_EN_SIZE);
}

static inline AL_VOID AlDmacAhb_ll_SetChannelEn(AL_REG BaseAddr, AL_U32 ChannelMask, AL_BOOL IsEnable)
{
    AL_U32 Reg = AL_REG32_READ(BaseAddr + DMAC_AHB_CHENREG_OFFSET);
    Reg |= ((IsEnable ? ChannelMask : 0) | (ChannelMask << DMAC_AHB_CHENREG_CH_EN_WE_SHIFT));
    AL_REG32_WRITE(BaseAddr + DMAC_AHB_CHENREG_OFFSET, Reg);
}

/* DW_ahb_dmac ID register(DmaIdReg) */
static inline AL_U32 AlDmacAhb_ll_ReadDmaIdReg(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + DMAC_AHB_DMAIDREG_OFFSET);
}

/* DMA Test registers(DmaTestReg) */
static inline AL_BOOL AlDmacAhb_ll_GetDmaTestMode(AL_REG BaseAddr)
{
    return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + DMAC_AHB_DMATESTREG_OFFSET, DMAC_AHB_DMATESTREG_TEST_SLV_IF_SHIFT);
}

static inline AL_VOID AlDmacAhb_ll_SetDmaTestMode(AL_REG BaseAddr, AL_BOOL IsTest)
{
    AL_REG32_SET_BIT(BaseAddr + DMAC_AHB_DMATESTREG_OFFSET, DMAC_AHB_DMATESTREG_TEST_SLV_IF_SHIFT, IsTest);
}

/* DW_ahb_dmac Component Parameters: channel x */
static inline AL_U32 AlDmacAhb_ll_ReadDmaCompChannelx(AL_REG BaseAddr, AL_U8 ChOffset)
{
    return AL_REG32_READ(BaseAddr + DMAC_AHB_DMA_COMP_PARAMS_2_LOWER_OFFSET - (ChOffset << 5));
}

// /* DW_ahb_dmac Component Parameters Register 6(DMA_COMP_PARAMS_6): channel 7 */
// static inline AL_U32 AlDmacAhb_ll_ReadDmaCompChannel7(AL_REG BaseAddr)
// {
//     return AL_REG32_READ(BaseAddr + DMAC_AHB_DMA_COMP_PARAMS_6_OFFSET);
// }

// /* DW_ahb_dmac Component Parameters Register 5(DMA_COMP_PARAMS_5): channel 6 */
// static inline AL_U32 AlDmacAhb_ll_ReadDmaCompChannel6(AL_REG BaseAddr)
// {
//     return AL_REG32_READ(BaseAddr + DMAC_AHB_DMA_COMP_PARAMS_5_LOWER_OFFSET);
// }

// /* DW_ahb_dmac Component Parameters Register 5(DMA_COMP_PARAMS_5): channel 5 */
// static inline AL_U32 AlDmacAhb_ll_ReadDmaCompChannel5(AL_REG BaseAddr)
// {
//     return AL_REG32_READ(BaseAddr + DMAC_AHB_DMA_COMP_PARAMS_5_UPPER_OFFSET);
// }

// /* DW_ahb_dmac Component Parameters Register 4(DMA_COMP_PARAMS_4): channel 4 */
// static inline AL_U32 AlDmacAhb_ll_ReadDmaCompChannel4(AL_REG BaseAddr)
// {
//     return AL_REG32_READ(BaseAddr + DMAC_AHB_DMA_COMP_PARAMS_4_LOWER_OFFSET);
// }

// /* DW_ahb_dmac Component Parameters Register 4(DMA_COMP_PARAMS_4): channel 3 */
// static inline AL_U32 AlDmacAhb_ll_ReadDmaCompChannel3(AL_REG BaseAddr)
// {
//     return AL_REG32_READ(BaseAddr + DMAC_AHB_DMA_COMP_PARAMS_4_UPPER_OFFSET);
// }

// /* DW_ahb_dmac Component Parameters Register 3(DMA_COMP_PARAMS_3): channel 2 */
// static inline AL_U32 AlDmacAhb_ll_ReadDmaCompChannel2(AL_REG BaseAddr)
// {
//     return AL_REG32_READ(BaseAddr + DMAC_AHB_DMA_COMP_PARAMS_3_LOWER_OFFSET);
// }

// /* DW_ahb_dmac Component Parameters Register 3(DMA_COMP_PARAMS_3): channel 1 */
// static inline AL_U32 AlDmacAhb_ll_ReadDmaCompChannel1(AL_REG BaseAddr)
// {
//     return AL_REG32_READ(BaseAddr + DMAC_AHB_DMA_COMP_PARAMS_3_UPPER_OFFSET);
// }

// /* DW_ahb_dmac Component Parameters Register 2(DMA_COMP_PARAMS_2): channel 0 */
// static inline AL_U32 AlDmacAhb_ll_ReadDmaCompChannel0(AL_REG BaseAddr)
// {
//     return AL_REG32_READ(BaseAddr + DMAC_AHB_DMA_COMP_PARAMS_2_LOWER_OFFSET);
// }

/* DW_ahb_dmac Component Parameters Register 2(DMA_COMP_PARAMS_2): multi blk type */
static inline AL_U32 AlDmacAhb_ll_ReadDmaCompChMultiBlkType(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + DMAC_AHB_DMA_COMP_PARAMS_2_UPPER_OFFSET);
}

/* DW_ahb_dmac Component Parameters Register 1(DMA_COMP_PARAMS_1): max blk size */
static inline AL_U32 AlDmacAhb_ll_ReadDmaCompChMaxBlkSize(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + DMAC_AHB_DMA_COMP_PARAMS_1_LOWER_OFFSET);
}

/* DW_ahb_dmac Component Parameters Register 1(DMA_COMP_PARAMS_1): other params */
static inline AL_U32 AlDmacAhb_ll_ReadDmaCompChOtherParams(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + DMAC_AHB_DMA_COMP_PARAMS_1_UPPER_OFFSET);
}

/* DMA Component Type Number = 0x44571110(DmaCompType) */
static inline AL_U32 AlDmacAhb_ll_ReadDmaCompType(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + DMAC_AHB_DMACOMPSID_LOWER_OFFSET);
}

/* DMA Component Version(DmaCompVer) */
static inline AL_U32 AlDmacAhb_ll_ReadDmaCompVer(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + DMAC_AHB_DMACOMPSID_UPPER_OFFSET);
}

#ifdef __cplusplus
}
#endif

#endif
