#ifndef __MTIMER_H__
#define __MTIMER_H__

//#include "al_types.h"
#include <stdint.h>

//typedef unsigned int uint32_t;
typedef unsigned long long AL_U64;

typedef struct _MtimerParams{
    AL_U64     StartPoint;
    AL_U64     CurPoint;
    AL_U64     TimerOut;
    uint32_t     IsTimerOut;
}MtimerParams;

uint32_t Mtimer_Delay(AL_U64 Us);

uint32_t Mtimer_Init(MtimerParams* Mtimer);
uint32_t Mtimer_Start(MtimerParams* Mtimer, AL_U64 Us);
uint32_t Mtimer_TimerOut(MtimerParams* Mtimer);
uint32_t Mtimer_IsPeriod(MtimerParams* Mtimer);
uint32_t Mtimer_IsTimerOut(MtimerParams* Mtimer);

static inline AL_U64 REG64_READ(unsigned long Address)
{
    return *(volatile AL_U64 *) Address;
}

#define MTIMER_OUT_CONDITION(us, timer, condition) \
        Mtimer_Start(timer, us); \
        while((condition) && (Mtimer_TimerOut(timer) == 0))

#endif /* _AL9000_MTIMER_H_ */
