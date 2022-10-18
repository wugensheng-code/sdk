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
#define DRR_BASE_1   (0x10001000)
#define CFG_CTRL_OCM  0xF88001BC
#define OCM_ADDRESS   0x61021030
#define INITVALUE     (0x88888888)
#define MODIFIEDVALUE (0x88888880)
int count = 16384;

void TestOcm1BitEcc()
{
    volatile uint32_t *  ptr_1step = (volatile uint32_t *)(OCM_ADDRESS);
    volatile uint32_t *  ptr_2step = (volatile uint32_t *)(OCM_ADDRESS);
    volatile uint32_t *  ptr_3step = (volatile uint32_t *)(OCM_ADDRESS);
    *((volatile uint32_t *)(CFG_CTRL_OCM)) = 0x1;
    for (int i = 0; i < 1000; i++)
    {
        *(ptr_1step++) = INITVALUE;
    }
    *((volatile uint32_t *)(CFG_CTRL_OCM)) = 0x0;
    for (int i = 0; i < 1000; i++)
    {
        *(ptr_2step++) = MODIFIEDVALUE;
    }
    *((volatile uint32_t *)(CFG_CTRL_OCM)) = 0x1;

    for (int i = 0; i < 1000; i++)
    {
        int value = *(ptr_3step++);
        printf("value = %08x\n", value);
    }

}

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
    TestOcm1BitEcc();
	return 0;
}



