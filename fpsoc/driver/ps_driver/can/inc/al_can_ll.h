#ifndef AL_CAN_LL_H
#define AL_CAN_LL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "al_reg_io.h"
#include "al_can_hw.h"

typedef enum
{
    AL_CAN_TBSEL_PTB = 0,
    AL_CAN_TBSEL_STB = 1
} AL_CAN_TbselEnum;

typedef enum
{
    AL_CAN_TSSTAT_EMPTY                   = 0,
    AL_CAN_TSSTAT_LESS_OR_EQUAL_HALF_FULL = 1,
    AL_CAN_TSSTAT_MORE_THAN_HALF          = 2,
    AL_CAN_TSSTAT_FULL                    = 3
} AL_CAN_TsstatEnum;

typedef enum
{
    AL_CAN_TTTBM_TTCAN_PTB_STB = 0,
    AL_CAN_TTTBM_FULL_TTCAN    = 1
} AL_CAN_TttbmEnum;

typedef enum
{
    AL_CAN_TSMODE_FIFO     = 0,
    AL_CAN_TSMODE_PRIORITY = 1
} AL_CAN_TsmodeEnum;

typedef enum
{
    AL_CAN_TSNEXT_NO_ACTION        = 0,
    AL_CAN_TSNEXT_STB_SLOT_FILLED  = 1
} AL_CAN_TsnextEnum;

typedef enum
{
    AL_CAN_FD_ISO_NONE     = 0,
    AL_CAN_FD_ISO_11898    = 1
} AL_CAN_FdIsoEnum;

typedef enum
{
    AL_CAN_RSTAT_EMPTY                 = 0,
    AL_CAN_RSTAT_LESS_THAN_ALMOST_FULL = 1,
    AL_CAN_RSTAT_NOT_FULL              = 2,
    AL_CAN_RSTAT_FULL                  = 3
} AL_CAN_RstatEnum;

typedef enum
{
    AL_CAN_RBALL_NORMAL    = 0,
    AL_CAN_RBALL_STORE_ALL = 1
} AL_CAN_RballEnum;

typedef enum
{
    AL_CAN_RREL_NO_RELEASE = 0,
    AL_CAN_RREL_RELEASE    = 1
} AL_CAN_RrelEnum;

typedef enum
{
    AL_CAN_ROM_NOT_STORE_NEW       = 0,
    AL_CAN_ROM_OVERWRITE_OLDEST    = 1
} AL_CAN_RomEnum;

typedef enum
{
    AL_CAN_SACK_NO_SELF_ACK    = 0,
    AL_CAN_SACK_SELF_ACK       = 1
} AL_CAN_SackEnum;

typedef enum
{
    AL_CAN_TSFF_FILLED = 0,
    AL_CAN_TSFF_EMPTY  = 1
} AL_CAN_TsffEnum;

typedef enum
{
    AL_CAN_SELMASK_ACCEPT_CODE = 0,
    AL_CAN_SELMASK_ACCEPT_MASK = 1
} AL_CAN_SelmaskEnum;

typedef enum
{
    AL_CAN_TIMEPOS_SOF = 0,
    AL_CAN_TIMEPOS_EOF = 1
} AL_CAN_TimeposEnum;

typedef enum
{
    AL_CAN_TPRESC_1 = 0,
    AL_CAN_TPRESC_2 = 1,
    AL_CAN_TPRESC_4 = 2,
    AL_CAN_TPRESC_8 = 3
} AL_CAN_TprescEnum;

typedef enum
{
    AL_CAN_TTYPE_IMMEDIATE_TRIGGER         = 0,
    AL_CAN_TTYPE_TIME_TRIGGER              = 1,
    AL_CAN_TTYPE_SINGLE_SHOT_TRANS_TRIGGER = 2,
    AL_CAN_TTYPE_TRANS_START_TRIGGER       = 3,
    AL_CAN_TTYPE_TRANS_STOP_TRIGGER        = 4
} AL_CAN_TtypeEnum;

// static inline AL_BOOL AlCan_ll_IsFdEnabled(AL_REG BaseAddr)
// {
//     return (AL_BOOL)AL_REG32_GET_BIT(CAN_TOP_NS_CFG_CTRL, ((BaseAddr == CAN_CAN0_BASE_ADDR) ?
//                             CAN_TOP_NS_CFG_CTRL_CAN0_FD_ENABLE_SHIFT : CAN_TOP_NS_CFG_CTRL_CAN1_FD_ENABLE_SHIFT));
// }

static inline AL_VOID AlCan_ll_SetFdEnabled(AL_REG BaseAddr, AL_BOOL IsEnabled)
{
    AL_REG32_SET_BIT(CAN_TOP_NS_CFG_CTRL, ((BaseAddr == CAN_CAN0_BASE_ADDR) ? CAN_TOP_NS_CFG_CTRL_CAN0_FD_ENABLE_SHIFT :
            CAN_TOP_NS_CFG_CTRL_CAN1_FD_ENABLE_SHIFT), IsEnabled);
}

// static inline AL_BOOL AlCan_ll_IsDmaEnabled(AL_REG BaseAddr)
// {
//     return (AL_BOOL)AL_REG32_GET_BIT(CAN_TOP_NS_CFG_CTRL, ((BaseAddr == CAN_CAN0_BASE_ADDR) ?
//                             CAN_TOP_NS_CFG_CTRL_CAN0_DMA_ENABLE_SHIFT : CAN_TOP_NS_CFG_CTRL_CAN1_DMA_ENABLE_SHIFT));
// }

// static inline AL_VOID AlCan_ll_SetDmaEnabled(AL_REG BaseAddr, AL_BOOL IsEnabled)
// {
//     AL_REG32_SET_BIT(CAN_TOP_NS_CFG_CTRL, ((BaseAddr == CAN_CAN0_BASE_ADDR) ? CAN_TOP_NS_CFG_CTRL_CAN0_DMA_ENABLE_SHIFT :
//             CAN_TOP_NS_CFG_CTRL_CAN1_DMA_ENABLE_SHIFT), IsEnabled);
// }

// static inline AL_BOOL AlCan_ll_IsBusOff(AL_REG BaseAddr)
// {
//     return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + CAN_RCTRL_TCTRL_TCMD_CFG_STAT_OFFSET,
//                             CAN_RCTRL_TCTRL_TCMD_CFG_STAT_BUSOFF_SHIFT);
// }

// static inline AL_VOID AlCan_ll_SetBusOff(AL_REG BaseAddr, AL_BOOL IsEnabled)
// {
//     AL_REG32_SET_BIT(BaseAddr + CAN_RCTRL_TCTRL_TCMD_CFG_STAT_OFFSET, CAN_RCTRL_TCTRL_TCMD_CFG_STAT_BUSOFF_SHIFT,
//             IsEnabled);
// }

// /* Transmission Secondary Single Shot(TSSS) mode for STB */
// static inline AL_BOOL AlCan_ll_IsTsssEnabled(AL_REG BaseAddr)
// {
//     return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + CAN_RCTRL_TCTRL_TCMD_CFG_STAT_OFFSET,
//                             CAN_RCTRL_TCTRL_TCMD_CFG_STAT_TSSS_SHIFT);
// }

static inline AL_VOID AlCan_ll_SetTsss(AL_REG BaseAddr, AL_BOOL IsEnabled)
{
    AL_REG32_SET_BIT(BaseAddr + CAN_RCTRL_TCTRL_TCMD_CFG_STAT_OFFSET, CAN_RCTRL_TCTRL_TCMD_CFG_STAT_TSSS_SHIFT,
            IsEnabled);
}

// /* Transmission Primary Single Shot(TPSS) mode for PTB */
// static inline AL_BOOL AlCan_ll_IsTpssEnabled(AL_REG BaseAddr)
// {
//     return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + CAN_RCTRL_TCTRL_TCMD_CFG_STAT_OFFSET,
//                             CAN_RCTRL_TCTRL_TCMD_CFG_STAT_TPSS_SHIFT);
// }

static inline AL_VOID AlCan_ll_SetTpss(AL_REG BaseAddr, AL_BOOL IsEnabled)
{
    AL_REG32_SET_BIT(BaseAddr + CAN_RCTRL_TCTRL_TCMD_CFG_STAT_OFFSET, CAN_RCTRL_TCTRL_TCMD_CFG_STAT_TPSS_SHIFT,
            IsEnabled);
}

// /* Loop Back Mode, Internal(LBMI) */
// static inline AL_BOOL AlCan_ll_IsLbmiEnabled(AL_REG BaseAddr)
// {
//     return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + CAN_RCTRL_TCTRL_TCMD_CFG_STAT_OFFSET,
//                             CAN_RCTRL_TCTRL_TCMD_CFG_STAT_LBMI_SHIFT);
// }

static inline AL_VOID AlCan_ll_SetLbmi(AL_REG BaseAddr, AL_BOOL IsEnabled)
{
    AL_REG32_SET_BIT(BaseAddr + CAN_RCTRL_TCTRL_TCMD_CFG_STAT_OFFSET, CAN_RCTRL_TCTRL_TCMD_CFG_STAT_LBMI_SHIFT,
            IsEnabled);
}

// /* Loop Back Mode, External(LBME) */
// static inline AL_BOOL AlCan_ll_IsLbmeEnabled(AL_REG BaseAddr)
// {
//     return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + CAN_RCTRL_TCTRL_TCMD_CFG_STAT_OFFSET,
//                             CAN_RCTRL_TCTRL_TCMD_CFG_STAT_LBME_SHIFT);
// }

static inline AL_VOID AlCan_ll_SetLbme(AL_REG BaseAddr, AL_BOOL IsEnabled)
{
    AL_REG32_SET_BIT(BaseAddr + CAN_RCTRL_TCTRL_TCMD_CFG_STAT_OFFSET, CAN_RCTRL_TCTRL_TCMD_CFG_STAT_LBME_SHIFT,
            IsEnabled);
}

// static inline AL_BOOL AlCan_ll_IsCanCtrlReset(AL_REG BaseAddr)
// {
//     return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + CAN_RCTRL_TCTRL_TCMD_CFG_STAT_OFFSET,
//                             CAN_RCTRL_TCTRL_TCMD_CFG_STAT_RESET_SHIFT);
// }

static inline AL_VOID AlCan_ll_SetCanCtrlReset(AL_REG BaseAddr, AL_BOOL IsEnabled)
{
    AL_REG32_SET_BIT(BaseAddr + CAN_RCTRL_TCTRL_TCMD_CFG_STAT_OFFSET, CAN_RCTRL_TCTRL_TCMD_CFG_STAT_RESET_SHIFT,
            IsEnabled);
}

// /* Transmit Secondary Abort(TSA), set to 1 but can not reset it to 0 */
// static inline AL_BOOL AlCan_ll_IsTsa(AL_REG BaseAddr)
// {
//     return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + CAN_RCTRL_TCTRL_TCMD_CFG_STAT_OFFSET,
//                             CAN_RCTRL_TCTRL_TCMD_CFG_STAT_TSA_SHIFT);
// }

// static inline AL_VOID AlCan_ll_SetTsa(AL_REG BaseAddr)
// {
//     AL_REG32_SET_BIT(BaseAddr + CAN_RCTRL_TCTRL_TCMD_CFG_STAT_OFFSET, CAN_RCTRL_TCTRL_TCMD_CFG_STAT_RESET_SHIFT,
//             AL_TRUE);
// }

// /* Transmit Secondary ALL frames(TSALL), set to 1 but can not reset it to 0 */
// static inline AL_BOOL AlCan_ll_IsTsall(AL_REG BaseAddr)
// {
//     return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + CAN_RCTRL_TCTRL_TCMD_CFG_STAT_OFFSET,
//                             CAN_RCTRL_TCTRL_TCMD_CFG_STAT_TSALL_SHIFT);
// }

// static inline AL_VOID AlCan_ll_SetTsall(AL_REG BaseAddr)
// {
//     AL_REG32_SET_BIT(BaseAddr + CAN_RCTRL_TCTRL_TCMD_CFG_STAT_OFFSET, CAN_RCTRL_TCTRL_TCMD_CFG_STAT_TSALL_SHIFT,
//             AL_TRUE);
// }

// /* Transmit Secondary ONE frame(TSONE), set to 1 but can not reset it to 0 */
// static inline AL_BOOL AlCan_ll_IsTsone(AL_REG BaseAddr)
// {
//     return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + CAN_RCTRL_TCTRL_TCMD_CFG_STAT_OFFSET,
//                             CAN_RCTRL_TCTRL_TCMD_CFG_STAT_TSONE_SHIFT);
// }

static inline AL_VOID AlCan_ll_SetTsone(AL_REG BaseAddr)
{
    AL_REG32_SET_BIT(BaseAddr + CAN_RCTRL_TCTRL_TCMD_CFG_STAT_OFFSET, CAN_RCTRL_TCTRL_TCMD_CFG_STAT_TSONE_SHIFT,
            AL_TRUE);
}

// /* Transmit Primary Abort(TPA), set to 1 but can not reset it to 0 */
// static inline AL_BOOL AlCan_ll_IsTpa(AL_REG BaseAddr)
// {
//     return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + CAN_RCTRL_TCTRL_TCMD_CFG_STAT_OFFSET,
//                             CAN_RCTRL_TCTRL_TCMD_CFG_STAT_TPA_SHIFT);
// }

// static inline AL_VOID AlCan_ll_SetTpa(AL_REG BaseAddr)
// {
//     AL_REG32_SET_BIT(BaseAddr + CAN_RCTRL_TCTRL_TCMD_CFG_STAT_OFFSET, CAN_RCTRL_TCTRL_TCMD_CFG_STAT_TPA_SHIFT,
//             AL_TRUE);
// }

// /* Transmit Primary Enable(TPE), set to 1 but can not reset it to 0 */
// static inline AL_BOOL AlCan_ll_IsTpe(AL_REG BaseAddr)
// {
//     return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + CAN_RCTRL_TCTRL_TCMD_CFG_STAT_OFFSET,
//                             CAN_RCTRL_TCTRL_TCMD_CFG_STAT_TPE_SHIFT);
// }

static inline AL_VOID AlCan_ll_SetTpe(AL_REG BaseAddr)
{
    AL_REG32_SET_BIT(BaseAddr + CAN_RCTRL_TCTRL_TCMD_CFG_STAT_OFFSET, CAN_RCTRL_TCTRL_TCMD_CFG_STAT_TPE_SHIFT,
            AL_TRUE);
}

/* Transceiver Standby Mode(STBY) */
static inline AL_BOOL AlCan_ll_IsStby(AL_REG BaseAddr)
{
    return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + CAN_RCTRL_TCTRL_TCMD_CFG_STAT_OFFSET,
                            CAN_RCTRL_TCTRL_TCMD_CFG_STAT_STBY_SHIFT);
}

static inline AL_VOID AlCan_ll_SetStby(AL_REG BaseAddr, AL_BOOL IsEnabled)
{
    AL_REG32_SET_BIT(BaseAddr + CAN_RCTRL_TCTRL_TCMD_CFG_STAT_OFFSET, CAN_RCTRL_TCTRL_TCMD_CFG_STAT_STBY_SHIFT,
            IsEnabled);
}

// /* Listen Only Mode(LOM) */
// static inline AL_BOOL AlCan_ll_IsLom(AL_REG BaseAddr)
// {
//     return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + CAN_RCTRL_TCTRL_TCMD_CFG_STAT_OFFSET,
//                             CAN_RCTRL_TCTRL_TCMD_CFG_STAT_LOM_SHIFT);
// }

static inline AL_VOID AlCan_ll_SetLom(AL_REG BaseAddr, AL_BOOL IsEnabled)
{
    AL_REG32_SET_BIT(BaseAddr + CAN_RCTRL_TCTRL_TCMD_CFG_STAT_OFFSET, CAN_RCTRL_TCTRL_TCMD_CFG_STAT_LOM_SHIFT,
            IsEnabled);
}

// /* Transmit Buffer Select(TBSEL) */
// static inline AL_CAN_TbselEnum AlCan_ll_GetTbsel(AL_REG BaseAddr)
// {
//     return (AL_CAN_TbselEnum)AL_REG32_GET_BIT(BaseAddr + CAN_RCTRL_TCTRL_TCMD_CFG_STAT_OFFSET,
//                                      CAN_RCTRL_TCTRL_TCMD_CFG_STAT_TBSEL_SHIFT);
// }

static inline AL_VOID AlCan_ll_SetTbsel(AL_REG BaseAddr, AL_CAN_TbselEnum TransBuffer)
{
    AL_REG32_SET_BIT(BaseAddr + CAN_RCTRL_TCTRL_TCMD_CFG_STAT_OFFSET, CAN_RCTRL_TCTRL_TCMD_CFG_STAT_TBSEL_SHIFT,
            TransBuffer);
}

// /* Transmission Secondary Status(TSSTAT) */
// static inline AL_CAN_TsstatEnum AlCan_ll_GetTsstat(AL_REG BaseAddr)
// {
//     return (AL_CAN_TsstatEnum)AL_REG32_GET_BITS(BaseAddr + CAN_RCTRL_TCTRL_TCMD_CFG_STAT_OFFSET,
//                                        CAN_RCTRL_TCTRL_TCMD_CFG_STAT_TSSTAT_SHIFT,
//                                        CAN_RCTRL_TCTRL_TCMD_CFG_STAT_TSSTAT_SIZE);
// }

// /* TTCAN Transmit Buffer Mode(TTTBM) */
// static inline AL_CAN_TttbmEnum AlCan_ll_GetTttbm(AL_REG BaseAddr)
// {
//     return (AL_CAN_TttbmEnum)AL_REG32_GET_BIT(BaseAddr + CAN_RCTRL_TCTRL_TCMD_CFG_STAT_OFFSET,
//                                      CAN_RCTRL_TCTRL_TCMD_CFG_STAT_TTTBM_SHIFT);
// }

// static inline AL_VOID AlCan_ll_SetTttbm(AL_REG BaseAddr, AL_CAN_TttbmEnum Mode)
// {
//     AL_REG32_SET_BIT(BaseAddr + CAN_RCTRL_TCTRL_TCMD_CFG_STAT_OFFSET, CAN_RCTRL_TCTRL_TCMD_CFG_STAT_TTTBM_SHIFT,
//             Mode);
// }

// /* Transmit buffer Secondary operation Mode(TSMODE) */
// static inline AL_CAN_TsmodeEnum AlCan_ll_GetTsmode(AL_REG BaseAddr)
// {
//     return (AL_CAN_TsmodeEnum)AL_REG32_GET_BIT(BaseAddr + CAN_RCTRL_TCTRL_TCMD_CFG_STAT_OFFSET,
//                                       CAN_RCTRL_TCTRL_TCMD_CFG_STAT_TSMODE_SHIFT);
// }

static inline AL_VOID AlCan_ll_SetTsmode(AL_REG BaseAddr, AL_CAN_TsmodeEnum Mode)
{
    AL_REG32_SET_BIT(BaseAddr + CAN_RCTRL_TCTRL_TCMD_CFG_STAT_OFFSET, CAN_RCTRL_TCTRL_TCMD_CFG_STAT_TSMODE_SHIFT,
            Mode);
}

// /* Transmit buffer Secondary NEXT(TSNEXT) */
// static inline AL_CAN_TsnextEnum AlCan_ll_GetTsnext(AL_REG BaseAddr)
// {
//     return (AL_CAN_TsnextEnum)AL_REG32_GET_BIT(BaseAddr + CAN_RCTRL_TCTRL_TCMD_CFG_STAT_OFFSET,
//                                       CAN_RCTRL_TCTRL_TCMD_CFG_STAT_TSNEXT_SHIFT);
// }

// static inline AL_VOID AlCan_ll_SetTsnext(AL_REG BaseAddr, AL_CAN_TsnextEnum Mode)
// {
//     AL_REG32_SET_BIT(BaseAddr + CAN_RCTRL_TCTRL_TCMD_CFG_STAT_OFFSET,  CAN_RCTRL_TCTRL_TCMD_CFG_STAT_TSNEXT_SHIFT,
//             Mode);
// }

// /* CAN FD ISO mode(FD_ISO) */
// static inline AL_CAN_FdIsoEnum AlCan_ll_GetFdIso(AL_REG BaseAddr)
// {
//     return (AL_CAN_FdIsoEnum)AL_REG32_GET_BIT(BaseAddr + CAN_RCTRL_TCTRL_TCMD_CFG_STAT_OFFSET,
//                                      CAN_RCTRL_TCTRL_TCMD_CFG_STAT_FD_ISO_SHIFT);
// }

// static inline AL_VOID AlCan_ll_SetFdIso(AL_REG BaseAddr, AL_CAN_FdIsoEnum Mode)
// {
//     AL_REG32_SET_BIT(BaseAddr + CAN_RCTRL_TCTRL_TCMD_CFG_STAT_OFFSET, CAN_RCTRL_TCTRL_TCMD_CFG_STAT_FD_ISO_SHIFT,
//             Mode);
// }

/* Receive buffer Status(RSTAT) */
static inline AL_CAN_RstatEnum AlCan_ll_GetRstat(AL_REG BaseAddr)
{
    return (AL_CAN_RstatEnum)AL_REG32_GET_BITS(BaseAddr + CAN_RCTRL_TCTRL_TCMD_CFG_STAT_OFFSET,
                                      CAN_RCTRL_TCTRL_TCMD_CFG_STAT_RSTAT_SHIFT,
                                      CAN_RCTRL_TCTRL_TCMD_CFG_STAT_RSTAT_SIZE);
}

// /* Receive Buffer stores ALL data frames(RBALL) */
// static inline AL_CAN_RballEnum AlCan_ll_GetRball(AL_REG BaseAddr)
// {
//     return (AL_CAN_RballEnum)AL_REG32_GET_BIT(BaseAddr + CAN_RCTRL_TCTRL_TCMD_CFG_STAT_OFFSET,
//                                      CAN_RCTRL_TCTRL_TCMD_CFG_STAT_RBALL_SHIFT);
// }

// static inline AL_VOID AlCan_ll_SetRball(AL_REG BaseAddr, AL_CAN_RballEnum Mode)
// {
//     AL_REG32_SET_BIT(BaseAddr + CAN_RCTRL_TCTRL_TCMD_CFG_STAT_OFFSET, CAN_RCTRL_TCTRL_TCMD_CFG_STAT_RBALL_SHIFT,
//             Mode);
// }

// /* Receive buffer Release(RREL) */
// static inline AL_CAN_RrelEnum AlCan_ll_GetRrel(AL_REG BaseAddr)
// {
//     return (AL_CAN_RrelEnum)AL_REG32_GET_BIT(BaseAddr + CAN_RCTRL_TCTRL_TCMD_CFG_STAT_OFFSET,
//                                     CAN_RCTRL_TCTRL_TCMD_CFG_STAT_RREL_SHIFT);
// }

static inline AL_VOID AlCan_ll_SetRrel(AL_REG BaseAddr, AL_CAN_RrelEnum Mode)
{
    AL_REG32_SET_BIT(BaseAddr + CAN_RCTRL_TCTRL_TCMD_CFG_STAT_OFFSET, CAN_RCTRL_TCTRL_TCMD_CFG_STAT_RREL_SHIFT, Mode);
}

// /* Receive buffer Overflow(ROV) */
// static inline AL_BOOL AlCan_ll_GetRov(AL_REG BaseAddr)
// {
//     return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + CAN_RCTRL_TCTRL_TCMD_CFG_STAT_OFFSET,
//                             CAN_RCTRL_TCTRL_TCMD_CFG_STAT_ROV_SHIFT);
// }

// /* Receive buffer Overflow Mode(ROM) */
// static inline AL_CAN_RomEnum AlCan_ll_GetRom(AL_REG BaseAddr)
// {
//     return (AL_CAN_RomEnum)AL_REG32_GET_BIT(BaseAddr + CAN_RCTRL_TCTRL_TCMD_CFG_STAT_OFFSET,
//                                    CAN_RCTRL_TCTRL_TCMD_CFG_STAT_ROM_SHIFT);
// }

// static inline AL_VOID AlCan_ll_SetRom(AL_REG BaseAddr, AL_CAN_RomEnum Mode)
// {
//     AL_REG32_SET_BIT(BaseAddr + CAN_RCTRL_TCTRL_TCMD_CFG_STAT_OFFSET, CAN_RCTRL_TCTRL_TCMD_CFG_STAT_ROM_SHIFT, Mode);
// }

// /* Self-ACKnowledge(SACK) */
// static inline AL_CAN_SackEnum AlCan_ll_GetSack(AL_REG BaseAddr)
// {
//     return (AL_CAN_SackEnum)AL_REG32_GET_BIT(BaseAddr + CAN_RCTRL_TCTRL_TCMD_CFG_STAT_OFFSET,
//                                     CAN_RCTRL_TCTRL_TCMD_CFG_STAT_SACK_SHIFT);
// }

// static inline AL_VOID AlCan_ll_SetSack(AL_REG BaseAddr, AL_CAN_SackEnum Mode)
// {
//     AL_REG32_SET_BIT(BaseAddr + CAN_RCTRL_TCTRL_TCMD_CFG_STAT_OFFSET, CAN_RCTRL_TCTRL_TCMD_CFG_STAT_SACK_SHIFT, Mode);
// }

// /* Transmit Secondary buffer Full Flag(TSFF) */
// static inline AL_CAN_TsffEnum AlCan_ll_GetTsff(AL_REG BaseAddr)
// {
//     return (AL_CAN_TsffEnum)AL_REG32_GET_BIT(BaseAddr + CAN_LIMIT_ERRINT_RTIF_RTIE_OFFSET,
//                                     CAN_LIMIT_ERRINT_RTIF_RTIE_TSFF_SHIFT);
// }

// /* Error Interrupt Enable(EIE) */
// static inline AL_BOOL AlCan_ll_IsEie(AL_REG BaseAddr)
// {
//     return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + CAN_LIMIT_ERRINT_RTIF_RTIE_OFFSET,
//                             CAN_LIMIT_ERRINT_RTIF_RTIE_EIE_SHIFT);
// }

// static inline AL_VOID AlCan_ll_SetEie(AL_REG BaseAddr, AL_BOOL IsEnabled)
// {
//     AL_REG32_SET_BIT(BaseAddr + CAN_LIMIT_ERRINT_RTIF_RTIE_OFFSET, CAN_LIMIT_ERRINT_RTIF_RTIE_EIE_SHIFT, IsEnabled);
// }

// /* Transmission Secondary Interrupt Enable(TSIE) */
// static inline AL_BOOL AlCan_ll_IsTsie(AL_REG BaseAddr)
// {
//     return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + CAN_LIMIT_ERRINT_RTIF_RTIE_OFFSET,
//                             CAN_LIMIT_ERRINT_RTIF_RTIE_TSIE_SHIFT);
// }

// static inline AL_VOID AlCan_ll_SetTsie(AL_REG BaseAddr, AL_BOOL IsEnabled)
// {
//     AL_REG32_SET_BIT(BaseAddr + CAN_LIMIT_ERRINT_RTIF_RTIE_OFFSET, CAN_LIMIT_ERRINT_RTIF_RTIE_TSIE_SHIFT, IsEnabled);
// }

// /* Transmission Primary Interrupt Enable(TPIE) */
// static inline AL_BOOL AlCan_ll_IsTpie(AL_REG BaseAddr)
// {
//     return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + CAN_LIMIT_ERRINT_RTIF_RTIE_OFFSET,
//                             CAN_LIMIT_ERRINT_RTIF_RTIE_TPIE_SHIFT);
// }

// static inline AL_VOID AlCan_ll_SetTpie(AL_REG BaseAddr, AL_BOOL IsEnabled)
// {
//     AL_REG32_SET_BIT(BaseAddr + CAN_LIMIT_ERRINT_RTIF_RTIE_OFFSET, CAN_LIMIT_ERRINT_RTIF_RTIE_TPIE_SHIFT, IsEnabled);
// }

// /* RB Almost Full Interrupt Enable(RAFIE) */
// static inline AL_BOOL AlCan_ll_IsRafie(AL_REG BaseAddr)
// {
//     return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + CAN_LIMIT_ERRINT_RTIF_RTIE_OFFSET,
//                             CAN_LIMIT_ERRINT_RTIF_RTIE_RAFIE_SHIFT);
// }

// static inline AL_VOID AlCan_ll_SetRafie(AL_REG BaseAddr, AL_BOOL IsEnabled)
// {
//     AL_REG32_SET_BIT(BaseAddr + CAN_LIMIT_ERRINT_RTIF_RTIE_OFFSET, CAN_LIMIT_ERRINT_RTIF_RTIE_RAFIE_SHIFT, IsEnabled);
// }

// /* RB Full Interrupt Enable(RFIE) */
// static inline AL_BOOL AlCan_ll_IsRfie(AL_REG BaseAddr)
// {
//     return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + CAN_LIMIT_ERRINT_RTIF_RTIE_OFFSET,
//                             CAN_LIMIT_ERRINT_RTIF_RTIE_RFIE_SHIFT);
// }

// static inline AL_VOID AlCan_ll_SetRfie(AL_REG BaseAddr, AL_BOOL IsEnabled)
// {
//     AL_REG32_SET_BIT(BaseAddr + CAN_LIMIT_ERRINT_RTIF_RTIE_OFFSET, CAN_LIMIT_ERRINT_RTIF_RTIE_RFIE_SHIFT, IsEnabled);
// }

// /* RB Overrun Interrupt Enable(ROIE) */
// static inline AL_BOOL AlCan_ll_IsRoie(AL_REG BaseAddr)
// {
//     return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + CAN_LIMIT_ERRINT_RTIF_RTIE_OFFSET,
//                             CAN_LIMIT_ERRINT_RTIF_RTIE_ROIE_SHIFT);
// }

// static inline AL_VOID AlCan_ll_SetRoie(AL_REG BaseAddr, AL_BOOL IsEnabled)
// {
//     AL_REG32_SET_BIT(BaseAddr + CAN_LIMIT_ERRINT_RTIF_RTIE_OFFSET, CAN_LIMIT_ERRINT_RTIF_RTIE_ROIE_SHIFT, IsEnabled);
// }

// /* Receive Interrupt Enable(RIE) */
// static inline AL_BOOL AlCan_ll_IsRie(AL_REG BaseAddr)
// {
//     return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + CAN_LIMIT_ERRINT_RTIF_RTIE_OFFSET,
//                             CAN_LIMIT_ERRINT_RTIF_RTIE_RIE_SHIFT);
// }

// static inline AL_VOID AlCan_ll_SetRie(AL_REG BaseAddr, AL_BOOL IsEnabled)
// {
//     AL_REG32_SET_BIT(BaseAddr + CAN_LIMIT_ERRINT_RTIF_RTIE_OFFSET, CAN_LIMIT_ERRINT_RTIF_RTIE_RIE_SHIFT, IsEnabled);
// }

// /* Abort Interrupt Flag(AIF) */
// static inline AL_BOOL AlCan_ll_GetAif(AL_REG BaseAddr)
// {
//     return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + CAN_LIMIT_ERRINT_RTIF_RTIE_OFFSET,
//                             CAN_LIMIT_ERRINT_RTIF_RTIE_AIF_SHIFT);
// }

// static inline AL_VOID AlCan_ll_ClrAif(AL_REG BaseAddr)
// {
//     AL_REG32_SET_BIT(BaseAddr + CAN_LIMIT_ERRINT_RTIF_RTIE_OFFSET, CAN_LIMIT_ERRINT_RTIF_RTIE_AIF_SHIFT, AL_TRUE);
// }

// /* Error Interrupt Flag(EIF) */
// static inline AL_BOOL AlCan_ll_GetEif(AL_REG BaseAddr)
// {
//     return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + CAN_LIMIT_ERRINT_RTIF_RTIE_OFFSET,
//                             CAN_LIMIT_ERRINT_RTIF_RTIE_EIF_SHIFT);
// }

// static inline AL_VOID AlCan_ll_ClrEif(AL_REG BaseAddr)
// {
//     AL_REG32_SET_BIT(BaseAddr + CAN_LIMIT_ERRINT_RTIF_RTIE_OFFSET,
//             CAN_LIMIT_ERRINT_RTIF_RTIE_EIF_SHIFT, AL_TRUE);
// }

// /* Transmission Secondary Interrupt Flag(TSIF) */
// static inline AL_BOOL AlCan_ll_GetTsif(AL_REG BaseAddr)
// {
//     return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + CAN_LIMIT_ERRINT_RTIF_RTIE_OFFSET,
//                             CAN_LIMIT_ERRINT_RTIF_RTIE_TSIF_SHIFT);
// }

// static inline AL_VOID AlCan_ll_ClrTsif(AL_REG BaseAddr)
// {
//     AL_REG32_SET_BIT(BaseAddr + CAN_LIMIT_ERRINT_RTIF_RTIE_OFFSET, CAN_LIMIT_ERRINT_RTIF_RTIE_TSIF_SHIFT, AL_TRUE);
// }

// /* Transmission Primary Interrupt Flag(TPIF) */
// static inline AL_BOOL AlCan_ll_GetTpif(AL_REG BaseAddr)
// {
//     return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + CAN_LIMIT_ERRINT_RTIF_RTIE_OFFSET,
//                             CAN_LIMIT_ERRINT_RTIF_RTIE_TPIF_SHIFT);
// }

// static inline AL_VOID AlCan_ll_ClrTpif(AL_REG BaseAddr)
// {
//     AL_REG32_SET_BIT(BaseAddr + CAN_LIMIT_ERRINT_RTIF_RTIE_OFFSET, CAN_LIMIT_ERRINT_RTIF_RTIE_TPIF_SHIFT, AL_TRUE);
// }

// /* RB Almost Full Interrupt Flag(RAFIF) */
// static inline AL_BOOL AlCan_ll_GetRafif(AL_REG BaseAddr)
// {
//     return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + CAN_LIMIT_ERRINT_RTIF_RTIE_OFFSET,
//                             CAN_LIMIT_ERRINT_RTIF_RTIE_RAFIF_SHIFT);
// }

// static inline AL_VOID AlCan_ll_ClrRafif(AL_REG BaseAddr)
// {
//     AL_REG32_SET_BIT(BaseAddr + CAN_LIMIT_ERRINT_RTIF_RTIE_OFFSET, CAN_LIMIT_ERRINT_RTIF_RTIE_RAFIF_SHIFT, AL_TRUE);
// }

// /* RB Full Interrupt Flag(RFIF) */
// static inline AL_BOOL AlCan_ll_GetRfif(AL_REG BaseAddr)
// {
//     return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + CAN_LIMIT_ERRINT_RTIF_RTIE_OFFSET,
//                             CAN_LIMIT_ERRINT_RTIF_RTIE_RFIF_SHIFT);
// }

// static inline AL_VOID AlCan_ll_ClrRfif(AL_REG BaseAddr)
// {
//     AL_REG32_SET_BIT(BaseAddr + CAN_LIMIT_ERRINT_RTIF_RTIE_OFFSET, CAN_LIMIT_ERRINT_RTIF_RTIE_RFIF_SHIFT, AL_TRUE);
// }

// /* RB Overrun Interrupt Flag(ROIF) */
// static inline AL_BOOL AlCan_ll_GetRoif(AL_REG BaseAddr)
// {
//     return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + CAN_LIMIT_ERRINT_RTIF_RTIE_OFFSET,
//                             CAN_LIMIT_ERRINT_RTIF_RTIE_ROIF_SHIFT);
// }

// static inline AL_VOID AlCan_ll_ClrRoif(AL_REG BaseAddr)
// {
//     AL_REG32_SET_BIT(BaseAddr + CAN_LIMIT_ERRINT_RTIF_RTIE_OFFSET, CAN_LIMIT_ERRINT_RTIF_RTIE_ROIF_SHIFT, AL_TRUE);
// }

// /* RB Interrupt Flag(RIF) */
// static inline AL_BOOL AlCan_ll_GetRif(AL_REG BaseAddr)
// {
//     return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + CAN_LIMIT_ERRINT_RTIF_RTIE_OFFSET,
//                             CAN_LIMIT_ERRINT_RTIF_RTIE_RIF_SHIFT);
// }

// static inline AL_VOID AlCan_ll_ClrRif(AL_REG BaseAddr)
// {
//     AL_REG32_SET_BIT(BaseAddr + CAN_LIMIT_ERRINT_RTIF_RTIE_OFFSET, CAN_LIMIT_ERRINT_RTIF_RTIE_RIF_SHIFT, AL_TRUE);
// }

// /* Bus Error Interrupt Flag(BEIF) */
// static inline AL_BOOL AlCan_ll_GetBeif(AL_REG BaseAddr)
// {
//     return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + CAN_LIMIT_ERRINT_RTIF_RTIE_OFFSET,
//                             CAN_LIMIT_ERRINT_RTIF_RTIE_BEIF_SHIFT);
// }

// static inline AL_VOID AlCan_ll_ClrBeif(AL_REG BaseAddr)
// {
//     AL_REG32_SET_BIT(BaseAddr + CAN_LIMIT_ERRINT_RTIF_RTIE_OFFSET, CAN_LIMIT_ERRINT_RTIF_RTIE_BEIF_SHIFT, AL_TRUE);
// }

// /* Bus Error Interrupt Enable(BEIE) */
// static inline AL_BOOL AlCan_ll_IsBeie(AL_REG BaseAddr)
// {
//     return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + CAN_LIMIT_ERRINT_RTIF_RTIE_OFFSET,
//                             CAN_LIMIT_ERRINT_RTIF_RTIE_BEIE_SHIFT);
// }

// static inline AL_VOID AlCan_ll_SetBeie(AL_REG BaseAddr, AL_BOOL IsEnabled)
// {
//     AL_REG32_SET_BIT(BaseAddr + CAN_LIMIT_ERRINT_RTIF_RTIE_OFFSET, CAN_LIMIT_ERRINT_RTIF_RTIE_BEIE_SHIFT, IsEnabled);
// }

// /* Arbitration Lost Interrupt Flag(ALIF) */
// static inline AL_BOOL AlCan_ll_GetAlif(AL_REG BaseAddr)
// {
//     return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + CAN_LIMIT_ERRINT_RTIF_RTIE_OFFSET,
//                             CAN_LIMIT_ERRINT_RTIF_RTIE_ALIF_SHIFT);
// }

// static inline AL_VOID AlCan_ll_ClrAlif(AL_REG BaseAddr)
// {
//     AL_REG32_SET_BIT(BaseAddr + CAN_LIMIT_ERRINT_RTIF_RTIE_OFFSET, CAN_LIMIT_ERRINT_RTIF_RTIE_ALIF_SHIFT, AL_TRUE);
// }

// /* Arbitration Lost Interrupt Enable(ALIE) */
// static inline AL_BOOL AlCan_ll_IsAlie(AL_REG BaseAddr)
// {
//     return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + CAN_LIMIT_ERRINT_RTIF_RTIE_OFFSET,
//                             CAN_LIMIT_ERRINT_RTIF_RTIE_ALIE_SHIFT);
// }

// static inline AL_VOID AlCan_ll_SetAlie(AL_REG BaseAddr, AL_BOOL IsEnabled)
// {
//     AL_REG32_SET_BIT(BaseAddr + CAN_LIMIT_ERRINT_RTIF_RTIE_OFFSET, CAN_LIMIT_ERRINT_RTIF_RTIE_ALIE_SHIFT, IsEnabled);
// }

// /* Error Passive Interrupt Flag(EPIF) */
// static inline AL_BOOL AlCan_ll_GetEpif(AL_REG BaseAddr)
// {
//     return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + CAN_LIMIT_ERRINT_RTIF_RTIE_OFFSET,
//                             CAN_LIMIT_ERRINT_RTIF_RTIE_EPIF_SHIFT);
// }

// static inline AL_VOID AlCan_ll_ClrEpif(AL_REG BaseAddr)
// {
//     AL_REG32_SET_BIT(BaseAddr + CAN_LIMIT_ERRINT_RTIF_RTIE_OFFSET, CAN_LIMIT_ERRINT_RTIF_RTIE_EPIF_SHIFT, AL_TRUE);
// }

// /* Error Passive Interrupt Enable(EPIE) */
// static inline AL_BOOL AlCan_ll_IsEpie(AL_REG BaseAddr)
// {
//     return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + CAN_LIMIT_ERRINT_RTIF_RTIE_OFFSET,
//                             CAN_LIMIT_ERRINT_RTIF_RTIE_EPIE_SHIFT);
// }

// static inline AL_VOID AlCan_ll_SetEpie(AL_REG BaseAddr, AL_BOOL IsEnabled)
// {
//     AL_REG32_SET_BIT(BaseAddr + CAN_LIMIT_ERRINT_RTIF_RTIE_OFFSET, CAN_LIMIT_ERRINT_RTIF_RTIE_EPIE_SHIFT, IsEnabled);
// }

// /* Error Passive mode active(EPASS) */
// static inline AL_BOOL AlCan_ll_GetEpass(AL_REG BaseAddr)
// {
//     return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + CAN_LIMIT_ERRINT_RTIF_RTIE_OFFSET,
//                             CAN_LIMIT_ERRINT_RTIF_RTIE_EPASS_SHIFT);
// }

// /* Error WARNing limit reached(EWARN) */
// static inline AL_BOOL AlCan_ll_GetEwarn(AL_REG BaseAddr)
// {
//     return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + CAN_LIMIT_ERRINT_RTIF_RTIE_OFFSET,
//                             CAN_LIMIT_ERRINT_RTIF_RTIE_EWARN_SHIFT);
// }

// /* Programmable Error Warning Limit(EWL) = (EWL+1)*8. Possible Limit values: 8, 16, … 128 */
// static inline AL_U32 AlCan_ll_GetEwl(AL_REG BaseAddr)
// {
//     return AL_REG32_GET_BITS(BaseAddr + CAN_LIMIT_ERRINT_RTIF_RTIE_OFFSET, CAN_LIMIT_ERRINT_RTIF_RTIE_EWL_SHIFT,
//                     CAN_LIMIT_ERRINT_RTIF_RTIE_EWL_SIZE);
// }

// static inline AL_VOID AlCan_ll_SetEwl(AL_REG BaseAddr, AL_U32 Value)
// {
//     AL_REG32_SET_BITS(BaseAddr + CAN_LIMIT_ERRINT_RTIF_RTIE_OFFSET, CAN_LIMIT_ERRINT_RTIF_RTIE_EWL_SHIFT,
//              CAN_LIMIT_ERRINT_RTIF_RTIE_EWL_SIZE, Value);
// }

// /* receive buffer Almost Full Warning Limit(AFWL) */
// static inline AL_U32 AlCan_ll_GetAfwl(AL_REG BaseAddr)
// {
//     return AL_REG32_GET_BITS(BaseAddr + CAN_LIMIT_ERRINT_RTIF_RTIE_OFFSET, CAN_LIMIT_ERRINT_RTIF_RTIE_AFWL_SHIFT,
//                     CAN_LIMIT_ERRINT_RTIF_RTIE_AFWL_SIZE);
// }

static inline AL_VOID AlCan_ll_SetAfwl(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_SET_BITS(BaseAddr + CAN_LIMIT_ERRINT_RTIF_RTIE_OFFSET, CAN_LIMIT_ERRINT_RTIF_RTIE_AFWL_SHIFT,
             CAN_LIMIT_ERRINT_RTIF_RTIE_AFWL_SIZE, Value);
}

/* Slow Speed Bit Timing Segment 1(S_Seg_1) */
static inline AL_U32 AlCan_ll_GetSseg1(AL_REG BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr + CAN_S_PRESC_S_SJW_S_SEG_2_S_SEG_1_OFFSET,
                    CAN_S_PRESC_S_SJW_S_SEG_2_S_SEG_1_S_SEG_1_SHIFT,
                    CAN_S_PRESC_S_SJW_S_SEG_2_S_SEG_1_S_SEG_1_SIZE);
}

static inline AL_VOID AlCan_ll_SetSseg1(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_SET_BITS(BaseAddr + CAN_S_PRESC_S_SJW_S_SEG_2_S_SEG_1_OFFSET,
             CAN_S_PRESC_S_SJW_S_SEG_2_S_SEG_1_S_SEG_1_SHIFT,
             CAN_S_PRESC_S_SJW_S_SEG_2_S_SEG_1_S_SEG_1_SIZE, (Value - 2));
}

/* Slow Speed Bit Timing Segment 2(S_Seg_2) */
static inline AL_U32 AlCan_ll_GetSseg2(AL_REG BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr + CAN_S_PRESC_S_SJW_S_SEG_2_S_SEG_1_OFFSET,
                    CAN_S_PRESC_S_SJW_S_SEG_2_S_SEG_1_S_SEG_2_SHIFT,
                    CAN_S_PRESC_S_SJW_S_SEG_2_S_SEG_1_S_SEG_2_SIZE);
}

static inline AL_VOID AlCan_ll_SetSseg2(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_SET_BITS(BaseAddr + CAN_S_PRESC_S_SJW_S_SEG_2_S_SEG_1_OFFSET,
             CAN_S_PRESC_S_SJW_S_SEG_2_S_SEG_1_S_SEG_2_SHIFT,
             CAN_S_PRESC_S_SJW_S_SEG_2_S_SEG_1_S_SEG_2_SIZE, (Value - 1));
}

/* Slow Speed Synchronization Jump Width(S_SJW) */
static inline AL_U32 AlCan_ll_GetSsjw(AL_REG BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr + CAN_S_PRESC_S_SJW_S_SEG_2_S_SEG_1_OFFSET,
                    CAN_S_PRESC_S_SJW_S_SEG_2_S_SEG_1_S_SJW_SHIFT,
                    CAN_S_PRESC_S_SJW_S_SEG_2_S_SEG_1_S_SJW_SIZE);
}

static inline AL_VOID AlCan_ll_SetSsjw(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_SET_BITS(BaseAddr + CAN_S_PRESC_S_SJW_S_SEG_2_S_SEG_1_OFFSET,
             CAN_S_PRESC_S_SJW_S_SEG_2_S_SEG_1_S_SJW_SHIFT,
             CAN_S_PRESC_S_SJW_S_SEG_2_S_SEG_1_S_SJW_SIZE, (Value - 1));
}

/* Slow Speed Prescaler Registers(S_PRESC) */
static inline AL_U32 AlCan_ll_GetSpresc(AL_REG BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr + CAN_S_PRESC_S_SJW_S_SEG_2_S_SEG_1_OFFSET,
                    CAN_S_PRESC_S_SJW_S_SEG_2_S_SEG_1_S_PRESC_SHIFT,
                    CAN_S_PRESC_S_SJW_S_SEG_2_S_SEG_1_S_PRESC_SIZE);
}

static inline AL_VOID AlCan_ll_SetSpresc(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_SET_BITS(BaseAddr + CAN_S_PRESC_S_SJW_S_SEG_2_S_SEG_1_OFFSET,
             CAN_S_PRESC_S_SJW_S_SEG_2_S_SEG_1_S_PRESC_SHIFT,
             CAN_S_PRESC_S_SJW_S_SEG_2_S_SEG_1_S_PRESC_SIZE, (Value - 1));
}

/* Fast Speed Bit Timing Segment 1(F_Seg_1) */
static inline AL_U32 AlCan_ll_GetFseg1(AL_REG BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr + CAN_F_PRESC_F_SJW_F_SEG_2_F_SEG_1_OFFSET,
                    CAN_F_PRESC_F_SJW_F_SEG_2_F_SEG_1_F_SEG_1_SHIFT,
                    CAN_F_PRESC_F_SJW_F_SEG_2_F_SEG_1_F_SEG_1_SIZE);
}

static inline AL_VOID AlCan_ll_SetFseg1(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_SET_BITS(BaseAddr + CAN_F_PRESC_F_SJW_F_SEG_2_F_SEG_1_OFFSET,
             CAN_F_PRESC_F_SJW_F_SEG_2_F_SEG_1_F_SEG_1_SHIFT,
             CAN_F_PRESC_F_SJW_F_SEG_2_F_SEG_1_F_SEG_1_SIZE, (Value - 2));
}

/* Fast Speed Bit Timing Segment 2(F_Seg_2) */
static inline AL_U32 AlCan_ll_GetFseg2(AL_REG BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr + CAN_F_PRESC_F_SJW_F_SEG_2_F_SEG_1_OFFSET,
                    CAN_F_PRESC_F_SJW_F_SEG_2_F_SEG_1_F_SEG_2_SHIFT,
                    CAN_F_PRESC_F_SJW_F_SEG_2_F_SEG_1_F_SEG_2_SIZE);
}

static inline AL_VOID AlCan_ll_SetFseg2(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_SET_BITS(BaseAddr + CAN_F_PRESC_F_SJW_F_SEG_2_F_SEG_1_OFFSET,
             CAN_F_PRESC_F_SJW_F_SEG_2_F_SEG_1_F_SEG_2_SHIFT,
             CAN_F_PRESC_F_SJW_F_SEG_2_F_SEG_1_F_SEG_2_SIZE, (Value - 1));
}

/* Fast Speed Synchronization Jump Width(F_SJW) */
static inline AL_U32 AlCan_ll_GetFsjw(AL_REG BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr + CAN_F_PRESC_F_SJW_F_SEG_2_F_SEG_1_OFFSET,
                    CAN_F_PRESC_F_SJW_F_SEG_2_F_SEG_1_F_SJW_SHIFT,
                    CAN_F_PRESC_F_SJW_F_SEG_2_F_SEG_1_F_SJW_SIZE);
}

static inline AL_VOID AlCan_ll_SetFsjw(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_SET_BITS(BaseAddr + CAN_F_PRESC_F_SJW_F_SEG_2_F_SEG_1_OFFSET,
             CAN_F_PRESC_F_SJW_F_SEG_2_F_SEG_1_F_SJW_SHIFT,
             CAN_F_PRESC_F_SJW_F_SEG_2_F_SEG_1_F_SJW_SIZE, (Value - 1));
}

/* Fast Speed Prescaler Registers(F_PRESC) */
static inline AL_U32 AlCan_ll_GetFpresc(AL_REG BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr + CAN_F_PRESC_F_SJW_F_SEG_2_F_SEG_1_OFFSET,
                    CAN_F_PRESC_F_SJW_F_SEG_2_F_SEG_1_F_PRESC_SHIFT,
                    CAN_F_PRESC_F_SJW_F_SEG_2_F_SEG_1_F_PRESC_SIZE);
}

static inline AL_VOID AlCan_ll_SetFpresc(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_SET_BITS(BaseAddr + CAN_F_PRESC_F_SJW_F_SEG_2_F_SEG_1_OFFSET,
             CAN_F_PRESC_F_SJW_F_SEG_2_F_SEG_1_F_PRESC_SHIFT,
             CAN_F_PRESC_F_SJW_F_SEG_2_F_SEG_1_F_PRESC_SIZE, (Value - 1));
}

// /* Arbitration Lost Capture(ALC) */
// static inline AL_U32 AlCan_ll_GetAlc(AL_REG BaseAddr)
// {
//     return AL_REG32_GET_BITS(BaseAddr + CAN_TECNT_REVNT_TDC_EALCAP_OFFSET, CAN_TECNT_REVNT_TDC_EALCAP_ALC_SHIFT,
//                     CAN_TECNT_REVNT_TDC_EALCAP_ALC_SIZE);
// }

/* Kind Of ERror(KOER) */
static inline AL_U32 AlCan_ll_GetKoer(AL_REG BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr + CAN_TECNT_REVNT_TDC_EALCAP_OFFSET, CAN_TECNT_REVNT_TDC_EALCAP_KOER_SHIFT,
                    CAN_TECNT_REVNT_TDC_EALCAP_KOER_SIZE);
}

// /* Secondary Sample Point OFFset(SSPOFF) */
// static inline AL_U32 AlCan_ll_GetSspoff(AL_REG BaseAddr)
// {
//     return AL_REG32_GET_BITS(BaseAddr + CAN_TECNT_REVNT_TDC_EALCAP_OFFSET, CAN_TECNT_REVNT_TDC_EALCAP_SSPOFF_SHIFT,
//                     CAN_TECNT_REVNT_TDC_EALCAP_SSPOFF_SIZE);
// }

static inline AL_VOID AlCan_ll_SetSspoff(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_SET_BITS(BaseAddr + CAN_TECNT_REVNT_TDC_EALCAP_OFFSET, CAN_TECNT_REVNT_TDC_EALCAP_SSPOFF_SHIFT,
             CAN_TECNT_REVNT_TDC_EALCAP_SSPOFF_SIZE, (Value - 2));
}

// /* Transmitter Delay Compensation ENable(TDCEN) */
// static inline AL_BOOL AlCan_ll_IsTdcen(AL_REG BaseAddr)
// {
//     return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + CAN_TECNT_REVNT_TDC_EALCAP_OFFSET,
//                             CAN_TECNT_REVNT_TDC_EALCAP_TDCEN_SHIFT);
// }

static inline AL_VOID AlCan_ll_SetTdcen(AL_REG BaseAddr, AL_BOOL IsEnabled)
{
    AL_REG32_SET_BIT(BaseAddr + CAN_TECNT_REVNT_TDC_EALCAP_OFFSET,
            CAN_TECNT_REVNT_TDC_EALCAP_TDCEN_SHIFT, IsEnabled);
}

// /* Receive Error CouNT(RECNT) */
// static inline AL_U32 AlCan_ll_GetRecnt(AL_REG BaseAddr)
// {
//     return AL_REG32_GET_BITS(BaseAddr + CAN_TECNT_REVNT_TDC_EALCAP_OFFSET, CAN_TECNT_REVNT_TDC_EALCAP_RECNT_SHIFT,
//                     CAN_TECNT_REVNT_TDC_EALCAP_RECNT_SIZE);
// }

// /* Transmit Error CouNT(TECNT) */
// static inline AL_U32 AlCan_ll_GetTecnt(AL_REG BaseAddr)
// {
//     return AL_REG32_GET_BITS(BaseAddr + CAN_TECNT_REVNT_TDC_EALCAP_OFFSET, CAN_TECNT_REVNT_TDC_EALCAP_TECNT_SHIFT,
//                     CAN_TECNT_REVNT_TDC_EALCAP_TECNT_SIZE);
// }

// /* acceptance filter address(ACFADR) */
// static inline AL_U32 AlCan_ll_GetAcfadr(AL_REG BaseAddr)
// {
//     return AL_REG32_GET_BITS(BaseAddr + CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_OFFSET,
//                     CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_ACFADR_SHIFT,
//                     CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_ACFADR_SIZE);
// }

static inline AL_VOID AlCan_ll_SetAcfadr(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_SET_BITS(BaseAddr + CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_OFFSET,
             CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_ACFADR_SHIFT,
             CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_ACFADR_SIZE, Value);
}

// /* SELect acceptance MASK(SELMASK) */
// static inline AL_CAN_SelmaskEnum AlCan_ll_GetSelmask(AL_REG BaseAddr)
// {
//     return (AL_CAN_SelmaskEnum)AL_REG32_GET_BIT(BaseAddr + CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_OFFSET,
//                                        CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_SELMASK_SHIFT);
// }

static inline AL_VOID AlCan_ll_SetSelmask(AL_REG BaseAddr, AL_CAN_SelmaskEnum Mode)
{
    AL_REG32_SET_BIT(BaseAddr + CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_OFFSET,
            CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_SELMASK_SHIFT, Mode);
}

// /* TIME-stamping ENable(TIMEEN) */
// static inline AL_BOOL AlCan_ll_IsTimeen(AL_REG BaseAddr)
// {
//     return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_OFFSET,
//                             CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_TIMEEN_SHIFT);
// }

// static inline AL_VOID AlCan_ll_SetTimeen(AL_REG BaseAddr, AL_BOOL IsEnabled)
// {
//     AL_REG32_SET_BIT(BaseAddr + CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_OFFSET,
//             CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_TIMEEN_SHIFT, IsEnabled);
// }

// /* TIME-stamping POSition(TIMEPOS) */
// static inline AL_CAN_TimeposEnum AlCan_ll_GetTimepos(AL_REG BaseAddr)
// {
//     return (AL_CAN_TimeposEnum)AL_REG32_GET_BIT(BaseAddr + CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_OFFSET,
//                                        CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_TIMEPOS_SHIFT);
// }

// static inline AL_VOID AlCan_ll_SetTimepos(AL_REG BaseAddr, AL_CAN_TimeposEnum Mode)
// {
//     AL_REG32_SET_BIT(BaseAddr + CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_OFFSET,
//             CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_TIMEPOS_SHIFT, Mode);
// }

// /* Acceptance Filter Enable(AE_x) */
// static inline AL_BOOL AlCan_ll_IsAex(AL_REG BaseAddr, AL_U32 Shift)
// {
//     return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_OFFSET, Shift);
// }

// static inline AL_VOID AlCan_ll_SetAex(AL_REG BaseAddr, AL_U32 Shift, AL_BOOL IsEnabled)
// {
//     AL_REG32_SET_BIT(BaseAddr + CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_OFFSET, Shift, IsEnabled);
// }

// /* Acceptance CODE/MASK ACODE_x/AMASK_x(ACODE_x/AMASK_x) */
// static inline AL_BOOL AlCan_ll_GetAcodexAmaskx(AL_REG BaseAddr, AL_U32 Shift)
// {
//     return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + CAN_ACF_3_ACF_2_ACF_1_ACF_0_OFFSET, Shift);
// }

// static inline AL_VOID AlCan_ll_SetAcodexAmaskx(AL_REG BaseAddr, AL_U32 Shift, AL_BOOL IsEnabled)
// {
//     AL_REG32_SET_BIT(BaseAddr + CAN_ACF_3_ACF_2_ACF_1_ACF_0_OFFSET, Shift, IsEnabled);
// }

// /* Acceptance mask IDE bit value(AIDE) */
// static inline AL_BOOL AlCan_ll_IsAide(AL_REG BaseAddr)
// {
//     return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + CAN_ACF_3_ACF_2_ACF_1_ACF_0_OFFSET,
//                             CAN_ACF_3_ACF_2_ACF_1_ACF_0_AIDE_SHIFT);
// }

// static inline AL_VOID AlCan_ll_SetAide(AL_REG BaseAddr, AL_BOOL IsEnabled)
// {
//     AL_REG32_SET_BIT(BaseAddr + CAN_ACF_3_ACF_2_ACF_1_ACF_0_OFFSET, CAN_ACF_3_ACF_2_ACF_1_ACF_0_AIDE_SHIFT,
//             IsEnabled);
// }

// /* Acceptance mask IDE bit check enable(AIDEE) */
// static inline AL_BOOL AlCan_ll_IsAidee(AL_REG BaseAddr)
// {
//     return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + CAN_ACF_3_ACF_2_ACF_1_ACF_0_OFFSET,
//                             CAN_ACF_3_ACF_2_ACF_1_ACF_0_AIDEE_SHIFT);
// }

// static inline AL_VOID AlCan_ll_SetAidee(AL_REG BaseAddr, AL_BOOL IsEnabled)
// {
//     AL_REG32_SET_BIT(BaseAddr + CAN_ACF_3_ACF_2_ACF_1_ACF_0_OFFSET,
//             CAN_ACF_3_ACF_2_ACF_1_ACF_0_AIDEE_SHIFT, IsEnabled);
// }

// /* Version Information(VER) */
// static inline AL_U32 AlCan_ll_GetVer(AL_REG BaseAddr)
// {
//     return AL_REG32_GET_BITS(BaseAddr + CAN_TTCFG_TBSLOT_VER_1_VER_0_OFFSET,
//                     CAN_TTCFG_TBSLOT_VER_1_VER_0_VER_1_VER_0_SHIFT,
//                     CAN_TTCFG_TBSLOT_VER_1_VER_0_VER_1_VER_0_SIZE);
// }

// /* Pointer to a TB message slot(TBPTR) */
// static inline AL_U32 AlCan_ll_GetTbptr(AL_REG BaseAddr)
// {
//     return AL_REG32_GET_BITS(BaseAddr + CAN_TTCFG_TBSLOT_VER_1_VER_0_OFFSET,
//                     CAN_TTCFG_TBSLOT_VER_1_VER_0_TBPTR_SHIFT,
//                     CAN_TTCFG_TBSLOT_VER_1_VER_0_TBPTR_SIZE);
// }

// static inline AL_VOID AlCan_ll_SetTbptr(AL_REG BaseAddr, AL_U32 Value)
// {
//     AL_REG32_SET_BITS(BaseAddr + CAN_TTCFG_TBSLOT_VER_1_VER_0_OFFSET,
//              CAN_TTCFG_TBSLOT_VER_1_VER_0_TBPTR_SHIFT,
//              CAN_TTCFG_TBSLOT_VER_1_VER_0_TBPTR_SIZE, Value);
// }

// /* set TB slot to “Filled”(TBF) */
// static inline AL_BOOL AlCan_ll_GetTbf(AL_REG BaseAddr)
// {
//     return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + CAN_TTCFG_TBSLOT_VER_1_VER_0_OFFSET,
//                             CAN_TTCFG_TBSLOT_VER_1_VER_0_TBF_SHIFT);
// }

// static inline AL_VOID AlCan_ll_SetTbf(AL_REG BaseAddr, AL_BOOL IsEnabled)
// {
//     AL_REG32_SET_BIT(BaseAddr + CAN_TTCFG_TBSLOT_VER_1_VER_0_OFFSET,
//             CAN_TTCFG_TBSLOT_VER_1_VER_0_TBF_SHIFT, IsEnabled);
// }

// /* set TB slot to “Empty”(TBE) */
// static inline AL_BOOL AlCan_ll_GetTbe(AL_REG BaseAddr)
// {
//     return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + CAN_TTCFG_TBSLOT_VER_1_VER_0_OFFSET,
//                             CAN_TTCFG_TBSLOT_VER_1_VER_0_TBE_SHIFT);
// }

// static inline AL_VOID AlCan_ll_SetTbe(AL_REG BaseAddr, AL_BOOL IsEnabled)
// {
//     AL_REG32_SET_BIT(BaseAddr + CAN_TTCFG_TBSLOT_VER_1_VER_0_OFFSET,
//             CAN_TTCFG_TBSLOT_VER_1_VER_0_TBE_SHIFT, IsEnabled);
// }

// /* Time Trigger Enable(TTEN) */
// static inline AL_BOOL AlCan_ll_IsTten(AL_REG BaseAddr)
// {
//     return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + CAN_TTCFG_TBSLOT_VER_1_VER_0_OFFSET,
//                             CAN_TTCFG_TBSLOT_VER_1_VER_0_TTEN_SHIFT);
// }

// static inline AL_VOID AlCan_ll_SetTten(AL_REG BaseAddr, AL_BOOL IsEnabled)
// {
//     AL_REG32_SET_BIT(BaseAddr + CAN_TTCFG_TBSLOT_VER_1_VER_0_OFFSET,
//             CAN_TTCFG_TBSLOT_VER_1_VER_0_TTEN_SHIFT, IsEnabled);
// }

// /* TTCAN Timer PRESCaler(T_PRESC) */
// static inline AL_CAN_TprescEnum AlCan_ll_GetTpresc(AL_REG BaseAddr)
// {
//     return (AL_CAN_TprescEnum)AL_REG32_GET_BITS(BaseAddr + CAN_TTCFG_TBSLOT_VER_1_VER_0_OFFSET,
//                                        CAN_TTCFG_TBSLOT_VER_1_VER_0_T_PRESC_SHIFT,
//                                        CAN_TTCFG_TBSLOT_VER_1_VER_0_T_PRESC_SIZE);
// }

// static inline AL_VOID AlCan_ll_SetTpresc(AL_REG BaseAddr, AL_CAN_TprescEnum Value)
// {
//     AL_REG32_SET_BITS(BaseAddr + CAN_TTCFG_TBSLOT_VER_1_VER_0_OFFSET,
//              CAN_TTCFG_TBSLOT_VER_1_VER_0_T_PRESC_SHIFT,
//              CAN_TTCFG_TBSLOT_VER_1_VER_0_T_PRESC_SIZE, Value);
// }

// /* Time Trigger Interrupt Flag(TTIF) */
// static inline AL_BOOL AlCan_ll_GetTtif(AL_REG BaseAddr)
// {
//     return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + CAN_TTCFG_TBSLOT_VER_1_VER_0_OFFSET,
//                             CAN_TTCFG_TBSLOT_VER_1_VER_0_TTIF_SHIFT);
// }

// static inline AL_VOID AlCan_ll_SetTtif(AL_REG BaseAddr, AL_BOOL IsEnabled)
// {
//     AL_REG32_SET_BIT(BaseAddr + CAN_TTCFG_TBSLOT_VER_1_VER_0_OFFSET,
//             CAN_TTCFG_TBSLOT_VER_1_VER_0_TTIF_SHIFT, IsEnabled);
// }

// /* Time Trigger Interrupt Enable(TTIE) */
// static inline AL_BOOL AlCan_ll_IsTtie(AL_REG BaseAddr)
// {
//     return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + CAN_TTCFG_TBSLOT_VER_1_VER_0_OFFSET,
//                             CAN_TTCFG_TBSLOT_VER_1_VER_0_TTIE_SHIFT);
// }

// static inline AL_VOID AlCan_ll_SetTtie(AL_REG BaseAddr, AL_BOOL IsEnabled)
// {
//     AL_REG32_SET_BIT(BaseAddr + CAN_TTCFG_TBSLOT_VER_1_VER_0_OFFSET,
//             CAN_TTCFG_TBSLOT_VER_1_VER_0_TTIE_SHIFT, IsEnabled);
// }

// /* Trigger Error Interrupt Flag(TEIF) */
// static inline AL_BOOL AlCan_ll_GetTeif(AL_REG BaseAddr)
// {
//     return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + CAN_TTCFG_TBSLOT_VER_1_VER_0_OFFSET,
//                             CAN_TTCFG_TBSLOT_VER_1_VER_0_TEIF_SHIFT);
// }

// static inline AL_VOID AlCan_ll_SetTeif(AL_REG BaseAddr, AL_BOOL IsEnabled)
// {
//     AL_REG32_SET_BIT(BaseAddr + CAN_TTCFG_TBSLOT_VER_1_VER_0_OFFSET, CAN_TTCFG_TBSLOT_VER_1_VER_0_TEIF_SHIFT,
//             IsEnabled);
// }

// /* Watch Trigger Interrupt Flag(WTIF) */
// static inline AL_BOOL AlCan_ll_GetWtif(AL_REG BaseAddr)
// {
//     return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + CAN_TTCFG_TBSLOT_VER_1_VER_0_OFFSET,
//                             CAN_TTCFG_TBSLOT_VER_1_VER_0_WTIF_SHIFT);
// }

// static inline AL_VOID AlCan_ll_SetWtif(AL_REG BaseAddr, AL_BOOL IsEnabled)
// {
//     AL_REG32_SET_BIT(BaseAddr + CAN_TTCFG_TBSLOT_VER_1_VER_0_OFFSET,  CAN_TTCFG_TBSLOT_VER_1_VER_0_WTIF_SHIFT,
//             IsEnabled);
// }

// /* Watch Trigger Interrupt Enable(WTIE) */
// static inline AL_BOOL AlCan_ll_IsWtie(AL_REG BaseAddr)
// {
//     return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + CAN_TTCFG_TBSLOT_VER_1_VER_0_OFFSET,
//         CAN_TTCFG_TBSLOT_VER_1_VER_0_WTIE_SHIFT);
// }

// static inline AL_VOID AlCan_ll_SetWtie(AL_REG BaseAddr, AL_BOOL IsEnabled)
// {
//     AL_REG32_SET_BIT(BaseAddr + CAN_TTCFG_TBSLOT_VER_1_VER_0_OFFSET, CAN_TTCFG_TBSLOT_VER_1_VER_0_WTIE_SHIFT,
//             IsEnabled);
// }

// /* REFerence message IDentifier(REF_ID) */
// static inline AL_U32 AlCan_ll_GetRefid(AL_REG BaseAddr)
// {
//     return AL_REG32_GET_BITS(BaseAddr + CAN_REF_MSG_OFFSET, CAN_REF_MSG_REF_ID_SHIFT, CAN_REF_MSG_REF_ID_SIZE);
// }

// static inline AL_VOID AlCan_ll_SetRefid(AL_REG BaseAddr, AL_U32 Value)
// {
//     AL_REG32_SET_BITS(BaseAddr + CAN_REF_MSG_OFFSET, CAN_REF_MSG_REF_ID_SHIFT, CAN_REF_MSG_REF_ID_SIZE, Value);
// }

// /* REFerence message ID Enable(REF_IDE) */
// static inline AL_BOOL AlCan_ll_IsRefide(AL_REG BaseAddr)
// {
//     return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + CAN_REF_MSG_OFFSET, CAN_REF_MSG_REF_IDE_SHIFT);
// }

// static inline AL_VOID AlCan_ll_SetRefide(AL_REG BaseAddr, AL_BOOL IsEnabled)
// {
//     AL_REG32_SET_BIT(BaseAddr + CAN_REF_MSG_OFFSET, CAN_REF_MSG_REF_IDE_SHIFT, IsEnabled);
// }

// /* Transmit Trigger TB slot Pointer(TTPTR) */
// static inline AL_U32 AlCan_ll_GetTtptr(AL_REG BaseAddr)
// {
//     return AL_REG32_GET_BITS(BaseAddr + CAN_TT_TRIG_TRIG_CFG_OFFSET, CAN_TT_TRIG_TRIG_CFG_TTPTR_SHIFT,
//                     CAN_TT_TRIG_TRIG_CFG_TTPTR_SIZE);
// }

// static inline AL_VOID AlCan_ll_SetTtptr(AL_REG BaseAddr, AL_U32 Value)
// {
//     AL_REG32_SET_BITS(BaseAddr + CAN_TT_TRIG_TRIG_CFG_OFFSET, CAN_TT_TRIG_TRIG_CFG_TTPTR_SHIFT,
//              CAN_TT_TRIG_TRIG_CFG_TTPTR_SIZE, Value);
// }

// /* Trigger Type(TTYPE) */
// static inline AL_CAN_TtypeEnum AlCan_ll_GetTtype(AL_REG BaseAddr)
// {
//     return (AL_CAN_TtypeEnum)AL_REG32_GET_BITS(BaseAddr + CAN_TT_TRIG_TRIG_CFG_OFFSET, CAN_TT_TRIG_TRIG_CFG_TTYPE_SHIFT,
//                                       CAN_TT_TRIG_TRIG_CFG_TTYPE_SIZE);
// }

// static inline AL_VOID AlCan_ll_SetTtype(AL_REG BaseAddr, AL_CAN_TtypeEnum Type)
// {
//     AL_REG32_SET_BITS(BaseAddr + CAN_TT_TRIG_TRIG_CFG_OFFSET, CAN_TT_TRIG_TRIG_CFG_TTYPE_SHIFT,
//              CAN_TT_TRIG_TRIG_CFG_TTYPE_SIZE, Type);
// }

// /* Transmit Enable Window(TEW) */
// static inline AL_U32 AlCan_ll_GetTew(AL_REG BaseAddr)
// {
//     return AL_REG32_GET_BITS(BaseAddr + CAN_TT_TRIG_TRIG_CFG_OFFSET, CAN_TT_TRIG_TRIG_CFG_TEW_3_0_SHIFT,
//                     CAN_TT_TRIG_TRIG_CFG_TEW_3_0_SIZE);
// }

// static inline AL_VOID AlCan_ll_SetTew(AL_REG BaseAddr, AL_U32 Value)
// {
//     AL_REG32_SET_BITS(BaseAddr + CAN_TT_TRIG_TRIG_CFG_OFFSET, CAN_TT_TRIG_TRIG_CFG_TEW_3_0_SHIFT,
//              CAN_TT_TRIG_TRIG_CFG_TEW_3_0_SIZE, Value);
// }

// /* Trigger Time(TT_TRIG) */
// static inline AL_U32 AlCan_ll_GetTttrig(AL_REG BaseAddr)
// {
//     return AL_REG32_GET_BITS(BaseAddr + CAN_TT_TRIG_TRIG_CFG_OFFSET, CAN_TT_TRIG_TRIG_CFG_TT_TRIG_SHIFT,
//                     CAN_TT_TRIG_TRIG_CFG_TT_TRIG_SIZE);
// }

// static inline AL_VOID AlCan_ll_SetTttrig(AL_REG BaseAddr, AL_U32 Value)
// {
//     AL_REG32_SET_BITS(BaseAddr + CAN_TT_TRIG_TRIG_CFG_OFFSET, CAN_TT_TRIG_TRIG_CFG_TT_TRIG_SHIFT,
//              CAN_TT_TRIG_TRIG_CFG_TT_TRIG_SIZE, Value);
// }

// /* Watch Trigger Time(TT_WTRIG) */
// static inline AL_U32 AlCan_ll_GetTtwtrig(AL_REG BaseAddr)
// {
//     return AL_REG32_GET_BITS(BaseAddr + CAN_TT_WTRIG_OFFSET, CAN_TT_WTRIG_TT_WTRIG_SHIFT, CAN_TT_WTRIG_TT_WTRIG_SIZE);
// }

// static inline AL_VOID AlCan_ll_SetTtwtrig(AL_REG BaseAddr, AL_U32 Value)
// {
//     AL_REG32_SET_BITS(BaseAddr + CAN_TT_WTRIG_OFFSET, CAN_TT_WTRIG_TT_WTRIG_SHIFT, CAN_TT_WTRIG_TT_WTRIG_SIZE, Value);
// }

/**
 * Enable all intr
*/
static inline AL_VOID AlCan_ll_SetAllIntrEnabled(AL_REG BaseAddr, AL_BOOL IsEnabled)
{
    AL_REG32_SET_BITS(BaseAddr + CAN_LIMIT_ERRINT_RTIF_RTIE_OFFSET, CAN_LIMIT_ERRINT_RTIF_RTIE_EIE_SHIFT,
             CAN_LIMIT_ERRINT_RTIF_RTIE_RIE_SHIFT, ((IsEnabled) ? 0x7F : 0x0));
}

/**
 * Acceptance Filter Enable(ACF_EN_0)
 * Acceptance Filter Enable(ACF_EN_1)
*/
static inline AL_U32 AlCan_ll_IsAcfen(AL_REG BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr + CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_OFFSET,
                    CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_AE_0_SHIFT,
                    CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_AE_X_SIZE);
}

static inline AL_VOID AlCan_ll_SetAcfen(AL_REG BaseAddr, AL_BOOL IsEnabled)
{
    AL_REG32_SET_BITS(BaseAddr + CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_OFFSET,
             CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_AE_1_SHIFT,
             CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_AE_X_SIZE, ((IsEnabled) ? 0x7FFF : 0x0));
}

static inline AL_BOOL AlCan_ll_IsAcfenx(AL_REG BaseAddr, AL_U32 Index)
{
    return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_OFFSET,
                            CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_AE_0_SHIFT + Index);
}

static inline AL_BOOL AlCan_ll_SetAcfenx(AL_REG BaseAddr, AL_U32 Index, AL_BOOL IsEnabled)
{
    AL_REG32_SET_BIT(BaseAddr + CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_OFFSET,
            CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_AE_0_SHIFT + Index, IsEnabled);
}


/**
 * Read receive frame Id
*/
static inline AL_U32 AlCan_ll_GetRecvExtId(AL_REG BaseAddr, AL_BOOL IsExtId)
{
    return AL_REG32_GET_BITS(BaseAddr + CAN_RBUF_0_3_OFFSET, CAN_RBUF_0_3_ID_7_0_SHIFT,
                    (IsExtId ? CAN_RBUF_0_3_ID_EXT_SIZE : CAN_RBUF_0_3_ID_STD_SIZE));
}

/**
 * Receive Buffer Registers(RBUFx)
*/
static inline AL_U32 AlCan_ll_ReadWordRecvBuffer(AL_REG BaseAddr, AL_U32 Offset)
{
    return AL_REG32_READ(BaseAddr + CAN_RBUF_0_3_OFFSET + (Offset << 2));
}

/**
 * Transmit Buffer Registers(TBUFx)
*/
static inline AL_VOID AlCan_ll_WriteWordSendBuffer(AL_REG BaseAddr, AL_U32 Offset, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + CAN_TBUF_0_3_OFFSET + (Offset << 2), Value);
}


// static inline AL_U32 AlCan_ll_GetIntrStatus(AL_REG BaseAddr)
// {
//     return AL_REG32_READ(BaseAddr + CAN_LIMIT_ERRINT_RTIF_RTIE_OFFSET) & CAN_LIMIT_ERRINT_RTIF_RTIE_ALLINTR_MASK;
// }

static inline AL_U32 AlCan_ll_GetUmaskIntrStatus(AL_REG BaseAddr)
{
    AL_U32 Status = AL_REG32_READ(BaseAddr + CAN_LIMIT_ERRINT_RTIF_RTIE_OFFSET);
    return (Status & ((Status & 0xFE) << 8)) | (Status & 0x100) | (Status & ((Status & 0x2A0000) >> 1));
}

static inline AL_VOID AlCan_ll_ClrIntrStatus(AL_REG BaseAddr)
{
    AL_U32 Status = AL_REG32_READ(BaseAddr + CAN_LIMIT_ERRINT_RTIF_RTIE_OFFSET);
    AL_REG32_WRITE(BaseAddr + CAN_LIMIT_ERRINT_RTIF_RTIE_OFFSET, (Status | CAN_LIMIT_ERRINT_RTIF_RTIE_ALLINTR_MASK));
}

// /**
//  * Configuration and Status Register(CFG_STAT)
//  * Command Register(TCMD)
//  * Transmit Control Register(TCTRL)
//  * Receive Control Register(RCTRL)
// */
// static inline AL_U32 AlCan_ll_ReadCfgstatTcmdTctrlRctrl(AL_REG BaseAddr)
// {
//     return AL_REG32_READ(BaseAddr + CAN_RCTRL_TCTRL_TCMD_CFG_STAT_OFFSET);
// }

// static inline AL_VOID AlCan_ll_WriteCfgstatTcmdTctrlRctrl(AL_REG BaseAddr, AL_U32 Value)
// {
//     AL_REG32_WRITE(BaseAddr + CAN_RCTRL_TCTRL_TCMD_CFG_STAT_OFFSET, Value);
// }

// /**
//  * Receive and Transmit Interrupt Enable Register(RTIE)
//  * Receive and Transmit Interrupt Flag Register(RTIF)
//  * ERRor INTerrupt Enable and Flag Register(ERRINT)
//  * Warning Limits Register(LIMIT)
// */
// static inline AL_U32 AlCan_ll_ReadLimitErrintRtifRtie(AL_REG BaseAddr)
// {
//     return AL_REG32_READ(BaseAddr + CAN_LIMIT_ERRINT_RTIF_RTIE_OFFSET);
// }

// static inline AL_VOID AlCan_ll_WriteLimitErrintRtifRtie(AL_REG BaseAddr, AL_U32 Value)
// {
//     AL_REG32_WRITE(BaseAddr + CAN_LIMIT_ERRINT_RTIF_RTIE_OFFSET, Value);
// }

// /**
//  * Slow Speed Bit Timing Segment 1(S_Seg_1)
//  * Slow Speed Bit Timing Segment 2(S_Seg_2)
//  * Slow Speed Synchronization Jump Width(S_SJW)
//  * Slow Speed Prescaler Registers(S_PRESC)
// */
// static inline AL_U32 AlCan_ll_ReadSspescSsjwSseg2Sseg1(AL_REG BaseAddr)
// {
//     return AL_REG32_READ(BaseAddr + CAN_S_PRESC_S_SJW_S_SEG_2_S_SEG_1_OFFSET);
// }

// static inline AL_VOID AlCan_ll_WriteSspescSsjwSseg2Sseg1(AL_REG BaseAddr, AL_U32 Value)
// {
//     AL_REG32_WRITE(BaseAddr + CAN_S_PRESC_S_SJW_S_SEG_2_S_SEG_1_OFFSET, Value);
// }

// /**
//  * Fast Speed Bit Timing Segment 1(F_Seg_1)
//  * Fast Speed Bit Timing Segment 2(F_Seg_2)
//  * Fast Speed Synchronization Jump Width(F_SJW)
//  * Fast Speed Prescaler Registers(F_PRESC)
// */
// static inline AL_U32 AlCan_ll_ReadFspescFsjwFseg2Fseg1(AL_REG BaseAddr)
// {
//     return AL_REG32_READ(BaseAddr + CAN_F_PRESC_F_SJW_F_SEG_2_F_SEG_1_OFFSET);
// }

// static inline AL_VOID AlCan_ll_WriteFspescFsjwFseg2Fseg1(AL_REG BaseAddr, AL_U32 Value)
// {
//     AL_REG32_WRITE(BaseAddr + CAN_F_PRESC_F_SJW_F_SEG_2_F_SEG_1_OFFSET, Value);
// }

// /**
//  * Error and Arbitration Lost Capture Register(EALCAP)
//  * Transmitter Delay Compensation Register(TDC)
//  * Receive Error Counter Registers(RECNT)
//  * Transmit Error Counter Registers(TECNT)
// */
// static inline AL_U32 AlCan_ll_ReadTecntRecntTdcEalcap(AL_REG BaseAddr)
// {
//     return AL_REG32_READ(BaseAddr + CAN_TECNT_REVNT_TDC_EALCAP_OFFSET);
// }

// static inline AL_VOID AlCan_ll_WriteTecntRecntTdcEalcap(AL_REG BaseAddr, AL_U32 Value)
// {
//     AL_REG32_WRITE(BaseAddr + CAN_TECNT_REVNT_TDC_EALCAP_OFFSET, Value);
// }

// /**
//  * Acceptance Filter Control Register(ACFCTRL)
//  * CiA 603 Time-Stamping(TIMECFG)
//  * Acceptance Filter Enable(ACF_EN_0)
//  * Acceptance Filter Enable(ACF_EN_1)
// */
// static inline AL_U32 AlCan_ll_ReadAcfenxTimecfgAcfctrl(AL_REG BaseAddr)
// {
//     return AL_REG32_READ(BaseAddr + CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_OFFSET);
// }

// static inline AL_VOID AlCan_ll_WriteAcfenxTimecfgAcfctrl(AL_REG BaseAddr, AL_U32 Value)
// {
//     AL_REG32_WRITE(BaseAddr + CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_OFFSET, Value);
// }

/**
 * Acceptance CODE/MASK ACODE_0/AMASK_0(ACODE_0/AMASK_0)
 * Acceptance CODE/MASK ACODE_1/AMASK_1(ACODE_1/AMASK_1)
 * Acceptance CODE/MASK ACODE_2/AMASK_2(ACODE_2/AMASK_2)
 * Acceptance CODE/MASK ACODE_3/AMASK_3(ACODE_3/AMASK_3)
*/
static inline AL_U32 AlCan_ll_ReadAcodexAmaskx(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + CAN_ACF_3_ACF_2_ACF_1_ACF_0_OFFSET);
}

static inline AL_VOID AlCan_ll_WriteAcodexAmaskx(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + CAN_ACF_3_ACF_2_ACF_1_ACF_0_OFFSET, Value);
}

// /**
//  * Version Information(VER_0)
//  * Version Information(VER_1)
//  * TTCAN:TB Slot Pointer(TBSLOT)
//  * TTCAN:Time Trigger Configuration(TTCFG)
// */
// static inline AL_U32 AlCan_ll_ReadVerxTbslotTtcfg(AL_REG BaseAddr)
// {
//     return AL_REG32_READ(BaseAddr + CAN_TTCFG_TBSLOT_VER_1_VER_0_OFFSET);
// }

// static inline AL_VOID AlCan_ll_WriteVerxTbslotTtcfg(AL_REG BaseAddr, AL_U32 Value)
// {
//     AL_REG32_WRITE(BaseAddr + CAN_TTCFG_TBSLOT_VER_1_VER_0_OFFSET, Value);
// }

// /**
//  * Reference Message(REF_MSG_0)
//  * Reference Message(REF_MSG_1)
//  * Reference Message(REF_MSG_2)
//  * Reference Message(REF_MSG_3)
// */
// static inline AL_U32 AlCan_ll_ReadRefmsgx(AL_REG BaseAddr)
// {
//     return AL_REG32_READ(BaseAddr + CAN_REF_MSG_OFFSET);
// }

// static inline AL_VOID AlCan_ll_WriteRefmsgx(AL_REG BaseAddr, AL_U32 Value)
// {
//     AL_REG32_WRITE(BaseAddr + CAN_REF_MSG_OFFSET, Value);
// }

// /**
//  * TTCAN: Trigger Configuration(TRIG_CFG_0)
//  * TTCAN: Trigger Configuration(TRIG_CFG_1)
//  * TTCAN: Trigger Time(TT_TRIG_0)
//  * TTCAN: Trigger Time(TT_TRIG_1)
// */
// static inline AL_U32 AlCan_ll_ReadTttrigxTrigcfgx(AL_REG BaseAddr)
// {
//     return AL_REG32_READ(BaseAddr + CAN_TT_TRIG_TRIG_CFG_OFFSET);
// }

// static inline AL_VOID AlCan_ll_WriteTttrigxTrigcfgx(AL_REG BaseAddr, AL_U32 Value)
// {
//     AL_REG32_WRITE(BaseAddr + CAN_TT_TRIG_TRIG_CFG_OFFSET, Value);
// }

// /**
//  * TTCAN: Watch Trigger Time(TT_WTRIG_0)
//  * TTCAN: Watch Trigger Time(TT_WTRIG_1)
//  * Memory Protection(MEM_PROT)
//  * Memory Status(MEM_STAT)
// */
// static inline AL_U32 AlCan_ll_ReadTtwtrigx(AL_REG BaseAddr)
// {
//     return AL_REG32_READ(BaseAddr + CAN_TT_TRIG_TRIG_CFG_OFFSET);
// }

// static inline AL_VOID AlCan_ll_WriteTtwtrigx(AL_REG BaseAddr, AL_U32 Value)
// {
//     AL_REG32_WRITE(BaseAddr + CAN_TT_TRIG_TRIG_CFG_OFFSET, Value);
// }


#ifdef __cplusplus
}
#endif

#endif
