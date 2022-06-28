/*
 * main.c
 *
 *  Created on: 2022年5月29日
 *      Author: jian.huang
 */
#include "al9000_xadc.h"

void xadc_handler(void)
{
	printf("i\r\n");

	if(XadcRead(ADC_FLAG) & 0x01)
	{
		printf("v:%d \r\n",XadcRead(ADC_VC0));

		XadcWrite(ADC_CONFIG3, (NO_OPERATION << REG_ADC_SOC_OFFSET)|
					(0 << CH_SEL_OFFSET)|
					(SIGNAL_CH_MODE << MODE_SEL_OFFSET));

		XadcWrite(ADC_CONFIG3, (START_CONVER << REG_ADC_SOC_OFFSET)|
					(0 << CH_SEL_OFFSET)|
					(SIGNAL_CH_MODE << MODE_SEL_OFFSET));

		XadcWrite(ADC_CONFIG1, (1 << INTR_DONE_OFFSET)|
					(1 << INTR_ERROR_MASK_OFFSET)|
					(1 << INTR_LTH_MASK_OFFSET)|
					(1 << INTR_GTH_MASK_OFFSET)|
					(0 << INTR_DONE_MASK_OFFSET));
	}
}


int main()
{
	/* Note:seldiff control signal must remain unchanged when a conversion is being done. */

	/* Note:selbg and selref control signals must remain unchanged after a startup sequence.
	 * A new startup sequence must be made for any change in these control signals. */


/*
#define PIN_MUX_BASE	(0xF8803000UL)

#define PS_MIO0			(0x00+PIN_MUX_BASE)
#define PS_MIO1			(0x04+PIN_MUX_BASE)
#define PS_MIO2			(0x08+PIN_MUX_BASE)
#define PS_MIO3			(0x0c+PIN_MUX_BASE)
#define PS_MIO4			(0x10+PIN_MUX_BASE)
#define PS_MIO5			(0x14+PIN_MUX_BASE)
#define PS_MIO6			(0x18+PIN_MUX_BASE)
#define PS_MIO7			(0x1c+PIN_MUX_BASE)
#define PS_MIO8			(0x20+PIN_MUX_BASE)
#define PS_MIO9			(0x24+PIN_MUX_BASE)
#define PS_MIO10		(0x28+PIN_MUX_BASE)
#define PS_MIO11		(0x2c+PIN_MUX_BASE)
#define PS_MIO12		(0x30+PIN_MUX_BASE)
#define PS_MIO13		(0x34+PIN_MUX_BASE)
#define PS_MIO14		(0x38+PIN_MUX_BASE)
#define PS_MIO15		(0x3c+PIN_MUX_BASE)
#define PS_MIO16		(0x40+PIN_MUX_BASE)
#define PS_MIO17		(0x44+PIN_MUX_BASE)
#define PS_MIO18		(0x48+PIN_MUX_BASE)
#define PS_MIO19		(0x4c+PIN_MUX_BASE)
#define PS_MIO20		(0x50+PIN_MUX_BASE)
#define PS_MIO21		(0x54+PIN_MUX_BASE)
#define PS_MIO22		(0x58+PIN_MUX_BASE)
#define PS_MIO23		(0x5c+PIN_MUX_BASE)
#define PS_MIO24		(0x60+PIN_MUX_BASE)
#define PS_MIO25		(0x64+PIN_MUX_BASE)
#define PS_MIO26		(0x68+PIN_MUX_BASE)
#define PS_MIO27		(0x6c+PIN_MUX_BASE)
#define PS_MIO28		(0x70+PIN_MUX_BASE)

	SMC_WriteReg(PS_MIO0, 0x02);
	SMC_WriteReg(PS_MIO1, 0x02);
	SMC_WriteReg(PS_MIO2, 0x02);
	SMC_WriteReg(PS_MIO3, 0x02);
	SMC_WriteReg(PS_MIO4, 0x02);
	SMC_WriteReg(PS_MIO5, 0x02);
	SMC_WriteReg(PS_MIO6, 0x02);
	SMC_WriteReg(PS_MIO7, 0x02);
	SMC_WriteReg(PS_MIO8, 0x02);
	SMC_WriteReg(PS_MIO9, 0x02);

	SMC_WriteReg(PS_MIO10, 0x02);
	SMC_WriteReg(PS_MIO11, 0x02);
	SMC_WriteReg(PS_MIO12, 0x02);
	SMC_WriteReg(PS_MIO13, 0x02);
	SMC_WriteReg(PS_MIO14, 0x02);
	SMC_WriteReg(PS_MIO15, 0x02);
	SMC_WriteReg(PS_MIO16, 0x02);
	SMC_WriteReg(PS_MIO17, 0x02);
	SMC_WriteReg(PS_MIO18, 0x02);
	SMC_WriteReg(PS_MIO19, 0x02);



	SMC_WriteReg(PS_MIO20, 0x02);
	SMC_WriteReg(PS_MIO21, 0x02);
	SMC_WriteReg(PS_MIO22, 0x02);
	SMC_WriteReg(PS_MIO23, 0x02);
*/

	ECLIC_Register_IRQ(XADC_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 0, 1, xadc_handler);
	__enable_irq();

	XadcInit();

	while(1);

	return 0;
}


