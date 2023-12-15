/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_smc_hal.h"
#include "al_core.h"

#include "alfsbl_data.h"
#include "alfsbl_boot.h"

AL_SMC_HalStruct SmcHal;

AL_SMC_ConfigsStruct SmcInitConfigs =
{
    .Cycles.d32 = 0,        /* Cycles is set to 0 ignore this value and use the default */
    .SmcWidth = MW_8BITS
};

AL_U8 RecvData[500] = { 0x0 };

AL_U32 AlFsbl_NandInit(AL_VOID)
{
    AL_U32 Ret;

    Ret = AlSmc_Hal_Init(&SmcHal, &SmcInitConfigs, 0);
    if (AL_OK != Ret) {
         AL_LOG(AL_LOG_LEVEL_DEBUG, "Fsbl AlSmc_Hal_Init error\r\n");
    }

    return Ret;
}

AL_U32 AlFsbl_NandCopy(uint64_t SrcAddress, PTRSIZE DestAddress, uint32_t Length, SecureInfo *pSecureInfo)
{
    AL_U32 Ret;

    AL_LOG(AL_LOG_LEVEL_DEBUG, "AlFsbl_NandCopy...\r\n");
    Ret = AlSmc_Hal_ReadPage(&SmcHal, SrcAddress, (AL_U8 *)DestAddress, Length);
    if (AL_OK != Ret) {
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Fsbl AlSmc_Hal_ReadPage error\r\n");
        Ret = Ret | (ALFSBL_BOOTMODE_NAND << 16);
    }

    return Ret;
}


uint32_t AlFsbl_NandRelease(void)
{
	return 0;
}
