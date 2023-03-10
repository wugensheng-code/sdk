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
uint32_t Ddrs0MpuTest(void) 
{
    uint32_t MPU_Status = 0;
    uint32_t MPU_Counter = 0;
    uint32_t value = 0;
    uint32_t region = 0;

    RegionAttr Attr;
    memset(&Attr, 0, sizeof(Attr));

    AlMpu *InstancePtr = (AlMpu *)MpuDdrs0;
    MPU_Status = SetAttrAndCompare(InstancePtr, Attr);

    return MPU_Status;
}

uint32_t main(void)
{
    uint32_t Status = MPU_SUCCESS;

    Status = Ddrs0MpuTest();
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
