/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***************************** Include Files *********************************/
#include "al_can_dev.h"

/************************** Constant Definitions *****************************/
/* Default init config */
static AL_CAN_InitStruct AlCan_DefInitConfig = {
    .OpsMode        = AL_CAN_MODE_NORMAL,
    .RunMode        = AL_CAN_RUN_INTR,
    .TransMode      = AL_CAN_TRANS_PTB,
    .RbAfwl         = AL_CAN_RB_LIMIT_8,
    .IsTimeStampEn  = AL_FALSE,
    .TimePos        = AL_CAN_TIMEPOS_EOF,
};

static AL_CAN_BitRateStruct AL_CAN_DefBitRateArray[] = {
    {.InputClk = 20000000,
     .BitRate = {
      {.Rate = 250000, .TimeSeg1 = 64, .TimeSeg2 = 16, .SyncJumpWidth = 16, .Prescaler = 1},
      {.Rate = 500000, .TimeSeg1 = 32, .TimeSeg2 = 8, .SyncJumpWidth = 8, .Prescaler = 1},
      {.Rate = 500000, .TimeSeg1 = 32, .TimeSeg2 = 8, .SyncJumpWidth = 8, .Prescaler = 1},
      {.Rate = 833000, .TimeSeg1 = 19, .TimeSeg2 = 5, .SyncJumpWidth = 5, .Prescaler = 1},
      {.Rate = 1000000, .TimeSeg1 = 16, .TimeSeg2 = 4, .SyncJumpWidth = 4, .Prescaler = 1},
      {.Rate = 1538000, .TimeSeg1 = 10, .TimeSeg2 = 3, .SyncJumpWidth = 3, .Prescaler = 1},
      {.Rate = 2000000, .TimeSeg1 = 8, .TimeSeg2 = 2, .SyncJumpWidth = 2, .Prescaler = 1},
      {.Rate = 3077000, .TimeSeg1 = 4, .TimeSeg2 = 1, .SyncJumpWidth = 1, .Prescaler = 1},
      {.Rate = 4000000, .TimeSeg1 = 3, .TimeSeg2 = 1, .SyncJumpWidth = 1, .Prescaler = 1},
      {.Rate = 5000000, .TimeSeg1 = 12, .TimeSeg2 = 4, .SyncJumpWidth = 4, .Prescaler = 1}}},
    {.InputClk = 40000000,
     .BitRate = {
      {.Rate = 250000, .TimeSeg1 = 64, .TimeSeg2 = 16, .SyncJumpWidth = 16, .Prescaler = 2},
      {.Rate = 500000, .TimeSeg1 = 64, .TimeSeg2 = 16, .SyncJumpWidth = 16, .Prescaler = 1},
      {.Rate = 500000, .TimeSeg1 = 32, .TimeSeg2 = 8, .SyncJumpWidth = 8, .Prescaler = 2},
      {.Rate = 833000, .TimeSeg1 = 19, .TimeSeg2 = 5, .SyncJumpWidth = 5, .Prescaler = 2},
      {.Rate = 1000000, .TimeSeg1 = 32, .TimeSeg2 = 8, .SyncJumpWidth = 8, .Prescaler = 1},
      {.Rate = 1538000, .TimeSeg1 = 20, .TimeSeg2 = 6, .SyncJumpWidth = 6, .Prescaler = 1},
      {.Rate = 2000000, .TimeSeg1 = 16, .TimeSeg2 = 4, .SyncJumpWidth = 4, .Prescaler = 1},
      {.Rate = 3077000, .TimeSeg1 = 10, .TimeSeg2 = 3, .SyncJumpWidth = 3, .Prescaler = 1},
      {.Rate = 4000000, .TimeSeg1 = 8, .TimeSeg2 = 2, .SyncJumpWidth = 2, .Prescaler = 1},
      {.Rate = 5000000, .TimeSeg1 = 6, .TimeSeg2 = 2, .SyncJumpWidth = 2, .Prescaler = 1},
      {.Rate = 6667000, .TimeSeg1 = 5, .TimeSeg2 = 1, .SyncJumpWidth = 1, .Prescaler = 1},
      {.Rate = 8000000, .TimeSeg1 = 4, .TimeSeg2 = 1, .SyncJumpWidth = 1, .Prescaler = 1},
      {.Rate = 10000000, .TimeSeg1 = 3, .TimeSeg2 = 1, .SyncJumpWidth = 1, .Prescaler = 1}}},
    {.InputClk = 80000000,
     .BitRate = {
      {.Rate = 250000, .TimeSeg1 = 64, .TimeSeg2 = 16, .SyncJumpWidth = 16, .Prescaler = 4},
      {.Rate = 500000, .TimeSeg1 = 64, .TimeSeg2 = 16, .SyncJumpWidth = 16, .Prescaler = 2},
      {.Rate = 500000, .TimeSeg1 = 32, .TimeSeg2 = 8, .SyncJumpWidth = 8, .Prescaler = 4},
      {.Rate = 833000, .TimeSeg1 = 19, .TimeSeg2 = 5, .SyncJumpWidth = 5, .Prescaler = 4},
      {.Rate = 1000000, .TimeSeg1 = 32, .TimeSeg2 = 8, .SyncJumpWidth = 8, .Prescaler = 2},
      {.Rate = 1538000, .TimeSeg1 = 20, .TimeSeg2 = 6, .SyncJumpWidth = 6, .Prescaler = 2},
      {.Rate = 2000000, .TimeSeg1 = 16, .TimeSeg2 = 4, .SyncJumpWidth = 4, .Prescaler = 2},
      {.Rate = 3077000, .TimeSeg1 = 10, .TimeSeg2 = 3, .SyncJumpWidth = 3, .Prescaler = 2},
      {.Rate = 4000000, .TimeSeg1 = 16, .TimeSeg2 = 4, .SyncJumpWidth = 4, .Prescaler = 1},
      {.Rate = 5000000, .TimeSeg1 = 12, .TimeSeg2 = 4, .SyncJumpWidth = 4, .Prescaler = 1},
      {.Rate = 6667000, .TimeSeg1 = 10, .TimeSeg2 = 2, .SyncJumpWidth = 2, .Prescaler = 1},
      {.Rate = 8000000, .TimeSeg1 = 8, .TimeSeg2 = 2, .SyncJumpWidth = 2, .Prescaler = 1},
      {.Rate = 10000000, .TimeSeg1 = 6, .TimeSeg2 = 2, .SyncJumpWidth = 2, .Prescaler = 1}}}};

static const AL_U8 * const ModeInfo[] = {
    [AL_CAN_MODE_NORMAL] = "Normal",
    [AL_CAN_MODE_IN_LOOPBACK] = "In loopback",
    [AL_CAN_MODE_EX_LOOPBACK] = "External loopback",
    [AL_CAN_MODE_STANDBY] = "Standby",
    [AL_CAN_MODE_LISTENONLY] = "Listen only",
};

static const AL_U8 * const TransModeInfo[] = {
    [AL_CAN_TRANS_FULL] = "Full",
    [AL_CAN_TRANS_PTB] = "PTB",
    [AL_CAN_TRANS_STB_FIFO] = "STB fifo",
    [AL_CAN_TRANS_STB_PRIO] = "STB priority",
};

/* Data length in word catch up with DLC */
static AL_U8 AL_CAN_DataLenArray[] = {0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 4, 5, 6, 8, 12, 16};
static AL_U8 AL_CAN_DataLenInByteArray[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20, 24, 32, 48, 64};

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/
/* Max bit timing params */
#define AL_CAN_MAX_S_SEG_1      63
#define AL_CAN_MAX_S_SEG_2      7
#define AL_CAN_MAX_S_SJW        15
#define AL_CAN_MAX_FS_SEG_1     63
#define AL_CAN_MAX_FS_SEG_2     31
#define AL_CAN_MAX_FS_SJW       15
#define AL_CAN_MAX_FF_SEG_1     15
#define AL_CAN_MAX_FF_SEG_2     7
#define AL_CAN_MAX_FF_SJW       7

#define AL_CAN_INTR_AIF     BIT(8)
#define AL_CAN_INTR_EIF     BIT(9)
#define AL_CAN_INTR_TSIF    BIT(10)
#define AL_CAN_INTR_TPIF    BIT(11)
#define AL_CAN_INTR_RAFIF   BIT(12)
#define AL_CAN_INTR_RFIF    BIT(13)
#define AL_CAN_INTR_ROIF    BIT(14)
#define AL_CAN_INTR_RIF     BIT(15)
#define AL_CAN_INTR_BEIF    BIT(16)
#define AL_CAN_INTR_ALIF    BIT(18)
#define AL_CAN_INTR_EPIF    BIT(20)

#define AL_CAN_RECV_INTR_MASK       (AL_CAN_INTR_RIF)
#define AL_CAN_RB_INTR_MASK         (AL_CAN_INTR_RAFIF | AL_CAN_INTR_RFIF | AL_CAN_INTR_ROIF)
#define AL_CAN_TRANS_INTR_MASK      (AL_CAN_INTR_TSIF | AL_CAN_INTR_TPIF)
#define AL_CAN_ERR_INTR_MASK        (AL_CAN_INTR_AIF | AL_CAN_INTR_EIF | \
                                    AL_CAN_INTR_BEIF | AL_CAN_INTR_ALIF | AL_CAN_INTR_EPIF)

#define AL_CAN_RECV_INTR(Status)    (Status & AL_CAN_RECV_INTR_MASK)
#define AL_CAN_RB_INTR(Status)      (Status & AL_CAN_RB_INTR_MASK)
#define AL_CAN_TRANS_INTR(Status)   (Status & AL_CAN_TRANS_INTR_MASK)
#define AL_CAN_ERR_INTR(Status)     (Status & AL_CAN_ERR_INTR_MASK)

#define AL_CAN_LOOP_MODE_DELAY      do {AlSys_UDelay(10);} while (0)
#define AL_CAN_RESET_RELEASE_DELAY  do {AlSys_UDelay(100);} while (0)

#define AL_CAN_CHK_TOUT_SET_MODE    (100)   /* 1ms */

#define IS_SAME_INITCONFIGS(Dst, Src)   ((Dst).OpsMode == (Src).OpsMode && \
                                         (Dst).RunMode == (Src).RunMode && \
                                         (Dst).TransMode == (Src).TransMode && \
                                         (Dst).RbAfwl == (Src).RbAfwl)
/************************** Variable Definitions *****************************/
/* Hardware config generated by TD */
extern AL_CAN_HwConfigStruct AlCan_HwConfig[];

/************************** Function Prototypes ******************************/

/************************** Function Definitions ******************************/
/**
 * This function converts a Data Length Code (DLC) to its corresponding data length in bytes.
 *
 * @param Dlc The Data Length Code to be converted.
 * @return The data length in bytes corresponding to the given DLC.
 */
AL_U32 AlCan_Dev_Dlc2Len(AL_CAN_DataLenEnum Dlc)
{
    return AL_CAN_DataLenArray[Dlc];
}

/**
 * This function converts a Data Length Code (DLC) to its corresponding data length in bytes.
 * This function explicitly casts the result to an AL_U32 type.
 *
 * @param Dlc The Data Length Code to be converted.
 * @return The data length in bytes corresponding to the given DLC, cast to AL_U32.
 */
AL_U32 AlCan_Dev_Dlc2LenInByte(AL_CAN_DataLenEnum Dlc)
{
    return (AL_U32) AL_CAN_DataLenInByteArray[Dlc];
}

/**
 * This function sets the CAN type for the device.
 *
 * @param Dev Pointer to the CAN device structure.
 * @return AL_OK on success, error code otherwise.
 */
static AL_S32 AlCan_Dev_SetCanType(AL_CAN_DevStruct *Dev)
{
    AL_ASSERT(Dev != AL_NULL, AL_CAN_ERR_NULL_PTR);

    switch (Dev->Config.Type) {
    case AL_CAN_TYPE_2_0B:
        AlCan_ll_SetFdEnabled(Dev->BaseAddr, AL_FALSE);
        break;
    case AL_CAN_TYPE_FD:
        AlCan_ll_SetFdEnabled(Dev->BaseAddr, AL_TRUE);
        break;
    default:
        break;
    }

    return AL_OK;
}

/**
 * This function waits for the device to enter the operational mode specified in its configuration.
 *
 * @param Dev Pointer to the CAN device structure.
 * @return AL_OK on success, AL_CAN_ERR_TIMEOUT if the operation times out,
 *         or AL_CAN_ERR_ILLEGAL_PARAM if an unsupported operational mode is specified.
 */
static AL_S32 AlCan_Dev_WaitForOpsMode(AL_CAN_DevStruct *Dev)
{
    AL_ASSERT(Dev != AL_NULL, AL_CAN_ERR_NULL_PTR);

    AL_S32 Ret = AL_OK;
    AL_U32 Timeout = AL_CAN_CHK_TOUT_SET_MODE;
    AL_BOOL State;

    if (Dev->Config.OpsMode == AL_CAN_MODE_NORMAL) {
        return Ret;
    }

    do {
        AL_CAN_LOOP_MODE_DELAY;
        if (Dev->Config.OpsMode == AL_CAN_MODE_IN_LOOPBACK) {
            State = AlCan_ll_IsLbmiEnabled(Dev->BaseAddr);
        } else if (Dev->Config.OpsMode == AL_CAN_MODE_EX_LOOPBACK) {
            State = AlCan_ll_IsLbmeEnabled(Dev->BaseAddr);
        } else if (Dev->Config.OpsMode == AL_CAN_MODE_LISTENONLY){
            State = AlCan_ll_IsLom(Dev->BaseAddr);
        } else if (Dev->Config.OpsMode == AL_CAN_MODE_STANDBY){
            State = AlCan_ll_IsStby(Dev->BaseAddr);
        } else {
            Ret = AL_CAN_ERR_ILLEGAL_PARAM;
            break;
        }
    } while ((!State) && (Timeout--));

    if (!Timeout) {
        Ret = AL_CAN_ERR_TIMEOUT;
    }

    return Ret;
}

/**
 * This function sets the operational mode for the device as specified in its configuration.
 *
 * @param Dev Pointer to the CAN device structure.
 * @return AL_OK on success, error code otherwise.
 */
static AL_S32 AlCan_Dev_SetOpsMode(AL_CAN_DevStruct *Dev)
{
    AL_ASSERT(Dev != AL_NULL, AL_CAN_ERR_NULL_PTR);

    AL_S32 Ret = AL_OK;

    switch (Dev->Config.OpsMode) {
    case AL_CAN_MODE_NORMAL:
        AlCan_ll_SetLbmi(Dev->BaseAddr, AL_FALSE);
        AlCan_ll_SetLbme(Dev->BaseAddr, AL_FALSE);
        break;
    case AL_CAN_MODE_IN_LOOPBACK:
        AlCan_ll_SetLbmi(Dev->BaseAddr, AL_TRUE);
        break;
    case AL_CAN_MODE_EX_LOOPBACK:
        AlCan_ll_SetLbme(Dev->BaseAddr, AL_TRUE);
        AlCan_ll_SetSack(Dev->BaseAddr, AL_CAN_SACK_SELF_ACK);
        break;
    case AL_CAN_MODE_LISTENONLY:
        AlCan_ll_SetLom(Dev->BaseAddr, AL_TRUE);
        AlCan_ll_SetLbme(Dev->BaseAddr, AL_TRUE);
        AlCan_ll_SetSack(Dev->BaseAddr, AL_CAN_SACK_SELF_ACK);
        break;
    case AL_CAN_MODE_STANDBY:
        AlCan_ll_SetStby(Dev->BaseAddr, AL_TRUE);
        break;
    default:
        break;
    }

    Ret = AlCan_Dev_WaitForOpsMode(Dev);

    return Ret;
}

/**
 * This function sets the bit rate for the CAN communication.
 *
 * @param Dev Pointer to the CAN device structure.
 * @param BitRate Pointer to the structure containing bit rate settings.
 * @return AL_OK on success, error code otherwise.
 */
AL_S32 AlCan_Dev_SetBitRate(AL_CAN_DevStruct *Dev, AL_CAN_BitRateCfgStruct *BitRate)
{
    AL_ASSERT((Dev != AL_NULL) && (BitRate != AL_NULL), AL_CAN_ERR_NULL_PTR);

    switch (BitRate->Type)
    {
    case AL_CAN_BIT_S:
        AlCan_ll_SetSseg1(Dev->BaseAddr, BitRate->TimeSeg1);
        AlCan_ll_SetSseg2(Dev->BaseAddr, BitRate->TimeSeg2);
        AlCan_ll_SetSsjw(Dev->BaseAddr, BitRate->SyncJumpWidth);
        AlCan_ll_SetSpresc(Dev->BaseAddr, BitRate->Prescaler);
        Dev->Config.SlowBitRate = *BitRate;
        break;
    case AL_CAN_BIT_F:
        AlCan_ll_SetFseg1(Dev->BaseAddr, BitRate->TimeSeg1);
        AlCan_ll_SetFseg2(Dev->BaseAddr, BitRate->TimeSeg2);
        AlCan_ll_SetFsjw(Dev->BaseAddr, BitRate->SyncJumpWidth);
        AlCan_ll_SetFpresc(Dev->BaseAddr, BitRate->Prescaler);
        Dev->Config.FastBitRate = *BitRate;
        break;
    default:
        break;
    }

    return AL_OK;
}

/**
 * This function retrieves the current bit rate settings of the CAN device.
 *
 * @param Dev Pointer to the CAN device structure.
 * @param BitRate Pointer to the structure where the current bit rate settings will be stored.
 * @return AL_OK on success, error code otherwise.
 */
AL_S32 AlCan_Dev_GetBitRate(AL_CAN_DevStruct *Dev, AL_CAN_BitRateCfgStruct *BitRate)
{
    AL_ASSERT((Dev != AL_NULL) && (BitRate != AL_NULL), AL_CAN_ERR_NULL_PTR);

    switch (BitRate->Type)
    {
    case AL_CAN_BIT_S:
        BitRate->Type           = AL_CAN_BIT_S;
        BitRate->TimeSeg1       = (AL_U8)AlCan_ll_GetSseg1(Dev->BaseAddr);
        BitRate->TimeSeg2       = (AL_U8)AlCan_ll_GetSseg2(Dev->BaseAddr);
        BitRate->SyncJumpWidth  = (AL_U8)AlCan_ll_GetSsjw(Dev->BaseAddr);
        BitRate->Prescaler      = (AL_U8)AlCan_ll_GetSpresc(Dev->BaseAddr);
        break;
    case AL_CAN_BIT_F:
        BitRate->Type           = AL_CAN_BIT_F;
        BitRate->TimeSeg1       = (AL_U8)AlCan_ll_GetFseg1(Dev->BaseAddr);
        BitRate->TimeSeg2       = (AL_U8)AlCan_ll_GetFseg2(Dev->BaseAddr);
        BitRate->SyncJumpWidth  = (AL_U8)AlCan_ll_GetFsjw(Dev->BaseAddr);
        BitRate->Prescaler      = (AL_U8)AlCan_ll_GetFpresc(Dev->BaseAddr);
        break;
    default:
        break;
    }

    return AL_OK;
}

/**
 * This function sets the default bit rate for the CAN device using predefined settings.
 *
 * @param Dev Pointer to the CAN device structure.
 * @param Type The type of bit rate to set.
 * @param BitRate Pointer to the structure containing the index to the predefined bit rate settings.
 * @return AL_OK on success, error code otherwise.
 */
AL_S32 AlCan_Dev_SetDefBitRate(AL_CAN_DevStruct *Dev, AL_CAN_BitRateTypeEnum Type,
                               AL_CAN_BitRateCfgStruct *BitRate)
{
    AL_ASSERT((Dev != AL_NULL) && (BitRate != AL_NULL), AL_CAN_ERR_NULL_PTR);

    BitRate->Type = Type;
    AlCan_Dev_SetBitRate(Dev, BitRate);

    if (BitRate->Type == AL_CAN_BIT_F) {
        AlCan_ll_SetSspoff(Dev->BaseAddr, BitRate->TimeSeg1);
        AlCan_ll_SetTdcen(Dev->BaseAddr, AL_TRUE);
    }

    return AL_OK;
}

/**
 * This function sets the transmission mode for the CAN device.
 *
 * @param Dev Pointer to the CAN device structure.
 * @return AL_OK on success, error code otherwise.
 */
static AL_S32 AlCan_Dev_SetTransMode(AL_CAN_DevStruct *Dev)
{
    AL_ASSERT(Dev != AL_NULL, AL_CAN_ERR_NULL_PTR);

    switch (Dev->Config.TransMode)
    {
    case AL_CAN_TRANS_PTB:
        AlCan_ll_SetTbsel(Dev->BaseAddr, AL_CAN_TBSEL_PTB);
        break;
    case AL_CAN_TRANS_STB_FIFO:
        AlCan_ll_SetTbsel(Dev->BaseAddr, AL_CAN_TBSEL_STB);
        AlCan_ll_SetTsmode(Dev->BaseAddr, AL_CAN_TSMODE_FIFO);
        break;
    case AL_CAN_TRANS_STB_PRIO:
        AlCan_ll_SetTbsel(Dev->BaseAddr, AL_CAN_TBSEL_STB);
        AlCan_ll_SetTsmode(Dev->BaseAddr, AL_CAN_TSMODE_PRIORITY);
        break;
    case AL_CAN_TRANS_FULL:
        break;
    default:
        break;
    }

    return AL_OK;
}

/**
 * This function retrieves the current state of the CAN device.
 *
 * @param Dev Pointer to the CAN device structure.
 * @param State The state to check.
 * @return AL_TRUE if the device is in the specified state, AL_FALSE otherwise.
 */
AL_BOOL AlCan_Dev_GetState(AL_CAN_DevStruct *Dev, AL_CAN_StateEnum State)
{
    return ((Dev->State & State) ? AL_TRUE : AL_FALSE);
}

/**
 * This function sets a state for the CAN device.
 *
 * @param Dev Pointer to the CAN device structure.
 * @param State The state to set.
 * @return None.
 */
AL_VOID AlCan_Dev_SetState(AL_CAN_DevStruct *Dev, AL_CAN_StateEnum State)
{
    Dev->State |= State;
}

/**
 * This function clears a state for the CAN device.
 *
 * @param Dev Pointer to the CAN device structure.
 * @param State The state to clear.
 * @return None.
 */
AL_VOID AlCan_Dev_ClrState(AL_CAN_DevStruct *Dev, AL_CAN_StateEnum State)
{
    Dev->State &= ~State;
}

/**
 * This function clears the receive empty state and notifies via callback.
 *
 * @param Dev Pointer to the CAN device structure.
 * @param IntrStatus The interrupt status that triggered the handler.
 * @return None.
 */
static AL_VOID AlCan_Dev_RecvFrameHandler(AL_CAN_DevStruct *Dev, AL_U32 IntrStatus)
{
    // AL_LOG(AL_LOG_LEVEL_DEBUG, "AlCan_Dev_RecvFrameHandler: Recv a frame!\r\n");
    /* IP has loop buffer, so nothing to do here */
    AlCan_Dev_ClrState(Dev, AL_CAN_STATE_RECV_EMPTY);
    AL_CAN_EventStruct Event = {
        .EventId    = AL_CAN_EVENT_RECV_DONE,
        .EventData  = 0
    };
    Dev->EventCallBack(&Event, Dev->EventCallBackRef);
}

/**
 * This function clears the send busy state and notifies via callback.
 *
 * @param Dev Pointer to the CAN device structure.
 * @param IntrStatus The interrupt status that triggered the handler.
 * @return None.
 */
static AL_VOID AlCan_Dev_SendFrameHandler(AL_CAN_DevStruct *Dev, AL_U32 IntrStatus)
{
    AL_CAN_EventStruct Event = {
        .EventId    = AL_CAN_EVENT_SEND_DONE,
        .EventData  = 0
    };
    AlCan_Dev_ClrState(Dev, AL_CAN_STATE_SEND_BUSY);
    Dev->EventCallBack(&Event, Dev->EventCallBackRef);
}

/**
 * This function determines the type of error based on the interrupt status and notifies via callback.
 *
 * @param Dev Pointer to the CAN device structure.
 * @param IntrStatus The interrupt status that triggered the handler.
 * @return None.
 */
static AL_VOID AlCan_Dev_ErrHandler(AL_CAN_DevStruct *Dev, AL_U32 IntrStatus)
{
    AL_CAN_EventStruct Event;
    AL_LOG(AL_LOG_LEVEL_DEBUG, "Can transfer error state: 0x%x\r\n", IntrStatus);

    if (IntrStatus & AL_CAN_INTR_AIF) {
        Event.EventId   = AL_CAN_EVENT_ABORT;
        Event.EventData = 0;
    }

    if (IntrStatus & AL_CAN_INTR_EIF) {
        Event.EventId   = AL_CAN_EVENT_ERR;
        Event.EventData = 0;
    }

    if (IntrStatus & AL_CAN_INTR_BEIF) {
        if (Dev->Config.OpsMode == AL_CAN_MODE_LISTENONLY) {
            AL_LOG(AL_LOG_LEVEL_DEBUG, "can bus error with listen only mode: 0x%x!\r\n",
            AlCan_ll_GetKoer(Dev->BaseAddr));
            AlCan_Dev_GetDecodeError(Dev);
        }

        Event.EventId   = AL_CAN_EVENT_BUS_ERR;
        Event.EventData = 0;
    }

    if (IntrStatus & AL_CAN_INTR_ALIF) {
        Event.EventId   = AL_CAN_EVENT_ARBITRATION_LOST;
        Event.EventData = 0;
    }

    if (IntrStatus & AL_CAN_INTR_EPIF) {
        Event.EventId   = AL_CAN_EVENT_ERR_PASSIVE;
        Event.EventData = 0;
    }

    AlCan_Dev_ClrState(Dev, AL_CAN_STATE_READY);
    Dev->EventCallBack(&Event, Dev->EventCallBackRef);
}

/**
 * This function handles CAN device events based on interrupt status.
 *
 * @param Dev Pointer to the CAN device structure.
 * @param IntrStatus Interrupt status indicating the type of event.
 * @return None.
 */
static AL_VOID AlCan_Dev_EventHandler(AL_CAN_DevStruct *Dev, AL_U32 IntrStatus)
{
    AL_CAN_EventStruct Event;

    if (IntrStatus & AL_CAN_INTR_RAFIF) {
        AlCan_Dev_SetState(Dev, AL_CAN_STATE_RECV_ALMOST_FULL);
        Event.EventId   = AL_CAN_EVENT_RBUFF_ALMOST_FULL;
        Event.EventData = 0;
    }

    if (IntrStatus & AL_CAN_INTR_RFIF) {
        AlCan_Dev_SetState(Dev, AL_CAN_STATE_RECV_FULL);
        Event.EventId   = AL_CAN_EVENT_RBUFF_FULL;
        Event.EventData = 0;
    }

    if (IntrStatus & AL_CAN_INTR_ROIF) {
        AlCan_Dev_SetState(Dev, AL_CAN_STATE_RECV_OVERFLOW);
        Event.EventId   = AL_CAN_EVENT_RBUFF_OVERFLOW;
        Event.EventData = 0;
    }

    Dev->EventCallBack(&Event, Dev->EventCallBackRef);
}

/**
 * This function interrupt handler for CAN device. It checks the interrupt status and calls the appropriate handler.
 *
 * @param Instance Pointer to the CAN device instance.
 * @return None.
 */
AL_VOID AlCan_Dev_IntrHandler(void *Instance)
{
    AL_CAN_DevStruct *Dev = (AL_CAN_DevStruct *)Instance;
    AL_U32 IntrStatus = AlCan_ll_GetUmaskIntrStatus(Dev->BaseAddr);

    if (AL_CAN_ERR_INTR(IntrStatus)) {
        AlCan_Dev_ErrHandler(Dev, IntrStatus);
        AlCan_ll_ClrIntrStatus(Dev->BaseAddr, AL_CAN_ERR_INTR_MASK);
    }

    if (AL_CAN_RECV_INTR(IntrStatus)) {
        AlCan_Dev_RecvFrameHandler(Dev, IntrStatus);
        AlCan_ll_ClrIntrStatus(Dev->BaseAddr, AL_CAN_RECV_INTR_MASK);
    }

    if (AL_CAN_RB_INTR(IntrStatus)) {
       AlCan_Dev_EventHandler(Dev, IntrStatus);
        AlCan_ll_ClrIntrStatus(Dev->BaseAddr, AL_CAN_RB_INTR_MASK);
    }

    if (AL_CAN_TRANS_INTR(IntrStatus)) {
        AlCan_Dev_SendFrameHandler(Dev, IntrStatus);
        AlCan_ll_ClrIntrStatus(Dev->BaseAddr, AL_CAN_TRANS_INTR_MASK);
    }
}

/**
 * This function registers a callback function for CAN device events.
 *
 * @param Dev Pointer to the CAN device structure.
 * @param CallBack Function pointer to the callback function.
 * @param CallBackRef User-defined reference passed to the callback function.
 * @return AL_OK on success, error code otherwise.
 */
AL_S32 AlCan_Dev_RegisterEventCallBack(AL_CAN_DevStruct *Dev, AL_CAN_EventCallBack CallBack, AL_VOID *CallBackRef)
{
    AL_ASSERT(Dev != AL_NULL, AL_CAN_ERR_NULL_PTR);

    if (Dev->EventCallBack != AL_NULL) {
#ifdef CAN_DEBUG
        AL_LOG(AL_LOG_LEVEL_WARNING, "can=%p duplicate register callback: replace old:%p with New: %p\r\n",
               Dev, Dev->EventCallBack, CallBack);
#endif
    }

    Dev->EventCallBack      = CallBack;
    Dev->EventCallBackRef   = CallBackRef;

    return AL_OK;
}

/**
 * This function unregisters the event callback function for a CAN device.
 *
 * @param Dev Pointer to the CAN device structure.
 * @return AL_OK on success, error code otherwise.
 */
AL_S32 AlCan_Dev_UnRegisterEventCallBack(AL_CAN_DevStruct *Dev)
{
    AL_ASSERT(Dev != AL_NULL, AL_CAN_ERR_NULL_PTR);

    Dev->EventCallBack      = (AL_CAN_EventCallBack)AL_NULL;
    Dev->EventCallBackRef   = AL_NULL;

    return AL_OK;
}

/**
 * This function initializes the CAN device structure with default values.
 *
 * @param Dev Pointer to the CAN device structure.
 * @return AL_OK on success, error code otherwise.
 */
AL_VOID AlCan_Dev_DecodeConfig(AL_CAN_DevStruct *Dev, AL_CAN_HwConfigStruct *HwConfig)
{
    AL_ASSERT(((Dev != AL_NULL) && (HwConfig != AL_NULL)), AL_CAN_ERR_NULL_PTR);

    Dev->BaseAddr           = HwConfig->BaseAddress;
    Dev->Config.InputClk    = HwConfig->InputClk;
    Dev->Config.Type        = HwConfig->Type;

    for (AL_U32 i = 0; i < sizeof(AL_CAN_DefBitRateArray) / sizeof(AL_CAN_BitRateStruct); i++) {
        if (Dev->Config.InputClk == AL_CAN_DefBitRateArray[i].InputClk) {
            Dev->Config.InputClkIndex = i;
            break;
        }
    }

    for (AL_U32 i = 0; i < AL_CAN_BIT_RATE_MAX; i++) {
        if (HwConfig->SlowBitRate == AL_CAN_DefBitRateArray[Dev->Config.InputClkIndex].BitRate[i].Rate) {
            Dev->Config.SlowBitRate = AL_CAN_DefBitRateArray[Dev->Config.InputClkIndex].BitRate[i];
            break;
        }
    }

    if (Dev->Config.Type == AL_CAN_TYPE_FD) {
        for (AL_U32 i = 0; i < AL_CAN_BIT_RATE_MAX; i++) {
            if (HwConfig->FastBitRate == AL_CAN_DefBitRateArray[Dev->Config.InputClkIndex].BitRate[i].Rate) {
                Dev->Config.FastBitRate = AL_CAN_DefBitRateArray[Dev->Config.InputClkIndex].BitRate[i];
                break;
            }
        }
    }

    return AL_OK;
}

/**
 * This function looks up the hardware configuration for a CAN device using its device ID.
 *
 * @param DeviceId The device ID of the CAN device.
 * @return Pointer to the hardware configuration structure if found, NULL otherwise.
 */
AL_CAN_HwConfigStruct *AlCan_Dev_LookupConfig(AL_U32 DeviceId)
{
    AL_CAN_HwConfigStruct *CfgPtr = AL_NULL;

    for (AL_U32 Index = 0; Index < AL_CAN_NUM_INSTANCE; Index++) {
        if (AlCan_HwConfig[Index].DeviceId == DeviceId) {
            CfgPtr = &AlCan_HwConfig[Index];
            break;
        }
    }

    return CfgPtr;
}

/**
 * This function sets or clears the reset state of a CAN device.
 *
 * @param Dev Pointer to the CAN device structure.
 * @param IsReset Boolean value to set (TRUE) or clear (FALSE) the reset state.
 * @return AL_OK on success, error code otherwise.
 */
AL_S32 AlCan_Dev_SetReset(AL_CAN_DevStruct *Dev, AL_BOOL IsReset)
{
    AL_ASSERT(Dev != AL_NULL, AL_CAN_ERR_NULL_PTR);

    AlCan_ll_SetCanCtrlReset(Dev->BaseAddr, IsReset);
    AL_CAN_RESET_RELEASE_DELAY;

    if (IsReset == AL_TRUE) {
        AlCan_Dev_SetState(Dev, AL_CAN_STATE_RESET);
    } else {
        AlCan_Dev_ClrState(Dev, AL_CAN_STATE_RESET);
    }

    return AL_OK;
}

/**
 * This function initializes a CAN device with specified hardware and initialization configurations.
 *
 * @param Dev Pointer to the CAN device structure.
 * @param HwConfig Pointer to the hardware configuration structure.
 * @param InitConfig Pointer to the initialization configuration structure. If NULL, default configuration is used.
 * @return AL_OK on success, error code otherwise.
 */
AL_S32 AlCan_Dev_Init(AL_CAN_DevStruct *Dev, AL_CAN_HwConfigStruct *HwConfig, AL_CAN_InitStruct *InitConfig)
{
    AL_S32 Ret = AL_OK;
    AL_CAN_DefBitRateStruct DefBitRate;

    AL_ASSERT((Dev != AL_NULL) && (HwConfig != AL_NULL), AL_CAN_ERR_NULL_PTR);

    if (AlCan_Dev_GetState(Dev, AL_CAN_STATE_READY)) {
        if (InitConfig == AL_NULL || IS_SAME_INITCONFIGS(Dev->Config, *InitConfig)) {
            return AL_OK;
        }
    }

    Dev->Config = (InitConfig == AL_NULL) ? AlCan_DefInitConfig : (*InitConfig);

    AlCan_Dev_DecodeConfig(Dev, HwConfig);

    AlCan_Dev_SetCanType(Dev);
    AlCan_Dev_SetReset(Dev, AL_TRUE);

    if (Dev->Config.TransMode == AL_CAN_TRANS_PTB) {
        AlCan_ll_SetTpss(Dev->BaseAddr, AL_TRUE);
    } else {
        AlCan_ll_SetTsss(Dev->BaseAddr, AL_TRUE);
    }

    if (Dev->Config.RunMode == AL_CAN_RUN_INTR) {
        AlCan_ll_SetAllIntrEnabled(Dev->BaseAddr, AL_TRUE);
    } else if (Dev->Config.RunMode == AL_CAN_RUN_INTR_DMA) {
        AlCan_ll_SetAllIntrEnabled(Dev->BaseAddr, AL_TRUE);
        /* Use dma soft request mode */
        // AlCan_ll_SetDmaEnabled(Dev->BaseAddr, AL_TRUE);
    } else {
        AlCan_ll_SetAllIntrEnabled(Dev->BaseAddr, AL_FALSE);
    }

    AlCan_Dev_SetDefBitRate(Dev, AL_CAN_BIT_S, &(Dev->Config.SlowBitRate));

    if (Dev->Config.Type == AL_CAN_TYPE_FD) {
        AlCan_Dev_SetDefBitRate(Dev, AL_CAN_BIT_F, &(Dev->Config.FastBitRate));
    }

    AlCan_ll_SetAfwl(Dev->BaseAddr, Dev->Config.RbAfwl);
    AlCan_ll_SetAcfen(Dev->BaseAddr, AL_FALSE);

    if (InitConfig->IsTimeStampEn) {
        /* CAN timer clk share with coresight timer clk */
        AL_REG32_WRITE(0xF8E05000, 0x1);
        AlCan_ll_SetTimeen(Dev->BaseAddr, AL_FALSE);
        AlCan_ll_SetTimepos(Dev->BaseAddr, InitConfig->TimePos);
        AlCan_ll_SetTimeen(Dev->BaseAddr, InitConfig->IsTimeStampEn);
    }

    for (AL_U32 i = 0; i < CAN_ACF_EN_1_ACF_EN_0_TIMECFG_ACFCTRL_AE_X_SIZE; i++) {
        if (InitConfig->Acf[i].FilterEn) {
            AlCan_Dev_SetFilter(Dev, &InitConfig->Acf[i]);
        }
    }

    AlCan_Dev_SetReset(Dev, AL_FALSE);
    AlCan_Dev_SetTransMode(Dev);
    Ret = AlCan_Dev_SetOpsMode(Dev);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can set ops mode error: 0x%x", Ret);
        return Ret;
    }

    AlCan_Dev_SetState(Dev, AL_CAN_STATE_READY | AL_CAN_STATE_RECV_EMPTY);

    AlCan_Dev_DisplayConfig(Dev);

    return Ret;
}

/**
 * This function sends a CAN frame using the specified CAN device.
 *
 * @param Dev Pointer to the CAN device structure.
 * @param Frame Pointer to the CAN frame structure to be sent.
 * @return AL_OK on success, error code otherwise.
 */
AL_S32 AlCan_Dev_SendFrame(AL_CAN_DevStruct *Dev, AL_CAN_FrameStruct *Frame)
{
    AL_U32 Id, Ctrl;
    AL_S32 Ret = AL_OK;

    AL_ASSERT((Dev != AL_NULL) && (Frame != AL_NULL), AL_CAN_ERR_NULL_PTR);
    AL_ASSERT(Frame->DataLen < AL_CAN_LEN_MAX, AL_CAN_ERR_ILLEGAL_PARAM);
    AL_ASSERT(!AlCan_Dev_GetState(Dev, AL_CAN_STATE_RESET), AL_CAN_ERR_STATE_RESET);
    AL_ASSERT(AlCan_Dev_GetState(Dev, AL_CAN_STATE_READY), AL_CAN_ERR_STATE_NOT_READY);
    AL_ASSERT(!AlCan_Dev_GetState(Dev, AL_CAN_STATE_SEND_BUSY), AL_CAN_ERR_BUSY);

    AlCan_Dev_SetState(Dev, AL_CAN_STATE_SEND_BUSY);

    Id = Frame->Id;
    Id |= (Frame->IsEnTts << CAN_TBUF_0_3_TTSEN_SHIFT);

    AlCan_ll_WriteWordSendBuffer(Dev->BaseAddr, 0, Id);

    Ctrl = Frame->DataLen & 0xF;

    if (Dev->Config.Type == AL_CAN_TYPE_FD) {
        Ctrl |= (((Frame->IsBitSwitch & 0x1) << CAN_TBUF_4_7_BRS_SHIFT) | (0x1 << CAN_TBUF_4_7_FDF_SHIFT));
    } else {
        Ctrl |= ((Frame->IsRemote & 0x1) << CAN_TBUF_4_7_RTR_SHIFT);
    }

    Ctrl |= ((Frame->IsIdExt & 0x1) << CAN_TBUF_4_7_IDE_SHIFT);

    AlCan_ll_WriteWordSendBuffer(Dev->BaseAddr, 1, Ctrl);

    for (AL_U32 i = 0; i < AL_CAN_DataLenArray[Frame->DataLen]; i++) {
        AlCan_ll_WriteWordSendBuffer(Dev->BaseAddr, (2 + i), Frame->Data[i]);
    }

    if (AlCan_ll_IsStby(Dev->BaseAddr)) {
        AlCan_Dev_ClrState(Dev, AL_CAN_STATE_SEND_BUSY);
        AL_LOG(AL_LOG_LEVEL_WARNING, "Can in standby mode\r\n");
        Ret = AL_CAN_ERR_IN_STANDBY_MODE;
    }

    switch (Dev->Config.TransMode)
    {
    case AL_CAN_TRANS_PTB:
        AlCan_ll_SetTpe(Dev->BaseAddr);
        break;
    case AL_CAN_TRANS_STB_FIFO:
    case AL_CAN_TRANS_STB_PRIO:
        AlCan_ll_SetTsnext(Dev->BaseAddr, AL_CAN_TSNEXT_STB_SLOT_FILLED);
        AlCan_ll_SetTsone(Dev->BaseAddr);
        break;
    default:
        break;
    }

    return Ret;
}

/**
 * This function decodes a CAN frame from the specified buffer address.
 *
 * @param BuffAddr Pointer to the buffer address where the frame is stored.
 * @param Frame Pointer to the CAN frame structure to store the decoded frame.
 * @return AL_OK on success, error code otherwise.
 */
AL_S32 AlCan_Dev_DecodeFrame(AL_U32 *BuffAddr, AL_CAN_FrameStruct *Frame)
{
    AL_ASSERT((BuffAddr != AL_NULL) && (Frame != AL_NULL), AL_CAN_ERR_NULL_PTR);

    AL_CAN_FrameCtrlUnion Ctrl;
    AL_U32 IdSize;
    AL_U32 DataWordLen;

    Ctrl.Reg = BuffAddr[1];
    Frame->Koer         = Ctrl.Bit.Koer;
    Frame->IsIdExt      = Ctrl.Bit.Ide;
    Frame->DataLen      = Ctrl.Bit.Dlc;
    Frame->IsBitSwitch  = Ctrl.Bit.Brs;
    Frame->IsRemote     = Ctrl.Bit.Rtr;

    IdSize = Frame->IsIdExt ? CAN_RBUF_0_3_ID_EXT_SIZE : CAN_RBUF_0_3_ID_STD_SIZE;
    Frame->Id = BuffAddr[0] & ((0x1 << IdSize) - 1);

    DataWordLen = AlCan_Dev_Dlc2Len(Frame->DataLen);
    for (AL_U32 i = 0; i < DataWordLen; i++) {
        Frame->Data[i] = BuffAddr[2 + i];
    }

    return AL_OK;
}

/**
 * This function checks the device and frame pointers for nullity, the device's state for readiness,
 * and the receive buffer for non-emptiness. It then reads the control word and the data from the receive buffer,
 * populates the frame structure accordingly, and releases the receive buffer. If the frame contains an error
 * or if the receive buffer is found empty after attempting to release it, appropriate error codes are returned.
 *
 * @param Dev Pointer to the CAN device structure.
 * @param Frame Pointer to the structure where the received frame will be stored.
 * @return AL_OK on success, error code otherwise.
 *
 * */
AL_S32 AlCan_Dev_RecvFrame(AL_CAN_DevStruct *Dev, AL_CAN_FrameStruct *Frame)
{
    AL_CAN_FrameCtrlUnion Ctrl;
    AL_U32 DataWordLen;

    AL_ASSERT((Dev != AL_NULL) && (Frame != AL_NULL), AL_CAN_ERR_NULL_PTR);
    AL_ASSERT(!AlCan_Dev_GetState(Dev, AL_CAN_STATE_RESET), AL_CAN_ERR_STATE_RESET);
    AL_ASSERT(AlCan_Dev_GetState(Dev, AL_CAN_STATE_READY), AL_CAN_ERR_STATE_NOT_READY);
    AL_ASSERT(!AlCan_Dev_GetState(Dev, AL_CAN_STATE_RECV_EMPTY), AL_CAN_ERR_RECV_EMPTY);

    if (AlCan_ll_GetRstat(Dev->BaseAddr) == AL_CAN_RSTAT_EMPTY) {
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can recv state not empty but register state empty\r\n");
    }

    Ctrl.Reg = AlCan_ll_ReadWordRecvBuffer(Dev->BaseAddr, 1);

    Frame->Koer         = Ctrl.Bit.Koer;
    if (Frame->Koer != AL_CAN_KOER_NONE) {
        return AL_CAN_ERR_RECV;
    }
    Frame->IsIdExt      = Ctrl.Bit.Ide;
    Frame->DataLen      = Ctrl.Bit.Dlc;
    Frame->IsBitSwitch  = Ctrl.Bit.Brs;
    Frame->IsRemote     = Ctrl.Bit.Rtr;

    Frame->Id = AlCan_ll_GetRecvExtId(Dev->BaseAddr, Frame->IsIdExt);
    DataWordLen = AlCan_Dev_Dlc2Len(Frame->DataLen);
    for (AL_U32 i = 0; i < DataWordLen; i++) {
        Frame->Data[i] = AlCan_ll_ReadWordRecvBuffer(Dev->BaseAddr, (2 + i));
    }

    AlCan_ll_SetRrel(Dev->BaseAddr, AL_CAN_RREL_RELEASE);

    if (AlCan_ll_GetRstat(Dev->BaseAddr) == AL_CAN_RSTAT_EMPTY) {
        AlCan_Dev_SetState(Dev, AL_CAN_STATE_RECV_EMPTY);
    }

    return AL_OK;
}

/**
 * This function validates the input pointers, checks if the filter is already enabled, and if not, enables it.
 * It then sets the filter address and mask according to the provided configuration. If the filter is already enabled,
 * a TODO comment suggests that additional actions might be required.
 *
 * @param Dev Pointer to the CAN device structure.
 * @param FilterCfg Pointer to the filter configuration structure.
 * @return AL_OK on success, error code otherwise.
 *
 * */
AL_S32 AlCan_Dev_SetFilter(AL_CAN_DevStruct *Dev, AL_CAN_FilterCfgStruct *FilterCfg)
{
    AL_ASSERT((Dev != AL_NULL) && (FilterCfg != AL_NULL), AL_CAN_ERR_NULL_PTR);

    if (!AlCan_ll_IsAcfenx(Dev->BaseAddr, FilterCfg->FilterIndex)) {
        AlCan_ll_SetAcfenx(Dev->BaseAddr, FilterCfg->FilterIndex, AL_TRUE);
    } else {
        /* TODO: Filter has enabled, should do somthing */
    }

    AlCan_ll_SetAcfadr(Dev->BaseAddr, FilterCfg->FilterIndex);
    AlCan_ll_SetSelmask(Dev->BaseAddr, AL_CAN_SELMASK_ACCEPT_MASK);
    AlCan_ll_WriteAcodexAmaskx(Dev->BaseAddr, (FilterCfg->MaskValue |
                               (FilterCfg->MaskType << CAN_ACF_3_ACF_2_ACF_1_ACF_0_AIDE_SHIFT)));
    AlCan_ll_SetSelmask(Dev->BaseAddr, AL_CAN_SELMASK_ACCEPT_CODE);
    AlCan_ll_WriteAcodexAmaskx(Dev->BaseAddr, FilterCfg->IdValue);

    return AL_OK;
}

/**
 * This function checks the input pointers for nullity and whether the filter is enabled. It then retrieves the filter configuration
 * from the device and stores it in the provided structure. If the filter is not enabled, an error is returned.
 *
 * @param Dev Pointer to the CAN device structure.
 * @param FilterCfg Pointer to the structure where the filter configuration will be stored.
 * @return AL_OK on success, AL_CAN_ERR_NULL_PTR if the filter is not enabled.
 *
 * */
AL_S32 AlCan_Dev_GetFilter(AL_CAN_DevStruct *Dev, AL_CAN_FilterCfgStruct *FilterCfg)
{
    AL_ASSERT((Dev != AL_NULL) && (FilterCfg != AL_NULL), AL_CAN_ERR_NULL_PTR);

    if (!AlCan_ll_IsAcfenx(Dev->BaseAddr, FilterCfg->FilterIndex)) {
        return AL_CAN_ERR_NULL_PTR;
    }

    AlCan_ll_SetAcfadr(Dev->BaseAddr, FilterCfg->FilterIndex);
    AlCan_ll_SetSelmask(Dev->BaseAddr, AL_CAN_SELMASK_ACCEPT_MASK);
    AL_U32 Value = AlCan_ll_ReadAcodexAmaskx(Dev->BaseAddr);
    FilterCfg->MaskValue = Value & CAN_ACF_3_ACF_2_ACF_1_ACF_0_ACODE_AMASK_VALUE_MASK;
    FilterCfg->MaskType = (Value & CAN_ACF_3_ACF_2_ACF_1_ACF_0_ACODE_AMASK_TYPE_MASK) >>
                                    CAN_ACF_3_ACF_2_ACF_1_ACF_0_AIDE_SHIFT;
    AlCan_ll_SetSelmask(Dev->BaseAddr, AL_CAN_SELMASK_ACCEPT_CODE);
    FilterCfg->IdValue = AlCan_ll_ReadAcodexAmaskx(Dev->BaseAddr);

    return AL_OK;
}

/**
 * This function checks the device pointer for nullity, retrieves the last decode error,
 * and logs an appropriate message. Depending on the error type, it may return a specific error code.
 * If the error type is unrecognized, it returns an error indicating a wrong parameter.
 *
 * @param Dev Pointer to the CAN device structure.
 * @return AL_OK if no error, specific error code corresponding to the decode error.
 *
 */
AL_S32 AlCan_Dev_GetDecodeError(AL_CAN_DevStruct *Dev)
{
    AL_ASSERT(Dev != AL_NULL, AL_CAN_ERR_NULL_PTR);

    AL_CAN_KoerEnum State = AlCan_ll_GetKoer(Dev->BaseAddr);

    switch (State)
    {
    case AL_CAN_KOER_NONE:
        AL_LOG(AL_LOG_LEVEL_INFO, "CAN success reception!\r\n");
        break;
    case AL_CAN_KOER_BIT:
        AL_LOG(AL_LOG_LEVEL_ERROR, "CAN bit error!\r\n");
        // if (Dev->State == AL_CAN_STATE_RECV_BUSY) {
        //     AL_LOG(AL_LOG_LEVEL_DEBUG, "CAN recv bit error, only in ACK slot, data valid!\r\n");
        //     return AL_CAN_ERR_KOER_RECV_BIT;
        // } else {
        //     AL_LOG(AL_LOG_LEVEL_DEBUG, "CAN send bit error, data invalid!\r\n");
        //     return AL_CAN_ERR_KOER_SEND_BIT;
        // }
        break;
    case AL_CAN_KOER_FORM:
        AL_LOG(AL_LOG_LEVEL_ERROR, "CAN form error!\r\n");
        return AL_CAN_ERR_KOER_FORM;
        break;
    case AL_CAN_KOER_STUFF:
        AL_LOG(AL_LOG_LEVEL_ERROR, "CAN stuff error!\r\n");
        // if (Dev->State == AL_CAN_STATE_RECV_BUSY) {
        //     AL_LOG(AL_LOG_LEVEL_DEBUG, "CAN recv stuff error, data invalid!\r\n");
        //     return AL_CAN_ERR_KOER_RECV_STUFF;
        // } else {
        //     AL_LOG(AL_LOG_LEVEL_DEBUG, "CAN send stuff error, only happen during arbitration, data invalid!\r\n");
        //     return AL_CAN_ERR_KOER_SEND_STUFF;
        // }
        break;
    case AL_CAN_KOER_ACK:
        AL_LOG(AL_LOG_LEVEL_ERROR, "CAN ack error!\r\n");
        // if (Dev->State == AL_CAN_STATE_RX_BUSY) {
        //     AL_LOG(AL_LOG_LEVEL_DEBUG, "CAN recv ack error, only in LOM, data valid!\r\n");
        //     return AL_CAN_ERR_KOER_RECV_ACK;
        // } else {
        //     AL_LOG(AL_LOG_LEVEL_DEBUG, "CAN send ack error, only loop back without self-ack, data valid!\r\n");
        //     return AL_CAN_ERR_KOER_SEND_ACK;
        // }
        break;
    case AL_CAN_KOER_CRC:
        AL_LOG(AL_LOG_LEVEL_ERROR, "CAN crc error, data invalid!\r\n");
        return AL_CAN_ERR_KOER_CRC;
        break;
    case AL_CAN_KOER_OTHER:
        AL_LOG(AL_LOG_LEVEL_ERROR, "CAN other error!\r\n");
        return AL_CAN_ERR_KOER_OTHER;
        break;
    default :
        AL_LOG(AL_LOG_LEVEL_ERROR, "CAN wrong KOER parameter\r\n");
        return AL_CAN_ERR_WRONG_KOER_PARAMETER;
        break;
    }

    return AL_OK;
}

/**
 * This function sets the bit rate for a CAN device.
 *
 * @param Dev Pointer to the CAN device structure.
 * @param BitRateCfg Pointer to the bit rate configuration structure.
 * @return AL_OK on success, error code otherwise.
 */
AL_S32 AlCan_Dev_ModifyHwConfig(AL_U32 DevId, AL_U32 Type, AL_U32 SlowBitRate, AL_U32 FastBitRate)
{
    AL_S32 Ret = AL_OK;

    AL_ASSERT(DevId < AL_CAN_NUM_INSTANCE, AL_CAN_ERR_ILLEGAL_PARAM);
    AL_ASSERT(Type == CANFD || Type == CAN20B, AL_CAN_ERR_ILLEGAL_PARAM);

    AlCan_HwConfig[DevId].Type = Type;
    if (SlowBitRate != 0) {
        AlCan_HwConfig[DevId].SlowBitRate = SlowBitRate;
    }

    if (Type == CANFD && FastBitRate != 0) {
        AlCan_HwConfig[DevId].FastBitRate = FastBitRate;
    }

    return AL_OK;
}

/**
 * This function validates the device pointer and switches on the command type. It supports setting
 * and getting bit rates, resetting the device, setting and getting filter configurations,
 * and getting and setting the device state. If the command is not recognized, an error is returned.
 *
 * @param Dev Pointer to the CAN device structure.
 * @param Cmd The control command to execute.
 * @param Data Pointer to the data required for the command.
 * @return AL_OK on success, AL_CAN_ERR_IOCTL_CMD if the command is unrecognized, or other error codes depending on the command execution result.
 *
 */
AL_S32 AlCan_Dev_IoCtl(AL_CAN_DevStruct *Dev, AL_CAN_IoCtlCmdEnum Cmd, AL_VOID *Data)
{
    AL_S32 Ret = AL_OK;

    AL_ASSERT(Dev != AL_NULL, AL_CAN_ERR_NULL_PTR);

    switch (Cmd)
    {
    case AL_CAN_IOCTL_SET_BIT_RATE: {
        AL_CAN_BitRateCfgStruct *SetBitRate = (AL_CAN_BitRateCfgStruct *)Data;
        AlCan_Dev_SetBitRate(Dev, SetBitRate);
        break;
    }
    case AL_CAN_IOCTL_GET_BIT_RATE: {
        AL_CAN_BitRateCfgStruct *GetBitRate = (AL_CAN_BitRateCfgStruct *)Data;
        AlCan_Dev_GetBitRate(Dev, GetBitRate);
        break;
    }
    case AL_CAN_IOCTL_SET_RESET: {
        AL_BOOL IsResetEnabled = *(AL_BOOL *)Data;
        AlCan_Dev_SetReset(Dev, IsResetEnabled);
        break;
    }
    case AL_CAN_IOCTL_SET_FILTER: {
        AL_CAN_FilterCfgStruct *SetFilterCfg = (AL_CAN_FilterCfgStruct *)Data;
        AlCan_Dev_SetFilter(Dev, SetFilterCfg);
        break;
    }
    case AL_CAN_IOCTL_GET_FILTER: {
        AL_CAN_FilterCfgStruct *GetFilterCfg = (AL_CAN_FilterCfgStruct *)Data;
        AlCan_Dev_GetFilter(Dev, GetFilterCfg);
        break;
    }
    case AL_CAN_IOCTL_GET_STATE: {
        AL_CAN_StateEnum *GetState = (AL_CAN_StateEnum *)Data;
        *GetState = Dev->State;
        break;
    }
    case AL_CAN_IOCTL_SET_STATE: {
        AL_CAN_StateEnum SetState = *(AL_CAN_StateEnum *)Data;
        AlCan_Dev_SetState(Dev, SetState);
        break;
    }
    case AL_CAN_IOCTL_CLR_STATE: {
        AL_CAN_StateEnum ClrState = *(AL_CAN_StateEnum *)Data;
        AlCan_Dev_ClrState(Dev, ClrState);
        break;
    }
    default:
        return AL_CAN_ERR_IOCTL_CMD;
        break;
    }

    return Ret;
}

/**
 * This function validates the frame pointer to ensure it is not NULL. It then logs the frame's ID, data length code (DLC),
 * actual data length in bytes, and whether the ID is extended, the frame is remote, and the bit rate is switched.
 * It iterates through the data words of the frame, logging each one. The function concludes by logging a completion message.
 *
 * @param Frame Pointer to the CAN frame structure to be displayed.
 * @return AL_OK on successful execution, AL_CAN_ERR_NULL_PTR if the frame pointer is NULL.
 */
AL_S32 AlCan_Dev_DisplayFrame(AL_CAN_FrameStruct *Frame)
{
    AL_ASSERT(Frame != AL_NULL, AL_CAN_ERR_NULL_PTR);

    AL_LOG(AL_LOG_LEVEL_INFO, "-------Recv Frame--------\r\n");
    AL_LOG(AL_LOG_LEVEL_INFO, "| Id: 0x%08x\r\n", Frame->Id);
    AL_LOG(AL_LOG_LEVEL_INFO, "| Dlc: 0x%08x\r\n", Frame->DataLen);
    AL_LOG(AL_LOG_LEVEL_INFO, "| Data len: 0x%d\r\n", AlCan_Dev_Dlc2LenInByte(Frame->DataLen));
    if (Frame->IsIdExt == AL_TRUE) {
        AL_LOG(AL_LOG_LEVEL_INFO, "| Extern Id\r\n");
    } else {
        AL_LOG(AL_LOG_LEVEL_INFO, "| Standard Id\r\n");
    }
    if (Frame->IsRemote == AL_TRUE) {
        AL_LOG(AL_LOG_LEVEL_INFO, "| Remote frame\r\n");
    } else {
        AL_LOG(AL_LOG_LEVEL_INFO, "| Not remote frame\r\n");
    }
    if (Frame->IsBitSwitch == AL_TRUE) {
        AL_LOG(AL_LOG_LEVEL_INFO, "| Switch fast bit rate\r\n");
    } else {
        AL_LOG(AL_LOG_LEVEL_INFO, "| Nominal bit rate\r\n");
    }
    AL_U32 DataWordLen = AlCan_Dev_Dlc2Len(Frame->DataLen);
    AL_LOG(AL_LOG_LEVEL_INFO, "| Data length in word is %d\r\n", DataWordLen);
    for (AL_U32 i = 0; i < DataWordLen; i++) {
        AL_LOG(AL_LOG_LEVEL_INFO, "| Data %02d: 0x%08x\r\n", i, Frame->Data[i]);
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "-----Recv Frame Done-----\r\n");

    return AL_OK;
}

/**
 * This function display controller config
 * @param   Dev is pointer to AL_CAN_DevStruct
 * @return
 *          - AL_OK is send done
 * @note
*/
AL_S32 AlCan_Dev_DisplayConfig(AL_CAN_DevStruct *Dev)
{
    AL_ASSERT(Dev != AL_NULL, AL_CAN_ERR_NULL_PTR);

    AL_LOG(AL_LOG_LEVEL_INFO, "-------IP Config--------\r\n");
    AL_LOG(AL_LOG_LEVEL_INFO, "| Input clk: %d\r\n", Dev->Config.InputClk);
    AL_LOG(AL_LOG_LEVEL_INFO, "| Mode: %s\r\n", ModeInfo[Dev->Config.OpsMode]);
    AL_LOG(AL_LOG_LEVEL_INFO, "| Type: %s\r\n", (Dev->Config.Type == AL_CAN_TYPE_FD) ? "FD" : "2_0B");
    AL_LOG(AL_LOG_LEVEL_INFO, "| Trans mode: %s\r\n", TransModeInfo[Dev->Config.TransMode]);
    AL_LOG(AL_LOG_LEVEL_INFO, "| Slow bit rate: %d\r\n", Dev->Config.SlowBitRate.Rate);
    AL_LOG(AL_LOG_LEVEL_INFO, "| Fast bit rate: %d\r\n", Dev->Config.FastBitRate.Rate);
    AL_LOG(AL_LOG_LEVEL_INFO, "-----IP Config Done-----\r\n");

    return AL_OK;
}
