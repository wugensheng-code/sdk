#ifndef _AL9000_MTIMER_H_
#define _AL9000_MTIMER_H_

//#include "al_types.h"

typedef unsigned int u32;
typedef unsigned long long u64;

#define COREN205_MTIMER_COUNTER64_LOW   (0x68000)
#define COREN205_MTIMER_COUNTER64_HI    (0x68004)

typedef struct _MtimerParams{
    u64     StartPoint;
    u64     CurPoint;
    u64     TimerOut;
    u32     IsTimerOut;
}MtimerParams;

u32 Mtimer_Delay(u64 Us);

u32 Mtimer_Init(MtimerParams* Mtimer);
u32 Mtimer_Start(MtimerParams* Mtimer, u64 Us);
u32 Mtimer_TimerOut(MtimerParams* Mtimer);
u32 Mtimer_IsPeriod(MtimerParams* Mtimer);
/*
static inline u64 REG64_READ(UINTPTR Address)
{
    return *(volatile u64 *) Address;
}

#define MTIMER_OUT_CONDITION(us, timer, condition) \
        Mtimer_Start(timer, us); \
        while((condition) && (Mtimer_TimerOut(timer) == 0))
*/
#endif /* _AL9000_MTIMER_H_ */
