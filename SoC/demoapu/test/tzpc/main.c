#include <stdio.h>
#include "type.h"

#define dmc_cfg    (0xF8420000U)
#define mpu_apu    (0xF841D000U)
#define mpu_npu    (0xF841C000U)
#define mpu_ocms2  (0xF841B000U)
#define mpu_hpm1   (0xF8413000U)
#define mpu_hpm0   (0xF8412000U)
#define mpu_ddrs1  (0xF840F000U)
#define mpu_ddrs0  (0xF840E000U)

uint32_t RegRead(uint32_t reg_address)
{
    return *((volatile uint32_t *)(reg_address));
}

void RegWrite(uint32_t reg_address, uint32_t reg_wdata)
{
    *((volatile uint32_t *)(reg_address)) = reg_wdata;
}

int main()
{
    uint32_t test_address[8] = {dmc_cfg, mpu_apu, mpu_npu, mpu_ocms2, mpu_hpm1, mpu_hpm0, mpu_ddrs1, mpu_ddrs0};

    char *test_name[8] = {"dmc_cfg_test","mpu_apu_test","mpu_npu_test","mpu_ocms2_test","mpu_hpm1_test","mpu_hpm0_test","mpu_ddrs1_test","mpu_ddrs0_test"};

    printf("\nDoing tzpc tests:\n");

    int i = 0;
    for (i = 0; i < 8; i++)
        {
            printf("%s started:\n", test_name[i]);
            RegRead(test_address[i]);
            RegWrite(test_address[i], 0x0);
            printf("%s ended.\n", test_name[i]);
	}

    printf("Finished doing tzpc tests\n");

    return 0;
}
