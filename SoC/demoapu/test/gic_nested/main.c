#include <stdio.h>
#include "config.h"
#include "gic.h"
#include "delay.h"
#include "type.h"
#include "timer.h"

volatile unsigned int ptimer_irq_status = 0;
volatile unsigned int vtimer_irq_status = 0;

unsigned int ptimer_priority, vtimer_priority;
unsigned int result = 0;

int generic_ptimer_disable(void)
{
	unsigned int v = 0;
	asm volatile(
		"msr cntp_ctl_el0, %x[v]"
		:
		: [v]"r"(v)
		: "memory");

	return 0;
}

int generic_vtimer_disable(void)
{
	unsigned int v = 0;
	asm volatile(
		"msr cntv_ctl_el0, %x[v]"
		:
		: [v]"r"(v)
		: "memory");

	return 0;
}


void generic_vtimer_irq_handler(void);

void generic_ptimer_irq_handler(void)
{
	generic_ptimer_disable();
	ptimer_irq_status = 1;

	if (ptimer_priority < vtimer_priority) {
		printf("Get high priority(%d) ptimer irq\n", ptimer_priority);
		printf("waiting low priority vtimer irq, priority: %d \n", vtimer_priority);
	} else {
		printf("Get low priority(%d) ptimer irq\n", ptimer_priority);
		printf("waiting high priority vtimer irq, priority: %d \n", vtimer_priority);
	}

	request_irq(27, generic_vtimer_irq_handler);
	irq_enable();

	while (!vtimer_irq_status) {
		;
	}

	if (ptimer_priority < vtimer_priority) {
		result = 0;
	} else {
		result = 1;
	}

	printf("exit generic_ptimer_irq_handler result: %d \n", result);
}


void generic_vtimer_irq_handler(void)
{
	generic_vtimer_disable();

	vtimer_irq_status = 1;

	if (ptimer_priority > vtimer_priority) {
		printf("Get high priority(%d) \n", vtimer_priority);
	} else {
		printf("Get low priority ptimer_irq_status \n", ptimer_priority);
	}
}


/**
 * @desc  : reset generic timer tval
 * @param {unsigned int} val
 * @return {*}
 */
int cntp_cntv_timer_set_timeout(unsigned int val)
{
	asm volatile(
		"msr cntp_tval_el0, %x[timer_val]"
		:
		: [timer_val] "r" (val)
		: "memory");

	asm volatile(
		"msr cntv_tval_el0, %x[timer_val]"
		:
		: [timer_val] "r" (val)
		: "memory");

	return 0;
}

int cntp_cntv_timer_enable(void)
{
	unsigned int v = 1;
	asm volatile(
		"msr cntp_ctl_el0, %x[v]"
		:
		: [v]"r"(v)
		: "memory");

	asm volatile(
		"msr cntv_ctl_el0, %x[v]"
		:
		: [v]"r"(v)
		: "memory");

	return 0;
}

/**
 * @desc  : main function call by startup64.S
 * boot flow: setup_vector --> clear bss --> lowlevel_init --> set sp --> main
 * @param {*}
 * @return {*}
 */
int main(void)
{
	ptimer_priority = 0x30;
	vtimer_priority = 0x20;

	ptimer_irq_status = 0;
	vtimer_irq_status = 0;

	printf("case 1: \n\n");
	printf("ptimer_priority = %d, vtimer_priority = %d \n", ptimer_priority, vtimer_priority);

	//round 1: vtimer priority is higher than ptimer priority
	*(volatile unsigned int *)0xf8e05000UL = 0x0;

	irq_disable();

	generic_ptimer_disable();
	generic_vtimer_disable();

	cntp_cntv_timer_set_timeout(0x10);

	request_irq(30, generic_ptimer_irq_handler);

	gicv3_set_irq_priority(27, vtimer_priority);
	gicv3_set_irq_priority(30, ptimer_priority);

	cntp_cntv_timer_enable();

	/* enable irq after all devices are ready */
	irq_enable();

	//start timer clock
	*(volatile unsigned int *)0xf8e05000UL = 0x1;

	while (!(ptimer_irq_status && vtimer_irq_status)) {
	    ;
	}

	printf("wait high priority irq %s \n\n\n\n\n", result?"pass":"fail");

	printf("case2 \n\n");
	ptimer_priority = 0x20;
	vtimer_priority = 0x30;

	ptimer_irq_status = 0;
	vtimer_irq_status = 0;

	printf("ptimer_priority = %d, vtimer_priority = %d \n", ptimer_priority, vtimer_priority);

	//round 1: vtimer priority is higher than ptimer priority
	*(volatile unsigned int *)0xf8e05000UL = 0x0;

	irq_disable();

	generic_ptimer_disable();
	generic_vtimer_disable();

	cntp_cntv_timer_set_timeout(0x10);

	request_irq(30, generic_ptimer_irq_handler);

	gicv3_set_irq_priority(27, vtimer_priority);
	gicv3_set_irq_priority(30, ptimer_priority);

	cntp_cntv_timer_enable();

	/* enable irq after all devices are ready */
	irq_enable();

	//start timer clock
	*(volatile unsigned int *)0xf8e05000UL = 0x1;

	while (!(ptimer_irq_status && vtimer_irq_status)) {
	    ;
	}

	return 0;
}
