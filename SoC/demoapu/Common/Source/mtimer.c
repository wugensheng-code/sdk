
#include "mtimer.h"
//#include "al_io.h"
//#include "al_printf.h"

u32 Mtimer_Delay(u64 Us)
{
    u64 DelayCnt;
    u64 Start;
    u64 End;
    u64 Tconsumed;

    DelayCnt = Us * 10;
    Start = REG64_READ(COREN205_MTIMER_COUNTER64_LOW);

    do{
        End = REG64_READ(COREN205_MTIMER_COUNTER64_LOW);
        Tconsumed = End - Start;
    } while(Tconsumed < DelayCnt);

    return 0;
}

u32 Mtimer_Init(MtimerParams* Mtimer)
{
    Mtimer->StartPoint = 0;
    Mtimer->CurPoint = 0;
    Mtimer->TimerOut = 0;
    Mtimer->IsTimerOut = 0;
    return 0;
}

u32 Mtimer_Start(MtimerParams* Mtimer, u64 Us)
{
    Mtimer->StartPoint = REG64_READ(COREN205_MTIMER_COUNTER64_LOW);
    Mtimer->CurPoint = Mtimer->StartPoint;
    Mtimer->TimerOut = Us * 10;
    Mtimer->IsTimerOut = 0;
    return 0;
}

u32 Mtimer_TimerOut(MtimerParams* Mtimer)
{
    u64 Tconsumed;
    u32 Ret = 0;
    Mtimer->CurPoint = REG64_READ(COREN205_MTIMER_COUNTER64_LOW);
    Tconsumed = Mtimer->CurPoint - Mtimer->StartPoint;
    if(Tconsumed >= Mtimer->TimerOut){
        Mtimer->IsTimerOut = 1;
        Ret = 1;
    }else{
        Mtimer->IsTimerOut = 0;
        Ret = 0;
    }
    return Ret;
}

u32 Mtimer_IsTimerOut(MtimerParams* Mtimer)
{
    return Mtimer->IsTimerOut;
}