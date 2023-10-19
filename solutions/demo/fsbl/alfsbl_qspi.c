/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_qspi_hal.h"
#include "al_spinor.h"
#include "al_dmacahb_hal.h"

#include "alfsbl_secure.h"
#include "alfsbl_boot.h"

AL_QSPI_HalStruct *Handle;

AL_QSPI_ConfigsStruct QspiX4InitConfigs =
{
    .ClkDiv             = 20,
    .SamplDelay         = 2,
    .SlvToggleEnum      = QSPI_SLV_TOGGLE_DISABLE,
    .SpiFrameFormat     = SPI_QUAD_FORMAT,
    .ClockStretch       = QSPI_EnableClockStretch
};

AL_U8 __attribute__((aligned(4))) FlashId[4] = { 0x0 };

AL_VOID AlNor_Wren(AL_VOID)
{
    AL_S32 Ret = AL_OK;
    AL_U8 SendData[4] = {0x0};

    Handle->Dev.Configs.Trans.TransMode = QSPI_TX_ONLY;
    Handle->Dev.Configs.SpiFrameFormat = SPI_STANDARD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.Trans.EnSpiCfg.InstLength = QSPI_INST_L8;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;

    SendData[0] = NOR_OP_WREN;

    Ret = AlQspi_Hal_SendDataBlock(Handle, SendData, 1, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AL_NOR_WREN error:0x%x\r\n", Ret);
    }
}


AL_VOID AlNor_WaitWip(AL_VOID)
{
    AL_S32 Ret = AL_OK;
    AL_U8 SendData[4] = {0x0};
    AL_U8 RecvData[4] = {0x0};

    Handle->Dev.Configs.Trans.TransMode  = QSPI_EEPROM;
    Handle->Dev.Configs.SpiFrameFormat = SPI_STANDARD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    Handle->Dev.Configs.Trans.EnSpiCfg.InstLength = QSPI_INST_L8;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;

    SendData[0] = NOR_OP_RDSR;

    do {
        Ret = AlQspi_Hal_TranferDataBlock(Handle, SendData, 1, RecvData, 1, 100000);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AL_NOR_WAITWIP error:0x%x\r\n", Ret);
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

    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.Trans.TransMode  = QSPI_EEPROM;
    Handle->Dev.Configs.SpiFrameFormat = SPI_STANDARD_FORMAT;

    Ret = AlQspi_Hal_TranferDataBlock(Handle, SendData, 1, &Data, 1, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_SetQuad ReadQuadCmd error\r\n");
    }

    Data = Data | (1 << QuadPos);

    AlNor_Wren();

    SendData[0] = SetQuadCmd;
    SendData[1] = Data;

    Handle->Dev.Configs.Trans.TransMode = QSPI_TX_ONLY;

    Ret = AlQspi_Hal_SendDataBlock(Handle, SendData, 2, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor_SetQuad SetQuadCmd error\r\n");
    }

    AlNor_WaitWip();

    return Ret;
}


uint32_t AlFsbl_Qspi24Init(uint32_t *pBlockSizeMax)
{
    AL_U32 Ret;
    AL_U8 SendData[4] = {0x0};

    Ret = AlQspi_Hal_Init(&Handle, &QspiX4InitConfigs, AL_NULL, 0);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlQspi_Hal_Init error:0x%x\r\n", Ret);
    }

    SendData[0] = NOR_OP_RDID;

    Handle->Dev.Configs.Trans.TransMode  = QSPI_EEPROM;
    Handle->Dev.Configs.SpiFrameFormat  = SPI_STANDARD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.Trans.EnSpiCfg.InstLength = QSPI_INST_L8;

    Ret = AlQspi_Hal_TranferDataBlock(Handle, SendData, 1, FlashId, 3, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AL_NOR_READID error:0x%x\r\n", Ret);
    }
    AL_LOG(AL_LOG_LEVEL_INFO, "Flash ID:0x%x, 0x%x, 0x%x\r\n", FlashId[0], FlashId[1], FlashId[2]);

    if((FlashId[0] != 0x01) && (FlashId[0] != 0x20) && (FlashId[0] != 0x0) && (FlashId[0] != 0xff)) {
        if((FlashId[0] != 0x9d) && (FlashId[0] != 0xc2)) {
            Ret = AlNor_SetQuad(0x31, 0x35, 1);
        } else {
            Ret = AlNor_SetQuad(0x01, 0x05, 6);
        }
    }
	*pBlockSizeMax = 8*1024;

	return Ret;
}

uint32_t AlFsbl_Qspi24Copy(uint64_t SrcAddress, PTRSIZE DestAddress, uint32_t Length, SecureInfo *pSecureInfo)
{
	uint32_t Ret, TranHandledCnt = 0;
    uint32_t i;
    AL_U16 RecvSize;

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

    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L24;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 8;
    Handle->Dev.Configs.SpiFrameFormat = SPI_QUAD_FORMAT;
    Handle->Dev.Configs.Trans.TransMode  = QSPI_RX_ONLY;

    SendData[0] = NOR_OP_READ_1_1_4;
    i = Length / 65532 + (Length % 65532 ? 1 : 0);

    for (; i > 0; i--) {
        SendData[1] = (SrcAddress >> 16) & 0xff;
        SendData[2] = (SrcAddress >> 8)&0xff;
        SendData[3] = SrcAddress&0xff;

        RecvSize = (Length > 65532) ? 65532 : Length;
        Ret = AlQspi_Hal_TranferDataBlock(Handle, SendData, 4, (AL_U8 *)DestAddress, RecvSize, 10000000);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AL_NOR_READPAGE error:0x%x\r\n", Ret);
        }
        SrcAddress += RecvSize;
        DestAddress += RecvSize;
        Length -= RecvSize;
    }
#ifdef ENABLE_MMU
    AlCache_FlushDcacheAll();
    //AlCache_FlushDcacheRange(DestAddress, RecvSize);
#endif

#endif

	if(Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor read byte error\r\n");
		Ret = Ret | (ALFSBL_BOOTMODE_QSPI24 << 16);
	}

	return Ret;
}

uint32_t AlFsbl_Qspi24Release(void)
{
	return 0;
}


