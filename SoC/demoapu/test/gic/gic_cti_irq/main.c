/*
 *
 */

#include <stdio.h>
#include "config.h"
#include "gic.h"
#include "delay.h"
#include "io.h"
#include "type.h"
#include "timer.h"
#include "gic_v3.h"
#include "gic_v3_addr.h"


static int cti_irq_happened = 0;

void cti_irq_handler(void)
{
	cti_irq_happened = 1;

	printf("cti_irq_handler happened \r\n");
}


#define DEBUG_BASE_REGISTER		(0xF9000000)
#define CTI_BASE_REGISTER		(DEBUG_BASE_REGISTER + 0x018000)


#define CTI_CONTROL				(0x0)		//
#define CTI_GATE				(0x140)		//Enable CTI Channel Gate register

#define CTI_APPSET				(0x14)
#define CTI_OUTENCTIAPPPULSE	(0x1c)

#define CTI_INEN(n)				(0x20 + (n * 4))
#define CTI_OUTEN(n)			(0xA0 + (n * 4))

#define CTI_LAR					(0xfb0)

#define write_cti_reg(offset, v) *(volatile unsigned int *)((offset) + CTI_BASE_REGISTER) = v
#define read_cti_reg(offset, v)  *(volatile unsigned int *)((offset) + CTI_BASE_REGISTER)


int main()
{
	unsigned long regvalue;

	printf("test cti_irq:::: \r\n");

	request_irq(24, cti_irq_handler);

	irq_enable();

	//////////////
	write_cti_reg(CTI_LAR, 0xc5acce55);			//0xF9018000[0] = 0xC5ACCE55

	write_cti_reg(CTI_CONTROL, 0x01);

	write_cti_reg(CTI_OUTEN(2), (0x1 << 2));	//

	write_cti_reg(CTI_OUTENCTIAPPPULSE, (0x1 << 2));

	while (!cti_irq_happened) {};

	printf("cti irq test pass\r\n");

	return 0;
}
