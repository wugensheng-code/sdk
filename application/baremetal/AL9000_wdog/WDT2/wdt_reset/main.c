#include <stdio.h>
#include <stdlib.h>
#include "nuclei_sdk_soc.h"
#include "al9000_wdog.h"

#define TEMP_DDR_1 ((volatile uint32_t *)(0X10000000UL+0X100000UL))

#define TOP_CFG_CTRL_WDT_BASEADDR (0XF8800000UL+0X168UL)
#define WDT_PAUSE_REG ((volatile uint32_t *)TOP_CFG_CTRL_WDT_BASEADDR)

#define PMU_ERR_HW_EN0_SET 	(0XF8806000UL)
#define PMU_SEL_WDT ((volatile uint32_t *)PMU_ERR_HW_EN0_SET)

#define PMU_INT_EN0_SET 	(0XF8806010UL)
#define PMU_EN_WDT ((volatile uint32_t *)PMU_INT_EN0_SET)

#define PMU_CLR_WDT_INTR 	(0XF8806030UL)
#define PMU_CLR_WDT ((volatile uint32_t *)PMU_CLR_WDT_INTR)

#define PMU_SW_PMU_SHACK 	(0XF8806080UL)
#define PMU_SHACK ((volatile uint32_t *)PMU_SW_PMU_SHACK)




/* case 1.2 */
volatile uint32_t interrupt_count=0;

void wdt_handler(void)
{
	uint32_t read_temp=0;
	
	if(GET_BIT(WDT2->STAT,0) == 0x1)
	{
		//printf("WDT2->STAT is %x\r\n", WDT2->STAT);
		interrupt_count++;
		if(interrupt_count < 4)
		{
			read_temp = WDT2->EOI;	//Can clear by reading WDT_EOI
			//printf("watch irq %d\r\n", interrupt_count);
			printf("l\r\n");
		}
		//wdt->CRR |= WDT_CCVR_VALUE;
	}
}

void pmu_handler(void)
{
	printf("pmu\r\n");
	*PMU_CLR_WDT  = 1 << 10;
}


int main(void)
{
	WDT_InitTypeDef WDT_InitStruct;

//	if(*TEMP_DDR_1 > 0xF){
//		*TEMP_DDR_1 = 0;
//		printf("temp ddr data is %d\r\n", *TEMP_DDR_1);
//	}
//	else{
//		*TEMP_DDR_1 = *TEMP_DDR_1 + 1;
//		printf("temp ddr data is %d\r\n", *TEMP_DDR_1);
//	}
	printf("S\r\n");

	ECLIC_Register_IRQ(WDT2_IRQn, ECLIC_NON_VECTOR_INTERRUPT, ECLIC_LEVEL_TRIGGER, 1, 1, wdt_handler);
	ECLIC_Register_IRQ(39, ECLIC_NON_VECTOR_INTERRUPT, ECLIC_LEVEL_TRIGGER, 2, 2, pmu_handler);
	__enable_irq();

	WDT_InitStruct.WDT_PuaseLength	= WDT_CR_RPL_2;
	WDT_InitStruct.WDT_Mode		= WDT_CR_RMOD_INTERRUPT;
	WDT_InitStruct.WDT_TimeOutValue	= WDT_TORR_TOP_167M;
	//WDT_InitStruct.WDT_TimeOutValue	= *TEMP_DDR_1;

	AL9000_wdt_init(WDT2,&WDT_InitStruct);

	printf("E\r\n");
	while(1);

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

	*PMU_SEL_WDT = 1 << 10;
	*PMU_EN_WDT = 1 << 10;
	*PMU_SHACK  = 1 << 2;

	(*WDT_PAUSE_REG) &= 0xfffffffb;
}





