#ifndef __AL_DEMO_DDR_INIT_H_
#define __AL_DEMO_DDR_INIT_H_

// DDR Init For Demo Board
// DDR3 @ 1066 MHz
// Demo Board 1GB DDR Space: 0x00000000 - 0x3fffffff
int demo_ddr_init();

// Return 1 if DDRC init is done
int ddrc_is_init();

#endif
