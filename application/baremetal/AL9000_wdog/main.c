#include <stdio.h>
#include <stdlib.h>
#include "nuclei_sdk_soc.h"
#include "al9000_wdog.h"


#define TOP_CFG_CTRL_WDT_BASEADDR (0XF8800000UL+0X168UL)
#define WDT_PAUSE_REG ((uint32_t *)TOP_CFG_CTRL_WDT_BASEADDR)



/* case 1.1
void wdt_handler(void)
{
	uint32_t read_temp=0;

	if(GET_BIT(WDT->STAT,0) == 0x1)
	{
		read_temp = WDT->EOI;	//Can clear by reading WDT_EOI
	}
}
*/


/* case 1.2 */
uint32_t interrupt_count=0;

void wdt_handler(void)
{
	uint32_t read_temp=0;

	if(GET_BIT(WDT->STAT,0) == 0x1)
	{
		interrupt_count++;
		if(interrupt_count < 5)
		{
			read_temp = WDT->EOI;	//Can clear by reading WDT_EOI
		}
		//wdt->CRR |= WDT_CCVR_VALUE;
	}
}




int main(void)
{
	WDT_InitTypeDef WDT_InitStruct;

	ECLIC_Register_IRQ(WDT_IRQn, ECLIC_NON_VECTOR_INTERRUPT, ECLIC_LEVEL_TRIGGER, 1, 1, wdt_handler);
	__enable_irq();

	WDT_InitStruct.WDT_PuaseLength	= WDT_CR_RPL_2;
	WDT_InitStruct.WDT_Mode		= WDT_CR_RMOD_INTERRUPT;
	WDT_InitStruct.WDT_TimeOutValue	= WDT_TORR_TOP_327U;

	AL9000_wdt_init(WDT,&WDT_InitStruct);

	//printf("E");
	while(1);

	return 0;
}

/* */

/* case 2
int main(void)
{

	WDT_InitTypeDef WDT_InitStruct;
	uint32_t read_temp=0;

	printf("S");


	WDT_InitStruct.WDT_PuaseLength	= WDT_CR_RPL_2;
	WDT_InitStruct.WDT_Mode		= WDT_CR_RMOD_RESET;
	WDT_InitStruct.WDT_TimeOutValue	= WDT_TORR_TOP_327U;


	AL9000_wdt_init(WDT,&WDT_InitStruct);

	printf("E");
	while(1)
	{
		if(WDT->CCVR <= 55)
		{
			read_temp = WDT->EOI;	//Can clear by reading WDT_EOI
		}
	}

	return 0;
}
*/


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

	(*WDT_PAUSE_REG) &= 0xfffffffe;
}





