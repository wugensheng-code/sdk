//uart_write
#include <stdio.h>
#include <stdlib.h>
#include "nuclei_sdk_soc.h"
#include "al9000_uart.h"
//#include "anl_printf.h"
#include <time.h>
//#include "al9000_sdio_sd.h"
#include "sdio_test.h"
/*
 *baudrate:115200
 *datebits:8
 *stopbits:2
 *fifomode:enable
 *function:uart_wrire
 *test_mode:printf read_irq	  :
*/


void SOC_INT89_IRQn_handler(void)
{
    printf("eq!\n\r"); 
    uart_read(AL9000_UART0);
}


int main(void){
#if 1
   ECLIC_Register_IRQ(SOC_INT89_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,SOC_INT89_IRQn_handler);
   __enable_irq();
   uart_readirq_enable(AL9000_UART0);
	for(int i = 1 ; i < 100 ; i++){ 
		uart_write_rfw(AL9000_UART0,i);
	}
#endif
	SD_Test();
	return 0; 
}
