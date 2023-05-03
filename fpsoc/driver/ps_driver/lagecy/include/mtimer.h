#ifndef __MTIMER_H__
#define __MTIMER_H__

//#include "al_types.h"
#include <stdint.h>

//typedef unsigned int uint32_t;
typedef unsigned long long u64;

typedef struct _MtimerParams{
    u64     StartPoint;
    u64     CurPoint;
    u64     TimerOut;
    uint32_t     IsTimerOut;
}MtimerParams;

uint32_t Mtimer_Delay(u64 Us);

uint32_t Mtimer_Init(MtimerParams* Mtimer);
uint32_t Mtimer_Start(MtimerParams* Mtimer, u64 Us);
uint32_t Mtimer_TimerOut(MtimerParams* Mtimer);
uint32_t Mtimer_IsPeriod(MtimerParams* Mtimer);
uint32_t Mtimer_IsTimerOut(MtimerParams* Mtimer);

static inline u64 REG64_READ(unsigned long Address)
{
    return *(volatile u64 *) Address;
}

#define MTIMER_OUT_CONDITION(us, timer, condition) \
        Mtimer_Start(timer, us); \
        while((condition) && (Mtimer_TimerOut(timer) == 0))

#endif /* _AL9000_MTIMER_H_ */
