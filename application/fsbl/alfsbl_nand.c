/*
 * alfsbl_emmc.c
 *
 *  Created on: Sept 10, 2022
 *      Author: hj
 */

#include <stdio.h>

#include "demosoc.h"

#include "alfsbl_nand.h"
#include "alfsbl_misc.h"

#include "./../../SoC/demosoc/Common/Include/nand_drv.h"
#include "./../../SoC/demosoc/Common/Include/smc_drv.h"



Nand_TypeDef nand;

uint32_t AlFsbl_NandInit(void)
{
    uint8_t Status;

    Status = Csu_NandInit(&nand);

    return Status;
}


uint32_t AlFsbl_NandCopy(uint32_t SrcAddress, PTRSIZE DestAddress, uint32_t Length)
{
    uint8_t Status;

    printf("AlFsbl_NandCopy...\r\n");
	Status = Csu_NandRead(SrcAddress - IMAGE_FLASH_OFFSET, (uint8_t *)(DestAddress), Length,  &nand);

    return Status;
}


uint32_t AlFsbl_NandRelease(void)
{

	return 0;
}
