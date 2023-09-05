/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>

#include "alfsbl_nand.h"
#include "alfsbl_misc.h"
#include "alfsbl_boot.h"

#include "nand_drv.h"
#include "smc_drv.h"



Nand_TypeDef nand;

uint32_t AlFsbl_NandInit(void)
{
    uint8_t Status;

    Status = Csu_NandInit(&nand);

    if(Status != 0) {
    	Status = Status | (ALFSBL_BOOTMODE_NAND << 16);
    }
    return Status;
}


uint32_t AlFsbl_NandCopy(uint64_t SrcAddress, PTRSIZE DestAddress, uint32_t Length, SecureInfo *pSecureInfo)
{
    uint8_t Status;

    printf("AlFsbl_NandCopy...\r\n");
	Status = Csu_NandRead(SrcAddress - IMAGE_FLASH_OFFSET, (uint8_t *)(DestAddress), Length,  &nand);

	if(Status != 0) {
		Status = Status | (ALFSBL_BOOTMODE_NAND << 16);
	}
    return Status;
}


uint32_t AlFsbl_NandRelease(void)
{
	return 0;
}
