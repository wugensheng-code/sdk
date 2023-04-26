#ifndef AL_MPU_HW_H
#define AL_MPU_HW_H

#ifdef _cplusplus
extern "C" {
#endif

/* MPU interrupt state register */
#define MPU_INTR_STATE_BASE_ADDR    (0xf8800214UL)
#define MPU_INTR_STATE_SHIFT        0
#define MPU_INTR_STATE_SHIFT_SIZE   7

/* MPU base address */
#define MPU__MPU_DDRS0__BASE_ADDR   0xF840E000UL
#define MPU__MPU_DDRS1__BASE_ADDR   0xF840F000UL
#define MPU__MPU_HPM0__BASE_ADDR    0xF8412000UL
#define MPU__MPU_HPM1__BASE_ADDR    0xF8413000UL
#define MPU__MPU_NPU__BASE_ADDR     0xF841C000UL
#define MPU__MPU_OCMS2__BASE_ADDR   0xF841B000UL
#define MPU__MPU_APU__BASE_ADDR     0xF841D000UL

/* The RASR register address of Region i */
#define MPU_REGION_I_BASE_ADDR(MPU_BASE_ADDR, REGION_NUMBER) \
                              (MPU_BASE_ADDR + (REGION_NUMBER * 0x10))

///////////////////////////////////////////////////////
// Register: MPU_TYPER
// 
///////////////////////////////////////////////////////
#define MPU__TYPER__OFFSET          0x0
#define MPU__TYPER__NUM             0x1

#define MPU__TYPER__INTR__SHIFT     0

///////////////////////////////////////////////////////
// Register: MPU_CTRL
// MPU enable and region to be configured next
///////////////////////////////////////////////////////
#define MPU__CTRL__OFFSET                   0x4
#define MPU__CTRL__NUM                      0x1

#define MPU__CTRL__ENABLE__SHIFT            0
#define MPU__CTRL__RESERVED_7_1__SHIFT      1
#define MPU__CTRL__REGION__SHIFT            8
#define MPU__CTRL__RESERVED_31_16__SHIFT    16

///////////////////////////////////////////////////////
// Register: MPU_SAR
// Region Base address register
///////////////////////////////////////////////////////
#define MPU__SAR__OFFSET                0x8
#define MPU__SAR__NUM                   0x1

#define MPU__SAR__RESERVED__SHIFT       0
#define MPU__SAR__START_ADDR__SHIFT     12

///////////////////////////////////////////////////////
// Register: MPU_EAR
// Region End address register
///////////////////////////////////////////////////////
#define MPU__EAR__OFFSET            0xC
#define MPU__EAR__NUM               0x1

#define MPU__EAR__RESERVED__SHIFT   0
#define MPU__EAR__END_ADDR__SHIFT   12

/*
  Note:The offset of the following registers is relative to each region RASR register.
 */

///////////////////////////////////////////////////////
// Register: MPU_RASR_REGION
// Region properties and capacity registers
///////////////////////////////////////////////////////
#define MPU__RASR_REGION__OFFSET                    0x0
#define MPU__RASR_REGION__NUM                       0x1

#define MPU__RASR_REGION__REGIONEN__SHIFT           0
#define MPU__RASR_REGION__REGIONSIZE__SHIFT         1
#define MPU__RASR_REGION__SECURE__SHIFT             6
#define MPU__RASR_REGION__PRIVILEGE__SHIFT          7
#define MPU__RASR_REGION__RW__SHIFT                 8
#define MPU__RASR_REGION__RESERVED_11_10__SHIFT     10
#define MPU__RASR_REGION__INTR_EN__SHIFT            12
#define MPU__RASR_REGION__INTR_MASK__SHIFT          13
#define MPU__RASR_REGION__INTR_CLR__SHIFT           14
#define MPU__RASR_REGION__RESERVED_31_15__SHIFT     15

#define MPU__RASR_REGION__REGIONSIZE__SIZE          5
#define MPU__RASR_REGION__RW__SIZE                  2

///////////////////////////////////////////////////////
// Register: MPU_SAR_REGION
// Region Base address register
///////////////////////////////////////////////////////
#define MPU__SAR_REGION__OFFSET             0x4
#define MPU__SAR_REGION__NUM                0x1

#define MPU__SAR_REGION__START_ADDR__SHIFT  0
///////////////////////////////////////////////////////
// Register: MPU_EAR_REGION
// Region End address register
///////////////////////////////////////////////////////
#define MPU__EAR_REGION__OFFSET             0x8
#define MPU__EAR_REGION__NUM                0x1

#define MPU__EAR_REGION__END_ADDR__SHIFT    0
///////////////////////////////////////////////////////
// Register: MPU_GROUPID_REGION
// 
///////////////////////////////////////////////////////
#define MPU__GROUPID_REGION__OFFSET             0xC
#define MPU__GROUPID_REGION__NUM                0x1

#define MPU__GROUPID_REGION__GROUPID__SHIFT     0

#ifdef __cplusplus
}
#endif

#endif /* AL_MPU_HW_H */