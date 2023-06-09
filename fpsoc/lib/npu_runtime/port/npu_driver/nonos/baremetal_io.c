
#include "npu_defines.h"
#include "npu_driver_io.h"
#include "npu_driver_init.h"

#define HARD_NPU_MASK 0x63f00000
#define SOFT_NPU_MASK 0x91000000

AL_VOID hard_npu_write(AL_U32 offset, AL_U32 value) {

    writel(value, offset + HARD_NPU_MASK) ;

    return ;
}

AL_U32 hard_npu_read(AL_U32 offset) {

    AL_U32 ret = readl(offset + HARD_NPU_MASK) ;

    return ret;
}

AL_VOID soft_npu_write(AL_U32 offset, AL_U32 value){

    writel(value, offset + SOFT_NPU_MASK) ;

    return ;
}

AL_U32 soft_npu_read(AL_U32 offset){

    AL_U32 ret = readl(offset + SOFT_NPU_MASK) ;

    return ret;
}

AL_S8 hard_npu_driver_init() { // nothing need to be done in baremetal
    return 0; 
}

AL_S8 hard_npu_driver_exit() { // nothing need to be done in baremetal
    return 0;
}

AL_S8 soft_npu_driver_init() { // nothing need to be done in baremetal
    return 0; 
}

AL_S8 soft_npu_driver_exit() { // nothing need to be done in baremetal
    return 0;
}