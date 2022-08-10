/*
 * main.c
 *
 *  Created on: 2021年12月17日
 *      Author: wei.pang
 */

#include <stdio.h>
#include <stdlib.h>
#include "nuclei_sdk_soc.h"
#include "AL_uart.h"
#include "AL_can.h"
#include <time.h>

uint8_t tx_buf1[64];
uint8_t tx_buf2[64];
uint8_t tx_buf3[64];
uint8_t tx_buf4[64];
uint8_t tx_buf5[64];
uint8_t rx_buf1[64];
uint8_t tx_buf5[64];
uint8_t rx_buf2[64];
// See LICENSE for license details.
#define mtimer_irq_handler     eclic_mtip_handler
#define mtimer_sw_irq_handler  eclic_msip_handler

static uint32_t int0_cnt = 0;    /* msip timer interrupt test counter */
static uint32_t int1_cnt = 0;    /* mtip timer interrupt test counter */
unsigned int msip_trig_flag = 1; /* sw trigger mtimer sw interrupt flag */

AL_CAN_TypeDef *AL_CAN_CUR = AL_CAN1;

void wait_seconds(size_t n)
{
    uint64_t start_mtime, delta_mtime;

    uint64_t tmp = SysTimer_GetLoadValue();
    do {
        start_mtime = SysTimer_GetLoadValue();
    } while (start_mtime == tmp);

    do {
        delta_mtime = SysTimer_GetLoadValue() - start_mtime;
    } while (delta_mtime < (n * SOC_TIMER_FREQ));
}

void mtimer_irq_handler(void)
{
    int0_cnt++;
    uint32_t data_length;
    wait_seconds(1);
    printf("MTimer IRQ handler %d\n\r", int0_cnt);
    uint64_t now = SysTimer_GetLoadValue();
    SysTimer_SetCompareValue(now + 0.5 * SOC_TIMER_FREQ);
    data_length =AlCan_ReceiveMsg(AL_CAN_CUR,rx_buf1,data_length_20);
    if(int0_cnt < 100 && data_length !=0){
    	int0_cnt = 0;
    }
    else if(int0_cnt > 100 ){
    	printf("Can receive data timeout \r\n");
    }
    else{
    	printf("Can receive data waiting ...\r\n");
    }
}

void mtimer_sw_irq_handler(void)
{
    SysTimer_ClearSWIRQ();
    int1_cnt++;
    printf("MTimer SW IRQ handler %d\n\r", int1_cnt);
    msip_trig_flag = 1;
}

void setup_timer()
{
    printf("init timer and start\n\r");
    uint64_t now = SysTimer_GetLoadValue();
    printf("now value = %08x\r\n",now);
    uint64_t then = now + 0.5 * SOC_TIMER_FREQ;
    SysTimer_SetCompareValue(then);
}



void SOC_CAN0_HANDLER(void)
{
	uint8_t count = 0;
	count++;
	//CAN_TX_COMPLETE(AL_CAN0);
	//Can_Receive_Msg(AL_CAN0,rx_buf1,data_length_20);
}

void SOC_CAN1_HANDLER(void)
{
	if(((AL_CAN_CUR->RTIE_RTIF_ERRINT_LIMIT & (1 << 15)) >> 15) == 1){
		printf("cur register is %x!\r\n", AL_CAN_CUR->RTIE_RTIF_ERRINT_LIMIT);
		printf("rcv msg!\r\n");
		AlCan_ReceiveMsg(AL_CAN_CUR,rx_buf1,data_length_20);
		//AlCan_Rx(AL_CAN_CUR,rx_buf1,data_length_20);
		AL_CAN_CUR->RTIE_RTIF_ERRINT_LIMIT |= (0x1 << 15);
		printf("cur register is %x!\r\n", AL_CAN_CUR->RTIE_RTIF_ERRINT_LIMIT);
	}else{
		printf("other int!\r\n");
		printf("cur register is %x!\r\n", AL_CAN_CUR->RTIE_RTIF_ERRINT_LIMIT);
	}
}

int main(){
	/*
	uint8_t tx_buf1[64];
	uint8_t rx_buf1[20];
	uint8_t tx_buf2[64];
	uint8_t rx_buf2[20];
	*/
	for (volatile uint8_t i = 0 ;i < 64 ; i++)
	{
		tx_buf1[i]=i;
		tx_buf2[i]=i+1;
		tx_buf3[i]=i+2;
		tx_buf4[i]=i+3;
		tx_buf5[i]=i+4;
	}
	//__RV_CSR_CLEAR(CSR_MMISC_CTL,MMISC_CTL_BPU);
    ECLIC_Register_IRQ(SOC_INT92_IRQn , ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,SOC_CAN1_HANDLER);
	*(uint32_t *)(0xf8803020) = 0x1;
	*(uint32_t *)(0xf8803024) = 0x1;
	*(uint32_t *)(0xf880340c) = 0x1;
	//ECLIC_Register_IRQ(\
	                     SysTimer_IRQn, ECLIC_NON_VECTOR_INTERRUPT, ECLIC_LEVEL_TRIGGER, 1, 0,\
	                     mtimer_irq_handler); /* register system timer interrupt */
	__enable_irq();
	Enablepinmux1();
	//setup_timer(); 	/* initialize timer */
	//setup_timer(); 		/* initialize timer */
	AlCan_SetResetMode(AL_CAN_CUR);
	AlCan_InterruptEnable(AL_CAN_CUR, RIE);
	AlCan_DeviceDriverBittimeConfiguration(AL_CAN_CUR,rate_5Mbit,AL_TOP0,can_fd);
	AlCan_TxMode(AL_CAN_CUR,NORMAL);
#if 0
	while(1){
		//setup_timer(); /* initialize timer */
	//for(uint8_t i = 0 ; i < 50 ; i++){
		//delay(1280);
		AlCan_SendMsg(AL_CAN_CUR,tx_buf1,XMIT_PTB_MODE,data_length_12,0x11);
		//delay(1200);
		AlCan_SendMsg(AL_CAN_CUR,tx_buf2,XMIT_PTB_MODE,data_length_20,0x12);
		//delay(1200);
		AlCan_SendMsg(AL_CAN_CUR,tx_buf3,XMIT_PTB_MODE,data_length_24,0x13);
		//delay(1200);
		AlCan_SendMsg(AL_CAN_CUR,tx_buf5,XMIT_PTB_MODE,data_length_32,0x14);
		//delay(1200);
		AlCan_SendMsg(AL_CAN_CUR,tx_buf4,XMIT_PTB_MODE,data_length_48,0x15);
		//delay(1200);
		AlCan_SendMsg(AL_CAN_CUR,tx_buf5,XMIT_PTB_MODE,data_length_16,0x16);
		//delay(1200);
		AlCan_SendMsg(AL_CAN_CUR,tx_buf5,XMIT_PTB_MODE,data_length_64,0x17);
	//}
		AlCan_TestError(AL_CAN_CUR);//if you want to get some debug error information,you can open this
	}
#endif
#if 1
	while(1){
		//AlCan_ReceiveMsg(AL_CAN_CUR,rx_buf1,data_length_20);
	}
#endif

		return 0;
}
