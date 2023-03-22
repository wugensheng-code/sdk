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
uint32_t Hp0MpuTest(void)
{
    uint32_t MPU_Status = 0;

    RegionAttr Attr;
    AlMpu *InstancePtr = (AlMpu *)MpuHpm0;
    AlMpu_Disable(InstancePtr);
    Attr.StartAddr = MPU_DDR_START_ADDR;
    Attr.EndAddr = MPU_DDR_END_ADDR;
    Attr.Privilege = PRIVILEGE_UNPROTECTED;
    Attr.Secure = NON_SECURE;
    Attr.GroupId = HP0_GROUPID_NUM;
    Attr.ReadWrite = NOREADWRITE;
    Attr.RegionEnable = REGION_ENABLED;

    AlMpu_Disable(InstancePtr);
    AlMpu_SetRegionAttr(InstancePtr, 1, Attr);
    AlMpu_Enable(InstancePtr);

    return MPU_Status;
}

/**
 * @brief Function is used to check all mpu of system
 */
uint32_t Hp1MpuTest(void)
{
    uint32_t MPU_Status = 0;

    RegionAttr Attr;
    AlMpu *InstancePtr = (AlMpu *)MpuHpm1;
    AlMpu_Disable(InstancePtr);
    Attr.StartAddr = MPU_DDR_START_ADDR;
    Attr.EndAddr = MPU_DDR_END_ADDR;
    Attr.Privilege = PRIVILEGE_UNPROTECTED;
    Attr.Secure = NON_SECURE;
    Attr.GroupId = HP1_GROUPID_NUM;
    Attr.ReadWrite = NOREADWRITE;
    Attr.RegionEnable = REGION_ENABLED;

    AlMpu_Disable(InstancePtr);
    AlMpu_SetRegionAttr(InstancePtr, 1, Attr);
    AlMpu_Enable(InstancePtr);

    return MPU_Status;
}


uint32_t main(void)
{
    uint32_t Status = MPU_SUCCESS;

    Status = Hp0MpuTest();
    if (Status != MPU_SUCCESS)
    {
        printf("[AUTOTEST]:[MPU]:[HP0]:[FAIL]]\r\n");
    }
    else
    {
        printf("[AUTOTEST]:[MPU]:[HP0]:[PASS]]\r\n");
    }

    Status = Hp1MpuTest();
    if (Status != MPU_SUCCESS)
    {
        printf("[AUTOTEST]:[MPU]:[HP1]:[FAIL]]\r\n");
    }
    else
    {
        printf("[AUTOTEST]:[MPU]:[HP1]:[PASS]]\r\n");
    }

    return Status;
}
