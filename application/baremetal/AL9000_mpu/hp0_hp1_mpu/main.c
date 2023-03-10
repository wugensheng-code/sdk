//uart_write
#include <stdio.h>
#include <stdlib.h>
#include "nuclei_sdk_soc.h"
#include "al9000_mpu.h"
#include <time.h>
#include "../al_mpu.h"

extern int DmaAxi(void);
extern int main_npujpu();

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
