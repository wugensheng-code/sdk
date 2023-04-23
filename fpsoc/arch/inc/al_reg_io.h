#ifndef AL_MMIO_H
#define AL_MMIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "al_type.h"

#define BIT(x)                          ((unsigned int)0x01U<<(x))
#define BIT64(x)                        ((unsigned long)0x01UL<<(x))
#define BITS(start, size)               ((0xFFFFFFFFUL << (start)) & (0xFFFFFFFFUL >> (32U - (start) - (size))))
#define BITS64(start, size)             (((~0x0UL) << (start)) & ((~0x0UL) >> (64U - (start) - (size))))

#define WRITE_REG(reg_addr, value) do { \
    volatile unsigned int *LocalAddress = (volatile unsigned int *) (reg_addr); \
    *LocalAddress = value; \
} while (0);

#define WRITE_REG64(reg_addr, value) do { \
    volatile unsigned long *LocalAddress = (volatile unsigned long *) (reg_addr); \
    *LocalAddress = value; \
} while (0);

#define READ_REG(reg_addr)                          (*(volatile unsigned int *) (unsigned int) (reg_addr))

#define READ_REG64(reg_addr)                        (*(volatile unsigned long *) (unsigned long) (reg_addr))

#define GET_BITS(reg_addr, shift, size)             (((READ_REG(reg_addr)) & BITS((shift),(size))) >> (shift))

#define GET_BITS64(reg_addr, shift, size)           (((READ_REG64(reg_addr)) & BITS64((shift),(size))) >> (shift))

#define GET_BIT(reg_addr, shift)                    (((READ_REG(reg_addr)) & BIT(shift)) >> (shift))

#define GET_BIT64(reg_addr, shift)                  (((READ_REG64(reg_addr)) & BIT64(shift)) >> (shift))

#define SET_BITS(reg_addr, shift, size, value)      WRITE_REG(reg_addr, (READ_REG(reg_addr)) & (~BITS(shift, size)) | ((value) << (shift)))

#define SET_BITS64(reg_addr, shift, size, value)    WRITE_REG64(reg_addr, (READ_REG64(reg_addr)) & (~BITS64(shift, size)) | ((value) << (shift)))

#define SET_BIT(reg_addr, shift, value)             WRITE_REG(reg_addr, (READ_REG(reg_addr)) & (~BIT(shift)) | ((value) << (shift)))

#define SET_BIT64(reg_addr, shift, value)           WRITE_REG64(reg_addr, (READ_REG64(reg_addr)) & (~BIT64(shift)) | ((value) << (shift)))

#define AL_LOG(level, format, ...) \
	do {  \
		if(level >= AL_ERR_LEVEL_DEBUG) \
		printf(format, ##__VA_ARGS__); \
	} while (0);

#ifdef __cplusplus
}
#endif

#endif
