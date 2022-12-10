/*
 * @Author: Guangxiang Rao
 * @Date: 2022-01-13 14:32:11
 * @LastEditors: Guangxiang Rao
 * @LastEditTime: 2022-05-02 10:07:42
 * @Description: ...
 * @FilePath: /alsoc-embedded-DE/apps/generic_timer_test/main.c
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

static int fiq_happened = 0;
static int irq_happened = 0;


unsigned int gic_fiq_get_int_id(void)
{
	unsigned int int_id = gic_read_iar0_common() & 0xffffff;
	return int_id;
}

void generic_timer_irq_handler(void)
{
	u32 generic_timer_rate = generic_timer_get_freq();
	generic_timer_rate *= 1;
	generic_timer_reset(generic_timer_rate);

	printf("irq handler: generic_timer_irq_handler\n");
	printf("delay 1s\n");

	irq_happened = 1;
}


void generic_timer_fiq_handler(void)
{
	u32 generic_timer_rate = generic_timer_get_freq();
	generic_timer_rate *= 1;
	generic_timer_reset(generic_timer_rate);

	printf("fiq handler: generic_timer_fiq_handler\n");
	printf("delay 1s\n");

	fiq_happened = 1;
}


void generic_irq_specail_handler(void)
{
	u32 generic_timer_rate = generic_timer_get_freq();
	generic_timer_rate *= 1;
	generic_timer_reset(generic_timer_rate);

	printf("special irq: generic_irq_specail_handler \n");
	printf("delay 1s\n");

	irq_happened = 1;
}


void generic_fiq_specail_handler(void)
{
	u32 generic_timer_rate = generic_timer_get_freq();
	generic_timer_rate *= 1;
	generic_timer_reset(generic_timer_rate);

	printf("special irq: generic_fiq_specail_handler \n");
	printf("delay 1s\n");

	fiq_happened = 1;
}


void gicv3_init(void)
{
    gicv3_dist_init(GROUP_0);
    gicv3_redist_init(GROUP_0);

    gicv3_cpu_init();

    printf("set generic timer (irq 30) to gic group_0, expected FIQ \n");
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

	request_irq(30, generic_timer_irq_handler);
	request_irq(1023, generic_irq_specail_handler);
	request_fiq(1023, generic_fiq_specail_handler);
	request_fiq(30, generic_timer_fiq_handler);

	/* enable irq after all devices are ready */
	irq_enable();

	printf("system initial complete.\n");

	int sgi_igroup0 = readl_relaxed(GICR_SGI_IGROUPR0);
	int sgi_igrpmod0 = readl_relaxed(GICR_SGI_IGRPMOD0);

	while (fiq_happened == 0 && irq_happened == 0) {
	    ;
	}

	if (fiq_happened && sgi_igroup0 == 0 && sgi_igrpmod0 == 0)
		printf("pass \n	fiq_happened=%d irq_happened=%d sgi_igroup0=%x sgi_igrpmod0=%x\n", fiq_happened, irq_happened, sgi_igroup0, sgi_igrpmod0);
	else
		printf("fail \n fiq_happened=%d irq_happened=%d sgi_igroup0=%x sgi_igrpmod0=%x\n", fiq_happened, irq_happened, sgi_igroup0, sgi_igrpmod0);

	return 0;
}
