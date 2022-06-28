/*
 * @Author: Guangxiang Rao
 * @Date: 2022-01-17 16:28:23
 * @LastEditors: Guangxiang Rao
 * @LastEditTime: 2022-05-04 08:57:25
 * @Description: ...
 * @FilePath: /alsoc-embedded-DE/bsp/drives/timer/src/generic_timer.c
 */

#include <io.h>
#include <timer.h>
#include <type.h>
#include <gic.h>
#include <stdio.h>

static unsigned int generic_timer_rate;

/**
 * @desc  : enable generic timer
 * @param {*}
 * @return {*}
 */
static int generic_timer_enable(void)
{
	unsigned int v = 1;
	asm volatile(
		"msr cntp_ctl_el0, %x[v]"
		:
		: [v]"r"(v)
		: "memory");
	return 0;
}

/**
 * @desc  : reset generic timer tval
 * @param {unsigned int} val
 * @return {*}
 */
int generic_timer_reset(unsigned int val)
{
	asm volatile(
		"msr cntp_tval_el0, %x[timer_val]"
		:
		: [timer_val] "r" (val)
		: "memory");

	return 0;
}

unsigned int generic_timer_get_freq(void)
{
	unsigned int freq;

	asm volatile(
		"mrs %0, cntfrq_el0"
		: "=r" (freq)
		:
		: "memory");

	return freq;
}


void generic_timer_init(void)
{
	generic_timer_rate = generic_timer_get_freq();

	// printf("cntp freq:0x%x\r\n", generic_timer_rate);
	// generic_timer_rate /= HZ;

	writel_relaxed(0x1,(void *)0xf8e05000UL);

	generic_timer_enable();
	generic_timer_reset(generic_timer_rate);

	// enable the irq until call register_irq to install irq handler
	//gicv3_enable_irq(GENERIC_TIMER_IRQ);
}

