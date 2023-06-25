
/********************************* including Files *********************************/
#include "al_qspi_hw.h"

/********************************* Constant Definitions *****************************/


/********************************* Type definitions *********************************/


/********************************* Macros  & Inline Functions Definitions ***********/


/******************************** Function Prototypes ******************************/


/******************************** Variable Prototypes ******************************/

AL_QSPI_HwConfigStruct AlQSPI_HwCfg[AL_QSPI_NUM_INSTANCE] =
{
    {
        .DeviceId           = 0,
        .BaseAddress        = QSPI0_BASE_ADDR,
        .InterrupId         = 85,
        .FifoLen            = 32
    },
    {
        .DeviceId           = 1,
        .BaseAddress        = QSPI1_BASE_ADDR,
        .InterrupId         = 87,
        .FifoLen            = 32
    }
};