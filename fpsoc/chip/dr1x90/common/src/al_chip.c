#include <stdint.h>
#include <stdio.h>

#include "al_log.h"
#include "al_core.h"
#include "al_chip.h"


AL_U64 SystemCoreClock = SYSTEM_CLOCK;  /* System Clock Frequency (Core Clock) */

void _premain_init(void)
{
    AlChip_Init();

    AlLog_Init();
}
