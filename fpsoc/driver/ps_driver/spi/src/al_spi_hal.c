/***************************** Include Files *********************************/
#include "al_spi_hal.h"
#include "al_intr.h"
#include "al_dmacahb_hal.h"
/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Variable Definitions *****************************/
static AL_SPI_DevStruct AL_SPI_DevInstance[AL_SPI_NUM_INSTANCE];

/********************************************************/
#ifdef USE_RTOS
/**
 * This function
 * @param   Handle is pointer to AL_SPI_HalStruct
 * @param   Timeout is max wait time
 * @return  Return whether to send timeout
 * @note    None
*/
static AL_S32 AlSpi_Hal_WaitTxDoneOrTimeout(AL_SPI_HalStruct *Handle, AL_U32 Timeout)
{
    /*
     * TODO: wait for event timeout
    */
    (void) Handle;
    (void) Timeout;
}

/**
 * This function
 * @param   Handle is pointer to AL_SPI_HalStruct
 * @param   Timeout is max wait time
 * @return  Return whether to send or receive timeout
 * @note    None
*/
static AL_S32 AlSpi_Hal_WaitRxDoneOrTimeout(AL_SPI_HalStruct *Handle, AL_U32 Timeout)
{
    /*
     * TODO:wait for event timeout
    */
    (void) Handle;
    (void) Timeout;
}

/**
 * This function
 * @param   Handle is pointer to AL_SPI_HalStruct
 * @param
 * @param
 * @return
 * @note    None
*/
static AlSpi_Hal_IntrHandler(AL_SPI_HalStruct *Handle, AL_U32 Event, AL_U32 EventData)
{
    /*
     * TODO:send event
    */
    (void) Handle;
    (void) Event;
    (void) EventData;
}

#define AL_SPI_HAL_LOCK(Handle)        do {} while (0)

#define AL_SPI_HAL_UNLOCK(Handle)      do {} while (0)

#else

/**
 * This function is wait send done or timeout
 * @param   Handle is pointer to AL_SPI_HalStruct
 * @param   Timeout is max wait time
 * @return  Return whether to send timeout
 * @note    None
*/
static AL_S32 AlSpi_Hal_WaitTxDoneOrTimeout(AL_SPI_HalStruct *Handle, AL_U32 Timeout)
{
    extern AL_BOOL AlSpi_Dev_IsTxBusy(AL_SPI_DevStruct *Spi);

#ifdef SPI_DEBUG
    AlSpi_Dev_DumpReg(Handle->Dev->BaseAddr);
    AL_LOG(AL_LOG_LEVEL_DEBUG, "AlSpi_Dev_IsTxBusy:%d\r\n", AlSpi_Dev_IsTxBusy(Handle->Dev));
#endif

    while(AlSpi_Dev_IsTxBusy(Handle->Dev));
    while(SPI_SR_BUSY == AlSpi_ll_IsBusy(Handle->Dev->BaseAddr));

    AlSpi_ll_SetSlvSel(Handle->Dev->BaseAddr, 0);


    return AL_OK;
}

/**
 * This function is wait receive done or timeout
 * @param   Handle is pointer to AL_SPI_HalStruct
 * @param   Timeout is max wait time
 * @return  Return whether to receive timeout
 * @note    None
*/
static AL_S32 AlSpi_Hal_WaitRxDoneOrTimeout(AL_SPI_HalStruct *Handle, AL_U32 Timeout)
{
    extern AL_BOOL AlSpi_Dev_IsRxBusy(AL_SPI_DevStruct *Spi);

#ifdef SPI_DEBUG
    AlSpi_Dev_DumpReg(Handle->Dev->BaseAddr);
    AL_LOG(AL_LOG_LEVEL_DEBUG, "AlSpi_Dev_IsRxBusy:%d\r\n", AlSpi_Dev_IsRxBusy(Handle->Dev));
#endif

    while(AlSpi_Dev_IsRxBusy(Handle->Dev));
    while(SPI_SR_BUSY == AlSpi_ll_IsBusy(Handle->Dev->BaseAddr));

    AlSpi_ll_SetSlvSel(Handle->Dev->BaseAddr, 0);


    return AL_OK;
}

/**
 * This function is wait receive done and send done or timeout
 * @param   Handle is pointer to AL_SPI_HalStruct
 * @param   Timeout is max wait time
 * @return  Return whether to send and receive timeout
 * @note    None
*/
static AL_S32 AlSpi_Hal_WaitTxRxDoneOrTimeout(AL_SPI_HalStruct *Handle, AL_U32 Timeout)
{
    extern AL_BOOL AlSpi_Dev_IsRxBusy(AL_SPI_DevStruct *Spi);
    extern AL_BOOL AlSpi_Dev_IsTxBusy(AL_SPI_DevStruct *Spi);
#ifdef SPI_DEBUG
    AlSpi_Dev_DumpReg(Handle->Dev->BaseAddr);
    AL_LOG(AL_LOG_LEVEL_DEBUG, "AlSpi_Dev_IsRxBusy:%d\r\n", AlSpi_Dev_IsRxBusy(Handle->Dev));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "AlSpi_Dev_IsTxBusy:%d\r\n", AlSpi_Dev_IsTxBusy(Handle->Dev));
#endif
    while(AlSpi_Dev_IsRxBusy(Handle->Dev) || AlSpi_Dev_IsTxBusy(Handle->Dev));
    while(SPI_SR_BUSY == AlSpi_ll_IsBusy(Handle->Dev->BaseAddr));

    AlSpi_ll_SetSlvSel(Handle->Dev->BaseAddr, 0);


    return AL_OK;
}



#define AL_SPI_HAL_LOCK(Handle)          do {} while (0)

#define AL_SPI_HAL_UNLOCK(Handle)        do {} while (0)

#endif

/**
 * This is default event callback function
 * @param   SpiEvent is a AL_SPI_EventStruct struct
 * @param   CallbackRef is parameter of callback function
 * @return  AL_OK
 * @note    None
*/
static AL_S32 AlSpi_DefEventCallBack(AL_SPI_EventStruct SpiEvent, void *CallbackRef)
{
    switch (SpiEvent.Event) {
    case AL_SPI_SEND_DONE:
#ifdef SPI_DEBUG
       AL_LOG(AL_LOG_LEVEL_DEBUG, "AlSpi_DefEventCallBack AL_SPI_SEND_DONE\r\n");
#endif
        break;
    case AL_SPI_RECEIVE_DONE:
#ifdef SPI_DEBUG
       AL_LOG(AL_LOG_LEVEL_DEBUG, "AlSpi_DefEventCallBack AL_SPI_RECEIVE_DONE\r\n");
#endif
        break;
    case AL_SPI_SEND_TIMEOUT:
#ifdef SPI_DEBUG
        AL_LOG(AL_LOG_LEVEL_DEBUG, "AlSpi_DefEventCallBack AL_SPI_SEND_TIMEOUT\r\n");
#endif
        break;
    case AL_SPI_RECEIVE_TIMEOUT:
#ifdef SPI_DEBUG
        AL_LOG(AL_LOG_LEVEL_DEBUG, "AlSpi_DefEventCallBack AL_SPI_RECEIVE_TIMEOUT\r\n");
#endif
        break;
    default:
#ifdef SPI_DEBUG
        AL_LOG(AL_LOG_LEVEL_DEBUG, "AlSpi_DefEventCallBack default\r\n");
#endif
        break;
    }

    return AL_OK;
}

/**
 * This function initialize the spi according to the specified
 *          parameters in the AL_SPI_ConfigsStruct and initialize the associated handle.
 * @param   Handle is pointer to AL_SPI_HalStruct
 * @param   InitConfig pointer to a AL_SPI_ConfigsStruct structure
 *          that contains the configuration information for the specified spi peripheral
 * @param   Callback is a function pointer to spi event callback function
 * @param   CallbackRef is parameter of callback function
 * @param   DevId is hardware module id
 * @return  The state of function execution
 * @note    None
*/
AL_S32 AlSpi_Hal_Init(AL_SPI_HalStruct *Handle, AL_SPI_ConfigsStruct *InitConfig, SPI_EventCallBack Callback,
                      AL_VOID *CallbackRef, AL_U32 DevId)
{
    AL_S32 ret = AL_OK;
    AL_SPI_HwConfigStruct *CfgPtr = AL_NULL;

    if (Handle == AL_NULL) {
        return AL_SPI_ERR_ILLEGAL_PARAM;
    }

    AL_SPI_HAL_LOCK(Handle);

    CfgPtr = AlSpi_Dev_LookupConfig(DevId);
    Handle->Dev = &AL_SPI_DevInstance[CfgPtr->DeviceId];
    Handle->Dev->BaseAddr = CfgPtr->BaseAddress;
    Handle->Dev->Fifolen  = CfgPtr->FifoLen;



    if (ret != AL_OK) {
        AL_SPI_HAL_UNLOCK(Handle);
        return ret;
    }

    ret = AlSpi_Dev_Init(Handle->Dev, InitConfig);
    if (ret != AL_OK) {
        AL_SPI_HAL_UNLOCK(Handle);
        return ret;
    }

    if (AL_SPI_USE_DMA != InitConfig->IsUseDma) {
        if(AL_NULL == Callback) {
            ret = AlSpi_Dev_RegisterIntrCallBack(Handle->Dev, AlSpi_DefEventCallBack, AL_NULL);
        } else {
            ret = AlSpi_Dev_RegisterIntrCallBack(Handle->Dev, Callback, CallbackRef);
        }

        AlIntr_RegHandler(CfgPtr->InterrupId + 13, AL_NULL, AlSpi_Dev_IntrHandler, Handle->Dev);
    } else {
        AlIntr_SetInterrupt(CfgPtr->InterrupId + 13, AL_FUNC_DISABLE);
    }

    AL_SPI_HAL_UNLOCK(Handle);

    return ret;
}

/**
 * This function is spi blocking send data
 * @param   Handle is pointer to AL_SPI_HalStruct
 * @param   Data is pointer to send data
 * @param   Size is send data size
 * @param   Timeout is max wait time for send done
 * @return  The state of function execution
 * @note    None
*/
AL_S32 AlSpi_Hal_SendDataBlock(AL_SPI_HalStruct *Handle, AL_U8 *Data, AL_U32 Size, AL_U32 Timeout)
{
    AL_S32 ret = AL_OK;

    /* check only Handle, more checks in AlSpi_Dev_Init function */
    if (Handle == AL_NULL) {
        return AL_SPI_ERR_ILLEGAL_PARAM;
    }

    AL_SPI_HAL_LOCK(Handle);

    ret = AlSpi_Dev_SendData(Handle->Dev, Data, Size);
    if (ret != AL_OK) {
        AL_SPI_HAL_UNLOCK(Handle);
        return ret;
    }

    /* wait until data send done */
    ret = AlSpi_Hal_WaitTxDoneOrTimeout(Handle, Timeout);
    if (ret != AL_OK) {
        ret = AL_SPI_ERR_TIMEOUT;
        AlSpi_ll_MaskIntr(Handle->Dev->BaseAddr, SPI_TXEIM);
    }

    AL_SPI_HAL_UNLOCK(Handle);

    return ret;
}

/**
 * This function is spi blocking receive data
 * @param   Handle is pointer to AL_SPI_HalStruct
 * @param   Data is pointer to receive data
 * @param   Size is receive data size
 * @param   Timeout is max wait time for receive done
 * @return  The state of function execution
 * @note    None
*/
AL_S32 AlSpi_Hal_RecvDataBlock(AL_SPI_HalStruct *Handle, AL_U8 *Data, AL_U32 Size, AL_U32 Timeout)
{
    AL_S32 ret = AL_OK;

    /* check only Handle, more checks in AlSpi_Dev_Init function */
    if (Handle == AL_NULL) {
        return AL_SPI_ERR_ILLEGAL_PARAM;
    }

    AL_SPI_HAL_LOCK(Handle);

    ret = AlSpi_Dev_RecvData(Handle->Dev, Data, Size);
    if (ret != AL_OK) {
        AL_SPI_HAL_UNLOCK(Handle);
        return ret;
    }

    /* wait until data receive done */
    ret = AlSpi_Hal_WaitRxDoneOrTimeout(Handle, Timeout);
    if (ret != AL_OK) {
        ret = AL_SPI_ERR_TIMEOUT;
        AlSpi_ll_MaskIntr(Handle->Dev->BaseAddr, SPI_RXFIM);
    }

    AL_SPI_HAL_UNLOCK(Handle);

    return ret;
}

/**
 * This function
 * @param   Handle is pointer to AL_SPI_HalStruct
 * @param   SendData is pointer to send data
 * @param   SendSize is send data size
 * @param   RecvData is pointer to receive data
 * @param   RecvSize is receive data size
 * @param   Timeout is max wait time for send done
 * @return  The state of function execution
 * @note    None
*/
AL_S32 AlSpi_Hal_TranferDataBlock(AL_SPI_HalStruct *Handle, AL_U8 *SendData, AL_U32 SendSize,
                                   AL_U8 *RecvData, AL_U16 RecvSize, AL_U32 Timeout)
{
    AL_S32 ret = AL_OK;

    /* check only Handle, more checks in AlSpi_Dev_Init function */
    if (Handle == AL_NULL) {
        return AL_SPI_ERR_ILLEGAL_PARAM;
    }

    AL_SPI_HAL_LOCK(Handle);

    ret = AlSpi_Dev_TranferData(Handle->Dev, SendData, SendSize, RecvData, RecvSize);
    if (ret != AL_OK) {
        AL_SPI_HAL_UNLOCK(Handle);
        return ret;
    }

    /* wait until data done */
    ret = AlSpi_Hal_WaitTxRxDoneOrTimeout(Handle, Timeout);
    if (ret != AL_OK) {
        ret = AL_SPI_ERR_TIMEOUT;
        AlSpi_ll_MaskIntr(Handle->Dev->BaseAddr, SPI_TXEIM | SPI_RXFIM);
    }

    AL_SPI_HAL_UNLOCK(Handle);

    return ret;
}


/*TransSize(AL_DMACAHB_ChTransStruct) * SrcTransWidth = Data Size */
/* Data Size % SrcBurstLength = 0 */
AL_S32 AlSpi_Hal_DmaStartBlockSend(AL_SPI_HalStruct *Handle, AL_U8 *SendData, AL_U32 SendSize, AL_U32 Timeout)
{
    AL_DMACAHB_ChInitStruct     Spi1TxDmacChConfig;
    AL_DMACAHB_HalStruct        Spi1TxDmacHandle;
    AL_DMACAHB_ChTransStruct    *Spi1TxDmacChTrans;

    AL_U32                      DmacDevId = 0;
    AL_S32                      Ret = AL_OK;

    /* check only Handle, more checks in AlSpi_Dev_Init function */
    if (Handle == AL_NULL) {
        return AL_SPI_ERR_ILLEGAL_PARAM;
    }

    AL_SPI_HAL_LOCK(Handle);

    Ret = AlSpi_Dev_DmaSendData(Handle->Dev);
    if (Ret != AL_OK) {
        AL_SPI_HAL_UNLOCK(Handle);
        return Ret;
    }

    Spi1TxDmacChConfig.Id = AL_DMACAHB_CHANNEL_6;
    Spi1TxDmacChConfig.TransType = AL_DMACAHB_TRANS_TYPE_1;
    Spi1TxDmacChConfig.Intr.IsIntrEn = AL_TRUE;
    Spi1TxDmacChConfig.Intr.IntrUnMask = AL_DMACAHB_CH_INTR_TFR | AL_DMACAHB_CH_INTR_ERR;
    Spi1TxDmacChConfig.SrcTransWidth = AL_DMACAHB_TRANS_WIDTH_8;
    Spi1TxDmacChConfig.DstTransWidth = AL_DMACAHB_TRANS_WIDTH_8;
    Spi1TxDmacChConfig.SrcAddrIncMode = AL_DMACAHB_ADDR_INC_INC;
    //Spi1TxDmacChConfig.DstAddrIncMode = AL_DMACAHB_ADDR_INC_INC;
    Spi1TxDmacChConfig.DstAddrIncMode = AL_DMACAHB_ADDR_INC_NO0;
    Spi1TxDmacChConfig.SrcBurstLength = AL_DMACAHB_MSIZE_1;
    Spi1TxDmacChConfig.DstBurstLength = AL_DMACAHB_MSIZE_1;

    Spi1TxDmacChConfig.Direction = AL_DMACAHB_TT_FC_MEM2PER;
    Spi1TxDmacChConfig.HandShaking.DstPer = AL_DMACAHB_PER_SPI1_TX;
    Spi1TxDmacChConfig.HandShaking.DstHsSel = AL_DMACAHB_HAND_SHAKING_HARDWARE;
    Spi1TxDmacChConfig.HandShaking.DstHsPol = AL_DMACAHB_HS_POL_ACTIVE_HI;

    Spi1TxDmacChConfig.SrcMasterSel = AL_DMACAHB_MASTER_SEL_1;
    Spi1TxDmacChConfig.DstMasterSel = AL_DMACAHB_MASTER_SEL_2;
    Spi1TxDmacChConfig.ChPrior = AL_DMACAHB_CH_PRIOR_6;
    Spi1TxDmacChConfig.FifoMode = AL_DMACAHB_FIFO_MODE_0;
    Spi1TxDmacChConfig.ProtCtl = AL_DMACAHB_PROT_0;
    Spi1TxDmacChConfig.SgrDsr.IsSrcGatherEn = AL_FALSE;
    Spi1TxDmacChConfig.SgrDsr.IsDstScatterEn = AL_FALSE;

    Ret = AlDmacAhb_Hal_Init(&Spi1TxDmacHandle, &Spi1TxDmacChConfig, AL_NULL, DmacDevId);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Spi1Tx Dmacahb hal Init error:0x%x\r\n", Ret);
        AL_SPI_HAL_UNLOCK(Handle);
        return Ret;
    }

    Spi1TxDmacChTrans = &(Spi1TxDmacHandle.Channel->Trans);
    Spi1TxDmacChTrans->SrcAddr        = (AL_REG)SendData;
    Spi1TxDmacChTrans->DstAddr        = Handle->Dev->BaseAddr + SPI_DR0_MST_OFFSET;
    Spi1TxDmacChTrans->TransSize      = SendSize;

    Ret = AlDmacAhb_Hal_StartBlock(&Spi1TxDmacHandle, 1000000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Spi1Tx Dmacahb hal Start Block error:0x%x\r\n", Ret);
    }

    AL_SPI_HAL_UNLOCK(Handle);

    return Ret;
}



AL_S32 AlSpi_Hal_DmaStartBlockReceive(AL_SPI_HalStruct *Handle, AL_U8 *RecvData, AL_U16 RecvSize, AL_U32 Timeout)
{
    AL_DMACAHB_ChInitStruct     Spi1RxDmacChConfig;
    AL_DMACAHB_HalStruct        Spi1RxDmacHandle;
    AL_DMACAHB_ChTransStruct    *Spi1RxDmacChTrans;

    AL_U32                      DmacDevId = 0;
    AL_S32                      Ret = AL_OK;

    /* check only Handle, more checks in AlSpi_Dev_Init function */
    if (Handle == AL_NULL) {
        return AL_SPI_ERR_ILLEGAL_PARAM;
    }

    AL_SPI_HAL_LOCK(Handle);

    Ret = AlSpi_Dev_DmaRecvData(Handle->Dev, RecvSize);
    if (Ret != AL_OK) {
        AL_SPI_HAL_UNLOCK(Handle);
        return Ret;
    }

    Spi1RxDmacChConfig.Id = AL_DMACAHB_CHANNEL_7;
    Spi1RxDmacChConfig.TransType = AL_DMACAHB_TRANS_TYPE_1;
    Spi1RxDmacChConfig.Intr.IsIntrEn = AL_TRUE;
    Spi1RxDmacChConfig.Intr.IntrUnMask = AL_DMACAHB_CH_INTR_TFR | AL_DMACAHB_CH_INTR_ERR;
    Spi1RxDmacChConfig.SrcTransWidth = AL_DMACAHB_TRANS_WIDTH_8;
    Spi1RxDmacChConfig.DstTransWidth = AL_DMACAHB_TRANS_WIDTH_8;
    Spi1RxDmacChConfig.SrcAddrIncMode = AL_DMACAHB_ADDR_INC_NO0;
    //Spi1RxDmacChConfig.SrcAddrIncMode = AL_DMACAHB_ADDR_INC_INC;
    Spi1RxDmacChConfig.DstAddrIncMode = AL_DMACAHB_ADDR_INC_INC;
    Spi1RxDmacChConfig.SrcBurstLength = AL_DMACAHB_MSIZE_1;
    Spi1RxDmacChConfig.DstBurstLength = AL_DMACAHB_MSIZE_1;
    Spi1RxDmacChConfig.Direction = AL_DMACAHB_TT_FC_PER2MEM;
    Spi1RxDmacChConfig.HandShaking.SrcPer = AL_DMACAHB_PER_SPI1_RX;
    Spi1RxDmacChConfig.HandShaking.SrcHsSel = AL_DMACAHB_HAND_SHAKING_HARDWARE;
    Spi1RxDmacChConfig.HandShaking.SrcHsPol = AL_DMACAHB_HS_POL_ACTIVE_HI;

    Spi1RxDmacChConfig.SrcMasterSel = AL_DMACAHB_MASTER_SEL_1;
    Spi1RxDmacChConfig.DstMasterSel = AL_DMACAHB_MASTER_SEL_2;
    Spi1RxDmacChConfig.ChPrior = AL_DMACAHB_CH_PRIOR_7;
    Spi1RxDmacChConfig.FifoMode = AL_DMACAHB_FIFO_MODE_0;
    Spi1RxDmacChConfig.ProtCtl = AL_DMACAHB_PROT_0;
    Spi1RxDmacChConfig.SgrDsr.IsSrcGatherEn = AL_FALSE;
    Spi1RxDmacChConfig.SgrDsr.IsDstScatterEn = AL_FALSE;

    Ret = AlDmacAhb_Hal_Init(&Spi1RxDmacHandle, &Spi1RxDmacChConfig, AL_NULL, DmacDevId);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Spi1Rx Dmacahb hal Init error:0x%x\r\n", Ret);
        AL_SPI_HAL_UNLOCK(Handle);
        return Ret;
    }

    Spi1RxDmacChTrans = &(Spi1RxDmacHandle.Channel->Trans);
    Spi1RxDmacChTrans->SrcAddr        = Handle->Dev->BaseAddr + SPI_DR0_MST_OFFSET;
    Spi1RxDmacChTrans->DstAddr        = (AL_REG)RecvData;
    Spi1RxDmacChTrans->TransSize      = RecvSize;

    Ret = AlDmacAhb_Hal_StartBlock(&Spi1RxDmacHandle, 1000000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Spi1Rx Dmacahb hal Start Block error:0x%x\r\n", Ret);
    }

    AL_SPI_HAL_UNLOCK(Handle);

    return Ret;
}




AL_S32 AlSpi_Hal_DmaStartBlockTranfer(AL_SPI_HalStruct *Handle, AL_U8 *SendData, AL_U32 SendSize,
                                       AL_U8 *RecvData, AL_U16 RecvSize, AL_U32 Timeout)
{
    AL_DMACAHB_ChInitStruct     Spi1TxDmacChConfig;
    AL_DMACAHB_HalStruct        Spi1TxDmacHandle;
    AL_DMACAHB_ChTransStruct    *Spi1TxDmacChTrans;

    AL_DMACAHB_ChInitStruct     Spi1RxDmacChConfig;
    AL_DMACAHB_HalStruct        Spi1RxDmacHandle;
    AL_DMACAHB_ChTransStruct    *Spi1RxDmacChTrans;

    AL_U32                      DmacDevId = 0;
    AL_S32                      Ret = AL_OK;

    /* check only Handle, more checks in AlSpi_Dev_Init function */
    if (Handle == AL_NULL) {
        return AL_SPI_ERR_ILLEGAL_PARAM;
    }

    AL_SPI_HAL_LOCK(Handle);

    Ret = AlSpi_Dev_DmaTranferData(Handle->Dev, RecvSize);
    if (Ret != AL_OK) {
        AL_SPI_HAL_UNLOCK(Handle);
        return Ret;
    }

    Spi1TxDmacChConfig.Id = AL_DMACAHB_CHANNEL_6;
    Spi1TxDmacChConfig.TransType = AL_DMACAHB_TRANS_TYPE_1;
    Spi1TxDmacChConfig.Intr.IsIntrEn = AL_TRUE;
    Spi1TxDmacChConfig.Intr.IntrUnMask = AL_DMACAHB_CH_INTR_TFR | AL_DMACAHB_CH_INTR_ERR;
    Spi1TxDmacChConfig.SrcTransWidth = AL_DMACAHB_TRANS_WIDTH_8;
    Spi1TxDmacChConfig.DstTransWidth = AL_DMACAHB_TRANS_WIDTH_8;
    Spi1TxDmacChConfig.SrcAddrIncMode = AL_DMACAHB_ADDR_INC_INC;
    //Spi1TxDmacChConfig.DstAddrIncMode = AL_DMACAHB_ADDR_INC_INC;
    Spi1TxDmacChConfig.DstAddrIncMode = AL_DMACAHB_ADDR_INC_NO0;
    Spi1TxDmacChConfig.SrcBurstLength = AL_DMACAHB_MSIZE_1;
    Spi1TxDmacChConfig.DstBurstLength = AL_DMACAHB_MSIZE_1;

    Spi1TxDmacChConfig.Direction = AL_DMACAHB_TT_FC_MEM2PER;
    Spi1TxDmacChConfig.HandShaking.DstPer = AL_DMACAHB_PER_SPI1_TX;
    Spi1TxDmacChConfig.HandShaking.DstHsSel = AL_DMACAHB_HAND_SHAKING_HARDWARE;
    Spi1TxDmacChConfig.HandShaking.DstHsPol = AL_DMACAHB_HS_POL_ACTIVE_HI;

    Spi1TxDmacChConfig.SrcMasterSel = AL_DMACAHB_MASTER_SEL_1;
    Spi1TxDmacChConfig.DstMasterSel = AL_DMACAHB_MASTER_SEL_2;
    Spi1TxDmacChConfig.ChPrior = AL_DMACAHB_CH_PRIOR_6;
    Spi1TxDmacChConfig.FifoMode = AL_DMACAHB_FIFO_MODE_0;
    Spi1TxDmacChConfig.ProtCtl = AL_DMACAHB_PROT_0;
    Spi1TxDmacChConfig.SgrDsr.IsSrcGatherEn = AL_FALSE;
    Spi1TxDmacChConfig.SgrDsr.IsDstScatterEn = AL_FALSE;

    Spi1RxDmacChConfig.Id = AL_DMACAHB_CHANNEL_7;
    Spi1RxDmacChConfig.TransType = AL_DMACAHB_TRANS_TYPE_1;
    Spi1RxDmacChConfig.Intr.IsIntrEn = AL_TRUE;
    Spi1RxDmacChConfig.Intr.IntrUnMask = AL_DMACAHB_CH_INTR_TFR | AL_DMACAHB_CH_INTR_ERR;
    Spi1RxDmacChConfig.SrcTransWidth = AL_DMACAHB_TRANS_WIDTH_8;
    Spi1RxDmacChConfig.DstTransWidth = AL_DMACAHB_TRANS_WIDTH_8;
    Spi1RxDmacChConfig.SrcAddrIncMode = AL_DMACAHB_ADDR_INC_NO0;
    //Spi1RxDmacChConfig.SrcAddrIncMode = AL_DMACAHB_ADDR_INC_INC;
    Spi1RxDmacChConfig.DstAddrIncMode = AL_DMACAHB_ADDR_INC_INC;
    Spi1RxDmacChConfig.SrcBurstLength = AL_DMACAHB_MSIZE_1;
    Spi1RxDmacChConfig.DstBurstLength = AL_DMACAHB_MSIZE_1;
    Spi1RxDmacChConfig.Direction = AL_DMACAHB_TT_FC_PER2MEM;
    Spi1RxDmacChConfig.HandShaking.SrcPer = AL_DMACAHB_PER_SPI1_RX;
    Spi1RxDmacChConfig.HandShaking.SrcHsSel = AL_DMACAHB_HAND_SHAKING_HARDWARE;
    Spi1RxDmacChConfig.HandShaking.SrcHsPol = AL_DMACAHB_HS_POL_ACTIVE_HI;

    Spi1RxDmacChConfig.SrcMasterSel = AL_DMACAHB_MASTER_SEL_1;
    Spi1RxDmacChConfig.DstMasterSel = AL_DMACAHB_MASTER_SEL_2;
    Spi1RxDmacChConfig.ChPrior = AL_DMACAHB_CH_PRIOR_7;
    Spi1RxDmacChConfig.FifoMode = AL_DMACAHB_FIFO_MODE_0;
    Spi1RxDmacChConfig.ProtCtl = AL_DMACAHB_PROT_0;
    Spi1RxDmacChConfig.SgrDsr.IsSrcGatherEn = AL_FALSE;
    Spi1RxDmacChConfig.SgrDsr.IsDstScatterEn = AL_FALSE;

    Ret = AlDmacAhb_Hal_Init(&Spi1TxDmacHandle, &Spi1TxDmacChConfig, AL_NULL, DmacDevId);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Spi1Tx Dmacahb hal Init error:0x%x\r\n", Ret);
        AL_SPI_HAL_UNLOCK(Handle);
        return Ret;
    }

    Ret = AlDmacAhb_Hal_Init(&Spi1RxDmacHandle, &Spi1RxDmacChConfig, AL_NULL, DmacDevId);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Spi1Rx Dmacahb hal Init error:0x%x\r\n", Ret);
        AL_SPI_HAL_UNLOCK(Handle);
        return Ret;
    }

    Spi1TxDmacChTrans = &(Spi1TxDmacHandle.Channel->Trans);
    Spi1TxDmacChTrans->SrcAddr        = (AL_REG)SendData;
    Spi1TxDmacChTrans->DstAddr        = Handle->Dev->BaseAddr + SPI_DR0_MST_OFFSET;
    Spi1TxDmacChTrans->TransSize      = SendSize;

    Spi1RxDmacChTrans = &(Spi1RxDmacHandle.Channel->Trans);
    Spi1RxDmacChTrans->SrcAddr        = Handle->Dev->BaseAddr + SPI_DR0_MST_OFFSET;
    Spi1RxDmacChTrans->DstAddr        = (AL_REG)RecvData;
    Spi1RxDmacChTrans->TransSize      = RecvSize;

    Ret = AlDmacAhb_Hal_Start(&Spi1RxDmacHandle);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Spi1Rx Dmacahb hal Start error:0x%x\r\n", Ret);
        AL_SPI_HAL_UNLOCK(Handle);
        return Ret;
    }

    Ret = AlDmacAhb_Hal_StartBlock(&Spi1TxDmacHandle, 1000000);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Spi1Tx Dmacahb hal Start Block error:0x%x\r\n", Ret);
    }

    AL_SPI_HAL_UNLOCK(Handle);

    return Ret;
}



/**
 * This function is excute operations to set or check spi configuration status.
 * @param   Handle is pointer to AL_SPI_HalStruct
 * @return  The state of function execution
 * @note    None
*/
AL_S32 AlSpi_Hal_IoCtl(AL_SPI_HalStruct *Handle, AL_Spi_IoCtlCmdEnum Cmd, AL_VOID *Data)
{
    AL_S32 Ret = AL_OK;

    if (Handle == AL_NULL) {
        return AL_SPI_ERR_ILLEGAL_PARAM;
    }

    AL_SPI_HAL_LOCK(Handle);

    Ret = AlSpi_Dev_IoCtl(Handle->Dev, Cmd, Data);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Spi io ctl cmd error:%d\r\n", Ret);
    }

    AL_SPI_HAL_LOCK(Handle);

    return Ret;
}


