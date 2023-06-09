#ifndef NPU_DEFINES_H
#define NPU_DEFINES_H

/* interfaces outside npu_runtime */
#include "al_type.h"
#include "al_reg_io.h"


/* continous memory reserved for weight */
#define WEIGHT_DATA_LENGTH (70 * 1024 * 1024) // 70M

/* variable define for buddy allocator */
#define FM_SIZE_MASK 0x7fff
#define FM_MIN_SIZE 0x8000
#define FM_MIN_SIZE_BIT_WIDTH 15
#define BUDDY_LEVEL 10

/* port to AL */
#define  readl(addr)  AL_REG32_READ(addr)
#define  writel(val,addr)  AL_REG32_WRITE(addr, val)

/* data type for fifo */
typedef union
{
    AL_U8  u8  [8] ;
    AL_U16 u16 [4] ;
    AL_U32 u32 [2] ;
    AL_U64 u64 ;
    AL_S8  i8  [8] ;
    AL_S16 i16 [4] ;
    AL_S32 i32 [2] ;
    AL_S64 i64 ;
    AL_VOID*   pointer ;
} any_data_t;



#endif