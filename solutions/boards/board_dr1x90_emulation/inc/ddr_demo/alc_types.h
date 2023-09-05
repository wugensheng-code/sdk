
#ifndef ALC_TYPES_H
#define ALC_TYPES_H

#include <stdint.h>
#include <stddef.h>

#ifndef TRUE
#    define TRUE 1U
#endif

#ifndef FALSE
#    define FALSE 0U
#endif

#ifndef NULL
#    define NULL 0U
#endif

typedef uint8_t  u8 ;
typedef uint16_t u16;
typedef uint32_t u32;

typedef int32_t  s32;


typedef intptr_t  INTPTR ;
typedef uintptr_t UINTPTR;
typedef ptrdiff_t PTRDIFF;

#endif
