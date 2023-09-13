/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_spi_hal.h"
#include "al_spinor.h"

AL_SPI_HalStruct *Handle;

AL_SPI_ConfigsStruct SpiInitConfigs =
{
    .Mode               = SPI_MASTER_MODE,
    .ProtFormat         = MOTOROLA_SPI,
    .ClockEnum          = SPI_CLK_MODE0,
    .SlvToggleEnum      = SPI_SLV_TOGGLE_DISABLE,
    .ClkDiv             = 40
};

AL_U8 __attribute__((aligned(4))) SendData[500] = { 0x0 };
AL_U8 __attribute__((aligned(4))) RecvData[500] = { 0x0 };
AL_U8 __attribute__((aligned(4))) FlashId[10] = { 0x0 };


void AL_NOR_RESET(void)
{
    AL_S32 ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode = SPI_TX_ONLY;
    SendData[0] = NOR_OP_INFINEON_SRST;

    ret = AlSpi_Hal_SendDataBlock(Handle, SendData, 1, 100000);
    if (AL_OK != ret) {
        printf("AL_NOR_RESET error, ret:0x%x\r\n", ret);
    }
}



void AL_NOR_WREN(void)
{
    AL_S32 ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode = SPI_TX_ONLY;
    SendData[0] = NOR_OP_WREN;

    ret = AlSpi_Hal_SendDataBlock(Handle, SendData, 1, 100000);
    if (AL_OK != ret) {
        printf("AL_NOR_WREN error, ret:0x%x\r\n", ret);
    };
}



void AL_NOR_SETSTATUS(AL_U8 data)
{
    AL_S32 ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode = SPI_TX_ONLY;
    SendData[0] = NOR_OP_WRSR;
    SendData[1] = data;

    ret = AlSpi_Hal_SendDataBlock(Handle, SendData, 2, 100000);
    if (AL_OK != ret) {
        printf("AL_NOR_SETSTATUS error, ret:0x%x\r\n", ret);
    }
}

void AL_NOR_WAITWIP(void)
{
    AL_S32 ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode  = SPI_EEPROM;
    SendData[0] = NOR_OP_RDSR;

    do {
        ret = AlSpi_Hal_TranferDataBlock(Handle, SendData, 1, RecvData, 1, 100000);
        if (AL_OK != ret) {
            printf("AL_NOR_WAITWIP error, ret:0x%x\r\n");
        }
#ifdef SPI_DEBUG
        printf("WAITWIP Nor Status1 Reg:%x\r\n", RecvData[0]);
#endif
    } while (RecvData[0] & SR_WIP);
}


void AL_NOR_READSTATUS(void)
{
    AL_S32 ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode  = SPI_EEPROM;
    SendData[0] = NOR_OP_RDSR;

    AlSpi_Hal_TranferDataBlock(Handle, SendData, 1, RecvData, 1, 100000);
    if (AL_OK != ret) {
        printf("AL_NOR_READSTATUS error, ret:0x%x\r\n", ret);
    }
#ifdef SPI_DEBUG
    printf("Nor Status1 Reg:%x\r\n", RecvData[0]);
#endif
}

void AL_NOR_ERASE(void)
{
    AL_S32 ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode  = SPI_TX_ONLY;
    SendData[0] = NOR_OP_SE;
    SendData[1] = 0;
    SendData[2] = 0;
    SendData[3] = 0;

    ret = AlSpi_Hal_SendDataBlock(Handle, SendData, 4, 100000);
    if (AL_OK != ret) {
        printf("AL_NOR_ERASE error, ret:0x%x\r\n", ret);
    }
}

void AL_NOR_ERASECHIP(void)
{
    AL_S32 ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode  = SPI_TX_ONLY;
    SendData[0] = NOR_OP_CE;

    ret = AlSpi_Hal_SendDataBlock(Handle, SendData, 1, 100000);
    if (AL_OK != ret) {
        printf("AL_NOR_ERASECHIP error, ret:0x%x\r\n", ret);
    }
}

void AL_NOR_READPAGE(void)
{
    AL_S32 ret = AL_OK;
    Handle->Dev.Configs.Trans.TransMode  = SPI_EEPROM;
    SendData[0] = NOR_OP_READ;
    SendData[1] = 0;
    SendData[2] = 0;
    SendData[3] = 0;

    ret = AlSpi_Hal_TranferDataBlock(Handle, SendData, 4, RecvData, 240, 100000);
    if (AL_OK != ret) {
        printf("AL_NOR_READPAGE error, ret:0x%x\r\n", ret);
    }
}


void AL_NOR_READPAGE1(void)
{
    AL_S32 ret = AL_OK;
    Handle->Dev.Configs.Trans.TransMode  = SPI_EEPROM;

    SendData[0] = NOR_OP_READ;
    SendData[1] = 0;
    SendData[2] = 0x10;
    SendData[3] = 0;

    ret = AlSpi_Hal_TranferDataBlock(Handle, SendData, 4, RecvData, 240, 100000);
    if (AL_OK !=ret) {
        printf("AL_NOR_READPAGE error, ret:0x%x\r\n", ret);
    }
}

void AL_NOR_WRITEPAGE(void)
{
    AL_S32 ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode  = SPI_TX_ONLY;
    SendData[0] = NOR_OP_PP;
    SendData[1] = 0;
    SendData[2] = 0;
    SendData[3] = 0;

    AL_U32 i = 0;
    for (i = 0; i < 400; i++) {
        SendData[i + 4] = i % 255;
    }
    ret = AlSpi_Hal_SendDataBlock(Handle, SendData, 240, 100000);
    if (AL_OK !=ret) {
        printf("AL_NOR_WRITEPAGE error ret:0x%x\r\n", ret);
    }
}

void AL_NOR_WRITEPAGE1(void)
{
    AL_S32 ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode  = SPI_TX_ONLY;
    SendData[0] = NOR_OP_PP;
    SendData[1] = 0;
    SendData[2] = 0x10;
    SendData[3] = 0;

    AL_U32 i = 0;
    for (i = 0; i < 400; i++) {
       SendData[i + 4] = i % 255;
    }
    ret = AlSpi_Hal_SendDataBlock(Handle, SendData, 240, 100000);
    if (AL_OK !=ret) {
        printf("AL_NOR_WRITEPAGE error, ret:0x%x\r\n", ret);
    }
}

AL_VOID AL_NOR_READID(AL_VOID)
{
    AL_S32 ret = AL_OK;
    Handle->Dev.Configs.Trans.TransMode  = SPI_EEPROM;
    SendData[0] = NOR_OP_RDID;
    ret = AlSpi_Hal_TranferDataBlock(Handle, SendData, 1, FlashId, 3, 100000);
    if (AL_OK !=ret) {
        printf("AL_NOR_READID error, ret:0x%x\r\n", ret);
    }
    printf("Flash ID:0x%x, 0x%x, 0x%x\r\n", FlashId[0], FlashId[1], FlashId[2]);
}


void main(void)
{
    AL_U32 i;
    AL_S32 ret = AL_OK;

    printf("Start FPSoc Spi Test\r\n");

    ret = AlSpi_Hal_Init(&Handle, &SpiInitConfigs, AL_NULL, 1);
    if (AL_OK != ret) {
        printf("AlSpi_Hal_Init error, ret:0x%x\r\n", ret);
    }

    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    AL_NOR_READID();

    AL_NOR_WREN();
    AL_NOR_READSTATUS();
    AL_NOR_ERASE();
    AL_NOR_WAITWIP();

    AL_NOR_READPAGE();
    for (i = 0; i < 240; i++) {
        if(0xff != RecvData[i]) {
            printf("AlSpi test erase norflash error\r\n");
            printf("Error RecvData[%d]:%d\r\n", i, RecvData[i]);
            while (1);
        }
    }

    printf("AlSpi test erase norflash success\r\n");

    AL_NOR_WREN();
    AL_NOR_WRITEPAGE();
    AL_NOR_WAITWIP();

    AL_NOR_WREN();
    AL_NOR_WRITEPAGE1();
    AL_NOR_WAITWIP();

    AL_NOR_READPAGE();
    for (i = 0; i < 230; i++) {
        if(i != RecvData[i]) {
            printf("AlSpi data write norflash test error, ret:0x%x\r\n", ret);
            printf("Error RecvData[%d]:%d\r\n", i, RecvData[i]);
            while (1);
        }
    }

    AL_NOR_READPAGE1();
    for (i = 0; i < 230; i++) {
        if(i != RecvData[i]) {
            printf("AlSpi data write1 norflash test error, ret:0x%x\r\n", ret);
            printf("Error RecvData[%d]:%d\r\n", i, RecvData[i]);
            while (1);
        }
    }

    printf("AlSpi test write norflash success\r\n");
}

