//uart_write
#include <stdio.h>
#include <stdlib.h>
#include "nuclei_sdk_soc.h"
#include "AL_uart.h"
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
	uint8_t buffer_485data[8];
	for(int i = 0 ; i < 8 ; i++){
		printf("good_job\r\n");
		buffer_485data[i] = i+1;
	}
	AlUart_Init(AL_UART1,9600,UART_BIT_LENGTH_8, AL_UART_STOP_BIT_1);
	AlUar_485TxEnable(AL_UART1);
	AlUart_485Type(AL_UART1);
	//printf("read_data = %08x\r\n",(*(uint32_t*)AL_UART0_BASE));
	printf("485 test");
	AlUart_Enable485(AL_UART1);
	while(1){
	for(int i = 0 ; i < 8 ; i++)
	{
		uart_write(AL_UART1, buffer_485data[i]);
		//uart_read(AL_UART0);
		printf("read_data = %08x\r\n",uart_read(AL_UART0));
		//printf("good_job\r\n");
	}
	}
	return 0; 
}
