#ifndef __SDIO_TEST_H
#define __SDIO_TEST_H

#include "AL_sdio_emmc_common.h"

u32 CardDetection();
u32 SendInitCmdSD();
u32 SwitchDataWidthSD();
u32 SD_WaitReadOperation(volatile DWC_mshc_block_registers* ptr);

/*****************************END OF FILE**************************/

#endif
