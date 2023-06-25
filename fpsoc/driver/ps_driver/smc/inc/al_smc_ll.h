#ifndef AL_SMC_LL_H
#define AL_SMC_LL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "al_smc_hw.h"
#include "al_reg_io.h"

typedef enum
{
    SMC_BUSY    = 0,
    SMC_READY   = 1
} AL_SMC_BusyEnum;

typedef enum
{
    UPDATEREGS_AND_AXI      = 0,
    MODEREG                 = 1,
    UPDATE_REGS             = 2,
    MODEREG_AND_UPDATEREGS  = 3
} AL_SMC_CmdTypeEnum;

typedef enum
{
    INTF0_CHIP1   = 0,
    INTF0_CHIP2   = 1,
    INTF0_CHIP3   = 2,
    INTF0_CHIP4   = 3,
    INTF1_CHIP1   = 4,
    INTF1_CHIP2   = 5,
    INTF1_CHIP3   = 6,
    INTF1_CHIP4   = 7
} AL_SMC_ChipNumEnum;

typedef union SMC_CyclesStruct
{  AL_U32 d32;
   struct
   {
        AL_U32 Trc          :4;     /* Returns the busy to re_n time. Minimum permitted value = 0 */
        AL_U32 Twc          :4;     /* Returns the ID read time. Minimum permitted value = 0 */
        AL_U32 Trea         :3;     /* Returns the status read time. Minimum permitted value = 0 */
        AL_U32 Twp          :3;     /* Returns the we_n deassertion delay. Minimum permitted value = 1 */
        AL_U32 Tclr         :3;     /* Returns the re_n assertion delay. Minimum permitted value = 1 */
        AL_U32 Tar          :3;     /* Returns the write cycle time. Minimum permitted value = 2 */
        AL_U32 Trr          :4;     /* Returns the read cycle time. Minimum permitted value = 2 */
        AL_U32 Reserved     :8;
   }
   b;
}AL_SMC_CyclesStruct;

typedef enum
{
    MW_8BITS      = 0,
    MW_16BITS     = 1
} AL_SMC_MemWidthEnum;

typedef enum
{
    ECC_READY   = 0,
    ECC_BUSY    = 1
} AL_SMC_EccBusyEnum;


typedef enum
{
    NO_512BYTE_SIZE     = 0,
    ONE_512BYTE_SIZE    = 1,
    TWO_512BYTE_SIZE    = 2,
    FOUR_512BYTE_SIZE   = 3
} AL_SMC_EccPageSizeEnum;

typedef enum
{
    ECC_BYPASSS     = 0,
    ECC_MODE_APB    = 1,
    ECC_MODE_MEM    = 2
} AL_SMC_EccModeEnum;

typedef enum
{
    InBoundary  = 0,
    InPageEnd   = 1
} AL_SMC_EccReadEndEnum;

typedef enum
{
    ECC_NOJUMP                  = 0,
    ECCJUMP_USE_COL_CHANGE      = 1,
    ECCJUMP_USE_FULL_CHANGE     = 2
} AL_SMC_EccJumpEnum;


typedef enum
{
    AL_SMC_WRITE_DONE       = (0x01),
    AL_SMC_READ_DONE        = (0x01 << 1),
    AL_SMC_WRITE_TIMEOUT    = (0x01 << 2),
    AL_SMC_READ_TIMEOUT     = (0x01 << 3)
} AL_SMC_EventIdEnum;

typedef struct
{
    AL_SMC_EventIdEnum      Event;
    AL_U32                  EventData;
} AL_SMC_EventStruct;

typedef struct
{
    AL_SMC_CyclesStruct     Cycles;
    AL_SMC_MemWidthEnum     SmcWidth;
} AL_SMC_ConfigsStruct;


static inline AL_SMC_BusyEnum AlSmc_ll_IsBusy(AL_REG SmcBaseAddr)
{
    return AL_REG32_GET_BIT(SmcBaseAddr + SMC_MEMC_STATUS_OFFSET, SMC_MEMC_STATUS_RAW_INT_STATUS1_SHIFT);
}

static inline AL_VOID AlSmc_ll_ClrIntr1(AL_REG SmcBaseAddr)
{
    AL_REG32_SET_BIT(SmcBaseAddr + SMC_MEM_CFG_CLR_OFFSET, SMC_MEM_CFG_CLR_INT_CLEAR1_SHIFT, 1);
}

static inline AL_VOID AlSmc_ll_SetCmdTypeAndChipNum(AL_REG SmcBaseAddr, AL_SMC_CmdTypeEnum CmdType, AL_SMC_ChipNumEnum ChipNum)
{
    AL_REG32_WRITE(SmcBaseAddr + SMC_DIRECT_CMD_OFFSET, (CmdType << SMC_DIRECT_CMD_CMD_TYPE_SHIFT) |
              ChipNum << SMC_DIRECT_CMD_CHIP_NMBR_SHIFT);
}

static inline AL_VOID AlSmc_ll_SetCycles(AL_REG SmcBaseAddr, AL_SMC_CyclesStruct Cycles)
{
    AL_REG32_WRITE(SmcBaseAddr + SMC_SET_CYCLES_OFFSET, Cycles.d32);
}

static inline AL_VOID AlSmc_ll_SetMemWidth(AL_REG SmcBaseAddr, AL_SMC_MemWidthEnum MemWidth)
{
    AL_REG32_SET_BITS(SmcBaseAddr + SMC_SET_OPMODE_OFFSET, SMC_SET_OPMODE_SET_MW_SHIFT, SMC_SET_OPMODE_SET_MW_SIZE, MemWidth);
}

static inline AL_SMC_EccBusyEnum AlSmc_ll_IsEccBusy(AL_REG SmcBaseAddr)
{
    return AL_REG32_GET_BIT(SmcBaseAddr + SMC_ECC1_STATUS_OFFSET, SMC_ECC1_STATUS_ECC_STATUS_SHIFT);
}

static inline AL_VOID AlSmc_ll_SetEccPageSize(AL_REG SmcBaseAddr, AL_SMC_EccPageSizeEnum PageSize)
{
    AL_REG32_SET_BITS(SmcBaseAddr + SMC_ECC1_CFG_OFFSET, SMC_ECC1_CFG_PAGE_SIZE_SHIFT, SMC_ECC1_CFG_PAGE_SIZE_SIZE, PageSize);
}

static inline AL_VOID AlSmc_ll_SetEccMode(AL_REG SmcBaseAddr, AL_SMC_EccModeEnum EccMode)
{
    AL_REG32_SET_BITS(SmcBaseAddr + SMC_ECC1_CFG_OFFSET, SMC_ECC1_CFG_ECC_MODE_SHIFT, SMC_ECC1_CFG_ECC_MODE_SIZE, EccMode);
}

static inline AL_VOID AlSmc_ll_SetEccReadEnd(AL_REG SmcBaseAddr, AL_SMC_EccReadEndEnum EccReadEnd)
{
    AL_REG32_SET_BITS(SmcBaseAddr + SMC_ECC1_CFG_OFFSET, SMC_ECC1_CFG_ECC_READ_END_SHIFT, SMC_ECC1_CFG_ECC_READ_END_SIZE, EccReadEnd);
}

static inline AL_VOID AlSmc_ll_SetEccJump(AL_REG SmcBaseAddr, AL_SMC_EccJumpEnum EccJump)
{
    AL_REG32_SET_BITS(SmcBaseAddr + SMC_ECC1_CFG_OFFSET, SMC_ECC1_CFG_ECC_JUMP_SHIFT, SMC_ECC1_CFG_ECC_JUMP_SIZE, EccJump);
}

static inline AL_VOID AlSmc_ll_SetEccCmd0(AL_REG SmcBaseAddr, AL_U32 EccCmd0)
{
    AL_REG32_WRITE(SmcBaseAddr + SMC_ECC1_MEMCMD0_OFFSET, EccCmd0);
}

static inline AL_VOID AlSmc_ll_SetEccCmd1(AL_REG SmcBaseAddr, AL_U32 EccCmd1)
{
    AL_REG32_WRITE(SmcBaseAddr + SMC_ECC1_MEMCMD1_OFFSET, EccCmd1);
}

static inline AL_U32 AlSmc_ll_ReadEccValue(AL_REG SmcBaseAddr, AL_U32 Offset)
{
    return AL_REG32_READ(SmcBaseAddr + SMC_ECC1_BLOCK0_OFFSET + Offset * 4);
}



#ifdef __cplusplus
}
#endif

#endif
