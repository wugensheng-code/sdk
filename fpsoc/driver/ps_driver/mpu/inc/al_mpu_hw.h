#ifndef AL_MPU_HW_H
#define AL_MPU_HW_H

#ifdef _cplusplus
extern "C" {
#endif

/* MPU interrupt state register */
#define INTR_MPU_STATE              (0xf8800214UL)

/* MPU base addrress */
#define MPU_DDRS0_BASE_ADDR         (0xF840E000UL)
#define MPU_DDRS1_BASE_ADDR         (0xF840F000UL)
#define MPU_HPM0_BASE_ADDR          (0xF8412000UL)
#define MPU_HPM1_BASE_ADDR          (0xF8413000UL)
#define MPU_OCMS2_BASE_ADDR         (0xF841B000UL)
#define MPU_NPU_BASE_ADDR           (0xF841C000UL)
#define MPU_APU_BASE_ADDR           (0xF841D000UL)

/* register offset and register field offset */
#define MPU_TYPER_OFFSET            0x0

#define MPU_CTRL_OFFSET             0x4
#define MPU_CTRL_ENABLE_SHIFT       0

#define MPU_SAR_OFFSET              0x8
#define MPU_EAR_OFFSET              0xC
#define MPU_RASR_REGION1_OFFSET     0x10

#define MPU_REGION_I_BASE_ADDR(MPU_BASE_ADDR, REGION_NUMBER) \
(MPU_BASE_ADDR + MPU_RASR_REGION1_OFFSET + (REGION_NUMBER * 0x10))

#define MPU_REGION_RASR_REGION_OFFSET       0x0
#define MPU_RASR_REGION_REGIONEN_SHIFT      0
#define MPU_RASR_REGION_REGIONSIZE_SHIFT    1
#define MPU_RASR_REGION_REGIONSIZE_SIZE     5
#define MPU_RASR_REGION_SECURE_SHIFT        6
#define MPU_RASR_REGION_PRIVILEDGE_SHIFT    7
#define MPU_RASR_REGION_RW_SHIFT            8
#define MPU_RASR_REGION_RW_SIZE             2
#define MPU_RASR_REGION_RESERVED_SHIFT      10
#define MPU_RASR_REGION_RESERVED_SIZE       2
#define MPU_RASR_REGION_INTR_EN_SHIFT       12
#define MPU_RASR_REGION_INTR_MASK_SHIFT     13
#define MPU_RASR_REGION_INTR_CLR_SHIFT      14

#define MPU_REGION_SAR_REGION_OFFSET        0x4
#define MPU_REGION_EAR_REGION_OFFSET        0x8
#define MPU_REGION_GROUPID_REGION_OFFSET    0xc

#ifdef __cplusplus
}
#endif

#endif /* AL_MPU_HW_H */