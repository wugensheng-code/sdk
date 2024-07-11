/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    al_qspi_dma_nor.c
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2023-09-01
 * @brief   qspi dma nor example
 */

/***************************** Include Files *********************************/
#include "al_qspi_hal.h"
#include "al_spinor.h"
#include "al_dmacahb_hal.h"

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

AL_U8 CACHE_LINE_ALIGN FlashId[10] = { 0x0 };

AL_U8 CACHE_LINE_ALIGN DmaSendData[500] = { 0x0 };
AL_U8 CACHE_LINE_ALIGN DmaRecvData[500] = { 0x0 };
AL_U8 InstAndAddr[10] = { 0x0 };

/************************** Function Prototypes ******************************/

/************************** Function Definitions ******************************/
/**
 * Resets the Infineon NOR flash memory using DMA.
 * This function configures the QSPI controller for a transmit-only operation, sets the instruction to the Infineon-specific software reset command, and initiates a block send operation via DMA.
 *
 * @param None.
 * @return None.
 */
AL_VOID AlNorDma_InfineonReset(AL_VOID)
{
    AL_S32 Ret = AL_OK;

    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.TransMode = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    Handle->Dev.Configs.SpiFrameFormat = SPI_STANDARD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;

    InstAndAddr[0] = NOR_OP_INFINEON_SRST;

    Ret = AlQspi_Hal_DmaStartBlockSend(Handle, DmaSendData, InstAndAddr, 0, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "DMA AL_NOR_RESET error:0x%x\r\n", Ret);
    }
}

/**
 * Performs a generic reset operation on the NOR flash memory using DMA.
 * This function prepares the QSPI controller for a reset operation by setting it to transmit-only mode, configuring the necessary parameters for the reset instruction, and then sends the reset enable and reset commands sequentially to the NOR flash.
 *
 * @param None.
 * @return None.
 */
AL_VOID AlNorDma_Reset(AL_VOID)
{
    AL_S32 Ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    Handle->Dev.Configs.SpiFrameFormat = SPI_STANDARD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.Trans.EnSpiCfg.InstLength = QSPI_INST_L8;

    InstAndAddr[0] = NOR_OP_SRSTEN;

    Ret = AlQspi_Hal_DmaStartBlockSend(Handle, DmaSendData, InstAndAddr, 0, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AL_NOR_RESET error:0x%x\r\n", Ret);
    }

    Handle->Dev.Configs.Trans.TransMode = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    Handle->Dev.Configs.SpiFrameFormat = SPI_STANDARD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    InstAndAddr[0] = NOR_OP_SRST;

    Ret = AlQspi_Hal_DmaStartBlockSend(Handle, DmaSendData, InstAndAddr, 0, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AL_NOR_RESET error:0x%x\r\n", Ret);
    }
}


/**
 * Enables write operations on the NOR flash memory using DMA.
 * It sets the QSPI controller to transmit-only mode, configures the necessary parameters, and sends the write enable (WREN) command to the NOR flash to allow write operations.
 *
 * @param None.
 * @return None.
 */
AL_VOID AlNorDma_Wren(AL_VOID)
{
    AL_S32 Ret = AL_OK;

    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.TransMode = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    Handle->Dev.Configs.SpiFrameFormat = SPI_STANDARD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.Trans.EnSpiCfg.InstLength = QSPI_INST_L8;

    InstAndAddr[0] = NOR_OP_WREN;

    Ret = AlQspi_Hal_DmaStartBlockSend(Handle, DmaSendData, InstAndAddr, 0, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "DMA AL_NOR_WREN error:0x%x\r\n", Ret);
    }
}


/**
 * Waits for the write-in-progress (WIP) bit to clear in the NOR flash's status register.
 * This function repeatedly reads the status register until the WIP bit is cleared, indicating that the NOR flash is ready for further operations. It uses DMA for the status register read operations.
 *
 * @param None.
 * @return None.
 */
AL_VOID AlNorDma_WaitWip(AL_VOID)
{
    AL_S32 Ret = AL_OK;

    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.TransMode  = QSPI_EEPROM;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    Handle->Dev.Configs.SpiFrameFormat = SPI_STANDARD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.Trans.EnSpiCfg.InstLength = QSPI_INST_L8;

    DmaSendData[0] = NOR_OP_RDSR;

    do {
        Ret = AlQspi_Hal_DmaStartBlockTranfer(Handle, DmaSendData, 1, DmaRecvData, 1, 100000);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AL_NOR_WAITWIP error:0x%x\r\n", Ret);
        }
#ifdef QSPI_DEBUG
        AL_LOG(AL_LOG_LEVEL_ERROR, "WAITWIP Nor Status1 Reg:%x\r\n", DmaRecvData[0]);
#endif
    } while (DmaRecvData[0] & SR_WIP);
}


/**
 * Reads the status register of the NOR flash memory using DMA.
 * This function configures the QSPI controller for EEPROM mode, sets up the read status register (RDSR) command, and performs a block transfer to read the status register.
 *
 * @param None.
 * @return None.
 */
AL_VOID AlNorDma_ReadStatus(AL_VOID)
{
    AL_S32 Ret = AL_OK;

    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.TransMode  = QSPI_EEPROM;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    Handle->Dev.Configs.SpiFrameFormat = SPI_STANDARD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;

    DmaSendData[0] = NOR_OP_RDSR;

    AlQspi_Hal_DmaStartBlockTranfer(Handle, DmaSendData, 1, DmaRecvData, 1, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "DMA AL_NOR_READSTATUS error:0x%x\r\n", Ret);
    }
#ifdef QSPI_DEBUG
    AL_LOG(AL_LOG_LEVEL_ERROR, "Nor Status1 Reg:%x\r\n", DmaRecvData[0]);
#endif
}


/**
 * Erases a sector in the NOR flash memory using DMA.
 * This function configures the QSPI controller for a transmit-only operation, sets up the sector erase (SE) command with the target sector address, and initiates the erase operation.
 *
 * @param addr The address of the sector to erase.
 * @return None.
 */
AL_VOID AlNorDma_Erase(AL_U32 addr)
{
    AL_S32 Ret = AL_OK;

    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.TransMode  = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L24;
    Handle->Dev.Configs.SpiFrameFormat = SPI_STANDARD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;

    InstAndAddr[0] = NOR_OP_SE;
    InstAndAddr[1] = (addr >> 16) & 0xff;
    InstAndAddr[2] = (addr >> 8)&0xff;
    InstAndAddr[3] = addr&0xff;

    Ret = AlQspi_Hal_DmaStartBlockSend(Handle, DmaSendData, InstAndAddr, 0, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AL_NOR_ERASE error:0x%x\r\n", Ret);
    }
}


/**
 * Reads a page from the NOR flash memory using DMA.
 * This function sets the QSPI controller for a receive-only operation, configures it for quad I/O read mode, sets up the read command with the target page address, and performs a block transfer to read the page data.
 *
 * @param addr The address of the page to read.
 * @return None.
 */
AL_VOID AlNorDma_ReadPage(AL_U32 addr)
{
    AL_S32 Ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode  = QSPI_RX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L24;
    Handle->Dev.Configs.SpiFrameFormat = SPI_QUAD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 8;

    DmaSendData[0] = NOR_OP_READ_1_1_4;
    DmaSendData[1] = (addr >> 16) & 0xff;
    DmaSendData[2] = (addr >> 8)&0xff;
    DmaSendData[3] = addr&0xff;

    Ret = AlQspi_Hal_DmaStartBlockTranfer(Handle, DmaSendData, 4, DmaRecvData, 240, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "DMA AL_NOR_READPAGE error:0x%x\r\n", Ret);
    }
}



/**
 * Writes a page to the NOR flash memory using DMA.
 * This function prepares the QSPI controller for a transmit-only operation in quad I/O mode, sets up the page program (PP) command with the target page address, fills a buffer with data to write, and initiates the write operation.
 *
 * @param addr The address of the page to write.
 * @return None.
 */
AL_VOID AlNorDma_WritePage(AL_U32 addr)
{
    AL_S32 Ret = AL_OK;

    Handle->Dev.Configs.Trans.TransMode  = QSPI_TX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L24;
    Handle->Dev.Configs.Trans.EnSpiCfg.InstLength = QSPI_INST_L8;
    Handle->Dev.Configs.SpiFrameFormat = SPI_QUAD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;

    InstAndAddr[0] = NOR_OP_PP_1_1_4;
    InstAndAddr[1] = (addr >> 16) & 0xff;
    InstAndAddr[2] = (addr >> 8)&0xff;
    InstAndAddr[3] = addr&0xff;

    AL_U32 i = 0;
    for (i = 0; i < 400; i++) {
        DmaSendData[i] = i % 255;
    }
    Ret = AlQspi_Hal_DmaStartBlockSend(Handle, DmaSendData, InstAndAddr, 240, 1000000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "DMA AL_NOR_WRITEPAGE error:0x%x\r\n", Ret);
    }
}

/**
 * Reads the identification (ID) of the NOR flash memory using DMA.
 * This function configures the QSPI controller for EEPROM mode, sets up the read ID (RDID) command, and performs a block transfer to read the flash memory's ID.
 *
 * @param None.
 * @return None.
 */
AL_VOID AlNorDma_ReadId(AL_VOID)
{
    AL_S32 Ret = AL_OK;
    Handle->Dev.Configs.Trans.TransMode  = QSPI_EEPROM;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    Handle->Dev.Configs.Trans.EnSpiCfg.InstLength = QSPI_INST_L8;
    Handle->Dev.Configs.SpiFrameFormat = SPI_STANDARD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;

    DmaSendData[0] = NOR_OP_RDID;;

    Ret = AlQspi_Hal_DmaStartBlockTranfer(Handle, DmaSendData, 1, FlashId, 3, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AL_NOR_READID error:0x%x\r\n", Ret);
    }

    AL_LOG(AL_LOG_LEVEL_ERROR, "DMA Read Flash ID:0x%x, 0x%x, 0x%x\r\n", FlashId[0], FlashId[1], FlashId[2]);
}


/**
 * @brief Main function for the Qspi DMA test.
 */
AL_VOID main(AL_VOID)
{
    AL_U32 i;
    AL_S32 Ret = AL_OK;

    AL_LOG(AL_LOG_LEVEL_ERROR, "Start FPSoc Qspi DMA Test\r\n");

    Ret = AlQspi_Hal_Init(&Handle, &QspiX4InitConfigs, AL_NULL, 0);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlQspi_Hal_Init error:0x%x\r\n", Ret);
    }

    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    AlNorDma_ReadId();

    AlNorDma_Wren();
    AlNorDma_Erase(0);
    AlNorDma_WaitWip();

    /**/
    AlNorDma_ReadPage(0);
    for (i = 0; i < 240; i++) {
        if(0xff != DmaRecvData[i]) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlQspi test erase norflash error\r\n");
            AL_LOG(AL_LOG_LEVEL_ERROR, "Error DmaRecvData[%d]:%d\r\n", i, DmaRecvData[i]);
            while (1);
        }
    }

    AL_LOG(AL_LOG_LEVEL_ERROR, "DMA AlQspi test erase norflash success\r\n");

    /**/
    AlNorDma_Wren();
    AlNorDma_WritePage(0);
    AlNorDma_WaitWip();

    AlNorDma_ReadPage(0);
    for (i = 0; i < 230; i++) {
        if(i != DmaRecvData[i]) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "DMA AlQspi data write norflash test error\r\n");
            AL_LOG(AL_LOG_LEVEL_ERROR, "Error DmaRecvData[%d]:%d\r\n", i, DmaRecvData[i]);
            while (1);
        }
    }
    AL_LOG(AL_LOG_LEVEL_ERROR, "DMA AlQspi test write norflash success\r\n");
}

