#ifndef AL_CAN_DEV_H
#define AL_CAN_DEV_H

#ifdef __cplusplus
extern "C" {
#endif

#include "al_can_ll.h"

#define AL_CAN_MAX_S_SEG_1      63
#define AL_CAN_MAX_S_SEG_2      7
#define AL_CAN_MAX_S_SJW        15
#define AL_CAN_MAX_FS_SEG_1     63
#define AL_CAN_MAX_FS_SEG_2     31
#define AL_CAN_MAX_FS_SJW       15
#define AL_CAN_MAX_FF_SEG_1     15
#define AL_CAN_MAX_FF_SEG_2     7
#define AL_CAN_MAX_FF_SJW       7

typedef enum
{
    CAN_ERR_RECV = 0x100,    /* receive message with error*/
    CAN_ERR_WRONG_KOER_PARAMETER,
    CAN_ERR_KOER_RECV_BIT,
    CAN_ERR_KOER_SEND_BIT,
    CAN_ERR_KOER_FORM,
    CAN_ERR_KOER_RECV_STUFF,
    CAN_ERR_KOER_SEND_STUFF,
    CAN_ERR_KOER_RECV_ACK,
    CAN_ERR_KOER_SEND_ACK,
    CAN_ERR_KOER_CRC,
    CAN_ERR_KOER_OTHER,
    CAN_ERR_RECV_EMPTY,
    CAN_ERR_STATE_NOT_READY,
} CAN_ERR_CODE;

#define AL_CAN_ERR_RECV                 AL_DEF_ERR(AL_CAN, AL_ERR_LEVEL_ERROR, CAN_ERR_RECV)
#define AL_CAN_ERR_WRONG_KOER_PARAMETER AL_DEF_ERR(AL_CAN, AL_ERR_LEVEL_ERROR, CAN_ERR_WRONG_KOER_PARAMETER)
#define AL_CAN_ERR_KOER_RECV_BIT        AL_DEF_ERR(AL_CAN, AL_ERR_LEVEL_ERROR, CAN_ERR_KOER_RECV_BIT)
#define AL_CAN_ERR_KOER_SEND_BIT        AL_DEF_ERR(AL_CAN, AL_ERR_LEVEL_ERROR, CAN_ERR_KOER_SEND_BIT)
#define AL_CAN_ERR_KOER_FORM            AL_DEF_ERR(AL_CAN, AL_ERR_LEVEL_ERROR, CAN_ERR_KOER_FORM)
#define AL_CAN_ERR_KOER_RECV_STUFF      AL_DEF_ERR(AL_CAN, AL_ERR_LEVEL_ERROR, CAN_ERR_KOER_RECV_STUFF)
#define AL_CAN_ERR_KOER_SEND_STUFF      AL_DEF_ERR(AL_CAN, AL_ERR_LEVEL_ERROR, CAN_ERR_KOER_SEND_STUFF)
#define AL_CAN_ERR_KOER_RECV_ACK        AL_DEF_ERR(AL_CAN, AL_ERR_LEVEL_ERROR, CAN_ERR_KOER_RECV_ACK)
#define AL_CAN_ERR_KOER_SEND_ACK        AL_DEF_ERR(AL_CAN, AL_ERR_LEVEL_ERROR, CAN_ERR_KOER_SEND_ACK)
#define AL_CAN_ERR_KOER_CRC             AL_DEF_ERR(AL_CAN, AL_ERR_LEVEL_ERROR, CAN_ERR_KOER_CRC)
#define AL_CAN_ERR_KOER_OTHER           AL_DEF_ERR(AL_CAN, AL_ERR_LEVEL_ERROR, CAN_ERR_KOER_OTHER)
#define AL_CAN_ERR_RECV_EMPTY           AL_DEF_ERR(AL_CAN, AL_ERR_LEVEL_INFO, CAN_ERR_RECV_EMPTY)
#define AL_CAN_ERR_STATE_NOT_READY      AL_DEF_ERR(AL_CAN, AL_ERR_LEVEL_ERROR, CAN_ERR_STATE_NOT_READY)

typedef enum
{
    AL_CAN_RUN_NOT_SET      = (0x0),
    AL_CAN_RUN_INTR         = (0x1 << 1),
    AL_CAN_RUN_INTR_DMA     = (0x1 << 2)
}AL_CAN_RunModeEnum;

typedef enum
{
    AL_CAN_TYPE_NOT_SET = (0x0),
    AL_CAN_TYPE_2_0B    = (0x1),
    AL_CAN_TYPE_FD      = (0x1 << 1)
}AL_CAN_TypeEnum;

typedef enum
{
    AL_CAN_ID_STD       = (0x0),
    AL_CAN_ID_EXT       = (0x1)
}AL_CAN_IdTypeEnum;

typedef enum
{
    AL_CAN_ARBITRATION__0_25M,
    AL_CAN_ARBITRATION__0_5M,
    AL_CAN__0_5M,
    AL_CAN__0_833M,
    AL_CAN__1_M,
    AL_CAN__1_538M,
    AL_CAN__2M,
    AL_CAN__3_077M,
    AL_CAN__4M,
    AL_CAN__5M,
    AL_CAN__6_667M,
    AL_CAN__8M,
    AL_CAN__10M
}AL_CAN_BitTimingEnum;

typedef enum
{
    AL_CAN_LEN_0,
    AL_CAN_LEN_1,
    AL_CAN_LEN_2,
    AL_CAN_LEN_3,
    AL_CAN_LEN_4,
    AL_CAN_LEN_5,
    AL_CAN_LEN_6,
    AL_CAN_LEN_7,
    AL_CAN_LEN_8,
    AL_CAN_LEN_12,
    AL_CAN_LEN_16,
    AL_CAN_LEN_20,
    AL_CAN_LEN_24,
    AL_CAN_LEN_32,
    AL_CAN_LEN_48,
    AL_CAN_LEN_64,
    AL_CAN_LEN_MAX
}AL_CAN_DataLenEnum;

typedef enum
{
    AL_CAN_BIT__2_0B__S = (0x1),
    AL_CAN_BIT__FD__S   = (0x1 << 1),
    AL_CAN_BIT__FD__F   = (0x1 << 2)
}AL_CAN_BitTimingTypeEnum;

typedef enum
{
    AL_CAN_MODE_NOT_SET     = (0x0),
    AL_CAN_MODE_NORMAL      = (0x1 << 1),
    AL_CAN_MODE_IN_LOOPBACK = (0x1 << 2),
    AL_CAN_MODE_EX_LOOPBACK = (0x1 << 3),
    AL_CAN_MODE_STANDBY     = (0x1 << 4),
    AL_CAN_MODE_LISTENONLY  = (0x1 << 5)
}AL_CAN_OpsModeEnum;

typedef enum
{
    AL_CAN_TRANS_FULL,
    AL_CAN_TRANS_PTB,
    AL_CAN_TRANS_STB_FIFO,
    AL_CAN_TRANS_STB_PRIO,
    AL_CAN_TRANS_MAX
}AL_CAN_TransModeEnum;

typedef enum
{
    AL_CAN_SELMASK_ACODE,
    AL_CAN_SELMASK_AMASK
}AL_CAN_SelMaskEnum;

typedef enum
{
    AL_CAN_RB_LIMIT_0_1 = (0x1),
    AL_CAN_RB_LIMIT_2,
    AL_CAN_RB_LIMIT_3,
    AL_CAN_RB_LIMIT_4,
    AL_CAN_RB_LIMIT_5,
    AL_CAN_RB_LIMIT_6,
    AL_CAN_RB_LIMIT_7,
    AL_CAN_RB_LIMIT_8,
    AL_CAN_RB_LIMIT_9,
    AL_CAN_RB_LIMIT_10,
    AL_CAN_RB_LIMIT_11,
    AL_CAN_RB_LIMIT_12,
    AL_CAN_RB_LIMIT_13,
    AL_CAN_RB_LIMIT_14,
    AL_CAN_RB_LIMIT_15,
    AL_CAN_RB_LIMIT_MAX
}AL_CAN_RbAwflEnum;

typedef enum
{
    AL_CAN_INTR_AIF     = (0x1 << 8),
    AL_CAN_INTR_EIF     = (0x1 << 9),
    AL_CAN_INTR_TSIF    = (0x1 << 10),
    AL_CAN_INTR_TPIF    = (0x1 << 11),
    AL_CAN_INTR_RAFIF   = (0x1 << 12),
    AL_CAN_INTR_RFIF    = (0x1 << 13),
    AL_CAN_INTR_ROIF    = (0x1 << 14),
    AL_CAN_INTR_RIF     = (0x1 << 15),
    AL_CAN_INTR_BEIF    = (0x1 << 16),
    AL_CAN_INTR_ALIF    = (0x1 << 18),
    AL_CAN_INTR_EPIF    = (0x1 << 20)
}AL_CAN_IntrTypeEnum;

typedef enum
{
    AL_CAN_EVENT_SEND_DONE          = (0x1),
    AL_CAN_EVENT_RECV_DONE          = (0x1 << 1),
    AL_CAN_EVENT_RBUFF_ALMOST_FULL  = (0x1 << 2),
    AL_CAN_EVENT_RBUFF_FULL         = (0x1 << 3),
    AL_CAN_EVENT_RBUFF_OVERFLOW     = (0x1 << 4),
    AL_CAN_EVENT_ABORT              = (0x1 << 5),
    AL_CAN_EVENT_ERR                = (0x1 << 6),
    AL_CAN_EVENT_BUS_ERR            = (0x1 << 7),
    AL_CAN_EVENT_ARBITRATION_LOST   = (0x1 << 8),
    AL_CAN_EVENT_ERR_PASSIVE        = (0x1 << 9)
}AL_CAN_EventIdEnum;

typedef enum
{
    AL_CAN_FILTER_0,
    AL_CAN_FILTER_1,
    AL_CAN_FILTER_2,
    AL_CAN_FILTER_MAX
}AL_CAN_FilterIndexEnum;

typedef enum
{
    AL_CAN_FILTER_MASK_BOTH = (0b00),
    AL_CAN_FILTER_MASK_STD  = (0b10),
    AL_CAN_FILTER_MASK_EXT  = (0b11)
}AL_CAN_FilterMaskTypeEnum;

typedef struct
{
    AL_CAN_EventIdEnum  EventId;
    AL_U32              EventData;
}AL_CAN_EventStruct;

typedef AL_VOID (*AL_CAN_EventCallBack)(AL_CAN_EventStruct *Event, AL_VOID *CallBackRef);

typedef struct
{
    AL_CAN_EventCallBack    Func;
    AL_VOID                 *Ref;
}AL_CAN_CallBackStruct;

typedef union
{
    AL_U32  Reg;
    struct {
        AL_U32  Dlc:4;
        AL_U32  Brs:1;
        AL_U32  Fdf:1;
        AL_U32  Rtr:1;
        AL_U32  Ide:1;
        AL_U32  Reserve:4;
        AL_U32  Tx:1;
        AL_U32  Koer:3;
        AL_U32  CycleTime:16;
    }Bit;
}AL_CAN_FrameCtrlUnion;

typedef struct
{
    AL_CAN_FilterIndexEnum      FilterIndex;
    AL_CAN_FilterMaskTypeEnum   MaskType;
    AL_U32                      MaskValue;
    AL_U32                      IdValue;
}AL_CAN_FilterCfgStruct;

typedef enum
{
    AL_CAN_KOER_NONE    = (0x0),
    AL_CAN_KOER_BIT,
    AL_CAN_KOER_FORM,
    AL_CAN_KOER_STUFF,
    AL_CAN_KOER_ACK,
    AL_CAN_KOER_CRC,
    AL_CAN_KOER_OTHER,
    AL_CAN_KOER_NOT_USED
}AL_CAN_KoerEnum;

typedef struct
{
    AL_U32              Id;
    AL_BOOL             IsIdExt;
    AL_BOOL             IsRemote;
    AL_BOOL             IsBitSwitch;
    AL_BOOL             IsEnTts;
    AL_CAN_KoerEnum     Koer;
    AL_CAN_DataLenEnum  DataLen;
    AL_U32              Data[16];
    AL_U32              Rts[2];
}AL_CAN_FrameStruct;

typedef enum {
    AL_CAN_STATE_NOT_INIT           = (0x0),
    AL_CAN_STATE_READY              = (0x01 << 0),
    AL_CAN_STATE_RESET              = (0x01 << 1),
    AL_CAN_STATE_SEND_BUSY          = (0x01 << 2),
    AL_CAN_STATE_RECV_BUSY          = (0x01 << 3),
    AL_CAN_STATE_RECV_EMPTY         = (0x01 << 4),
    AL_CAN_STATE_RECV_ALMOST_FULL   = (0x01 << 5),
    AL_CAN_STATE_RECV_FULL          = (0x01 << 6),
    AL_CAN_STATE_RECV_OVERFLOW      = (0x01 << 7),
} AL_CAN_StateEnum;

typedef struct
{
    AL_CAN_TypeEnum         Type;
    AL_CAN_OpsModeEnum      OpsMode;
    AL_CAN_RunModeEnum      RunMode;
    AL_CAN_TransModeEnum    TransMode;
    AL_CAN_BitTimingEnum    SlowBitTiming;
    AL_CAN_BitTimingEnum    FastBitTiming;
    AL_CAN_RbAwflEnum       RbAfwl;     /* receive buffer Almost Full Warning Limit */
}AL_CAN_InitStruct;

typedef struct
{
    AL_REG                BaseAddr;
    AL_CAN_InitStruct       Config;
    AL_CAN_CallBackStruct   EventCallBack;
    AL_CAN_StateEnum        State;
    AL_CAN_FrameStruct      *RecvBuffer;
} AL_CAN_DevStruct;

AL_CAN_HwConfigStruct *AlCan_Dev_LookupConfig(AL_U32 DeviceId);
AL_S32 AlCan_Dev_Init(AL_CAN_DevStruct *Dev, AL_CAN_HwConfigStruct *HwConfig, \
    AL_CAN_InitStruct *InitConfig);
AL_VOID AlCan_Dev_SetBitTiming(AL_CAN_DevStruct *Dev, AL_CAN_BitTimingTypeEnum Type, \
    AL_U8 TimeSeg1, AL_U8 TimeSeg2, AL_U8 SyncJumpWidth);
AL_VOID AlCan_Dev_GetBitTiming(AL_CAN_DevStruct *Dev, AL_CAN_BitTimingTypeEnum Type, \
    AL_U8 *TimeSeg1, AL_U8 *TimeSeg2, AL_U8 *SyncJumpWidth);
AL_VOID AlCan_Dev_SetBaudRatePrescaler(AL_CAN_DevStruct *Dev, AL_CAN_BitTimingTypeEnum Type, \
    AL_U8 Prescaler);
AL_U8 AlCan_Dev_GetBaudRatePrescaler(AL_CAN_DevStruct *Dev, AL_CAN_BitTimingTypeEnum Type);
AL_VOID AlCan_Dev_SetDefBitTimingAndPrescaler(AL_CAN_DevStruct *Dev, \
    AL_CAN_BitTimingTypeEnum Type, AL_CAN_BitTimingEnum BitTiming);
AL_VOID AlCan_Dev_IntrHandler(void *Instance);
AL_S32 AlCan_Dev_RegisterEventCallBack(AL_CAN_DevStruct *Dev, AL_CAN_CallBackStruct *CallBack);
AL_S32 AlCan_Dev_UnRegisterEventCallBack(AL_CAN_DevStruct *Dev);
AL_VOID AlCan_Dev_SetReset(AL_CAN_DevStruct *Dev, AL_BOOL IsReset);
AL_S32 AlCan_Dev_SendFrame(AL_CAN_DevStruct *Dev, AL_CAN_FrameStruct *Frame);
AL_S32 AlCan_Dev_RecvFrame(AL_CAN_DevStruct *Dev, AL_CAN_FrameStruct *Frame);
AL_S32 AlCan_Dev_SetFilter(AL_CAN_DevStruct *Dev, AL_CAN_FilterCfgStruct *FilterCfg);
AL_S32 AlCan_Dev_GetFilter(AL_CAN_DevStruct *Dev, AL_CAN_FilterCfgStruct *FilterCfg);
AL_S32 AlCan_Dev_GetDecodeError(AL_CAN_DevStruct *Dev);
AL_S32 AlCan_Dev_DisplayFrame(AL_CAN_FrameStruct *Frame);

#ifdef __cplusplus
}
#endif

#endif
