#ifndef _AL_EMMC_WRITE_H_
#define _AL_EMMC_WRITE_H_

#include "AL_emmc.h"
#include "AL_mmc.h"


u32 EMMC_WriteSingleBlock(uint8_t *writebuff, uint32_t WriteAddr, uint16_t BlockSize);

#endif
