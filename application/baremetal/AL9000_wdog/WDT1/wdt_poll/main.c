#include <stdio.h>
#include <stdlib.h>
#include "nuclei_sdk_soc.h"
#include "al9000_wdog.h"

#define TEMP_DDR_1 ((volatile uint32_t *)(0X10000000UL+0X100000UL))

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



int main(void)
{

	WDT_InitTypeDef WDT_InitStruct;
	uint32_t read_temp=0;

	printf("S\r\n");


	WDT_InitStruct.WDT_PuaseLength	= WDT_CR_RPL_2;
	WDT_InitStruct.WDT_Mode		= WDT_CR_RMOD_RESET;
	WDT_InitStruct.WDT_TimeOutValue	= WDT_TORR_TOP_167M;


	AL9000_wdt_init(WDT1,&WDT_InitStruct);

	printf("E\r\n");
	while(1)
	{
		if(WDT1->CCVR <= 550)
		{
			WDT1->CRR |= WDT_CCVR_VALUE;	//Can clear by reading WDT_EOI
			printf("C\r\n");
		}
	}

	return 0;
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

	volatile unsigned int read_temp = wdt->EOI;

	wdt->TORR &= TORR_CLR_MASK;

	wdt->TORR |= WDT_InitStruct->WDT_TimeOutValue;

	wdt->CR	&= CR_CLR_MASK;

	wdt->CR |= WDT_InitStruct->WDT_PuaseLength;
    wdt->CR |= WDT_InitStruct->WDT_Mode;
    wdt->CR |= WDT_CR_WDT_EN;

	wdt->CRR = WDT_CCVR_VALUE;

	*PMU_SEL_WDT = 1 << 9;
	*PMU_EN_WDT = 1 << 9;
	*PMU_SHACK  = 1 << 2;

	(*WDT_PAUSE_REG) &= 0xfffffffd;
}





