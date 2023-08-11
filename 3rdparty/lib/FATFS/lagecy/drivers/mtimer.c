#include "mtimer.h"

extern AL_U64 AlSys_GetTimerTick(void);
extern AL_U64 AlSys_GetTimerFreq();

uint32_t Mtimer_Delay(AL_U64 Usec)
{
    AL_U64 DelayCnt;
    AL_U64 Start;
    AL_U64 End;
    AL_U64 Tconsumed;

    DelayCnt = Usec * AlSys_GetTimerFreq() / 1000000;
    Start = AlSys_GetTimerTick();

    do{
        End = AlSys_GetTimerTick();
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

uint32_t Mtimer_Start(MtimerParams* Mtimer, AL_U64 Usec)
{
    Mtimer->StartPoint = AlSys_GetTimerTick();
    Mtimer->CurPoint = Mtimer->StartPoint;
    Mtimer->TimerOut = (AL_U64)Usec * AlSys_GetTimerFreq() / 1000000;
    Mtimer->IsTimerOut = 0;
    return 0;
}

uint32_t Mtimer_TimerOut(MtimerParams* Mtimer)
{
    AL_U64 Tconsumed;
    uint32_t Ret = 0;
    Mtimer->CurPoint = AlSys_GetTimerTick();
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