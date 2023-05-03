/*
 * @Author: Guangxiang Rao
 * @Date: 2022-04-29 00:00:37
 * @LastEditors: Guangxiang Rao
 * @LastEditTime: 2022-05-01 22:38:40
 * @Description: ...
 * @FilePath: /alsoc-embedded-DE/bsp/drives/timer/inc/timer.h
 */
#ifndef __TIMER_H
#define __TIMER_H

#define HZ 1
#define KHZ 1000
#define MHZ 1000000

#define GENERIC_TIMER_IRQ  30

/**
 * @desc  : init the generic timer
 * @param {*}
 * @return {*}
 */
void generic_timer_init(void);

/**
 * @desc  : reset the generic timer
 * @param {unsigned int} val: count value
 * @return {*}
 */
int generic_timer_reset(unsigned int val);

/**
 * @desc  : 
 * @param {*} 
 * @return {unsigned int}: return the timer clock frequency
 */
unsigned int generic_timer_get_freq(void);

#endif
