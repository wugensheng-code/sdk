#ifndef _AL_SD_WRITE_H_
#define _AL_SD_WRITE_H_

#include "al_mmc.h"
#include "al_sd.h"

uint32_t AlSd_WriteSingleBlock(uint8_t *writebuff, uint32_t WriteAddr, uint16_t BlockSize);

#endif
