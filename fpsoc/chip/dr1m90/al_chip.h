#ifndef __AL_CHIP_H__
#define __AL_CHIP_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "al_type.h"

AL_VOID AlChip_Dr1m90Init(AL_VOID);

#define AlChip_Init AlChip_Dr1m90Init

#ifdef __cplusplus
}
#endif
#endif /* __AL_CHIP_H__ */
