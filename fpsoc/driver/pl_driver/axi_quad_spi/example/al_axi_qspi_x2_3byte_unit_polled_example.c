/**
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * 
 * @file    al_axi_qspi_x1_intr_example.c
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2024-08-27
 * @brief   axi spi x1 nor example, test on flash S25FL256SAGMFIR01
 */

/***************************** Include Files *********************************/
#include <stdio.h>
#include "al_axi_quad_spi_hal.h"
#include "al_axi_quad_spi_dev.h"
#include "al_intr.h"
#include "al_osal.h"
#include "al_reg_io.h"
#include "al_spinor.h"
#include "al_hwcfg.h"

/***************** Macros (Inline Functions) Definitions *********************/
#define ASSERT_EQUAL(expected, actual) \
    if ((expected) != (actual)) { \
        printf("Assertion failed: %s == %s, expected: %d, actual: %d, file: %s, line: %d\n", \
               #expected, #actual, (expected), (actual), __FILE__, __LINE__); \
        return 1; \
    }

#define RUN_TEST(test) \
    if (test()) { \
        printf("%s: FAILED\n", #test); \
    } else { \
        printf("%s: PASSED\n", #test); \
    }

#define QSPI_FLASH_PAGE_SIZE         256
#define AL_AXI_QSPI_TEST_SIZE       1024
#define TIMEOUT_FOR_TEST          100000

#define TOP_NS_PLS_PROT_OFFSET                 0x80UL
#define TOP_CRP_SRST_CTRL2_OFFSET              0x78UL

/************************** Variable Definitions *****************************/
static AL_U8 SendData[AL_AXI_QSPI_TEST_SIZE] = { 0 };
static AL_U8 RecvData[AL_AXI_QSPI_TEST_SIZE] = { 0 };

/************************** Function Prototypes ******************************/
static AL_VOID GP_Port_Enable(AL_VOID);
static AL_U32 AlAxiQspi_FlashReadStatusReg(AlAxiQspi_HalStruct *AlAxiQspiHandle, AL_U8 *StatusReg);
static AL_U32 AlAxiQspi_FlashReadId(AlAxiQspi_HalStruct *AlAxiQspiHandle, AL_U8 *Id, AL_U32 IdSize);
static AL_U32 AlAxiQspi_FlashWren(AlAxiQspi_HalStruct *AlAxiQspiHandle);
static AL_U32 AlAxiQspi_FlashEraseChip(AlAxiQspi_HalStruct *AlAxiQspiHandle);
static AL_U32 AlAxiQspi_FlashEraseSector(AlAxiQspi_HalStruct *AlAxiQspiHandle, AL_U32 Addr);
static AL_U32 AlAxiQspi_FlashWaitWip(AlAxiQspi_HalStruct *AlAxiQspiHandle, AL_U32 Msleep);
static AL_U32 AlAxiQspi_FlashOutputRead(AlAxiQspi_HalStruct *AlAxiQspiHandle, AL_U8 *Data, AL_U32 Addr, AL_U32 Size);
static AL_U32 AlAxiQspi_FlashWritePage(AlAxiQspi_HalStruct *AlAxiQspiHandle, AL_U8 *Data, AL_U32 Addr, AL_U32 Size);
static AL_U32 AlAxiQspi_FlashIORead(AlAxiQspi_HalStruct *AlAxiQspiHandle, AL_U8 *Data, AL_U32 Addr, AL_U32 Size);
static AL_U32 AlAxiQspi_FlashWrite(AlAxiQspi_HalStruct *AlAxiQspiHandle, AL_U8 *Data, AL_U32 Addr, AL_U32 Size);

/************************** Function Definitions ******************************/

AL_U32 AlAxiQspi_X2_Polled_3Byte_example()
{
    AL_U8 StatusReg = 0x0U;
    AL_U8 FlashId[3] = {0};
    AL_U32 Status = AL_OK;
    AL_U32 Index = 0;
    AL_U32 TestSize = AL_AXI_QSPI_TEST_SIZE;
    AlAxiQspi_HalStruct *AlAxiQspiHandle;

    AL_LOG(AL_LOG_LEVEL_INFO, "Axi-qspi x2 3 byte polled mode flash test start...\r\n");

    GP_Port_Enable();
    AL_LOG(AL_LOG_LEVEL_INFO, "Enable GP port.\r\n");

    Status = AlAxiQspi_Hal_Init(&AlAxiQspiHandle, 0, AL_NULL, AL_NULL);

    if (Status != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Axi-qspi init error: 0x%x\r\n", Status);
        return Status;
    }

    Status = AlAxiQspi_FlashReadId(AlAxiQspiHandle, FlashId, 3);
    if (Status != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Read id error: 0x%x\r\n", Status);
        return Status;
    } else {
        AL_LOG(AL_LOG_LEVEL_INFO, "Read flash id: 0x%x, 0x%x, 0x%x\r\n", FlashId[0], FlashId[1], FlashId[2]);
    }

    //read status register
    Status = AlAxiQspi_FlashReadStatusReg(AlAxiQspiHandle, &StatusReg);
    if (Status != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Read status register error: 0x%x\r\n", Status);
        return Status;
    } else {
        AL_LOG(AL_LOG_LEVEL_INFO, "Read status reg: 0x%x\r\n", StatusReg);
    }

    // Erase sector at address 0x000000(NO 1 sector)
    Status = AlAxiQspi_FlashEraseSector(AlAxiQspiHandle, 0x00);
    if (Status != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Erase sector error: 0x%x\r\n", Status);
        return Status;
    } else {
        AL_LOG(AL_LOG_LEVEL_INFO, "Erase sector at address 0x000000\r\n");
    }

    RecvData[0] = 0x00;
    Status = AlAxiQspi_FlashIORead(AlAxiQspiHandle, RecvData, 0x00, QSPI_FLASH_PAGE_SIZE);
    if (Status != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Flash read error: 0x%x\r\n", Status);
        return Status;
    }

    for (Index = 0; Index < QSPI_FLASH_PAGE_SIZE; Index++) {
        if (RecvData[Index] != 0xFF) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Data error after sector erase\r\n");
            while (1);
        }
    }

    RecvData[0] = 0x00;
    // read data with output read command
    Status = AlAxiQspi_FlashOutputRead(AlAxiQspiHandle, RecvData, 0x00, TestSize);
    if (Status != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Flash read error: 0x%x\r\n", Status);
        return Status;
    }

    for (Index = 0; Index < QSPI_FLASH_PAGE_SIZE; Index++) {
        if (RecvData[Index] != 0xFF) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Data error after sector erase\r\n");
            while (1);
        }
    }

    // Erase sector at address 0x20000 (NO 3 sector)
    Status = AlAxiQspi_FlashEraseSector(AlAxiQspiHandle, 0x20000);
    if (Status != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Erase sector error: 0x%x\r\n", Status);
        return Status;
    } else {
        AL_LOG(AL_LOG_LEVEL_INFO, "Erase sector at address 0x20000\r\n");
    }

    RecvData[0] = 0x00;
    Status = AlAxiQspi_FlashIORead(AlAxiQspiHandle, RecvData, 0x20000, QSPI_FLASH_PAGE_SIZE);
    if (Status != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Flash read error: 0x%x\r\n", Status);
        return Status;
    }

    for (Index = 0; Index < QSPI_FLASH_PAGE_SIZE; Index++) {
        if (RecvData[Index] != 0xFF) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Data error after sector erase\r\n");
            while (1);
        }
    }

    RecvData[0] = 0x00;
    // read data with output read command
    Status = AlAxiQspi_FlashOutputRead(AlAxiQspiHandle, RecvData, 0x20000, TestSize);
    if (Status != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Flash read error: 0x%x\r\n", Status);
        return Status;
    }

    for (Index = 0; Index < QSPI_FLASH_PAGE_SIZE; Index++) {
        if (RecvData[Index] != 0xFF) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Data error after sector erase\r\n");
            while (1);
        }
    }

    for (Index = 0; Index < TestSize; Index++) {
        SendData[Index] = (Index + 100) % 256;
    }

    // write sector at address 0x000000(NO 1 sector)
    Status = AlAxiQspi_FlashReadStatusReg(AlAxiQspiHandle, &StatusReg);
    if (Status != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Read status register error: 0x%x\r\n", Status);
        return Status;
    } else {
        AL_LOG(AL_LOG_LEVEL_INFO, "Read status reg: 0x%x\r\n", StatusReg);
    }

    Status = AlAxiQspi_FlashWrite(AlAxiQspiHandle, SendData, 0x00, TestSize);
    if (Status != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Flash write error: 0x%x\r\n", Status);
        return Status;
    }

    RecvData[0] = 0x00;
   // read data after write with output read command
    Status = AlAxiQspi_FlashOutputRead(AlAxiQspiHandle, RecvData, 0x00, TestSize);
    if (Status != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Flash read error: 0x%x\r\n", Status);
        return Status;
    }

    for (Index = 0; Index < TestSize; Index++) {
        if (RecvData[Index] != SendData[Index]) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Data error after write date at Index %d\r\n", Index);
            while (1);
        }
    }

     RecvData[0] = 0x00;
    Status = AlAxiQspi_FlashIORead(AlAxiQspiHandle, RecvData, 0x00, QSPI_FLASH_PAGE_SIZE);
    if (Status != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Flash read error: 0x%x\r\n", Status);
        return Status;
    }
    for (Index = 0; Index < TestSize; Index++) {
        if (RecvData[Index] != SendData[Index]) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Data error after write date at Index %d\r\n", Index);
            while (1);
        }
    }


    // write sector at address 0x000000(NO 3 sector)
    Status = AlAxiQspi_FlashReadStatusReg(AlAxiQspiHandle, &StatusReg);
    if (Status != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Read status register error: 0x%x\r\n", Status);
        return Status;
    } else {
        AL_LOG(AL_LOG_LEVEL_INFO, "Read status reg: 0x%x\r\n", StatusReg);
    }

    Status = AlAxiQspi_FlashWrite(AlAxiQspiHandle, SendData, 0x20000, TestSize);
    if (Status != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Flash write error: 0x%x\r\n", Status);
        return Status;
    }

    // read data after write with output read command
    RecvData[0] = 0x00;
    Status = AlAxiQspi_FlashOutputRead(AlAxiQspiHandle, RecvData, 0x20000, TestSize);
    if (Status != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Flash read error: 0x%x\r\n", Status);
        return Status;
    }

    for (Index = 0; Index < TestSize; Index++) {
        if (RecvData[Index] != SendData[Index]) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Data error after write date at Index %d\r\n", Index);
            while (1);
        }
    }

    RecvData[0] = 0x00;
    Status = AlAxiQspi_FlashIORead(AlAxiQspiHandle, RecvData, 0x20000, QSPI_FLASH_PAGE_SIZE);
    if (Status != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Flash read error: 0x%x\r\n", Status);
        return Status;
    }
    for (Index = 0; Index < TestSize; Index++) {
        if (RecvData[Index] != SendData[Index]) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Data error after write date at Index %d\r\n", Index);
            while (1);
        }
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "Axi-qspi flash test success.\r\n");

    return AL_OK;
}

static AL_VOID GP_Port_Enable(AL_VOID)
{
    AL_U32 val = 0;

    /* Enable gp normal access */
    val = AL_REG32_READ(TOP_NS_BASE_ADDR + TOP_NS_PLS_PROT_OFFSET);
    val &= ~0x3;
    AL_REG32_WRITE(TOP_NS_BASE_ADDR + TOP_NS_PLS_PROT_OFFSET, val);

    /* Soft reset hp0, hp1, gp0, gp1 */
    val = 0;
    val = AL_REG32_READ(TOP_CRP_BASE_ADDR + TOP_CRP_SRST_CTRL2_OFFSET);
    AL_REG32_WRITE(TOP_CRP_BASE_ADDR + TOP_CRP_SRST_CTRL2_OFFSET, val & (~0x33));
    AlSys_MDelay(1);
    AL_REG32_WRITE(TOP_CRP_BASE_ADDR + TOP_CRP_SRST_CTRL2_OFFSET, val | 0x33);
}

static AL_U32 AlAxiQspi_FlashReadId(AlAxiQspi_HalStruct *AlAxiQspiHandle, AL_U8 *Id, AL_U32 IdSize)
{
    AL_U8 TempCmd = NOR_OP_RDID;
    return AlAxiQspi_Hal_TransferDataPolledBlock(AlAxiQspiHandle, &TempCmd, 1, Id, IdSize, TIMEOUT_FOR_TEST);
}

static AL_U32 AlAxiQspi_FlashWren(AlAxiQspi_HalStruct *AlAxiQspiHandle)
{
    AL_U8 TempCmd = NOR_OP_WREN;
    return AlAxiQspi_Hal_TransferDataPolledBlock(AlAxiQspiHandle, &TempCmd, 1, AL_NULL, 0, TIMEOUT_FOR_TEST);
}

static AL_U32 AlAxiQspi_FlashEraseChip(AlAxiQspi_HalStruct *AlAxiQspiHandle)
{
    AL_U8 TempCmd = NOR_OP_CE;
    AL_U32 Status = AL_OK;

    Status = AlAxiQspi_FlashWren(AlAxiQspiHandle);
    if (Status != AL_OK) {
        return Status;
    }

    Status = AlAxiQspi_Hal_TransferDataBlock(AlAxiQspiHandle, &TempCmd, 1, AL_NULL, 0, TIMEOUT_FOR_TEST);
    if (Status != AL_OK) {
        return Status;
    }

    Status = AlAxiQspi_FlashWaitWip(AlAxiQspiHandle, 10);
    if (Status != AL_OK) {
        return Status;
    }

    return AL_OK;
}

static AL_U32 AlAxiQspi_FlashEraseSector(AlAxiQspi_HalStruct *AlAxiQspiHandle, AL_U32 Addr)
{
    AL_U8 TempArray[4] = {0};
    AL_U32 Status = AL_OK;

    Status = AlAxiQspi_FlashWren(AlAxiQspiHandle);
    if (Status != AL_OK) {
        return Status;
    }

    TempArray[0] = NOR_OP_SE;
    TempArray[1] = (Addr >> 16) & 0xFF;
    TempArray[2] = (Addr >> 8) & 0xFF;
    TempArray[3] = Addr & 0xFF;
    Status = AlAxiQspi_Hal_TransferDataPolledBlock(AlAxiQspiHandle, TempArray, 4, AL_NULL, 0, TIMEOUT_FOR_TEST);
    if (Status != AL_OK) {
        return Status;
    }

    Status = AlAxiQspi_FlashWaitWip(AlAxiQspiHandle, 1);
    if (Status != AL_OK) {
        return Status;
    }

    return AL_OK;
}

static AL_U32 AlAxiQspi_FlashReadStatusReg(AlAxiQspi_HalStruct *AlAxiQspiHandle, AL_U8 *StatusReg)
{
    AL_U8 TempCmd = NOR_OP_RDSR;
    return AlAxiQspi_Hal_TransferDataPolledBlock(AlAxiQspiHandle, &TempCmd, 1, StatusReg, 1, TIMEOUT_FOR_TEST);
}

static AL_U32 AlAxiQspi_FlashWaitWip(AlAxiQspi_HalStruct *AlAxiQspiHandle, AL_U32 Msleep)
{
    AL_U32 Status = AL_OK;
    AL_U8 TempCmd = NOR_OP_RDSR;
    AL_U8 StatusReg = 0;

    StatusReg = SR_WIP;
    while (StatusReg & SR_WIP) {
        Status = AlAxiQspi_Hal_TransferDataPolledBlock(AlAxiQspiHandle, &TempCmd, 1, &StatusReg, 1, TIMEOUT_FOR_TEST);
        if (Status != AL_OK) {
            return Status;
        }

        if (Msleep != 0) {
            AlOsal_Sleep(Msleep);
        }
    }

    return AL_OK;
}

static AL_U32 AlAxiQspi_FlashOutputRead(AlAxiQspi_HalStruct *AlAxiQspiHandle, AL_U8 *Data, AL_U32 Addr, AL_U32 Size)
{
    AL_U8 TempArray[6] = {0};
    TempArray[0] = NOR_OP_READ_1_1_2;
    TempArray[1] = (Addr >> 16) & 0xFF;
    TempArray[2] = (Addr >> 8) & 0xFF;
    TempArray[3] = Addr & 0xFF;
    return AlAxiQspi_Hal_TransferDataPolledBlock(AlAxiQspiHandle, TempArray, 6, Data, Size, TIMEOUT_FOR_TEST);
}

static AL_U32 AlAxiQspi_FlashIORead(AlAxiQspi_HalStruct *AlAxiQspiHandle, AL_U8 *Data, AL_U32 Addr, AL_U32 Size)
{
    AL_U8 TempArray[5] = {0};
    TempArray[0] = NOR_OP_READ_1_2_2;
    TempArray[1] = (Addr >> 16) & 0xFF;
    TempArray[2] = (Addr >> 8) & 0xFF;
    TempArray[3] = Addr & 0xFF;
    return AlAxiQspi_Hal_TransferDataPolledBlock(AlAxiQspiHandle, TempArray, 5, Data, Size, TIMEOUT_FOR_TEST);
}

static AL_U32 AlAxiQspi_FlashWritePage(AlAxiQspi_HalStruct *AlAxiQspiHandle, AL_U8 *Data, AL_U32 Addr, AL_U32 Size)
{
    AL_U8 TempArray[QSPI_FLASH_PAGE_SIZE + 4] = {0};
    AL_U32 Index = 0;

    TempArray[0] = NOR_OP_PP;
    TempArray[1] = (Addr >> 16) & 0xFF;
    TempArray[2] = (Addr >> 8) & 0xFF;
    TempArray[3] = Addr & 0xFF;
    for (Index = 0; Index < Size; Index++) {
        TempArray[Index + 4] = Data[Index];
    }
    return AlAxiQspi_Hal_TransferDataPolledBlock(AlAxiQspiHandle, TempArray, Size + 4, AL_NULL, 0, TIMEOUT_FOR_TEST);
}

static AL_U32 AlAxiQspi_FlashWrite(AlAxiQspi_HalStruct *AlAxiQspiHandle, AL_U8 *Data, AL_U32 Addr, AL_U32 Size)
{
    AL_U32 Status = 0;
    AL_U32 SendCnt = 0;
    AL_U32 CurSendSize = 0;

    while (SendCnt < Size) {
        CurSendSize = (Size - SendCnt) > QSPI_FLASH_PAGE_SIZE ? QSPI_FLASH_PAGE_SIZE : (Size - SendCnt);

        Status = AlAxiQspi_FlashWren(AlAxiQspiHandle);
        if (Status != AL_OK) {
            return Status;
        }

        Status = AlAxiQspi_FlashWritePage(AlAxiQspiHandle, &Data[SendCnt], Addr + SendCnt, CurSendSize);
        if (Status != AL_OK) {
            return Status;
        }

        Status = AlAxiQspi_FlashWaitWip(AlAxiQspiHandle, 0);
        if (Status != AL_OK) {
            return Status;
        }

        SendCnt += CurSendSize;
    }

    return AL_OK;
}


int main(void) {
    RUN_TEST(AlAxiQspi_X2_Polled_3Byte_example);

    return 0;
}