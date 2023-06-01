/*
 * @Author: Guangxiang Rao
 * @Date: 2022-04-22 20:15:18
 * @LastEditors: Guangxiang Rao
 * @LastEditTime: 2022-04-25 17:02:27
 * @Description: ...
 * @FilePath: /alsoc-embedded-DE/bsp/armv8/src/delay.c
 */
/*
 * @Author: Guangxiang Rao
 * @Date: 2022-03-07 14:18:48
 * @LastEditors: Guangxiang Rao
 * @LastEditTime: 2022-03-07 15:58:38
 * @Description: delay function for a35
 * @FilePath: /alsoc-embedded-sw/bsp/common/lib/src/delay.c
 */
#include "type.h"
#include "io.h"
#include "sysregs.h"
/**
 * @desc  : cpu delay by reading system counter
 * @param {u64_t} count
 * @return {*}
 */

static void __delay(u64_t count)
{
	u64_t tEnd, tCur;
	tCur = read_sysreg(CNTPCT_EL0);
	tEnd = tCur +count;
	do {
		tCur = read_sysreg(CNTPCT_EL0);
	} while (tCur < tEnd);
}

/**
 * @desc  : get cpu system counter freq
 * @param {*}
 * @return: system counter freq
 */
static u64_t syscnt_get_freq(void)
{
	unsigned int freq;

	asm volatile(
		"mrs %0, cntfrq_el0"
		: "=r" (freq)
		:
		: "memory");

	return freq;
}

u64 get_SystickTimer(void)
{
	isb();
	return read_sysreg(CNTPCT_EL0);
}

uint64_t get_MTimerOutValue(uint64_t count)
{
	u64_t freq = syscnt_get_freq();
    return (count*(freq/1000000));
}

void _delay_us(u64_t count)
{
    u64_t freq = syscnt_get_freq();
    u64_t delay_cnt = (u64_t)(count*(freq/1000000));
    __delay(delay_cnt);
}

void _delay_ms(u64_t count)
{
    u64_t freq = syscnt_get_freq();
    u64_t delay_cnt = (u64_t)(count*(freq/1000));
    __delay(delay_cnt);
}

void _delay_s(u64_t count)
{
    u64_t freq = syscnt_get_freq();
    u64_t delay_cnt = (u64_t)(count*freq);
    __delay(delay_cnt);
}

u64 get_Us(u64 start, u64 end)
{
    u64_t freq = syscnt_get_freq();
    return ((end - start)/(freq/1000000));
}

void AlDelay_Us(u64 Count)
{
	_delay_us(Count);
}

void AlDelay_Ms(u64 Count)
{
	_delay_ms(Count);
}

void AlDelay_S(u64 Count)
{
	_delay_s(Count);
}

u64 AlDelay_CalcUs(u64 Start, u64 End)
{
	return get_Us(Start, End);
}

u64 AlSys_GetTimer(void)
{
	return get_SystickTimer();
}