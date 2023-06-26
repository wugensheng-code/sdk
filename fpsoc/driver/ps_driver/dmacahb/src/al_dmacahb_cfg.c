#include "al_dmacahb_hw.h"

AL_DMACAHB_HwConfigStruct AlDmacAhb_HwConfig[AL_DMACAHB_NUM_INSTANCE] =
{
    {
        .DeviceId       = 0,
        .BaseAddress    = DMACAHB__BASE_ADDR,
        .ChannelNum     = DMACAHB_CH_NUM,
        .IntrId         = SOC_DMACAHB_IRQn,
        .LockAddress    = DMACAHB_LOCK_ADDR,
        .ChStateAddr    = DMACAHB_CH_STATE_ADDR
    }
};