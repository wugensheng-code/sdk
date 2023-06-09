#ifndef __NPU_RUNTIME_INTERRUPT_H__
#define __NPU_RUNTIME_INTERRUPT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "al_type.h"

AL_S8 intr_init();

AL_S8 intr_deinit();

AL_S8 wait_for_hard_npu_irq();

AL_S8 wait_for_soft_npu_irq();

AL_S8 wait_for_yuv2rgb_irq();



#ifdef __cplusplus
}
#endif

#endif