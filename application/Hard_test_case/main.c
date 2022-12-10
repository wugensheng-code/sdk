/*
 * main.c
 *
 *  Created on: 2022/3/20
 *  Author: pang.wei
 *  Function": demo_per
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "nuclei_sdk_soc.h"
#include "board_nuclei_fpga_eval.h"
#include "../../../SoC/demosoc/Common/Include/demosoc_xec.h"
#include "al9000_test.h"
#include "mtimer.h"
#include "al9000_wdog.h"

#define TEMP_DDR_1 ((volatile uint32_t *)(0X10000000UL+0X100000UL))	//timeoutvalue
#define TEMP_DDR_2 ((volatile uint32_t *)(0X10000000UL+0X100004UL))	//int_count_num
#define TIMEOUT_VAL_S	5
#define TIMEOUT_VAL_E	0xF
#define INT_NUM_S		5
#define INT_NUM_E		0xA

#define TOP_CFG_CTRL_WDT_BASEADDR (0XF8800000UL+0X168UL)
#define WDT_PAUSE_REG ((volatile uint32_t *)TOP_CFG_CTRL_WDT_BASEADDR)

#define PMU_ERR_HW_EN0_SET 	(0XF8806000)
#define PMU_SEL_WDT ((volatile uint32_t *)PMU_ERR_HW_EN0_SET)

#define PMU_INT_EN0_SET 	(0XF8806010)
#define PMU_EN_WDT ((volatile uint32_t *)PMU_INT_EN0_SET)

#define PMU_CLR_WDT_INTR 	(0XF8806030)
#define PMU_CLR_WDT ((volatile uint32_t *)PMU_CLR_WDT_INTR)

#define PMU_SW_PMU_SHACK 	(0XF8806080)
#define PMU_SHACK ((volatile uint32_t *)PMU_SW_PMU_SHACK)

extern u32 EMMC_Test(void);
static void wdt_init(void);


uint32_t main(){
#if 0
	MtimerParams mtimer;
	uint32_t count = 0;
	Mtimer_Init(&mtimer);
	for(;count<5;){
		MTIMER_OUT_CONDITION(1000*1000, &mtimer, 1);
		printf("count is %d\r\n", count++);
	}
	printf("hard_test_case start!\r\n");
	while(1){
		printf("_delay_ms(1000)\r\n");
		_delay_ms(1000);
	}
#endif
	wdt_init(); 
    //EMMC_Test();
	SD_Test();
	//iic_test();
	//qspi_test();
	//spi_test();
	//dma_test();
	//gbe_test();
	//can_test();
	//timer_test();
	//gpio_test();
#if __riscv
	benchmark_test();
#endif

	//plps_test();
	//usb_test();
	
}

/* case 1.2 */
volatile uint32_t interrupt_count=0;
volatile uint32_t int_num = 0;

void wdt_handler(void)
{
	uint32_t read_temp=0;
	
	if(GET_BIT(WDT->STAT,0) == 0x1)
	{
		//printf("WDT->STAT is %x\r\n", WDT->STAT);
		interrupt_count++;
		if(interrupt_count < int_num)
		{
			read_temp = WDT->EOI;	//Can clear by reading WDT_EOI
			//printf("watch irq %d\r\n", interrupt_count);
			printf("l\r\n");
		}
		//wdt->CRR |= WDT_CCVR_VALUE;
	}
}

void pmu_handler(void)
{
	printf("pmu\r\n");
	*PMU_CLR_WDT  = 0x00000100;
}

/**
 * \brief  This function initialize wdt
 * \param  wdt: wdt regs struct
 * \param  WDT_InitTypeDef: contains the configuration for the specified WDT peripheral
 * \retval if wdt==null or WDT_InitStruct==NULL ,return -1 ; otherwise 0;
 */
int AL9000_wdt_init(WDT_TypeDef *wdt,WDT_InitTypeDef *WDT_InitStruct)
{
	if((__RARELY(wdt == NULL))||(__RARELY(WDT_InitStruct == NULL)))
	{
		printf("e1");
		return -1;
	}
	//printf("i");

	volatile unsigned int read_temp = WDT->EOI;

	wdt->TORR &= TORR_CLR_MASK;

	wdt->TORR |= WDT_InitStruct->WDT_TimeOutValue;

	wdt->CR	&= CR_CLR_MASK;

	wdt->CR |= WDT_InitStruct->WDT_PuaseLength;
    wdt->CR |= WDT_InitStruct->WDT_Mode;
    wdt->CR |= WDT_CR_WDT_EN;

	wdt->CRR = WDT_CCVR_VALUE;

	*PMU_SEL_WDT = 1 << 8;
	*PMU_EN_WDT = 1 << 8;
	*PMU_SHACK  = 0xfffffeff;

	(*WDT_PAUSE_REG) &= 0xfffffffe;
}

static void wdt_init(void)
{
	WDT_InitTypeDef WDT_InitStruct;

	if(*TEMP_DDR_1 > TIMEOUT_VAL_E){
		*TEMP_DDR_1 = TIMEOUT_VAL_S;
		printf("time out value is %d\r\n", *TEMP_DDR_1);
	}
	else{
		*TEMP_DDR_1 = *TEMP_DDR_1 + 1;
		printf("time out value is %d\r\n", *TEMP_DDR_1);
	}
	if(*TEMP_DDR_2 > INT_NUM_E){
		*TEMP_DDR_2 = INT_NUM_S;
		int_num = INT_NUM_S;
		printf("interrupt num is %d\r\n", *TEMP_DDR_2);
	}
	else{
		*TEMP_DDR_2 = *TEMP_DDR_2 + 1;
		int_num = *TEMP_DDR_2;
		printf("interrupt num is %d\r\n", *TEMP_DDR_2);
	}
	printf("S\r\n");

	ECLIC_Register_IRQ(WDT_IRQn, ECLIC_NON_VECTOR_INTERRUPT, ECLIC_LEVEL_TRIGGER, 1, 1, wdt_handler);
	ECLIC_Register_IRQ(39, ECLIC_NON_VECTOR_INTERRUPT, ECLIC_LEVEL_TRIGGER, 2, 2, pmu_handler);
	__enable_irq();

	WDT_InitStruct.WDT_PuaseLength	= WDT_CR_RPL_2;
	WDT_InitStruct.WDT_Mode		= WDT_CR_RMOD_INTERRUPT;
	//WDT_InitStruct.WDT_TimeOutValue	= WDT_TORR_TOP_167M;
	WDT_InitStruct.WDT_TimeOutValue	= *TEMP_DDR_1;

	AL9000_wdt_init(WDT,&WDT_InitStruct);

	printf("E\r\n");
	while(1);

	return 0;
}

