/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_spi_hal.h"
#include "al_spinor.h"

#include "al_dmacahb_hal.h"

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
AL_U8 __attribute__((aligned(4))) SendTemp[300] = { 0x0 };
AL_U8 __attribute__((aligned(4))) RecvData[500] = { 0x0 };
AL_U8 __attribute__((aligned(4))) FlashId[10] = { 0x0 };


AL_VOID AlNor_Reset(AL_VOID)
{
    AL_S32 ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode = SPI_TX_ONLY;
    SendData[0] = NOR_OP_INFINEON_SRST;

    ret = AlSpi_Hal_SendDataBlock(Handle, SendData, 1, 100000);
    if (AL_OK != ret) {
        printf("AL_NOR_RESET error, ret:0x%x\r\n", ret);
    }
}


AL_VOID AlNor_WrEn(AL_VOID)
{
    AL_S32 ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode = SPI_TX_ONLY;
    SendData[0] = NOR_OP_WREN;

    ret = AlSpi_Hal_SendDataBlock(Handle, SendData, 1, 100000);
    if (AL_OK != ret) {
        printf("AlNor_WrEn error, ret:0x%x\r\n", ret);
    };
}


AL_VOID AlNor_SetStatus(AL_U8 data)
{
    AL_S32 ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode = SPI_TX_ONLY;
    SendData[0] = NOR_OP_WRSR;
    SendData[1] = data;

    ret = AlSpi_Hal_SendDataBlock(Handle, SendData, 2, 100000);
    if (AL_OK != ret) {
        printf("AlNor_SetStatus error, ret:0x%x\r\n", ret);
    }
}

AL_VOID AlNor_WaitWip(AL_VOID)
{
    AL_S32 ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode  = SPI_EEPROM;
    SendData[0] = NOR_OP_RDSR;

    do {
        ret = AlSpi_Hal_TranferDataBlock(Handle, SendData, 1, RecvData, 1, 100000);
        if (AL_OK != ret) {
            printf("AlNor_WaitWip error, ret:0x%x\r\n");
        }
#ifdef SPI_DEBUG
        printf("WAITWIP Nor Status1 Reg:%x\r\n", RecvData[0]);
#endif
    } while (RecvData[0] & SR_WIP);
}


AL_VOID AlNor_ReadStatus(AL_VOID)
{
    AL_S32 ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode  = SPI_EEPROM;
    SendData[0] = NOR_OP_RDSR;

    AlSpi_Hal_TranferDataBlock(Handle, SendData, 1, RecvData, 1, 100000);
    if (AL_OK != ret) {
        printf("AlNor_ReadStatus error, ret:0x%x\r\n", ret);
    }
    printf("Nor Status1 Reg:%x\r\n", RecvData[0]);
}


AL_VOID AlNor_SectorErase(AL_U32 SectorAddr)
{
    AL_S32 ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode  = SPI_TX_ONLY;
    SendData[0] = NOR_OP_SE;
    SendData[1] = (SectorAddr >> 16) & 0xff;
    SendData[2] = (SectorAddr >> 8)&0xff;
    SendData[3] = SectorAddr&0xff;

    ret = AlSpi_Hal_SendDataBlock(Handle, SendData, 4, 100000);
    if (AL_OK != ret) {
        printf("AlNor_SectorErase error, ret:0x%x\r\n", ret);
    }
}


AL_VOID AlNor_EraseChip(AL_VOID)
{
    AL_S32 ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode  = SPI_TX_ONLY;
    SendData[0] = NOR_OP_CE;

    ret = AlSpi_Hal_SendDataBlock(Handle, SendData, 1, 100000);
    if (AL_OK != ret) {
        printf("AlNor_EraseChip error, ret:0x%x\r\n", ret);
    }
    printf("AlNor_EraseChip runnig\r\n");
}


AL_VOID AlNor_Read(AL_U32 SrcAddress, AL_U8 *RecvData, AL_U32 Length)
{
    AL_S32 ret = AL_OK, i, RecvSize;
    Handle->Dev.Configs.Trans.TransMode  = SPI_EEPROM;

    SendData[0] = NOR_OP_READ_1_1_4;
    i = Length / 65532 + (Length % 65532 ? 1 : 0);

    for (; i > 0; i--) {
        SendData[1] = (SrcAddress >> 16) & 0xff;
        SendData[2] = (SrcAddress >> 8)&0xff;
        SendData[3] = SrcAddress&0xff;

        RecvSize = MIN(65532, Length);
        ret = AlSpi_Hal_TranferDataBlock(Handle, SendData, 4, RecvData, RecvSize, 100000);
        if (AL_OK != ret) {
            printf("AlNor_Read error, ret:0x%x\r\n", ret);
            return ret;
        }
        SrcAddress += RecvSize;
        RecvData += RecvSize;
        Length -= RecvSize;
    }

    return AL_OK;
}

AL_VOID AlNor_Write(AL_U32 SrcAddress, AL_U8 *SendData, AL_U32 Length)
{
    AL_S32 ret = AL_OK;
    AL_U32 i = 0, Size = Length, Temp;

    Handle->Dev.Configs.Trans.TransMode  = SPI_TX_ONLY;
    SendTemp[0] = NOR_OP_PP;

    while (Size > 0)
    {
        SendTemp[1] = (SrcAddress >> 16) & 0xff;
        SendTemp[2] = (SrcAddress >> 8)&0xff;
        SendTemp[3] = SrcAddress&0xff;
        Temp = MIN(255, Size);
        for (i = 0; i < Temp; i++)
        {
            SendTemp[i + 4] = *SendData;
            SendData++;
        }
        SrcAddress += Temp;

        ret = AlSpi_Hal_SendDataBlock(Handle, SendTemp, Temp+4, 100000);
        if (AL_OK !=ret) {
            printf("AlNor_Write error ret:0x%x\r\n", ret);
            return ret;
        }
    }

    return AL_OK;
}

AL_VOID AlNor_ReadId(AL_VOID)
{
    AL_S32 ret = AL_OK;
    Handle->Dev.Configs.Trans.TransMode  = SPI_EEPROM;
    SendData[0] = NOR_OP_RDID;
    ret = AlSpi_Hal_TranferDataBlock(Handle, SendData, 1, FlashId, 3, 100000);
    if (AL_OK !=ret) {
        printf("AlNor_ReadId error, ret:0x%x\r\n", ret);
    }
    printf("Flash ID:0x%x, 0x%x, 0x%x\r\n", FlashId[0], FlashId[1], FlashId[2]);
}


AL_VOID main(AL_VOID)
{
    AL_U32 i;
    AL_S32 ret = AL_OK;

    printf("Start FPSoc Spi Test\r\n");

    ret = AlSpi_Hal_Init(&Handle, &SpiInitConfigs, AL_NULL, 1);
    if (AL_OK != ret) {
        printf("AlSpi_Hal_Init error, ret:0x%x\r\n", ret);
    }

    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    AlNor_ReadId();
    AlNor_WrEn();
    AlNor_ReadStatus();

    AlNor_WrEn();
    // AlNor_SectorErase(0);
    AlNor_EraseChip();
    AlNor_WaitWip();

    AlNor_Read(0, RecvData, 288);
    for (i = 0; i < 288; i++) {
        if(0xff != RecvData[i]) {
            printf("AlSpi test erase norflash error\r\n");
            printf("Error RecvData[%d]:%d\r\n", i, RecvData[i]);
            while (1);
        }
    }

    printf("AlSpi test erase norflash success\r\n");

    for (i = 0; i < 400; i++) {
       SendData[i + 4] = i % 255;
    }

    AlNor_WrEn();
    AlNor_Write(0, SendData, 288);
    AlNor_WaitWip();

    AlNor_Read(0, RecvData, 288);
    for (i = 0; i < 288; i++) {
        if(i != RecvData[i]) {
            printf("AlSpi data write norflash test error, ret:0x%x\r\n", ret);
            printf("Error RecvData[%d]:%d\r\n", i, RecvData[i]);
            while (1);
        }
    }

    printf("AlSpi test write norflash success\r\n");
}

