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

uint8_t XadcInit(void)
{
	INTR_MASK_TypeDef intr_mask;
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


	cfg.bitfiled.ENABLE_PS_ADC = ENABLE_ADC_PS_ACCESS;
	//cfg.bitfiled.TCK_RATE = QQ_XADC_PCLK;
	XADC->CFG |= cfg.reg;
	printf("XADC->CFG:%x \r\n",XADC->CFG);

	printf("Status:%x\r\n",XADC->STATUS);
	printf("INTR:%x\r\n",XADC->INTR);
	printf("ADC_FLAG:%x \r\n",XadcRead(ADC_FLAG));
	printf("ADC_CONFIG1:%x \r\n",XadcRead(ADC_CONFIG1));

	intr_mask.reg = XADC->INTR_MASK;
	intr_mask.bitfiled.MASK_INTR = ENABLE_SAMPLE_INTER;
	XADC->INTR_MASK = intr_mask.reg;

	printf("XADC->INTR_MASK:%x \r\n",XADC->INTR_MASK);

	printf("end\r\n");



	printf("XADC->CTRL:%x \r\n",XADC->CTRL);

	/* ???  */
	//XadcWrite(ADC_CHANNEL_0_1, (IO_MUX_0 << OFFSET_1ST)| (IO_MUX_1 << OFFSET_2ND));


	XadcWrite(ADC_CONFIG0, (REG_SEL << REG_PIB_SEL_OFFSET)|
				(MUX_ENABLE << MUX_ENBALE_OFFSET)|
				(NORMAL_MODE << PHY_EXTERNAL_M_OFFSET)|
				(SINGLE_MODE << INPUT_M_OFFSET)|
				(CURRENT_RESOLUTION << RES_OFFSET)|
				(REF_INTER_REF << REF_OFFSET)|
				(BAG_ACTIVE << BAG_SEL_OFFSET)|
				(ADC_SW_RESET << SW_RESET_OFFSET));

	/* Enable ADC PS Access */


	printf("e0\r\n");
	XadcWrite(ADC_CONFIG0, (REG_SEL << REG_PIB_SEL_OFFSET)|
			(MUX_ENABLE << MUX_ENBALE_OFFSET)|
			(NORMAL_MODE << PHY_EXTERNAL_M_OFFSET)|
			(SINGLE_MODE << INPUT_M_OFFSET)|
			(CURRENT_RESOLUTION << RES_OFFSET)|
			(REF_INTER_REF << REF_OFFSET)|
			(BAG_ACTIVE << BAG_SEL_OFFSET)|
			(ADC_RELEASE << SW_RESET_OFFSET));
	printf("e1\r\n");
	XadcWrite(ADC_CONFIG2, (ADC_PL_CLK_70M << ADC_CLK_SEL_OFFSET)|
				(1 << ADC_ENABLE_OFFSET));
	printf("e2\r\n");


	printf("e3\r\n");

	XadcWrite(ADC_CONFIG1, (1 << INTR_ERROR_MASK_OFFSET)|
					(1 << INTR_LTH_MASK_OFFSET)|
					(1 << INTR_GTH_MASK_OFFSET)|
					(0 << INTR_DONE_MASK_OFFSET));
	XadcWrite(ADC_CONFIG3, (START_CONVER << REG_ADC_SOC_OFFSET)|
				(CURRENT_CH << CH_SEL_OFFSET)|
				(CURRENT_MODE << MODE_SEL_OFFSET));

	printf("e\r\n");

/*
	printf("Config0:%x \r\n", XadcRead(ADC_CONFIG0));
	printf("Config1:%x \r\n", XadcRead(ADC_CONFIG1));
	printf("Config2:%x \r\n", XadcRead(ADC_CONFIG2));
	printf("Config3:%x \r\n", XadcRead(ADC_CONFIG3));
*/
	return 0;
}

void xadc_handler(void)
{
	printf("i\r\n");

	XadcStatus = XADC->STATUS;

	printf("Status:%x\r\n",XadcStatus);

	if(XadcStatus&0x01)
	{
		//printf("s\r\n");
		//printf("data fifo level %d\r\n",XADC->STATUS);
		XadcConfig1 = XadcRead(ADC_CONFIG1);
		printf("XadcConfig1:%x \r\n",XadcConfig1);

		/* ADC Done Flag */
		if(XadcConfig1 & (0x01 << 8))
		{
			value = XadcRead(ADC_VC0);
			printf("V0:%x\r\n",value >> 4);

			value = XadcRead(ADC_VC1);
			printf("V1:%x\r\n",value >> 4);

			value = XadcRead(ADC_VC2);
			printf("V2:%x\r\n",value >> 4);

			value = XadcRead(ADC_VC3);
			printf("V3:%x\r\n",value >> 4);

			value = XadcRead(ADC_VC4);
			printf("V4:%x\r\n",value >> 4);

			value = XadcRead(ADC_VC5);
			printf("V5:%x\r\n",value >> 4);

			value = XadcRead(ADC_VC6);
			printf("V6:%x\r\n",value >> 4);

			value = XadcRead(ADC_VC7);
			printf("V7:%x\r\n",value >> 4);

			/*
			XadcWrite(ADC_CONFIG3, (NO_OPERATION << REG_ADC_SOC_OFFSET)|
									(CURRENT_CH << CH_SEL_OFFSET)|
									(CURRENT_MODE << MODE_SEL_OFFSET));
*/

			/* Clear PL interrupt Signal */
			XadcWrite(ADC_CONFIG1, (1 << INTR_DONE_OFFSET)|
							(1 << INTR_ERROR_MASK_OFFSET)|
							(1 << INTR_LTH_MASK_OFFSET)|
							(1 << INTR_GTH_MASK_OFFSET)|
							(0 << INTR_DONE_MASK_OFFSET));

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
			printf("ADC Error\r\n");
		}
		/* ADC_VC* value lower than ADC_CH*_LTH Interrupt */
		else if(XadcConfig1 & (0x01 << 10))
		{
			printf("ADC_VC* value lower than ADC_CH*_LTH\r\n");
		}
		/* ADC_VC* value greater than ADC_CH*_GTH Interrupt */
		else if(XadcConfig1 & (0x01 << 9))
		{
			printf("ADC_VC* value greater than ADC_CH*_GTH\r\n");
		}



		/* Clear PS interrupt Signal */
		XADC->INTR |= 0x1;
	}

	/* Data FIFO less level threshold interrupt. */
	else if(XadcStatus&(0x01 << 8))
	{
		printf("Data FIFO less level threshold interrupt.\r\n");
		/* Clear PS interrupt Signal */
		XADC->INTR |= (0x1 << 8);
	}
	/* Command FIFO less level threshold interrupt. */
	else if(XadcStatus&(0x01 << 9))
	{
		printf("Command FIFO less level threshold interrupt.\r\n");
		/* Clear PS interrupt Signal */
		XADC->INTR |= (0x1 << 9);
	}
	/* Data FIFO greater level threshold interrupt. */
	else if(XadcStatus&(0x01 << 10))
	{
		printf("Data FIFO greater level threshold interrupt.\r\n");
		/* Clear PS interrupt Signal */
		XADC->INTR |= (0x1 << 10);
	}
	/* Command FIFO greater level threshold interrupt. */
	else if(XadcStatus&(0x01 << 11))
	{
		printf("Command FIFO greater level threshold interrupt.\r\n");
		/* Clear PS interrupt Signal */
		XADC->INTR |= (0x1 << 11);
	}


	/*
	XadcWrite(ADC_CH0_GTH, );
	XadcWrite(ADC_CH1_GTH, );
	XadcWrite(ADC_CH2_GTH, );
	XadcWrite(ADC_CH3_GTH, );
	XadcWrite(ADC_CH4_GTH, );
	XadcWrite(ADC_CH5_GTH, );
	XadcWrite(ADC_CH6_GTH, );
	XadcWrite(ADC_CH7_GTH, );

	XadcWrite(ADC_CH0_LTH, );
	XadcWrite(ADC_CH1_LTH, );
	XadcWrite(ADC_CH2_LTH, );
	XadcWrite(ADC_CH3_LTH, );
	XadcWrite(ADC_CH4_LTH, );
	XadcWrite(ADC_CH5_LTH, );
	XadcWrite(ADC_CH6_LTH, );
	XadcWrite(ADC_CH7_LTH, );

	XadcConfig1 = XadcRead(ADC_CONFIG1);
	if()
	{

	}

	*/

}






int main()
{

	volatile uint32_t k = 0;
	/* Note:seldiff control signal must remain unchanged when a conversion is being done. */

	/* Note:selbg and selref control signals must remain unchanged after a startup sequence.
	 * A new startup sequence must be made for any change in these control signals.*/

	printf("hi\r\n");
	ECLIC_Register_IRQ(XADC_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 0, 1, xadc_handler);

	__enable_irq();
	XadcInit();
	//printf("ADC_CONFIG1:%x \r\n",XadcRead(ADC_CONFIG1));
	//printf("ADC_CONFIG2:%x \r\n",XadcRead(ADC_CONFIG2));


	while(1);

	return 0;
}


