#ifndef __ALSOC_TYPE_H
#define __ALSOC_TYPE_H


#define SZ_1K				0x00000400
#define SZ_4K				0x00001000
#define SZ_1M				0x00100000
#define SZ_1G				0x40000000

#ifdef __ASSEMBLY__
#define _AC(X,Y)	X
#define _AT(T,X)	X
#else
#define __AC(X,Y)	(X##Y)
#define _AC(X,Y)	__AC(X,Y)
#define _AT(T,X)	((T)(X))
#endif

#define U(x)		(_U(x))
#define UL(x)		(_UL(x))
#define ULL(x)		(_ULL(x))
#define L(_x)		(_L(x))
#define LL(_x)		(_LL(x))

#define _U(x)		(_AC(x, U))
#define _UL(x)		(_AC(x, UL))
#define _ULL(x)		(_AC(x, ULL))
#define _L(x)		(_AC(x, L))
#define _LL(x)		(_AC(x, LL))

#ifndef BIT
#define BIT(nr)		(1UL << (nr))
#endif

#ifndef BIT_32
#define BIT_32(nr)	(U(1) << (nr))
#endif

#ifndef BIT_64
#define BIT_64(nr)	(ULL(1) << (nr))
#endif

#define _BITUL(x)	(_UL(1) << (x))
#define _BITULL(x)	(_ULL(1) << (x))

#define __ALIGN_MASK(x, mask)	(((x) + (mask)) & ~(mask))
#define ALIGN(x, a) __ALIGN_MASK(x, (typeof(x))(a) - 1)

#ifndef __ASSEMBLY__
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

typedef char char8;
typedef char s8_t;
typedef char i8_t;
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned char u8_t;

typedef short s16_t;
typedef short i16_t;
typedef unsigned short u16;
typedef signed short  s16;
typedef unsigned short u16_t;

typedef int   s32_t;
typedef int   i32_t;
typedef unsigned int u32;
typedef signed int s32;
typedef unsigned int u32_t;

typedef long long s64_t;
typedef long long i64_t;
typedef unsigned long long AL_U64;
typedef signed long long  s64;
typedef unsigned long long u64_t;

typedef unsigned long u_register_t;

#ifndef min
#define min(a, b) (((a) < (b))?(a):(b))
#endif

#define APU_COMPONENT_IS_READY     0x11111111U  /**< In device drivers, This macro will be
                                                 assigend to "IsReady" member of driver
												 instance to indicate that driver
												 instance is initialized and ready to use. */
#define APU_COMPONENT_IS_STARTED   0x22222222U  /**< In device drivers, This macro will be assigend to
                                                 "IsStarted" member of driver instance
												 to indicate that driver instance is
												 started and it can be enabled. */

#endif /* __ASSEMBLY__ */
#endif /*_TYPE_H*/
