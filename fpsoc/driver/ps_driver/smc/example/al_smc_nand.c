/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_smc_hal.h"
#include "al_core.h"

AL_SMC_HalStruct SmcHal;

AL_SMC_ConfigsStruct SmcInitConfigs =
{
    .Cycles.d32 = 0,        /* Cycles is set to 0 ignore this value and use the default */
    .SmcWidth = MW_8BITS
};

#define TRANS_SIZE 5000
AL_U8 SendData[TRANS_SIZE] = { 0x0 };
AL_U8 RecvData[TRANS_SIZE] = { 0x0 };

int main(void)
{
    printf("Start FPSoc Smc Test\r\n");
    AL_U32 i, Ret = AL_OK;

    Ret = AlSmc_Hal_Init(&SmcHal, &SmcInitConfigs, 0);
    if (AL_OK != Ret) {
         printf("AlSmc_Hal_Init error:%d\r\n", Ret);
    }

    Ret = AlSmc_Hal_EraseBlock(&SmcHal, 0, 0);
    if (AL_OK != Ret) {
         printf("AlSmc_Hal_EraseBlock error:%d\r\n", Ret);
    }

    for (i = 0; i < sizeof(SendData); i++) {
        SendData[i] = i%256;
    }

    Ret = AlSmc_Hal_ReadPage(&SmcHal, 0, RecvData, 2048, 0);
    if (AL_OK != Ret) {
         printf("AlSmc_Hal_ReadPage error:%d\r\n", Ret);
         while(1);
    }

    for (i = 0; i < 2048; i++) {
        if(0xff != RecvData[i]) {
            printf("AlSmc test erase nandflash error\r\n");
            printf("Error RecvData[%d]:%d\r\n", i, RecvData[i]);
        }
    }

    Ret = AlSmc_Hal_WritePage(&SmcHal, 0, SendData, sizeof(SendData), 0);
    if (AL_OK != Ret) {
         printf("AlSmc_Hal_WritePage error:%d\r\n", Ret);
         while(1);
    }

    Ret = AlSmc_Hal_ReadPage(&SmcHal, 0, RecvData, sizeof(RecvData), 0);
    if (AL_OK != Ret) {
         printf("AlSmc_Hal_ReadPage error:%d\r\n", Ret);
         while(1);
    }

    for (i = 0; i < sizeof(RecvData); i++) {
        if((i%256) != RecvData[i]) {
            printf("AlSmc data write nandflash test error\r\n");
            printf("Error RecvData[%d]:%d\r\n", i, RecvData[i]);
            while (1);
        }
    }
    printf("FPSOC smc nand test pass\r\n");
}
