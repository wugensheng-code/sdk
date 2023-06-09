/***************************** Include Files *********************************/
#include "al_spi_hal.h"
#include "al_intr.h"
/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Variable Definitions *****************************/
static AL_SPI_DevStruct AL_SPI_DevInstance[AL_SPI_NUM_INSTANCE];

/********************************************************/
#ifdef USE_RTOS
static AL_S32 AlSpi_Hal_WaitTxDoneOrTimeout(AL_SPI_HalStruct *Handle, AL_U32 Timeout)
{
    /*
     * TODO: wait for event timeout
    */
    (void) Handle;
    (void) Timeout;
}

static AL_S32 AlSpi_Hal_WaitRxDoneOrTimeout(AL_SPI_HalStruct *Handle, AL_U32 Timeout)
{
    /*
     * TODO:wait for event timeout
    */
    (void) Handle;
    (void) Timeout;
}

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

static AL_S32 AlSpi_Hal_WaitTxDoneOrTimeout(AL_SPI_HalStruct *Handle, AL_U32 Timeout)
{
    extern AL_BOOL AlSpi_Dev_IsTxBusy(AL_SPI_DevStruct *Spi);
    
    // AlSpi_Dev_DumpReg(Handle->Dev->BaseAddr);
    // printf("AlSpi_Dev_IsTxBusy:%d\r\n", AlSpi_Dev_IsTxBusy(Handle->Dev));

    while(AlSpi_Dev_IsTxBusy(Handle->Dev));
    while(SPI_SR_BUSY == AlSpi_ll_IsBusy(Handle->Dev->BaseAddr));

    AlSpi_ll_SetSlvSel(Handle->Dev->BaseAddr, 0);
  //  AlSpi_ll_Disable(Handle->Dev->BaseAddr);

    return AL_OK;
}

static AL_S32 AlSpi_Hal_WaitRxDoneOrTimeout(AL_SPI_HalStruct *Handle, AL_U32 Timeout)
{
    extern AL_BOOL AlSpi_Dev_IsRxBusy(AL_SPI_DevStruct *Spi);

    while(AlSpi_Dev_IsRxBusy(Handle->Dev));
    while(SPI_SR_BUSY == AlSpi_ll_IsBusy(Handle->Dev->BaseAddr));

    AlSpi_ll_SetSlvSel(Handle->Dev->BaseAddr, 0);
   // AlSpi_ll_Disable(Handle->Dev->BaseAddr);

    return AL_OK;
}

static AL_S32 AlSpi_Hal_WaitTxRxDoneOrTimeout(AL_SPI_HalStruct *Handle, AL_U32 Timeout)
{
    extern AL_BOOL AlSpi_Dev_IsRxBusy(AL_SPI_DevStruct *Spi);
    extern AL_BOOL AlSpi_Dev_IsTxBusy(AL_SPI_DevStruct *Spi);
// AlSpi_Dev_DumpReg(Handle->Dev->BaseAddr);
//         printf("AlSpi_Dev_IsRxBusy:%d\r\n", AlSpi_Dev_IsRxBusy(Handle->Dev));
//         printf("AlSpi_Dev_IsTxBusy:%d\r\n", AlSpi_Dev_IsTxBusy(Handle->Dev));
    while(AlSpi_Dev_IsRxBusy(Handle->Dev) || AlSpi_Dev_IsTxBusy(Handle->Dev));
    while(SPI_SR_BUSY == AlSpi_ll_IsBusy(Handle->Dev->BaseAddr));

    AlSpi_ll_SetSlvSel(Handle->Dev->BaseAddr, 0);
   // AlSpi_ll_Disable(Handle->Dev->BaseAddr);

    return AL_OK;
}



#define AL_SPI_HAL_LOCK(Handle)          do {} while (0)

#define AL_SPI_HAL_UNLOCK(Handle)        do {} while (0)

#endif

static AL_S32 AlSpi_DefEventCallBack(AL_SPI_EventStruct SpiEvent, void *CallbackRef)
{
    switch (SpiEvent.Event) {
    case AL_SPI_SEND_DONE:
#ifdef SPI_DEBUG
      //  printf("AlSpi_DefEventCallBack AL_SPI_SEND_DONE\r\n");
#endif
        break;
    case AL_SPI_RECEIVE_DONE:
#ifdef SPI_DEBUG
     //   printf("AlSpi_DefEventCallBack AL_SPI_RECEIVE_DONE\r\n");
#endif
        break;
    case AL_SPI_SEND_TIMEOUT:
#ifdef SPI_DEBUG
        printf("AlSpi_DefEventCallBack AL_SPI_SEND_TIMEOUT\r\n");
#endif
        break;
    case AL_SPI_RECEIVE_TIMEOUT:
#ifdef SPI_DEBUG
        printf("AlSpi_DefEventCallBack AL_SPI_RECEIVE_TIMEOUT\r\n");
#endif
        break;
    default:
#ifdef SPI_DEBUG
        printf("AlSpi_DefEventCallBack default\r\n");
#endif
        break;
    }

    return AL_OK;
}



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

    if(AL_NULL == Callback) {
        ret = AlSpi_Dev_RegisterIntrCallBack(Handle->Dev, AlSpi_DefEventCallBack, AL_NULL);
    } else {
        ret = AlSpi_Dev_RegisterIntrCallBack(Handle->Dev, Callback, CallbackRef);
    }

    if (ret != AL_OK) {
        AL_SPI_HAL_UNLOCK(Handle);
        return ret;
    }

    ret = AlSpi_Dev_Init(Handle->Dev, InitConfig);
    if (ret != AL_OK) {
        AL_SPI_HAL_UNLOCK(Handle);
        return ret;
    }

    AlIntr_RegHandler(CfgPtr->InterrupId + 13, AL_NULL, AlSpi_Dev_IntrHandler, Handle->Dev);
    AL_SPI_HAL_UNLOCK(Handle);

    return ret;
}


AL_S32 AlSpi_Hal_SendDataBlock(AL_SPI_HalStruct *Handle, AL_U8 *Data, AL_U32 Size, AL_U32 Timeout)
{
    AL_S32 ret = AL_OK;

    /*
     * check only Handle, more checks in AlSpi_Dev_Init function;
    */
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
// printf("AlSpi_Dev_TranferData finish\r\n");
// printf("AlSpi_Dev_TranferData %d %d %d\r\n", *RecvData, *(RecvData + 1), *(RecvData + 2));
    /* wait until data done */
    ret = AlSpi_Hal_WaitTxRxDoneOrTimeout(Handle, Timeout);
    if (ret != AL_OK) {
        ret = AL_SPI_ERR_TIMEOUT;
        AlSpi_ll_MaskIntr(Handle->Dev->BaseAddr, SPI_TXEIM | SPI_RXFIM);
    }

    AL_SPI_HAL_UNLOCK(Handle);

    return ret;
}

AL_S32 AlSpi_Hal_IoCtl(AL_SPI_HalStruct *Handle, AL_Spi_IoCtlCmdEnum Cmd, AL_VOID *Data)
{
    AL_S32 Ret = AL_OK;

    if (Handle == AL_NULL) {
        return AL_SPI_ERR_ILLEGAL_PARAM;
    }

    AL_SPI_HAL_LOCK(Handle);

    Ret = AlSpi_Dev_IoCtl(Handle->Dev, Cmd, Data);
    if (Ret != AL_OK) {
        AL_LOG(AL_ERR_LEVEL_ERROR, "Spi io ctl cmd error:%d\r\n", Ret);
    }

    AL_SPI_HAL_LOCK(Handle);

    return Ret;
}


