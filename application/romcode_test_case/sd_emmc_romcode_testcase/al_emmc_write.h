#ifndef _AL_EMMC_WRITE_H_
#define _AL_EMMC_WRITE_H_

#include "al_emmc.h"
#include "al_mmc.h"


uint32_t AlEmmc_WriteSingleBlock(uint8_t *Writebuff, uint32_t WriteAddr, uint16_t BlockSize);

#endif
