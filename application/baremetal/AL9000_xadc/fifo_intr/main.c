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






INTR_MASK_TypeDef intr_mask;

uint8_t XadcFifoInit(void)
{

	CFG_TypeDef cfg;
	CTRL_TypeDef ctrl;

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

	cfg.reg = XADC->CFG;

	cfg.bitfiled.ENABLE_PS_ADC = ENABLE_ADC_PS_ACCESS;
	cfg.bitfiled.CMD_FIFO_LVL_H  = 4;
	cfg.bitfiled.CMD_FIFO_LVL_L  = 0;
	cfg.bitfiled.DATA_FIFO_LVL_H = 2;
	cfg.bitfiled.DATA_FIFO_LVL_L = 0;

	//cfg.bitfiled.TCK_RATE = QQ_XADC_PCLK;
	XADC->CFG = cfg.reg;


	//printf("XADC->CFG:%x \r\n",XADC->CFG);

//	printf("Status:%x\r\n",XADC->STATUS);
//	printf("INTR:%x\r\n",XADC->INTR);
//	printf("ADC_FLAG:%x \r\n",XadcRead(ADC_FLAG));
//	printf("ADC_CONFIG1:%x \r\n",XadcRead(ADC_CONFIG1));

	intr_mask.reg = XADC->INTR_MASK;
	intr_mask.bitfiled.MASK_INTR = ENABLE_SAMPLE_INTER;
	/**/
	intr_mask.bitfiled.MASK_DATA_FIFO_GTH = MASK_DATA_FIFO_GTH_INTER;
	intr_mask.bitfiled.MASK_DATA_FIFO_LTH = MASK_DATA_FIFO_LTH_INTER;
	intr_mask.bitfiled.MASK_CMD_FIFO_LTH = MASK_CMD_FIFO_LTH_INTER;
	intr_mask.bitfiled.MASK_CMD_FIFO_GTH = ENABLE_CMD_FIFO_GTH_INTER;

	XADC->INTR_MASK = intr_mask.reg;



//	printf("XADC->INTR_MASK:%x \r\n",XADC->INTR_MASK);

//	printf("XADC->CTRL:%x \r\n",XADC->CTRL);


}










void ClearLthIntr(void)
{
	uint16_t temp;
	temp = XadcRead(ADC_CONFIG1);

	/* Clear PL interrupt Signal */
	XadcWrite(ADC_CONFIG1,
			(1 << INTR_LTH_OFFSET)|temp);
}

void ClearGthIntr(void)
{
	uint16_t temp;
	temp = XadcRead(ADC_CONFIG1);

	/* Clear PL interrupt Signal */
	XadcWrite(ADC_CONFIG1,
			(1 << INTR_GTH_OFFSET)|temp);
}

void ClearDoneIntr(void)
{
	uint16_t temp;
	temp = XadcRead(ADC_CONFIG1);

	/* Clear PL interrupt Signal */
	XadcWrite(ADC_CONFIG1,
			(1 << INTR_DONE_OFFSET)|temp);
}

void ClearErrIntr(void)
{
	uint16_t temp;
	temp = XadcRead(ADC_CONFIG1);

	/* Clear PL interrupt Signal */
	XadcWrite(ADC_CONFIG1,
			(1 << INTR_ERROR_OFFSET)|temp);
}

#define DATA_FIFO_LESS			1
#define COMMAND_FIFO_LESS		2
#define DATA_FIFO_GREATER		3
#define COMMAND_FIFO_GREATER	4
#define OTHER					5


volatile uint32_t fifoState = 0;


volatile uint32_t DataFifoTemp = 0;


void xadc_handler(void)
{
	CFG_TypeDef cfg;

	//printf("i\r\n");

	XadcStatus = XADC->STATUS;

	printf("Status:%x\r\n",XadcStatus);

	if(XADC->INTR&0x01)
	{
		//printf("s\r\n");
		//printf("data fifo level %d\r\n",XADC->STATUS);
		XadcConfig1 = XadcRead(ADC_CONFIG1);
	//	printf("XadcConfig1:%x \r\n",XadcConfig1);

		/* ADC Done Flag */
		if(XadcConfig1 & (0x01 << 8))
		{
			value = XadcRead(ADC_VC0);
		//	printf("V0:%x\r\n",value >> 4);

			value = XadcRead(ADC_VC1);
		//	printf("V1:%x\r\n",value >> 4);

			value = XadcRead(ADC_VC2);
		//	printf("V2:%x\r\n",value >> 4);

			value = XadcRead(ADC_VC3);
		//	printf("V3:%x\r\n",value >> 4);

			value = XadcRead(ADC_VC4);
		//	printf("V4:%x\r\n",value >> 4);

			value = XadcRead(ADC_VC5);
		//	printf("V5:%x\r\n",value >> 4);

			value = XadcRead(ADC_VC6);
		//	printf("V6:%x\r\n",value >> 4);

			value = XadcRead(ADC_VC7);
		//	printf("V7:%x\r\n",value >> 4);


			ClearDoneIntr();

			/*
			XadcWrite(ADC_CONFIG3, (NO_OPERATION << REG_ADC_SOC_OFFSET)|
										(CURRENT_CH << CH_SEL_OFFSET)|
								(CURRENT_MODE << MODE_SEL_OFFSET));*/
			/*


			XadcWrite(ADC_CONFIG3, (START_CONVER << REG_ADC_SOC_OFFSET)|
										(CURRENT_CH << CH_SEL_OFFSET)|
										(CURRENT_MODE << MODE_SEL_OFFSET));
			*/

			//printf("2:%x \r\n",XADC->INTR);


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
			flagValue = XadcRead(ADC_FLAG);

			if(flagValue&(1 << FLAG_VC0))
			{
				value = XadcRead(ADC_VC0);
				printf("V0:%x\r\n",value >> 4);
			}
			else if(flagValue&(1 << FLAG_VC1))
			{
				value = XadcRead(ADC_VC1);
				printf("V1:%x\r\n",value >> 4);
			}
			else if(flagValue&(1 << FLAG_VC2))
			{
				value = XadcRead(ADC_VC2);
				printf("V2:%x\r\n",value >> 4);
			}
			else if(flagValue&(1 << FLAG_VC3))
			{
				value = XadcRead(ADC_VC3);
				printf("V3:%x\r\n",value >> 4);
			}
			else if(flagValue&(1 << FLAG_VC4))
			{
				value = XadcRead(ADC_VC4);
				printf("V4:%x\r\n",value >> 4);
			}
			else if(flagValue&(1 << FLAG_VC5))
			{
				value = XadcRead(ADC_VC5);
				printf("V5:%x\r\n",value >> 4);
			}
			else if(flagValue&(1 << FLAG_VC6))
			{
				value = XadcRead(ADC_VC6);
				printf("V6:%x\r\n",value >> 4);
			}
			else if(flagValue&(1 << FLAG_VC7))
			{
				value = XadcRead(ADC_VC7);
				printf("V7:%x\r\n",value >> 4);
			}


			ClearLthIntr();
			printf("lower than\r\n");
		}
		/* ADC_VC* value greater than ADC_CH*_GTH Interrupt */
		else if(XadcConfig1 & (0x01 << 9))
		{
			flagValue = XadcRead(ADC_FLAG);

			if(flagValue&(1 << FLAG_VC0))
			{
				value = XadcRead(ADC_VC0);
				printf("V0:%x\r\n",value >> 4);
			}
			else if(flagValue&(1 << FLAG_VC1))
			{
				value = XadcRead(ADC_VC1);
				printf("V1:%x\r\n",value >> 4);
			}
			else if(flagValue&(1 << FLAG_VC2))
			{
				value = XadcRead(ADC_VC2);
				printf("V2:%x\r\n",value >> 4);
			}
			else if(flagValue&(1 << FLAG_VC3))
			{
				value = XadcRead(ADC_VC3);
				printf("V3:%x\r\n",value >> 4);
			}
			else if(flagValue&(1 << FLAG_VC4))
			{
				value = XadcRead(ADC_VC4);
				printf("V4:%x\r\n",value >> 4);
			}
			else if(flagValue&(1 << FLAG_VC5))
			{
				value = XadcRead(ADC_VC5);
				printf("V5:%x\r\n",value >> 4);
			}
			else if(flagValue&(1 << FLAG_VC6))
			{
				value = XadcRead(ADC_VC6);
				printf("V6:%x\r\n",value >> 4);
			}
			else if(flagValue&(1 << FLAG_VC7))
			{
				value = XadcRead(ADC_VC7);
				printf("V7:%x\r\n",value >> 4);
			}

			ClearGthIntr();
			printf("greater than\r\n");
		}



		/* Clear PS interrupt Signal */
		XADC->INTR |= 0x1;
	}

	/* Data FIFO less level threshold interrupt. */
	if(XADC->INTR&(0x01 << 8))
	{
		printf("data less level.\r\n");

		printf("Data Fifo:%d\r\n",(XADC->STATUS >> 12)&0xf);

		fifoState = DATA_FIFO_LESS;

		cfg.reg = XADC->CFG;

		cfg.bitfiled.ENABLE_PS_ADC = ENABLE_ADC_PS_ACCESS;
		cfg.bitfiled.CMD_FIFO_LVL_H  = 5;
		cfg.bitfiled.CMD_FIFO_LVL_L  = 0;
		cfg.bitfiled.DATA_FIFO_LVL_H = 4;
		cfg.bitfiled.DATA_FIFO_LVL_L = 0;

		//cfg.bitfiled.TCK_RATE = QQ_XADC_PCLK;
		XADC->CFG = cfg.reg;

		/* Clear PS interrupt Signal */
		XADC->INTR |= (0x1 << 8);

		intr_mask.reg = XADC->INTR_MASK;
		intr_mask.bitfiled.MASK_INTR = MASK_SAMPLE_INTER;
		intr_mask.bitfiled.MASK_DATA_FIFO_GTH = MASK_DATA_FIFO_GTH_INTER;
		intr_mask.bitfiled.MASK_DATA_FIFO_LTH = MASK_DATA_FIFO_LTH_INTER;
		intr_mask.bitfiled.MASK_CMD_FIFO_LTH = MASK_CMD_FIFO_LTH_INTER;
		intr_mask.bitfiled.MASK_CMD_FIFO_GTH = MASK_CMD_FIFO_GTH_INTER;

		XADC->INTR_MASK = intr_mask.reg;

	}
	/* Data FIFO greater level threshold interrupt. */
	else if(XADC->INTR&(0x01 << 10))
	{
		printf("Data greater level.\r\n");

		printf("Data Fifo:%d\r\n",(XADC->STATUS >> 12)&0xf);

		/* read all data form data fifo */
		DataFifoTemp = XADC->DATA;
		DataFifoTemp = XADC->DATA;
		DataFifoTemp = XADC->DATA;
		DataFifoTemp = XADC->DATA;
		DataFifoTemp = XADC->DATA;
		DataFifoTemp = XADC->DATA;
		DataFifoTemp = XADC->DATA;


		fifoState = DATA_FIFO_GREATER;

		/* Clear PS interrupt Signal */
		XADC->INTR |= (0x1 << 10);

		intr_mask.reg = XADC->INTR_MASK;
		intr_mask.bitfiled.MASK_INTR = MASK_SAMPLE_INTER;
		intr_mask.bitfiled.MASK_DATA_FIFO_GTH = MASK_DATA_FIFO_GTH_INTER;
		intr_mask.bitfiled.MASK_DATA_FIFO_LTH = ENABLE_DATA_FIFO_LTH_INTER;
		intr_mask.bitfiled.MASK_CMD_FIFO_LTH = MASK_CMD_FIFO_LTH_INTER;
		intr_mask.bitfiled.MASK_CMD_FIFO_GTH = MASK_CMD_FIFO_GTH_INTER;

		XADC->INTR_MASK = intr_mask.reg;
	}


	/* Command FIFO less level threshold interrupt. */
	if(XADC->INTR&(0x01 << 9))
	{
		printf("Com less level.\r\n");

		printf("Cmd Fifo:%d\r\n",(XADC->STATUS >> 16)&0xf);

		fifoState = COMMAND_FIFO_LESS;

		cfg.reg = XADC->CFG;

		cfg.bitfiled.ENABLE_PS_ADC = ENABLE_ADC_PS_ACCESS;
		cfg.bitfiled.CMD_FIFO_LVL_H  = 5;
		cfg.bitfiled.CMD_FIFO_LVL_L  = 0;
		cfg.bitfiled.DATA_FIFO_LVL_H = 4;
		cfg.bitfiled.DATA_FIFO_LVL_L = 0;

		//cfg.bitfiled.TCK_RATE = QQ_XADC_PCLK;
		XADC->CFG = cfg.reg;

		intr_mask.reg = XADC->INTR_MASK;
		intr_mask.bitfiled.MASK_INTR = MASK_SAMPLE_INTER;
		intr_mask.bitfiled.MASK_DATA_FIFO_GTH = ENABLE_DATA_FIFO_GTH_INTER;
		intr_mask.bitfiled.MASK_DATA_FIFO_LTH = MASK_DATA_FIFO_LTH_INTER;
		intr_mask.bitfiled.MASK_CMD_FIFO_LTH = MASK_CMD_FIFO_LTH_INTER;
		intr_mask.bitfiled.MASK_CMD_FIFO_GTH = MASK_CMD_FIFO_GTH_INTER;

		XADC->INTR_MASK = intr_mask.reg;

		/* Clear PS interrupt Signal */
		XADC->INTR |= (0x1 << 9);
	}
	/* Command FIFO greater level threshold interrupt. */
	else if(XADC->INTR&(0x01 << 11))
	{
		printf("Command greater level.\r\n");

		//printf("Cmd Fifo:%d\r\n",(XADC->STATUS >> 16)&0xf);
		printf("Cmd Fifo:%d\r\n",XADC->STATUS);

		fifoState = COMMAND_FIFO_GREATER;

		/* Clear PS interrupt Signal */
		XADC->INTR |= (0x1 << 11);

		intr_mask.reg = XADC->INTR_MASK;
		intr_mask.bitfiled.MASK_INTR = MASK_SAMPLE_INTER;
		intr_mask.bitfiled.MASK_DATA_FIFO_GTH = ENABLE_DATA_FIFO_GTH_INTER;
		intr_mask.bitfiled.MASK_DATA_FIFO_LTH = MASK_DATA_FIFO_LTH_INTER;
		intr_mask.bitfiled.MASK_CMD_FIFO_LTH = ENABLE_CMD_FIFO_LTH_INTER;
		intr_mask.bitfiled.MASK_CMD_FIFO_GTH = MASK_CMD_FIFO_GTH_INTER;

		XADC->INTR_MASK = intr_mask.reg;
	}
}



static void XadcReadFifo(uint8_t Addr)
{
	uint32_t Cmd = READ_CMD << 24 |
				Addr << 16;

	XADC->CMD = Cmd;


}

int main()
{
	CFG_TypeDef cfg;
	volatile uint32_t k = 0;
	volatile uint8_t OnceRun = 0;
	/* Note:seldiff control signal must remain unchanged when a conversion is being done. */

	/* Note:selbg and selref control signals must remain unchanged after a startup sequence.
	 * A new startup sequence must be made for any change in these control signals.*/

//	printf("fifo_and_compare_intr\r\n");


	XadcFifoInit();
	ECLIC_Register_IRQ(XADC_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 0, 1, xadc_handler);

		__enable_irq();

	while(1)
	{
		printf("fifoState:%d\r\n",fifoState);

		if(fifoState == 0)
		{
			printf("s1\r\n");
			if(OnceRun != 1)
			{
				OnceRun = 1;
				fifoState = OTHER;
			}

			XadcWrite(0x80, 1);
			XadcWrite(0x80, 2);
			XadcWrite(0x80, 3);
			XadcWrite(0x80, 4);
			XadcWrite(0x80, 5);
			XadcWrite(0x80, 6);
			XadcWrite(0x80, 7);
			XadcWrite(0x80, 8);
			XadcWrite(0x80, 9);
			XadcWrite(0x80, 10);


		}
		else if(fifoState == COMMAND_FIFO_GREATER)
		{
			printf("s2\r\n");
			if(OnceRun != 2)
			{
				OnceRun = 2;
				fifoState = OTHER;
			}

			cfg.reg = XADC->CFG;

			cfg.bitfiled.ENABLE_PS_ADC = ENABLE_ADC_PS_ACCESS;
			cfg.bitfiled.CMD_FIFO_LVL_H  = 5;
			cfg.bitfiled.CMD_FIFO_LVL_L  = 1;
			cfg.bitfiled.DATA_FIFO_LVL_H = 4;
			cfg.bitfiled.DATA_FIFO_LVL_L = 0;

			//cfg.bitfiled.TCK_RATE = QQ_XADC_PCLK;
			XADC->CFG = cfg.reg;
		}
		else if(fifoState == COMMAND_FIFO_LESS)
		{
			printf("s3\r\n");
			if(OnceRun != 3)
			{
				OnceRun = 3;
				fifoState = OTHER;
			}

			XadcReadFifo(0x80);
			XadcReadFifo(0x80);
			XadcReadFifo(0x80);
			XadcReadFifo(0x80);
			XadcReadFifo(0x80);
			XadcReadFifo(0x80);
			XadcReadFifo(0x80);
		}
		else if(fifoState == DATA_FIFO_GREATER)
		{
			printf("s4\r\n");
			if(OnceRun != 4)
			{
				OnceRun = 4;
				fifoState = OTHER;
			}

			cfg.reg = XADC->CFG;

			cfg.bitfiled.ENABLE_PS_ADC = ENABLE_ADC_PS_ACCESS;
			cfg.bitfiled.CMD_FIFO_LVL_H  = 5;
			cfg.bitfiled.CMD_FIFO_LVL_L  = 0;
			cfg.bitfiled.DATA_FIFO_LVL_H = 4;
			cfg.bitfiled.DATA_FIFO_LVL_L = 1;

			//cfg.bitfiled.TCK_RATE = QQ_XADC_PCLK;
			XADC->CFG = cfg.reg;

			k = XADC->DATA;
			k = XADC->DATA;
			k = XADC->DATA;
			k = XADC->DATA;
			k = XADC->DATA;

		}
		else if(fifoState == DATA_FIFO_LESS)
		{
			printf("s6\r\n");
			fifoState = OTHER;
			while(1);
			//printf("XADC FIFO Pass \r\n");
		}


	}

	return 0;
}

