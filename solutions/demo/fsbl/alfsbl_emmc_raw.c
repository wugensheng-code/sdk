/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>

#include "alfsbl_sd.h"
#include "alfsbl_misc.h"
#include "alfsbl_boot.h"

#include "al_mmc_hal.h"

#define MMC_RD_WR_TIMEOUT_MS    (10000)
#define BLOCK_SIZE              (0x200)

static AL_MMC_HalStruct Handle = {0};
static DevId = 0;
static AL_MMC_InitStruct InitConfig = {
    .CardType           = AL_MMC_CARD_TYPE_AUTO_DETECT,
    .DmaMode            = AL_MMC_DMA_MODE_SDMA,
    .FreqKhz            = AL_MMC_FREQ_KHZ_25000,
    .SpdMode            = AL_MMC_SPD_DS_SDR12,
    .BusWidth           = AL_MMC_BUS_WIDTH_4BIT,
    .Switch1v8          = AL_FUNC_DISABLE,
    .AutoGenAdmaTblEn   = AL_FUNC_DISABLE,
    .DmaBdary           = AL_MMC_BDARY_32K
};
static AL_U8 SharedBuffer[BLOCK_SIZE];

uint32_t AlFsbl_EmmcRawInit(void)
{
	uint32_t status = AL_OK;
	status = AlMmc_Hal_Init(&Handle, DevId, &InitConfig, AL_NULL);

	if(status != AL_OK) {
		status = status | (ALFSBL_BOOTMODE_EMMC << 16);
	}
	return status;
}

uint32_t AlFsbl_EmmcRawCopy(uint64_t SrcAddress, PTRSIZE DestAddress, uint32_t Length, SecureInfo *pSecureInfo)
{
	uint32_t status             = AL_OK;
    uint8_t *pdestaddr          = DestAddress;
	uint32_t Offset             = SrcAddress - IMAGE_FLASH_OFFSET;
    uint32_t blocksize          = BLOCK_SIZE;       //block size
    uint32_t startblock         = Offset / blocksize;
    uint32_t endpoint           = Offset + Length - 1;
    uint32_t endblock           = endpoint / blocksize;
    uint32_t firstblockoffset   = Offset % blocksize;
    uint32_t firstblockstore    = blocksize - firstblockoffset;
    uint32_t firstblockbytes    = (firstblockstore > Length)? Length : firstblockstore;
    uint32_t lastblockbytes     = endpoint % blocksize + 1;

    printf("offset = %d, Length = %d\r\n", Offset, Length);
    printf("startblock: %d\tfirstblockoffset: %d\tfirstblockbytes: %d\r\n", startblock, firstblockoffset, firstblockbytes);
    printf("endblock: %d\tlastblockbytes: %d\t\r\n", endblock, lastblockbytes);

    for (uint32_t i = startblock; i <= endblock; i++) {
        if (i == startblock) {
            status = AlMmc_Hal_ReadBlocked(&Handle, SharedBuffer, i, 1, MMC_RD_WR_TIMEOUT_MS);
            if (status != AL_OK) {
                goto END;
            }
            memcpy(pdestaddr, &SharedBuffer[firstblockoffset], firstblockbytes);
            pdestaddr += firstblockbytes;
        } else if (i == endblock) {
            status = AlMmc_Hal_ReadBlocked(&Handle, SharedBuffer, i, 1, MMC_RD_WR_TIMEOUT_MS);
            if (status != AL_OK) {
                goto END;
            }
            memcpy(pdestaddr, SharedBuffer, lastblockbytes);
            pdestaddr += lastblockbytes;
        } else {
            status = AlMmc_Hal_ReadBlocked(&Handle, pdestaddr, i, (endblock - i), MMC_RD_WR_TIMEOUT_MS);
            if (status != AL_OK) {
                goto END;
            }
            pdestaddr += blocksize * (endblock - i);
            i = (endblock - 1);
        }
    }

END:
    if(status != 0) {
		status = status | (ALFSBL_BOOTMODE_EMMC << 16);
	}

    return status;
}

uint32_t AlFsbl_EmmcRawRelease(void)
{
	return 0;
}
