//uart_write
#include <stdio.h>
#include <stdlib.h>
#include "nuclei_sdk_soc.h"
#include "al9000_mpu.h"
#include <time.h>
#include "../al_mpu.h"

#define REG_WRITE(reg_address, reg_wdata)  *(unsigned int*)(reg_address) = reg_wdata
#define REG_READ(reg_address)  *(unsigned int*)reg_address

/**
 * @desc  : do_bad_sync handles the impossible case in the Synchronous Abort vector,
 * 		     you must re-implement event handle.
 * @param {pt_regs} *pt_regs
 * @param {unsigned int} esr
 * @return {*}
 */
#define vfwp printf
void do_sync_handle(struct pt_regs *pt_regs, unsigned int esr)
{
	vfwp("test pass \r\n");
}


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

	__enable_irq_abort();

	while (1)
	{
		vfwp("loop = %d\r\n", loop++);
		*(unsigned int *)(Attr.StartAddr * 0x1000 + 4) = 0x87654321;
	}

	return MPU_FAILURE;
}

/*
* For this case, cpu run at EL0;
* Abort need to be disable to avoid return to EL1
*/

uint32_t main(void)
{
	int Status = ApuMpu_PriviledgeTest();

	if (Status != MPU_SUCCESS) {
			vfwp("[AUTOTEST]:[MPU]:[APU]:[FAIL]]\r\n");
		#ifdef SIMULATIION
			FAIL();
		#endif
	}
	else {
		vfwp("[AUTOTEST]:[MPU]:[APU]:[PASS]]\r\n");
		#ifdef SIMULATIION
			SUCCESS();
		#endif
	}

    return Status;
}
