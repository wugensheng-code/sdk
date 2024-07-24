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
#define PAGE_SIZE       (256)
#define WRITE_SIZE      (4096)
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

AL_U8 SendData[WRITE_SIZE] = { 0x0 };
AL_U8 RecvData[WRITE_SIZE] = { 0x0 };
AL_U8 FlashId[10] = { 0x0 };

/************************** Function Prototypes ******************************/

/************************** Function Definitions ******************************/
/**
 * Resets the NOR flash memory using the Infineon specific software reset command.
 * This function configures the QSPI controller for transmission only mode, sets the address length,
 * and sends the reset command to the NOR flash.
 *
 * @param None.
 * @return None.
 */
AL_VOID AlNor_Reset(AL_VOID)
{
    AL_S32 Ret = AL_OK;
    AL_U8 SendBuf[10] = {0x0};

    Handle->Dev.Configs.SpiFrameFormat  = SPI_STANDARD_FORMAT;
    Handle->Dev.Configs.Trans.TransMode = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    SendBuf[0] = NOR_OP_INFINEON_SRST;

    Ret = AlQspi_Hal_SendDataBlock(Handle, SendBuf, 1, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_Reset error:0x%x\r\n", Ret);
    }
}

/**
 * Enables write operations on the NOR flash memory by sending the Write Enable (WREN) command.
 * This function sets the QSPI controller to transmission only mode, specifies the address length,
 * instruction length, and the transaction type before sending the WREN command.
 *
 * @param None.
 * @return None.
 */
AL_VOID AlNor_Wren(AL_VOID)
{
    AL_S32 Ret = AL_OK;
    AL_U8 SendBuf[10] = {0x0};
    
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.TransMode = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    Handle->Dev.Configs.SpiFrameFormat = SPI_STANDARD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.Trans.EnSpiCfg.InstLength = QSPI_INST_L8;

    SendBuf[0] = NOR_OP_WREN;

    Ret = AlQspi_Hal_SendDataBlock(Handle, SendBuf, 1, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_Wren error:0x%x\r\n", Ret);
    }
}

/**
 * Waits for the Write In Progress (WIP) bit to clear in the NOR flash's status register.
 * This function repeatedly reads the status register until the WIP bit is cleared, indicating
 * that the NOR flash is ready for further operations. It uses EEPROM mode for transactions.
 *
 * @param None.
 * @return None.
 */
AL_VOID AlNor_WaitWip(AL_VOID)
{
    AL_S32 Ret = AL_OK;
    AL_U8 SendBuf[10] = {0x0};
    
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.TransMode  = QSPI_EEPROM;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    Handle->Dev.Configs.Trans.EnSpiCfg.InstLength = QSPI_INST_L8;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.SpiFrameFormat = SPI_STANDARD_FORMAT;

    SendBuf[0] = NOR_OP_RDSR;

    do {
        Ret = AlQspi_Hal_TranferDataBlock(Handle, SendBuf, 1, RecvData, 1, 100000);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_WaitWip error:0x%x\r\n", Ret);
        }
#ifdef QSPI_DEBUG
        AL_LOG(AL_LOG_LEVEL_ERROR, "WAITWIP Nor Status1 Reg:%x\r\n", RecvData[0]);
#endif
    } while (RecvData[0] & SR_WIP);
}

/**
 * Reads the status register of the NOR flash memory.
 * This function configures the QSPI controller for EEPROM mode transactions and reads the status
 * register to check the current status of the NOR flash.
 *
 * @param None.
 * @return None.
 */
AL_VOID AlNor_ReadStatus(AL_VOID)
{
    AL_S32 Ret = AL_OK;
    AL_U8 SendBuf[10] = {0x0};

    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.TransMode  = QSPI_EEPROM;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.Trans.EnSpiCfg.InstLength = QSPI_INST_L8;

    SendBuf[0] = NOR_OP_RDSR;

    AlQspi_Hal_TranferDataBlock(Handle, SendBuf, 1, RecvData, 1, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_ReadStatus error:0x%x\r\n", Ret);
    }
#ifdef QSPI_DEBUG
    AL_LOG(AL_LOG_LEVEL_ERROR, "Nor Status1 Reg:%x\r\n", RecvData[0]);
#endif
}



/**
 * Erases the entire NOR flash memory.
 * This function sends the Chip Erase (CE) command to the NOR flash, which erases all data stored
 * in the memory. It sets the QSPI controller to transmission only mode before sending the command.
 *
 * @param None.
 * @return None.
 */
AL_VOID AlNor_EraseChip(AL_VOID)
{
    AL_S32 Ret = AL_OK;
    AL_U8 SendBuf[10] = {0x0};

    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.TransMode  = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;

    SendBuf[0] = NOR_OP_CE;

    AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_EraseChip running\r\n");
    Ret = AlQspi_Hal_SendDataBlock(Handle, SendBuf, 1, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_EraseChip error:0x%x\r\n", Ret);
    }
}

/**
 * Reads a page from the NOR flash memory using the 1-1-4 fast read command.
 * This function sets the QSPI controller for reception only mode, specifies the address length,
 * and sends the 1-1-4 fast read command along with the address to read data from the NOR flash.
 *
 * @param Addr The address of the page to read from the NOR flash.
 * @return None.
 */
AL_VOID AlNor_ReadPage_1_1_4(AL_U32 Addr)
{
    AL_S32 Ret = AL_OK;
    AL_U8 SendBuf[10] = {0x0};

    Handle->Dev.Configs.Trans.TransMode  = QSPI_RX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L24;
    Handle->Dev.Configs.SpiFrameFormat = SPI_QUAD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 8;

    SendBuf[0] = NOR_OP_READ_1_1_4;
    SendBuf[1] = (Addr >> 16) & 0xff;
    SendBuf[2] = (Addr >> 8) & 0xff;
    SendBuf[3] = Addr & 0xff;

    Ret = AlQspi_Hal_TranferDataBlock(Handle, SendBuf, 4, RecvData, WRITE_SIZE, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AL_NOR_READPAGE error:0x%x\r\n", Ret);
    }
}

/**
 * Writes a page to the NOR flash memory using the 1-1-4 page program command.
 * This function prepares the data to be written, sets the QSPI controller to transmission only mode,
 * specifies the address length, and sends the 1-1-4 page program command along with the address and data.
 *
 * @param Addr The address of the page to write.
 * @param Data A pointer to the buffer to be write.
 * @param Size The size of the data buffer to be write.
 * @return None.
 */
AL_VOID AlNor_WritePage_1_1_4(AL_U32 Addr,AL_U8 *Data, AL_U32 Size)
{
    AL_S32 Ret = AL_OK;
    AL_U32 i = 0;
    AL_U8 SendBuf[260] = {0x0};

    Handle->Dev.Configs.Trans.TransMode  = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L24;
    Handle->Dev.Configs.SpiFrameFormat = SPI_QUAD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;

    SendBuf[0] = NOR_OP_PP_1_1_4;
    SendBuf[1] = (Addr >> 16) & 0xff;
    SendBuf[2] = (Addr >> 8) & 0xff;
    SendBuf[3] = Addr & 0xff;

    for (i = 0; i < 256; i++) {
        SendBuf[i + 4] = Data[i];
    }

    Ret = AlQspi_Hal_SendDataBlock(Handle, SendBuf, Size + 4, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AL_NOR_WRITEPAGE error:0x%x\r\n", Ret);
    }
}

/**
 * Writes a page to the NOR flash memory using the standard page program command.
 * This function prepares the data to be written, sets the QSPI controller to transmission only mode,
 * specifies the address length, and sends the standard page program command along with the address and data.
 *
 * @param None.
 * @return None.
 */
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

/**
 * Reads the identification (ID) of the NOR flash memory.
 * This function sends the Read ID (RDID) command to the NOR flash and reads the returned ID.
 * It configures the QSPI controller for EEPROM mode transactions before sending the command.
 *
 * @param None.
 * @return None.
 */
AL_VOID AlNor_ReadId(AL_VOID)
{
    AL_S32 Ret = AL_OK;
    AL_U8 SendBuf[10] = {0x0};

    Handle->Dev.Configs.SpiFrameFormat  = SPI_STANDARD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.TransMode  = QSPI_EEPROM;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.Trans.EnSpiCfg.InstLength = QSPI_INST_L8;

    SendBuf[0] = NOR_OP_RDID;
    
    Ret = AlQspi_Hal_TranferDataBlock(Handle, SendBuf, 1, FlashId, 3, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_ReadId error:0x%x\r\n", Ret);
    }
    AL_LOG(AL_LOG_LEVEL_ERROR, "Flash ID:0x%x, 0x%x, 0x%x\r\n", FlashId[0], FlashId[1], FlashId[2]);
}


/**
 * Enables the Quad mode in the NOR flash memory by setting a specific bit in a configuration register.
 * This function first reads the current value of the configuration register, modifies it to enable Quad mode,
 * and then writes it back to the NOR flash. It ensures the NOR flash is ready for write operations before
 * and after modifying the configuration register by calling AlNor_Wren() and AlNor_WaitWip().
 *
 * @param SetQuadCmd The command to write the configuration register to enable Quad mode.
 * @param ReadQuadCmd The command to read the current value of the configuration register.
 * @param QuadPos The bit position in the configuration register that enables Quad mode.
 * @return AL_OK if the operation was successful, otherwise an error code.
 */
AL_S32 AlNor_SetQuad1ByteMode(AL_U8 SetQuadCmd, AL_U8 ReadQuadCmd, AL_U8 QuadPos)
{
    AL_S32  Ret = AL_OK;
    AL_U8 SendBuf[4] = {0x0}, Data = 0;

    SendBuf[0] = ReadQuadCmd;

    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.Trans.TransMode  = QSPI_EEPROM;
    Handle->Dev.Configs.SpiFrameFormat = SPI_STANDARD_FORMAT;

    Ret = AlQspi_Hal_TranferDataBlock(Handle, SendBuf, 1, &Data, 1, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_SetQuad1ByteMode ReadQuadCmd error\r\n");
    }

    Data = Data | (1 << QuadPos);

    AlNor_Wren();

    SendBuf[0] = SetQuadCmd;
    SendBuf[1] = Data;

    Handle->Dev.Configs.Trans.TransMode = QSPI_TX_ONLY;

    Ret = AlQspi_Hal_SendDataBlock(Handle, SendBuf, 2, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_SetQuad1ByteMode SetQuadCmd error\r\n");
    }

    AlNor_WaitWip();

    return Ret;
}


/**
 * Sets the Quad 2-Byte mode for the NOR flash device.
 *
 * @param SetQuadCmd The command to set the Quad 2-Byte mode.
 * @param ReadQuadCmd1 The command to read the Quad 2-Byte mode.
 * @param ReadQuadCmd2 The command to read the Quad 2-Byte mode.
 * @param QuadPos The position of the Quad bit in the command.
 * @return Returns AL_OK if successful, otherwise returns an error code.
 */
AL_S32 AlNor_SetQuad2ByteMode(AL_U8 SetQuadCmd, AL_U8 ReadQuadCmd1, AL_U8 ReadQuadCmd2, AL_U8 QuadPos)
{
    AL_S32  Ret = AL_OK;
    AL_U8 SendBuf[4] = {0x0}, RecvData = 0;

    SendBuf[0] = ReadQuadCmd1;

    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType  = QSPI_TT0;
    Handle->Dev.Configs.Trans.TransMode = QSPI_EEPROM;
    Handle->Dev.Configs.SpiFrameFormat  = SPI_STANDARD_FORMAT;

    Ret = AlQspi_Hal_TranferDataBlock(Handle, SendBuf, 1, &RecvData, 1, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_SetQuad2ByteMode ReadQuadCmd error\r\n");
    }
    SendBuf[1] = RecvData;

    SendBuf[0] = ReadQuadCmd2;
    Ret = AlQspi_Hal_TranferDataBlock(Handle, SendBuf, 1, &RecvData, 1, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_SetQuad2ByteMode ReadQuadCmd error\r\n");
    }
    SendBuf[2] = RecvData;

    if (QuadPos < 8)
        SendBuf[1] |= (1 << QuadPos);
    else
        SendBuf[2] |= (1 << QuadPos - 8);

    AlNor_Wren();

    SendBuf[0] = SetQuadCmd;

    Handle->Dev.Configs.Trans.TransMode = QSPI_TX_ONLY;

    Ret = AlQspi_Hal_SendDataBlock(Handle, SendBuf, 3, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_SetQuad2ByteMode SetQuadCmd error\r\n");
    }

    AlNor_WaitWip();

    return Ret;
}

/**
 * Call AlNor_WritePage_1_1_4 to write data to flash
 *
 * @param Addr The address of the page to write.
 * @param Data A pointer to the buffer to be write.
 * @param Size The size of the data buffer to be write.
 * @return None.
 */
AL_VOID AlNor_WriteFlash(AL_U32 Addr, AL_U8 *Data, AL_U32 Size)
{
    AL_U32 CycleCount = 0;

    while (Size > 0) {
        if (Size > PAGE_SIZE) {
            AlNor_WritePage_1_1_4(Addr, Data + CycleCount * PAGE_SIZE, PAGE_SIZE);
            AlNor_WaitWip();
            AlNor_Wren();
            Addr += PAGE_SIZE;
            Size -= PAGE_SIZE;
            CycleCount += 1;
        }
        else {
            AlNor_WritePage_1_1_4(Addr, Data + CycleCount * PAGE_SIZE, Size);
            break;
        }
    }
}

/**
 *  Main function for the FPSoc Qspi X4 Test.
 */
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
    for (i = 0; i < WRITE_SIZE; i++) {
        if(0xff != RecvData[i]) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlQspi test erase norflash error\r\n");
            AL_LOG(AL_LOG_LEVEL_ERROR, "Error RecvData[%d]:%d\r\n", i, RecvData[i]);
            while (1);
        }
    }
    AL_LOG(AL_LOG_LEVEL_ERROR, "AlQspi test erase norflash success\r\n");

    for (i=0; i < WRITE_SIZE; i++) {
        SendData[i] = i & 0xff;
    }

    AlNor_Wren();
    AlNor_WriteFlash(0, SendData, WRITE_SIZE);
    AlNor_WaitWip();

    AlNor_ReadPage_1_1_4(0);
    for (i = 0; i < WRITE_SIZE; i++) {
        if ((i & 0xff) != RecvData[i]) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Error RecvData[%d]:%d\r\n", i, RecvData[i]);
            while (1);
        }
    }

    AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_WritePage_1_1_4 test Success\r\n");
}

