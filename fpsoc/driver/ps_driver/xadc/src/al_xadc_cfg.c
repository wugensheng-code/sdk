
/********************************* including Files *********************************/
#include "al_xadc_hw.h"

/********************************* Constant Definitions *****************************/

AL_XADC_HwConfigStruct AlXadc_HwConfig[AL_XADC_NUM_INSTANCE] =
{
    {
        .DeviceId           = 0,
        .XadcBaseAddress    = XADC_BASE_ADDR,
        .GpBaseAddress      = GP0_BASE_ADDR,
        .IntrNum            = SOC_XADC_IRQn
    }
};