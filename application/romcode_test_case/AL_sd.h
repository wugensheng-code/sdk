#ifndef __AL_SD_H__
#define __AL_SD_H__

#include "AL_mmc.h"

//EMMC ONLY ERROR   201~300
#define SD_GET_VALID_VOLTAGE_TIMEOUT_ERROR    201

#define SD_GET_VALID_VOLTAGE_TIMEOUT_VAL      (1000*1000)  //1s



typedef struct{
    u32 SDId;     //emmc device ID
    u32 SDSize;   //emmc size in kBytes
}RawSDParam_t;



u32 CardDetection();
u32 SendInitCmdSD();
u32 SwitchDataWidthSD();
u32 SD_WaitReadOperation(volatile DWC_mshc_block_registers* ptr);
u32 SD_Init(void);
u32 SD_ReadSingleBlock(uint8_t *readbuff, uint32_t ReadAddr, uint16_t BlockSize);
u32 Csu_RawSDInit(RawSDParam_t *Param);
u32 Csu_RawSDRead(u32 Offset, u8* Dest, u32 Length);
/*****************************END OF FILE**************************/

#endif
