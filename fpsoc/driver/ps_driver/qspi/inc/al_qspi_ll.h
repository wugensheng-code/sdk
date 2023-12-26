/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_QSPI_LL_H_
#define __AL_QSPI_LL_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files ********************************/
#include "al_qspi_hw.h"
#include "al_core.h"

/************************** Constant Definitions ****************************/
/**
 * @brief Qspi format enum
 */
typedef enum
{
    SPI_STANDARD_FORMAT = 0,
    SPI_DUAL_FORMAT     = 1,
    SPI_QUAD_FORMAT     = 2
} AL_QSPI_FrfEnum;

/**
 * @brief Slave Select Toggle Enable enum
 */
typedef enum
{
    QSPI_TOGGLE_DISABLE  = 0,
    QSPI_TOGGLE_EN       = 1
} AL_QSPI_SstEnableEnum;

/**
 * @brief Qspi loopback or normal mode enum
 */
typedef enum
{
    QSPI_NORMAL_MODE    = 0,
    QSPI_TEST_MODE      = 1
} AL_QSPI_LoopBackTestEnum;

/**
 * @brief Qspi slave output enable enum
 */
typedef enum
{
    QSPI_SLV_OE_ENABLE  = 0,
    QSPI_SLV_OE_DISABLE = 1
} AL_QSPI_SlvOutEnableEnum;

/**
 * @brief Qspi transfer mode enum
 */
typedef enum
{
    QSPI_TX_RX           = 0,
    QSPI_TX_ONLY         = 1,
    QSPI_RX_ONLY         = 2,
    QSPI_EEPROM          = 3
} AL_QSPI_TransferMode;

/**
 * @brief Qspi dfs(data frame size) enum
 */
typedef enum
{
    QSPI_DFS_4BITS     = 0x3,
    QSPI_DFS_5BITS     = 0x4,
    QSPI_DFS_6BITS     = 0x5,
    QSPI_DFS_7BITS     = 0x6,
    QSPI_DFS_8BITS     = 0x7,
    QSPI_DFS_9BITS     = 0x8,
    QSPI_DFS_10BITS     = 0x9,
    QSPI_DFS_11BITS     = 0xa,
    QSPI_DFS_12BITS     = 0xb,
    QSPI_DFS_13BITS     = 0xc,
    QSPI_DFS_14BITS     = 0xd,
    QSPI_DFS_15BITS     = 0xe,
    QSPI_DFS_16BITS     = 0xf,
    QSPI_DFS_17BITS     = 0x10,
    QSPI_DFS_18BITS     = 0x11,
    QSPI_DFS_19BITS     = 0x12,
    QSPI_DFS_20BITS     = 0x13,
    QSPI_DFS_21BITS     = 0x14,
    QSPI_DFS_22BITS     = 0x15,
    QSPI_DFS_23BITS     = 0x16,
    QSPI_DFS_24BITS     = 0x17,
    QSPI_DFS_25BITS     = 0x18,
    QSPI_DFS_26BITS     = 0x19,
    QSPI_DFS_27BITS     = 0x1a,
    QSPI_DFS_28BITS     = 0x1b,
    QSPI_DFS_29BITS     = 0x1c,
    QSPI_DFS_30BITS     = 0x1d,
    QSPI_DFS_31BITS     = 0x1e,
    QSPI_DFS_32BITS     = 0x1f
} AL_QSPI_DataFrameSize;

/**
 * @brief Qspi slave select enum
 */
typedef enum
{
    QSPI_SER_SS_DIS      = 0,
    QSPI_SER_SS0_EN      = (1 << 0),
    QSPI_SER_SS1_EN      = (1 << 1),
    QSPI_SER_SS2_EN      = (1 << 2)
} AL_QSPI_SlvSelEnum;

/**
 * @brief Qspi slave select toggle enum
 */
typedef enum
{
    QSPI_SLV_TOGGLE_DISABLE   = 0,
    QSPI_SLV_TOGGLE_ENABLE    = 1
} AL_QSPI_SlvSelToggleEnum;

/**
 * @brief Qspi tx fifo empty enum
 */
typedef enum
{
    QSPI_SR_TXFIFO_NOTEMPTY   = 0,
    QSPI_SR_TXFIFO_EMPTY      = 1
} AL_QSPI_TxFifoEmptyEnum;

/**
 * @brief Qspi tx fifo full enum
 */
typedef enum
{
    QSPI_SR_TXFIFO_FULL      = 0,
    QSPI_SR_TXFIFO_NOTFULL   = 1
} AL_QSPI_TxFifoFullEnum;

/**
 * @brief Qspi busy enum
 */
typedef enum
{
    QSPI_SR_IDLE     = 0,
    QSPI_SR_BUSY     = 1
} AL_QSPI_BusyEnum;

/**
 * @brief Qspi rx fifo empty enum
 */
typedef enum
{
    QSPI_SR_RXFIFO_EMPTY      = 0,
    QSPI_SR_RXFIFO_NOTEMPTY   = 1
} AL_QSPI_RxFifoEmptyEnum;

/**
 * @brief Qspi rx fifo full enum
 */
typedef enum
{
    QSPI_SR_RXFIFO_NOTFULL    = 0,
    QSPI_SR_RXFIFO_FULL       = 1
} AL_QSPI_RxFifoFullEnum;

/**
 * @brief Qspi status reg enum
 */
typedef enum
{
    QSPI_BUSY = 0x1,      /* SSI Busy Flag */
    QSPI_TFNF = 0x2,      /* Transmit FIFO Not Full */
    QSPI_TFE  = 0x4,      /* Transmit FIFO Empty */
    QSPI_RFNE = 0x8,      /* Receive FIFO Not Empty */
    QSPI_RFF  = 0x10,     /* Receive FIFO Full */
    QSPI_TXE  = 0x20,     /* Transmission Error */
    QSPI_DCOL = 0x40      /* Data Collision Error */
} AL_QSPI_SrEnum;

/**
 * @brief Qspi interrupt type enum
 */
typedef enum
{
    QSPI_TXEIM    = 1 << 0,   /* Transmit FIFO Empty Interrupt */
    QSPI_TXOIM    = 1 << 1,   /* Transmit FIFO Overflow Interrupt  */
    QSPI_RXUIM    = 1 << 2,   /* Receive FIFO Underflow Interrupt */
    QSPI_RXOIM    = 1 << 3,   /* Receive FIFO Overflow Interrupt */
    QSPI_RXFIM    = 1 << 4,   /* Receive FIFO Full Interrupt */
    QSPI_XRXOIM   = 1 << 6,   /* XIP Receive FIFO Overflow Interrupt Mask */
} AL_QSPI_IntrTypeEnum;

/**
 * @brief Qspi interrupt status enum
 */
typedef enum
{
    QSPI_TXEIS           = 0x1,      /* Transmit FIFO Empty Interrupt Status */
    QSPI_TXOIS           = 0x2,      /* Transmit FIFO Overflow Interrupt Status */
    QSPI_RXUIS           = 0x4,      /* Receive FIFO Underflow Interrupt Status */
    QSPI_RXOIS           = 0x8,      /* Receive FIFO Overflow Interrupt Status */
    QSPI_RXFIS           = 0x10,     /* Receive FIFO Full Interrupt Status */
    QSPI_XRXOIS          = 0x40,     /* XIP Receive FIFO Overflow Interrupt Status */
} AL_QSPI_IntrStatusEnum;

/**
 * @brief Qspi edge sampling enum
 */
typedef enum
{
    QSPI_PostiveEdgeSampling = 0,
    QSPI_NegativeEdgeSampling = 1
} AL_QSPI_RecvDataSamplEdgeEnum;

/**
 * @brief Qspi clock stretch enum
 */
typedef enum
{
    QSPI_DisableClockStretch = 0,
    QSPI_EnableClockStretch  = 1
} AL_QSPI_ClockStretchEnum;

/**
 * @brief Qspi xip prefetch enable enum
 */
typedef enum
{
    QSPI_DisableXipPrefetch = 0,
    QSPI_EnableXipPrefetch  = 1
} AL_QSPI_XipPrefetchEnableEnum;

/**
 * @brief Qspi xip mode bits length enum
 */
typedef enum
{
    QSPI_MBL_2 = 0,
    QSPI_MBL_4 = 1,
    QSPI_MBL_8 = 2,
    QSPI_MBL_16 = 3,
} AL_QSPI_XipModeBitsLengthEnum;

/**
 * @brief Qspi xip continuous transfer enable enum
 */
typedef enum
{
    QSPI_DisableXipContTrans = 0,
    QSPI_EnableXipContTrans  = 1
} AL_QSPI_XipContTransEnableEnum;

/**
 * @brief Qspi xip instruct phase enable enum
 */
typedef enum
{
    QSPI_XipInstPhaseDisable = 0,
    QSPI_XipInstPhaseEnable  = 1
} AL_QSPI_XipInstPhaseEnableEnum;

/**
 * @brief Qspi xip dfs config enum
 */
typedef enum
{
    QSPI_XipDfsChange = 0,
    QSPI_XipDfsFix    = 1
} AL_QSPI_XipDfsFixEnum;

/**
 * @brief Qspi instruct phase ddr mode enable enum
 */
typedef enum
{
    QSPI_InstDdrDisable = 0,
    QSPI_InstDdrEnable  = 1
} AL_QSPI_InstDdrEnableEnum;

/**
 * @brief Qspi ddr mode enable enum
 */
typedef enum
{
    QSPI_SpiDdrDisable = 0,
    QSPI_SpiDdrEnable  = 1
} AL_QSPI_SpiDdrEnableEnum;

/**
 * @brief Qspi instruct length enum
 */
typedef enum
{
    QSPI_INST_L0 = 0,
    QSPI_INST_L4 = 1,
    QSPI_INST_L8 = 2,
    QSPI_INST_L16 = 3
} AL_QSPI_InstLengthEnum;

/**
 * @brief Qspi mode bit enable enum
 */
typedef enum
{
    QSPI_XipModeBitDisable = 0,
    QSPI_XipModeBitEnable  = 1
} AL_QSPI_XipModeBitEnableEnum;

/**
 * @brief Qspi xip port1 norflash size enum
 */
typedef enum
{
    QSPI_XipPort1NorFlash_1MB = 7,
    QSPI_XipPort1NorFlash_2MB = 6,
    QSPI_XipPort1NorFlash_4MB = 5,
    QSPI_XipPort1NorFlash_8MB = 4,
    QSPI_XipPort1NorFlash_16MB = 3,
    QSPI_XipPort1NorFlash_32MB = 2,
    QSPI_XipPort1NorFlash_64MB = 1,
    QSPI_XipPort1NorFlash_128MB = 0
} AL_QSPI_XipPort1NorFlashSize;

/**
 * @brief Qspi addr length enum
 */
typedef enum
{
    QSPI_ADDR_L0 = 0x0,
    QSPI_ADDR_L4 = 0x1,
    QSPI_ADDR_L8 = 0x2,
    QSPI_ADDR_L12 = 0x3,
    QSPI_ADDR_L16 = 0x4,
    QSPI_ADDR_L20 = 0x5,
    QSPI_ADDR_L24 = 0x6,
    QSPI_ADDR_L28 = 0x7,
    QSPI_ADDR_L32 = 0x8,
    QSPI_ADDR_L36 = 0x9,
    QSPI_ADDR_L40 = 0xa,
    QSPI_ADDR_L44 = 0xb,
    QSPI_ADDR_L48 = 0xc,
    QSPI_ADDR_L52 = 0xd,
    QSPI_ADDR_L56 = 0xe,
    QSPI_ADDR_L60 = 0xf
} AL_QSPI_AddrLengthEnum;

/**
 * @brief Qspi transfer type enum
 */
typedef enum
{
    QSPI_TT0 = 0,   /* Instruction and Address will be sent in Standard SPI Mode */
    QSPI_TT1 = 1,   /* Instruction will be sent in Standard SPI Mode and Address will
                     be sent in the mode specified by CTRLR0.SPI_FRF */
    QSPI_TT2 = 2    /* Both Instruction and Address will be sent in the mode specified by SPI_FRF */
} AL_QSPI_TransType;

/**
 * @brief  Run mode enum
 */
typedef enum
{
    AL_QSPI_USE_INTR,
    AL_QSPI_USE_DMA
} AL_QSPI_IsDmaEnum;

/**
 * @brief Qspi event id enum
 */
typedef enum
{
    AL_QSPI_SEND_DONE                = BIT(0),
    AL_QSPI_RECEIVE_DONE             = BIT(1),
    AL_QSPI_SEND_TIMEOUT             = BIT(2),
    AL_QSPI_RECEIVE_TIMEOUT          = BIT(3),
    AL_QSPI_TX_FO                    = BIT(4),
    AL_QSPI_RX_FO                    = BIT(5),
    AL_QSPI_RX_FU                    = BIT(6),
    AL_QSPI_TX_FIFO_EMPTY            = BIT(7),
    AL_QSPI_RX_FIFO_FULL             = BIT(8),
} AL_QSPI_EventIdEnum;

/**************************** Type Definitions ******************************/
/**
 * @brief Qspi Event Struct
 */
typedef struct
{
    AL_QSPI_EventIdEnum     Events;
    AL_U32                  EventData;
} AL_QSPI_EventStruct;

/**
 * @brief Enhanced Qspi Configs Struct
 */
typedef struct
{
    AL_QSPI_AddrLengthEnum      AddrLength;
    AL_QSPI_InstLengthEnum      InstLength;
    AL_QSPI_TransType           TransType;
    AL_U16                      WaitCycles;
} AL_QSPI_EnSpiCfgStruct;

/**
 * @brief Qspi Xip Config Struct
 */
typedef struct
{
    AL_QSPI_XipPrefetchEnableEnum   PrefetchEnable;
    AL_QSPI_XipModeBitEnableEnum    ModeBitEnable;
    AL_QSPI_XipModeBitsLengthEnum   ModeBitsLength;
    AL_QSPI_XipContTransEnableEnum  ContTransEnable;
    AL_QSPI_XipInstPhaseEnableEnum  InstPhaseEnable;
    AL_QSPI_XipDfsFixEnum           DfsFix;
    AL_QSPI_XipPort1NorFlashSize    Nor1FlashSize;
} AL_QSPI_XipCfgStruct;

/**
 * @brief Qspi Transfer Struct
 */
typedef struct
{
    AL_QSPI_TransferMode        TransMode;
    AL_QSPI_DataFrameSize       DataFrameSize;
    AL_QSPI_SlvSelEnum          SlvSelEnum;
    AL_QSPI_EnSpiCfgStruct      EnSpiCfg;
} AL_QSPI_TransStruct;

/**
 * @brief Configs Struct
 */
typedef struct
{
    AL_QSPI_FrfEnum             SpiFrameFormat;
    AL_U16                      ClkDiv;
    AL_U16                      SamplDelay;
    AL_QSPI_SlvSelToggleEnum    SlvToggleEnum;
    AL_QSPI_ClockStretchEnum    ClockStretch;  /* clock stretch is useful in SPI_QUAD_FORMAT and SPI_QUAD_FORMAT */
    AL_QSPI_XipCfgStruct        XipCfg;
    AL_QSPI_TransStruct         Trans;
} AL_QSPI_ConfigsStruct;

/***************** Macros (Inline Functions) Definitions ********************/

/******************************** Variable Prototypes ******************************/

/******************************** Function Prototypes ******************************/
/**
 * This function disbale qspi.
 * @param   QspiBaseAddr is the qspi base addr
 * @return  AL_VOID
 * @note    Before configuring qspi,must disable the qspi.
 */
static inline AL_VOID AlQspi_ll_Disable(AL_REG QspiBaseAddr)
{
    AL_REG32_WRITE(QspiBaseAddr + QSPI_SSIENR_OFFSET, 0);
}

/**
 * This function enable qspi.
 * @param   QspiBaseAddr is the qspi base addr
 * @return  AL_VOID
 * @note    none
 */
static inline AL_VOID AlQspi_ll_Enable(AL_REG QspiBaseAddr)
{
    AL_REG32_WRITE(QspiBaseAddr + QSPI_SSIENR_OFFSET, 1);
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
    AL_REG32_SET_BITS(QspiBaseAddr + QSPI_CTRLR0_OFFSET, QSPI_CTRLR0_TMOD_SHIFT,
             QSPI_CTRLR0_TMOD_SIZE, QspiTransfMode);
}


static inline AL_VOID AlQspi_ll_SetQspiFrameFormat(AL_REG QspiBaseAddr, AL_QSPI_FrfEnum QspiFrameFormat)
{
    AL_REG32_SET_BITS(QspiBaseAddr + QSPI_CTRLR0_OFFSET, QSPI_CTRLR0_SPI_FRF_SHIFT,
             QSPI_CTRLR0_SPI_FRF_SIZE, QspiFrameFormat);
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
    AL_REG32_SET_BITS(QspiBaseAddr + QSPI_CTRLR0_OFFSET, QSPI_CTRLR0_DFS_SHIFT,
             QSPI_CTRLR0_DFS_SIZE, QspiDataFrameSize);
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
    AL_REG32_SET_BITS(QspiBaseAddr + QSPI_BAUDR_OFFSET, QSPI_BAUDR_SCKDV_SHIFT,
             QSPI_BAUDR_SCKDV_SIZE, QspiClkDiv);
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
    AL_REG32_SET_BITS(QspiBaseAddr + QSPI_CTRLR0_OFFSET, QSPI_CTRLR0_SSTE_SHIFT,
             QSPI_CTRLR0_SSTE_SIZE, QspiSlvSelToggleEnum);
}

static inline AL_VOID AlQspi_ll_SetLoopBack(AL_REG QspiBaseAddr, AL_QSPI_LoopBackTestEnum LoopBack)
{
    AL_REG32_SET_BITS(QspiBaseAddr + QSPI_CTRLR0_OFFSET, QSPI_CTRLR0_SRL_SHIFT,
             QSPI_CTRLR0_SRL_SIZE, LoopBack);
}

static inline AL_VOID AlQspi_ll_SetSlvOutEnable(AL_REG QspiBaseAddr, AL_QSPI_SlvOutEnableEnum SlvOutEnable)
{
    AL_REG32_SET_BITS(QspiBaseAddr + QSPI_CTRLR0_OFFSET, QSPI_CTRLR0_SLV_OE_SHIFT,
             QSPI_CTRLR0_SLV_OE_SIZE, SlvOutEnable);
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
    AL_REG32_SET_BITS(QspiBaseAddr + QSPI_SER_OFFSET, QSPI_SER_SER_SHIFT,
             QSPI_SER_SER_SIZE, QspiSlvSelEnum);
}


static inline AL_VOID AlQspi_ll_SetTxStartFifoLevel(AL_REG QspiBaseAddr, AL_U8 QspiTxStartFifoLevel)
{
    AL_REG32_SET_BITS(QspiBaseAddr + QSPI_TXFTLR_OFFSET, QSPI_TXFTLR_TXFTHR_SHIFT,
             QSPI_TXFTLR_TXFTHR_SIZE, QspiTxStartFifoLevel);
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
    AL_REG32_SET_BITS(QspiBaseAddr + QSPI_TXFTLR_OFFSET, QSPI_TXFTLR_TFT_SHIFT,
             QSPI_TXFTLR_TFT_SIZE, QspiTxFifoThrLevel);
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
    AL_REG32_SET_BITS(QspiBaseAddr + QSPI_RXFTLR_OFFSET, QSPI_RXFTLR_RFT_SHIFT,
             QSPI_RXFTLR_RFT_SIZE, QspiRxFifoThrLevel);
}

static inline AL_U32 AlQspi_ll_ReadTxFifoLevel(AL_REG QspiBaseAddr)
{
    return AL_REG32_READ(QspiBaseAddr + QSPI_TXFLR_OFFSET);
}

static inline AL_U32 AlQspi_ll_ReadRxFifoLevel(AL_REG QspiBaseAddr)
{
    return AL_REG32_READ(QspiBaseAddr + QSPI_RXFLR_OFFSET);
}

static inline AL_U32 AlQspi_ll_ReadRawIntrStatus(AL_REG QspiBaseAddr)
{
    return AL_REG32_READ(QspiBaseAddr + QSPI_RISR_OFFSET);
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
    AL_REG32_WRITE(QspiBaseAddr + QSPI_DR0_OFFSET, Data);
}

/**
 * This function qspi receive data.
 * @param   QspiBaseAddr is the qspi base addr
 * @return  The data being read
 * @note    none.
 */
static inline AL_U32 AlQspi_ll_DataReceive(AL_REG QspiBaseAddr)
{
    return AL_REG32_READ(QspiBaseAddr + QSPI_DR0_OFFSET);
}

/**
 * This function return whether qspi transmit FIFO empty.
 * @param   QspiBaseAddr is the qspi base addr
 * @return  The transmit fifo whether empty
 * @note    none.
 */
static inline AL_QSPI_TxFifoEmptyEnum AlQspi_ll_IsTxFifoEmpty(AL_REG QspiBaseAddr)
{
    return  AL_REG32_GET_BITS(QspiBaseAddr + QSPI_SR_OFFSET, QSPI_SR_TFE_SHIFT, QSPI_SR_TFE_SIZE);
}

/**
 * This function return whether qspi transmit FIFO full.
 * @param   QspiBaseAddr is the qspi base addr
 * @return  The transmit fifo whether full
 * @note    none.
 */
static inline AL_QSPI_TxFifoFullEnum AlQspi_ll_IsTxFifoFull(AL_REG QspiBaseAddr)
{
    return  AL_REG32_GET_BITS(QspiBaseAddr + QSPI_SR_OFFSET, QSPI_SR_TFNF_SHIFT, QSPI_SR_TFNF_SIZE);
}

/**
 * This function return qspi whether busy.
 * @param   QspiBaseAddr is the qspi base addr
 * @return  The qspi whether busy
 * @note    none.
 */
static inline AL_QSPI_BusyEnum AlQspi_ll_IsBusy(AL_REG QspiBaseAddr)
{
    return  AL_REG32_GET_BITS(QspiBaseAddr + QSPI_SR_OFFSET, QSPI_SR_BUSY_SHIFT, QSPI_SR_BUSY_SIZE);
}

/**
 * This function return whether qspi receive FIFO empty.
 * @param   QspiBaseAddr is the qspi base addr
 * @return  The receive fifo whether empty
 * @note    none.
 */
static inline AL_QSPI_RxFifoEmptyEnum AlQspi_ll_IsRxFifoEmpty(AL_REG QspiBaseAddr)
{
    return  AL_REG32_GET_BITS(QspiBaseAddr + QSPI_SR_OFFSET, QSPI_SR_RFNE_SHIFT, QSPI_SR_RFNE_SIZE);
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
    return  AL_REG32_GET_BITS(QspiBaseAddr + QSPI_SR_OFFSET, QSPI_SR_RFF_SHIFT, QSPI_SR_RFF_SIZE);
}

static inline AL_VOID AlQspi_ll_SetRecvNumOfDataFrames(AL_REG QspiBaseAddr, AL_U16 QspiRecvDataFramesNum)
{
     AL_REG32_SET_BITS(QspiBaseAddr + QSPI_CTRLR1_OFFSET, QSPI_CTRLR1_NDF_SHIFT,
                     QSPI_CTRLR1_NDF_SIZE, QspiRecvDataFramesNum);
}

static inline AL_VOID AlQspi_ll_MaskAllIntr(AL_REG QspiBaseAddr)
{
    AL_REG32_WRITE(QspiBaseAddr + QSPI_IMR_OFFSET,  0);
}

static inline AL_VOID AlQspi_ll_MaskIntr(AL_REG QspiBaseAddr, AL_QSPI_IntrTypeEnum QspiMaskIntr)
{
    AL_REG32_WRITE(QspiBaseAddr + QSPI_IMR_OFFSET, AL_REG32_READ(QspiBaseAddr + QSPI_IMR_OFFSET) & (~QspiMaskIntr));
}

static inline AL_VOID AlQspi_ll_EnableIntr(AL_REG QspiBaseAddr, AL_QSPI_IntrTypeEnum QspiEnableIntr)
{
    AL_REG32_WRITE(QspiBaseAddr + QSPI_IMR_OFFSET, AL_REG32_READ(QspiBaseAddr + QSPI_IMR_OFFSET) | QspiEnableIntr);
}

static inline AL_U32 AlQspi_ll_GetIntrStatus(AL_REG QspiBaseAddr)
{
    return AL_REG32_READ(QspiBaseAddr + QSPI_ISR_OFFSET);
}

static inline AL_VOID AlQspi_ll_ClearAllIntr(AL_REG BaseAddr)
{
    AL_REG32_READ(BaseAddr + QSPI_ICR_OFFSET);
}

static inline AL_VOID AlQspi_ll_ClearMsticrIntr(AL_REG BaseAddr)
{
    AL_REG32_READ(BaseAddr + QSPI_MSTICR_OFFSET);
}

static inline AL_VOID AlQspi_ll_ClearRxuicrIntr(AL_REG BaseAddr)
{
    AL_REG32_READ(BaseAddr + QSPI_RXUICR_OFFSET);
}

static inline AL_VOID AlQspi_ll_ClearRxoicrIntr(AL_REG BaseAddr)
{
    AL_REG32_READ(BaseAddr + QSPI_RXOICR_OFFSET);
}

static inline AL_VOID AlQspi_ll_ClearTxoicrIntr(AL_REG BaseAddr)
{
    AL_REG32_READ(BaseAddr + QSPI_TXOICR_OFFSET);
}

static inline AL_VOID AlQspi_ll_TxDmaEnable(AL_REG BaseAddr)
{
    AL_REG32_SET_BIT(BaseAddr + QSPI_DMACR_OFFSET, QSPI_DMACR_TDMAE_SHIFT, AL_FUNC_ENABLE);
}

static inline AL_VOID AlQspi_ll_RxDmaEnable(AL_REG BaseAddr)
{
    AL_REG32_SET_BIT(BaseAddr + QSPI_DMACR_OFFSET, QSPI_DMACR_RDMAE_SHIFT, AL_FUNC_ENABLE);
}

static inline AL_VOID AlQspi_ll_DmaDisable(AL_REG BaseAddr)
{
    AL_REG32_WRITE(BaseAddr + QSPI_DMACR_OFFSET, AL_FUNC_DISABLE);
}

static inline AL_VOID AlQspi_ll_SetDmaTransLevel(AL_REG BaseAddr, AL_U8 Level)
{
    AL_REG32_WRITE(BaseAddr + QSPI_DMATDLR_OFFSET, Level);
}

static inline AL_VOID AlQspi_ll_SetDmaRecevLevel(AL_REG BaseAddr, AL_U8 Level)
{
    AL_REG32_WRITE(BaseAddr + QSPI_DMARDLR_OFFSET, Level);
}

static inline AL_VOID AlQspi_ll_SetRecvDataSamplEdge(AL_REG BaseAddr, AL_QSPI_RecvDataSamplEdgeEnum SamplEdge)
{
    AL_REG32_SET_BIT(BaseAddr + QSPI_RX_SAMPLE_DELAY_OFFSET, QSPI_RX_SAMPLE_DELAY_SE_SHIFT, SamplEdge);
}

static inline AL_VOID AlQspi_ll_SetRecvDataSamplDelay(AL_REG BaseAddr, AL_U8 RxSmapleDelay)
{
    AL_REG32_SET_BITS(BaseAddr + QSPI_RX_SAMPLE_DELAY_OFFSET, QSPI_RX_SAMPLE_DELAY_RSD_SHIFT,
                      QSPI_RX_SAMPLE_DELAY_RSD_SIZE, RxSmapleDelay);
}

static inline AL_VOID AlQspi_ll_SetClockStretch(AL_REG BaseAddr, AL_QSPI_ClockStretchEnum ClockStretch)
{
    AL_REG32_SET_BIT(BaseAddr + QSPI_SPI_CTRLR0_OFFSET, QSPI_SPI_CTRLR0_CLK_STRETCH_EN_SHIFT, ClockStretch);
}

static inline AL_VOID AlQspi_ll_SetXipPrefetch(AL_REG BaseAddr, AL_QSPI_XipPrefetchEnableEnum XipPrefetch)
{
    AL_REG32_SET_BIT(BaseAddr + QSPI_SPI_CTRLR0_OFFSET, QSPI_SPI_CTRLR0_XIP_PREFETCH_EN_SHIFT, XipPrefetch);
}

static inline AL_VOID AlQspi_ll_SetXipMbl(AL_REG BaseAddr, AL_QSPI_XipModeBitsLengthEnum XipMbl)
{
    AL_REG32_SET_BITS(BaseAddr + QSPI_SPI_CTRLR0_OFFSET, QSPI_SPI_CTRLR0_XIP_MBL_SHIFT,
                      QSPI_SPI_CTRLR0_XIP_MBL_SIZE, XipMbl);
}

static inline AL_VOID AlQspi_ll_SetXipContTrans(AL_REG BaseAddr, AL_QSPI_XipContTransEnableEnum XipContTrans)
{
    AL_REG32_SET_BIT(BaseAddr + QSPI_SPI_CTRLR0_OFFSET, QSPI_SPI_CTRLR0_SSIC_XIP_CONT_XFER_EN_SHIFT, XipContTrans);
}

static inline AL_VOID AlQspi_ll_SetXipInstPhase(AL_REG BaseAddr, AL_QSPI_XipInstPhaseEnableEnum XipInstPhase)
{
    AL_REG32_SET_BIT(BaseAddr + QSPI_SPI_CTRLR0_OFFSET, QSPI_SPI_CTRLR0_XIP_INST_EN_SHIFT, XipInstPhase);
}

static inline AL_VOID AlQspi_ll_SetXipDfsFix(AL_REG BaseAddr, AL_QSPI_XipDfsFixEnum XipDfsFix)
{
    AL_REG32_SET_BIT(BaseAddr + QSPI_SPI_CTRLR0_OFFSET, QSPI_SPI_CTRLR0_XIP_DFS_HC_SHIFT, XipDfsFix);
}

static inline AL_VOID AlQspi_ll_SetInstDdr(AL_REG BaseAddr, AL_QSPI_InstDdrEnableEnum InstDdrInfo)
{
    AL_REG32_SET_BIT(BaseAddr + QSPI_SPI_CTRLR0_OFFSET, QSPI_SPI_CTRLR0_INST_DDR_EN_SHIFT, InstDdrInfo);
}

static inline AL_VOID AlQspi_ll_SetSpiDdr(AL_REG BaseAddr, AL_QSPI_SpiDdrEnableEnum SpiDdrInfo)
{
    AL_REG32_SET_BIT(BaseAddr + QSPI_SPI_CTRLR0_OFFSET, QSPI_SPI_CTRLR0_SPI_DDR_EN_SHIFT, SpiDdrInfo);
}

static inline AL_VOID AlQspi_ll_SetWaitCycles(AL_REG BaseAddr, AL_U16 WaitCycles)
{
    AL_REG32_SET_BITS(BaseAddr + QSPI_SPI_CTRLR0_OFFSET, QSPI_SPI_CTRLR0_WAIT_CYCLES_SHIFT,
                        QSPI_SPI_CTRLR0_WAIT_CYCLES_SIZE, WaitCycles);
}

static inline AL_VOID AlQspi_ll_SetInstLength(AL_REG BaseAddr, AL_QSPI_InstLengthEnum InstLength)
{
    AL_REG32_SET_BITS(BaseAddr + QSPI_SPI_CTRLR0_OFFSET, QSPI_SPI_CTRLR0_INST_L_SHIFT,
                        QSPI_SPI_CTRLR0_INST_L_SIZE, InstLength);
}

static inline AL_VOID AlQspi_ll_SetXipModeBitEn(AL_REG BaseAddr, AL_QSPI_XipModeBitEnableEnum XipModeBit)
{
    AL_REG32_SET_BIT(BaseAddr + QSPI_SPI_CTRLR0_OFFSET, QSPI_SPI_CTRLR0_XIP_MD_BIT_EN_SHIFT, XipModeBit);
}

static inline AL_VOID AlQspi_ll_SetAddrLength(AL_REG BaseAddr, AL_QSPI_AddrLengthEnum AddrLength)
{
    AL_REG32_SET_BITS(BaseAddr + QSPI_SPI_CTRLR0_OFFSET, QSPI_SPI_CTRLR0_ADDR_L_SHIFT,
                        QSPI_SPI_CTRLR0_ADDR_L_SIZE, AddrLength);
}

static inline AL_VOID AlQspi_ll_SetTransType(AL_REG BaseAddr, AL_QSPI_TransType TransType)
{
    AL_REG32_SET_BITS(BaseAddr + QSPI_SPI_CTRLR0_OFFSET, QSPI_SPI_CTRLR0_TRANS_TYPE_SHIFT,
                        QSPI_SPI_CTRLR0_TRANS_TYPE_SIZE, TransType);
}

static inline AL_VOID AlQspi_ll_SetXipModeBits(AL_REG BaseAddr, AL_U16 XipModeBits)
{
    AL_REG32_SET_BITS(BaseAddr + QSPI_XIP_MODE_BITS_OFFSET, QSPI_XIP_MODE_BITS_XIP_MD_BITS_SHIFT,
                        QSPI_XIP_MODE_BITS_XIP_MD_BITS_SIZE, XipModeBits);
}

static inline AL_VOID AlQspi_ll_SetXipIncrInst(AL_REG BaseAddr, AL_U16 XipInstOpcode)
{
    AL_REG32_SET_BITS(BaseAddr + QSPI_XIP_INCR_INST_OFFSET, QSPI_XIP_INCR_INST_INCR_INST_SHIFT,
                        QSPI_XIP_INCR_INST_INCR_INST_SIZE, XipInstOpcode);
}

static inline AL_VOID AlQspi_ll_SetXipWrapInst(AL_REG BaseAddr, AL_U16 XipWrapInst)
{
    AL_REG32_SET_BITS(BaseAddr + QSPI_XIP_WRAP_INST_OFFSET, QSPI_XIP_WRAP_INST_WRAP_INST_SHIFT,
                        QSPI_XIP_WRAP_INST_WRAP_INST_SIZE, XipWrapInst);
}

static inline AL_VOID AlQspi_ll_SetXipCntTimeOut(AL_REG BaseAddr, AL_U8 XipCntTimeOut)
{
    AL_REG32_SET_BITS(BaseAddr + QSPI_XIP_CNT_TIME_OUT_OFFSET, QSPI_XIP_CNT_TIME_OUT_XTOC_SHIFT,
                        QSPI_XIP_CNT_TIME_OUT_XTOC_SIZE, XipCntTimeOut);
}

static inline AL_VOID AlQspi_ll_SetXipPort1NorFlashSize(AL_REG BaseAddr, AL_QSPI_XipPort1NorFlashSize Nor1FlashSize)
{
    AL_REG32_SET_BITS(TOP_NS_CFG_CTRL_QSPI_ADDR, TOP_NS_CFG_CTRL_QSPI_CTRL_QSPI_SHIFT,
                        TOP_NS_CFG_CTRL_QSPI_CTRL_QSPI_SIZE, Nor1FlashSize);
}

#ifdef __cplusplus
}
#endif

#endif
