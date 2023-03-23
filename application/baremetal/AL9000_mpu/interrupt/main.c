//uart_write
#include <stdio.h>
#include <stdlib.h>
#include "nuclei_sdk_soc.h"
#include "al9000_mpu.h"
#include <time.h>
#include "al_mpu.h"

extern int DmaAxi(void);
extern int main_npujpu();

static volatile char InterruptFlag = 0;

void DisplayMpuInterruptInfo()
{
    char InterruptId = -1;

    InterruptId = *((unsigned int *)INTR_MPU_STATE);
    unsigned int *MpuRegion1;

    printf("Mpu interrupt id is 0x%x, ", InterruptId);
    switch (InterruptId)
    {
        case MPU_DDRS0:
            MpuRegion1 = (unsigned int *)(MpuDdrs0 + MPU_RASR_REGION_R);
            SET_BIT(*MpuRegion1, INTERRUPT_CLR_BIT);
            printf("This is DDRS0 mpu interrupt.\r\n");
            break;

        case MPU_DDRS1:
            MpuRegion1 = (unsigned int *)(MpuDdrs1 + MPU_RASR_REGION_R);
            SET_BIT(*MpuRegion1, INTERRUPT_CLR_BIT);
            printf("This is DDRS1 mpu interrupt.\r\n");
            break;

        case MPU_HPM0:
            MpuRegion1 = (unsigned int *)(MpuHpm0 + MPU_RASR_REGION_R);
            SET_BIT(*MpuRegion1, INTERRUPT_CLR_BIT);
            printf("This is HPM0 mpu interrupt.\r\n");
            break;

        case MPU_HPM1:
            MpuRegion1 = (unsigned int *)(MpuHpm1 + MPU_RASR_REGION_R);
            SET_BIT(*MpuRegion1, INTERRUPT_CLR_BIT);
            printf("This is HPM1 mpu interrupt.\r\n");
            break;

        case MPU_NPU:
            MpuRegion1 = (unsigned int *)(MpuNpu + MPU_RASR_REGION_R);
            SET_BIT(*MpuRegion1, INTERRUPT_CLR_BIT);
            printf("This is NPU mpu interrupt.\r\n");
            break;

        case MPU_OCMS2:
            MpuRegion1 = (unsigned int *)(MpuOcms2 + MPU_RASR_REGION_R);
            SET_BIT(*MpuRegion1, INTERRUPT_CLR_BIT);
            printf("This is OCMS2 mpu interrupt.\r\n");
            break;

        case MPU_APU:
            MpuRegion1 = (unsigned int *)(MpuApu + MPU_RASR_REGION_R);
            SET_BIT(*MpuRegion1, INTERRUPT_CLR_BIT);
            printf("This is APU mpu interrupt.\r\n");
            break;

        default:
            printf("This is no defined interrupt.\r\n");
            break;
    }

    InterruptFlag = 1;

}

/***************************************************************************/
/**
 * @brief    interrupt function for MPU Host Controller
 *
 * @param  None
 * @return None
 *
 ******************************************************************************/
void Mpu_IRQn_Handler(void)
{
    printf("This is MPU interrupt!!!\r\n");
    DisplayMpuInterruptInfo();
    printf("\r\n");
}

void Ddrs0InterruptTest()
{
    RegionAttr Attr;

    AlMpu *InstancePtr = (AlMpu *)MpuDdrs0;
    AlMpu_Disable(InstancePtr);
    Attr.StartAddr = 0x10;
    Attr.EndAddr = 0x20;
    Attr.Privilege = PRIVILEGE_UNPROTECTED;
    Attr.Secure = NON_SECURE;
    Attr.GroupId = APU_GROUPID_NUM;
    Attr.ReadWrite = NOREADWRITE;
    Attr.RegionEnable = REGION_ENABLED;
    Attr.InterruptEnable = INTERRUPT_ENABLED;

    AlMpu_Disable(InstancePtr);
    AlMpu_SetRegionAttr(InstancePtr, 1, Attr);
    AlMpu_Enable(InstancePtr);

    RegWrite(0x10000, 0x99999);
}

void Ddrs1InterruptTest()
{
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
    Attr.InterruptEnable = INTERRUPT_ENABLED;

    AlMpu_Disable(InstancePtr);
    AlMpu_SetRegionAttr(InstancePtr, 1, Attr);
    AlMpu_Enable(InstancePtr);
}

void Hpm0InterruptTest()
{
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
    Attr.InterruptEnable = INTERRUPT_ENABLED;

    AlMpu_Disable(InstancePtr);
    AlMpu_SetRegionAttr(InstancePtr, 1, Attr);
    AlMpu_Enable(InstancePtr);
}

void Hpm1InterruptTest()
{
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
    Attr.InterruptEnable = INTERRUPT_ENABLED;

    AlMpu_Disable(InstancePtr);
    AlMpu_SetRegionAttr(InstancePtr, 1, Attr);
    AlMpu_Enable(InstancePtr);
}

void NpuInterruptTest()
{
    RegionAttr Attr;

    AlMpu *InstancePtr = (AlMpu *)MpuNpu;
    AlMpu_Disable(InstancePtr);
    Attr.StartAddr = 0x63e00;
    Attr.EndAddr = 0x63e01;
    Attr.Privilege = PRIVILEGE_UNPROTECTED;
    Attr.Secure = NON_SECURE;
    Attr.GroupId = APU_GROUPID_NUM;
    Attr.ReadWrite = NOREADWRITE;
    Attr.RegionEnable = REGION_ENABLED;
    Attr.InterruptEnable = INTERRUPT_ENABLED;

    AlMpu_Disable(InstancePtr);
    AlMpu_SetRegionAttr(InstancePtr, 1, Attr);
    AlMpu_Enable(InstancePtr);

    RegWrite(0x63e00000, 0x99999);
}

void Ocms2InterruptTest()
{
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
    Attr.InterruptEnable = INTERRUPT_ENABLED;

    AlMpu_Disable(InstancePtr);
    AlMpu_SetRegionAttr(InstancePtr, 1, Attr);
    AlMpu_Enable(InstancePtr);

     RegWrite(0x61007000, 0x99999);
}

void ApuInterruptTest()
{
    uint32_t MPU_Status = 0;
    RegionAttr Attr;

    AlMpu *InstancePtr = (AlMpu *)MpuApu;
    AlMpu_Disable(InstancePtr);
    Attr.StartAddr = MPU_DMA_AXI_START_ADDR;
    Attr.EndAddr = MPU_DMA_AXI_END_ADDR;
    Attr.Privilege = PRIVILEGE_UNPROTECTED;
    Attr.Secure = NON_SECURE;
    Attr.GroupId = APU_GROUPID_NUM;
    Attr.ReadWrite = NOREADWRITE;
    Attr.RegionEnable = REGION_ENABLED;
    Attr.InterruptEnable = INTERRUPT_ENABLED;

    AlMpu_Disable(InstancePtr);
    AlMpu_SetRegionAttr(InstancePtr, 1, Attr);
    AlMpu_Enable(InstancePtr);

    RegWrite(0x61007000, 0x99999);
}

void AutoInterruptTest()
{
    InterruptFlag = 0;
    Ddrs0InterruptTest();
    while(!InterruptFlag);

    InterruptFlag = 0;
    NpuInterruptTest();
    while(!InterruptFlag);

    InterruptFlag = 0;
    Ocms2InterruptTest();
    while(!InterruptFlag);

    InterruptFlag = 0;
    ApuInterruptTest();
    while(!InterruptFlag);
}

void ManualInterruptTest(void)
{
    *((unsigned int *)0xf8801078) = 0x133;
    *((unsigned int *)0xf8801074) = 0x73f0;
    *((unsigned int *)0xf840e004) = 0x0;
    *((unsigned int *)0xf840f004) = 0x0;

    InterruptFlag = 0;
    Ddrs1InterruptTest();
    while(!InterruptFlag);

    InterruptFlag = 0;
    Hpm0InterruptTest();
    while(!InterruptFlag);

    InterruptFlag = 0;
    Hpm1InterruptTest();
    while(!InterruptFlag);
}

/**
 * @brief Function is used to test interrupt
 */
void InterruptTest()
{
    AutoInterruptTest();

    /* Manual testing required */
#ifdef DDRS0_HP_INTERRUPT_TEST
    ManualInterruptTest();
#endif /* DDRS0_HP_INTERRUPT_TEST */
}

uint32_t main(void)
{
    ECLIC_Register_IRQ(SOC_INT130_IRQn, ECLIC_NON_VECTOR_INTERRUPT, ECLIC_LEVEL_TRIGGER, 1, 1, Mpu_IRQn_Handler);
    __enable_irq();
    InterruptTest();

    printf("[AUTOTEST]:[MPU]:[INTERRUPTTEST]:[PASS]]\r\n");

    return 0;
}
