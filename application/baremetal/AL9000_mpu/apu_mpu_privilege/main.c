//uart_write
#include <stdio.h>
#include <stdlib.h>
#include "nuclei_sdk_soc.h"
#include "al9000_mpu.h"
#include <time.h>
#include "../al_mpu.h"

/**
 * @brief Function is used to check all mpu of system
 */
uint32_t ApuMpu_PriviledgeTest(void)
{
    uint32_t MPU_Status = 0;
	volatile int loop = 0;

    RegionAttr Attr = {
		.StartAddr		= 0x61010,
		.EndAddr		= 0x61012,
		.Privilege		= 0x1,
		.RegionEnable	= 0x1
	};

	AlMpu_SetRegionAttr((AlMpu *)MpuApu, 0, Attr);

	while (1)
	{
		printf("loop = %d\r\n", loop++);

		/*
	 	* readme: write this register will casue abort
		* and EL1 cpu does not initilize,
		* so if run to abort handler, this case pass;
		*/

		*(unsigned int *)(Attr.StartAddr * 0x1000 + 4) = 0x87654321;
	}

	return MPU_FAILURE;
}


uint32_t main(void)
{
	int Status = ApuMpu_PriviledgeTest();

	if (Status != MPU_SUCCESS) {
		printf("[AUTOTEST]:[MPU]:[APU]:[FAIL]]\r\n");
	}
	else {
		printf("[AUTOTEST]:[MPU]:[APU]:[PASS]]\r\n");
	}

    return Status;
}
