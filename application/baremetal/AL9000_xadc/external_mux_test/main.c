/*
 * main.c
 *
 *  Created on: 2022年5月29日
 *      Author: jian.huang
 */
#include "al9000_xadc.h"


volatile uint16_t value = 0;
volatile uint32_t XadcStatus = 0;
volatile uint16_t XadcConfig1 = 0;
volatile uint16_t flagValue = 0;





#define CURRENT_MODE		CONTINUE_MODE
#define CURRENT_CH 			7				//0 1



#define USE_12BIT 1

#if USE_12BIT
#define CURRENT_RESOLUTION 	RES_12BIT
#endif

#if USE_10BIT
#define CURRENT_RESOLUTION 	RES_10BIT
#endif

#if USE_8BIT
#define CURRENT_RESOLUTION 	RES_8BIT
#endif

#if USE_6BIT
#define CURRENT_RESOLUTION 	RES_6BIT
#endif





uint8_t XadcInit(void)
{
	INTR_MASK_TypeDef intr_mask;
	CFG_TypeDef cfg;
	CTRL_TypeDef ctrl;

	/* Reset XADC */
	ctrl.bitfiled.CMD_FIFO_FLUSE = FLUSH_CMD_FIFO;
	ctrl.bitfiled.DATA_FIFO_FLUSE = FLUSH_DAYA_FIFO;
	ctrl.bitfiled.RESET = RESET_XADC_PS_CH;

	XADC->CTRL = ctrl.reg;

	ctrl.bitfiled.CMD_FIFO_FLUSE = RECOVER_CMD_FIFO;
	ctrl.bitfiled.DATA_FIFO_FLUSE = RECOVER_DAYA_FIFO;
	ctrl.bitfiled.RESET = RECOVER_XADC_PS_CH;
	XADC->CTRL = ctrl.reg;

	/* Clear All Interrupt Signal */
	XADC->INTR |= 0x1|
			(0x1 << 11)|
			(0x1 << 10)|
			(0x1 << 9)|
			(0x1 << 8);


	cfg.bitfiled.ENABLE_PS_ADC = ENABLE_ADC_PS_ACCESS;
	//cfg.bitfiled.TCK_RATE = QQ_XADC_PCLK;
	XADC->CFG |= cfg.reg;
	printf("XADC->CFG:%x \r\n",XADC->CFG);

	printf("Status:%x\r\n",XADC->STATUS);
	printf("INTR:%x\r\n",XADC->INTR);


	intr_mask.reg = XADC->INTR_MASK;
	intr_mask.bitfiled.MASK_INTR = ENABLE_SAMPLE_INTER;
	XADC->INTR_MASK = intr_mask.reg;

	printf("XADC->INTR_MASK:%x \r\n",XADC->INTR_MASK);
	printf("XADC->CTRL:%x \r\n",XADC->CTRL);

	/**/
	G0_XADC_CONFIG0 = ((4 << REG_PIB_SEL_OFFSET)|
				(MUX_ENABLE << MUX_ENBALE_OFFSET)|
				(PHY_EXTERNAL_MODE << PHY_EXTERNAL_M_OFFSET)|
				(SINGLE_MODE << INPUT_M_OFFSET)|
				(CURRENT_RESOLUTION << RES_OFFSET)|
				(REF_INTER_REF << REF_OFFSET)|
				(BAG_ACTIVE << BAG_SEL_OFFSET)|
				(ADC_SW_RESET << SW_RESET_OFFSET));

	/* Enable ADC PS Access */


	printf("e0000\r\n");

	G0_XADC_CONFIG0 = ((REG_SEL << REG_PIB_SEL_OFFSET)|
			(MUX_ENABLE << MUX_ENBALE_OFFSET)|
			(PHY_EXTERNAL_MODE << PHY_EXTERNAL_M_OFFSET)|
			(SINGLE_MODE << INPUT_M_OFFSET)|
			(CURRENT_RESOLUTION << RES_OFFSET)|
			(REF_INTER_REF << REF_OFFSET)|
			(BAG_ACTIVE << BAG_SEL_OFFSET)|
			(ADC_RELEASE << SW_RESET_OFFSET));

	printf("e1\r\n");

	G0_XADC_CONFIG2 = ((ADC_PL_CLK_70M << ADC_CLK_SEL_OFFSET)|
				(1 << ADC_ENABLE_OFFSET));
	printf("e2\r\n");

	G0_XADC_CONFIG1 = ((1 << INTR_ERROR_MASK_OFFSET)|
					(1 << INTR_LTH_MASK_OFFSET)|
					(1 << INTR_GTH_MASK_OFFSET)|
					(0 << INTR_DONE_MASK_OFFSET));

	printf("ADC_FLAG:%x \r\n",G0_XADC_FLAG);
	printf("ADC_CONFIG1:%x \r\n",G0_XADC_CONFIG1);

	printf("e3\r\n");

	G0_XADC_CHANNEL10 = IO_MUX_1 << 8 | IO_MUX_16;
	/*G0_XADC_CHANNEL32 = IO_MUX_11 << 8 | IO_MUX_10;
	G0_XADC_CHANNEL54 = IO_MUX_13 << 8 | IO_MUX_12;
	G0_XADC_CHANNEL76 = IO_MUX_15 << 8 | IO_MUX_16;


	G0_XADC_CHANNEL67 = IO_MUX_9 << 8 | IO_MUX_8;
	G0_XADC_CHANNEL45 = IO_MUX_11 << 8 | IO_MUX_10;
	G0_XADC_CHANNEL23 = IO_MUX_13 << 8 | IO_MUX_12;
	G0_XADC_CHANNEL01 = IO_MUX_15 << 8 | IO_MUX_16;
*/

	printf("e4\r\n");

	G0_XADC_CONFIG3 = ((START_CONVER << REG_ADC_SOC_OFFSET)|
				(CURRENT_CH << EXTERNAL_CH_SEL_OFFSET)|
				(0 << CH_SEL_OFFSET)|
				(CURRENT_MODE << MODE_SEL_OFFSET));

	printf("e\r\n");
/*
	printf("Config0:%x \r\n", G0_XADC_CONFIG0));
	printf("Config1:%x \r\n", G0_XADC_CONFIG1));
	printf("Config2:%x \r\n", G0_XADC_CONFIG2));
	printf("Config3:%x \r\n", G0_XADC_CONFIG3));
*/
	return 0;
}



void ClearLthIntr(void)
{
	uint16_t temp;
	temp = G0_XADC_CONFIG1;

	/* Clear PL interrupt Signal */
	G0_XADC_CONFIG1 = (1 << INTR_LTH_OFFSET)|temp;
}

void ClearGthIntr(void)
{
	uint16_t temp;
	temp = G0_XADC_CONFIG1;;

	/* Clear PL interrupt Signal */
	G0_XADC_CONFIG1 = (1 << INTR_GTH_OFFSET)|temp;
}

void ClearDoneIntr(void)
{
	uint16_t temp;
	temp = G0_XADC_CONFIG1;;

	/* Clear PL interrupt Signal */
	G0_XADC_CONFIG1 = (1 << INTR_DONE_OFFSET)|temp;
}

void ClearErrIntr(void)
{
	uint16_t temp;
	temp = G0_XADC_CONFIG1;;

	/* Clear PL interrupt Signal */
	G0_XADC_CONFIG1 = (1 << INTR_ERROR_OFFSET)|temp;
}




void xadc_handler(void)
{
	CFG_TypeDef cfg;

	printf("i\r\n");

	XadcStatus = XADC->STATUS;

	printf("Status:%x\r\n",XadcStatus);

	if(XADC->INTR&0x01)
	{
		XadcConfig1 = G0_XADC_CONFIG1;
	//	printf("XadcConfig1:%x \r\n",XadcConfig1);

		/* ADC Done Flag */
		if(XadcConfig1 & (0x01 << 8))
		{
			switch(CURRENT_CH)
			{
				/* 逻辑需要此处case不使用break*/
				case 7:
					value = G0_XADC_VC7;
					printf("V7:%x\r\n",value >> 4);
				case 6:
					value = G0_XADC_VC6;
					printf("V6:%x\r\n",value >> 4);
				case 5:
					value = G0_XADC_VC5;
					printf("V5:%x\r\n",value >> 4);
				case 4:
					value = G0_XADC_VC4;
					printf("V4:%x\r\n",value >> 4);
				case 3:
					value = G0_XADC_VC3;
					printf("V3:%x\r\n",value >> 4);
				case 2:
					value = G0_XADC_VC2;
					printf("V2:%x\r\n",value >> 4);
				case 1:
					value = G0_XADC_VC1;
					printf("V1:%x\r\n",value >> 4);
				case 0:
					value = G0_XADC_VC0;
					printf("V0:%x\r\n",value >> 4);
					break;
				default:
					printf("ADC Done Parem Error.\r\n");
					break;

			}
			ClearDoneIntr();
		}
		/* ADC Error Happened Interrupt */
		else if(XadcConfig1 & (0x01 << 11))
		{
			ClearErrIntr();
			printf("ADC Error\r\n");
		}
		/* ADC_VC* value lower than ADC_CH*_LTH Interrupt */
		else if(XadcConfig1 & (0x01 << 10))
		{
			flagValue = G0_XADC_FLAG;

			if(flagValue&(1 << FLAG_VC0))
			{
				value = G0_XADC_VC0;
				printf("V0:%x\r\n",value >> 4);
			}
			else if(flagValue&(1 << FLAG_VC1))
			{
				value = G0_XADC_VC1;
				printf("V1:%x\r\n",value >> 4);
			}
			else if(flagValue&(1 << FLAG_VC2))
			{
				value = G0_XADC_VC2;
				printf("V2:%x\r\n",value >> 4);
			}
			else if(flagValue&(1 << FLAG_VC3))
			{
				value = G0_XADC_VC3;
				printf("V3:%x\r\n",value >> 4);
			}
			else if(flagValue&(1 << FLAG_VC4))
			{
				value = G0_XADC_VC4;
				printf("V4:%x\r\n",value >> 4);
			}
			else if(flagValue&(1 << FLAG_VC5))
			{
				value = G0_XADC_VC5;
				printf("V5:%x\r\n",value >> 4);
			}
			else if(flagValue&(1 << FLAG_VC6))
			{
				value = G0_XADC_VC6;
				printf("V6:%x\r\n",value >> 4);
			}
			else if(flagValue&(1 << FLAG_VC7))
			{
				value = G0_XADC_VC7;
				printf("V7:%x\r\n",value >> 4);
			}


			ClearLthIntr();
			printf("lower than\r\n");
		}
		/* ADC_VC* value greater than ADC_CH*_GTH Interrupt */
		else if(XadcConfig1 & (0x01 << 9))
		{
			flagValue = G0_XADC_FLAG;

			if(flagValue&(1 << FLAG_VC0))
			{
				value = G0_XADC_VC0;
				printf("V0:%x\r\n",value >> 4);
			}
			else if(flagValue&(1 << FLAG_VC1))
			{
				value = G0_XADC_VC1;
				printf("V1:%x\r\n",value >> 4);
			}
			else if(flagValue&(1 << FLAG_VC2))
			{
				value = G0_XADC_VC2;
				printf("V2:%x\r\n",value >> 4);
			}
			else if(flagValue&(1 << FLAG_VC3))
			{
				value = G0_XADC_VC3;
				printf("V3:%x\r\n",value >> 4);
			}
			else if(flagValue&(1 << FLAG_VC4))
			{
				value = G0_XADC_VC4;
				printf("V4:%x\r\n",value >> 4);
			}
			else if(flagValue&(1 << FLAG_VC5))
			{
				value = G0_XADC_VC5;
				printf("V5:%x\r\n",value >> 4);
			}
			else if(flagValue&(1 << FLAG_VC6))
			{
				value = G0_XADC_VC6;
				printf("V6:%x\r\n",value >> 4);
			}
			else if(flagValue&(1 << FLAG_VC7))
			{
				value = G0_XADC_VC7;
				printf("V7:%x\r\n",value >> 4);
			}

			ClearGthIntr();
			printf("value greater than\r\n");
		}

		/* Clear PS interrupt Signal */
		XADC->INTR |= 0x1;
	}

	/* Data FIFO less level threshold interrupt. */
	if(XADC->INTR&(0x01 << 8))
	{
		printf("data less level.\r\n");

		printf("Data Fifo:%d\r\n",(XADC->STATUS >> 12)&0xf);

		/* Clear PS interrupt Signal */
		XADC->INTR |= (0x1 << 8);
	}
	/* Data FIFO greater level threshold interrupt. */
	else if(XADC->INTR&(0x01 << 10))
	{
		printf("Data greater level.\r\n");

		printf("Data Fifo:%d\r\n",(XADC->STATUS >> 12)&0xf);

		/* Clear PS interrupt Signal */
		XADC->INTR |= (0x1 << 10);
	}


	/* Command FIFO less level threshold interrupt. */
	if(XADC->INTR&(0x01 << 9))
	{
		printf("Com less level.\r\n");

		printf("Cmd Fifo:%d\r\n",(XADC->STATUS >> 16)&0xf);

		/* Clear PS interrupt Signal */
		XADC->INTR |= (0x1 << 9);
	}
	/* Command FIFO greater level threshold interrupt. */
	else if(XADC->INTR&(0x01 << 11))
	{
		printf("Command greater level.\r\n");

		//printf("Cmd Fifo:%d\r\n",(XADC->STATUS >> 16)&0xf);
		printf("Cmd Fifo:%d\r\n",XADC->STATUS);

		/* Clear PS interrupt Signal */
		XADC->INTR |= (0x1 << 11);
	}
}








int main()
{
	/* Note:seldiff control signal must remain unchanged when a conversion is being done. */

	/* Note:selbg and selref control signals must remain unchanged after a startup sequence.
	 * A new startup sequence must be made for any change in these control signals.*/

	printf("mask test\r\n");

	GP_PROTEN &= 0xfffffffd;

	XadcInit();
	ECLIC_Register_IRQ(XADC_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 0, 1, xadc_handler);

		__enable_irq();

	while(1);
	{

//		printf("G0_XADC_CONFIG1:%x\r\n",G0_XADC_CONFIG1);
//		printf("G0_XADC_FLAG:%x\r\n",G0_XADC_FLAG);
		printf("G0_XADC_vc0:%x\r\n",G0_XADC_VC0);
		printf("G0_XADC_vc1:%x\r\n",G0_XADC_VC1);
		printf("G0_XADC_vc2:%x\r\n",G0_XADC_VC2);

	}

	return 0;
}

