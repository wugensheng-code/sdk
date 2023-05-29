#include "al_core.h"

extern void Enablepinmux1(void);
extern void Enablepinmux1_mode2(void);

void board_init()
{
#if ENABLE_PINMUX_MODE1 == 1
    Enablepinmux1();
#endif

#if ENABLE_PINMUX_MODE2 == 1
    Enablepinmux1_mode2();
#endif
}



