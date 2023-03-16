//uart_write
#include <stdio.h>
#include <stdlib.h>
#include "nuclei_sdk_soc.h"
#include "al9000_mpu.h"
#include <time.h>
#include "../al_mpu.h"

extern int DmaAxi(void);
extern int main_npujpu();

/**
 * @desc  : do_bad_sync handles the impossible case in the Synchronous Abort vector,
 * 		     you must re-implement event handle.
 * @param {pt_regs} *pt_regs
 * @param {unsigned int} esr
 * @return {*}
 */

void do_sync_handle(struct pt_regs *pt_regs, unsigned int esr)
{
	asm volatile("mrs x25, elr_el1; add x25, x25, #0x04; msr elr_el1, x25" ::: "x25");
}

/**
 * @brief Function is used to check all mpu of system
 */
uint32_t ApuMpuTest(void)
{
    uint32_t MPU_Status = 0;

    RegionAttr Attr;
    memset(&Attr, 0, sizeof(Attr));
    Attr.EndAddr = 0x61010;

    AlMpu *InstancePtr = (AlMpu *)MpuApu;
    MPU_Status = SetAttrAndCompare(InstancePtr, Attr);

    return MPU_Status;
}

uint32_t main(void)
{
    uint32_t Status = MPU_SUCCESS;

    Status = ApuMpuTest();
    if (Status != MPU_SUCCESS)
    {
        printf("[AUTOTEST]:[MPU]:[APU]:[FAIL]]\r\n");
    }
    else
    {
        printf("[AUTOTEST]:[MPU]:[APU]:[PASS]]\r\n");
    }

    return Status;
}
