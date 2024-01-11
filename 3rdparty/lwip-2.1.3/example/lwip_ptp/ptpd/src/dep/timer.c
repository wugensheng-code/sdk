/* timer.c */

#include "../ptpd.h"

IntervalTimer *packettimer = NULL;
unsigned int elapsed_ms;

void catchAlarm(UInteger32 time_ms)
{
    elapsed_ms += time_ms;
    DBGVV("catch_alarm: elapsed %d\n", elapsed_ms);
}

void initTimer(PtpClock *ptpClock)
{
    DBG("initTimer\r\n");
    elapsed_ms = 0;
    packettimer = ptpClock->itimer;
}

void timerUpdate(IntervalTimer *packettimer)
{

    int i, delta;

    delta = elapsed_ms;

    if (delta <= 0)
        return;

    for (i = 0; i < TIMER_ARRAY_SIZE; ++i)
    {
        if ((packettimer[i].interval) > 0 && ((packettimer[i].left) -= delta) <= 0)
        {
            packettimer[i].left = packettimer[i].interval;
            packettimer[i].expire = TRUE;
            DBGV("timerUpdate: timer %u expired\n", i);
        }
    }

    elapsed_ms = 0;
}

void timerStop(UInteger16 index)
{
    if (index >= TIMER_ARRAY_SIZE)
        return;

    packettimer[index].interval = 0;
}

void timerStart(UInteger16 index, UInteger32 interval_ms)
{
    if (index >= TIMER_ARRAY_SIZE)
        return;

    packettimer[index].expire = FALSE;

    packettimer[index].left = interval_ms;

    packettimer[index].interval = packettimer[index].left;

    DBGV("timerStart: set timer %d to %d\n", index, interval_ms);
}

char timerExpired(UInteger16 index)
{
    timerUpdate(packettimer);

    if (index >= TIMER_ARRAY_SIZE)
        return FALSE;

    if (!packettimer[index].expire)
        return FALSE;

    packettimer[index].expire = FALSE;

    return TRUE;
}
