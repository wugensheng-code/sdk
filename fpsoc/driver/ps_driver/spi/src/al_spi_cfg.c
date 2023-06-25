
/********************************* including Files *********************************/
#include "al_spi_hw.h"

/********************************* Constant Definitions *****************************/


/********************************* Type definitions *********************************/


/********************************* Macros  & Inline Functions Definitions ***********/


/******************************** Function Prototypes ******************************/


/******************************** Variable Prototypes ******************************/

AL_SPI_HwConfigStruct AlSpi_HwCfg[AL_SPI_NUM_INSTANCE] =
{
    {
        .DeviceId           = 0,
        .BaseAddress        = SPI0_BASE_ADDR,
        .InterrupId         = 85,
        .FifoLen            = 32
    },
    {
        .DeviceId           = 1,
        .BaseAddress        = SPI1_BASE_ADDR,
        .InterrupId         = 87,
        .FifoLen            = 32
    }
};