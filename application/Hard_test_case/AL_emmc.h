#ifndef _AL_EMMC_H_
#define _AL_EMMC_H_

#include <stdio.h>    
#include "AL_sdio_emmc_common.h"    

//CMD PARAMETER
#define EMMC_CMD0_PARA_GO_IDLE_STATE            ((uint32_t)0x0)
#define EMMC_CMD0_PARA_GO_PRE_IDLE_STATE        ((uint32_t)0xF0F0F0F0)
#define EMMC_CMD0_PARA_BOOT_INITIATION          ((uint32_t)0xFFFFFFFA)
#define EMMC_CMD3_PARA_DEFAULT_VAL              ((uint32_t)0x10000) //bit[31:16]    default relative addr
#define EMMC_CMD6_PARA_4_BIT_WIDTH_BUS          ((uint32_t)0x03B70100)
#define EMMC_CMD6_PARA_8_BIT_WIDTH_BUS          ((uint32_t)0x03B70200)
//OCR register status
#define EMMC_OCR_HIGH_VOLTAGE               0x0
#define EMMC_OCR_DUAL_VOLTAGE               0x1
#define EMMC_OCR_ACCESS_MODE_BYTE_MODE      0x0
#define EMMC_OCR_ACCESS_MODE_SECTOR_MODE    0x2

//EMMC ONLY ERROR   101~200
#define EMMC_GET_VALID_VOLTAGE_TIMEOUT_ERROR    101

#define EMMC_GET_VALID_VOLTAGE_TIMEOUT_VAL      (150*1000)  //150ms

typedef union{
    __IO uint32_t d32;
    struct {
       __IO uint32_t reserved6_0 : 7;
       __IO uint32_t voltage_mode : 1;
       __IO uint32_t reserved14_8 : 7;
       __IO uint32_t reserved23_15 : 9;
       __IO uint32_t reserved28_24 : 5;
       __IO uint32_t access_mode : 2;
       __IO uint32_t card_power_up_status : 1;
    }bit;
}OCR_R;

u32 SendInitCmdEmmc();
u32 SwitchDataWidthEmmc();


/*****************************END OF FILE**************************/
#endif
