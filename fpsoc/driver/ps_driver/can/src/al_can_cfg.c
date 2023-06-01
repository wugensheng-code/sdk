#include "al_can_hw.h"

AL_CAN_HwConfigStruct AlCan_HwConfig[AL_CAN_NUM_INSTANCE] = {
    {
        .DeviceId       = 0,
        .BaseAddress    = CAN_CAN0_BASE_ADDR,
        .IntrId         = SOC_CAN0_IRQn
    },
    {
        .DeviceId       = 1,
        .BaseAddress    = CAN_CAN1_BASE_ADDR,
        .IntrId         = SOC_CAN1_IRQn
    }
};