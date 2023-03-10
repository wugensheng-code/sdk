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
