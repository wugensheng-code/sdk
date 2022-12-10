/*
 * main.c
 *
 *  Created on: 2021年11月10日
 *      Author: wei.pang
 */

#include <stdio.h>
#include <stdlib.h>
#include "nuclei_sdk_soc.h"
#include "AL_uart.h"
#include "AL_dmac.h"
#include <time.h>
/*
 *instruction
 *test_mode		:test ddr  mode
 *importannt parameter	:
 *
 *
 *
*/
#define DRR_BASE_1 (0x)
int count = 16384;
int main(void){
	//__RV_CSR_CLEAR(CSR_MMISC_CTL,MMISC_CTL_BPU);
	uint32_t test_buffer[count];
	for(volatile int i = 0; i < count ; i ++ ){
		test_buffer[i] = i;
	}
	write_To_OCM((uint32_t*)test_buffer,count,(uint32_t*)DDR_BASE);
	for(volatile int j = 0 ; j < count ; j ++ ){
			if(test_buffer[j] == (*(uint32_t*)(DDR_BASE + j*4)))
			{
				printf("test_bueferdata = %d\r\n",test_buffer[j]);
				printf("DDR_data = %d\r\n",(*(uint32_t*)(DDR_BASE + j*4)));
				printf("check data is right\r\n");
				}
			else{
				printf("check data is fail please check you dma set data\r\n");
				while(1){
					printf("check data is fail please check you dma set data\r\n");
				};
			}
				printf("\r\n");
		}
	return 0;
}



