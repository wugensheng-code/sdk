#ifndef __SYSREGS_H
#define __SYSREGS_H

#define HCR_RW                          (1 << 31)

#define HCR_HOST_NVHE_FLAGS             (HCR_RW)
#define SCR_RW_AARCH64                  (1 << 10)
#define SCR_NS                          (1 << 0 )
//#define SCTLR_ELx_M	                    (1<<0)

#define SCTLR_EE_LITTLE_ENDIAN          (0 << 25)
#define SCTLR_EOE_LITTLE_ENDIAN         (0 << 24)
#define SCTLR_MMU_DISABLED              (0 << 0)
#define SCTLR_VALUE_MMU_DISABLED        (SCTLR_MMU_DISABLED | SCTLR_EE_LITTLE_ENDIAN | SCTLR_EOE_LITTLE_ENDIAN )

#define SPSR_MASK_ALL                   (0x07 << 6)
#define SPSR_EL0t                       (0 << 0)
#define SPSR_EL1h                       (5 << 0)
#define SPSR_EL2h                       (9 << 0)

// enable data abort
#define SPSR_EL0                        (SPSR_MASK_ALL | SPSR_EL0t)

#define SPSR_EL1                        (SPSR_MASK_ALL | SPSR_EL1h)
#define SPSR_EL2                        (SPSR_MASK_ALL | SPSR_EL2h)

#define CurrentEL_EL1                   (1 << 2)
#define CurrentEL_EL2                   (2 << 2)
#define CurrentEL_EL3                   (3 << 2)

#ifndef __ASSEMBLY__
#include "sysreg.h"
#endif
#endif /* __SYSREGS_H */
