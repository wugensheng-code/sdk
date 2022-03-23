//uart_write
#include <stdio.h>
#include <stdlib.h>
#include "nuclei_sdk_soc.h"
#include "al9000_uart.h"
//#include "anl_printf.h"
#include <time.h>
/*
 *baudrate:115200
 *datebits:8
 *stopbits:2
 *fifomode:enable
 *function:uart_wrire
 *mode	  :autoflow
*/




int main(void){
//	AL9000_uart_init(AL9000_UART0,115200,UART_BIT_LENGTH_8);
//	AL9000_uart_config_stopbit(AL9000_UART0,AL9000_UART_STOP_BIT_1);
//	AL9000_uart_fifo_enable(AL9000_UART0);
for(int i = 0 ; i < 5 ; i++)
{
	printf("good_job\r\n");
}
	return 0; 
}
