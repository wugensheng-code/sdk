#ifndef AL_QSPI_LL_H
#define AL_QSPI_LL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "al_qspi_hw.h"
#include "stdio.h"
#include "al_core.h"

typedef enum
{
    QSPI_MASTER_MODE      = 0,
    QSPI_SLAVE_MODE       = 1
} AL_QSPI_Mode;

typedef enum
{
    QSPI_TX_RX           = 0,
    QSPI_TX_ONLY         = 1,
    QSPI_RX_ONLY         = 2,
    QSPI_EEPROM          = 3
} AL_QSPI_TransferMode;

typedef enum
{
    QSPI_MOTOROLA_QSPI         = 0,   /* Motorola QSPI Frame Format */
    QSPI_TEXAS_SSP            = 1,   /* Texas Instruments SSP Frame */
    QSPI_NS_MICROWIRE         = 2    /* National Microwire Format */
} AL_QSPI_ProtFormat;

typedef enum
{
    QSPI_CLK_MODE0       = 0,    /* CPOL = 0, CPHA = 0 */
    QSPI_CLK_MODE1       = 1,    /* CPOL = 0, CPHA = 1 */
    QSPI_CLK_MODE2       = 2,    /* CPOL = 1, CPHA = 0 */
    QSPI_CLK_MODE3       = 3     /* CPOL = 1, CPHA = 1 */
} AL_QSPI_ClockEnum;

typedef enum
{
    QSPI_FRAME_4BITS     = 0x3,
    QSPI_FRAME_5BITS     = 0x4,
    QSPI_FRAME_6BITS     = 0x5,
    QSPI_FRAME_7BITS     = 0x6,
    QSPI_FRAME_8BITS     = 0x7,
    QSPI_FRAME_9BITS     = 0x8,
    QSPI_FRAME_10BITS     = 0x9,
    QSPI_FRAME_11BITS     = 0xa,
    QSPI_FRAME_12BITS     = 0xb,
    QSPI_FRAME_13BITS     = 0xc,
    QSPI_FRAME_14BITS     = 0xd,
    QSPI_FRAME_15BITS     = 0xe,
    QSPI_FRAME_16BITS     = 0xf,
    QSPI_FRAME_17BITS     = 0x10,
    QSPI_FRAME_18BITS     = 0x11,
    QSPI_FRAME_19BITS     = 0x12,
    QSPI_FRAME_20BITS     = 0x13,
    QSPI_FRAME_21BITS     = 0x14,
    QSPI_FRAME_22BITS     = 0x15,
    QSPI_FRAME_23BITS     = 0x16,
    QSPI_FRAME_24BITS     = 0x17,
    QSPI_FRAME_25BITS     = 0x18,
    QSPI_FRAME_26BITS     = 0x19,
    QSPI_FRAME_27BITS     = 0x1a,
    QSPI_FRAME_28BITS     = 0x1b,
    QSPI_FRAME_29BITS     = 0x1c,
    QSPI_FRAME_30BITS     = 0x1d,
    QSPI_FRAME_31BITS     = 0x1e,
    QSPI_FRAME_32BITS     = 0x1f
} AL_QSPI_DataFrameSize;

typedef enum
{
    QSPI_SLV_TOGGLE_DISABLE   = 0,
    QSPI_SLV_TOGGLE_ENABLE    = 1
} AL_QSPI_SlvSelToggleEnum;


typedef enum
{
    QSPI_SER_SS0_EN      = (1 << 0),
    QSPI_SER_SS1_EN      = (1 << 1),
    QSPI_SER_SS2_EN      = (1 << 2)
} AL_QSPI_SlvSelEnum;

/**
 * @brief  Run mode enum
 */
typedef enum
{
    AL_QSPI_RUN_INTR,
    AL_QSPI_RUN_DMA
} AL_QSPI_RunModeEnum;

typedef enum
{
    QSPI_SR_TXFIFO_NOTEMPTY   = 0,
    QSPI_SR_TXFIFO_EMPTY      = 1
} AL_QSPI_TxFifoEmptyEnum;

typedef enum
{
    QSPI_SR_TXFIFO_FULL      = 0,
    QSPI_SR_TXFIFO_NOTFULL   = 1
} AL_QSPI_TxFifoFullEnum;

typedef enum
{
    QSPI_SR_IDLE     = 0,
    QSPI_SR_BUSY     = 1
} AL_QSPI_BusyEnum;

typedef enum
{
    QSPI_SR_RXFIFO_EMPTY      = 0,
    QSPI_SR_RXFIFO_NOTEMPTY   = 1
} AL_QSPI_RxFifoEmptyEnum;

typedef enum
{
    QSPI_SR_RXFIFO_NOTFULL    = 0,
    QSPI_SR_RXFIFO_FULL       = 1
} AL_QSPI_RxFifoFullEnum;

typedef enum
{
    AL_QSPI_SEND_DONE                = (0x01),
    AL_QSPI_RECEIVE_DONE             = (0x01 << 1),
    AL_QSPI_SEND_TIMEOUT             = (0x01 << 2),
    AL_QSPI_RECEIVE_TIMEOUT          = (0x01 << 3)
} AL_QSPI_EventIdEnum;

typedef struct
{
    AL_QSPI_EventIdEnum      Event;
    AL_U32                  EventData;
} AL_QSPI_EventStruct;

/**
 * @brief  Configs Struct
 */
typedef struct
{
    AL_QSPI_Mode             Mode;
    AL_QSPI_TransferMode     TransMode;
    AL_QSPI_ProtFormat       ProtFormat;
    AL_QSPI_ClockEnum        ClockEnum;
    AL_QSPI_DataFrameSize    DataFrameSize;
    AL_U16                  ClkDiv;
    AL_QSPI_SlvSelToggleEnum SlvToggleEnum;
    AL_QSPI_SlvSelEnum       SlvSelEnum;
} AL_QSPI_ConfigsStruct;

typedef enum
{
    QSPI_TXEIS           = 0x1,      /* Transmit FIFO Empty Interrupt Status */
    QSPI_TXOIS           = 0x2,      /* Transmit FIFO Overflow Interrupt Status */
    QSPI_RXUIS           = 0x4,      /* Receive FIFO Underflow Interrupt Status */
    QSPI_RXOIS           = 0x8,      /* Receive FIFO Overflow Interrupt Status */
    QSPI_RXFIS           = 0x10,     /* Receive FIFO Full Interrupt Status */
} AL_QSPI_IntrStatusEnum;

typedef enum
{
    QSPI_TXEIM    = 1 << 0,   /* Transmit FIFO Empty Interrupt */
    QSPI_TXOIM    = 1 << 1,   /* Transmit FIFO Overflow Interrupt  */
    QSPI_RXUIM    = 1 << 2,   /* Receive FIFO Underflow Interrupt */
    QSPI_RXOIM    = 1 << 3,   /* Receive FIFO Overflow Interrupt */
    QSPI_RXFIM    = 1 << 4,   /* Receive FIFO Full Interrupt */
    QSPI_MSTIM    = 1 << 5    /* Multi-Master Contention Interrupt */
} AL_QSPI_IntrTypeEnum;


/**
 * This function set qspi master or slave.
 * @param   QspiBaseAddr is the qspi base addr
 * @param   QspiMode is qspi master or slave enmu
 * @return  AL_VOID
 * @note    none.
 */
static inline AL_VOID AlQspi_ll_SetQspiMode(AL_REG QspiBaseAddr, AL_QSPI_Mode QspiMode)
{
    if (QSPI_MASTER_MODE == QspiMode) {

    } else if (QSPI_SLAVE_MODE == QspiMode) {

    }
}

/**
 * This function disbale qspi.
 * @param   QspiBaseAddr is the qspi base addr
 * @return  AL_VOID
 * @note    Before configuring qspi,must disable the qspi.
 */
static inline AL_VOID AlQspi_ll_Disable(AL_REG QspiBaseAddr)
{
    AL_REG32_WRITE(QspiBaseAddr + QSPI_SSIENR_MST_OFFSET, 0);
}

/**
 * This function enable qspi.
 * @param   QspiBaseAddr is the qspi base addr
 * @return  AL_VOID
 * @note    none
 */
static inline AL_VOID AlQspi_ll_Enable(AL_REG QspiBaseAddr)
{
    AL_REG32_WRITE(QspiBaseAddr + QSPI_SSIENR_MST_OFFSET, 1);
}

/**
 * This function set qspi transfer mode.
 * @param   QspiBaseAddr is the qspi base addr
 * @param   QspiTransfMode is qspi transfer mode
 * @return  AL_VOID
 * @note    None
 */
static inline AL_VOID AlQspi_ll_SetTransfMode(AL_REG QspiBaseAddr, AL_QSPI_TransferMode QspiTransfMode)
{
    AL_REG32_SET_BITS(QspiBaseAddr + QSPI_CTRLR0_MST_OFFSET, QSPI_CTRLR0_MST_TMOD_SHIFT,
             QSPI_CTRLR0_MST_TMOD_SIZE, QspiTransfMode);
}

/**
 * This function set qspi protocol format.
 * @param   QspiBaseAddr is the qspi base addr
 * @param   QspiProtFormat is qspi protocol format
 * @return  AL_VOID
 * @note    none
 */
static inline AL_VOID AlQspi_ll_SetProtFormat(AL_REG QspiBaseAddr, AL_QSPI_ProtFormat QspiProtFormat)
{
    AL_REG32_SET_BITS(QspiBaseAddr + QSPI_CTRLR0_MST_OFFSET, QSPI_CTRLR0_MST_FRF_SHIFT,
             QSPI_CTRLR0_MST_FRF_SIZE, QspiProtFormat);
}

/**
 * This function set qspi clock polarity(CPOL) and clock phase (CPHA).
 * @param   QspiBaseAddr is the qspi base addr
 * @param   QspiClockEnum is qspi clock Enum
 * @return  AL_VOID
 * @note    Valid when the frame format is set to Motorola QSPI.
 */
static inline AL_VOID AlQspi_ll_SetCpolAndCpha(AL_REG QspiBaseAddr, AL_QSPI_ClockEnum QspiClockEnum)
{
    AL_REG32_SET_BITS(QspiBaseAddr + QSPI_CTRLR0_MST_OFFSET, QSPI_CTRLR0_MST_SCPH_SHIFT,
             QSPI_CTRLR0_MST_SCPH_SIZE, QspiClockEnum);
}

/**
 * This function set qspi data frame size.
 * @param   QspiBaseAddr is the qspi base addr
 * @param   QspiDataFrameSize is qspi data frame size
 * @return  AL_VOID
 * @note    none.
 */
static inline AL_VOID AlQspi_ll_SetDataFrameSize(AL_REG QspiBaseAddr, AL_QSPI_DataFrameSize QspiDataFrameSize)
{
    AL_REG32_SET_BITS(QspiBaseAddr + QSPI_CTRLR0_MST_OFFSET, QSPI_CTRLR0_MST_DFS_32_SHIFT,
             QSPI_CTRLR0_MST_DFS_32_SIZE, QspiDataFrameSize);
}

static inline AL_VOID AlQspi_ll_EnableEndianConver(AL_REG QspiBaseAddr)
{
    AL_REG32_SET_BITS(QspiBaseAddr + QSPI_CTRLR0_MST_OFFSET, QSPI_CTRLR0_MST_SECONV_SHIFT,
             QSPI_CTRLR0_MST_SECONV_SIZE, 1);
}

static inline AL_VOID AlQspi_ll_DisableEndianConver(AL_REG QspiBaseAddr)
{
    AL_REG32_SET_BITS(QspiBaseAddr + QSPI_CTRLR0_MST_OFFSET, QSPI_CTRLR0_MST_SECONV_SHIFT,
             QSPI_CTRLR0_MST_SECONV_SIZE, 0);
}

/**
 * This function set qspi clock divider.
 * @param   QspiBaseAddr is the qspi base addr
 * @param   QspiClkDiv is qspi clk divider
 * @return  AL_VOID
 * @note    The LSB for this field is always set to 0 and is unaffected by a
 * write operation, which ensures an even value is held in this register.
 * For Fssi_clk = 50MHz and SCKDV =2. Fsclk_out = 50/(2 << 1) = 12.5MHz
 */
static inline AL_VOID AlQspi_ll_SetClkDiv(AL_REG QspiBaseAddr, AL_U16 QspiClkDiv)
{
    AL_REG32_SET_BITS(QspiBaseAddr + QSPI_BAUDR_MST_OFFSET, QSPI_BAUDR_MST_SCKDV_SHIFT,
             QSPI_BAUDR_MST_SCKDV_SIZE, QspiClkDiv);
}

/**
 * This function set qspi clock divider.
 * @param   QspiBaseAddr is the qspi base addr
 * @param   QspiClkDiv is qspi clk divider
 * @return  AL_VOID
 * @note    The LSB for this field is always set to 0 and is unaffected by a
 * write operation, which ensures an even value is held in this register.
 * For Fssi_clk = 50MHz and SCKDV =2. Fsclk_out = 50/(2 << 1) = 12.5MHz
 */
static inline AL_VOID AlQspi_ll_SetSlvSelToggle(AL_REG QspiBaseAddr, AL_QSPI_SlvSelToggleEnum QspiSlvSelToggleEnum)
{
    AL_REG32_SET_BITS(QspiBaseAddr + QSPI_CTRLR0_MST_OFFSET, QSPI_CTRLR0_MST_SSTE_SHIFT,
             QSPI_CTRLR0_MST_SSTE_SIZE, QspiSlvSelToggleEnum);
}


/**
 * This function set qspi slave select.
 * @param   QspiBaseAddr is the qspi base addr
 * @param   QspiSlvSelEnum is qspi slave select
 * @return  AL_VOID
 * @note    none
 */
static inline AL_VOID AlQspi_ll_SetSlvSel(AL_REG QspiBaseAddr, AL_QSPI_SlvSelEnum QspiSlvSelEnum)
{
    AL_REG32_SET_BITS(QspiBaseAddr + QSPI_SER_MST_OFFSET, QSPI_SER_MST_SER_SHIFT,
             QSPI_SER_MST_SER_SIZE, QspiSlvSelEnum);
}

/**
 * This function set qspi transmit FIFO threshold level.
 * @param   QspiBaseAddr is the qspi base addr
 * @param   QspiTxFifoThrLevel is transmit FIFO threshold level
 * @return  AL_VOID
 * @note    none.
 */
static inline AL_VOID AlQspi_ll_SetTxFifoThrLevel(AL_REG QspiBaseAddr, AL_U8 QspiTxFifoThrLevel)
{
    AL_REG32_SET_BITS(QspiBaseAddr + QSPI_TXFTLR_MST_OFFSET, QSPI_TXFTLR_MST_TFT_SHIFT,
             QSPI_TXFTLR_MST_TFT_SIZE, QspiTxFifoThrLevel);
}

/**
 * This function set qspi receive FIFO threshold level.
 * @param   QspiBaseAddr is the qspi base addr
 * @param   QspiRxFifoThrLevel is receive FIFO threshold level
 * @return  AL_VOID
 * @note    none.
 */
static inline AL_VOID AlQspi_ll_SetRxFifoThrLevel(AL_REG QspiBaseAddr, AL_U8 QspiRxFifoThrLevel)
{
    AL_REG32_SET_BITS(QspiBaseAddr + QSPI_RXFTLR_MST_OFFSET, QSPI_RXFTLR_MST_RFT_SHIFT,
             QSPI_RXFTLR_MST_RFT_SIZE, QspiRxFifoThrLevel);
}

static inline AL_U32 AlQspi_ll_ReadTxFifoLevel(AL_REG QspiBaseAddr)
{
    return AL_REG32_READ(QspiBaseAddr + QSPI_TXFLR_MST_OFFSET);
}

static inline AL_U32 AlQspi_ll_ReadRxFifoLevel(AL_REG QspiBaseAddr)
{
    return AL_REG32_READ(QspiBaseAddr + QSPI_RXFLR_MST_OFFSET);
}

static inline AL_U32 AlQspi_ll_ReadRawIntrStatus(AL_REG QspiBaseAddr)
{
    return AL_REG32_READ(QspiBaseAddr + QSPI_RISR_MST_OFFSET);
}

/**
 * This function qspi transmit data.
 * @param   QspiBaseAddr is the qspi base addr
 * @param   Data is transmit data
 * @return  AL_VOID
 * @note    none.
 */
static inline AL_VOID AlQspi_ll_DataTransmit(AL_REG QspiBaseAddr, AL_U32 Data)
{
    AL_REG32_WRITE(QspiBaseAddr + QSPI_DR0_MST_OFFSET, Data);
}

/**
 * This function qspi receive data.
 * @param   QspiBaseAddr is the qspi base addr
 * @return  The data being read
 * @note    none.
 */
static inline AL_U32 AlQspi_ll_DataReceive(AL_REG QspiBaseAddr)
{
    return AL_REG32_READ(QspiBaseAddr + QSPI_DR0_MST_OFFSET);
}

/**
 * This function return whether qspi transmit FIFO empty.
 * @param   QspiBaseAddr is the qspi base addr
 * @return  The transmit fifo whether empty
 * @note    none.
 */
static inline AL_QSPI_TxFifoEmptyEnum AlQspi_ll_IsTxFifoEmpty(AL_REG QspiBaseAddr)
{
    return  AL_REG32_GET_BITS(QspiBaseAddr + QSPI_SR_MST_OFFSET, QSPI_SR_MST_TFE_SHIFT, QSPI_SR_MST_TFE_SIZE);
}

/**
 * This function return whether qspi transmit FIFO full.
 * @param   QspiBaseAddr is the qspi base addr
 * @return  The transmit fifo whether full
 * @note    none.
 */
static inline AL_QSPI_TxFifoFullEnum AlQspi_ll_IsTxFifoFull(AL_REG QspiBaseAddr)
{
    return  AL_REG32_GET_BITS(QspiBaseAddr + QSPI_SR_MST_OFFSET, QSPI_SR_MST_TFNF_SHIFT, QSPI_SR_MST_TFNF_SIZE);
}

/**
 * This function return qspi whether busy.
 * @param   QspiBaseAddr is the qspi base addr
 * @return  The qspi whether busy
 * @note    none.
 */
static inline AL_QSPI_BusyEnum AlQspi_ll_IsBusy(AL_REG QspiBaseAddr)
{
    return  AL_REG32_GET_BITS(QspiBaseAddr + QSPI_SR_MST_OFFSET, QSPI_SR_MST_BUSY_SHIFT, QSPI_SR_MST_BUSY_SIZE);
}

/**
 * This function return whether qspi receive FIFO empty.
 * @param   QspiBaseAddr is the qspi base addr
 * @return  The receive fifo whether empty
 * @note    none.
 */
static inline AL_QSPI_RxFifoEmptyEnum AlQspi_ll_IsRxFifoEmpty(AL_REG QspiBaseAddr)
{
    return  AL_REG32_GET_BITS(QspiBaseAddr + QSPI_SR_MST_OFFSET, QSPI_SR_MST_RFNE_SHIFT, QSPI_SR_MST_RFNE_SIZE);
}

/**
 * This function return whether qspi receive FIFO full.
 * @param   QspiBaseAddr is the qspi base addr
 * @param   QspiDataFramesNum is Number of Data Frames
 * @return  AL_VOID
 * @note    this register serves no purpose and is not present when the DW_apb_ssi is configured as a serial slave.
 */
static inline AL_QSPI_RxFifoFullEnum AlQspi_ll_IsRxFifoFull(AL_REG QspiBaseAddr)
{
    return  AL_REG32_GET_BITS(QspiBaseAddr + QSPI_SR_MST_OFFSET, QSPI_SR_MST_RFF_SHIFT, QSPI_SR_MST_RFF_SIZE);
}

static inline AL_VOID AlQspi_ll_SetRecvNumOfDataFrames(AL_REG QspiBaseAddr, AL_U16 QspiRecvDataFramesNum)
{
     AL_REG32_SET_BITS(QspiBaseAddr + QSPI_CTRLR1_MST_OFFSET, QSPI_CTRLR1_MST_NDF_SHIFT,
                     QSPI_CTRLR1_MST_NDF_SIZE, QspiRecvDataFramesNum);
}

static inline AL_VOID AlQspi_ll_MaskAllIntr(AL_REG QspiBaseAddr)
{
    AL_REG32_WRITE(QspiBaseAddr + QSPI_IMR_MST_OFFSET,  0);
}

static inline AL_VOID AlQspi_ll_MaskIntr(AL_REG QspiBaseAddr, AL_QSPI_IntrTypeEnum QspiMaskIntr)
{
    AL_REG32_WRITE(QspiBaseAddr + QSPI_IMR_MST_OFFSET, AL_REG32_READ(QspiBaseAddr + QSPI_IMR_MST_OFFSET) & (~QspiMaskIntr));
}

static inline AL_VOID AlQspi_ll_EnableIntr(AL_REG QspiBaseAddr, AL_QSPI_IntrTypeEnum QspiEnableIntr)
{
    AL_REG32_WRITE(QspiBaseAddr + QSPI_IMR_MST_OFFSET, AL_REG32_READ(QspiBaseAddr + QSPI_IMR_MST_OFFSET) | QspiEnableIntr);
}

static inline AL_U32 AlQspi_ll_GetIntrStatus(AL_REG QspiBaseAddr)
{
    return AL_REG32_READ(QspiBaseAddr + QSPI_ISR_MST_OFFSET);
}

static inline AL_VOID AlQspi_ll_ClearAllIntr(AL_REG BaseAddr)
{
    AL_REG32_READ(BaseAddr + QSPI_ICR_MST_OFFSET);
}

static inline AL_VOID AlQspi_ll_ClearRxuicrIntr(AL_REG BaseAddr)
{
    AL_REG32_READ(BaseAddr + QSPI_RXUICR_MST_OFFSET);
}

static inline AL_VOID AlQspi_ll_ClearRxoicrIntr(AL_REG BaseAddr)
{
    AL_REG32_READ(BaseAddr + QSPI_RXOICR_MST_OFFSET);
}

static inline AL_VOID AlQspi_ll_ClearTxoicrIntr(AL_REG BaseAddr)
{
    AL_REG32_READ(BaseAddr + QSPI_TXOICR_MST_OFFSET);
}

static inline AL_VOID AlQspi_ll_TxDmaEnable(AL_REG BaseAddr)
{
    AL_REG32_SET_BIT(BaseAddr + QSPI_DMACR_MST_OFFSET, QSPI_DMACR_MST_RDMAE_SHIFT, AL_FUNC_ENABLE);
}

static inline AL_VOID AlQspi_ll_RxDmaEnable(AL_REG BaseAddr)
{
    AL_REG32_SET_BIT(BaseAddr + QSPI_DMACR_MST_OFFSET, QSPI_DMACR_MST_RDMAE_SHIFT, AL_FUNC_ENABLE);
}

static inline AL_VOID AlQspi_ll_DmaDisable(AL_REG BaseAddr)
{
    AL_REG32_WRITE(BaseAddr + QSPI_DMACR_MST_OFFSET, AL_FUNC_DISABLE);
}

static inline AL_VOID AlQspi_ll_SetDmaTransLevel(AL_REG BaseAddr, AL_U8 Level)
{
    AL_REG32_WRITE(BaseAddr + QSPI_DMATDLR_MST_OFFSET, Level);
}

static inline AL_VOID AlQspi_ll_SetDmaRecevLevel(AL_REG BaseAddr, AL_U8 Level)
{
    AL_REG32_WRITE(BaseAddr + QSPI_DMARDLR_MST_OFFSET, Level);
}

#ifdef __cplusplus
}
#endif

#endif
