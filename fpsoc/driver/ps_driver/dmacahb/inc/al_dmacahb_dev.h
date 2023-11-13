/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    al_dmacahb_dev.h
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2023-04-17
 * @brief   dmac ahb device driver
 */

#ifndef __AL_DMACAHB_DEV_H_
#define __AL_DMACAHB_DEV_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#include "al_dmacahb_ll.h"

/************************** Constant Definitions *****************************/

/***************** Macros (Inline Functions) Definitions *********************/
#define AL_DMACAHB_CHANNEL_NUM  (8)
#define AL_DMACAHB_CH_INTR_NUM  (5)

#define DMACAHB_EVENT_START_BIT 6

typedef enum
{
    DMACAHB_ERR_ENABLED_CHANNEL = AL_ERR_MAX,    /* receive message with error*/
    DMACAHB_ERR_NONE_AVAILABLE_CH,
    DMACAHB_ERR_HANDLE_WITHOUT_CH,
    DMACAHB_ERR_CH_WITHOUT_DMAC,
    DMACAHB_ERR_IOCTL_CMD,
    DMACAHB_ERR_ADDR_NOT_ALIGN,
    DMACAHB_ERR_TRANS_TYPE,
    DMACAHB_ERR_TRANS_BUSY,
    DMACAHB_ERR_STATE_NOT_READY,
    DMACAHB_ERR_FETCH_LOCK,
    DMACAHB_ERR_UNSUPPORT_MODE,
} AL_DMACAHB_ErrCodeEnum;

#define AL_DMACAHB_ERR_ILLEGAL_PARAM        AL_DEF_ERR(AL_DMACAHB, AL_LOG_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM)
#define AL_DMACAHB_ERR_NULL_PTR             AL_DEF_ERR(AL_DMACAHB, AL_LOG_LEVEL_ERROR, AL_ERR_NULL_PTR)
#define AL_DMACAHB_ERR_NOT_SUPPORT          AL_DEF_ERR(AL_DMACAHB, AL_LOG_LEVEL_ERROR, AL_ERR_NOT_SUPPORT)
#define AL_DMACAHB_ERR_TIMEOUT              AL_DEF_ERR(AL_DMACAHB, AL_LOG_LEVEL_ERROR, AL_ERR_TIMEOUT)
#define AL_DMACAHB_ERR_BUSY                 AL_DEF_ERR(AL_DMACAHB, AL_LOG_LEVEL_ERROR, AL_ERR_BUSY)
#define AL_DMACAHB_ERR_ENABLED_CHANNEL      AL_DEF_ERR(AL_DMACAHB, AL_LOG_LEVEL_ERROR, DMACAHB_ERR_ENABLED_CHANNEL)
#define AL_DMACAHB_ERR_NONE_AVAILABLE_CH    AL_DEF_ERR(AL_DMACAHB, AL_LOG_LEVEL_ERROR, DMACAHB_ERR_NONE_AVAILABLE_CH)
#define AL_DMACAHB_ERR_HANDLE_WITHOUT_CH    AL_DEF_ERR(AL_DMACAHB, AL_LOG_LEVEL_ERROR, DMACAHB_ERR_HANDLE_WITHOUT_CH)
#define AL_DMACAHB_ERR_CH_WITHOUT_DMAC      AL_DEF_ERR(AL_DMACAHB, AL_LOG_LEVEL_ERROR, DMACAHB_ERR_CH_WITHOUT_DMAC)
#define AL_DMACAHB_ERR_IOCTL_CMD            AL_DEF_ERR(AL_DMACAHB, AL_LOG_LEVEL_ERROR, DMACAHB_ERR_IOCTL_CMD)
#define AL_DMACAHB_ERR_ADDR_NOT_ALIGN       AL_DEF_ERR(AL_DMACAHB, AL_LOG_LEVEL_ERROR, DMACAHB_ERR_ADDR_NOT_ALIGN)
#define AL_DMACAHB_ERR_TRANS_TYPE           AL_DEF_ERR(AL_DMACAHB, AL_LOG_LEVEL_ERROR, DMACAHB_ERR_TRANS_TYPE)
#define AL_DMACAHB_ERR_TRANS_BUSY           AL_DEF_ERR(AL_DMACAHB, AL_LOG_LEVEL_ERROR, DMACAHB_ERR_TRANS_BUSY)
#define AL_DMACAHB_ERR_STATE_NOT_READY      AL_DEF_ERR(AL_DMACAHB, AL_LOG_LEVEL_ERROR, DMACAHB_ERR_STATE_NOT_READY)
#define AL_DMACAHB_ERR_FETCH_LOCK           AL_DEF_ERR(AL_DMACAHB, AL_LOG_LEVEL_ERROR, DMACAHB_ERR_FETCH_LOCK)
#define AL_DMACAHB_ERR_UNSUPPORT_MODE       AL_DEF_ERR(AL_DMACAHB, AL_LOG_LEVEL_ERROR, DMACAHB_ERR_UNSUPPORT_MODE)
#define AL_DMACAHB_EVENTS_TO_ERRS(Events)   (AL_DEF_ERR(AL_DMACAHB, AL_LOG_LEVEL_ERROR, \
                                                        (Events << DMACAHB_EVENT_START_BIT)))

#define AL_DMACAHB_RELOAD_CNT_MAX           (0xFFFFFFFF)

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
    AL_DMACAHB_CHANNEL_7,
    AL_DMACAHB_CHANNEL_UNAVAILABLE
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
    AL_DMACAHB_CH_INTR_TFR   = BIT(0),
    AL_DMACAHB_CH_INTR_BLOCK = BIT(1),
    AL_DMACAHB_CH_INTR_SRCT  = BIT(2),
    AL_DMACAHB_CH_INTR_DSTT  = BIT(3),
    AL_DMACAHB_CH_INTR_ERR   = BIT(4)
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
 * @brief  Channel soft request enum
 */
typedef enum
{
    AL_DMACAHB_SOFT_REQ_BURST,
    AL_DMACAHB_SOFT_REQ_SINGLE
} AL_DMACAHB_SoftReqEnum;

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
    AL_DMACAHB_TRANS_TYPE_1,    /* Single-block or last transfer of multi-block, tfr intr done */
    AL_DMACAHB_TRANS_TYPE_2,    /* Auto-reload multi-block transfer with contiguous SAR, blk intr done */
    AL_DMACAHB_TRANS_TYPE_3,    /* Auto-reload multi-block transfer with contiguous DAR, blk intr done */
    AL_DMACAHB_TRANS_TYPE_4,    /* Auto-reload multi-block transfer, blk intr done */
    AL_DMACAHB_TRANS_TYPE_5,    /* Single-block or last transfer of multi-block, tfr intr done */
    AL_DMACAHB_TRANS_TYPE_6,    /* Linked list multi-block transfer with contiguous SAR, tfr intr done */
    AL_DMACAHB_TRANS_TYPE_7,    /* Linked list multi-block transfer with auto-reload SAR, blk intr done */
    AL_DMACAHB_TRANS_TYPE_8,    /* Linked list multi-block transfer with contiguous DAR, tfr intr done */
    AL_DMACAHB_TRANS_TYPE_9,    /* Linked list multi-block transfer with auto-reload DAR, blk intr done */
    AL_DMACAHB_TRANS_TYPE_10    /* Linked list multi-block transfer, tfr intr done */
} AL_DMACAHB_TransTypeEnum;

/**
 * @brief  Dmac ahb event id enum
 */
typedef enum
{
    AL_DMACAHB_EVENT_TRANS_READY        = BIT(0),
    AL_DMACAHB_EVENT_TRANS_COMP         = BIT(1),
    AL_DMACAHB_EVENT_BLOCK_TRANS_COMP   = BIT(2),
    AL_DMACAHB_EVENT_SRC_TRANS_COMP     = BIT(3),
    AL_DMACAHB_EVENT_DST_TRANS_COMP     = BIT(4),
    AL_DMACAHB_EVENT_ERR                = BIT(5),
    AL_DMACAHB_EVENT_RELOAD             = BIT(6),
} AL_DMACAHB_EventIdEnum;

/**
 * @brief  Channel state enum
 */
typedef enum
{
    AL_DMACAHB_STATE_NOT_INIT               = (0x0),
    AL_DMACAHB_STATE_READY                  = BIT(1),
    AL_DMACAHB_STATE_TRANS_BUSY             = BIT(2),    /*Not use, to be remove*/
    AL_DMACAHB_STATE_BLOCK_TRANS_BUSY       = BIT(3),    /*Not use, to be remove*/
    AL_DMACAHB_STATE_SINGLE_MODE_BUSY       = BIT(4),
    AL_DMACAHB_STATE_RELOAD_MODE_BUSY       = BIT(5),
    AL_DMACAHB_STATE_LLP_MODE_BUSY          = BIT(6),
    AL_DMACAHB_STATE_LLP_RELOAD_MODE_BUSY   = BIT(7),
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
    AL_DMACAHB_IOCTL_SET_RELOAD_LAST_TRANS, /* TODO: increase width/size... params set */
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
    AL_U32                      LlpNext;    /* Addr of next link list item */
    AL_DMACAHB_CtlLoUnion       CtlLow;
    AL_DMACAHB_CtlHiUnion       CtlHigh;    /* Ctl high reg, if write back enabled, save trans state in done bit */
} AL_DMACAHB_LliStruct;

/**
 * @brief  Dmac ahb trans config struct
 */
typedef struct
{
    AL_U32                  SrcAddr;
    AL_U32                  DstAddr;
    AL_U32                  TransSize;      /* Size in Src trans width */
    AL_U32                  ReloadCountNum; /* use in reload mode, save reload times */
    AL_U32                  ReloadCount;    /* save current reload times */
    AL_DMACAHB_LliStruct    *Lli;           /* Pointer to link list item */
    AL_U32                  SrcBurstCnt;    /* soft request remain burst cnt */
    AL_U32                  SrcSingleCnt;   /* soft request remain single cnt */
} AL_DMACAHB_ChTransStruct;

/**
 * @brief  Dmac ahb current state struct
 */
typedef struct
{
    AL_BOOL     DmacEn;
    AL_BOOL     IntrEn;
    AL_U8       ChEn;       /* indicate used channel in this core for intr handler */
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

struct DMACAHB_ChStruct;

/**
 * @brief  Dmac ahb dev struct, basicly corresponds to AlDmacAhb_HwConfig
 */
typedef struct
{
    AL_REG                  BaseAddr;                           /* Dmac Base address */
    AL_U32                  DevId;                              /* Num of Dmac, start from 0 */
    AL_U32                  IntrId;                             /* Intr id in intr list */
    AL_U32                  ChannelNum;                         /* How many Channel in this controller */
    AL_U32                  LockAddress;                        /* Spin lock address */
    AL_U32                  ChStateAddr;                        /* Channel state address */
    AL_DMACAHB_StateStruct  State;                              /* Controller state */
    struct DMACAHB_ChStruct *Channel[AL_DMACAHB_CHANNEL_NUM];   /* Poniter to its channel struct */
} AL_DMACAHB_DmacStruct;

/**
 * @brief  Channel hand shaking config struct
 */
typedef struct
{
    AL_DMACAHB_HsSelEnum        SrcHsSel;   /* Src handshaking, Soft or hard */
    AL_DMACAHB_HsSelEnum        DstHsSel;   /* Dst handshaking, Soft or hard */
    AL_DMACAHB_HsPolSelEnum     SrcHsPol;   /* Src handshaking polarity */
    AL_DMACAHB_HsPolSelEnum     DstHsPol;   /* Dst handshaking polarity */
    AL_DMACAHB_PerSelEnum       SrcPer;     /* Src hardware handshaking select, ignore in soft handshaking */
    AL_DMACAHB_PerSelEnum       DstPer;     /* Dst hardware handshaking select, ignore in soft handshaking */
} AL_DMACAHB_ChHsStruct;

/**
 * @brief  Channel source gather & destination scatter struct
 */
typedef struct
{
    AL_BOOL         IsSrcGatherEn;
    AL_U32          SrcGatherCount;         /* Src contiguous trans count */
    AL_U32          SrcGatherInterval;      /* Src gather interval between contiguous trans in system memory */
    AL_BOOL         IsDstScatterEn;
    AL_U32          DstScatterCount;        /* Dst contiguous recv count */
    AL_U32          DstScatterInterval;     /* Src scatter interval between contiguous recv in system memory */
} AL_DMACAHB_ChSgrDsrStruct;

/**
 * @brief  Channel intr struct
 */
typedef struct
{
    AL_BOOL IsIntrEn;
    AL_U8   IntrUnMask;     /* Intr unmask state, reference to AL_DMACAHB_ChIntrEnum */
} AL_DMACAHB_ChIntrStruct;

/**
 * @brief  Channel init struct
 */
typedef struct
{
    AL_DMACAHB_ChIdEnum         Id;             /* Channel Num */
    AL_DMACAHB_TransTypeEnum    TransType;      /* reference to AL_DMACAHB_TransTypeEnum */
    AL_DMACAHB_ChIntrStruct     Intr;           /* Intr enable/disable and umask state */
    AL_DMACAHB_TransWidthEnum   SrcTransWidth;  /* Src trans width in bit */
    AL_DMACAHB_TransWidthEnum   DstTransWidth;  /* Dst trans width in bit */
    AL_DMACAHB_AddrIncEnum      SrcAddrIncMode; /* Src address auto inc mode, reference to AL_DMACAHB_AddrIncEnum */
    AL_DMACAHB_AddrIncEnum      DstAddrIncMode; /* Dst address auto inc mode, reference to AL_DMACAHB_AddrIncEnum */
    AL_DMACAHB_MsizeEnum        SrcBurstLength; /* Src burst trans length in SrcTransWidth */
    AL_DMACAHB_MsizeEnum        DstBurstLength; /* Dst burst trans length in DstTransWidth */
    AL_DMACAHB_TtFcEnum         Direction;      /* Trans direction, reference to AL_DMACAHB_TtFcEnum */
    AL_DMACAHB_MasterSelEnum    SrcMasterSel;   /* Src trans master interface select */
    AL_DMACAHB_MasterSelEnum    DstMasterSel;   /* Dst trans master interface select */
    AL_DMACAHB_MasterSelEnum    ListMasterSel;  /* Link list item trans master interface select */
    AL_DMACAHB_ChPriorEnum      ChPrior;        /* Channel priority, 7 is highest priority */
    AL_DMACAHB_FifoModeEnum     FifoMode;       /* reference to AL_DMACAHB_FifoModeEnum */
    AL_DMACAHB_ProtCtlSelEnum   ProtCtl;        /* Corresponds to AHB bus prot wire */
    AL_DMACAHB_ChHsStruct       HandShaking;    /* Hardware handshaking cofig */
    AL_DMACAHB_ChSgrDsrStruct   SgrDsr;         /* Src gather and Dst scatter */
} AL_DMACAHB_ChInitStruct;

/**
 * @brief  Channel dev struct
 */
typedef struct DMACAHB_ChStruct
{
    AL_DMACAHB_ChParamStruct    Param;              /* Channel register offset and mask */
    AL_DMACAHB_DmacStruct       *Dmac;              /* Pointer to which dmac belogs to */
    AL_DMACAHB_ChInitStruct     Config;             /* Function config */
    AL_DMACAHB_ChTransStruct    Trans;              /* Trans addr, size, count and Link list item */
    AL_DMACAHB_ChStateEnum      State;              /* Current Channel state */
    AL_DMACAHB_ChEventCallBack  EventCallBack;      /* Callback for user to handle */
    AL_VOID                     *EventCallBackRef;
} AL_DMACAHB_ChStruct;

/************************** Variable Definitions *****************************/

/************************** Function Prototypes ******************************/
AL_DMACAHB_HwConfigStruct *AlDmacAhb_Dev_LookupConfig(AL_U32 DeviceId);

AL_S32 AlDmacAhb_Dev_Init(AL_DMACAHB_ChStruct *Channel, AL_DMACAHB_HwConfigStruct *HwConfig,
                          AL_DMACAHB_ChInitStruct *InitConfig);

AL_S32 AlDmacAhb_Dev_DeInit(AL_DMACAHB_ChStruct *Channel);

AL_S32 AlDmacAhb_Dev_FillLliWithCtl(AL_DMACAHB_ChStruct *Channel, AL_DMACAHB_LliStruct *Lli);

AL_S32 AlDmacAhb_Dev_SetTransParams(AL_DMACAHB_ChStruct *Channel);

AL_S32 AlDmacAhb_Dev_SoftRequest(AL_DMACAHB_ChStruct *Channel);

AL_S32 AlDmacAhb_Dev_Start(AL_DMACAHB_ChStruct *Channel);

AL_S32 AlDmacAhb_Dev_RegisterChEventCallBack(AL_DMACAHB_ChStruct *Channel, AL_DMACAHB_ChEventCallBack CallBack,
                                             AL_VOID *CallBackRef);

AL_S32 AlDmacAhb_Dev_UnRegisterChEventCallBack(AL_DMACAHB_ChStruct *Channel);

AL_VOID AlDmacAhb_Dev_IntrHandler(AL_VOID *Instance);

AL_S32 AlDmacAhb_Dev_IoCtl(AL_DMACAHB_ChStruct *Channel, AL_DMACAHB_IoCtlCmdEnum Cmd, AL_VOID *Data);

AL_S32 AlDmacAhb_Dev_TransTypeToState(AL_DMACAHB_TransTypeEnum Type, AL_DMACAHB_ChStateEnum *State);

AL_S32 AlDmacAhb_Dev_RequestCh(AL_DMACAHB_HwConfigStruct   *HwConfig, AL_DMACAHB_ChIdEnum RequestId,
                               AL_DMACAHB_ChIdEnum *AvailableId);

AL_S32 AlDmacAhb_Dev_ReleaseCh(AL_DMACAHB_ChStruct *Channel);

#ifdef __cplusplus
}
#endif

#endif
