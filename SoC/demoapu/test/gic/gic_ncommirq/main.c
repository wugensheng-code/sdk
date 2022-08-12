#include <stdio.h>
#include "config.h"
#include "gic.h"
#include "delay.h"
#include "type.h"
#include "timer.h"
#include "sysreg.h"

// 5499 armv8_arm.pdf
// <systemreg> op0 op1 CRn CRm op2
// DBGDTRTX_EL0 10 011 0000 0101 000

#define N_COMM_IRQ		(22)

static int irq_happened = 0;

/**
 * @desc  : main function call by startup64.S
 * boot flow: setup_vector --> clear bss --> lowlevel_init --> set sp --> main
 * @param {*}
 * @return {*}
 */
void ncomm_irq_handler(void)
{
	unsigned int value;

	printf("ncomm_irq_handler \r\n");

	value = read_sysreg_s(SYS_MDCCSR_EL0);
	if (value & (0x01 << 29)) {
		printf("DTRTX full is set\r\n");
	} else {
		printf("DTRTX full is empty\r\n");
	}

	printf("write SYS_DBGDTRTX_EL0\r\n");
	write_sysreg_s(0x01, SYS_DBGDTRTX_EL0);
	irq_happened = 1;

	value = read_sysreg_s(SYS_MDCCSR_EL0);
	if (value & (0x01 << 29)) {
		printf("DTRTX full is set\r\n");
	} else {
		printf("DTRTX full is empty\r\n");
	}
}


int main()
{
	int data = 0;

	request_irq(N_COMM_IRQ, ncomm_irq_handler);

	/* enable irq after all devices are ready */
	irq_enable();

	write_sysreg_s((0x01 << 30) | (0x01 << 29), SYS_MDCCINT_EL1);

	while(irq_happened == 0) {
	}

	printf("test pass \r\n");

	return 0;
}
