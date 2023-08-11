#include <stdint.h>
#include <stdio.h>

#include "al_log.h"
#include "al_core.h"
#include "al_chip.h"


AL_U64 SystemCoreClock = SYSTEM_CLOCK;  /* System Clock Frequency (Core Clock) */


/**
 * \brief early init function before main
 * \details
 * This function is executed right before main function.
 * For RISC-V gnu toolchain, _init function might not be called
 * by __libc_init_array function, so we defined a new function
 * to do initialization
 */
void _premain_init(void)
{
    AlChip_Init();

    AlLog_Init();
}
