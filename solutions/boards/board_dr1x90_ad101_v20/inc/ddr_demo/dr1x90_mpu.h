#ifndef __AL_DR1X90_MPU_H_
#define __AL_DR1X90_MPU_H_

#include <stdint.h>

struct mpu_region_map
{
    volatile uint32_t rasr;
    volatile uint32_t sar;
    volatile uint32_t ear;
    volatile uint32_t groupid;
};

struct mpu_map
{
    volatile uint32_t typer;
    volatile uint32_t ctrl;
    volatile uint32_t sar;
    volatile uint32_t ear;
    volatile struct mpu_region_map region[32];
};

typedef volatile struct mpu_map* mpu_handle;

// MPU instances
#define MPU_DDR0_INST ((mpu_handle)0xF840E000)
#define MPU_DDR1_INST ((mpu_handle)0xF840F000)
#define MPU_HP0_INST  ((mpu_handle)0xF8412000)
#define MPU_HP1_INST  ((mpu_handle)0xF8413000)
#define MPU_OCM_INST  ((mpu_handle)0xF841B000)
#define MPU_NPU_INST  ((mpu_handle)0xF841C000)
#define MPU_APU_INST  ((mpu_handle)0xF841D000)

// MPU Protect Flags
#define MPU_NONE   (0U)
#define MPU_SECURE (1U << 6)
#define MPU_PRIV   (1U << 7)
#define MPU_WRITE  (1U << 8)
#define MPU_READ   (1U << 9)
#define MPU_UNCFG  (1U << 10)

void mpu_enable(mpu_handle inst);
void mpu_disable(mpu_handle inst);

void mpu_group_enable(mpu_handle inst, uint32_t rid, uint32_t base, uint32_t size, uint32_t flag, uint32_t group_mask);
void mpu_group_disable(mpu_handle inst, uint32_t rid);

// TOP CFG Regs Setting
enum mpu_groupid_t {
    GROUPID_CSU = 0,
    GROUPID_RPU,
    GROUPID_APU,
    GROUPID_NPU = 4,
    GROUPID_JPU,
    GROUPID_DMA_AXI,
    GROUPID_HP0 = 8,
    GROUPID_HP1,
    GROUPID_GP0,
    GROUPID_GP1,
    GROUPID_AHB_BUS
};
void mpu_groupid_set(enum mpu_groupid_t idx, uint32_t gid);

enum tzpc_t {
    TZPC_MPU_DDR0 = 0x1U << 0,
    TZPC_MPU_DDR1 = 0x1U << 1,
    TZPC_MPU_HP0  = 0x1U << 2,
    TZPC_MPU_HP1  = 0x1U << 3,
    TZPC_MPU_OCM  = 0x1U << 4,
    TZPC_MPU_NPU  = 0x1U << 5,
    TZPC_MPU_APU  = 0x1U << 6,
    TZPC_DDR_CFG  = 0x1U << 7
};
void tzpc_prot_set(enum tzpc_t flag);
void tzpc_prot_clr(enum tzpc_t flag);

#endif
