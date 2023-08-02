/**************************************************************************//**
 * @file     cmsis_gcc.h
 * @brief    CMSIS compiler GCC header file
 * @version  V5.0.4
 * @date     09. April 2018
 ******************************************************************************/
/*
 * Copyright (c) 2009-2018 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __NMSIS_GCC_H__
#define __NMSIS_GCC_H__

#include <stdint.h>

#ifdef __cplusplus
 extern "C" {
#endif

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wunused-parameter"

/* Fallback for __has_builtin */
#ifndef __has_builtin
  #define __has_builtin(x) (0)
#endif

/* NMSIS compiler specific defines */
/** \brief Pass information from the compiler to the assembler. */
#ifndef   __ASM
  #define __ASM                                  __asm
#endif

/** \brief Recommend that function should be inlined by the compiler. */
#ifndef   __INLINE
  #define __INLINE                               inline
#endif

/** \brief Define a static function that may be inlined by the compiler. */
#ifndef   __STATIC_INLINE
  #define __STATIC_INLINE                        static inline
#endif

/** \brief Define a static function that should be always inlined by the compiler. */
#ifndef   __STATIC_FORCEINLINE
  #define __STATIC_FORCEINLINE                   __attribute__((always_inline)) static inline
#endif

/** \brief Inform the compiler that a function does not return. */
#ifndef   __NO_RETURN
  #define __NO_RETURN                            __attribute__((__noreturn__))
#endif

/** \brief Inform that a variable shall be retained in executable image. */
#ifndef   __USED
  #define __USED                                 __attribute__((used))
#endif

/** \brief restrict pointer qualifier to enable additional optimizations. */
#ifndef   __WEAK
  #define __WEAK                                 __attribute__((weak))
#endif

/** \brief specified the vector size of the variable, measured in bytes */
#ifndef   __VECTOR_SIZE
  #define __VECTOR_SIZE(x)                       __attribute__((vector_size(x)))
#endif

/** \brief Request smallest possible alignment. */
#ifndef   __PACKED
  #define __PACKED                               __attribute__((packed, aligned(1)))
#endif

/** \brief Request smallest possible alignment for a structure. */
#ifndef   __PACKED_STRUCT
  #define __PACKED_STRUCT                        struct __attribute__((packed, aligned(1)))
#endif

/** \brief Request smallest possible alignment for a union. */
#ifndef   __PACKED_UNION
  #define __PACKED_UNION                         union __attribute__((packed, aligned(1)))
#endif

#ifndef   __UNALIGNED_UINT16_WRITE
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wpacked"
  #pragma GCC diagnostic ignored "-Wattributes"
  /** \brief Packed struct for unaligned uint16_t write access */
  __PACKED_STRUCT T_UINT16_WRITE {
      uint16_t v;
  };
  #pragma GCC diagnostic pop
  /** \brief Pointer for unaligned write of a uint16_t variable. */
  #define __UNALIGNED_UINT16_WRITE(addr, val)    (void)((((struct T_UINT16_WRITE *)(void *)(addr))->v) = (val))
#endif

#ifndef   __UNALIGNED_UINT16_READ
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wpacked"
  #pragma GCC diagnostic ignored "-Wattributes"
  /** \brief Packed struct for unaligned uint16_t read access */
  __PACKED_STRUCT T_UINT16_READ {
      uint16_t v;
  };
  #pragma GCC diagnostic pop
  /** \brief Pointer for unaligned read of a uint16_t variable. */
  #define __UNALIGNED_UINT16_READ(addr)          (((const struct T_UINT16_READ *)(const void *)(addr))->v)
#endif

#ifndef   __UNALIGNED_UINT32_WRITE
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wpacked"
  #pragma GCC diagnostic ignored "-Wattributes"
  /** \brief Packed struct for unaligned uint32_t write access */
  __PACKED_STRUCT T_UINT32_WRITE {
      uint32_t v;
  };
  #pragma GCC diagnostic pop
  /** \brief Pointer for unaligned write of a uint32_t variable. */
  #define __UNALIGNED_UINT32_WRITE(addr, val)    (void)((((struct T_UINT32_WRITE *)(void *)(addr))->v) = (val))
#endif

#ifndef   __UNALIGNED_UINT32_READ
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wpacked"
  #pragma GCC diagnostic ignored "-Wattributes"
  /** \brief Packed struct for unaligned uint32_t read access */
  __PACKED_STRUCT T_UINT32_READ {
      uint32_t v;
  };
  #pragma GCC diagnostic pop
  /** \brief Pointer for unaligned read of a uint32_t variable. */
  #define __UNALIGNED_UINT32_READ(addr)          (((const struct T_UINT32_READ *)(const void *)(addr))->v)
#endif

/** \brief Minimum `x` bytes alignment for a variable. */
#ifndef   __ALIGNED
  #define __ALIGNED(x)                           __attribute__((aligned(x)))
#endif

/** \brief restrict pointer qualifier to enable additional optimizations. */
#ifndef   __RESTRICT
  #define __RESTRICT                             __restrict
#endif

/** \brief Barrier to prevent compiler from reordering instructions. */
#ifndef   __COMPILER_BARRIER
  #define __COMPILER_BARRIER()                   __ASM volatile("":::"memory")
#endif

#define __branch_check__(x, expect, is_constant) ({            \
        long ______r;                    \
        ______r = __builtin_expect(!!(x), expect);    \
        ______r;                    \
    })

/*
 * Using __builtin_constant_p(x) to ignore cases where the return
 * value is always the same.  This idea is taken from a similar patch
 * written by Daniel Walker.
 */
# ifndef LIKELY
#  define LIKELY(x)    (__branch_check__(x, 1, __builtin_constant_p(x)))
# endif

# ifndef UNLIKELY
#  define UNLIKELY(x)    (__branch_check__(x, 0, __builtin_constant_p(x)))
# endif

/** \brief Use this attribute to indicate that the specified function is an interrupt handler. */
#ifndef   __INTERRUPT
  #define __INTERRUPT                            __attribute__((interrupt))
#endif

#ifdef __cplusplus
  #define   __I     volatile
#else
  #define   __I     volatile const
#endif
/** \brief Defines 'write only' permissions */
#define     __O     volatile
/** \brief Defines 'read / write' permissions */
#define     __IO    volatile

/* following defines should be used for structure members */
/** \brief Defines 'read only' structure member permissions */
#define     __IM     volatile const
/** \brief Defines 'write only' structure member permissions */
#define     __OM     volatile
/** \brief Defines 'read/write' structure member permissions */
#define     __IOM    volatile

#ifdef __cplusplus
}
#endif
#endif
