//uart_write
#include <stdio.h>
#include <stdlib.h>
#include "nuclei_sdk_soc.h"
#include "al9000_mpu.h"
#include <time.h>
#include "al_mpu.h"

extern int axidma(void);
#define OCM

void SimulationWave()
{
    #ifdef OCM
    for (int i = 0; i < 2; i++)
    {
        //GroupIdTest_AllTest();
        *((volatile uint32_t *)(0xF841B004)) = 0x0;
        *((volatile uint32_t *)(0xF841B010)) = 0x81;
        *((volatile uint32_t *)(0xF841B014)) = 0x61001; 
        *((volatile uint32_t *)(0xF841B018)) = 0x61002;
        *((volatile uint32_t *)(0xF841B01c)) = 0x1;
        *((volatile uint32_t *)(0xF841B004)) = 0x1;

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
        *((uint32_t *)(0xF840e004)) = 0x0;
        *((uint32_t *)(0xF840e010)) = 0x81;
        *((uint32_t *)(0xF840e014)) = 1; 
        *((uint32_t *)(0xF840e018)) = 2;
        *((uint32_t *)(0xF840e01c)) = 0x1;
        *((uint32_t *)(0xF840e004)) = 0x1;
        //asm volatile("dsb     sy");

        *((uint32_t *)(0x1034)) = 0x9999999; 
        //int value = *((uint32_t *)(0x1034));
    }
    *((uint32_t *)(0xF840e004)) = 0x0;
    //asm volatile("dsb     sy");

    *((uint32_t *)(0x1034)) = 0x88888888; 
    int value = *((uint32_t *)(0x1034));

    #endif
    while(1)
    ;
}

int main()
{
    //main_npujpu();
    //axidma();
    /* release memory */
    //*(uint32_t *)(0xf8801074) |= 0x4000; 
    //AlMpu_AllTest();
    //ApuMpuTest();
    //OcmMpuTest();
    //Ddrs0MpuTest();
    //Ddr1MpuTest();
    //NpuMpuTest();
    //Hp0MpuTest();
    //Hp1MpuTest();
    //SimulationWave();
    while(1)
        ;
    return 0;
}
