/*
 * AL_UART.C
 *
 *  Created on: 2021年9月17日
 *      Author: wei.pang
 */

#include "demosoc.h"
#include "stdlib.h"
#include "nuclei_sdk_soc.h"
#define uart_clock (40000000UL)
/*!
    \brief  uart initialize
    \param  uart: uart parameter stuct
    \param  baudrate: uart buadrate
    \param  bit_length: bit length (5/6/7/8)
    \retval 0,if uart!=null; otherwise -1;
*/
int32_t AlUart_Init(AL_UART_TypeDef *uart, uint32_t baudrate,AL_UART_BIT_LENGTH bit_length, AL_UART_STOP_BIT stopbit)
{
    if (__RARELY(uart == NULL)) {
        return -1;
    }
    setvbuf(stdout, NULL, _IONBF, 0);
    //AlUart_enablePtime(uart);					//enable programmable THR empty mode
    AlUart_SetBaudrate(uart, baudrate);		//set_baudrate
    AlUart_DatalengConfig(uart,bit_length);	//set_dataleng
    AlUart_ConfigStopbit(uart,stopbit);		//set_stopbit
    AlUart_FifoEnable(uart);
    return 0;
}
uint8_t AlUart_SetBaudrate(AL_UART_TypeDef *uart, uint32_t baudrate){
    uart->LCR 		 |= UART_REG_LCR_DLAB_MASK ;
    uart->DLH_IER 	  = ((uint16_t)(uart_clock / (baudrate*16))) >> 8 ;
    uart->RBR_THR_DLL = ((uint8_t)(uart_clock / (baudrate*16))) ;
    uart->MCR 		 |= UART_REG_MCR_DTR_MASK;
    uart->MCR 		 |= UART_REG_MCR_RTS_MASK;
    uart->LCR 		 &= (uint8_t)(0x7f);
    return 0;
}
uint8_t AlUart_DatalengConfig(AL_UART_TypeDef *uart,AL_UART_BIT_LENGTH bit_length){
	switch (bit_length)
	    {
	    case 0:
	    	uart->LCR |= UART_REG_LCR_DLS_5BITS;//(bit_length<<UART_REG_LCR_DLS) & UART_REG_LCR_DLS_MASK;
	    	break;
	    case 1:
	    	uart->LCR |= UART_REG_LCR_DLS_6BITS;//(bit_length<<UART_REG_LCR_DLS) & UART_REG_LCR_DLS_MASK;
	    	break;
	    case 2:
	        uart->LCR |= UART_REG_LCR_DLS_7BITS;//(bit_length<<UART_REG_LCR_DLS) & UART_REG_LCR_DLS_MASK;
	        break;
	    default:
	    	uart->LCR |= UART_REG_LCR_DLS_8BITS;
	    	break;
	    }
	return 0;
}
/*!
    \brief  uart stop bit config
    \param  uart: uart parameter stuct
    \param  stopbit: stop bit (1/2/0.5/1.5)
    \retval 0,if uart!=null; otherwise -1;
*/
int32_t AlUart_ConfigStopbit(AL_UART_TypeDef *uart, AL_UART_STOP_BIT stopbit)
{
   if (__RARELY(uart == NULL)) {
        return -1;
    }
   switch(stopbit)
      {
      case 0:
   	   uart->LCR |=  UART_REG_LCR_STOP_1BITS;
   	   break;
      case 1:
   	   uart->LCR &= ~UART_REG_LCR_DLS_5BITS;
   	   uart->LCR |=  UART_REG_LCR_STOP_2BITS;
   	   break;
      default:
   	   uart->LCR |=  UART_REG_LCR_STOP_2BITS;
   	   break;
      }
    return 0;
}
/*!
    \brief  uart FIFOF enable
    \param  uart: uart parameter stuct
    \retval FCR:  value
*/
uint8_t AlUart_FifoEnable(AL_UART_TypeDef *uart)
{

    if (__RARELY(uart == NULL)) {
        return -1;
    }
    uart ->IIR_FCR = UART_REG_FCR_FIFOE_MASK;
    return 0;
}
/*!
    \brief  uart TXFIFO transmitte
    \param  uart: uart parameter stuct
    \param  val: value of TXFIFO
    \retval 0,if uart!=null; otherwise -1;
*/
int32_t uart_write(AL_UART_TypeDef *uart, volatile uint8_t val)
{
   if (__RARELY(uart == NULL)) {
        return -1;
    }
	   while (0==(uart->LSR & 0x20));
	   uart ->RBR_THR_DLL = val & 0xFF ;
    return 0;
}
int32_t uart_dmac_write(AL_UART_TypeDef *uart, volatile uint8_t val)
{
   if (__RARELY(uart == NULL)) {
        return -1;
    }
	   while (0 == (uart->LSR & 0x20));
	   uart ->RBR_THR_DLL = val & 0xFF ;
    return 0;
}
/*!
    \brief  uart RXFIFO receive
    \param  uart: uart parameter stuct
    \retval RXFIFO value
*/
uint8_t uart_read(AL_UART_TypeDef *uart)
{
    uint32_t reg;
    if (__RARELY(uart == NULL)) {
        return -1;
    }
    while ( 0 == ((uart ->LSR) & 0x01));
    reg = uart ->RBR_THR_DLL;
    //printf("reg_interrupt %.2x\r\n",reg);
    return (uint8_t)(reg & 0xFF);
}
#if 0
uint8_t uart_read(AL_UART_TypeDef *uart)
{
    uint32_t reg;
    uint8_t rx_buffer[64];
    uint8_t i = 0;
    uint8_t count = 0;
    count++;
    uint32_t timeout = 0xffff;
    if (__RARELY(uart == NULL)) {
        return -1;
    }
    //Wait receive data untill timeout
    //if timeout uart_receive error
    while ( 0 == ((uart ->LSR) & 0x01))/*{
    	timeout --;
    	if(timeout < 0) {
    	        printf("\n[irq] timed out waiting to receive string\n");
    	    }
    	    else {
    	    	printf("\n[irq] uart2 received the following string:\n");
    	    	while(1);
    	    }
    };*/
    /*
     * uart rx filter
     */
    /*for(i = 0 ; i < 64 ; i++){
    	rx_buffer[i] = uart->RBR_THR_DLL;
    	}
    for(i = 0 ; i < 64 ; i++){
    	reg += rx_buffer[i];
    }
    reg = reg/64;
    printf("tx_ocm_data = %.2x\r\n",reg);*/
    return (uint8_t)(reg & 0xFF);
}
#endif
uint8_t uart_ReadirqEnable(AL_UART_TypeDef *uart){
    if (__RARELY(uart == NULL)) {
        return -1;
    }
    uart ->DLH_IER  |=UART_REG_IER_ERBFI_MASK;
    return 0;
}
void AlUart_ResetFifo(AL_UART_TypeDef *uart)
{
		uart ->IIR_FCR &=~UART_REG_FCR_XFIFOR_MASK;
		uart ->IIR_FCR &=~UART_REG_FCR_RFIFOR_MASK;
}
/**********************************************************************/
void AlUart_SetParity(AL_UART_TypeDef *uart)
{
	if(GET_BIT(uart ->LCR,3) == 0)
	{
		uart ->LCR |= UART_REG_LCR_PEN_MASK;
	}
}
/**********************************************************************/

int AlUart_EnablePtime(AL_UART_TypeDef *uart) //enable THRE Mode
{
	if (__RARELY(uart == NULL)) {
	        return -1;
	    }
	uart ->DLH_IER |=UART_REG_IER_PTIME_MASK;
    return 0;
}

/**********************************************************************/

int AlUart_DisablePtime(AL_UART_TypeDef *uart)
{
    if(GET_BIT(uart ->DLH_IER,7) == 1){
    	uart ->DLH_IER &=~UART_REG_IER_PTIME_MASK;
        }
    else{
    	printf("thre mode is disable");
    }

    return 0;
}

/**********************************************************************/
/*
 * This is used to cause a break condition to be transmitted to the
 * receiving device. If set to 1, the serial output is forced to the spacing (logic 0) state.
 * When not in Loopback Mode, as determined by MCR[4], the sout line is forced low
 * until the Break bit is cleared.
 */
/*
void AL_uart_setBreak(AL_UART_TypeDef *uart, enum AL_state state)
{
	if(GET_BIT(uart ->LCR,6) == 0){
		uart ->LCR |=UART_REG_LCR_BREAK_MASK;
	}
}*/
/**********************************************************************/
/*This is used to put the UART into a diagnostic mode for test purposes.
 *If operating in UART mode (SIR_MODE != Enabled or not active, MCR[6] set to 0),
 *data on the sout line is held high, while serial data output is looped back to the sin
 *line, internally. In this mode all the interrupts are fully functional. Also, in loopback
 *mode, the modem control inputs (dsr_n, cts_n, ri_n, dcd_n) are disconnected and the
 *modem control outputs (dtr_n, rts_n, out1_n, out2_n) are looped back to the inputs,
 *internally.
*/
void AlUart_EnableLoopback(AL_UART_TypeDef *uart)
{
    if(GET_BIT(uart ->MCR,5) == 0) {
    	uart ->MCR |=UART_REG_MCR_LoopBack_MASK;
    }
}

/**********************************************************************/

void AlUart_DisableLoopback(AL_UART_TypeDef *uart)
{
    if(GET_BIT(uart ->MCR,5) == 1) {
    	uart ->MCR &=~UART_REG_MCR_LoopBack_MASK;
    }
}
/**********************************************************************/

int AlUart_EnableAfc(AL_UART_TypeDef *uart)
{
    if(GET_BIT(uart ->MCR,5) == 0) {
    	uart ->MCR |=UART_REG_MCR_ACFE_MASK;
    }
    return 0;
}

/**********************************************************************/

int AlUart_DisableAfc(AL_UART_TypeDef *uart)
{
	 if(GET_BIT(uart ->MCR,5) == 1) {
	    	uart ->MCR &=~UART_REG_MCR_ACFE_MASK;
	    }
	    return 0;
}

/**********************************************************************/

void AlUart_SetDmaMode(AL_UART_TypeDef *uart)
{


    // update stored FCR value
	if(GET_BIT(uart ->IIR_FCR,3)==0){
		uart ->IIR_FCR |= UART_REG_FCR_DMAM_MASK;
	}
}

/**********************************************************************/

void AlUart_EnableIrq(AL_UART_TypeDef *uart, enum AL_uart_irq interrupts)
{
	uint8_t reg = 0;

    // get current interrupt enable settings
    reg = GET_BITS(uart ->DLH_IER,0,7);
    // set interrupts to be enabled
    reg |= interrupts;
    // update copy of IER value (used when avoiding shared data issues)
    uart ->DLH_IER |=reg;
}

/**********************************************************************/

void AlUart_DisableIrq(AL_UART_TypeDef *uart,enum AL_uart_irq interrupts)
{
	uint8_t reg = 0;
	    // get current interrupt enable settings
	    reg = GET_BITS(uart ->DLH_IER,0,7);
	    // set interrupts to be enabled
	    reg |= interrupts;
	    printf("reg_interrupt %.2x\r\n",reg);
	    // update copy of IER value (used when avoiding shared data issues)
	    uart ->DLH_IER &=~reg;
}
/**********************************************************************/

void AlUart_SetTxTrigger(AL_UART_TypeDef *uart, enum AL_uart_tx_trigger trigger)
{
	switch(trigger){
	case FIFO_empty:
		uart ->IIR_FCR |=UART_REF_FCR_TX_EMPTY_00;
		break;
	case characters_in_the_FIFO_is_2:
		uart ->IIR_FCR |=UART_REF_FCR_TX_EMPTY_01;
		break;
	case FIFO_1_4_full:
		uart ->IIR_FCR |=UART_REF_FCR_TX_EMPTY_10;
		break;
	case FIFO_1_2full:
		uart ->IIR_FCR |=UART_REF_FCR_TX_EMPTY_11;
		break;
	default:
		uart ->IIR_FCR |=UART_REF_FCR_TX_EMPTY_11;
		break;
	}
}


void AlUart_SetRxTrigger(AL_UART_TypeDef *uart, enum AL_uart_rx_trigger trigger)
{
	switch(trigger){
	case one_character_in_the_FIFO:
		uart ->IIR_FCR |=UART_REF_FCR_RX_EMPTY_00;
		break;
	case RX_FIFO_1_4_full:
		uart ->IIR_FCR |=UART_REF_FCR_RX_EMPTY_01;
		break;
	case RX_FIFO_1_2full:
		uart ->IIR_FCR |=UART_REF_FCR_RX_EMPTY_10;
		break;
	case FIFO_2_less_than_full:
		uart ->IIR_FCR |=UART_REF_FCR_RX_EMPTY_11;
		break;
	default:
		uart ->IIR_FCR |=UART_REF_FCR_RX_EMPTY_11;
		break;
	}
}

/**********************************************************************/

int AlUart_UserIrqHandler(AL_UART_TypeDef *uart)
{
    int retval;
    int callbackArg;
    uint32_t reg;
    enum AL_uart_event event;
    callbackArg = 0;

    // what caused this interrupt
    reg = GET_BITS(uart ->IIR_FCR,0,3);
    event = (enum AL_uart_event) (reg & 0xf);
    // assume an interrupt will be processed
    retval = TRUE;

    // process event
    switch(event) {
        case Uart_event_none:
            // no interrupt has occurred
            retval = FALSE;
            retval = Uart_event_none;
            break;
        case Uart_event_modem:
            printf("Clear to send or data set ready or ring indicator or data carrier detect");
            retval = Uart_event_modem;
            break;
        case Uart_event_thre:
        	printf("Transmitter holding register empty or XMIT FIFO at or below threshold");
        	retval =Uart_event_thre;
        	break;
        case Uart_event_timeout:
        	printf("No characters in or out of the RCVR FIFO during the last 4 character times and there is at least 1 character in it during this time");
        	retval = Uart_event_timeout;
        	break;
        case Received_data_available:
        	printf("Receiver data available (nonFIFO mode or FIFOs disabled) or RCVR FIFO trigger level reached");
        	retval = Received_data_available;
            break;
        case Receiver_line_status:
        	printf("Overrun/parity/ framing errors or break interrupt");
        	retval = Receiver_line_status;
            break;
        case Uart_event_busy:
        	printf("Transmitter holding register empty or XMIT FIFO at or below threshold");
        	retval = Uart_event_busy;
            break;
        default:
            // If we've reached this point, the value read from the
            // iir_fcr register is unrecognized.
            retval = 0;
            break;
    }

    // If an interrupt has occurred, pass it to the user listener
    // function.
    return retval;
}

/**********************************************************************/

int AlUart_EnableFifoAccess(AL_UART_TypeDef *uart)
{
    int retval;
    if(GET_BIT(uart ->CPR,9) == TRUE) {
    	uart ->FAR |=UART_REG_FAR_FIFOAR_MASK;
    }
    else
    	uart ->FAR &=~UART_REG_FAR_FIFOAR_MASK;
    return retval;
}

/**********************************************************************/

int AlUart_HaltTx(AL_UART_TypeDef *uart)
{
    int retval;
    if(GET_BIT(uart ->CPR,9) == TRUE) {
        	uart ->HTX |=UART_REG_HTX_Halt_tx;
        }
        else{
        	printf("fifo error or fifo not be enable");
        }
    return retval;
}

/**********************************************************************/

int AlUart_ResumeTx(AL_UART_TypeDef *uart)
{
	int retval;
	    if(GET_BIT(uart ->CPR,9) == TRUE) {
	        	uart ->HTX &=~UART_REG_HTX_Halt_tx;
	        }
	        else{
	        	printf("fifo error or fifo not be enable");
	        }
	    return retval;
}



