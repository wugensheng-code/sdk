/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef AL_MPU_HW_H
#define AL_MPU_HW_H

#ifdef __cplusplus
extern "C" {
#endif

#include "al_core.h"

#define AL_MPU_NUM_INSTANCE         (7)
/* apu mpu is not included here */
#define AL_MPU_NUM_COMMON_INSTANCE  (6)

/* MPU interrupt state register */
#define MPU_INTR_STATE_BASE_ADDR    (0xf8800214UL)
#define MPU_INTR_STATE_SHIFT        0
#define MPU_INTR_STATE_SHIFT_SIZE   7

/* MPU base address */
#define MPU_DDRS0_BASE_ADDR         0xF840E000UL
#define MPU_DDRS1_BASE_ADDR         0xF840F000UL
#define MPU_HPM0_BASE_ADDR          0xF8412000UL
#define MPU_HPM1_BASE_ADDR          0xF8413000UL
#define MPU_NPU_BASE_ADDR           0xF841C000UL
#define MPU_OCMS2_BASE_ADDR         0xF841B000UL
#define MPU_APU_BASE_ADDR           0xF841D000UL

/* The RASR register address of Region number */
#define MPU_REGION_BASE_ADDR(MPU_BASE_ADDR, REGION_NUMBER)  (MPU_BASE_ADDR + 0x10 + (REGION_NUMBER * 0x10))

///////////////////////////////////////////////////////
// Register: MPU_TYPER
//
///////////////////////////////////////////////////////
#define MPU_TYPER_OFFSET        0x0
#define MPU_TYPER_NUM           0x1

#define MPU_TYPER_INTR_SHIFT    0

///////////////////////////////////////////////////////
// Register: MPU_CTRL
// MPU enable and region to be configured next
///////////////////////////////////////////////////////
#define MPU_CTRL_OFFSET                   0x4
#define MPU_CTRL_NUM                      0x1

#define MPU_CTRL_ENABLE_SHIFT             0
#define MPU_CTRL_RESERVED_7_1_SHIFT       1
#define MPU_CTRL_REGION_SHIFT             8
#define MPU_CTRL_RESERVED_31_16_SHIFT     16

///////////////////////////////////////////////////////
// Register: MPU_SAR
// Region Base address register
///////////////////////////////////////////////////////
#define MPU_SAR_OFFSET                0x8
#define MPU_SAR_NUM                   0x1

#define MPU_SAR_RESERVED_SHIFT        0
#define MPU_SAR_START_ADDR_SHIFT      12

///////////////////////////////////////////////////////
// Register: MPU_EAR
// Region End address register
///////////////////////////////////////////////////////
#define MPU_EAR_OFFSET            0xC
#define MPU_EAR_NUM               0x1

#define MPU_EAR_RESERVED_SHIFT    0
#define MPU_EAR_END_ADDR_SHIFT    12

/*
  Note:The offset of the following registers is relative to each region RASR register.
 */

///////////////////////////////////////////////////////
// Register: MPU_RASR_REGION
// Region properties and capacity registers
///////////////////////////////////////////////////////
#define MPU_RASR_REGION_OFFSET                  0x0
#define MPU_RASR_REGION_NUM                     0x1

#define MPU_RASR_REGION_REGIONEN_SHIFT          0
#define MPU_RASR_REGION_REGIONSIZE_SHIFT        1
#define MPU_RASR_REGION_SECURE_SHIFT            6
#define MPU_RASR_REGION_PRIVILEGE_SHIFT         7
#define MPU_RASR_REGION_RW_SHIFT                8
#define MPU_RASR_REGION_RESERVED_11_10_SHIFT    10
#define MPU_RASR_REGION_INTR_EN_SHIFT           12
#define MPU_RASR_REGION_INTR_MASK_SHIFT         13
#define MPU_RASR_REGION_INTR_CLR_SHIFT          14
#define MPU_RASR_REGION_RESERVED_31_15_SHIFT    15

#define MPU_RASR_REGION_REGIONSIZE_SIZE         5
#define MPU_RASR_REGION_RW_SIZE                 2

///////////////////////////////////////////////////////
// Register: MPU_SAR_REGION
// Region Base address register
///////////////////////////////////////////////////////
#define MPU_SAR_REGION_OFFSET             0x4
#define MPU_SAR_REGION_NUM                0x1

#define MPU_SAR_REGION_START_ADDR_SHIFT   0
///////////////////////////////////////////////////////
// Register: MPU_EAR_REGION
// Region End address register
///////////////////////////////////////////////////////
#define MPU_EAR_REGION_OFFSET             0x8
#define MPU_EAR_REGION_NUM                0x1

#define MPU_EAR_REGION_END_ADDR_SHIFT     0
///////////////////////////////////////////////////////
// Register: MPU_GROUPID_REGION
//
///////////////////////////////////////////////////////
#define MPU_GROUPID_REGION_OFFSET         0xC
#define MPU_GROUPID_REGION_NUM            0x1

#define MPU_GROUPID_REGION_GROUPID_SHIF   0

/**************************** Type Definitions ******************************/
typedef struct {
    AL_U32                  DeviceId;
    AL_U32                  BaseAddress;
} AL_MPU_HwConfigStruct;

#ifdef __cplusplus
}
#endif

#endif /* AL_MPU_HW_H */