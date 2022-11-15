#include "al9000_wdog.h"
#include "demosoc.h"



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

volatile uint32_t interrupt_count=0;


void wdt_handler(void)
{
	uint32_t read_temp=0;

	if(GET_BIT(WDT->STAT,0) == 0x1)
	{
		//printf("WDT->STAT is %x\r\n", WDT->STAT);
		interrupt_count++;
		if(interrupt_count < 5)
		{
			read_temp = WDT->EOI;	//Can clear by reading WDT_EOI
			//printf("watch irq %d\r\n", interrupt_count);
			//printf("[wdog]:i\r\n");
		}
		//wdt->CRR |= WDT_CCVR_VALUE;
		//printf("[wdog]:o\r\n");
	}
}

void pmu_handler(void)
{
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
		printf("[wdog]:e1");
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

void wdt_apu_init(void)
{
	WDT_InitTypeDef WDT_InitStruct;

	ECLIC_Register_IRQ(WDT_IRQn, ECLIC_NON_VECTOR_INTERRUPT, ECLIC_LEVEL_TRIGGER, 1, 1, wdt_handler);
	ECLIC_Register_IRQ(39, ECLIC_NON_VECTOR_INTERRUPT, ECLIC_LEVEL_TRIGGER, 2, 2, pmu_handler);
	__enable_irq();

	WDT_InitStruct.WDT_PuaseLength	= WDT_CR_RPL_2;
	WDT_InitStruct.WDT_Mode		= WDT_CR_RMOD_INTERRUPT;
	//WDT_InitStruct.WDT_TimeOutValue	= WDT_TORR_TOP_1342M;
	WDT_InitStruct.WDT_TimeOutValue	= WDT_TORR_TOP_10M;
	//int_num = 5;
	AL9000_wdt_init(WDT,&WDT_InitStruct);

	//while(1);

	return ;
}








