/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_qspi_hal.h"
#include "al_spinor.h"

#include <alfsbl_secure.h>
#include "alfsbl_boot.h"

AL_QSPI_HalStruct QspiHal;

AL_QSPI_ConfigsStruct QspiInitConfigs =
{
    .TransMode          = QSPI_EEPROM,
    .SpiFrameFormat     = SPI_STANDARD_FORMAT,
    .DataFrameSize      = QSPI_DFS_8BITS,
    .EnSpiCfg.AddrLength    = QSPI_ADDR_L0,
    .EnSpiCfg.InstLength    = QSPI_INST_L8,
    .EnSpiCfg.TransType     = QSPI_TT0,
    .EnSpiCfg.WaitCycles    = 0,
    .EnSpiCfg.ClockStretch  = QSPI_EnableClockStretch,
    .ClkDiv             = 10,
    .SamplDelay         = 0,
    .SlvToggleEnum      = QSPI_SLV_TOGGLE_DISABLE,
    .SlvSelEnum         = QSPI_SER_SS0_EN,
    .IsUseDma           = AL_QSPI_USE_INTR
};
AL_U8 __attribute__((aligned(4))) FlashId[4] = { 0x0 };

AL_VOID AlNor_Wren(AL_VOID)
{
    AL_S32 Ret = AL_OK;
    AL_U8 SendData[4] = {0x0};

    QspiHal.Dev->Configs.EnSpiCfg.WaitCycles = 0;
    QspiHal.Dev->Configs.TransMode = QSPI_TX_ONLY;
    QspiHal.Dev->Configs.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    QspiHal.Dev->Configs.SpiFrameFormat = SPI_STANDARD_FORMAT;
    QspiHal.Dev->Configs.EnSpiCfg.TransType = QSPI_TT0;

    SendData[0] = NOR_OP_WREN;

    Ret = AlQspi_Hal_SendDataBlock(&QspiHal, SendData, 1, 10000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_DEBUG, "AL_NOR_WREN error\r\n");
    }
}


AL_VOID AlNor_WaitWip(AL_VOID)
{
    AL_S32 Ret = AL_OK;
    AL_U8 SendData[4] = {0x0};
    AL_U8 RecvData[4] = {0x0};

    QspiHal.Dev->Configs.EnSpiCfg.WaitCycles = 0;
    QspiHal.Dev->Configs.TransMode  = QSPI_EEPROM;
    QspiHal.Dev->Configs.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    QspiHal.Dev->Configs.SpiFrameFormat = SPI_STANDARD_FORMAT;
    QspiHal.Dev->Configs.EnSpiCfg.TransType = QSPI_TT0;

    SendData[0] = NOR_OP_RDSR;

    do {
        Ret = AlQspi_Hal_TranferDataBlock(&QspiHal, SendData, 1, RecvData, 1, 100000);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_DEBUG, "AL_NOR_WAITWIP error\r\n");
        }
#ifdef QSPI_DEBUG
        AL_LOG(AL_LOG_LEVEL_DEBUG, "WAITWIP Nor Status1 Reg:%x\r\n", RecvData[0]);
#endif
    } while (RecvData[0] & SR_WIP);
}

AL_S32 AlNor_SetQuad(AL_U8 SetQuadCmd, AL_U8 ReadQuadCmd, AL_U8 QuadPos)
{
    AL_S32  Ret = AL_OK;
    AL_U8 SendData[4] = {0x0}, Data = 0;

    SendData[0] = ReadQuadCmd;
    QspiHal.Dev->Configs.EnSpiCfg.WaitCycles = 0;
    QspiHal.Dev->Configs.TransMode  = QSPI_EEPROM;
    QspiHal.Dev->Configs.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    QspiHal.Dev->Configs.SpiFrameFormat = SPI_STANDARD_FORMAT;
    QspiHal.Dev->Configs.EnSpiCfg.TransType = QSPI_TT0;

    Ret = AlQspi_Hal_TranferDataBlock(&QspiHal, SendData, 1, &Data, 1, 10000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_DEBUG, "AlNor_SetQuad ReadQuadCmd error\r\n");
    }

    Data = Data | (1 << QuadPos);

    AlNor_Wren();

    SendData[0] = SetQuadCmd;
    SendData[1] = Data;

    QspiHal.Dev->Configs.TransMode = QSPI_TX_ONLY;

    Ret = AlQspi_Hal_SendDataBlock(&QspiHal, SendData, 2, 10000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_DEBUG, "AlNor_SetQuad SetQuadCmd error\r\n");
    }

    AlNor_WaitWip();

    return Ret;
}

#ifdef SIMU_AL9000_DV
uint32_t AlFsbl_Qspi24Init(uint32_t *pBlockSizeMax)
{
	QspiParams qspi_params;
	Csu_QspiInit(&qspi_params);

//	QSPI_FLASH_SR_BIT_SET(9, 1);
    Csu_QspiSetMode(QSPI_WIDTH_X2, QSPI_ADDR_24);

    Qspi_Disable(g_pdev);
	Qspi_SckdivCfg(g_pdev,0x2); // ahb: 200M, spi: 200 / 4 = 50M

	g_pdev->regs->SPI_CTRLR0 = (g_pdev->regs->SPI_CTRLR0) | (1 << 30);

    Qspi_Enable(g_pdev);

    *pBlockSizeMax = 512;   // this is only for simulation test

	return 0;
}

#else
uint32_t AlFsbl_Qspi24Init(uint32_t *pBlockSizeMax)
{
    AL_U32 Ret;
    AL_U8 SendData[4] = {0x0};

    Ret = AlQspi_Hal_Init(&QspiHal, &QspiInitConfigs, AL_NULL, AL_NULL, 10000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Fsbl AlQspi_Hal_Init error\r\n");
    }

    QspiHal.Dev->Configs.EnSpiCfg.WaitCycles = 0;
    QspiHal.Dev->Configs.TransMode  = QSPI_EEPROM;
    QspiHal.Dev->Configs.EnSpiCfg.AddrLength = QSPI_ADDR_L0;

    SendData[0] = NOR_OP_RDID;
    Ret = AlQspi_Hal_TranferDataBlock(&QspiHal, SendData, 1, FlashId, 3, 1);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_DEBUG, "AlNor read id error\r\n");
    }
    AL_LOG(AL_LOG_LEVEL_DEBUG, "Flash ID:0x%x, 0x%x, 0x%x\r\n", FlashId[0], FlashId[1], FlashId[2]);

    if((FlashId[0] != 0x01) && (FlashId[0] != 0x20) && (FlashId[0] != 0x0) && (FlashId[0] != 0xff)) {
        if((FlashId[0] != 0x9d) && (FlashId[0] != 0xc2)) {
            Ret = AlNor_SetQuad(0x31, 0x35, 1);
        } else {
            Ret = AlNor_SetQuad(0x01, 0x05, 6);
        }
    }

	return Ret;
}
#endif

uint32_t AlFsbl_Qspi24Copy(uint64_t SrcAddress, PTRSIZE DestAddress, uint32_t Length, SecureInfo *pSecureInfo)
{
	uint32_t Ret;
#ifdef QSPI_XIP_THROUTH_CSU_DMA
	AL_LOG(AL_LOG_LEVEL_DEBUG, "xip mode\r\n");
	if(pSecureInfo != NULL) {
		pSecureInfo->InputAddr  = SrcAddress + QSPI_XIP_BASEADDR;
		pSecureInfo->OutputAddr = DestAddress;
		pSecureInfo->DataLength = Length;

		Ret = AlFsbl_DecHash(pSecureInfo);
	}
	else {
		Ret = AlFsbl_CsuDmaCopy(
				SrcAddress + QSPI_XIP_BASEADDR,
				DestAddress,
				Length,
				CSUDMA_DST_INCR | CSUDMA_SRC_INCR);
	}
#else
    AL_U8 SendData[4] = {0x0};

    QspiHal.Dev->Configs.TransMode  = QSPI_RX_ONLY;
    QspiHal.Dev->Configs.EnSpiCfg.TransType = QSPI_TT0;
    QspiHal.Dev->Configs.EnSpiCfg.AddrLength = QSPI_ADDR_L24;
    QspiHal.Dev->Configs.SpiFrameFormat = SPI_QUAD_FORMAT;
    QspiHal.Dev->Configs.EnSpiCfg.WaitCycles = 8;

    SendData[0] = NOR_OP_READ_1_1_4;
    SendData[1] = (SrcAddress >> 16)&0xff;
    SendData[2] = (SrcAddress >> 8)&0xff;
    SendData[3] = SrcAddress&0xff;

    Ret = AlQspi_Hal_TranferDataBlock(&QspiHal, SendData, 4, (AL_U8 *)DestAddress, Length, 10000000);
#endif

	if(Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_DEBUG, "AlNor read byte error\r\n");
		Ret = Ret | (ALFSBL_BOOTMODE_QSPI24 << 16);
	}

	return Ret;
}

uint32_t AlFsbl_Qspi24Release(void)
{
	return 0;
}


