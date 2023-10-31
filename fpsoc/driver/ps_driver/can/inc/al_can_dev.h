/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    al_can_dev.h
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2023-04-7
 * @brief   can device driver
 */

#ifndef __AL_CAN_DEV_H_
#define __AL_CAN_DEV_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#include "al_can_ll.h"

/************************** Constant Definitions *****************************/

/***************** Macros (Inline Functions) Definitions *********************/

#define CAN_EVENT_START_BIT     6

/**
 * @brief  Error code enum
 */
typedef enum
{
    CAN_ERR_RECV                    = AL_ERR_MAX,
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
    CAN_ERR_STATE_RESET,
    CAN_ERR_IOCTL_CMD,
    CAN_ERR_DMA_CONFIG,
    CAN_ERR_IN_STANDBY_MODE,
} AL_CAN_ErrCodeEnum;

#define AL_CAN_ERR_ILLEGAL_PARAM        AL_DEF_ERR(AL_CAN, AL_LOG_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM)
#define AL_CAN_ERR_NULL_PTR             AL_DEF_ERR(AL_CAN, AL_LOG_LEVEL_ERROR, AL_ERR_NULL_PTR)
#define AL_CAN_ERR_NOT_SUPPORT          AL_DEF_ERR(AL_CAN, AL_LOG_LEVEL_ERROR, AL_ERR_NOT_SUPPORT)
#define AL_CAN_ERR_TIMEOUT              AL_DEF_ERR(AL_CAN, AL_LOG_LEVEL_ERROR, AL_ERR_TIMEOUT)
#define AL_CAN_ERR_BUSY                 AL_DEF_ERR(AL_CAN, AL_LOG_LEVEL_ERROR, AL_ERR_BUSY)
#define AL_CAN_ERR_NOMEM                AL_DEF_ERR(AL_CAN, AL_LOG_LEVEL_ERROR, AL_ERR_NOMEM)
#define AL_CAN_ERR_RECV                 AL_DEF_ERR(AL_CAN, AL_LOG_LEVEL_ERROR, CAN_ERR_RECV)
#define AL_CAN_ERR_WRONG_KOER_PARAMETER AL_DEF_ERR(AL_CAN, AL_LOG_LEVEL_ERROR, CAN_ERR_WRONG_KOER_PARAMETER)
#define AL_CAN_ERR_KOER_RECV_BIT        AL_DEF_ERR(AL_CAN, AL_LOG_LEVEL_ERROR, CAN_ERR_KOER_RECV_BIT)
#define AL_CAN_ERR_KOER_SEND_BIT        AL_DEF_ERR(AL_CAN, AL_LOG_LEVEL_ERROR, CAN_ERR_KOER_SEND_BIT)
#define AL_CAN_ERR_KOER_FORM            AL_DEF_ERR(AL_CAN, AL_LOG_LEVEL_ERROR, CAN_ERR_KOER_FORM)
#define AL_CAN_ERR_KOER_RECV_STUFF      AL_DEF_ERR(AL_CAN, AL_LOG_LEVEL_ERROR, CAN_ERR_KOER_RECV_STUFF)
#define AL_CAN_ERR_KOER_SEND_STUFF      AL_DEF_ERR(AL_CAN, AL_LOG_LEVEL_ERROR, CAN_ERR_KOER_SEND_STUFF)
#define AL_CAN_ERR_KOER_RECV_ACK        AL_DEF_ERR(AL_CAN, AL_LOG_LEVEL_ERROR, CAN_ERR_KOER_RECV_ACK)
#define AL_CAN_ERR_KOER_SEND_ACK        AL_DEF_ERR(AL_CAN, AL_LOG_LEVEL_ERROR, CAN_ERR_KOER_SEND_ACK)
#define AL_CAN_ERR_KOER_CRC             AL_DEF_ERR(AL_CAN, AL_LOG_LEVEL_ERROR, CAN_ERR_KOER_CRC)
#define AL_CAN_ERR_KOER_OTHER           AL_DEF_ERR(AL_CAN, AL_LOG_LEVEL_ERROR, CAN_ERR_KOER_OTHER)
#define AL_CAN_ERR_RECV_EMPTY           AL_DEF_ERR(AL_CAN, AL_LOG_LEVEL_INFO, CAN_ERR_RECV_EMPTY)
#define AL_CAN_ERR_STATE_NOT_READY      AL_DEF_ERR(AL_CAN, AL_LOG_LEVEL_ERROR, CAN_ERR_STATE_NOT_READY)
#define AL_CAN_ERR_STATE_RESET          AL_DEF_ERR(AL_CAN, AL_LOG_LEVEL_ERROR, CAN_ERR_STATE_RESET)
#define AL_CAN_ERR_IOCTL_CMD            AL_DEF_ERR(AL_CAN, AL_LOG_LEVEL_ERROR, CAN_ERR_IOCTL_CMD)
#define AL_CAN_ERR_DMA_CONFIG           AL_DEF_ERR(AL_CAN, AL_LOG_LEVEL_ERROR, CAN_ERR_DMA_CONFIG)
#define AL_CAN_ERR_IN_STANDBY_MODE      AL_DEF_ERR(AL_CAN, AL_LOG_LEVEL_ERROR, CAN_ERR_IN_STANDBY_MODE)
#define AL_CAN_EVENTS_TO_ERRS(Events)   (AL_DEF_ERR(AL_CAN, AL_LOG_LEVEL_ERROR, (Events << CAN_EVENT_START_BIT)))

/**************************** Type Definitions *******************************/
/**
 * @brief  Run mode enum
 */
typedef enum
{
    AL_CAN_RUN_NOT_SET,
    AL_CAN_RUN_INTR,
    AL_CAN_RUN_INTR_DMA
} AL_CAN_RunModeEnum;

/**
 * @brief  Protocol type enum
 */
typedef enum
{
    AL_CAN_TYPE_NOT_SET,
    AL_CAN_TYPE_2_0B,
    AL_CAN_TYPE_FD
} AL_CAN_TypeEnum;

/**
 * @brief  Frame id enum
 */
typedef enum
{
    AL_CAN_ID_STD,
    AL_CAN_ID_EXT
} AL_CAN_IdTypeEnum;

/**
 * @brief  Bit rate enum
 */
typedef enum
{
    AL_CAN_ARBITRATION_0_25M,
    AL_CAN_ARBITRATION_0_5M,
    AL_CAN_0_5M,
    AL_CAN_0_833M,
    AL_CAN_1_M,
    AL_CAN_1_538M,
    AL_CAN_2M,
    AL_CAN_3_077M,
    AL_CAN_4M,
    AL_CAN_5M,
    AL_CAN_6_667M,
    AL_CAN_8M,
    AL_CAN_10M
} AL_CAN_BitRateEnum;

/**
 * @brief  Frame data length DLC enum
 */
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
} AL_CAN_DataLenEnum;

/**
 * @brief  Frame bit rate type enum
 */
typedef enum
{
    AL_CAN_BIT_S,
    AL_CAN_BIT_F
} AL_CAN_BitRateTypeEnum;

/**
 * @brief  Operation mode enum
 */
typedef enum
{
    AL_CAN_MODE_NOT_SET,
    AL_CAN_MODE_NORMAL,
    AL_CAN_MODE_IN_LOOPBACK,
    AL_CAN_MODE_EX_LOOPBACK,
    AL_CAN_MODE_STANDBY,
    AL_CAN_MODE_LISTENONLY
} AL_CAN_OpsModeEnum;

/**
 * @brief  Frame trans mode enum
 */
typedef enum
{
    AL_CAN_TRANS_FULL,
    AL_CAN_TRANS_PTB,
    AL_CAN_TRANS_STB_FIFO,
    AL_CAN_TRANS_STB_PRIO,
    AL_CAN_TRANS_MAX
} AL_CAN_TransModeEnum;

/**
 * @brief  ACF mask mode enum
 */
typedef enum
{
    AL_CAN_SELMASK_ACODE,
    AL_CAN_SELMASK_AMASK
} AL_CAN_SelMaskEnum;

/**
 * @brief  Recv buffer almost full limit enum
 */
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
} AL_CAN_RbAwflEnum;

/**
 * @brief  Event id enum
 */
typedef enum
{
    AL_CAN_EVENT_SEND_READY         = BIT(0),
    AL_CAN_EVENT_SEND_DONE          = BIT(1),
    AL_CAN_EVENT_RECV_READY         = BIT(2),
    AL_CAN_EVENT_RECV_DONE          = BIT(3),
    AL_CAN_EVENT_RBUFF_ALMOST_FULL  = BIT(4),
    AL_CAN_EVENT_RBUFF_FULL         = BIT(5),
    AL_CAN_EVENT_RBUFF_OVERFLOW     = BIT(6),
    AL_CAN_EVENT_ABORT              = BIT(7),
    AL_CAN_EVENT_ERR                = BIT(8),
    AL_CAN_EVENT_BUS_ERR            = BIT(9),
    AL_CAN_EVENT_ARBITRATION_LOST   = BIT(10),
    AL_CAN_EVENT_ERR_PASSIVE        = BIT(11)
} AL_CAN_EventIdEnum;

/**
 * @brief  Filter enum
 */
typedef enum
{
    AL_CAN_FILTER_0,
    AL_CAN_FILTER_1,
    AL_CAN_FILTER_2,
    AL_CAN_FILTER_MAX
} AL_CAN_FilterIndexEnum;

/**
 * @brief  Filter mask type enum
 */
typedef enum
{
    AL_CAN_FILTER_MASK_BOTH = (0b00),
    AL_CAN_FILTER_MASK_STD  = (0b10),
    AL_CAN_FILTER_MASK_EXT  = (0b11)
} AL_CAN_FilterMaskTypeEnum;

/**
 * @brief  Frame error enum
 */
typedef enum
{
    AL_CAN_KOER_NONE,
    AL_CAN_KOER_BIT,
    AL_CAN_KOER_FORM,
    AL_CAN_KOER_STUFF,
    AL_CAN_KOER_ACK,
    AL_CAN_KOER_CRC,
    AL_CAN_KOER_OTHER,
    AL_CAN_KOER_NOT_USED
} AL_CAN_KoerEnum;

/**
 * @brief  Module running state enum
 */
typedef enum {
    AL_CAN_STATE_NOT_INIT           = (0x0),
    AL_CAN_STATE_READY              = BIT(1),
    AL_CAN_STATE_RESET              = BIT(2),
    AL_CAN_STATE_SEND_BUSY          = BIT(3),
    AL_CAN_STATE_RECV_BUSY          = BIT(4),     /* to be removed, after align with yonbo */
    AL_CAN_STATE_RECV_EMPTY         = BIT(5),
    AL_CAN_STATE_RECV_ALMOST_FULL   = BIT(6),
    AL_CAN_STATE_RECV_FULL          = BIT(7),
    AL_CAN_STATE_RECV_OVERFLOW      = BIT(8),
} AL_CAN_StateEnum;

/**
 * @brief  Io ctl cmd enum
 */
typedef enum
{
    AL_CAN_IOCTL_SET_BIT_RATE,
    AL_CAN_IOCTL_GET_BIT_RATE,
    AL_CAN_IOCTL_SET_DEF_BIT_RATE,
    AL_CAN_IOCTL_SET_RESET,
    AL_CAN_IOCTL_SET_FILTER,
    AL_CAN_IOCTL_GET_FILTER,
    AL_CAN_IOCTL_GET_KOER,
    AL_CAN_IOCTL_GET_STATE,
    AL_CAN_IOCTL_SET_STATE,
    AL_CAN_IOCTL_CLR_STATE
} AL_CAN_IoCtlCmdEnum;

/**
 * @brief  Frame control registers union
 */
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
} AL_CAN_FrameCtrlUnion;

/**
 * @brief  Frame bit rate struct
 */
typedef struct
{
    AL_CAN_BitRateTypeEnum      Type;
    AL_U8                       TimeSeg1;
    AL_U8                       TimeSeg2;
    AL_U8                       SyncJumpWidth;
    AL_U8                       Prescaler;
} AL_CAN_BitRateStruct;

/**
 * @brief  Frame bit rate struct
 */
typedef struct
{
    AL_CAN_BitRateTypeEnum      Type;
    AL_CAN_BitRateEnum          Index;
} AL_CAN_DefBitRateStruct;

/**
 * @brief  Event struct
 */
typedef struct
{
    AL_CAN_EventIdEnum  EventId;
    AL_U32              EventData;
} AL_CAN_EventStruct;

typedef AL_VOID (*AL_CAN_EventCallBack)(AL_CAN_EventStruct *Event, AL_VOID *CallBackRef);

/**
 * @brief  Filter config struct
 */
typedef struct
{
    AL_CAN_FilterIndexEnum      FilterIndex;
    AL_CAN_FilterMaskTypeEnum   MaskType;
    AL_U32                      MaskValue;      /* Mask bit */
    AL_U32                      IdValue;        /* Compare bit */
} AL_CAN_FilterCfgStruct;

/**
 * @brief  Frame struct
 */
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
    AL_U32              Rts[2];         /* Reception Time Stamps(CiA 603) */
} AL_CAN_FrameStruct;

/**
 * @brief  Module Init config struct
 */
typedef struct
{
    AL_CAN_TypeEnum         Type;
    AL_CAN_OpsModeEnum      OpsMode;
    AL_CAN_RunModeEnum      RunMode;
    AL_CAN_TransModeEnum    TransMode;
    AL_CAN_BitRateEnum      SlowBitRate;
    AL_CAN_BitRateEnum      FastBitRate;
    AL_CAN_RbAwflEnum       RbAfwl;         /* Receive buffer Almost Full Warning Limit */
} AL_CAN_InitStruct;

/**
 * @brief  Module device struct
 */
typedef struct
{
    AL_REG                  BaseAddr;
    AL_CAN_InitStruct       Config;
    AL_CAN_EventCallBack    EventCallBack;
    AL_VOID                 *EventCallBackRef;
    AL_CAN_StateEnum        State;
    AL_VOID                 *Private;
} AL_CAN_DevStruct;

/************************** Variable Definitions *****************************/

/************************** Function Prototypes ******************************/
AL_CAN_HwConfigStruct *AlCan_Dev_LookupConfig(AL_U32 DeviceId);

AL_S32 AlCan_Dev_Init(AL_CAN_DevStruct *Dev, AL_CAN_HwConfigStruct *HwConfig, AL_CAN_InitStruct *InitConfig);

AL_U32 AlCan_Dev_Dlc2Len(AL_CAN_DataLenEnum Dlc);

AL_U32 AlCan_Dev_Dlc2LenInByte(AL_CAN_DataLenEnum Dlc);

AL_VOID AlCan_Dev_IntrHandler(AL_VOID *Instance);

AL_S32 AlCan_Dev_RegisterEventCallBack(AL_CAN_DevStruct *Dev, AL_CAN_EventCallBack CallBack, AL_VOID *CallBackRef);

AL_S32 AlCan_Dev_UnRegisterEventCallBack(AL_CAN_DevStruct *Dev);

AL_S32 AlCan_Dev_SendFrame(AL_CAN_DevStruct *Dev, AL_CAN_FrameStruct *Frame);

AL_S32 AlCan_Dev_RecvFrame(AL_CAN_DevStruct *Dev, AL_CAN_FrameStruct *Frame);

AL_S32 AlCan_Dev_DecodeFrame(AL_U32 *BuffAddr, AL_CAN_FrameStruct *Frame);

AL_S32 AlCan_Dev_GetDecodeError(AL_CAN_DevStruct *Dev);

AL_S32 AlCan_Dev_IoCtl(AL_CAN_DevStruct *Dev, AL_CAN_IoCtlCmdEnum Cmd, AL_VOID *Data);

AL_S32 AlCan_Dev_DisplayFrame(AL_CAN_FrameStruct *Frame);

#ifdef __cplusplus
}
#endif

#endif
