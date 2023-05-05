#include "al_dmacahb_hw.h"

AL_DMACAHB_HwConfigStruct AlDmacAhb_HwConfig[AL_DMACAHB_NUM_INSTANCE] = 
{
    {
        .DeviceId       = 0,
        .BaseAddress    = DMAC_AHB_BASE_ADDR,
        .ChannelNum     = 8,
        .IntrId         = 65
    }
};