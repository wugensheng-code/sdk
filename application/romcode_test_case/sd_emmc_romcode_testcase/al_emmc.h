#ifndef _AL_EMMC_H_
#define _AL_EMMC_H_

#include <stdio.h>    
#include "al_mmc.h"    

//CMD PARAMETER
#define EMMC_CMD0_PARA_GO_IDLE_STATE            ((uint32_t)0x0)
#define EMMC_CMD0_PARA_GO_PRE_IDLE_STATE        ((uint32_t)0xF0F0F0F0)
#define EMMC_CMD0_PARA_BOOT_INITIATION          ((uint32_t)0xFFFFFFFA)
#define EMMC_CMD3_PARA_DEFAULT_VAL              ((uint32_t)0x10000) //bit[31:16]    default relative addr
#define EMMC_CMD6_PARA_4_BIT_WIDTH_BUS          ((uint32_t)0x03B70100)
#define EMMC_CMD6_PARA_8_BIT_WIDTH_BUS          ((uint32_t)0x03B70200)
#define EMMC_CMD16_PARA_BLOCK_LEN_512           ((uint32_t)0x200)
//OCR register status
#define EMMC_OCR_HIGH_VOLTAGE               0x0
#define EMMC_OCR_DUAL_VOLTAGE               0x1
#define EMMC_OCR_VOLTAGE_2V7_3V6            0x1FF
#define EMMC_OCR_ACCESS_MODE_BYTE_MODE      0x0
#define EMMC_OCR_ACCESS_MODE_SECTOR_MODE    0x2

#define EMMC_GET_VALID_VOLTAGE_TIMEOUT_VAL      (15000*1000)  //15s

typedef union{
    __IO uint32_t d32;
    struct {
       __IO uint32_t reserved6_0 : 7;
       __IO uint32_t voltage_mode : 1;
       __IO uint32_t reserved14_8 : 7;
       __IO uint32_t voltage2v7_3v6 : 9;
       __IO uint32_t reserved28_24 : 5;
       __IO uint32_t access_mode : 2;
       __IO uint32_t card_power_up_status : 1;
    }bit;
}OCR_R;

typedef enum{
    EMMC_FREQ_400K,
    EMMC_FREQ_10M,
    EMMC_FREQ_MAX
}EMMC_FREQ;

typedef struct{
    uint32_t EmmcId;     //emmc device ID
    uint32_t EmmcSize;   //emmc size in kBytes
}RawEmmcParam_t;

extern SD_CardInfo EmmcCardInfo;

uint32_t AlEmmc_HostControllerClockSetup(volatile DWC_mshc_block_registers* ptr, uint32_t freq);
uint32_t AlEmmc_SendInitCmd();
uint32_t AlEmmc_SwitchDataWidth();
uint32_t AlEmmc_Init(void);
uint32_t AlEmmc_ReadSingleBlock(uint8_t *Readbuff, uint32_t ReadAddr, uint16_t BlockSize);
uint32_t Csu_RawEmmcInit(RawEmmcParam_t *Param);
uint32_t Csu_RawEmmcRead(uint32_t Offset, uint8_t* Dest, uint32_t Length);
uint32_t Csu_RawEmmcSetMode(uint32_t Mode, uint32_t Data);
uint32_t AlEmmc_GetCardInfo(SD_CardInfo *Cardinfo);
uint32_t AlEmmc_GetCardStatus(void);


/*****************************END OF FILE**************************/
#endif
