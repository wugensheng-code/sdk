/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    al_qspi_x4_nor.c
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2023-09-01
 * @brief   qspi x4 nor example
 */

/***************************** Include Files *********************************/
#include "al_qspi_hal.h"
#include "al_spinor.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Variable Definitions *****************************/
AL_QSPI_HalStruct *Handle;

/* When SpiFrameFormat is Dual Quad,TransMode can only use QSPI_TX_ONLY,QSPI_RX_ONLY mode */
AL_QSPI_ConfigsStruct QspiX4InitConfigs =
{
    .SamplDelay         = 2,
    .SlvToggleEnum      = QSPI_SLV_TOGGLE_DISABLE,
    .SpiFrameFormat     = SPI_QUAD_FORMAT,
    .ClockStretch       = QSPI_EnableClockStretch
};

AL_U8 SendData[500] = { 0x0 };
AL_U8 RecvData[500] = { 0x0 };
AL_U8 FlashId[10] = { 0x0 };

/************************** Function Prototypes ******************************/

/************************** Function Definitions ******************************/
AL_VOID AlNor_Reset(AL_VOID)
{
    AL_S32 Ret = AL_OK;

    Handle->Dev.Configs.SpiFrameFormat  = SPI_STANDARD_FORMAT;
    Handle->Dev.Configs.Trans.TransMode = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    SendData[0] = NOR_OP_INFINEON_SRST;

    Ret = AlQspi_Hal_SendDataBlock(Handle, SendData, 1, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_Reset error:0x%x\r\n", Ret);
    }
}

AL_VOID AlNor_Wren(AL_VOID)
{
    AL_S32 Ret = AL_OK;

    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.TransMode = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    Handle->Dev.Configs.SpiFrameFormat = SPI_STANDARD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.Trans.EnSpiCfg.InstLength = QSPI_INST_L8;

    SendData[0] = NOR_OP_WREN;

    Ret = AlQspi_Hal_SendDataBlock(Handle, SendData, 1, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_Wren error:0x%x\r\n", Ret);
    }
}

AL_VOID AlNor_WaitWip(AL_VOID)
{
    AL_S32 Ret = AL_OK;

    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.TransMode  = QSPI_EEPROM;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    Handle->Dev.Configs.Trans.EnSpiCfg.InstLength = QSPI_INST_L8;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.SpiFrameFormat = SPI_STANDARD_FORMAT;

    SendData[0] = NOR_OP_RDSR;

    do {
        Ret = AlQspi_Hal_TranferDataBlock(Handle, SendData, 1, RecvData, 1, 100000);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_WaitWip error:0x%x\r\n", Ret);
        }
#ifdef QSPI_DEBUG
        AL_LOG(AL_LOG_LEVEL_ERROR, "WAITWIP Nor Status1 Reg:%x\r\n", RecvData[0]);
#endif
    } while (RecvData[0] & SR_WIP);
}


AL_VOID AlNor_ReadStatus(AL_VOID)
{
    AL_S32 Ret = AL_OK;

    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.TransMode  = QSPI_EEPROM;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.Trans.EnSpiCfg.InstLength = QSPI_INST_L8;

    SendData[0] = NOR_OP_RDSR;

    AlQspi_Hal_TranferDataBlock(Handle, SendData, 1, RecvData, 1, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_ReadStatus error:0x%x\r\n", Ret);
    }
#ifdef QSPI_DEBUG
    AL_LOG(AL_LOG_LEVEL_ERROR, "Nor Status1 Reg:%x\r\n", RecvData[0]);
#endif
}




AL_VOID AlNor_EraseChip(AL_VOID)
{
    AL_S32 Ret = AL_OK;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.TransMode  = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    SendData[0] = NOR_OP_CE;

    AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_EraseChip running\r\n");
    Ret = AlQspi_Hal_SendDataBlock(Handle, SendData, 1, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_EraseChip error:0x%x\r\n", Ret);
    }
}


AL_VOID AlNor_ReadPage_1_1_4(AL_U32 addr)
{
    AL_S32 Ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode  = QSPI_RX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L24;
    Handle->Dev.Configs.SpiFrameFormat = SPI_QUAD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 8;

    SendData[0] = NOR_OP_READ_1_1_4;
    SendData[1] = (addr >> 16) & 0xff;
    SendData[2] = (addr >> 8)&0xff;
    SendData[3] = addr&0xff;

    Ret = AlQspi_Hal_TranferDataBlock(Handle, SendData, 4, RecvData, 240, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AL_NOR_READPAGE error:0x%x\r\n", Ret);
    }
}


AL_VOID AlNor_WritePage_1_1_4(AL_U32 addr)
{
    AL_S32 Ret = AL_OK;
    AL_U32 i = 0;

    Handle->Dev.Configs.Trans.TransMode  = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L24;
    Handle->Dev.Configs.SpiFrameFormat = SPI_QUAD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;

    SendData[0] = NOR_OP_PP_1_1_4;
    SendData[1] = (addr >> 16) & 0xff;
    SendData[2] = (addr >> 8)&0xff;
    SendData[3] = addr&0xff;

    for (i = 0; i < 400; i++) {
        SendData[i + 4] = i % 255;
    }
    Ret = AlQspi_Hal_SendDataBlock(Handle, SendData, 240, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AL_NOR_WRITEPAGE error:0x%x\r\n", Ret);
    }
}

AL_VOID AL_NOR_WRITEPAGE(AL_VOID)
{
    AL_S32 Ret = AL_OK;
    Handle->Dev.Configs.SpiFrameFormat  = SPI_STANDARD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.TransMode  = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L24;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.Trans.EnSpiCfg.InstLength = QSPI_INST_L8;

    SendData[0] = NOR_OP_PP;
    SendData[1] = 0;
    SendData[2] = 0;
    SendData[3] = 0;

    AL_U32 i = 0;
    for (i = 0; i < 400; i++) {
        SendData[i + 4] = i % 255;
    }
    Ret = AlQspi_Hal_SendDataBlock(Handle, SendData, 240, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AL_NOR_WRITEPAGE error:0x%x\r\n", Ret);
    }
}


AL_VOID AlNor_ReadId(AL_VOID)
{
    AL_S32 Ret = AL_OK;
    SendData[0] = NOR_OP_RDID;
    Handle->Dev.Configs.SpiFrameFormat  = SPI_STANDARD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.TransMode  = QSPI_EEPROM;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.Trans.EnSpiCfg.InstLength = QSPI_INST_L8;

    Ret = AlQspi_Hal_TranferDataBlock(Handle, SendData, 1, FlashId, 3, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_ReadId error:0x%x\r\n", Ret);
    }
    AL_LOG(AL_LOG_LEVEL_ERROR, "Flash ID:0x%x, 0x%x, 0x%x\r\n", FlashId[0], FlashId[1], FlashId[2]);
}



/**
 * This function is set norflash QE bit
 * @param   SetQuadCmd is set QE bit cmd
 * @param   ReadQuadCmd is read QE bit cmd
 * @param   QuadPos is the location of the QE bit
 * @return  Different manufacturers of flash require different parameters to be passed
 * @note
*/
AL_S32 AlNor_SetQuad1ByteMode(AL_U8 SetQuadCmd, AL_U8 ReadQuadCmd, AL_U8 QuadPos)
{
    AL_S32  Ret = AL_OK;
    AL_U8 SendData[4] = {0x0}, Data = 0;

    SendData[0] = ReadQuadCmd;

    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.Trans.TransMode  = QSPI_EEPROM;
    Handle->Dev.Configs.SpiFrameFormat = SPI_STANDARD_FORMAT;

    Ret = AlQspi_Hal_TranferDataBlock(Handle, SendData, 1, &Data, 1, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_SetQuad1ByteMode ReadQuadCmd error\r\n");
    }

    Data = Data | (1 << QuadPos);

    AlNor_Wren();

    SendData[0] = SetQuadCmd;
    SendData[1] = Data;

    Handle->Dev.Configs.Trans.TransMode = QSPI_TX_ONLY;

    Ret = AlQspi_Hal_SendDataBlock(Handle, SendData, 2, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_SetQuad1ByteMode SetQuadCmd error\r\n");
    }

    AlNor_WaitWip();

    return Ret;
}


/*
 * function to set QSPI quad mode
 * The setting step;
 * -- Read status1 by ReadQuadCmd1
 * -- Read status2 by ReadQuadCmd2
 * if QuadPos < 8
 * -- set status register: SetQuadCmd （status1 | (0x01 << QuadPos)）status2
  * if QuadPos > 8
 * -- set status register: SetQuadCmd status1 (status2 | (0x01 << QuadPos - 8))
 *
 *  for example GD25LQ255E
 * Read Status Register-1        :05H (S7-S0) (cont.)
 * Read Status Register-2        :35H (S15-S8) (cont.)
 * Write Status Register-1&2     :01H S7-S0 S15-S8
*/
AL_S32 AlNor_SetQuad2ByteMode(AL_U8 SetQuadCmd, AL_U8 ReadQuadCmd1, AL_U8 ReadQuadCmd2, AL_U8 QuadPos)
{
    AL_S32  Ret = AL_OK;
    AL_U8 SendData[4] = {0x0}, RecvData = 0;

    SendData[0] = ReadQuadCmd1;

    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType  = QSPI_TT0;
    Handle->Dev.Configs.Trans.TransMode = QSPI_EEPROM;
    Handle->Dev.Configs.SpiFrameFormat  = SPI_STANDARD_FORMAT;

    Ret = AlQspi_Hal_TranferDataBlock(Handle, SendData, 1, &RecvData, 1, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_SetQuad2ByteMode ReadQuadCmd error\r\n");
    }
    SendData[1] = RecvData;

    SendData[0] = ReadQuadCmd2;
    Ret = AlQspi_Hal_TranferDataBlock(Handle, SendData, 1, &RecvData, 1, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_SetQuad2ByteMode ReadQuadCmd error\r\n");
    }
    SendData[2] = RecvData;

    if (QuadPos < 8)
        SendData[1] |= (1 << QuadPos);
    else
        SendData[2] |= (1 << QuadPos - 8);

    AlNor_Wren();

    SendData[0] = SetQuadCmd;

    Handle->Dev.Configs.Trans.TransMode = QSPI_TX_ONLY;

    Ret = AlQspi_Hal_SendDataBlock(Handle, SendData, 3, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_SetQuad2ByteMode SetQuadCmd error\r\n");
    }

    AlNor_WaitWip();

    return Ret;
}



AL_VOID main(AL_VOID)
{
    AL_U32 i;
    AL_S32 Ret = AL_OK;

    AL_LOG(AL_LOG_LEVEL_ERROR, "Start FPSoc Qspi X4 Test\r\n");

    Ret = AlQspi_Hal_Init(&Handle, &QspiX4InitConfigs, AL_NULL, 0);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlQspi_Hal_Init error:0x%x\r\n", Ret);
    }

    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    AlNor_ReadId();

    AlNor_ReadStatus();

    if((FlashId[0] != 0x01) && (FlashId[0] != 0x20) && (FlashId[0] != 0x0) && (FlashId[0] != 0xff)) {
        if((FlashId[0] == 0xc8) && (FlashId[1] == 0x60)) {
            Ret = AlNor_SetQuad2ByteMode(0x1, 0x5, 0x35, 9);
        } else  if((FlashId[0] != 0x9d) && (FlashId[0] != 0xc2)) {
            Ret = AlNor_SetQuad1ByteMode(0x31, 0x35, 1);
        } else {
            Ret = AlNor_SetQuad1ByteMode(0x01, 0x05, 6);
        }
    }

    AlNor_Wren();
    AlNor_EraseChip();
    AlNor_WaitWip();

    AlNor_ReadPage_1_1_4(0);
    for (i = 0; i < 240; i++) {
        if(0xff != RecvData[i]) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlQspi test erase norflash error\r\n");
            AL_LOG(AL_LOG_LEVEL_ERROR, "Error RecvData[%d]:%d\r\n", i, RecvData[i]);
        }
    }
    AL_LOG(AL_LOG_LEVEL_ERROR, "AlQspi test erase norflash success\r\n");

    AlNor_Wren();
    AlNor_WritePage_1_1_4(0);
    AlNor_WaitWip();

    AlNor_ReadPage_1_1_4(0);
    for (i = 0; i < 230; i++) {
        if (i != RecvData[i]) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Error RecvData[%d]:%d\r\n", i, RecvData[i]);
        }
    }
    AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_WritePage_1_1_4 test Success\r\n");
}

