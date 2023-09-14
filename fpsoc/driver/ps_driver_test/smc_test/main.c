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
    .Cycles = 0,        /* Cycles is set to 0 ignore this value and use the default */
    .SmcWidth = MW_8BITS
};

AL_U8 SendData[500] = { 0x0 };
AL_U8 RecvData[500] = { 0x0 };

void main(void)
{
#define PIN_MUX_BASE    (0xF8803000UL)

#define PS_MIO0         (0x00+PIN_MUX_BASE)
#define PS_MIO1         (0x04+PIN_MUX_BASE)
#define PS_MIO2         (0x08+PIN_MUX_BASE)
#define PS_MIO3         (0x0c+PIN_MUX_BASE)
#define PS_MIO4         (0x10+PIN_MUX_BASE)
#define PS_MIO5         (0x14+PIN_MUX_BASE)
#define PS_MIO6         (0x18+PIN_MUX_BASE)
#define PS_MIO7         (0x1c+PIN_MUX_BASE)
#define PS_MIO8         (0x20+PIN_MUX_BASE)
#define PS_MIO9         (0x24+PIN_MUX_BASE)
#define PS_MIO10        (0x28+PIN_MUX_BASE)
#define PS_MIO11        (0x2c+PIN_MUX_BASE)
#define PS_MIO12        (0x30+PIN_MUX_BASE)
#define PS_MIO13        (0x34+PIN_MUX_BASE)
#define PS_MIO14        (0x38+PIN_MUX_BASE)
#define PS_MIO15        (0x3c+PIN_MUX_BASE)
#define PS_MIO16        (0x40+PIN_MUX_BASE)
#define PS_MIO17        (0x44+PIN_MUX_BASE)
#define PS_MIO18        (0x48+PIN_MUX_BASE)
#define PS_MIO19        (0x4c+PIN_MUX_BASE)
#define PS_MIO20        (0x50+PIN_MUX_BASE)
#define PS_MIO21        (0x54+PIN_MUX_BASE)
#define PS_MIO22        (0x58+PIN_MUX_BASE)
#define PS_MIO23        (0x5c+PIN_MUX_BASE)
#define PS_MIO24        (0x60+PIN_MUX_BASE)
#define PS_MIO25        (0x64+PIN_MUX_BASE)
#define PS_MIO26        (0x68+PIN_MUX_BASE)
#define PS_MIO27        (0x6c+PIN_MUX_BASE)
#define PS_MIO28        (0x70+PIN_MUX_BASE)

    AL_REG32_WRITE(PS_MIO0, 0x02);
    //AL_REG32_WRITE(PS_MIO1, 0x02);
    AL_REG32_WRITE(PS_MIO2, 0x02);
    AL_REG32_WRITE(PS_MIO3, 0x02);
    AL_REG32_WRITE(PS_MIO4, 0x02);
    AL_REG32_WRITE(PS_MIO5, 0x02);
    AL_REG32_WRITE(PS_MIO6, 0x02);
    AL_REG32_WRITE(PS_MIO7, 0x02);
    AL_REG32_WRITE(PS_MIO8, 0x02);
    AL_REG32_WRITE(PS_MIO9, 0x02);

    AL_REG32_WRITE(PS_MIO10, 0x02);
    AL_REG32_WRITE(PS_MIO11, 0x02);
    AL_REG32_WRITE(PS_MIO12, 0x02);
    AL_REG32_WRITE(PS_MIO13, 0x02);
    AL_REG32_WRITE(PS_MIO14, 0x02);


    printf("Start FPSoc Smc Test\r\n");
    AL_U32 i, Ret = AL_OK;

    for (i = 0; i < sizeof(SendData); i++) {
        SendData[i] = i;
    }

    Ret = AlSmc_Hal_Init(&SmcHal, &SmcInitConfigs, 0);
    if (AL_OK != Ret) {
         printf("AlSmc_Hal_Init error!!!!!\r\n");
    }

    Ret = AlSmc_Hal_EraseBlock(&SmcHal, 0, 0);
    if (AL_OK != Ret) {
         printf("AlSmc_Hal_EraseBlock error!!!!!\r\n");
    }

    Ret = AlSmc_Hal_ReadPage(&SmcHal, 0, RecvData, 500, 0);
    if (AL_OK != Ret) {
         printf("AlSmc_Hal_ReadPage error!!!!!\r\n");
         while(1);
    }

    for (i = 0; i < sizeof(RecvData); i++) {
        if(0xff != RecvData[i]) {
            printf("AlSmc test erase nandflash error!!!!!\r\n");
            printf("Error RecvData[%d]:%d\r\n", i, RecvData[i]);
            while (1);
        }
    }

    Ret = AlSmc_Hal_WritePage(&SmcHal, 0, SendData, 500, 0);
    if (AL_OK != Ret) {
         printf("AlSmc_Hal_WritePage error!!!!!\r\n");
         while(1);
    }

    Ret = AlSmc_Hal_ReadPage(&SmcHal, 0, RecvData, 500, 0);
    if (AL_OK != Ret) {
         printf("AlSmc_Hal_ReadPage error!!!!!\r\n");
         while(1);
    }

    for (i = 0; i < sizeof(RecvData); i++) {
        if((i%256) != RecvData[i]) {
            printf("AlSmc data write nandflash test error!!!!!\r\n");
            printf("Error RecvData[%d]:%d\r\n", i, RecvData[i]);
            while (1);
        }
    }
}
