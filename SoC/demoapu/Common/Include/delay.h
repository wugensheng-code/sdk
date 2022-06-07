#ifndef __ALSOC_APU_DELAY_H__
#define __ALSOC_APU_DELAY_H__

//#include "type.h"

/**
 * @desc  : Second delay by cpu
 * @param {u64_t} count
 * @return {*}
 */
void delay_s(u64_t count);

/**
 * @desc  : Millisecond delay by cpu
 * @param {u64_t} count
 * @return {*}
 */
void delay_ms(u64_t count);

/**
 * @desc  : microsecond delay by cpu
 * @param {u64_t} count
 * @return {*}
 */
void delay_us(u64_t count);


#endif /* __ALSOC_APU_DELAY_H__ */
