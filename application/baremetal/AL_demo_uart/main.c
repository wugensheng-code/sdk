//uart_write
#include <stdio.h>
#include <stdlib.h>
#include "nuclei_sdk_soc.h"
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
for(int i = 0 ; i < 5 ; i++)
{
	printf("good_job\r\n");
}
	return 0; 
}
