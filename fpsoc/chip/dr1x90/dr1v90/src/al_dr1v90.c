#include "al_core.h"
#include "al_rv_core.h"
#include "al_chip.h"
#include "al_rv64_timer.h"

extern void Exception_Init(void);
extern void ECLIC_Init(void);

AL_VOID AlChip_Dr1V90Init(AL_VOID)
{
    Exception_Init();
    ECLIC_Init();

#ifndef RTOS_RTTHREAD
    SysTimer_clk_sel();
#endif
}
