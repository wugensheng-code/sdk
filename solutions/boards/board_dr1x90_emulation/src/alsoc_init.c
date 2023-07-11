#include "al_core.h"

extern void Enablepinmux1(void);
extern void Enablepinmux1_mode2(void);

__attribute__((__used__)) void board_init()
{
#if ENABLE_PINMUX_MODE1 == 1
    Enablepinmux1();
#endif

#if ENABLE_PINMUX_MODE2 == 1
    Enablepinmux1_mode2();
#endif
}

/*
 * generate by FD
*/
int get_device_version ()
{
    return AL_OK;
}


int soc_platform_init()
{
    return AL_OK;
}
