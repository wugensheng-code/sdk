
/********************************* including Files *********************************/
#include "al_mpu_hw.h"

/********************************* Constant Definitions *****************************/


/********************************* Type definitions *********************************/


/********************************* Macros  & Inline Functions Definitions ***********/


/******************************** Function Prototypes ******************************/


/******************************** Variable Prototypes ******************************/

AL_MPU_HwConfigStruct AlMpu_HwConfig[AL_MPU_NUM_INSTANCE] =
{
    {
        .DeviceId       = 0,
        .BaseAddress    = MPU_DDRS0_BASE_ADDR,
        .IntrStateId    = 0x1,
    },

    {
        .DeviceId       = 1,
        .BaseAddress    = MPU_DDRS1_BASE_ADDR,
        .IntrStateId    = 0x2,
    },

    {
        .DeviceId       = 2,
        .BaseAddress    = MPU_HPM0_BASE_ADDR,
        .IntrStateId    = 0x4,
    },

    {
        .DeviceId       = 3,
        .BaseAddress    = MPU_HPM1_BASE_ADDR,
        .IntrStateId    = 0x8,
    },

    {
        .DeviceId       = 4,
        .BaseAddress    = MPU_NPU_BASE_ADDR,
        .IntrStateId    = 0x10,
    },

    {
        .DeviceId       = 5,
        .BaseAddress    = MPU_OCMS2_BASE_ADDR,
        .IntrStateId    = 0x20,
    },

    {
        .DeviceId       = 6,
        .BaseAddress    = MPU_APU_BASE_ADDR,
        .IntrStateId    = 0x40,
    }
};