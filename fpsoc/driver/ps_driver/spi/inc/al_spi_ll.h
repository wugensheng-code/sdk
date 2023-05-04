#ifndef AL_SPI_LL_H
#define AL_SPI_LL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "al_spi_hw.h"
#include "al_reg_io.h"
#include "stdio.h"

/*
 * ffs: find first bit set. This is defined the same way as
 * the libc and compiler builtin ffs routines, therefore
 * differs in spirit from the above ffz (man ffs).
 */

static inline int generic_fls(AL_U32 x)
{
	int r = 1;

	if (!x)
		return 0;
	if (!(x & 0xffff)) {
		x >>= 16;
		r += 16;
	}
	if (!(x & 0xff)) {
		x >>= 8;
		r += 8;
	}
	if (!(x & 0xf)) {
		x >>= 4;
		r += 4;
	}
	if (!(x & 3)) {
		x >>= 2;
		r += 2;
	}
	if (!(x & 1)) {
		x >>= 1;
		r += 1;
	}
	return r;
}

/**
 * fls - find last (most-significant) bit set
 * @x: the word to search
 *
 * This is defined the same way as ffs.
 * Note fls(0) = 0, fls(1) = 1, fls(0x80000000) = 32.
 */
static inline int GenericFfs(int x)
{
	int r = 32;

	if (!x)
		return 0;
	if (!(x & 0xffff0000u)) {
		x <<= 16;
		r -= 16;
	}
	if (!(x & 0xff000000u)) {
		x <<= 8;
		r -= 8;
	}
	if (!(x & 0xf0000000u)) {
		x <<= 4;
		r -= 4;
	}
	if (!(x & 0xc0000000u)) {
		x <<= 2;
		r -= 2;
	}
	if (!(x & 0x80000000u)) {
		x <<= 1;
		r -= 1;
	}
	return r;
}

typedef enum
{
    SPI_MASTER_MODE      = 0,
    SPI_SLAVE_MODE       = 1
} AL_SPI_Mode;

typedef enum
{
    SPI_TX_RX           = 0,
    SPI_TX_ONLY         = 1,
    SPI_RX_ONLY         = 2,
    SPI_EEPROM          = 3
} AL_SPI_TransferMode;

typedef enum
{
    MOTOROLA_SPI         = 0,   /* Motorola SPI Frame Format */
    TEXAS_SSP            = 1,   /* Texas Instruments SSP Frame */
    NS_MICROWIRE         = 2    /* National Microwire Format */
} AL_SPI_ProtFormat;

typedef enum
{
    SPI_CLK_MODE0       = 0,    /* CPOL = 0, CPHA = 0 */
    SPI_CLK_MODE1       = 1,    /* CPOL = 0, CPHA = 1 */
    SPI_CLK_MODE2       = 2,    /* CPOL = 1, CPHA = 0 */
    SPI_CLK_MODE3       = 3     /* CPOL = 1, CPHA = 1 */
} AL_SPI_ClockEnum;

typedef enum
{
    SPI_FRAME_04BITS     = 0x3,
    SPI_FRAME_05BITS     = 0x4,
    SPI_FRAME_06BITS     = 0x5,
    SPI_FRAME_07BITS     = 0x6,
    SPI_FRAME_08BITS     = 0x7,
    SPI_FRAME_09BITS     = 0x8,
    SPI_FRAME_10BITS     = 0x9,
    SPI_FRAME_11BITS     = 0xa,
    SPI_FRAME_12BITS     = 0xb,
    SPI_FRAME_13BITS     = 0xc,
    SPI_FRAME_14BITS     = 0xd,
    SPI_FRAME_15BITS     = 0xe,
    SPI_FRAME_16BITS     = 0xf, 
    SPI_FRAME_17BITS     = 0x10,
    SPI_FRAME_18BITS     = 0x11,
    SPI_FRAME_19BITS     = 0x12,
    SPI_FRAME_20BITS     = 0x13,
    SPI_FRAME_21BITS     = 0x14,
    SPI_FRAME_22BITS     = 0x15,
    SPI_FRAME_23BITS     = 0x16,
    SPI_FRAME_24BITS     = 0x17,
    SPI_FRAME_25BITS     = 0x18,
    SPI_FRAME_26BITS     = 0x19,
    SPI_FRAME_27BITS     = 0x1a,
    SPI_FRAME_28BITS     = 0x1b,
    SPI_FRAME_29BITS     = 0x1c, 
    SPI_FRAME_30BITS     = 0x1d,
    SPI_FRAME_31BITS     = 0x1e,
    SPI_FRAME_32BITS     = 0x1f
} AL_SPI_DataFrameSize;

typedef enum
{
    SPI_SLV_TOGGLE_DISABLE   = 0,    
    SPI_SLV_TOGGLE_ENABLE    = 1
} AL_SPI_SlvSelToggleEnum;


typedef enum
{
    SPI_SER_SS0_EN      = (1 << 0),
    SPI_SER_SS1_EN      = (1 << 1),    
    SPI_SER_SS2_EN      = (1 << 2)
} AL_SPI_SlvSelEnum;

typedef enum
{
    SPI_SR_TXFIFO_NOTEMPTY   = 0,    
    SPI_SR_TXFIFO_EMPTY      = 1
} AL_SPI_TxFifoEmptyEnum;

typedef enum
{
    SPI_SR_TXFIFO_FULL      = 0,    
    SPI_SR_TXFIFO_NOTFULL   = 1
} AL_SPI_TxFifoFullEnum;

typedef enum
{
    SPI_SR_IDLE     = 0,    
    SPI_SR_BUSY     = 1
} AL_SPI_BusyEnum;

typedef enum
{
    SPI_SR_RXFIFO_EMPTY      = 0,    
    SPI_SR_RXFIFO_NOTEMPTY   = 1
} AL_SPI_RxFifoEmptyEnum;

typedef enum
{
    SPI_SR_RXFIFO_NOTFULL    = 0,    
    SPI_SR_RXFIFO_FULL       = 1
} AL_SPI_RxFifoFullEnum;

typedef enum
{
    AL_SPI_SEND_DONE                = (0x01),
    AL_SPI_RECEIVE_DONE             = (0x01 << 1),
    AL_SPI_SEND_TIMEOUT             = (0x01 << 2),
    AL_SPI_RECEIVE_TIMEOUT          = (0x01 << 3)
} AL_SPI_EventIdEnum;

typedef struct
{
    AL_SPI_EventIdEnum      Event;
    AL_U32                  EventData;
} AL_SPI_EventStruct;

/**
 * @brief  Configs Struct
 */
typedef struct
{
    // AL_U32                  DevID;
    AL_SPI_Mode             Mode;
    AL_SPI_TransferMode     TransMode;
    AL_SPI_ProtFormat       ProtFormat;
    AL_SPI_ClockEnum        ClockEnum;
    AL_SPI_DataFrameSize    DataFrameSize;
    AL_U16                  ClkDiv;
    AL_SPI_SlvSelToggleEnum SlvToggleEnum;
    AL_SPI_SlvSelEnum       SlvSelEnum;
} AL_SPI_ConfigsStruct;

typedef enum
{
    TXEIS           = 0x1,      /* Transmit FIFO Empty Interrupt Status */
    TXOIS           = 0x2,      /* Transmit FIFO Overflow Interrupt Status */
    RXUIS           = 0x4,      /* Receive FIFO Underflow Interrupt Status */
    RXOIS           = 0x8,      /* Receive FIFO Overflow Interrupt Status */
    RXFIS           = 0x10,     /* Receive FIFO Full Interrupt Status */
} AL_SPI_InterruptEnum;

typedef enum
{
    SPI_MASK_TXEIM    = (1 << SPI_IMR_MST_TXEIM_SHIFT),   /* Transmit FIFO Empty Interrupt */
    SPI_MASK_TXOIM    = (1 << SPI_IMR_MST_TXOIM_SHIFT),   /* Transmit FIFO Overflow Interrupt  */
    SPI_MASK_RXUIM    = (1 << SPI_IMR_MST_RXUIM_SHIFT),   /* Receive FIFO Underflow Interrupt */
    SPI_MASK_RXOIM    = (1 << SPI_IMR_MST_RXOIM_SHIFT),   /* Receive FIFO Overflow Interrupt */
    SPI_MASK_RXFIM    = (1 << SPI_IMR_MST_RXFIM_SHIFT),   /* Receive FIFO Full Interrupt */
    SPI_MASK_MSTIM    = (1 << SPI_IMR_MST_MSTIM_SHIFT)    /* Multi-Master Contention Interrupt */
} AL_SPI_IntrShiftEnum;


/**  
 * This function set spi master or slave. 
 * @param   SpiBaseAddr is the spi base addr
 * @param   SpiMode is spi master or slave enmu
 * @return  AL_VOID 
 * @note    none.
 */ 
static inline AL_VOID AlSpi_ll_SetSpiMode(AL_REG SpiBaseAddr, AL_SPI_Mode SpiMode)
{
    if (SPI_MASTER_MODE == SpiMode) {

    } else if (SPI_SLAVE_MODE == SpiMode) {

    }
}

/**  
 * This function disbale spi. 
 * @param   SpiBaseAddr is the spi base addr
 * @return  AL_VOID 
 * @note    Before configuring spi,must disable the spi.
 */ 
static inline AL_VOID AlSpi_ll_Disable(AL_REG SpiBaseAddr)
{
    AL_REG32_WRITE(SpiBaseAddr + SPI_SSIENR_MST_OFFSET, 0);
}

/**  
 * This function enable spi. 
 * @param   SpiBaseAddr is the spi base addr
 * @return  AL_VOID 
 * @note    none
 */ 
static inline AL_VOID AlSpi_ll_Enable(AL_REG SpiBaseAddr)
{
    AL_REG32_WRITE(SpiBaseAddr + SPI_SSIENR_MST_OFFSET, 1);
}

/**  
 * This function set spi transfer mode. 
 * @param   SpiBaseAddr is the spi base addr
 * @param   SpiTransfMode is spi transfer mode
 * @return  AL_VOID 
 * @note    none GenericFfs(SPI_CTRLR0_MST_TMOD_MASK >> SPI_CTRLR0_MST_TMOD_SHIFT)
 */ 
static inline AL_VOID AlSpi_ll_SetTransfMode(AL_REG SpiBaseAddr, AL_SPI_TransferMode SpiTransfMode)
{
	AL_REG32_SET_BITS(SpiBaseAddr + SPI_CTRLR0_MST_OFFSET, SPI_CTRLR0_MST_TMOD_SHIFT, 
             GenericFfs(SPI_CTRLR0_MST_TMOD_MASK >> SPI_CTRLR0_MST_TMOD_SHIFT), SpiTransfMode);
}

/**  
 * This function set spi protocol format. 
 * @param   SpiBaseAddr is the spi base addr
 * @param   SpiProtFormat is spi protocol format
 * @return  AL_VOID 
 * @note    none
 */ 
static inline AL_VOID AlSpi_ll_SetProtFormat(AL_REG SpiBaseAddr, AL_SPI_ProtFormat SpiProtFormat)
{
    AL_REG32_SET_BITS(SpiBaseAddr + SPI_CTRLR0_MST_OFFSET, SPI_CTRLR0_MST_FRF_SHIFT, 
             GenericFfs(SPI_CTRLR0_MST_FRF_MASK >> SPI_CTRLR0_MST_FRF_SHIFT), SpiProtFormat);
}

/**  
 * This function set spi clock polarity(CPOL) and clock phase (CPHA). 
 * @param   SpiBaseAddr is the spi base addr
 * @param   SpiClockEnum is spi clock Enum
 * @return  AL_VOID 
 * @note    Valid when the frame format is set to Motorola SPI.
 */ 
static inline AL_VOID AlSpi_ll_SetCpolAndCpha(AL_REG SpiBaseAddr, AL_SPI_ClockEnum SpiClockEnum)
{
    AL_REG32_SET_BITS(SpiBaseAddr + SPI_CTRLR0_MST_OFFSET, SPI_CTRLR0_MST_SCPH_SHIFT, 
             GenericFfs((SPI_CTRLR0_MST_SCPH_MASK + SPI_CTRLR0_MST_SCPOL_MASK) >> SPI_CTRLR0_MST_SCPH_SHIFT), SpiClockEnum);
}

/**  
 * This function set spi data frame size. 
 * @param   SpiBaseAddr is the spi base addr
 * @param   SpiDataFrameSize is spi data frame size
 * @return  AL_VOID 
 * @note    none.
 */ 
static inline AL_VOID AlSpi_ll_SetDataFrameSize(AL_REG SpiBaseAddr, AL_SPI_DataFrameSize SpiDataFrameSize)
{
    AL_REG32_SET_BITS(SpiBaseAddr + SPI_CTRLR0_MST_OFFSET, SPI_CTRLR0_MST_DFS_32_SHIFT, 
             GenericFfs(SPI_CTRLR0_MST_DFS_32_MASK >> SPI_CTRLR0_MST_DFS_32_SHIFT), SpiDataFrameSize);
}

/**  
 * This function set spi clock divider.
 * @param   SpiBaseAddr is the spi base addr
 * @param   SpiClkDiv is spi clk divider
 * @return  AL_VOID 
 * @note    The LSB for this field is always set to 0 and is unaffected by a
 * write operation, which ensures an even value is held in this register.
 * For Fssi_clk = 50MHz and SCKDV =2. Fsclk_out = 50/(2 << 1) = 12.5MHz
 */ 
static inline AL_VOID AlSpi_ll_SetClkDiv(AL_REG SpiBaseAddr, AL_U16 SpiClkDiv)
{
    AL_REG32_SET_BITS(SpiBaseAddr + SPI_BAUDR_MST_OFFSET, SPI_BAUDR_MST_SCKDV_SHIFT, 
             GenericFfs(SPI_BAUDR_MST_SCKDV_MASK >> SPI_BAUDR_MST_SCKDV_SHIFT), SpiClkDiv);
}

/**  
 * This function set spi clock divider.
 * @param   SpiBaseAddr is the spi base addr
 * @param   SpiClkDiv is spi clk divider
 * @return  AL_VOID 
 * @note    The LSB for this field is always set to 0 and is unaffected by a
 * write operation, which ensures an even value is held in this register.
 * For Fssi_clk = 50MHz and SCKDV =2. Fsclk_out = 50/(2 << 1) = 12.5MHz
 */ 
static inline AL_VOID AlSpi_ll_SetSlvSelToggle(AL_REG SpiBaseAddr, AL_SPI_SlvSelToggleEnum SpiSlvSelToggleEnum)
{
    AL_REG32_SET_BITS(SpiBaseAddr + SPI_CTRLR0_MST_OFFSET, SPI_CTRLR0_MST_SSTE_SHIFT, 
             GenericFfs(SPI_CTRLR0_MST_SSTE_MASK >> SPI_CTRLR0_MST_SSTE_SHIFT), SpiSlvSelToggleEnum);
}


/**  
 * This function set spi clock divider.
 * @param   SpiBaseAddr is the spi base addr
 * @param   SpiClkDiv is spi clk divider
 * @return  AL_VOID 
 * @note    The LSB for this field is always set to 0 and is unaffected by a
 * write operation, which ensures an even value is held in this register.
 * For Fssi_clk = 50MHz and SCKDV =2. Fsclk_out = 50/(2 << 1) = 12.5MHz
 */ 
static inline AL_VOID AlSpi_ll_SetSlvSel(AL_REG SpiBaseAddr, AL_SPI_SlvSelEnum SpiSlvSelEnum)
{
    AL_REG32_SET_BITS(SpiBaseAddr + SPI_SER_MST_OFFSET, SPI_SER_MST_SER_SHIFT, 
             GenericFfs(SPI_SER_MST_SER_MASK >> SPI_SER_MST_SER_SHIFT), SpiSlvSelEnum);
}


/**  
 * This function set spi transmit FIFO threshold level.
 * @param   SpiBaseAddr is the spi base addr
 * @param   SpiTxFifoThrLevel is transmit FIFO threshold level
 * @return  AL_VOID 
 * @note    none.
 */ 
static inline AL_VOID AlSpi_ll_SetTxFifoThrLevel(AL_REG SpiBaseAddr, AL_U8 SpiTxFifoThrLevel)
{
    AL_REG32_SET_BITS(SpiBaseAddr + SPI_TXFTLR_MST_OFFSET, SPI_TXFTLR_MST_TFT_SHIFT, 
             GenericFfs(SPI_TXFTLR_MST_TFT_MASK >> SPI_TXFTLR_MST_TFT_SHIFT), SpiTxFifoThrLevel);
}

/**  
 * This function set spi receive FIFO threshold level.
 * @param   SpiBaseAddr is the spi base addr
 * @param   SpiRxFifoThrLevel is receive FIFO threshold level
 * @return  AL_VOID 
 * @note    none.
 */ 
static inline AL_VOID AlSpi_ll_SetRxFifoThrLevel(AL_REG SpiBaseAddr, AL_U8 SpiRxFifoThrLevel)
{
    AL_REG32_SET_BITS(SpiBaseAddr + SPI_RXFTLR_MST_OFFSET, SPI_RXFTLR_MST_RFT_SHIFT, 
             GenericFfs(SPI_RXFTLR_MST_RFT_MASK >> SPI_RXFTLR_MST_RFT_SHIFT), SpiRxFifoThrLevel);
}



static inline AL_U32 AlSpi_ll_ReadTxFifoLevel(AL_REG SpiBaseAddr)
{
    return AL_REG32_READ(SpiBaseAddr + SPI_TXFLR_MST_OFFSET);
}

static inline AL_U32 AlSpi_ll_ReadRxFifoLevel(AL_REG SpiBaseAddr)
{
    return AL_REG32_READ(SpiBaseAddr + SPI_RXFLR_MST_OFFSET);
}

static inline AL_U32 AlSpi_ll_ReadRawIntrStatus(AL_REG SpiBaseAddr)
{
    return AL_REG32_READ(SpiBaseAddr + SPI_RISR_MST_OFFSET);
}

/**  
 * This function spi transmit data.
 * @param   SpiBaseAddr is the spi base addr
 * @param   Data is transmit data
 * @return  AL_VOID 
 * @note    none.
 */ 
static inline AL_VOID AlSpi_ll_DataTransmit(AL_REG SpiBaseAddr, AL_U32 Data)
{
	AL_REG32_WRITE(SpiBaseAddr + SPI_DR0_MST_OFFSET, Data);
}

/**  
 * This function spi receive data.
 * @param   SpiBaseAddr is the spi base addr
 * @return  The data being read
 * @note    none.
 */ 
static inline AL_U32 AlSpi_ll_DataReceive(AL_REG SpiBaseAddr)
{
    return AL_REG32_READ(SpiBaseAddr + SPI_DR0_MST_OFFSET);
}

/**  
 * This function return whether spi transmit FIFO empty.
 * @param   SpiBaseAddr is the spi base addr
 * @return  The transmit fifo whether empty
 * @note    none.
 */ 
static inline AL_SPI_TxFifoEmptyEnum AlSpi_ll_IsTxFifoEmpty(AL_REG SpiBaseAddr)
{
    return  AL_REG32_GET_BITS(SpiBaseAddr + SPI_SR_MST_OFFSET, SPI_SR_MST_TFE_SHIFT, 
            GenericFfs(SPI_SR_MST_TFE_MASK >> SPI_SR_MST_TFE_SHIFT));
}

/**  
 * This function return whether spi transmit FIFO full.
 * @param   SpiBaseAddr is the spi base addr
 * @return  The transmit fifo whether full
 * @note    none.
 */ 
static inline AL_SPI_TxFifoFullEnum AlSpi_ll_IsTxFifoFull(AL_REG SpiBaseAddr)
{
    return  AL_REG32_GET_BITS(SpiBaseAddr + SPI_SR_MST_OFFSET, SPI_SR_MST_TFNF_SHIFT, 
            GenericFfs(SPI_SR_MST_TFNF_MASK >> SPI_SR_MST_TFNF_SHIFT));
}

/**  
 * This function return spi whether busy.
 * @param   SpiBaseAddr is the spi base addr
 * @return  The spi whether busy
 * @note    none.
 */ 
static inline AL_SPI_BusyEnum AlSpi_ll_IsBusy(AL_REG SpiBaseAddr)
{
    return  AL_REG32_GET_BITS(SpiBaseAddr + SPI_SR_MST_OFFSET, SPI_SR_MST_BUSY_SHIFT, 
            GenericFfs(SPI_SR_MST_BUSY_MASK >> SPI_SR_MST_BUSY_SHIFT));
}

/**  
 * This function return whether spi receive FIFO empty.
 * @param   SpiBaseAddr is the spi base addr
 * @return  The receive fifo whether empty
 * @note    none.
 */ 
static inline AL_SPI_RxFifoEmptyEnum AlSpi_ll_IsRxFifoEmpty(AL_REG SpiBaseAddr)
{
    return  AL_REG32_GET_BITS(SpiBaseAddr + SPI_SR_MST_OFFSET, SPI_SR_MST_RFNE_SHIFT, 
            GenericFfs(SPI_SR_MST_RFNE_MASK >> SPI_SR_MST_RFNE_SHIFT));
}

/**  
 * This function return whether spi receive FIFO full.
 * @param   SpiBaseAddr is the spi base addr
 * @param   SpiDataFramesNum is Number of Data Frames
 * @return  AL_VOID
 * @note    this register serves no purpose and is not present when the DW_apb_ssi is configured as a serial slave.
 */ 
static inline AL_SPI_RxFifoFullEnum AlSpi_ll_IsRxFifoFull(AL_REG SpiBaseAddr)
{
    return  AL_REG32_GET_BITS(SpiBaseAddr + SPI_SR_MST_OFFSET, SPI_SR_MST_RFF_SHIFT, 
            GenericFfs(SPI_SR_MST_RFF_MASK >> SPI_SR_MST_RFF_SHIFT));
}


static inline AL_VOID AlSpi_ll_SetRecvNumOfDataFrames(AL_REG SpiBaseAddr, AL_U16 SpiRecvDataFramesNum)
{
     AL_REG32_SET_BITS(SpiBaseAddr + SPI_CTRLR1_MST_OFFSET, SPI_CTRLR1_MST_NDF_SHIFT, 
             GenericFfs(SPI_CTRLR1_MST_NDF_MASK >> SPI_CTRLR1_MST_NDF_SHIFT), SpiRecvDataFramesNum);
}


static inline AL_VOID AlSpi_ll_MaskAllIntr(AL_REG SpiBaseAddr)
{
     AL_REG32_WRITE(SpiBaseAddr + SPI_IMR_MST_OFFSET,  SPI_IMR_MST_RESERVED_31_6_MASK);
}

 
static inline AL_VOID AlSpi_ll_MaskIntr(AL_REG SpiBaseAddr, AL_SPI_IntrShiftEnum SpiMaskIntr)
{
      AL_REG32_SET_BITS(SpiBaseAddr + SPI_IMR_MST_OFFSET, GenericFfs(SpiMaskIntr)-1, 
             1, 0);
}


static inline AL_VOID AlSpi_ll_EnableIntr(AL_REG SpiBaseAddr, AL_SPI_IntrShiftEnum SpiEnableIntr)
{
     AL_REG32_SET_BITS(SpiBaseAddr + SPI_IMR_MST_OFFSET, GenericFfs(SpiEnableIntr)-1, 
             1, 1);
}


static inline AL_U32 AlSpi_ll_GetIntrStatus(AL_REG SpiBaseAddr)
{
    return AL_REG32_READ(SpiBaseAddr + SPI_ISR_MST_OFFSET);
}



static inline AL_VOID AlSpi_ll_ClearAllIntr(AL_REG BaseAddr)
{
    AL_REG32_READ(BaseAddr + SPI_ICR_MST_OFFSET);
}

static inline AL_VOID AlSpi_ll_ClearRxuicrIntr(AL_REG BaseAddr)
{
    AL_REG32_READ(BaseAddr + SPI_RXUICR_MST_OFFSET);
}

static inline AL_VOID AlSpi_ll_ClearRxoicrIntr(AL_REG BaseAddr)
{
    AL_REG32_READ(BaseAddr + SPI_RXOICR_MST_OFFSET);
}

static inline AL_VOID AlSpi_ll_ClearTxoicrIntr(AL_REG BaseAddr)
{
    AL_REG32_READ(BaseAddr + SPI_TXOICR_MST_OFFSET);
}



#ifdef __cplusplus
}
#endif

#endif
