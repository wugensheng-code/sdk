#ifndef __SDIO_TEST_H
#define __SDIO_TEST_H

#include "AL_sdio_emmc_common.h"

//EMMC ONLY ERROR   201~300
#define SDIO_GET_VALID_VOLTAGE_TIMEOUT_ERROR    201

#define SDIO_GET_VALID_VOLTAGE_TIMEOUT_VAL      (1000*1000)  //1s

u32 CardDetection();
u32 SendInitCmdSD();
u32 SwitchDataWidthSD();
u32 SD_WaitReadOperation(volatile DWC_mshc_block_registers* ptr);

/*****************************END OF FILE**************************/

#endif
