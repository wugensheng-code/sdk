/*
 * AL_UART.H
 *
 *  Created on: 2021年9月17日
 *      Author: wei.pang
 */

/**
 ****************************************************************************************
 * @defgroup UART
 * @ingroup DRIVERS
 * @brief UART Drivers.
 *        Functions specified for UART.
 *
 * @{
 ****************************************************************************************
 */

#ifndef _AL_UART_H
#define _AL_UART_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Register offsets */
#define UART_REG_RBR         	0x00
#define UART_REG_THR         	0x00
#define UART_REG_DLL         	0x00
#define UART_REG_DLH         	0x04
#define UART_REG_IER            0x04
#define UART_REG_IIR            0x08
#define UART_REG_FCR            0x08
#define UART_REG_LCR            0x0C
#define UART_REG_MCR            0x10
#define UART_REG_LSR            0x14
#define UART_REG_MSR            0x18
#define UART_REG_SCR            0x1c
#define UART_REG_FAR            0x70
#define UART_REG_TFR            0x74
#define UART_REG_RFW            0x78
#define UART_REG_USR            0x7C
#define UART_REG_TFL            0x80
#define UART_REG_RFL            0x84
#define UART_REG_CPR            0xF4

/*******************  Bit definition for UART_REG_RBR register  ********************/

/*******************  Bit definition for UART_REG_THR register  ********************/
 
/*******************  Bit definition for UART_REG_DLL register  ********************/
//#define UART_REG_DLL               0x1
//#define UART_TXWM_DLH(x)            (((x) & 0xffff) << 16) 
/*******************  Bit definition for UART_REG_DLH register  ********************/
//#define UART_REG_DLH               0x1
//#define UART_TXWM_DLH(x)            (((x) & 0xffff) << 16) 
 
 
 
 
/*******************  Bit definition for UART_REG_IER register  ********************/
 /*
Enable Received Data Available Interrupt. 
  */
 #define UART_REG_IER_ERBFI   (0)
 #define UART_REG_IER_ERBFI_MASK  (0x1 << UART_REG_IER_ERBFI)
  /*
Enable Transmit Holding Register Empty Interrupt. 
  */ 
#define UART_REG_IER_ETBEI   (1)
#define UART_REG_IER_ETBEI_MASK  (0x1 << UART_REG_IER_ETBEI)
  /*
Enable Receiver Line Status Interrupt. 
  */ 
#define UART_REG_IER_ELSI   (2)
#define UART_REG_IER_ELSI_MASK  (0x1 << UART_REG_IER_ELSI)
  /*
Enable Modem Status Interrupt. 
  */ 
#define UART_REG_IER_EDSSI   (3)
#define UART_REG_IER_EDSSI_MASK  (0x1 << UART_REG_IER_EDSSI)
  /*
Programmable THRE Interrupt Mode Enable.
  */
#define UART_REG_IER_PTIME   (7)
#define UART_REG_IER_PTIME_MASK  (0x1 << UART_REG_IER_PTIME)
 
 
/*******************  Bit definition for UART_REG_IIR register  ********************/
/*
Interrupt ID 
 */
#define UART_REG_IIR_IID_MASK  ((uint8_t) 0x07)
/*
FIFOs Enabled. 
 */
#define UART_REG_IIR_FIFOSE   (0)   
#define UART_REG_IIR_FIFOSE_MASK   
/*******************  Bit definition for UART_REG_FCR register  ********************/
 /*
FIFO Enable. 
  */
 #define UART_REG_FCR_FIFOE   (0)
 #define UART_REG_FCR_FIFOE_MASK  (0x1 << UART_REG_FCR_FIFOE)
  /*
RCVR FIFO Reset. 
  */ 
#define UART_REG_FCR_RFIFOR   (1)
#define UART_REG_FCR_RFIFOR_MASK  (0x1 << UART_REG_FCR_RFIFOR)
  /*
XMIT FIFO Reset. 
  */ 
#define UART_REG_FCR_XFIFOR   (2)
#define UART_REG_FCR_XFIFOR_MASK  (0x1 << UART_REG_FCR_XFIFOR)
  /*
DMA Mode.
  */ 
#define UART_REG_FCR_DMAM   (3)
#define UART_REG_FCR_DMAM_MASK  (0x1 << UART_REG_FCR_DMAM)
  /*
TX Empty Trigger.  
  */
#define UART_REG_FCR_TET   (4)
#define UART_REG_FCR_TET_MASK  (0x3 << UART_REG_FCR_TET)
  /*
RCVR Trigger. 
  */
#define UART_REG_FCR_RCVR   (6)
#define UART_REG_FCR_RCVR_MASK  (0x3 << UART_REG_FCR_RCVR)
 
 
 
/*******************  Bit definition for UART_REG_LCR register  ********************/
 /*
Data Length Select.  
 */
#define UART_REG_LCR_DLS_5BITS  				((uint8_t)0x00)
#define UART_REG_LCR_DLS_6BITS  				((uint8_t)0x01)
#define UART_REG_LCR_DLS_7BITS  				((uint8_t)0x02)
#define UART_REG_LCR_DLS_8BITS  				((uint8_t)0x03)
#define UART_REF_FCR_TX_EMPTY_00				((uint8_t)0x00)
#define UART_REF_FCR_TX_EMPTY_01				((uint8_t)0x10)
#define UART_REF_FCR_TX_EMPTY_10				((uint8_t)0x20)
#define UART_REF_FCR_TX_EMPTY_11				((uint8_t)0x30)
#define UART_REF_FCR_RX_EMPTY_00				((uint8_t)0x00)
#define UART_REF_FCR_RX_EMPTY_01				((uint8_t)0x40)
#define UART_REF_FCR_RX_EMPTY_10				((uint8_t)0x80)
#define UART_REF_FCR_RX_EMPTY_11				((uint8_t)0xC0)
 /*
Number of stop bits.
 */ 
#define UART_REG_LCR_STOP_1BITS  ((uint8_t)0x00)
#define UART_REG_LCR_STOP_2BITS  ((uint8_t)0x04)
#define UART_REG_LCR_STOP_1P5BITS  ((uint8_t)0x04)
 /*
Parity Enable.
 */ 
#define UART_REG_LCR_PEN   (3)
#define UART_REG_LCR_PEN_MASK  (0x1 << UART_REG_LCR_PEN)
 /*
Even Parity Select. 
 */ 
#define UART_REG_LCR_EPS   (4)
#define UART_REG_LCR_EPS_MASK  (0x1 << UART_REG_LCR_EPS)
 /*
Stick Parity.  
 */
#define UART_REG_LCR_STICK_PARITY   (5)
#define UART_REG_LCR_STICK_PARITY_MASK  (0x1 << UART_REG_LCR_STICK_PARITY)
 /*
Break Control Bit. 
 */
#define UART_REG_LCR_BREAK   (6)
#define UART_REG_LCR_BREAK_MASK  (0x1 << UART_REG_LCR_BREAK)
 /*
Divisor Latch Access Bit.
 */ 
#define UART_REG_LCR_DLAB   (7)
#define UART_REG_LCR_DLAB_MASK  (0x1 << UART_REG_LCR_DLAB)
 
 
 
 
/*******************  Bit definition for UART_REG_MCR register  ********************/
 /*
Data Terminal Ready. 
 */
#define UART_REG_MCR_DTR   (0)
#define UART_REG_MCR_DTR_MASK  (0x1 << UART_REG_MCR_DTR)
 /*
Request to Send. 
 */ 
#define UART_REG_MCR_RTS   (1)
#define UART_REG_MCR_RTS_MASK  (0x1 << UART_REG_MCR_RTS)
 /*
out1. 
 */ 
#define UART_REG_MCR_OUT1   (2)
#define UART_REG_MCR_OUT1_MASK  (0x1 << UART_REG_MCR_OUT1)
 /*
out2. 
 */ 
#define UART_REG_MCR_OUT2   (3)
#define UART_REG_MCR_OUT2_MASK  (0x1 << UART_REG_MCR_OUT2)
 /*
LoopBack Bit.  
 */
#define UART_REG_MCR_LoopBack   (4)
#define UART_REG_MCR_LoopBack_MASK  (0x1 << UART_REG_MCR_LoopBack)
 /*
Auto Flow Control Enable. 
 */
#define UART_REG_MCR_ACFE   (5)
#define UART_REG_MCR_ACFE_MASK  (0x1 << UART_REG_MCR_ACFE)
 /*
SIR Mode Enable. 
 */ 
#define UART_REG_MCR_SIRE   (6)
#define UART_REG_MCR_SIRE_MASK  (0x1 << UART_REG_MCR_SIRE)

 
 
 
/*******************  Bit definition for UART_REG_LSR register  ********************/
 /*
Data Ready bit.  
 */
#define UART_REG_LSR_DR   (0)
#define UART_REG_LSR_DR_MASK  (0x1 << UART_REG_LSR_DR)
 /*
Overrun error bit. 
 */ 
#define UART_REG_LSR_OE   (1)
#define UART_REG_LSR_OE_MASK  (0x1 << UART_REG_LSR_OE)
 /*
Parity Error bit.
 */ 
#define UART_REG_LSR_PE   (2)
#define UART_REG_LSR_PE_MASK  (0x1 << UART_REG_LSR_PE)
 /*
Framing Error bit.
 */ 
#define UART_REG_LSR_FE   (3)
#define UART_REG_LSR_FE_MASK  (0x1 << UART_REG_LSR_FE)
 /*
Break Interrupt bit.
 */
#define UART_REG_LSR_BI   (4)
#define UART_REG_LSR_BI_MASK  (0x1 << UART_REG_LSR_BI)
 /*
Transmit Holding Register Empty bit.
 */
#define UART_REG_LSR_THRE   (5)
#define UART_REG_LSR_THRE_MASK  (0x1 << UART_REG_LSR_THRE)
#define UART_REG_LSR_THRE_offset_MASK  (0x0 << UART_REG_LSR_THRE)
 /*
Transmitter Empty bit.
 */ 
#define UART_REG_LSR_TEMT   (6)
#define UART_REG_LSR_TEMT_MASK  (0x1 << UART_REG_LSR_TEMT)
 /*
Receiver FIFO Error bit.
 */ 
#define UART_REG_LSR_RFE   (7)
#define UART_REG_LSR_RFE_MASK  (0x1 << UART_REG_LSR_RFE)
/*******************  Bit definition for UART_REG_MSR register  ********************/
/*******************  Bit definition for UART_REG_SCR register  ********************/
/*******************  Bit definition for UART_REG_FAR register  ********************/
#define UART_REG_FAR_FIFOAR   (0)
#define UART_REG_FAR_FIFOAR_MASK  (0x1 << UART_REG_FAR_FIFOAR) 
/*******************  Bit definition for UART_REG_TFR register  ********************/
/*******************  Bit definition for UART_REG_RFW register  ********************/
/*******************  Bit definition for UART_REG_USR register  ********************/
#define UART_REG_HTX_Halt_tx   BIT(0)
/*
 Framing Error bit.
  */ 
 #define UART_REG_USR_BUSY   (16)
 #define UART_REG_USR_BUSY_MASK  (0x7 << UART_REG_USR_BUSY)
  /*
 Break Interrupt bit.
  */
#define UART_REG_USR_TFNF   (16)
#define UART_REG_USR_TFNF_MASK  (0x7 << UART_REG_USR_TFNF)
  /*
 Transmit Holding Register Empty bit.
  */
#define UART_REG_USR_TFE   (16)
#define UART_REG_USR_TFE_MASK  (0x7 << UART_REG_USR_TFE)
  /*
 Transmitter Empty bit.
  */ 
#define UART_REG_USR_RFNE   (16)
#define UART_REG_USR_RFNE_MASK  (0x7 << UART_REG_USR_RFNE)
  /*
 Receiver FIFO Error bit.
  */ 
#define UART_REG_USR_RFF   (16)
#define UART_REG_USR_RFF_MASK  (0x7 << UART_REG_USR_RFF)
/*******************  Bit definition for UART_REG_TFL register  ********************/
/*******************  Bit definition for UART_REG_RFL register  ********************/
/*******************  Bit definition for UART_REG_CPR register  ********************/
 
typedef enum AL_uart_stop_bit {
    AL_UART_STOP_BIT_1 = 0,
    AL_UART_STOP_BIT_1p5 = 1,
	AL_UART_STOP_BIT_2 = 2
} AL_UART_STOP_BIT;

typedef enum AL_uart_pariy_bit {
    UART_EVEN_BIT = 0,
    UART_ODD_BIT = 1,
} AL_UART_PARITY_BIT;


typedef enum AL_uart_bit_length {
    UART_BIT_LENGTH_5 = 0 ,
    UART_BIT_LENGTH_6 = 1 ,
    UART_BIT_LENGTH_7 = 2 ,
    UART_BIT_LENGTH_8 = 3 ,
} AL_UART_BIT_LENGTH;
enum AL_uart_tx_trigger{
	FIFO_empty,
	characters_in_the_FIFO_is_2,
	FIFO_1_4_full,
	FIFO_1_2full
};
enum AL_uart_event{
	Uart_event_modem = 0,
	Uart_event_none	 = 1,
	Uart_event_thre  = 2,
	Received_data_available = 4,
	Receiver_line_status = 6,
	Uart_event_busy		=7,
	Uart_event_timeout	=12,
};
enum AL_uart_rx_trigger{
	one_character_in_the_FIFO,
	RX_FIFO_1_4_full,
	RX_FIFO_1_2full,
	FIFO_2_less_than_full
};
/****d* uart.data/dw_uart_irq
 * DESCRIPTION
 *  These are the bit definitions used for managing UART interrupts.
 *  The functionality of ETBEI and ERBFI alters when programmable THRE
 *  interrupt mode is active (dw_uart_enablePtime()).  See the
 *  DW_apb_uart databook for a detailed description.
 * NOTES
 *  This data type relates to the following register bit field(s):
 *   - ier/erbfi
 *   - ier/etbei
 *   - ier/elsi
 *   - ier/edssi
 * SEE ALSO
 *  dw_uart_enableIrq(), dw_uart_disableIrq(), dw_uart_isIrqEnabled()
 * SOURCE
 */
enum AL_uart_irq {
    Uart_irq_erbfi 	= 0x01,      // receive data available
    Uart_irq_etbei 	= 0x02,      // transmitter holding register empty
    Uart_irq_elsi 	= 0x04,       // receiver line status
    Uart_irq_edssi 	= 0x08,      // modem status
    Uart_irq_all 	= 0x0f         // all interrupts
};
enum AL_dma_mode{
	TX_DMA_MODE = 1,
	RX_DMA_MODE = 2,
	TX_RX_DMA_MODE = 3
};

int32_t AlUart_Init(AL_UART_TypeDef *uart, uint32_t baudrate,AL_UART_BIT_LENGTH bit_length, AL_UART_STOP_BIT stopbit);
uint8_t AlUart_SetBaudrate(AL_UART_TypeDef *uart, uint32_t baudrate);
uint8_t AlUart_DatalengConfig(AL_UART_TypeDef *uart,AL_UART_BIT_LENGTH bit_length);
int32_t AlUart_ConfigStopbit(AL_UART_TypeDef *uart, AL_UART_STOP_BIT stopbit);
uint8_t AlUart_FifoEnable(AL_UART_TypeDef *uart);
int32_t uart_write(AL_UART_TypeDef *uart, volatile uint8_t val);
uint8_t uart_read(AL_UART_TypeDef *uart);
uint8_t uart_ReadirqEnable(AL_UART_TypeDef *uart);
void 	AlUart_ResetFifo(AL_UART_TypeDef *uart);
void 	AlUart_SetParity(AL_UART_TypeDef *uart);
int 	AlUart_EnablePtime(AL_UART_TypeDef *uart);
int 	AlUart_DisablePtime(AL_UART_TypeDef *uart);
void 	AlUart_EnableLoopback(AL_UART_TypeDef *uart);
void 	AlUart_DisableLoopback(AL_UART_TypeDef *uart);
int 	AlUart_EnableAfc(AL_UART_TypeDef *uart);
int 	AlUart_DisableAfc(AL_UART_TypeDef *uart);
void 	AlUart_SetDmaMode(AL_UART_TypeDef *uart);
void 	AlUart_EnableIrq(AL_UART_TypeDef *uart, enum AL_uart_irq interrupts);
void 	AlUart_DisableIrq(AL_UART_TypeDef *uart,enum AL_uart_irq interrupts);
void 	AlUart_SetTxTrigger(AL_UART_TypeDef *uart, enum AL_uart_tx_trigger trigger);
void 	AlUart_SetRxTrigger(AL_UART_TypeDef *uart, enum AL_uart_rx_trigger trigger);
int 	AlUart_UserIrqHandler(AL_UART_TypeDef *uart);
int 	AlUart_EnableFifoAccess(AL_UART_TypeDef *uart);
int 	AlUart_ResumeTx(AL_UART_TypeDef *uart);

#ifdef __cplusplus
}
#endif
#endif /* _HBIRD_UART_H */


