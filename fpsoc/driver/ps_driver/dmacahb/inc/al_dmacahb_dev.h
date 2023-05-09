/**
 * @file    al_dmacahb_dev.h
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2023-04-17
 * @brief   dmac ahb device driver
 */

#ifndef AL_DMACAHB_DEV_H
#define AL_DMACAHB_DEV_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#include "al_dmacahb_ll.h"

/************************** Constant Definitions *****************************/

/***************** Macros (Inline Functions) Definitions *********************/
#define AL_DMACAHB_CHANNEL_NUM  (8)
#define AL_DMACAHB_CH_INTR_NUM  (5)

typedef enum
{
    DMACAHB_ERR_ENABLED_CHANNEL = 0x100,    /* receive message with error*/
    DMACAHB_ERR_INVALID_CH_ID,
    DMACAHB_ERR_HANDLE_WITHOUT_CH,
    DMACAHB_ERR_CH_WITHOUT_DMAC,
    DMACAHB_ERR_IOCTL_CMD,
    DMACAHB_ERR_ADDR_NOT_ALIGN,
    DMACAHB_ERR_TRANS_TYPE,
    DMACAHB_ERR_TRANS_BUSY,
    DMACAHB_ERR_STATE_NOT_READY,
} AL_DMACAHB_ErrCodeEnum;

#define AL_DMACAHB_ERR_INVALID_DEVID        AL_DEF_ERR(AL_DMACAHB, AL_ERR_LEVEL_ERROR, AL_ERR_INVALID_DEVID)
#define AL_DMACAHB_ERR_INVALID_CHNID        AL_DEF_ERR(AL_DMACAHB, AL_ERR_LEVEL_ERROR, AL_ERR_INVALID_CHNID)
#define AL_DMACAHB_ERR_ILLEGAL_PARAM        AL_DEF_ERR(AL_DMACAHB, AL_ERR_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM)
#define AL_DMACAHB_ERR_EXIST                AL_DEF_ERR(AL_DMACAHB, AL_ERR_LEVEL_ERROR, AL_ERR_EXIST)
#define AL_DMACAHB_ERR_UNEXIST              AL_DEF_ERR(AL_DMACAHB, AL_ERR_LEVEL_ERROR, AL_ERR_UNEXIST)
#define AL_DMACAHB_ERR_NULL_PTR             AL_DEF_ERR(AL_DMACAHB, AL_ERR_LEVEL_ERROR, AL_ERR_NULL_PTR)
#define AL_DMACAHB_ERR_NOT_CONFIG           AL_DEF_ERR(AL_DMACAHB, AL_ERR_LEVEL_ERROR, AL_ERR_NOT_CONFIG)
#define AL_DMACAHB_ERR_NOT_SUPPORT          AL_DEF_ERR(AL_DMACAHB, AL_ERR_LEVEL_ERROR, AL_ERR_NOT_SUPPORT)
#define AL_DMACAHB_ERR_TIMEOUT              AL_DEF_ERR(AL_DMACAHB, AL_ERR_LEVEL_ERROR, AL_ERR_TIMEOUT)
#define AL_DMACAHB_ERR_BUSY                 AL_DEF_ERR(AL_DMACAHB, AL_ERR_LEVEL_ERROR, AL_ERR_BUSY)
#define AL_DMACAHB_ERR_ENABLED_CHANNEL      AL_DEF_ERR(AL_DMACAHB, AL_ERR_LEVEL_ERROR, DMACAHB_ERR_ENABLED_CHANNEL)
#define AL_DMACAHB_ERR_INVALID_CH_ID        AL_DEF_ERR(AL_DMACAHB, AL_ERR_LEVEL_ERROR, DMACAHB_ERR_INVALID_CH_ID)
#define AL_DMACAHB_ERR_HANDLE_WITHOUT_CH    AL_DEF_ERR(AL_DMACAHB, AL_ERR_LEVEL_ERROR, DMACAHB_ERR_HANDLE_WITHOUT_CH)
#define AL_DMACAHB_ERR_CH_WITHOUT_DMAC      AL_DEF_ERR(AL_DMACAHB, AL_ERR_LEVEL_ERROR, DMACAHB_ERR_CH_WITHOUT_DMAC)
#define AL_DMACAHB_ERR_IOCTL_CMD            AL_DEF_ERR(AL_DMACAHB, AL_ERR_LEVEL_ERROR, DMACAHB_ERR_IOCTL_CMD)
#define AL_DMACAHB_ERR_ADDR_NOT_ALIGN       AL_DEF_ERR(AL_DMACAHB, AL_ERR_LEVEL_ERROR, DMACAHB_ERR_ADDR_NOT_ALIGN)
#define AL_DMACAHB_ERR_TRANS_TYPE           AL_DEF_ERR(AL_DMACAHB, AL_ERR_LEVEL_ERROR, DMACAHB_ERR_TRANS_TYPE)
#define AL_DMACAHB_ERR_TRANS_BUSY           AL_DEF_ERR(AL_DMACAHB, AL_ERR_LEVEL_ERROR, DMACAHB_ERR_TRANS_BUSY)
#define AL_DMACAHB_ERR_STATE_NOT_READY      AL_DEF_ERR(AL_DMACAHB, AL_ERR_LEVEL_ERROR, DMACAHB_ERR_STATE_NOT_READY)

/**************************** Type Definitions *******************************/
/**
 * @brief  Channel id enum
 */
typedef enum
{
    AL_DMACAHB_CHANNEL_0,
    AL_DMACAHB_CHANNEL_1,
    AL_DMACAHB_CHANNEL_2,
    AL_DMACAHB_CHANNEL_3,
    AL_DMACAHB_CHANNEL_4,
    AL_DMACAHB_CHANNEL_5,
    AL_DMACAHB_CHANNEL_6,
    AL_DMACAHB_CHANNEL_7
} AL_DMACAHB_ChIdEnum;

/**
 * @brief  Channel member enum
 */
typedef enum
{
    AL_DMACAHB_CH_MEMBER_SRC,
    AL_DMACAHB_CH_MEMBER_DST
} AL_DMACAHB_ChMemberEnum;

/**
 * @brief  Channel params struct
 */
typedef struct
{
    AL_U32  ChBaseOffset;
    AL_U32  ChMask;
} AL_DMACAHB_ChParamStruct;

/**
 * @brief  Channel intr id enum
 */
typedef enum
{
    AL_DMACAHB_CH_INTR_TFR   = BIT_32(0),
    AL_DMACAHB_CH_INTR_BLOCK = BIT_32(1),
    AL_DMACAHB_CH_INTR_SRCT  = BIT_32(2),
    AL_DMACAHB_CH_INTR_DSTT  = BIT_32(3),
    AL_DMACAHB_CH_INTR_ERR   = BIT_32(4)
} AL_DMACAHB_ChIntrEnum;

/**
 * @brief  Channel master sel enum
 */
typedef enum
{
    AL_DMACAHB_MASTER_SEL_1,
    AL_DMACAHB_MASTER_SEL_2
} AL_DMACAHB_MasterSelEnum;

/**
 * @brief  Channel link list pointer sel enum
 */
typedef enum
{
    AL_DMACAHB_LLP_NONE,
    AL_DMACAHB_LLP_DST,
    AL_DMACAHB_LLP_SRC,
    AL_DMACAHB_LLP_BOTH
} AL_DMACAHB_LlpSelEnum;

/**
 * @brief  Dmac ahb trans type enum
 */
typedef enum
{
    AL_DMACAHB_TRANS_TYPE_1,    /* Single-block or last transfer of multi-block,  */
    AL_DMACAHB_TRANS_TYPE_2,    /* Auto-reload multi-block transfer with contiguous SAR */
    AL_DMACAHB_TRANS_TYPE_3,    /* Auto-reload multi-block transfer with contiguous DAR */
    AL_DMACAHB_TRANS_TYPE_4,    /* Auto-reload multi-block transfer */
    AL_DMACAHB_TRANS_TYPE_5,    /* Single-block or last transfer of multi-block */
    AL_DMACAHB_TRANS_TYPE_6,    /* Linked list multi-block transfer with contiguous SAR */
    AL_DMACAHB_TRANS_TYPE_7,    /* Linked list multi-block transfer with auto-reload SAR */
    AL_DMACAHB_TRANS_TYPE_8,    /* Linked list multi-block transfer with contiguous DAR */
    AL_DMACAHB_TRANS_TYPE_9,    /* Linked list multi-block transfer with auto-reload DAR */
    AL_DMACAHB_TRANS_TYPE_10    /* Linked list multi-block transfer */
} AL_DMACAHB_TransTypeEnum;

/**
 * @brief  Dmac ahb event id enum
 */
typedef enum
{
    AL_DMACAHB_EVENT_TRANS_COMP         = BIT_32(1),
    AL_DMACAHB_EVENT_BLOCK_TRANS_COMP   = BIT_32(2),
    AL_DMACAHB_EVENT_SRC_TRANS_COMP     = BIT_32(3),
    AL_DMACAHB_EVENT_DST_TRANS_COMP     = BIT_32(4),
    AL_DMACAHB_EVENT_ERR                = BIT_32(5),
} AL_DMACAHB_EventIdEnum;

/**
 * @brief  Channel state enum
 */
typedef enum
{
    AL_DMACAHB_STATE_NOT_INIT               = (0x0),
    AL_DMACAHB_STATE_READY                  = BIT_32(1),
    AL_DMACAHB_STATE_TRANS_BUSY             = BIT_32(2),    /*Not use, to be remove*/
    AL_DMACAHB_STATE_BLOCK_TRANS_BUSY       = BIT_32(3),    /*Not use, to be remove*/
    AL_DMACAHB_STATE_SINGLE_MODE_BUSY       = BIT_32(4),
    AL_DMACAHB_STATE_RELOAD_MODE_BUSY       = BIT_32(5),
    AL_DMACAHB_STATE_LLP_MODE_BUSY          = BIT_32(6),
    AL_DMACAHB_STATE_LLP_RELOAD_MODE_BUSY   = BIT_32(7),
}AL_DMACAHB_ChStateEnum;

/**
 * @brief  Io ctl cmd enum
 */
typedef enum
{
    AL_DMACAHB_IOCTL_FILL_LLI_WITH_CTL,
    AL_DMACAHB_IOCTL_GET_STATE,
    AL_DMACAHB_IOCTL_SET_STATE,
    AL_DMACAHB_IOCTL_CLR_STATE,
    AL_DMACAHB_IOCTL_READ_CTL_LO_REG,
    AL_DMACAHB_IOCTL_READ_CTL_HI_REG,
    AL_DMACAHB_IOCTL_READ_CFG_LO_REG,
    AL_DMACAHB_IOCTL_READ_CFG_HI_REG,
    AL_DMACAHB_IOCTL_WRITE_CTL_LO_REG,
    AL_DMACAHB_IOCTL_WRITE_CTL_HI_REG,
    AL_DMACAHB_IOCTL_WRITE_CFG_LO_REG,
    AL_DMACAHB_IOCTL_WRITE_CFG_HI_REG,
    AL_DMACAHB_IOCTL_GET_PARAM_CHANNEL_0,
    AL_DMACAHB_IOCTL_GET_PARAM_CHANNEL_1,
    AL_DMACAHB_IOCTL_GET_PARAM_CHANNEL_2,
    AL_DMACAHB_IOCTL_GET_PARAM_CHANNEL_3,
    AL_DMACAHB_IOCTL_GET_PARAM_CHANNEL_4,
    AL_DMACAHB_IOCTL_GET_PARAM_CHANNEL_5,
    AL_DMACAHB_IOCTL_GET_PARAM_CHANNEL_6,
    AL_DMACAHB_IOCTL_GET_PARAM_CHANNEL_7,
    AL_DMACAHB_IOCTL_SET_CHANNEL_EN,
    AL_DMACAHB_IOCTL_SET_RELOAD_LAST_TRANS,
}AL_DMACAHB_IoCtlCmdEnum;

/**
 * @brief  Dmac ahb component parameters union
 */
typedef union
{
    AL_U32 Reg;
    struct {
        AL_U32 ChDtw:3;
        AL_U32 ChStw:3;
        AL_U32 ChStatDst:1;
        AL_U32 ChStatSrc:1;
        AL_U32 ChDstScaEn:1;
        AL_U32 ChSrcGatEn:1;
        AL_U32 ChLockEn:1;
        AL_U32 ChMultiBlkEn:1;
        AL_U32 ChCtlWbEn:1;
        AL_U32 ChHcLlp:1;
        AL_U32 ChFc:2;
        AL_U32 ChMaxMultiSize:3;
        AL_U32 ChDms:3;
        AL_U32 ChLms:3;
        AL_U32 ChSms:3;
        AL_U32 ChFifoDepth:3;
        AL_U32 Rsvd31:1;
    }Bit;
} AL_DMACAHB_DmaCompChParamsUnion;

/**
 * @brief  Dmac ahb control low register union
 */
typedef union
{
    AL_U32 Reg;
    struct {
        AL_U32      IntEn:1;
        AL_U32      DstTransWidth:3;
        AL_U32      SrcTransWidth:3;
        AL_U32      DstInc:2;
        AL_U32      SrcInc:2;
        AL_U32      DstMsize:3;
        AL_U32      SrcMsize:3;
        AL_U32      SrcGattterEn:1;
        AL_U32      DstScatterEn:1;
        AL_U32      RsvdCtl:1;
        AL_U32      TtFc:3;
        AL_U32      DstMasterSel:2;
        AL_U32      SrcMasterSel:2;
        AL_U32      LlpDstEn:1;
        AL_U32      LlpSrcEn:1;
        AL_U32      Rsvd1Ctl:3;
    }Bit;
} AL_DMACAHB_CtlLoUnion;

/**
 * @brief  Dmac ahb control high register union
 */
typedef union
{
    AL_U32 Reg;
    struct {
        AL_U32      BlockTransSize:12;
        AL_U32      Done:1;
        AL_U32      Rsvd3Ctl:19;
    }Bit;
} AL_DMACAHB_CtlHiUnion;

/**
 * @brief  Dmac ahb config low register union
 */
typedef union
{
    AL_U32 Reg;
    struct {
        AL_U32      RsvdCfg:5;
        AL_U32      ChPrior:3;
        AL_U32      ChSusp:1;
        AL_U32      FifoEmpty:1;
        AL_U32      DstHsSel:1;
        AL_U32      SrcHsSel:1;
        AL_U32      Rsvd17_12:6;
        AL_U32      DstHsPol:1;
        AL_U32      SrcHsPol:1;
        AL_U32      Rsvd29_20:10;
        AL_U32      AutoSrcReloadEn:1;
        AL_U32      AutoDstReloadEn:1;
    }Bit;
} AL_DMACAHB_CfgLoUnion;

/**
 * @brief  Dmac ahb config high register union
 */
typedef union
{
    AL_U32 Reg;
    struct {
        AL_U32      FcMode:1;
        AL_U32      FifoMode:1;
        AL_U32      ProtCtl:3;
        AL_U32      Rsvd38_37:2;
        AL_U32      SrcPer:4;
        AL_U32      DstPer:4;
        AL_U32      Rsvd63_47:17;
    }Bit;
} AL_DMACAHB_CfgHiUnion;

/**
 * @brief  Dmac ahb link list item struct
 */
typedef struct
{
    AL_U32                      SrcAddr;
    AL_U32                      DstAddr;
    AL_U32                      LlpNext;
    AL_DMACAHB_CtlLoUnion       CtlLow;
    AL_DMACAHB_CtlHiUnion       CtlHigh;
} AL_DMACAHB_LliStruct;

/**
 * @brief  Dmac ahb trans config struct
 */
typedef struct
{
    AL_U32                  SrcAddr;
    AL_U32                  DstAddr;
    AL_U32                  TransSize;
    AL_U32                  ReloadCountNum;
    AL_U32                  ReloadCount;
    AL_DMACAHB_LliStruct    *Lli;
} AL_DMACAHB_ChTransStruct;

/**
 * @brief  Dmac ahb current state struct
 */
typedef struct
{
    AL_BOOL     DmacEn;
    AL_BOOL     IntrEn;
    AL_U8       ChEn;       /* channel enabled status */
} AL_DMACAHB_StateStruct;

/**
 * @brief  Dmac ahb event struct
 */
typedef struct
{
    AL_DMACAHB_EventIdEnum  EventId;
    AL_U32                  EventData;
} AL_DMACAHB_EventStruct;

typedef AL_VOID (*AL_DMACAHB_ChEventCallBack)(AL_DMACAHB_EventStruct *Event, AL_VOID *CallBackRef);

/**
 * @brief  Channel call back struct
 */
typedef struct
{
    AL_DMACAHB_ChEventCallBack  Func;
    AL_VOID                     *Ref;
} AL_DMACAHB_ChCallBackStruct;

struct DMACAHB_ChStruct;

/**
 * @brief  Dmac ahb dev struct
 */
typedef struct
{
    AL_REG                  BaseAddr;
    AL_U32                  DevId;
    AL_U32                  IntrId;
    AL_U32                  ChannelNum;
    AL_DMACAHB_StateStruct  State;
    struct DMACAHB_ChStruct *Channel[AL_DMACAHB_CHANNEL_NUM];
} AL_DMACAHB_DmacStruct;

/**
 * @brief  Channel hand shaking config struct
 */
typedef struct
{
    AL_DMACAHB_HsSelEnum        SrcHsSel;
    AL_DMACAHB_HsSelEnum        DstHsSel;
    AL_DMACAHB_HsPolSelEnum     SrcHsPol;
    AL_DMACAHB_HsPolSelEnum     DstHsPol;
    AL_DMACAHB_PerSelEnum       SrcPer;
    AL_DMACAHB_PerSelEnum       DstPer;
} AL_DMACAHB_ChHsStruct;

/**
 * @brief  Channel source gather & destination scatter struct
 */
typedef struct
{
    AL_BOOL         IsSrcGatherEn;
    AL_U32          SrcGatherCount;
    AL_U32          SrcGatherInterval;
    AL_BOOL         IsDstScatterEn;
    AL_U32          DstScatterCount;
    AL_U32          DstScatterInterval;
} AL_DMACAHB_ChSgrDsrStruct;

/**
 * @brief  Channel intr struct
 */
typedef struct
{
    AL_BOOL                 IsIntrEn;
    AL_DMACAHB_ChIntrEnum   IntrUnMask;
} AL_DMACAHB_ChIntrStruct;

/**
 * @brief  Channel init struct
 */
typedef struct
{
    AL_DMACAHB_ChIdEnum         Id;
    AL_DMACAHB_TransTypeEnum    TransType;
    AL_DMACAHB_ChIntrStruct     Intr;
    AL_DMACAHB_TransWidthEnum   SrcTransWidth;
    AL_DMACAHB_TransWidthEnum   DstTransWidth;
    AL_DMACAHB_AddrIncEnum      SrcAddrIncMode;
    AL_DMACAHB_AddrIncEnum      DstAddrIncMode;
    AL_DMACAHB_MsizeEnum        SrcBurstLength;
    AL_DMACAHB_MsizeEnum        DstBurstLength;
    AL_DMACAHB_TtFcEnum         Direction;
    AL_DMACAHB_MasterSelEnum    SrcMasterSel;
    AL_DMACAHB_MasterSelEnum    DstMasterSel;
    AL_DMACAHB_MasterSelEnum    ListMasterSel;
    AL_DMACAHB_ChPriorEnum      ChPrior;
    AL_DMACAHB_FifoModeEnum     FifoMode;
    AL_DMACAHB_ProtCtlSelEnum   ProtCtl;
    AL_DMACAHB_ChHsStruct       HandShaking;
    AL_DMACAHB_ChSgrDsrStruct   SgrDsr;
} AL_DMACAHB_ChInitStruct;

/**
 * @brief  Channel dev struct
 */
typedef struct DMACAHB_ChStruct
{
    AL_DMACAHB_ChParamStruct    Param;
    AL_DMACAHB_DmacStruct       *Dmac;
    AL_DMACAHB_ChInitStruct     Config;
    AL_DMACAHB_ChTransStruct    Trans;
    AL_DMACAHB_ChStateEnum      State;
    AL_DMACAHB_ChCallBackStruct EventCallBack;
} AL_DMACAHB_ChStruct;

/************************** Variable Definitions *****************************/

/************************** Function Prototypes ******************************/
AL_DMACAHB_HwConfigStruct *AlDmacAhb_Dev_LookupConfig(AL_U32 DeviceId);

AL_S32 AlDmacAhb_Dev_Init(AL_DMACAHB_ChStruct *Channel, AL_DMACAHB_HwConfigStruct *HwConfig,
                          AL_DMACAHB_ChInitStruct *InitConfig);

AL_S32 AlDmacAhb_Dev_DeInit(AL_DMACAHB_ChStruct *Channel);

AL_VOID AlDmacAhb_Dev_FillLliWithCtl(AL_DMACAHB_ChStruct *Channel, AL_DMACAHB_LliStruct *Lli);

AL_S32 AlDmacAhb_Dev_SetTransParams(AL_DMACAHB_ChStruct *Channel);

AL_S32 AlDmacAhb_Dev_Start(AL_DMACAHB_ChStruct *Channel);

AL_S32 AlDmacAhb_Dev_UnRegisterIntrHandler(AL_DMACAHB_ChStruct *Channel);

AL_S32 AlDmacAhb_Dev_RegisterChEventCallBack(AL_DMACAHB_ChStruct *Channel, AL_DMACAHB_ChCallBackStruct *CallBack);

AL_S32 AlDmacAhb_Dev_UnRegisterChEventCallBack(AL_DMACAHB_ChStruct *Channel);

AL_VOID AlDmacAhb_Dev_IntrHandler(AL_VOID *Instance);

AL_S32 AlDmacAhb_Dev_IoCtl(AL_DMACAHB_ChStruct *Channel, AL_DMACAHB_IoCtlCmdEnum Cmd, AL_VOID *Data);

AL_S32 AlDmacAhb_Dev_TransTypeToState(AL_DMACAHB_TransTypeEnum Type, AL_DMACAHB_ChStateEnum *State);

#ifdef __cplusplus
}
#endif

#endif
