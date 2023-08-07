#include "al_iic_dev.h"

static AL_IIC_InitStruct IicDefInitConfigs =
{
    .Mode           = AL_IIC_MODE_MASTER,
    .AddrMode       = AL_IIC_ADDR_7BIT,
    .SpeedMode      = AL_IIC_STANDARD_MODE,
};

extern AL_IIC_HwConfigStruct AlIic_HwConfig[AL_IIC_NUM_INSTANCE];

AL_IIC_HwConfigStruct *AlIic_Dev_LookupConfig(AL_U32 DevId)
{
    AL_U32 Index;
    AL_IIC_HwConfigStruct *ConfigPtr = AL_NULL;

    for (Index = 0; Index < AL_IIC_NUM_INSTANCE; Index++) {
        if (AlIic_HwConfig[Index].DeviceId == DevId) {
            ConfigPtr = &AlIic_HwConfig[Index];
            break;
        }
    }

    return ConfigPtr;
}

AL_BOOL AlIic_Dev_IsTxBusy(AL_IIC_DevStruct *Iic)
{
    return (AL_BOOL)(Iic->State & AL_IIC_STATE_TX_BUSY);
}

AL_BOOL AlIic_Dev_IsRxBusy(AL_IIC_DevStruct *Iic)
{
    return (AL_BOOL)(Iic->State & AL_IIC_STATE_RX_BUSY);
}

AL_VOID AlIic_Dev_SetTxBusy(AL_IIC_DevStruct *Iic)
{
    Iic->State |= AL_IIC_STATE_TX_BUSY;
}

AL_VOID AlIic_Dev_SetRxBusy(AL_IIC_DevStruct *Iic)
{
    Iic->State |= AL_IIC_STATE_RX_BUSY;
}

AL_VOID AlIic_Dev_ClrTxBusy(AL_IIC_DevStruct *Iic)
{
    Iic->State &= (~AL_IIC_STATE_TX_BUSY);
}

AL_VOID AlIic_Dev_ClrRxBusy(AL_IIC_DevStruct *Iic)
{
    Iic->State &= (~AL_IIC_STATE_RX_BUSY);
}

static AL_VOID AlIic_Dev_InitSclHighLowCout(AL_IIC_DevStruct *Iic, AL_IIC_SpeedModeEnum SpeedMode)
{
    AL_REG IicBaseAddr = (AL_REG)(Iic->HwConfig.BaseAddress);
    AL_U16 ClockMHz = (Iic->HwConfig.ClockHz) / 1000000;

    if (SpeedMode == AL_IIC_STANDARD_MODE) {
        AL_U16 SsSclHigh, SsSclLow;

        SsSclHigh = ((AL_U16) (((AL_IIC_SS_MIN_SCL_HIGH * ClockMHz) / 1000) + (25-7)));
        SsSclLow = ((AL_U16) (((AL_IIC_SS_MIN_SCL_LOW * ClockMHz) / 1000) + 25));

        AlIic_ll_SetStdSpeedSclHighCount(IicBaseAddr, SsSclHigh);
        AlIic_ll_SetStdSpeedSclLowCount(IicBaseAddr, SsSclLow);
    } else if (SpeedMode == AL_IIC_FAST_MODE) {
        AL_U16 FsSclHigh, FsSclLow;

        FsSclHigh = ((AL_U16) (((AL_IIC_FS_MIN_SCL_HIGH * ClockMHz) / 1000) + (25-7)));
        FsSclLow = ((AL_U16) (((AL_IIC_FS_MIN_SCL_LOW * ClockMHz) / 1000) + 25));

        AlIic_ll_SetFastSpeedSclHighCount(IicBaseAddr, FsSclHigh);
        AlIic_ll_SetFastSpeedSclLowCount(IicBaseAddr, FsSclLow);
    } else {
        AL_U16 HsSclHigh, HsSclLow;

        HsSclHigh = ((AL_U16) (((AL_IIC_HS_MIN_SCL_HIGH_100PF * ClockMHz) / 1000) + (25-7)));
        HsSclLow  = ((AL_U16) (((AL_IIC_HS_MIN_SCL_LOW_100PF * ClockMHz) / 100) + 25));

        AlIic_ll_SetHighSpeedSclHighCount(IicBaseAddr, HsSclHigh);
        AlIic_ll_SetHighSpeedSclLowCount(IicBaseAddr, HsSclLow);
    }
}

static AL_S32 AlIic_Dev_CheckConfigParam(AL_IIC_InitStruct *InitConfig)
{
    if (!(InitConfig->Mode == AL_IIC_MODE_MASTER || InitConfig->Mode == AL_IIC_MODE_SLAVE)) {
        return AL_IIC_ERR_ILLEGAL_PARAM;
    }

    if (!(InitConfig->AddrMode == AL_IIC_ADDR_7BIT || InitConfig->AddrMode == AL_IIC_ADDR_10BIT)) {
        return AL_IIC_ERR_ILLEGAL_PARAM;
    }

    if (!(InitConfig->SpeedMode == AL_IIC_STANDARD_MODE || InitConfig->SpeedMode == AL_IIC_FAST_MODE ||
          InitConfig->SpeedMode == AL_IIC_HIGH_SPEED_MODE)) {
        return AL_IIC_ERR_ILLEGAL_PARAM;
    }

    return AL_OK;
}

AL_S32 AlIic_Dev_Init(AL_IIC_DevStruct *Iic, AL_IIC_HwConfigStruct *HwConfig, AL_IIC_InitStruct *InitConfig)
{
    AL_S32 Ret;
    AL_REG IicBaseAddr;

    if (Iic == AL_NULL) {
        return AL_IIC_ERR_ILLEGAL_PARAM;
    }

    /* check the InitConfig */
    if (InitConfig != AL_NULL) {
        Ret = AlIic_Dev_CheckConfigParam(InitConfig);
        if (Ret != AL_OK) {
            return Ret;
        }

        Iic->Configs = *InitConfig;
    } else {
        Iic->Configs = IicDefInitConfigs;
    }

    Iic->HwConfig = *HwConfig;
    IicBaseAddr = (AL_REG)(Iic->HwConfig.BaseAddress);

    AlIic_ll_SetEnable(IicBaseAddr, AL_IIC_FUNC_DISABLE);

    if (InitConfig->Mode == AL_IIC_MODE_MASTER) {
        AlIic_ll_SetSlaveDisable(IicBaseAddr, AL_IIC_FUNC_ENABLE);
        AlIic_ll_SetRestartEnable(IicBaseAddr, AL_IIC_FUNC_ENABLE);
        AlIic_ll_SetMasterAddrMode(IicBaseAddr, InitConfig->AddrMode);
    } else {
        AlIic_ll_SetSar(IicBaseAddr, InitConfig->SarAddress);
        AlIic_ll_SetSlaveDisable(IicBaseAddr, AL_IIC_FUNC_DISABLE);
        AlIic_ll_SetSlaveAddrMode(IicBaseAddr, InitConfig->AddrMode);
    }

    AlIic_ll_SetSpeedMode(IicBaseAddr, InitConfig->SpeedMode);

    if (InitConfig->Mode == AL_IIC_MODE_MASTER) {
        AlIic_ll_SetMaterEnable(IicBaseAddr, AL_IIC_FUNC_ENABLE);
//        AlIic_ll_SetTar(IicBaseAddr, InitConfig->Address);
        AlIic_Dev_InitSclHighLowCout(Iic, InitConfig->SpeedMode);

        Iic->CmdOption = AL_IIC_CMD_OPTION_STOP;
    } else {
        AlIic_ll_SetMaterEnable(IicBaseAddr, AL_IIC_FUNC_DISABLE);
    }

    AlIic_ll_MaskAllIntr(IicBaseAddr);

    /* Bus clear feture */
    AlIic_ll_SetBusClrFeatureCtrl(IicBaseAddr, AL_IIC_FUNC_ENABLE);
    AlIic_ll_SetSclStuckLowTimeout(IicBaseAddr, DEFAULT_SDA_SCL_AT_LOW_TIMEOUT);
    AlIic_ll_SetSdaStuckLowTimeout(IicBaseAddr, DEFAULT_SDA_SCL_AT_LOW_TIMEOUT);
    AlIic_ll_SetSclStuckAtLowIntr(IicBaseAddr, AL_IIC_FUNC_ENABLE);

    AlIic_ll_SetSdaTxHold(IicBaseAddr, 4);
    AlIic_ll_SetSdaRxHold(IicBaseAddr, 1);

    // todo, get tx/rx fifo depth here
    AlIic_ll_SetRxFifoThr(IicBaseAddr, 0);
    AlIic_ll_SetTxFifoThr(IicBaseAddr, 8);

    AlIic_ll_SetEnable(IicBaseAddr, AL_IIC_FUNC_ENABLE);

    Iic->State |= AL_IIC_STATE_READY;

    return AL_OK;
}

static inline AL_VOID AlIic_Dev_MasterSetTar(AL_IIC_DevStruct *Iic, AL_U16 SlaveAddr)
{
    /* I2C_DYNAMIC_TAR_UPDATE not enabled, need disable IIC first */
    AlIic_ll_SetEnable((AL_REG)(Iic->HwConfig.BaseAddress), AL_IIC_FUNC_DISABLE);

    AlIic_ll_SetTar((AL_REG)(Iic->HwConfig.BaseAddress), SlaveAddr);

    AlIic_ll_SetEnable((AL_REG)(Iic->HwConfig.BaseAddress), AL_IIC_FUNC_ENABLE);
}

AL_S32 AlIic_Dev_MasterSendData(AL_IIC_DevStruct *Iic, AL_U16 SlaveAddr, AL_U8 *SendBuf, AL_U32 SendSize)
{
    AL_U32 DataCmd;
    AL_U8 Data;
    AL_REG IicBaseAddr;

    if (SendBuf == AL_NULL || SendSize == 0 || SlaveAddr > AL_IIC_MAX_SLAVE_ADDR) {
        return AL_IIC_ERR_ILLEGAL_PARAM;
    }

    if ((Iic->State & AL_IIC_STATE_READY) == 0) {
        return AL_IIC_ERR_NOT_READY;
    }

    if (AlIic_Dev_IsTxBusy(Iic)) {
        return AL_IIC_ERR_BUSY;
    }

    /* Change Status */
    AlIic_Dev_SetTxBusy(Iic);

    AlIic_Dev_MasterSetTar(Iic, SlaveAddr);

    IicBaseAddr = (AL_REG)(Iic->HwConfig.BaseAddress);

    Iic->OpState = AL_IIC_OP_STATE_MASTER_TX;

    Iic->SendBuffer.BufferPtr      = SendBuf;
    Iic->SendBuffer.RequestedCnt   = SendSize;
    Iic->SendBuffer.HandledCnt     = 0;

    /*
       Need to trigger tx empty interrupt, send data in the interrupt handler
     */
    AlIic_ll_SetTxEmptyIntr(IicBaseAddr, AL_IIC_FUNC_ENABLE);
    AlIic_ll_SetTxAbrtIntr(IicBaseAddr, AL_IIC_FUNC_ENABLE);

    return AL_OK;
}

AL_S32 AlIic_Dev_MasterRecvData(AL_IIC_DevStruct *Iic, AL_U16 SlaveAddr, AL_U8 *RecvBuf, AL_U32 RecvSize)
{
    AL_REG IicBaseAddr;
    AL_U8 RxFifoThrLevel;

    if (RecvBuf == AL_NULL || RecvSize == 0 || SlaveAddr > AL_IIC_MAX_SLAVE_ADDR) {
        return AL_IIC_ERR_ILLEGAL_PARAM;
    }

    if ((Iic->State & AL_IIC_STATE_READY) == 0) {
        return AL_IIC_ERR_NOT_READY;
    }

    if (AlIic_Dev_IsRxBusy(Iic)) {
        return AL_IIC_ERR_BUSY;
    }

    /* Change Status */
    AlIic_Dev_SetRxBusy(Iic);

    AlIic_Dev_MasterSetTar(Iic, SlaveAddr);

    IicBaseAddr = (AL_REG)(Iic->HwConfig.BaseAddress);

    Iic->OpState                    = AL_IIC_OP_STATE_MASTER_RX;

    Iic->RecvBuffer.BufferPtr       = RecvBuf;
    Iic->RecvBuffer.RequestedCnt    = RecvSize;
    Iic->RecvBuffer.HandledCnt      = 0;

    /*
       Use this buffer to record the number of read requests sent,
       equal to the length of the data received.
     */
    Iic->SendBuffer.RequestedCnt    = RecvSize;
    Iic->SendBuffer.HandledCnt      = 0;

    /* Make sure the fifo full interrupt is triggered, and receive data in the interrupt */
    RxFifoThrLevel = AlIic_ll_GetRxFifoThr(IicBaseAddr);
    if (RecvSize <= RxFifoThrLevel) {
        AlIic_ll_SetRxFifoThr(IicBaseAddr, RecvSize - 1);
    }

    AlIic_ll_SetTxEmptyIntr(IicBaseAddr, AL_IIC_FUNC_ENABLE);
    AlIic_ll_SetRxFullIntr(IicBaseAddr, AL_IIC_FUNC_ENABLE);
    AlIic_ll_SetTxAbrtIntr(IicBaseAddr, AL_IIC_FUNC_ENABLE);

    return AL_OK;
}

AL_S32 AlIic_Dev_SlaveSendData(AL_IIC_DevStruct *Iic, AL_U8 *SendBuf, AL_U32 SendSize)
{
    AL_U32 DataCmd;
    AL_U8 Data;
    AL_REG IicBaseAddr;

    if (SendBuf == AL_NULL || SendSize == 0) {
        return AL_IIC_ERR_ILLEGAL_PARAM;
    }

    if ((Iic->State & AL_IIC_STATE_READY) == 0) {
        return AL_IIC_ERR_NOT_READY;
    }

    if (AlIic_Dev_IsTxBusy(Iic)) {
        return AL_IIC_ERR_BUSY;
    }

    /* Change Status */
    AlIic_Dev_SetTxBusy(Iic);

    IicBaseAddr = (AL_REG)(Iic->HwConfig.BaseAddress);

    Iic->OpState = AL_IIC_OP_STATE_SLAVE_TX;

    Iic->SendBuffer.BufferPtr      = SendBuf;
    Iic->SendBuffer.RequestedCnt   = SendSize;
    Iic->SendBuffer.HandledCnt     = 0;

    /*
      For slave transmitter, tx_empty interrupt is not enabled,
      Because it sends one byte at a time in the rd_req interrupt.
     */
    AlIic_ll_SetTxAbrtIntr(IicBaseAddr, AL_IIC_FUNC_ENABLE);
    AlIic_ll_SetRdReqIntr(IicBaseAddr, AL_IIC_FUNC_ENABLE);
    AlIic_ll_SetRxDoneIntr(IicBaseAddr, AL_IIC_FUNC_ENABLE);

    return AL_OK;
}

AL_S32 AlIic_Dev_SlaveRecvData(AL_IIC_DevStruct *Iic, AL_U8 *RecvBuf, AL_U32 RecvSize)
{
    AL_U32 DataCmd;
    AL_U8 Data;
    AL_REG IicBaseAddr;
    AL_U8 RxFifoThrLevel;

    if (RecvBuf == AL_NULL || RecvSize == 0) {
        return AL_IIC_ERR_ILLEGAL_PARAM;
    }

    if ((Iic->State & AL_IIC_STATE_READY) == 0) {
        return AL_IIC_ERR_NOT_READY;
    }

    if (AlIic_Dev_IsRxBusy(Iic)) {
        return AL_IIC_ERR_BUSY;
    }

    /* Change Status */
    AlIic_Dev_SetRxBusy(Iic);

    IicBaseAddr = (AL_REG)(Iic->HwConfig.BaseAddress);

    Iic->OpState = AL_IIC_OP_STATE_SLAVE_RX;

    Iic->RecvBuffer.BufferPtr      = RecvBuf;
    Iic->RecvBuffer.RequestedCnt   = RecvSize;
    Iic->RecvBuffer.HandledCnt     = 0;

    /* Make sure the fifo full interrupt is triggered, and receive data in the interrupt */
    RxFifoThrLevel = AlIic_ll_GetRxFifoThr(IicBaseAddr);
    if (RecvSize <= RxFifoThrLevel) {
        AlIic_ll_SetRxFifoThr(IicBaseAddr, RecvSize - 1);
    }

    AlIic_ll_SetRxFullIntr(IicBaseAddr, AL_IIC_FUNC_ENABLE);

    return AL_OK;
}

static AL_VOID AlIic_Dev_MasterSendDataHandler(AL_IIC_DevStruct *Iic)
{
    AL_REG IicBaseAddr = (AL_REG)(Iic->HwConfig.BaseAddress);
    AL_U8 Data;
    AL_U16 DataCmd;

    while ((AlIic_ll_IsTxFifoNotFull(IicBaseAddr) == AL_TRUE) &&
           (Iic->SendBuffer.HandledCnt < Iic->SendBuffer.RequestedCnt)) {

        Data = Iic->SendBuffer.BufferPtr[Iic->SendBuffer.HandledCnt];

        if (Iic->SendBuffer.HandledCnt != Iic->SendBuffer.RequestedCnt - 1 )
            DataCmd = AL_IIC_CMD_WRITE | Data;
        else {
            /* Last byte, need set cmd by option */
            if (Iic->CmdOption == AL_IIC_CMD_OPTION_STOP)
                DataCmd = AL_IIC_CMD_WRITE | AL_IIC_STOP_ENABLE | Data;
            else if (Iic->CmdOption == AL_IIC_CMD_OPTION_RESTART)
                DataCmd = AL_IIC_CMD_WRITE | AL_IIC_RESTART_ENABLE | Data;
            else
                DataCmd = AL_IIC_CMD_WRITE | Data;
        }


        AlIic_ll_SetDataCmd(IicBaseAddr, DataCmd);

        Iic->SendBuffer.HandledCnt++;
    }

    if (Iic->SendBuffer.HandledCnt == Iic->SendBuffer.RequestedCnt) {
        AlIic_ll_SetTxEmptyIntr(IicBaseAddr, AL_IIC_FUNC_DISABLE);

        /* Clear Status */
        AlIic_Dev_ClrTxBusy(Iic);
    }
}

static AL_VOID AlIic_Dev_MasterRecvDataIssueReadCmd(AL_IIC_DevStruct *Iic)
{
    AL_U16 Cmd = 0;
    AL_REG IicBaseAddr = (AL_REG)(Iic->HwConfig.BaseAddress);

    while ((AlIic_ll_IsTxFifoNotFull(IicBaseAddr) == AL_TRUE) &&
           (Iic->SendBuffer.HandledCnt < Iic->SendBuffer.RequestedCnt)) {

        if (Iic->SendBuffer.HandledCnt != (Iic->SendBuffer.RequestedCnt - 1))
            Cmd = AL_IIC_CMD_READ;
        else {
            /* Last byte, need set cmd by option */
            if (Iic->CmdOption == AL_IIC_CMD_OPTION_STOP)
                Cmd = AL_IIC_CMD_READ | AL_IIC_STOP_ENABLE;
            else if (Iic->CmdOption == AL_IIC_CMD_OPTION_RESTART)
                Cmd = AL_IIC_CMD_READ | AL_IIC_RESTART_ENABLE;
            else
                Cmd = AL_IIC_CMD_WRITE;
        }

        AlIic_ll_SetDataCmd(IicBaseAddr, Cmd);

        Iic->SendBuffer.HandledCnt++;
    }

    if (Iic->SendBuffer.HandledCnt == Iic->SendBuffer.RequestedCnt) {
        AlIic_ll_SetTxEmptyIntr(IicBaseAddr, AL_IIC_FUNC_DISABLE);
    }

}

static AL_VOID AlIic_Dev_MasterRecvDataHandler(AL_IIC_DevStruct *Iic)
{
    AL_U8 Data;
    AL_U32 RxRemainCnt;
    AL_U8 RxFifoThrLevel;
    AL_REG IicBaseAddr = (AL_REG)(Iic->HwConfig.BaseAddress);

    while ((AlIic_ll_IsRxFifoNotEmpty(IicBaseAddr) == AL_TRUE) &&
           (Iic->RecvBuffer.HandledCnt < Iic->RecvBuffer.RequestedCnt)) {

        Data = AlIic_ll_ReadDataCmdData(IicBaseAddr);
        Iic->RecvBuffer.BufferPtr[Iic->RecvBuffer.HandledCnt] = Data;

        Iic->RecvBuffer.HandledCnt++;
    }

    if (Iic->RecvBuffer.HandledCnt < Iic->RecvBuffer.RequestedCnt)
    {
        RxRemainCnt = Iic->RecvBuffer.RequestedCnt - Iic->RecvBuffer.HandledCnt;
        RxFifoThrLevel = AlIic_ll_GetRxFifoThr(IicBaseAddr);
        if (RxRemainCnt <= RxFifoThrLevel) {
            AlIic_ll_SetRxFifoThr(IicBaseAddr, RxRemainCnt - 1);
        }
    } else if (Iic->RecvBuffer.HandledCnt == Iic->RecvBuffer.RequestedCnt) {
        AlIic_ll_SetTxEmptyIntr(IicBaseAddr, AL_IIC_FUNC_DISABLE);
        AlIic_Dev_ClrRxBusy(Iic);
    }

}

static AL_VOID AlIic_Dev_SlaveRecvDataHandler(AL_IIC_DevStruct *Iic)
{
    AL_U8 Data;
    AL_U8 RxFifoThrLevel;
    AL_U32 RxRemainCnt;
    AL_REG IicBaseAddr = (AL_REG)(Iic->HwConfig.BaseAddress);

    while ((AlIic_ll_IsRxFifoNotEmpty(IicBaseAddr) == AL_TRUE) &&
           (Iic->RecvBuffer.HandledCnt < Iic->RecvBuffer.RequestedCnt)) {

        Data = AlIic_ll_ReadDataCmdData(IicBaseAddr);
        Iic->RecvBuffer.BufferPtr[Iic->RecvBuffer.HandledCnt] = Data;

        Iic->RecvBuffer.HandledCnt++;
    }

    if (Iic->RecvBuffer.HandledCnt < Iic->RecvBuffer.RequestedCnt) {
        RxRemainCnt = Iic->RecvBuffer.RequestedCnt - Iic->RecvBuffer.HandledCnt;
        RxFifoThrLevel = AlIic_ll_GetRxFifoThr(IicBaseAddr);
        if (RxRemainCnt <= RxFifoThrLevel) {
            AlIic_ll_SetRxFifoThr(IicBaseAddr, RxRemainCnt - 1);
        }
    } else if (Iic->RecvBuffer.HandledCnt == Iic->RecvBuffer.RequestedCnt ) {

        /* Clear Status */
        AlIic_Dev_ClrRxBusy(Iic);
    }
}

static AL_VOID AlIic_Dev_SlaveSendDataHandler(AL_IIC_DevStruct *Iic)
{
    AL_U8 Data;
    AL_REG IicBaseAddr = (AL_REG)(Iic->HwConfig.BaseAddress);

    /* Just send one byte */
    if ((AlIic_ll_IsTxFifoNotFull(IicBaseAddr) == AL_TRUE) &&
        (Iic->SendBuffer.HandledCnt < Iic->SendBuffer.RequestedCnt)) {

        Data = Iic->SendBuffer.BufferPtr[Iic->SendBuffer.HandledCnt];

        AlIic_ll_SetDataCmd(IicBaseAddr, AL_IIC_CMD_WRITE | Data);

        Iic->SendBuffer.HandledCnt++;
    }

    if (Iic->SendBuffer.HandledCnt == Iic->SendBuffer.RequestedCnt) {

        if (Iic->EventCallBack) {
            AL_IIC_EventStruct IicEvent = {
                .Event         = AL_IIC_EVENT_RD_REQ,
                .EventData     = 0,
            };
            (*Iic->EventCallBack)(&IicEvent, Iic->EventCallBackRef);
        }

        /* Clear Status */
        AlIic_Dev_ClrTxBusy(Iic);
    }
}

static AL_VOID AlIic_Dev_SlaveSendDataDoneHandler(AL_IIC_DevStruct *Iic)
{
    if (Iic->EventCallBack) {
        AL_IIC_EventStruct IicEvent = {
            .Event         = AL_IIC_EVENT_RX_DONE,
            .EventData     =  Iic->SendBuffer.HandledCnt,
        };
        (*Iic->EventCallBack)(&IicEvent, Iic->EventCallBackRef);
    }

    AlIic_Dev_ClrRxBusy(Iic);
}

static AL_VOID AlIic_Dev_TxAbrtHandler(AL_IIC_DevStruct *Iic)
{
    AL_IIC_AbrtSrcEnum AbrtSrc = AlIic_ll_GetAbrtSrc(Iic->HwConfig.BaseAddress);

    if (Iic->EventCallBack) {
        AL_IIC_EventStruct IicEvent = {
            .Event         = AL_IIC_EVENT_TX_ABRT,
            .EventData     = AbrtSrc,
        };
        (*Iic->EventCallBack)(&IicEvent, Iic->EventCallBackRef);
    }

    if (AbrtSrc == AL_IIC_ABRT_SDA_STUCK_AT_LOW) {
        AlIic_ll_SetSdaStuckRecoveryEnable(Iic->HwConfig.BaseAddress, AL_IIC_FUNC_ENABLE);

        /* Poll for 9 scl clocks */
        while(AlIic_ll_GetSdaStuckRecoveryEnableStatus(Iic->HwConfig.BaseAddress));

        if (AlIic_ll_IsSdaStuckNotRecovery(Iic->HwConfig.BaseAddress)) {
            AL_LOG(AL_LOG_LEVEL_ALERT,"SDA at low timeout, Bus not recovered, need reset entire IIC\r\n");
        } else {
            AL_LOG(AL_LOG_LEVEL_NOTICE,"SDA at low timeout, Bus recovered\r\n");
        }
    }

}

static AL_VOID AlIic_Dev_ErrorHandler(AL_IIC_DevStruct *Iic, AL_IIC_EventIdEnum EventId)
{
    if (Iic->EventCallBack) {
        AL_IIC_EventStruct IicEvent = {
            .Event         = EventId,
            .EventData     = 0,
        };
        (*Iic->EventCallBack)(&IicEvent, Iic->EventCallBackRef);
    }
}

#define IIC_IN_RX_UNDER_INTR(Status)            (Status & AL_IIC_INTR_RX_UNDER)
#define IIC_IN_RX_OVER_INTR(Status)             (Status & AL_IIC_INTR_RX_OVER)
#define IIC_IN_RX_FULL_INTR(Status)             (Status & AL_IIC_INTR_RX_FULL)
#define IIC_IN_TX_OVER_INTR(Status)             (Status & AL_IIC_INTR_TX_OVER)
#define IIC_IN_THRE_EMPTY_INTR(Status)          (Status & AL_IIC_INTR_TX_EMPTY)
#define IIC_IN_RD_REQ_INTR(Status)              (Status & AL_IIC_INTR_RD_REQ)
#define IIC_IN_TX_ABRT_INTR(Status)             (Status & AL_IIC_INTR_TX_ABRT)
#define IIC_IN_RX_DONE_INTR(Status)             (Status & AL_IIC_INTR_RX_DONE)
#define IIC_IN_ACTIVITY_INTR(Status)            (Status & AL_IIC_INTR_ACTIVITY)
#define IIC_IN_STOP_DET_INTR(Status)            (Status & AL_IIC_INTR_STOP_DET)
#define IIC_IN_START_DET_INTR(Status)           (Status & AL_IIC_INTR_START_DET)
#define IIC_IN_GEN_CALL_INTR(Status)            (Status & AL_IIC_INTR_GEN_CALL)
#define IIC_IN_RESTART_DET_INTR(Status)         (Status & AL_IIC_INTR_RESTART_DET)
#define IIC_INTR_MASTER_ON_HOLD_INTR(Status)    (Status & AL_IIC_INTR_MASTER_ON_HOLD)
#define IIC_INTR_SCL_STUCK_ATLOW_INTR(Status)   (Status & AL_IIC_INTR_SCL_STUCK_ATLOW)

AL_VOID AlIic_Dev_IntrHandler(AL_VOID *instance)
{
    AL_IIC_DevStruct *Iic = (AL_IIC_DevStruct *)instance;
    AL_REG IicBaseAddr = (AL_REG)(Iic->HwConfig.BaseAddress);
    AL_IIC_IntrStatusEnum IntrStatus = AlIic_ll_GetIntrStatus(IicBaseAddr);

    if (IIC_IN_RX_UNDER_INTR(IntrStatus)) {
        AlIic_Dev_ErrorHandler(Iic, AL_IIC_EVENT_RX_UNDER);
        AlIic_ll_ClrRxUnder(IicBaseAddr);

    } else if (IIC_IN_RX_OVER_INTR(IntrStatus)) {
        AlIic_Dev_ErrorHandler(Iic, AL_IIC_EVENT_RX_OVER);
        AlIic_ll_ClrRxOver(IicBaseAddr);

    } else if (IIC_IN_TX_OVER_INTR(IntrStatus)) {
        AlIic_Dev_ErrorHandler(Iic, AL_IIC_EVENT_TX_OVER);
        AlIic_ll_ClrTxOver(IicBaseAddr);

    } else if (IIC_IN_RX_FULL_INTR(IntrStatus)) {
        /* Clear by hardware */
        if (Iic->OpState == AL_IIC_OP_STATE_MASTER_RX) {
            AlIic_Dev_MasterRecvDataHandler(Iic);
        } else if (Iic->OpState == AL_IIC_OP_STATE_SLAVE_RX) {
            AlIic_Dev_SlaveRecvDataHandler(Iic);
        }

    } else if (IIC_IN_THRE_EMPTY_INTR(IntrStatus)) {
        if (Iic->OpState == AL_IIC_OP_STATE_MASTER_TX) {
            AlIic_Dev_MasterSendDataHandler(Iic);
        } else if (Iic->OpState == AL_IIC_OP_STATE_MASTER_RX) {
            AlIic_Dev_MasterRecvDataIssueReadCmd(Iic);
        }

    } else if (IIC_IN_RD_REQ_INTR(IntrStatus)) {
        /* Just for slave transmitter */
        AlIic_Dev_SlaveSendDataHandler(Iic);
        AlIic_ll_ClrRdReq(IicBaseAddr);

    } else if (IIC_IN_TX_ABRT_INTR(IntrStatus)) {
        /* For Master or slave transmit, there is any data remaining in the Tx FIFO */
        AlIic_Dev_TxAbrtHandler(Iic);
        AlIic_ll_ClrTxAbrt(IicBaseAddr);

    } else if (IIC_IN_RX_DONE_INTR(IntrStatus)) {
        /* Just for slave transmitter, occurs on the last byte of the transmission */
        AlIic_Dev_SlaveSendDataDoneHandler(Iic);
        AlIic_ll_ClrRxDone(IicBaseAddr);

    } else if (IIC_IN_ACTIVITY_INTR(IntrStatus)) {
        /*
          This is a normal interrupt , Indicate that there was
          activity on the bus. do nothing, just clear it
         */
        AlIic_ll_ClrActivity(IicBaseAddr);

    } else if (IIC_IN_STOP_DET_INTR(IntrStatus)) {
        /*
          Indicates whether a STOP condition has occurred on the I2C
          interface regardless of whether DW_apb_i2c is operating in
          slave or master mode. do nothing, jusr clear it
         */
        AlIic_ll_ClrStopDet(IicBaseAddr);

    } else if (IIC_IN_START_DET_INTR(IntrStatus)) {
        /*
          Indicates whether a START or RESTART condition has
          occurred on the I2C interface regardless of whether
          DW_apb_i2c is operating in slave or master mode. do nothing, jusr clear it
         */
        AlIic_ll_ClrStartDet(IicBaseAddr);

    } else if (IIC_IN_GEN_CALL_INTR(IntrStatus)) {
        AlIic_ll_ClrGenCall(IicBaseAddr);

    } else if (IIC_INTR_SCL_STUCK_ATLOW_INTR(IntrStatus)) {
        AlIic_ll_ClrSclStuckDet(IicBaseAddr);
        AL_LOG(AL_LOG_LEVEL_ALERT,"SCL at low, Need reset entire IIC\r\n");
    }

}

AL_S32 AlIic_Dev_RegisterEventCallBack(AL_IIC_DevStruct *Iic, AL_IIC_EventCallBack Callback, void *CallbackRef)
{
    if (Iic == AL_NULL || Callback == AL_NULL) {
        return AL_IIC_ERR_ILLEGAL_PARAM;
    }

    Iic->EventCallBack        = Callback;
    Iic->EventCallBackRef     = CallbackRef;

    return AL_OK;
}

AL_S32 AlIic_Dev_MasterSetCmdOption(AL_IIC_DevStruct *Iic, AL_IIC_CmdOptionEnum CmdOption)
{
    if (Iic == AL_NULL) {
        return AL_IIC_ERR_ILLEGAL_PARAM;
    }

    switch (CmdOption)
    {
    case AL_IIC_CMD_OPTION_NO_STOP_RESTART:
    case AL_IIC_CMD_OPTION_STOP:
    case AL_IIC_CMD_OPTION_RESTART:
        Iic->CmdOption = CmdOption;
        break;

    default:
        return AL_IIC_ERR_ILLEGAL_PARAM;
    }

    return AL_OK;
}

AL_IIC_CmdOptionEnum AlIic_Dev_MasterGetCmdOption(AL_IIC_DevStruct *Iic)
{
    if (Iic == AL_NULL) {
        return AL_IIC_CMD_OPTION_NONE;
    }

    return Iic->CmdOption;
}

AL_S32 AlIic_Dev_IoCtl(AL_IIC_DevStruct *Iic, AL_IIC_IoCtlCmdEnum Cmd, AL_VOID *Data)
{
    AL_S32 Ret = AL_OK;

    if (Iic == AL_NULL) {
        return AL_IIC_ERR_ILLEGAL_PARAM;
    }

    AL_REG IicBaseAddr = (AL_REG)(Iic->HwConfig.BaseAddress);

    switch (Cmd)
    {
    case AL_IIC_IOCTL_SET_ADDR_MODE: {
        AL_IIC_AddrModeEnum AddrMode = *(AL_IIC_AddrModeEnum *)Data;
        AlIic_ll_SetMasterAddrMode(IicBaseAddr, AddrMode);
        break;
    }
    case AL_IIC_IOCTL_GET_ADDR_MODE: {
        AL_IIC_AddrModeEnum *AddrMode = (AL_IIC_AddrModeEnum *)Data;
        *AddrMode = AlIic_ll_GetMasterAddrMode(IicBaseAddr);
        break;
    }
    case AL_IIC_IOCTL_SET_SPEED_MODE: {
        AL_IIC_SpeedModeEnum SpeedMode = *(AL_IIC_SpeedModeEnum *)Data;
        AlIic_ll_SetSpeedMode(IicBaseAddr, SpeedMode);
        AlIic_Dev_InitSclHighLowCout(Iic, SpeedMode);
        break;
    }
    case AL_IIC_IOCTL_GET_SPEED_MODE: {
        AL_IIC_SpeedModeEnum *SpeedMode = (AL_IIC_SpeedModeEnum *)Data;
        *SpeedMode = AlIic_ll_GetSpeedMode(IicBaseAddr);
        break;
    }
    default:
        return AL_IIC_ERR_IOCTL_CMD_NOT_SUPPORT;
        break;
    }

    return Ret;
}