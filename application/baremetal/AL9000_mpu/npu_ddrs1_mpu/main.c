//uart_write
#include <stdio.h>
#include <stdlib.h>
#include "nuclei_sdk_soc.h"
#include "al9000_mpu.h"
#include <time.h>
#include "../al_mpu.h"

/*
  For dma axi test, note that this address should be outside
  the address where the code is running if it is running ocm.
  And this address is 32 byte aligned for dma axi test.
 */
volatile unsigned int *Src = (unsigned int *)(MPU_DMA_AXI_START_ADDR << 12);
volatile unsigned int *Dst = (unsigned int *)(MPU_DMA_AXI_START_ADDR << 12) + 0x400;

uint32_t NpuMpuTest(void);
unsigned int RegRead(uint32_t reg_address);
void RegWrite(uint32_t reg_address, uint32_t reg_wdata);
uint32_t Gp0Test(void);
uint32_t Gp1Test(void);
uint32_t DmaAxiTest(void);
uint32_t OcmMpuTest(void);


extern int DmaAxi(void);
extern int main_npujpu();

/**
 * @brief Function is used to check all mpu of system
 */
uint32_t NpuJpuTest(void)
{
    uint32_t MPU_Status = 0;
    uint32_t RegionStartAddr = 0x500;
    uint32_t RegionEndAddr = 0x503;

    RegionAttr Attr;
    AlMpu *InstancePtr = (AlMpu *)MpuDdrs1;
    AlMpu_Disable(InstancePtr);
    Attr.StartAddr = RegionStartAddr;
    Attr.EndAddr = RegionEndAddr;
    Attr.Privilege = PRIVILEGE_UNPROTECTED;
    Attr.Secure = NON_SECURE;
    Attr.GroupId = 0x18;
    Attr.ReadWrite = NOREADWRITE;
    Attr.RegionEnable = REGION_ENABLED;


    AlMpu_Disable(InstancePtr);
    AlMpu_SetRegionAttr(InstancePtr, 1, Attr);
    AlMpu_Enable(InstancePtr);
    MPU_Status = main_npujpu();

    return MPU_Status;
}

/**
 * @brief Function is used to check all mpu of system
 */
uint32_t Ddrs1MpuTest(void)
{
    uint32_t MPU_Status = 0;

#if 0
    MPU_Status = NpuJpuTest();
    MPU_Status = Gp0Test();
    MPU_Status = Gp1Test();
#endif

    /* With secure dma, only secure mode is supported */
    MPU_Status = DmaAxiTest();

    return MPU_Status;
}

/**
 * @brief Function is used to check all mpu of system
 */
uint32_t NpuMpuTest(void)
{
    uint32_t MPU_Status = 0;
    uint32_t RegionStartAddr = 0x500;
    uint32_t RegionEndAddr = 0x503;
    uint32_t RegionGroupId = 0x18;

    RegionAttr Attr;
    AlMpu *InstancePtr = (AlMpu *)MpuNpu;
    AlMpu_Disable(InstancePtr);
    Attr.StartAddr = RegionStartAddr;
    Attr.EndAddr = RegionEndAddr;
    Attr.Privilege = PRIVILEGE_UNPROTECTED;
    Attr.Secure = NON_SECURE;
    Attr.GroupId = 0x18;
    Attr.ReadWrite = NOREADWRITE;
    Attr.RegionEnable = REGION_ENABLED;
    for (int i = 0; i < 8; i++)
    {
        AlMpu_Disable(InstancePtr);
        AlMpu_SetRegionAttr(InstancePtr, i, Attr);
        AlMpu_Enable(InstancePtr);
        MPU_Status = main_npujpu();
    }

    main_npujpu();

    return MPU_Status;
}








/**
 * @brief Function is used to check all mpu of system
 */
uint32_t Gp0Test(void)
{
    uint32_t MPU_Status = 0;

    RegionAttr Attr;
    AlMpu *InstancePtr = (AlMpu *)MpuDdrs1;
    AlMpu_Disable(InstancePtr);
    Attr.StartAddr = MPU_DDR_START_ADDR;
    Attr.EndAddr = MPU_DDR_END_ADDR;
    Attr.Privilege = PRIVILEGE_UNPROTECTED;
    Attr.Secure = NON_SECURE;
    Attr.GroupId = GP0_GROUPID_NUM;
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
uint32_t Gp1Test(void)
{
    uint32_t MPU_Status = 0;

    RegionAttr Attr;
    AlMpu *InstancePtr = (AlMpu *)MpuDdrs1;
    AlMpu_Disable(InstancePtr);
    Attr.StartAddr = MPU_DDR_START_ADDR;
    Attr.EndAddr = MPU_DDR_END_ADDR;
    Attr.Privilege = PRIVILEGE_UNPROTECTED;
    Attr.Secure = NON_SECURE;
    Attr.GroupId = GP1_GROUPID_NUM;
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
uint32_t DmaAxiTest(void)
{
    int ret;
    uint32_t MPU_Status = 0;

    RegionAttr Attr;
    AlMpu *InstancePtr = (AlMpu *)MpuOcms2;
    AlMpu_Disable(InstancePtr);
    Attr.StartAddr = MPU_DMA_AXI_START_ADDR;
    Attr.EndAddr = MPU_DMA_AXI_END_ADDR;
    Attr.Privilege = PRIVILEGE_UNPROTECTED;
    Attr.Secure = NON_SECURE;
    Attr.GroupId = DMA_AXI_GROUPID_NUM;
    Attr.ReadWrite = NOREADWRITE;
    Attr.RegionEnable = REGION_ENABLED;

    AlMpu_Disable(InstancePtr);
    AlMpu_SetRegionAttr(InstancePtr, 1, Attr);
    AlMpu_Enable(InstancePtr);

    /*
      A return value of 1 indicates that the dma failed to transfer the data,
      that the mpu protected the data properly, and that the test was successful,
      whereas the test failed.
     */
    ret = DmaAxi();
    if (ret == 1)
        MPU_Status = MPU_SUCCESS;
    else
        MPU_Status = MPU_FAILURE;

    return MPU_Status;
}

uint32_t main(void)
{
    uint32_t Status = MPU_SUCCESS;

    Status = Ddrs1MpuTest();
    if (Status != MPU_SUCCESS)
    {
        printf("[AUTOTEST]:[MPU]:[APU]:[FAIL]]\r\n");
    }
    else
    {
        printf("[AUTOTEST]:[MPU]:[APU]:[PASS]]\r\n");
    }

    Status = NpuMpuTest();
    if (Status != MPU_SUCCESS)
    {
        printf("[AUTOTEST]:[MPU]:[NPU]:[FAIL]]\r\n");
    }
    else
    {
        printf("[AUTOTEST]:[MPU]:[NPU]:[PASS]]\r\n");
    }

    return Status;
}
