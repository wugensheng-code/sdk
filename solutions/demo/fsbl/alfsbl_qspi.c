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
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.Trans.TransMode  = QSPI_EEPROM;
    Handle->Dev.Configs.SpiFrameFormat = SPI_STANDARD_FORMAT;

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


AL_VOID AlNor_ReadPage_1_1_4(AL_U32 addr)
{
    AL_S32 Ret = AL_OK;
    AL_U8 SendData[256];
    AL_U8 RecvData[256];
    AL_U32 i;

    Handle->Dev.Configs.Trans.TransMode  = QSPI_RX_ONLY;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT1;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L24;
    Handle->Dev.Configs.SpiFrameFormat = SPI_QUAD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0x0a;

    SendData[0] = NOR_OP_READ_1_4_4;
    SendData[1] = (addr >> 16) & 0xff;
    SendData[2] = (addr >> 8)&0xff;
    SendData[3] = addr&0xff;

    Ret = AlQspi_Hal_TranferDataBlock(Handle, SendData, 4, RecvData, 64, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AL_NOR_READPAGE error:0x%x\r\n", Ret);
    }

    for (i = 0; i < 64; i ++) {
        AL_LOG(AL_LOG_LEVEL_INFO, "%d:0x%x", i, RecvData[i]);
    }
}


AL_S32 AlNor_SetWrap(void)
{
    AL_S32  Ret = AL_OK;
    AL_U8 SendData[4] = {0x0}, Data = 0;
    AL_U8 v_data[10];

    Handle->Dev.Configs.Trans.TransMode  = QSPI_EEPROM;
    Handle->Dev.Configs.SpiFrameFormat  = SPI_STANDARD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 0;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L0;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.Trans.EnSpiCfg.InstLength = QSPI_INST_L8;

    //0x85: read volatile register
    SendData[0] = 0x85;
    v_data[0] = 0;
    Ret = AlQspi_Hal_TranferDataBlock(Handle, SendData, 1, v_data, 5, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AL_NOR_READID error:0x%x\r\n", Ret);
    }

    AlNor_Wren();

    SendData[0] = 0x81;
    SendData[1] = 0xf2;

    Ret = AlQspi_Hal_SendDataBlock(Handle, SendData, 2, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AL_NOR_READID error:0x%x\r\n", Ret);
    }

    AlNor_WaitWip();

    SendData[0] = 0x85;
    v_data[0] = 0;
    Ret = AlQspi_Hal_TranferDataBlock(Handle, SendData, 1, v_data, 5, 100000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AL_NOR_READID error:0x%x\r\n", Ret);
    }

    return Ret;
}



AL_U32 AlFsbl_QspiInit(void)
{
    AL_U32 Ret;
    AL_U8 SendData[8] = {0x0};

    AL_REG32_SET_BITS(0xF8801030UL, 0, 6, 4); /// set div_qspi 5 bansed on IO 1000，then div 4, generate 50M SCLK

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
        if((FlashId[0] == 0xc8) && (FlashId[1] == 0x60)) {
            Ret = AlNor_SetQuad2ByteMode(0x1, 0x5, 0x35, 9);
        } else  if((FlashId[0] != 0x9d) && (FlashId[0] != 0xc2)) {
            Ret = AlNor_SetQuad1ByteMode(0x31, 0x35, 1);
        } else {
            Ret = AlNor_SetQuad1ByteMode(0x01, 0x05, 6);
        }
    }

    return Ret;
}

AL_U32 AlFsbl_Qspi24Init(void)
{
    AL_U32 Ret;
    Ret = AlFsbl_QspiInit();
    if (Ret != AL_OK) {
        return Ret;
    }

#ifdef QSPI_XIP_THROUTH_CSU_DMA
    Ret = AlQspi_Dev_XipAddr24InitForDMA(&Handle->Dev);
#endif

    return Ret;
}

AL_U32 AlFsbl_Qspi32Init(void)
{
    AL_U32 Ret;
    Ret = AlFsbl_QspiInit();
    if (Ret != AL_OK) {
        return Ret;
    }

#ifdef QSPI_XIP_THROUTH_CSU_DMA
    Ret = AlQspi_Dev_XipAddr32Init(&Handle->Dev);
#endif

    return Ret;
}

AL_U32 AlFsbl_Qspi24Copy(PTRSIZE SrcAddress, PTRSIZE DestAddress, AL_U32 Length, SecureInfo *pSecureInfo)
{
    AL_U32 Ret = 0;
    AL_U32 i;
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

#elif (defined QSPI_USE_AHB_DMA)
	AL_U8 SendData[4] = {0x0};
    AL_LOG(AL_LOG_LEVEL_DEBUG, "ahb dma mode\r\n");
    Handle->Dev.Configs.Trans.TransMode  = QSPI_RX_ONLY;
    Handle->Dev.Configs.SpiFrameFormat = SPI_QUAD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 8;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L24;
    Handle->Dev.Configs.Trans.EnSpiCfg.InstLength = QSPI_INST_L8;

    SendData[0] = NOR_OP_READ_1_1_4;
    i = Length / 4032 + (Length % 4032 ? 1 : 0);

    for (; i > 0; i--) {
        SendData[1] = (SrcAddress >> 16) & 0xff;
        SendData[2] = (SrcAddress >> 8)&0xff;
        SendData[3] = SrcAddress&0xff;

        RecvSize = (Length > 4032) ? 4032 : Length;

        Ret = AlQspi_Hal_DmaStartTranferBlock(Handle, SendData, 4, (AL_U8 *)DestAddress, RecvSize, 50000000);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "DMA AL_NOR_READPAGE error:0x%x\r\n", Ret);
        }
        SrcAddress += RecvSize;
        DestAddress += RecvSize;
        Length -= RecvSize;
    }

#else
    AL_U8 SendData[4] = {0x0};
    AL_LOG(AL_LOG_LEVEL_DEBUG, "regular mode\r\n");
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L24;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 8;
    Handle->Dev.Configs.Trans.EnSpiCfg.InstLength = QSPI_INST_L8;
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

#endif

#ifdef ENABLE_MMU
    AlCache_FlushDcacheAll();
#endif

    if(Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor read byte error\r\n");
        Ret = Ret | (ALFSBL_BOOTMODE_QSPI24 << 16);
    }

    return Ret;
}


AL_U32 AlFsbl_Qspi32Copy(PTRSIZE SrcAddress, PTRSIZE DestAddress, AL_U32 Length, SecureInfo *pSecureInfo)
{
    AL_U32 Ret = 0;
    AL_U32 i;
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

#elif (defined QSPI_USE_AHB_DMA)
    AL_U8 SendData[5] = {0x0};
    Handle->Dev.Configs.Trans.TransMode  = QSPI_RX_ONLY;
    Handle->Dev.Configs.SpiFrameFormat = SPI_QUAD_FORMAT;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 8;
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L32;
    Handle->Dev.Configs.Trans.EnSpiCfg.InstLength = QSPI_INST_L8;

    SendData[0] = NOR_OP_READ_1_1_4_4B;
    i = Length / 4032 + (Length % 4032 ? 1 : 0);

    for (; i > 0; i--) {
        SendData[1] = (SrcAddress >> 24) & 0xff;
        SendData[2] = (SrcAddress >> 16) & 0xff;
        SendData[3] = (SrcAddress >> 8)&0xff;
        SendData[4] = SrcAddress&0xff;

        RecvSize = (Length > 4032) ? 4032 : Length;

        Ret = AlQspi_Hal_DmaStartTranferBlock(Handle, SendData, 5, (AL_U8 *)DestAddress, RecvSize, 5000000);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AL_NOR_READPAGE error:0x%x\r\n", Ret);
        }
        SrcAddress += RecvSize;
        DestAddress += RecvSize;
        Length -= RecvSize;
    }

#else
    AL_U8 SendData[5] = {0x0};
    Handle->Dev.Configs.Trans.EnSpiCfg.TransType = QSPI_TT0;
    Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength = QSPI_ADDR_L32;
    Handle->Dev.Configs.Trans.EnSpiCfg.WaitCycles = 8;
    Handle->Dev.Configs.Trans.EnSpiCfg.InstLength = QSPI_INST_L8;
    Handle->Dev.Configs.SpiFrameFormat = SPI_QUAD_FORMAT;
    Handle->Dev.Configs.Trans.TransMode  = QSPI_RX_ONLY;

    SendData[0] = NOR_OP_READ_1_1_4_4B;
    i = Length / 65532 + (Length % 65532 ? 1 : 0);

    for (; i > 0; i--) {
        SendData[1] = (SrcAddress >> 24) & 0xff;
        SendData[2] = (SrcAddress >> 16) & 0xff;
        SendData[3] = (SrcAddress >> 8)&0xff;
        SendData[4] = SrcAddress&0xff;

        RecvSize = (Length > 65532) ? 65532 : Length;
        Ret = AlQspi_Hal_TranferDataBlock(Handle, SendData, 5, (AL_U8 *)DestAddress, RecvSize, 10000000);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AL_NOR_READPAGE error:0x%x\r\n", Ret);
        }
        SrcAddress += RecvSize;
        DestAddress += RecvSize;
        Length -= RecvSize;
    }

#endif

#ifdef ENABLE_MMU
    AlCache_FlushDcacheAll();
#endif

    if(Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlNor read byte error\r\n");
        Ret = Ret | (ALFSBL_BOOTMODE_QSPI24 << 16);
    }

    return Ret;
}

AL_U32 AlFsbl_Qspi24Release(void)
{
    return 0;
}

AL_U32 AlFsbl_QspiXipInit(void)
{
    AlNor_SetWrap();
    return AlQspi_Dev_XipAddr24Init(&Handle->Dev);
}
