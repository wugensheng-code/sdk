#include "alsecure_misc.h"

extern uint64_t get_SystickTimer(void);
extern uint64_t switch_TimeToUs(uint64_t time);

uint64_t AlSecure_GetCurTime(void)
{
    return get_SystickTimer();
}

uint64_t AlSecure_CalcTimeToUs(uint64_t StartTime, uint64_t EndTime)
{
    return (switch_TimeToUs(EndTime) - switch_TimeToUs(StartTime));
}