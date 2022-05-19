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
#include "type.h"
#include "timer.h"

void generic_timer_irq_handler(void)
{
    u32 generic_timer_rate = generic_timer_get_freq();
    generic_timer_rate *= 5;
	generic_timer_reset(generic_timer_rate);
	printf("Core0 Timer interrupt received\n");
	printf("delay 5s\n");
}

/**
 * @desc  : main function call by startup64.S
 * boot flow: setup_vector --> clear bss --> lowlevel_init --> set sp --> main
 * @param {*}
 * @return {*}
 */
int main()
{
    printf("\nsystem initial start\n");

    /* register generic timer interrupt handler */
    request_irq(30, generic_timer_irq_handler);
    /* enable irq after all devices are ready */
    irq_enable();

    printf("system initial complete.\n");
    while (1) {
        ;
    }

    return 0;
}
