#include "al_dmacahb_hw.h"

AL_DMACAHB_HwConfigStruct AlDmacAhb_HwConfig[AL_DMACAHB_NUM_INSTANCE] =
{
    {
        .DeviceId       = 0,
        .BaseAddress    = DMAC_AHB_BASE_ADDR,
        .ChannelNum     = DMAC_AHB_CH_NUM_8,
        .IntrId         = DMAC_AHB_INTR_ID,
        .LockAddress    = DMAC_AHB_LOCK_ADDR,
        .ChStateAddr    = DMAC_AHB_CH_STATE_ADDR
    }
};