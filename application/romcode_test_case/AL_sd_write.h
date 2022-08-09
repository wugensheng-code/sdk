#ifndef _AL_SD_WRITE_H_
#define _AL_SD_WRITE_H_

#include "AL_mmc.h"
#include "AL_sd.h"

u32 SD_WriteSingleBlock(uint8_t *writebuff, uint32_t WriteAddr, uint16_t BlockSize);
u32 SD_ReadMultiBlocks(uint8_t *readbuff, uint32_t ReadAddr, uint16_t BlockSize, uint32_t BlockNum);
u32 SD_WriteMultiBlocks(uint8_t *writebuff, uint32_t WriteAddr, uint16_t BlockSize, uint32_t BlockNum);

#endif
