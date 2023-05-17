#include "io.h"
#include "driver_api.h"
#include "bare_metal_io.h"

#define HARD_NPU_MASK 0x63f00000
#define SOFT_NPU_MASK 0x91000000

void hard_npu_write(uint32_t offset, uint32_t value) {

    writel(value, offset + HARD_NPU_MASK) ;

    return ;
}

uint32_t hard_npu_read(uint32_t offset) {

    uint32_t ret = readl(offset + HARD_NPU_MASK) ;

    return ret;
}

void soft_npu_write(uint32_t offset, uint32_t value){

    writel(value, offset + SOFT_NPU_MASK) ;

    return ;
}

uint32_t soft_npu_read(uint32_t offset){

    uint32_t ret = readl(offset + SOFT_NPU_MASK) ;

    return ret;
}
