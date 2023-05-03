#ifndef AL_CORE_H
#define AL_CORE_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include "al_barrier.h"
#include "al_cache.h"
#include "al_io.h"
#include "al_irq.h"
#include "al_systimer.h"

AL_VOID al_mmu_disable(AL_VOID);

AL_U32 al_mmu_status(AL_VOID);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* AL_CORE_H */
