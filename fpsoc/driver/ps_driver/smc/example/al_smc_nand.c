/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    al_smc_nand.c
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2023-09-01
 * @brief   smc nand example
 */

/***************************** Include Files *********************************/
#include "al_smc_hal.h"
#include "al_core.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/
#define TRANS_SIZE 5000

/************************** Variable Definitions *****************************/
AL_SMC_HalStruct SmcHal;

AL_SMC_ConfigsStruct SmcInitConfigs =
{
    .Cycles.d32 = 0,        /* Cycles is set to 0 ignore this value and use the default */
    .SmcWidth = MW_8BITS
};

AL_U8 SendData[TRANS_SIZE] = { 0x0 };
AL_U8 RecvData[TRANS_SIZE] = { 0x0 };

/************************** Function Prototypes ******************************/

/************************** Function Definitions ******************************/
/**
 *
 * This function serves as the starting point for the FPSoc SMC test. It initializes the SMC hardware
 * with predefined configurations, attempts to erase a block in the SMC device, and prepares a buffer
 * of data to be sent. It prints status messages to indicate the progress and result of these operations.
 *
 * @param None.
 *
 * @return int Returns 0 if the program executes successfully, or a non-zero error code if initialization
 *         or block erasure fails.
 *
 * @note This function demonstrates the basic usage of the SMC hardware interface, including initialization,
 *       block erasure, and data preparation for transmission. Error codes from SMC operations are printed
 *       to the standard output.
 *
 */
int main(void)
{
    printf("Start FPSoc Smc Test\r\n");
    AL_U32 i, Ret = AL_OK;

    Ret = AlSmc_Hal_Init(&SmcHal, &SmcInitConfigs, 0);
    if (AL_OK != Ret) {
         printf("AlSmc_Hal_Init error:0x%x\r\n", Ret);
    }

    Ret = AlSmc_Hal_EraseBlock(&SmcHal, 0);
    if (AL_OK != Ret) {
         printf("AlSmc_Hal_EraseBlock error:0x%x\r\n", Ret);
    }

    for (i = 0; i < sizeof(SendData); i++) {
        SendData[i] = i%256;
    }

    Ret = AlSmc_Hal_ReadPage(&SmcHal, 0, RecvData, 2048);
    if (AL_OK != Ret) {
         printf("AlSmc_Hal_ReadPage error:0x%x\r\n", Ret);
         while(1);
    }

    for (i = 0; i < 2048; i++) {
        if(0xff != RecvData[i]) {
            printf("AlSmc test erase nandflash error\r\n");
            printf("Error RecvData[%d]:%d\r\n", i, RecvData[i]);
        }
    }

    Ret = AlSmc_Hal_WritePage(&SmcHal, 0, SendData, sizeof(SendData));
    if (AL_OK != Ret) {
         printf("AlSmc_Hal_WritePage error:0x%x\r\n", Ret);
         while(1);
    }

    Ret = AlSmc_Hal_ReadPage(&SmcHal, 0, RecvData, sizeof(RecvData));
    if (AL_OK != Ret) {
         printf("AlSmc_Hal_ReadPage error:0x%x\r\n", Ret);
         while(1);
    }

    for (i = 0; i < sizeof(RecvData); i++) {
        if((i%256) != RecvData[i]) {
            printf("AlSmc data write nandflash test error\r\n");
            printf("Error RecvData[%d]:%d\r\n", i, RecvData[i]);
            while (1);
        }
    }
    printf("FPSOC smc nand test pass\r\n");
}
