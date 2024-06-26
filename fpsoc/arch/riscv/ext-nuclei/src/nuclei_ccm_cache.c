/*
 * Copyright (c) 2019 Nuclei Limited. All rights reserved.
 *
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
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

#include "al_type.h"
#include "al_rv64_core.h"
#include "al_cache.h"
#include "nuclei_sysregs.h"
#include "al_barrier.h"
#include "al_log.h"

AL_VOID ccm_invalidate_icache_all(AL_VOID)
{
    ARCH_SYSREG_WRITE(CSR_CCM_MCOMMAND, CCM_IC_INVAL_ALL);
    ARCH_SYSREG_WRITE(CSR_CCM_FPIPE, 0x1);
}

AL_VOID ccm_flush_dcache_all(AL_VOID)
{
    ARCH_SYSREG_WRITE(CSR_CCM_MCOMMAND, CCM_DC_WB_ALL);
    ARCH_SYSREG_WRITE(CSR_CCM_FPIPE, 0x1);
}


AL_VOID ccm_invalidate_dcache_all(AL_VOID)
{
    ARCH_SYSREG_WRITE(CSR_CCM_MCOMMAND, CCM_DC_INVAL_ALL);
    ARCH_SYSREG_WRITE(CSR_CCM_FPIPE, 0x1);
}


AL_VOID ccm_flush_dcache_range(AL_UINTPTR Start, AL_UINTPTR End)
{
    AL_UINTPTR _start = Start;
    ARCH_SYSREG_WRITE(CSR_CCM_MBEGINADDR, _start);
    while (_start < End) {
        ARCH_SYSREG_WRITE(CSR_CCM_MCOMMAND, CCM_DC_WB);
        _start += L1_CACHE_BYTES;
    }
    ARCH_SYSREG_WRITE(CSR_CCM_FPIPE, 0x1);
}

AL_VOID ccm_flush_invalidate_dcache_range(AL_UINTPTR Start, AL_UINTPTR End)
{
    AL_UINTPTR _start = Start;
    ARCH_SYSREG_WRITE(CSR_CCM_MBEGINADDR, _start);
    while (_start < End) {
        ARCH_SYSREG_WRITE(CSR_CCM_MCOMMAND, CCM_DC_WBINVAL);
        _start += L1_CACHE_BYTES;
    }
    ARCH_SYSREG_WRITE(CSR_CCM_FPIPE, 0x1);
}

AL_VOID ccm_invalidate_icache_range(AL_UINTPTR Start, AL_UINTPTR End)
{
    /*
     * RISC-V does not have an instruction for invalidating parts of the
     * instruction cache. Invalidate all of it instead.
     */
    AL_UINTPTR _start = Start;
    ARCH_SYSREG_WRITE(CSR_CCM_MBEGINADDR, _start);
    while (_start < End) {
        ARCH_SYSREG_WRITE(CSR_CCM_MCOMMAND, CCM_IC_INVAL);
        _start += L1_CACHE_BYTES;
    }

    ARCH_SYSREG_WRITE(CSR_CCM_FPIPE, 0x1);
}

AL_VOID ccm_invalidate_dcache_range(AL_UINTPTR Start, AL_UINTPTR End)
{
    AL_UINTPTR _start = Start;

    ARCH_SYSREG_WRITE(CSR_CCM_MBEGINADDR, _start);
    while (_start < End) {
        ARCH_SYSREG_WRITE(CSR_CCM_MCOMMAND, CCM_DC_INVAL);
        _start += L1_CACHE_BYTES;
    }

    ARCH_SYSREG_WRITE(CSR_CCM_FPIPE, 0x1);
}


AL_VOID ccm_cache_flush(AL_VOID)
{
    ccm_invalidate_icache_all();
    ccm_flush_dcache_all();

    ARCH_SYSREG_WRITE(CSR_CCM_FPIPE, 0x1);
}

AL_VOID ccm_flush_cache(AL_UINTPTR Start, AL_UINTPTR Size)
{
    ccm_invalidate_icache_range(Start, Start + Size);
    ccm_flush_dcache_range(Start, Start + Size);
}

AL_VOID ccm_set_noncache(AL_UINTPTR Base, AL_UINTPTR Mask)
{
    ARCH_SYSREG_WRITE(CSR_MNOCM, (Mask&0xFFFFFFFC));
    ARCH_SYSREG_WRITE(CSR_MNOCB, (Base&0xFFFFFFFC)|1);
    ISB();
}


AL_VOID AlCache_InvalidateIcacheAll(AL_VOID)
{
    ccm_invalidate_icache_all();
}

AL_VOID AlCache_FlushDcacheRange(AL_UINTPTR Start, AL_UINTPTR End)
{
    ccm_flush_dcache_range(Start, End);
}

AL_VOID AlCache_InvalidateDcacheRange(AL_UINTPTR Start, AL_UINTPTR End)
{
    ccm_invalidate_dcache_range(Start, End);
}


AL_VOID AlCache_FlushAndInvalidateSameDcacheRange(AL_UINTPTR Start, AL_UINTPTR End)
{
    ccm_flush_invalidate_dcache_range(Start, End);
}

AL_VOID AlCache_FlushAndInvalidateDiffDcacheRange(AL_UINTPTR FStart, AL_UINTPTR FEnd, AL_UINTPTR IStart, AL_UINTPTR IEnd)
{
    ccm_flush_dcache_range(FStart, FEnd);
    ccm_invalidate_dcache_range(IStart, IEnd);
}

AL_VOID AlCache_InvalidateDcacheAll(AL_VOID)
{
    ccm_invalidate_dcache_all();
}

AL_VOID AlCache_FlushDcacheAll(AL_VOID)
{
    ccm_flush_dcache_all();
}

AL_VOID AlCache_DisableMmu(AL_VOID)
{
    return;
}


#define RV64_PAGE_SHIFT (12)
#define RV64_PAGE_SIZE  (1 << RV64_PAGE_SHIFT)


AL_S32 AlCache_SetMemoryAttr(AL_UINTPTR Start, AL_UINTPTR End, AL_MemAttr Attr)
{
    AL_U32 Shift;
    AL_UINTPTR Mask;

    if ((End - Start) < RV64_PAGE_SIZE) {
        return AL_ERR_ILLEGAL_PARAM;
    }

    Shift = RV64_PAGE_SHIFT - 1;
    do {
        Shift++;
        Mask = (1UL << Shift) - 1;
    } while ((Start & ~Mask) != ((End - 1) & ~Mask));

    if ((Start & Mask) || ((End - Start) & Mask)) {
        AL_LOG(AL_LOG_LEVEL_WARNING, "noncache addr/size should be aligned to %lx\n", Mask+1);
    }

    ccm_set_noncache((Start & ~Mask), ~Mask);

    return AL_OK;
}
