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


static int vtimer_irq_happened = 0;
static int hptimer_irq_happened = 0;

void generic_vtimer_irq_handler(void)
{
	vtimer_irq_happened = 1;

	write_sysreg_s(0x00, SYS_CNTV_CTL_EL0);
	printf("vtimer pass \r\n");
}


void generic_hptimer_irq_handler(void)
{
	hptimer_irq_happened = 1;

	printf("hptimer pass \r\n");

	irq_disable();

	while (1) {

	}
}


/**
 * @desc  : main function call by startup64.S
 * boot flow: setup_vector --> clear bss --> lowlevel_init --> set sp --> main
 * @param {*}
 * @return {*}
 */
int main()
{
	printf("\n system initial start\n");

	irq_enable();

	request_irq(27, generic_vtimer_irq_handler);

	write_sysreg_s(0x01, SYS_CNTV_CTL_EL0);
	while (vtimer_irq_happened == 0) {
	    ;
	}

	request_irq(26, generic_hptimer_irq_handler);
	while (hptimer_irq_happened == 0) {
	    ;
	}

	return 0;
}
