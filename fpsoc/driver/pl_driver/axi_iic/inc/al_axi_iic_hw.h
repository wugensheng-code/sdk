/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef AL_AXI_IIC_HW_H_
#define AL_AXI_IIC_HW_H_

#ifdef __cplusplus
extern "C" {
#endif

// test, parameter from TD hpf file 
#define AL_AXI_IIC_NUM_INSTANCE 2
#define AXI_I2C0__BASE_ADDR 0x80000000
#define AXI_I2C1__BASE_ADDR 0x80100000
#define AXI_IIC0_IRQn 114
#define AXI_IIC1_IRQn 115

/***************************** Include Files ********************************/
#include "al_core.h"

typedef struct {
    AL_U32      DeviceId;
    AL_U32      BaseAddress;
    AL_U32      IntrId;
} AlAxiIic_HwConfigStruct;

/***************************** Register Offset ********************************/
#define AXI_IIC_ID_OFFSET       0x00
#define AXI_IIC_CFG_OFFSET      0x10
#define AXI_IIC_INTEN_OFFSET    0x14
#define AXI_IIC_SR_OFFSET       0x18
#define AXI_IIC_ADDR_OFFSET     0x1c
#define AXI_IIC_DATA_OFFSET     0x20
#define AXI_IIC_CR_OFFSET       0x24
#define AXI_IIC_CMD_OFFSET      0x28
#define AXI_IIC_SETUP_OFFSET    0x2c
#define AXI_IIC_TPM_OFFSET      0x30

/***************************** AXI_IIC_ID_OFFSET Register    *****************************/
#define AXI_IIC_ID          0x020210
#define AXI_IIC_ID_START    8
#define AXI_IIC_ID_SIZE     24

/***************************** AXI_IIC_CFG_OFFSET Register   *****************************/
#define AXI_IIC_CFG_START    0
#define AXI_IIC_CFG_SIZE     2

/***************************** AXI_IIC_INTEN_OFFSET Register *****************************/
#define INT_CMPL            9
#define INT_BYTE_RECV       8
#define INT_BYTE_TRANS      7
#define INT_START           6
#define INT_STOP            5
#define INT_ARB_LOSE        4
#define INT_ADDR_HIT        3
#define INT_FIFO_HALF       2
#define INT_FIFO_FULL       1
#define INT_FIFO_EMPTY      0

/***************************** AXI_IIC_SR_OFFSET Register    *****************************/
#define SR_LINE_SDA        14
#define SR_LINE_SCL        13
#define SR_GEN_CALL        12
#define SR_BUS_BUSY        11
#define SR_ACK             10
#define SR_CMPL            9
#define SR_BYTE_RECV       8
#define SR_BYTE_TRANS      7
#define SR_START           6
#define SR_STOP            5
#define SR_ARB_LOSE        4
#define SR_ADDR_HIT        3
#define SR_FIFO_HALF       2
#define SR_FIFO_FULL       1
#define SR_FIFO_EMPTY      0

/***************************** AXI_IIC_ADDR_OFFSET Register  *****************************/
/***************************** AXI_IIC_DATA_OFFSET Register  *****************************/

/***************************** AXI_IIC_CR_OFFSET Register    *****************************/
#define CR_PHASE_START      12
#define CR_PHASE_ADDR       11
#define CR_PHASE_DATA       10
#define CR_PHASE_STOP       9
#define CR_DIR              8
#define CR_DATA_CNT         0        // 0 - 7

#define CR_DATA_CNT_SIZE    8

/***************************** AXI_IIC_CMD_OFFSET Register   *****************************/
#define CMD         0 // 0 - 2
#define CMD_SIZE    3

/***************************** AXI_IIC_SETUP_OFFSET Register *****************************/
#define T_SUDAT             24
#define T_SP                21
#define T_HDDAT             16
#define T_SCL_RATIO         13
#define T_SCL_HIGH          4
#define IIC_MODE            2
#define IIC_ADDRESS_MODE    1
#define IIC_EN              0

#define T_SUDAT_SIZE        5
#define T_SP_SIZE           3
#define T_HDDAT_SIZE        5
#define T_SCL_HIGH_SIZE     9
/***************************** AXI_IIC_TPM_OFFSET Register   *****************************/

#ifdef __cplusplus
}
#endif

#endif


