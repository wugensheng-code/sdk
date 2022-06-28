/*
 * al9000_xadc.c
 *
 *  Created on: 2022年5月29日
 *      Author: jian.huang
 */
#include "al9000_xadc.h"

/* XADC Read PL Reg */
uint16_t XadcRead(uint8_t Addr)
{
	uint32_t Cmd = READ_CMD << 24 |
				Addr << 16;

	XADC->CMD = Cmd;

	while(!(XADC->INTR&0x01));

	return (uint16_t)XADC->DATA;
}

/* XADC Write PL Reg */
/* ??没写进去 */
void XadcWrite(uint8_t Addr, uint16_t Data)
{
	uint32_t Cmd = WRITE_CMD << 24 |
				Addr << 16|
				Data;

	XADC->CMD = Cmd;
}



uint8_t XadcInit(void)
{
	INTR_TypeDef intr;
	CFG_TypeDef cfg;

	/* Enable ADC PS Access */
	cfg.bitfiled.ENABLE_PS_ADC = ENABLE_ADC_PS_ACCESS;

/*
	cfg.bitfiled.CMD_FIFOTH =
	cfg.bitfiled.DATA_FIFOOTH =
	cfg.bitfiled.TCK_RATE =

	 *
	 */

	XADC->CFG |= cfg.reg;



	intr.bitfiled.INTR = ENABLE_SAMPLE_INTER;

	/*
	intr.bitfiled.CMD_FIFO_GTH =
	intr.bitfiled.CMD_FIFO_LTH =
	intr.bitfiled.DATA_FIFO_GTH =
	intr.bitfiled.DATA_FIFO_LTH =
	*/

	XADC->INTR_MASK = intr.reg;

	/* ???  */
	XadcWrite(ADC_CHANNEL_0_1, (IO_MUX_2 << OFFSET_1ST)| (IO_MUX_1 << OFFSET_2ND));

	/*   */
	XadcWrite(ADC_CONFIG0, (REG_SEL << REG_PIB_SEL_OFFSET)|
			(MUX_ENABLE << MUX_ENBALE_OFFSET)|
			(NORMAL_MODE << PHY_EXTERNAL_M_OFFSET)|
			(SINGLE_MODE << INPUT_M_OFFSET)|
			(RES_12BIT << RES_OFFSET)|
			(REF_VREF << REF_OFFSET)|
			(BAG_POWERDOWN << BAG_SEL_OFFSET)|
			(ADC_RELEASE << SW_RESET_OFFSET));

	/*   */
	XadcWrite(ADC_CONFIG1, (1 << INTR_ERROR_MASK_OFFSET)|
			(1 << INTR_LTH_MASK_OFFSET)|
			(1 << INTR_GTH_MASK_OFFSET)|
			(0 << INTR_DONE_MASK_OFFSET));

	/*   */
	XadcWrite(ADC_CONFIG2, (ADC_PL_CLK_70M << ADC_CLK_SEL_OFFSET)|
			(1 << ADC_ENABLE_OFFSET));


	/*   */
	XadcWrite(ADC_CONFIG3, (START_CONVER << REG_ADC_SOC_OFFSET)|
			(0 << CH_SEL_OFFSET)|
			(SIGNAL_CH_MODE << MODE_SEL_OFFSET));

	return 0;
}


