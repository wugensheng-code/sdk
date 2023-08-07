
/********************************* including Files *********************************/
#include "al_gbe_hw.h"

/********************************* Constant Definitions *****************************/


/********************************* Type definitions *********************************/


/********************************* Macros  & Inline Functions Definitions ***********/


/******************************** Function Prototypes ******************************/


/******************************** Variable Prototypes ******************************/

AL_GBE_HwConfigStruct AlGbe_HwConfig[AL_GBE_NUM_INSTANCE] =
{
    {
        .DeviceId       = 0,
        .BaseAddress    = GBE0__BASE_ADDR,
        .InputClockHz   = GBE_CLOCK,
        .IntrNum        = SOC_GBE0_IRQn,
    },

    {
        .DeviceId       = 1,
        .BaseAddress    = GBE1__BASE_ADDR,
        .InputClockHz   = GBE_CLOCK,
        .IntrNum        = SOC_GBE1_IRQn
    },
};