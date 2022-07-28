
#include "mtimer.h"
//#include "al_io.h"
//#include "al_printf.h"

uint32_t Mtimer_Delay(u64 Us)
{
    u64 DelayCnt;
    u64 Start;
    u64 End;
    u64 Tconsumed;

    DelayCnt = get_MTimerOutValue(Us);
    Start = get_SystickTimer();

    do{
        End = get_SystickTimer();
        Tconsumed = End - Start;
    } while(Tconsumed < DelayCnt);

    return 0;
}

uint32_t Mtimer_Init(MtimerParams* Mtimer)
{
    Mtimer->StartPoint = 0;
    Mtimer->CurPoint = 0;
    Mtimer->TimerOut = 0;
    Mtimer->IsTimerOut = 0;
    return 0;
}

uint32_t Mtimer_Start(MtimerParams* Mtimer, u64 Us)
{
    Mtimer->StartPoint = get_SystickTimer();
    Mtimer->CurPoint = Mtimer->StartPoint;
    Mtimer->TimerOut = get_MTimerOutValue(Us);
    Mtimer->IsTimerOut = 0;
    return 0;
}

uint32_t Mtimer_TimerOut(MtimerParams* Mtimer)
{
    u64 Tconsumed;
    uint32_t Ret = 0;
    Mtimer->CurPoint = get_SystickTimer();
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

uint32_t Mtimer_IsTimerOut(MtimerParams* Mtimer)
{
    return Mtimer->IsTimerOut;
}