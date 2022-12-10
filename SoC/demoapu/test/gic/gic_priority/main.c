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


void generic_ptimer_irq_handler(void)
{
    generic_ptimer_disable();
    ptimer_irq_status = 1;

    if ((ptimer_priority < vtimer_priority) && vtimer_irq_status == 0) {
        printf("Got ptimer first, pass\n");
        result = 1;
    } else if ((ptimer_priority > vtimer_priority) && vtimer_irq_status == 1) {
        //
    } else {
        printf("Got ptimer first, fail\n");
        result = 0;
    }
}

void generic_vtimer_irq_handler(void)
{
    generic_vtimer_disable();
    vtimer_irq_status = 1;

    if ((ptimer_priority > vtimer_priority) && ptimer_irq_status == 0) {
        printf("Got vtimer first, pass\n");
        result = 1;
    } else if((ptimer_priority < vtimer_priority) && ptimer_irq_status == 1) {
        //
    } else {
        printf("Got vtimer first, fail\n");
        result = 0;
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
int main()
{
    int round = 0;
    while (round < 200)
    {
        if (round & 0x01) {
            ptimer_priority = 0x20;
            vtimer_priority = 0x30;
        } else {
            ptimer_priority = 0x30;
            vtimer_priority = 0x20;
        }

        ptimer_irq_status = 0;
        vtimer_irq_status = 0;

        printf("round [%d] start\n", round);
        printf("ptimer_priority = %d, vtimer_priority = %d \n", ptimer_priority, vtimer_priority);

        request_irq(25, generic_vtimer_irq_handler);
        request_irq(26, generic_vtimer_irq_handler);
        request_irq(27, generic_vtimer_irq_handler);
        request_irq(28, generic_vtimer_irq_handler);
        request_irq(29, generic_vtimer_irq_handler);
        request_irq(30, generic_ptimer_irq_handler);

        //round 1: vtimer priority is higher than ptimer priority
        *(volatile unsigned int *)0xf8e05000UL = 0x0;

        irq_disable();

        generic_ptimer_disable();
        generic_vtimer_disable();

        cntp_cntv_timer_set_timeout(0x10);

        /* register generic timer interrupt handler */

        //request_irq(27, generic_vtimer_irq_handler);
        //request_irq(30, generic_ptimer_irq_handler);

        gicv3_set_irq_priority(27, vtimer_priority);
        gicv3_set_irq_priority(30, ptimer_priority);

        cntp_cntv_timer_enable();

        /* enable irq after all devices are ready */
        irq_enable();

        //start timer clock
        *(volatile unsigned int *)0xf8e05000UL = 0x1;

        while (1) {
            ;
        }

        round ++;

        printf("round [%d] end\n\n\n\n", round);

        if (result != 1) {
            printf("test fail, exit");
             break;
        }
    }

    return 0;
}
