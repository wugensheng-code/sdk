
#ifndef __AL_ALC_DDRC_H_
#define __AL_ALC_DDRC_H_

#include "alc_types.h"

u32 dr1x90_ddr_reg_write(u32* useless, u32 addr, u32 data);
u32 dr1x90_ddr_reg_read(u32* useless, u32 addr, u32* data);

void dr1x90_reg_write  (u16 addr, u32 data);
u32  dr1x90_reg_read   (u16 addr);
void dr1x90_reg_update (u16 addr, u32 oldData, u32 newData);
void dr1x90_field_write(u16 addr, u8 offset, u32 mask, u32 data);
u32  dr1x90_field_read (u16 addr, u8 offset, u32 mask);
u32  dr1x90_field_set  (u16 addr, u8 offset, u32 mask, u32 data, u32 oldData);
int  dr1x90_field_wait (u16 addr, u8 offset, u32 mask, u32 expect, u32 timeout);

#define DDRC_ADDR_GPLL       0x0000
#define DDRC_ADDR_DPLL       0x0800
#define DDRC_ADDR_UMCTL2     0x1000
#define DDRC_ADDR_PPC        0x2000
#define DDRC_ADDR_BK0_IOMC1  0x3000
#define DDRC_ADDR_BK1_IOMC1  0x3800
#define DDRC_ADDR_BK0_VREF   0x4000
#define DDRC_ADDR_BK1_VREF   0x6000
#define DDRC_ADDR_BK0_T0     0x8000
#define DDRC_ADDR_BK0_T1     0x9000
#define DDRC_ADDR_BK0_T2     0xa000
#define DDRC_ADDR_BK0_T3     0xb000
#define DDRC_ADDR_BK1_T0     0xc000
#define DDRC_ADDR_BK1_T1     0xd000
#define DDRC_ADDR_BK1_T2     0xe000
#define DDRC_ADDR_BK1_T3     0xf000

extern u32 ftcHandle;

#endif // end of AL_ALC_DDRC_H

