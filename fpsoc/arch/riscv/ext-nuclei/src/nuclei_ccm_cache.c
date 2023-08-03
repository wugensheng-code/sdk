#include "al_type.h"
#include "al_core.h"
#include "al_params.h"
#include "core_feature_base.h"

/* === Nuclei CCM Registers === */
#define CSR_CCM_MBEGINADDR      0x7CB
#define CSR_CCM_MCOMMAND        0x7CC
#define CSR_CCM_MDATA           0x7CD
#define CSR_CCM_SUEN            0x7CE
#define CSR_CCM_SBEGINADDR      0x5CB
#define CSR_CCM_SCOMMAND        0x5CC
#define CSR_CCM_SDATA           0x5CD
#define CSR_CCM_UBEGINADDR      0x4CB
#define CSR_CCM_UCOMMAND        0x4CC
#define CSR_CCM_UDATA           0x4CD
#define CSR_CCM_FPIPE           0x4CF

/**
 * \brief Cache CCM Command Types
 */
typedef enum CCM_CMD {
    CCM_DC_INVAL = 0x0,                 /*!< Unlock and invalidate D-Cache line specified by CSR CCM_XBEGINADDR */
    CCM_DC_WB = 0x1,                    /*!< Flush the specific D-Cache line specified by CSR CCM_XBEGINADDR */
    CCM_DC_WBINVAL = 0x2,               /*!< Unlock, flush and invalidate the specific D-Cache line specified by CSR CCM_XBEGINADDR */
    CCM_DC_LOCK = 0x3,                  /*!< Lock the specific D-Cache line specified by CSR CCM_XBEGINADDR */
    CCM_DC_UNLOCK = 0x4,                /*!< Unlock the specific D-Cache line specified by CSR CCM_XBEGINADDR */
    CCM_DC_WBINVAL_ALL = 0x6,           /*!< Unlock and flush and invalidate all the valid and dirty D-Cache lines */
    CCM_DC_WB_ALL = 0x7,                /*!< Flush all the valid and dirty D-Cache lines */
    CCM_DC_INVAL_ALL = 0x17,            /*!< Unlock and invalidate all the D-Cache lines */
    CCM_IC_INVAL = 0x8,                 /*!< Unlock and invalidate I-Cache line specified by CSR CCM_XBEGINADDR */
    CCM_IC_LOCK = 0xb,                  /*!< Lock the specific I-Cache line specified by CSR CCM_XBEGINADDR */
    CCM_IC_UNLOCK = 0xc,                /*!< Unlock the specific I-Cache line specified by CSR CCM_XBEGINADDR */
    CCM_IC_INVAL_ALL = 0xd              /*!< Unlock and invalidate all the I-Cache lines */
} CCM_CMD_Type;


AL_VOID ccm_invalidate_icache_all(AL_VOID)
{
    __RV_CSR_WRITE(CSR_CCM_SCOMMAND, CCM_IC_INVAL_ALL);
    __RV_CSR_WRITE(CSR_CCM_FPIPE, 0x1);
}

AL_VOID ccm_flush_dcache_all(AL_VOID)
{
    __RV_CSR_WRITE(CSR_CCM_SCOMMAND, CCM_DC_WB_ALL);
    __RV_CSR_WRITE(CSR_CCM_FPIPE, 0x1);
}


AL_VOID ccm_invalidate_dcache_all(AL_VOID)
{
    __RV_CSR_WRITE(CSR_CCM_SCOMMAND, CCM_DC_INVAL_ALL);
    __RV_CSR_WRITE(CSR_CCM_FPIPE, 0x1);
}


AL_VOID ccm_flush_dcache_range(AL_UINTPTR Start, AL_UINTPTR End)
{
    AL_UINTPTR _start = Start;
    __RV_CSR_WRITE(CSR_CCM_SBEGINADDR, _start);
    while (_start < End) {
        __RV_CSR_WRITE(CSR_CCM_SCOMMAND, CCM_DC_WB);
        _start += ARCH_DMA_MINALIGN;
    }
    __RV_CSR_WRITE(CSR_CCM_FPIPE, 0x1);
}

AL_VOID ccm_invalidate_icache_range(AL_UINTPTR Start, AL_UINTPTR End)
{
    /*
     * RISC-V does not have an instruction for invalidating parts of the
     * instruction cache. Invalidate all of it instead.
     */
    AL_UINTPTR _start = Start;
    __RV_CSR_WRITE(CSR_CCM_SBEGINADDR, _start);
    while (_start < End) {
        __RV_CSR_WRITE(CSR_CCM_SCOMMAND, CCM_IC_INVAL);
        _start += ARCH_DMA_MINALIGN;
    }

    __RV_CSR_WRITE(CSR_CCM_FPIPE, 0x1);
}

AL_VOID ccm_invalidate_dcache_range(AL_UINTPTR Start, AL_UINTPTR End)
{
    AL_UINTPTR _start = Start;

    __RV_CSR_WRITE(CSR_CCM_SBEGINADDR, _start);
    while (_start < End) {
        __RV_CSR_WRITE(CSR_CCM_SCOMMAND, CCM_DC_INVAL);
        _start += ARCH_DMA_MINALIGN;
    }

    __RV_CSR_WRITE(CSR_CCM_FPIPE, 0x1);
}


AL_VOID ccm_cache_flush(AL_VOID)
{
    ccm_invalidate_icache_all();
    ccm_flush_dcache_all();

    __RV_CSR_WRITE(CSR_CCM_FPIPE, 0x1);
}

AL_VOID ccm_flush_cache(AL_UINTPTR Start, AL_UINTPTR Size)
{
    ccm_invalidate_icache_range(Start, Start + Size);
    ccm_flush_dcache_range(Start, Start + Size);
}


AL_VOID AlCache_InvalidateIcacheAll(AL_VOID)
{
    ccm_invalidate_icache_all();
}

AL_VOID AlCache_FlushDcacheRange(AL_UINTPTR Start, AL_UINTPTR End)
{
    ccm_invalidate_dcache_range(Start, End);
}

AL_VOID AlCache_InvalidateDcacheRange(AL_UINTPTR Start, AL_UINTPTR End)
{
    ccm_invalidate_dcache_range(Start, End);
}

AL_VOID AlCache_InvalidateDcacheAll(AL_VOID)
{
    ccm_invalidate_dcache_all();
}

AL_VOID AlCache_FlushDcacheAll(AL_VOID)
{
    ccm_flush_dcache_all();
}