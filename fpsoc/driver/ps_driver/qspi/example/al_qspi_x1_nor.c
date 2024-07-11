/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    al_qspi_x1_nor.c
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2023-09-01
 * @brief   qspi x1 nor example
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
AL_QSPI_ConfigsStruct QspiInitConfigs =
{
    .SamplDelay         = 0,
    .SlvToggleEnum      = QSPI_SLV_TOGGLE_DISABLE,
    .SpiFrameFormat     = SPI_STANDARD_FORMAT,
    .ClockStretch       = QSPI_EnableClockStretch
};

AL_U8 SendData[500] = { 0x0 };
AL_U8 RecvData[500] = { 0x0 };
AL_U8 FlashId[10] = { 0x0 };

/************************** Function Prototypes ******************************/

/************************** Function Definitions ******************************/
/**
 * Resets the Infineon NOR flash memory by sending a specific reset command.
 * This function configures the SPI frame format, transmission mode, and address length before sending the reset command.
 *
 * @param None.
 * @return None.
 */
AL_VOID AlNor_InfineonReset(AL_VOID)
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

/**
 * Performs a generic reset operation on the NOR flash memory.
 * This function first enables the reset command, then sends the reset command itself.
 * It configures the SPI frame format, transmission mode, and address length for both operations.
 *
 * @param None.
 * @return None.
 */
AL_VOID AlNor_Reset(AL_VOID)
{
    AL_S32 Ret = AL_OK;

    Handle->Dev.Configs.SpiFrameFormat  = SPI_STANDARD_FORMAT;
    Handle->Dev.Configs.Trans.TransMode = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    SendData[0] = NOR_OP_SRSTEN;

    Ret = AlQspi_Hal_SendDataBlock(Handle, SendData, 1, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_RESET error:0x%x\r\n", Ret);
    }

    Handle->Dev.Configs.Trans.TransMode = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    SendData[0] = NOR_OP_SRST;

    Ret = AlQspi_Hal_SendDataBlock(Handle, SendData, 1, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_RESET error:0x%x\r\n", Ret);
    }
}

/**
 * Enables write operations on the NOR flash memory.
 * This function configures the SPI frame format, transmission mode, address length, and instruction length before sending the write enable command.
 *
 * @param None.
 * @return None.
 */
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

/**
 * Sets the status register of the NOR flash memory with the provided data.
 * This function configures the SPI frame format, transmission mode, address length, and instruction length before sending the write status register command along with the data.
 *
 * @param data The data to be written to the status register.
 * @return None.
 */
AL_VOID AlNor_SetStatus(AL_U8 data)
{
    AL_S32 Ret = AL_OK;

    Handle->Dev.Configs.SpiFrameFormat  = SPI_STANDARD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.TransMode = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    Handle->Dev.Configs.Trans.EnSpiCfg.InstLength = QSPI_INST_L8;

    SendData[0] = NOR_OP_WRSR;
    SendData[1] = data;

    Ret = AlQspi_Hal_SendDataBlock(Handle, SendData, 2, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_SETSTATUS error:0x%x\r\n", Ret);
    }
}

/**
 * Waits for the Write-In-Progress (WIP) bit to clear in the status register of the NOR flash memory.
 * This function repeatedly reads the status register until the WIP bit is cleared, indicating that the write operation is complete.
 * It configures the SPI frame format, transmission mode, address length, and instruction length for the read operation.
 *
 * @param None.
 * @return None.
 */
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

/**
 * Reads the status register of the NOR flash memory.
 * This function configures the SPI frame format, transmission mode, address length, and instruction length before reading the status register.
 *
 * @param None.
 * @return None.
 */
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
// #ifdef QSPI_DEBUG
    AL_LOG(AL_LOG_LEVEL_ERROR, "Nor Status1 Reg:%x\r\n", RecvData[0]);
// #endif
}

/**
 * Erases a sector of the NOR flash memory at the specified address.
 * This function configures the SPI frame format, transmission mode, address length, and instruction length before sending the sector erase command along with the address.
 *
 * @param addr The address of the sector to be erased.
 * @return None.
 */
AL_VOID AlNor_Erase(AL_U32 addr)
{
    AL_S32 Ret = AL_OK;

    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.TransMode  = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L24;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.SpiFrameFormat = SPI_STANDARD_FORMAT;

    SendData[0] = NOR_OP_SE;
    SendData[1] = (addr >> 16) & 0xff;
    SendData[2] = (addr >> 8)&0xff;
    SendData[3] = addr&0xff;

    Ret = AlQspi_Hal_SendDataBlock(Handle, SendData, 4, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_Erase error:0x%x\r\n", Ret);
    }
}

/**
 * Erases the entire NOR flash memory chip.
 * This function configures the SPI frame format and transmission mode before sending the chip erase command.
 *
 * @param None.
 * @return None.
 */
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


/**
 * Reads a page of data from the NOR flash memory at the specified address.
 * This function configures the SPI frame format, transmission mode, address length, and instruction length before reading the data into a buffer.
 *
 * @param addr The address of the page to be read.
 * @return None.
 */
AL_VOID AlNor_ReadPage(AL_U32 addr)
{
    AL_S32 Ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode  = QSPI_EEPROM;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L24;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.SpiFrameFormat = SPI_STANDARD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;

    SendData[0] = NOR_OP_READ;
    SendData[1] = (addr >> 16) & 0xff;
    SendData[2] = (addr >> 8)&0xff;
    SendData[3] = addr&0xff;

    Ret = AlQspi_Hal_TranferDataBlock(Handle, SendData, 4, RecvData, 240, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_ReadPage error:0x%x\r\n", Ret);
    }
}

/**
 * Writes a page of data to the NOR flash memory at the specified address.
 * This function configures the SPI frame format, transmission mode, address length, and instruction length before writing the data from a buffer.
 *
 * @param addr The address of the page to be written.
 * @return None.
 */
AL_VOID AlNor_WritePage(AL_U32 addr)
{
    AL_S32 Ret = AL_OK;
    Handle->Dev.Configs.SpiFrameFormat  = SPI_STANDARD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.TransMode  = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L24;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.Trans.EnSpiCfg.InstLength = QSPI_INST_L8;

    SendData[0] = NOR_OP_PP;
    SendData[1] = (addr >> 16) & 0xff;
    SendData[2] = (addr >> 8)&0xff;
    SendData[3] = addr&0xff;

    AL_U32 i = 0;
    for (i = 0; i < 400; i++) {
        SendData[i + 4] = i % 255;
    }
    Ret = AlQspi_Hal_SendDataBlock(Handle, SendData, 240, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_WritePage error:0x%x\r\n", Ret);
    }
}

/**
 * Reads the identification data from the NOR flash memory.
 * This function configures the SPI frame format, transmission mode, address length, and instruction length before reading the ID into a buffer.
 *
 * @param None.
 * @return None.
 */
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
 *
 * This function initializes the QSPI driver, performs various QSPI operations
 * such as erasing and writing to the NOR flash, and verifies the results.
 */
AL_VOID main(AL_VOID)
{
    AL_U32 i;
    AL_S32 Ret = AL_OK;

    AL_LOG(AL_LOG_LEVEL_ERROR, "Start FPSoc Qspi X1 Test\r\n");

    Ret = AlQspi_Hal_Init(&Handle, &QspiInitConfigs, AL_NULL, 0);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlQspi_Hal_Init error:0x%x\r\n", Ret);
    }

    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    AlNor_ReadId();

    AlNor_Wren();
    AlNor_EraseChip();
    AlNor_WaitWip();

    AlNor_ReadPage(0);
    for (i = 0; i < 240; i++) {
        if(0xff != RecvData[i]) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlQspi test erase norflash error\r\n");
            AL_LOG(AL_LOG_LEVEL_ERROR, "Error RecvData[%d]:%d\r\n", i, RecvData[i]);
            while (1);
        }
    }

    AL_LOG(AL_LOG_LEVEL_ERROR, "AlQspi test erase norflash success\r\n");

    /**/
    AlNor_Wren();
    AlNor_WritePage(0);
    AlNor_WaitWip();

    AlNor_ReadPage(0);
    for (i = 0; i < 230; i++) {
        if(i != RecvData[i]) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlQspi data write norflash test error\r\n");
            AL_LOG(AL_LOG_LEVEL_ERROR, "Error RecvData[%d]:%d\r\n", i, RecvData[i]);
            while (1);
        }
    }

    AL_LOG(AL_LOG_LEVEL_ERROR, "AlQspi test write norflash success\r\n");
}

