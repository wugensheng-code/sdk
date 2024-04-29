/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifdef ENABLE_MMU
#include "al_mmu.h"
    .globl MMUTableL0
    .globl MMUTableL1
    .globl MMUTableL2
    .globl mmu_2m_list

    .section .page_table,"a"
    .balign 0x1000
MMUTableL0:
#ifdef DDR_2M_MAPPING
    .set SECT, MMUTableL1
    .8byte SECT + 3            /* 0x00000000 - 0x3F000000 */
    .set SECT, SECT + 0x1000
    .8byte SECT + 3            /* 0x40000000 - 0x7F000000 */

    .set SECT, 0x80000000
    .8byte SECT + DEVICE_MEM    /* 0x80000000 - 0xBF000000 */
    .set SECT, SECT + 0x40000000

    .8byte SECT + DEVICE_MEM    /* 0xC0000000 - 0xFF000000 */
    .set SECT, SECT + 0x40000000

    .set SECT, MMUTableL1 + 0x2000
    .8byte SECT + 3            /* 0x100000000 - 0x13F000000 */
    .set SECT, SECT + 0x1000

    .8byte SECT + 3            /* 0x140000000 - 0x17F000000 */
    .set SECT, SECT + 0x1000

    .8byte SECT + 3            /* 0x180000000 - 0x1BF000000 */
    .set SECT, SECT + 0x1000

    .set SECT, 0x1C0000000
    .8byte SECT + DEVICE_MEM    /* 0x1C0000000 - 0x1FF000000 */
    .set SECT, SECT + 0x40000000
#else
    .set SECT, 0x00000000
    .8byte SECT + NORM_CACHE    /* 0x00000000 - 0x3F000000 */

    .set SECT, MMUTableL1
    .8byte SECT + 3            /* 0x40000000 - 0x7F000000 */

    .set SECT, 0x80000000
    .8byte SECT + DEVICE_MEM    /* 0x80000000 - 0xBF000000 */
    .set SECT, SECT + 0x40000000

    .8byte SECT + DEVICE_MEM    /* 0xC0000000 - 0xFF000000 */
    .set SECT, SECT + 0x40000000

    .8byte SECT + NORM_CACHE    /* 0x100000000 - 0x13F000000 */
    .set SECT, SECT + 0x40000000

    .8byte SECT + NORM_CACHE    /* 0x140000000 - 0x17F000000 */
    .set SECT, SECT + 0x40000000

    .8byte SECT + NORM_CACHE    /* 0x180000000 - 0x1BF000000 */
    .set SECT, SECT + 0x40000000

    .set SECT, 0x1C0000000
    .8byte SECT + DEVICE_MEM    /* 0x1C0000000 - 0x1FF000000 */
    .set SECT, SECT + 0x40000000
#endif

    .rept (0x8000000000-SECT)/0x40000000
    .8byte SECT + RESERVED
    .set SECT, SECT + 0x40000000
    .endr

    .balign 0x1000
MMUTableL1:
#ifdef DDR_2M_MAPPING
    .set SECT, 0x00000000
#else
    .set SECT, 0x40000000
#endif
    .rept (0x63E00000-SECT)/0x200000
    .8byte SECT + NORM_CACHE
    .set SECT, SECT + 0x200000
    .endr

    .rept (0x64000000-SECT)/0x200000
    .8byte SECT + DEVICE_MEM
    .set SECT, SECT + 0x200000
    .endr

    .rept (0x70000000-SECT)/0x200000
    .8byte SECT + RESERVED
    .set SECT, SECT + 0x200000
    .endr

    .rept (0x80000000-SECT)/0x200000
    .8byte SECT + NORM_CACHE
    .set SECT, SECT + 0x200000
    .endr

#ifdef DDR_2M_MAPPING
    .set SECT, 0x100000000
    .rept (0x1C0000000-SECT)/0x200000
    .8byte SECT + NORM_CACHE
    .set SECT, SECT + 0x200000
    .endr
#endif

#if defined(DDR_2M_MAPPING) && defined(CODE_READONLY)
    .balign 0x1000
MMUTableL2:
    .rept 512
    .8byte 0
    .endr
#endif

    .section .rodata,"a"
mmu_2m_list:
#ifdef DDR_2M_MAPPING
    .set SECT, MMUTableL1
    .8byte 0x00000000,  SECT, 0x40000000    /* 0x00000000 - 0x3F000000 */
    .set SECT, SECT + 0x1000
    .8byte 0x40000000,  SECT, 0x40000000    /* 0x40000000 - 0x7F000000 */
    .set SECT, SECT + 0x1000
    .8byte 0x100000000, SECT, 0x40000000    /* 0x100000000 - 0x13F000000 */
    .set SECT, SECT + 0x1000
    .8byte 0x140000000, SECT, 0x40000000    /* 0x140000000 - 0x17F000000 */
    .set SECT, SECT + 0x1000
    .8byte 0x180000000, SECT, 0x40000000    /* 0x180000000 - 0x1BF000000 */
#else
    .set SECT, MMUTableL1
    .8byte 0x40000000,  SECT, 0x40000000    /* 0x40000000 - 0x7F000000 */
#endif
    .8byte 0, 0, 0

#endif
.end
