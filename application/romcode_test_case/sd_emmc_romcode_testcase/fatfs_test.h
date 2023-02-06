#ifndef _AL_FATFS_TEST_H_
#define _AL_FATFS_TEST_H_

#include <string.h>
#include <stdio.h>
#include "al_mmc.h"
#include "FATFS/ff.h"
#include "al_sd_write.h"

uint32_t Emmc_FatfsBoundaryCoverageTest(void);
uint32_t Sd_FatfsBoundaryCoverageTest(void);

#endif
