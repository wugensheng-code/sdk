#ifndef __AL_SD_H__
#define __AL_SD_H__

#include "al_mmc.h"

#define SD_ACMD6_PARA_1BIT_MODE     0x0
#define SD_ACMD6_PARA_4BIT_MODE     0x2
#define SD_CMD8_PARA_2V7_3V6        0x100

#define SD_GET_VALID_VOLTAGE_TIMEOUT_VAL      (20000*1000)  //20s



typedef struct{
    uint32_t SDId;     //emmc device ID
    uint32_t SDSize;   //emmc size in kBytes
}RawSDParam_t;

extern SD_CardInfo SDCardInfo;

uint32_t CardDetection();
uint32_t AlSd_SendInitCmdSD();
uint32_t AlSd_SwitchDataWidthSD();
uint32_t AlSd_WaitReadOperation(volatile DWC_mshc_block_registers* ptr);
uint32_t AlSd_Init(void);
uint32_t AlSd_ReadSingleBlock(uint8_t *readbuff, uint32_t ReadAddr, uint16_t BlockSize);
uint32_t AlSd_GetCardInfo(SD_CardInfo *cardinfo);
uint32_t Csu_RawSdSetMode(uint32_t Mode, uint32_t Data);
/*****************************END OF FILE**************************/

#endif
