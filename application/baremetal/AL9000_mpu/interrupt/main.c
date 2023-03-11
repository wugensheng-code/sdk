//uart_write
#include <stdio.h>
#include <stdlib.h>
#include "nuclei_sdk_soc.h"
#include "al9000_mpu.h"
#include <time.h>
#include "al_mpu.h"

extern int DmaAxi(void);
extern int main_npujpu();


/**
 * @brief Function is used to check all mpu of system
 */
uint32_t InterruptTest(void) 
{
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
    Attr.InterruptEnable = INTERRUPT_ENABLED;

    AlMpu_Disable(InstancePtr);
    AlMpu_SetRegionAttr(InstancePtr, 1, Attr);
    AlMpu_Enable(InstancePtr);

    RegWrite(MPU_DMA_AXI_START_ADDR, 0x99999);

    return MPU_Status;
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
    printf("[AUTOTEST]:[MPU]:[INTERRUPTTEST]:[PASS]]\r\n");
}

uint32_t main(void)
{
    uint32_t Status = MPU_SUCCESS;

    ECLIC_Register_IRQ(SOC_INT130_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,Mpu_IRQn_Handler);
    __enable_irq();
    InterruptTest();

    return Status;
}
