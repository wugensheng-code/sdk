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
void Mpu_IRQn_handler(void)
{

    printf("SOC_MPU_IRQn_handler\n\r");

}

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

int main()
{
    ECLIC_Register_IRQ(SOC_INT130_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,Mpu_IRQn_handler);
    __enable_irq();

    //AxiDmaTest();

   //main_npujpu();

    /* release memory */
    //*(uint32_t *)(0xf8801074) |= 0x4000; 
    //AlMpu_AllTest();
    //ApuMpuTest();
    //OcmMpuTest();
    //Ddrs0MpuTest();
    //Ddrs1MpuTest();
    //NpuMpuTest();
    //Hp0MpuTest();
    //Hp1MpuTest();
    SimulationWave();
    //hp0_mpu_simulation();
    while(1)
        ;
    return 0;
}
