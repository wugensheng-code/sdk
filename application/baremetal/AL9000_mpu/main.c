//uart_write
#include <stdio.h>
#include <stdlib.h>
#include "nuclei_sdk_soc.h"
#include "al9000_mpu.h"
#include <time.h>
#include "al_mpu.h"

extern int axidma(void);
extern int main_npujpu();
//#define OCM

/***************************************************************************/
/**
 * @brief    interrupt function for MPU Host Controller
 *
 * @param  None
 * @return None
 *
 ******************************************************************************/
#ifdef MPU_INTERRUPTTEST
void Mpu_IRQn_Handler(void)
{
    printf("SOC_MPU_IRQn_Handler\n\r");
}
#endif

void SimulationWave()
{

    #ifdef OCM
    for (int i = 0; i < 2; i++)
    {
        //GroupIdTest_AllTest();
        RegWrite((mpu_ocms2 + MPU_CTRL_R), 0x0);
        RegWrite((mpu_ocms2 + MPU_RASR_REGION_R), 0x81);
        RegWrite((mpu_ocms2 + MPU_SAR_REGION_R), 0x61001);
        RegWrite((mpu_ocms2 + MPU_EAR_REGION_R), 0x61002);
        RegWrite((mpu_ocms2 + MPU_GROUPID_REGION_R), 1);
        RegWrite((mpu_ocms2 + MPU_CTRL_R), 0x1);

        *((volatile uint32_t *)(0x61001038)) = 0x9999999; 
        int value = *((volatile uint32_t *)(0x61001038));
        while (1)
        {
            value = *((volatile uint32_t *)(0x61001038));
            int value1 = *((volatile uint32_t *)(0x0));
            printf("value = %08x\n", value);
            //printf("value1 = %08x\n", value1);
        }   
    }
    #else
    for (int i = 0; i < 30000; i++)
    {
        RegWrite((mpu_ddrs0 + MPU_CTRL_R), 0x0);
        RegWrite((mpu_ddrs0 + MPU_RASR_REGION_R), 0x301);
        RegWrite((mpu_ddrs0 + MPU_SAR_REGION_R), 1);
        RegWrite((mpu_ddrs0 + MPU_EAR_REGION_R), 2);
        RegWrite((mpu_ddrs0 + MPU_GROUPID_REGION_R), 1);
        
        asm volatile("dsb     sy");
        asm volatile("isb     sy");
        asm volatile("dmb     sy");

        *((uint32_t *)(0x1034)) = 0x9999999; 
        //int value = *((uint32_t *)(0x1034));
    }
    RegWrite((mpu_ddrs0 + MPU_CTRL_R), 0x1);
    //asm volatile("dsb     sy");

    *((uint32_t *)(0x1034)) = 0x88888888; 
    int value = *((uint32_t *)(0x1034));

    #endif
    #if 0
    //for (int i = 0; i < 1; i++)
    {
        *((uint32_t *)(0xF841d004)) = 0x0;
        *((uint32_t *)(0xF841d010)) = 0x1c1;
        *((uint32_t *)(0xF841d014)) = 1; 
        *((uint32_t *)(0xF841d018)) = 2;
        *((uint32_t *)(0xF841d01c)) = 0x20;
        
        //asm volatile("dsb     sy");

        *((uint32_t *)(0x1034)) = 0xaefee; 
        *((uint32_t *)(0xF841d004)) = 0x1;
        //int value = *((uint32_t *)(0x1034));
    }
    //*((uint32_t *)(0xF840e004)) = 0x0;
    //asm volatile("dsb     sy");

    //*((uint32_t *)(0x1034)) = 0x88888888; 
    int value = *((uint32_t *)(0x1034));
    printf("value1 = %08x\n", value);
#endif


    while(1)
    ;
}


void ddr_mpu_simulation()
{

}

void hp0_mpu_simulation()
{
    RegWrite((mpu_hpm0 + MPU_CTRL_R), 0x0);
    RegWrite((mpu_hpm0 + MPU_RASR_REGION_R), 0x1);
    RegWrite((mpu_hpm0 + MPU_SAR_REGION_R), 0x44a00);
    RegWrite((mpu_hpm0 + MPU_EAR_REGION_R), 0x44a02);
    RegWrite((mpu_hpm0 + MPU_GROUPID_REGION_R), 0x40);
    RegWrite((mpu_hpm0 + MPU_CTRL_R), 0x1);

    *((volatile uint32_t *)(0x44a00038)) = 0x9999999; 
    int value = *((volatile uint32_t *)(0x44a00038));
}

void hp1_mpu_simulation()
{
    RegWrite((mpu_hpm1 + MPU_CTRL_R), 0x0);
    RegWrite((mpu_hpm1 + MPU_RASR_REGION_R), 0x1);
    RegWrite((mpu_hpm1 + MPU_SAR_REGION_R), 0x44a00);
    RegWrite((mpu_hpm1 + MPU_EAR_REGION_R), 0x44a02);
    RegWrite((mpu_hpm1 + MPU_GROUPID_REGION_R), 0x80);
    RegWrite((mpu_hpm1 + MPU_CTRL_R), 0x1);

    *((volatile uint32_t *)(0x44a00038)) = 0x9999999; 
    int value = *((volatile uint32_t *)(0x44a00038));
}

void AxiDmaTest()
{
    RegWrite((mpu_ocms2 + MPU_CTRL_R), 0x0);
    RegWrite((mpu_ocms2 + MPU_RASR_REGION_R), 0x1);
    RegWrite((mpu_ocms2 + MPU_SAR_REGION_R), 0x61007);
    RegWrite((mpu_ocms2 + MPU_EAR_REGION_R), 0x61009);
    RegWrite((mpu_ocms2 + MPU_GROUPID_REGION_R), 0x20);
    RegWrite((mpu_ocms2 + MPU_CTRL_R), 0x1);

    axidma();
}

uint32_t main(void)
{
    uint32_t Status = MPU_SUCCESS;
#ifdef MPU_OCMTEST
    Status = OcmMpuTest();
    if (Status != MPU_SUCCESS)
    {
        printf("OcmMpuTest Failed: %d\n\r", Status);
    }
    else
    {
        printf("OcmMpuTest Test Success\n\r");
    }
#endif

#ifdef MPU_APUTEST
    Status = ApuMpuTest();
    if (Status != MPU_SUCCESS)
    {
        printf("ApuMpuTest Failed: %d\n\r", Status);
    }
    else
    {
        printf("ApuMpuTest Test Success\n\r");
    }
#endif

#ifdef MPU_DDRS0TEST
    Status = Ddrs0MpuTest();
    if (Status != MPU_SUCCESS)
    {
        printf("Ddrs0MpuTest Failed: %d\n\r", Status);
    }
    else
    {
        printf("Ddrs0MpuTest Test Success\n\r");
    }
#endif

#ifdef MPU_DDRS1TEST
    Status = Ddrs1MpuTest();
    if (Status != MPU_SUCCESS)
    {
        printf("Ddrs1MpuTest Failed: %d\n\r", Status);
    }
    else
    {
        printf("Ddrs1MpuTest Test Success\n\r");
    }
#endif

#ifdef MPU_NPUTEST
    Status = NpuMpuTest();
    if (Status != MPU_SUCCESS)
    {
        printf("NpuMpuTest Failed: %d\n\r", Status);
    }
    else
    {
        printf("NpuMpuTest Test Success\n\r");
    }
#endif

#ifdef MPU_HP0TEST
    Status = Hp0MpuTest();
    if (Status != MPU_SUCCESS)
    {
        printf("Hp0MpuTest Failed: %d\n\r", Status);
    }
    else
    {
        printf("Hp0MpuTest Test Success\n\r");
    }
#endif

#ifdef MPU_HP1TEST
    Status = Hp1MpuTest();
    if (Status != MPU_SUCCESS)
    {
        printf("Hp1MpuTest Failed: %d\n\r", Status);
    }
    else
    {
        printf("Hp1MpuTest Test Success\n\r");
    }
#endif

#ifdef MPU_SIMULATIONWAVE
    Status = SimulationWave();
    if (Status != MPU_SUCCESS)
    {
        printf("SimulationWave Failed: %d\n\r", Status);
    }
    else
    {
        printf("SimulationWave Test Success\n\r");
    }
#endif

#ifdef MPU_INTERRUPTTEST
    ECLIC_Register_IRQ(SOC_INT130_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,Mpu_IRQn_Handler);
    __enable_irq();
#endif

    return Status;
}
