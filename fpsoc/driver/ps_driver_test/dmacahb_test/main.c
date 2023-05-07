#include "al_dmacahb_hal.h"
#include "al_errno.h"
#include "mtimer.h"

#define AL_DMACAHB_TEST_MEM_SIZE            7
#define AL_DMACAHB_TEST_ARRAY_SIZE          256
#define AL_DMACAHB_AUTO_RELOAD_COUNT_MAX    3
#define AL_DMACAHB_LLI_STRUCT_NUM           5

static AL_DMACAHB_ChInitStruct ChSingleModeExample1 = {
    .Id                     = AL_DMACAHB_CHANNEL_0,
    .TransType              = AL_DMACAHB_TRANS_TYPE_1,
    .Intr.IsIntrEn          = AL_TRUE,
    .Intr.IntrUnMask        = AL_DMACAHB_CH_INTR_TFR | AL_DMACAHB_CH_INTR_ERR,
    .SrcTransWidth          = AL_DMACAHB_TRANS_WIDTH_8,
    .DstTransWidth          = AL_DMACAHB_TRANS_WIDTH_8,
    .SrcAddrIncMode         = AL_DMACAHB_ADDR_INC_INC,
    .DstAddrIncMode         = AL_DMACAHB_ADDR_INC_INC,
    .SrcBurstLength         = AL_DMACAHB_MSIZE_1,
    .DstBurstLength         = AL_DMACAHB_MSIZE_1,
    .Direction              = AL_DMACAHB_TT_FC_MEM2MEM,
    .SrcMasterSel           = AL_DMACAHB_MASTER_SEL_1,
    .DstMasterSel           = AL_DMACAHB_MASTER_SEL_1,
    .ChPrior                = AL_DMACAHB_CH_PRIOR_7,
    .FifoMode               = AL_DMACAHB_FIFO_MODE_0,
    .ProtCtl                = AL_DMACAHB_PROT_0,
    .SgrDsr.IsSrcGatherEn   = AL_FALSE,
    .SgrDsr.IsDstScatterEn  = AL_FALSE,
};

static AL_DMACAHB_ChInitStruct ChAutoReloadModeExample1 = {
    .Id                     = AL_DMACAHB_CHANNEL_1,
    .TransType              = AL_DMACAHB_TRANS_TYPE_4,
    .Intr.IsIntrEn          = AL_TRUE,
    .Intr.IntrUnMask        = AL_DMACAHB_CH_INTR_BLOCK | AL_DMACAHB_CH_INTR_ERR,
    .SrcTransWidth          = AL_DMACAHB_TRANS_WIDTH_16,
    .DstTransWidth          = AL_DMACAHB_TRANS_WIDTH_16,
    .SrcAddrIncMode         = AL_DMACAHB_ADDR_INC_INC,
    .DstAddrIncMode         = AL_DMACAHB_ADDR_INC_INC,
    .SrcBurstLength         = AL_DMACAHB_MSIZE_4,
    .DstBurstLength         = AL_DMACAHB_MSIZE_4,
    .Direction              = AL_DMACAHB_TT_FC_MEM2MEM,
    .SrcMasterSel           = AL_DMACAHB_MASTER_SEL_2,
    .DstMasterSel           = AL_DMACAHB_MASTER_SEL_2,
    .ChPrior                = AL_DMACAHB_CH_PRIOR_6,
    .FifoMode               = AL_DMACAHB_FIFO_MODE_0,
    .ProtCtl                = AL_DMACAHB_PROT_0,
    .SgrDsr.IsSrcGatherEn   = AL_FALSE,
    .SgrDsr.IsDstScatterEn  = AL_FALSE,
};

static AL_DMACAHB_ChInitStruct ChLlpModeExample1 = {
    .Id                     = AL_DMACAHB_CHANNEL_2,
    .TransType              = AL_DMACAHB_TRANS_TYPE_10,
    .Intr.IsIntrEn          = AL_TRUE,
    .Intr.IntrUnMask        = AL_DMACAHB_CH_INTR_BLOCK | AL_DMACAHB_CH_INTR_ERR,
    .SrcTransWidth          = AL_DMACAHB_TRANS_WIDTH_32,
    .DstTransWidth          = AL_DMACAHB_TRANS_WIDTH_32,
    .SrcAddrIncMode         = AL_DMACAHB_ADDR_INC_INC,
    .DstAddrIncMode         = AL_DMACAHB_ADDR_INC_INC,
    .SrcBurstLength         = AL_DMACAHB_MSIZE_8,
    .DstBurstLength         = AL_DMACAHB_MSIZE_8,
    .Direction              = AL_DMACAHB_TT_FC_MEM2MEM,
    .SrcMasterSel           = AL_DMACAHB_MASTER_SEL_3,
    .DstMasterSel           = AL_DMACAHB_MASTER_SEL_3,
    .ChPrior                = AL_DMACAHB_CH_PRIOR_5,
    .FifoMode               = AL_DMACAHB_FIFO_MODE_0,
    .ProtCtl                = AL_DMACAHB_PROT_0,
    .SgrDsr.IsSrcGatherEn   = AL_FALSE,
    .SgrDsr.IsDstScatterEn  = AL_FALSE,
};

AL_U8 __attribute__((aligned(64))) SRC_MEM[AL_DMACAHB_TEST_MEM_SIZE][AL_DMACAHB_TEST_ARRAY_SIZE];
AL_U8 __attribute__((aligned(64))) DST_MEM[AL_DMACAHB_TEST_MEM_SIZE][AL_DMACAHB_TEST_ARRAY_SIZE];

static AL_VOID AlDmacAhb_Test_InitSrc(AL_REG Addr, AL_U32 ByteSize, AL_U32 DataOffset);
static AL_S32 AlDmacAhb_Test_DataCheck(AL_REG SrcAddr, AL_REG DstAddr, AL_U32 ByteSize);
static AL_VOID AlDmacAhb_Test_AutoReloadExp1CallBack(AL_DMACAHB_EventStruct *Event, AL_VOID *CallBackRef);
static AL_S32 AlDmacAhb_Test_FillLliStruct(AL_DMACAHB_HalStruct *Handle, AL_DMACAHB_LliStruct *Lli, AL_U32 SrcAddr,
                                           AL_U32 DstAddr, AL_DMACAHB_LliStruct *Next);

AL_U32 main(AL_VOID)
{
    AL_DMACAHB_HalStruct        Handle;
    AL_DMACAHB_ChInitStruct     ChConfig;
    AL_DMACAHB_ChCallBackStruct CallBack;
    AL_U32 DeviceId = 0;
    AL_U32 Ret = AL_OK;
    AL_U32 Timeout = 1000;

#if CONFIG_SINGLE_MODE_TEST
    AL_LOG(AL_ERR_LEVEL_INFO, "Dmacahb single mode test!\r\n");

    ChConfig = ChSingleModeExample1;

    Ret = AlDmacAhb_Hal_Init(&Handle, &ChConfig, AL_NULL, DeviceId);
    if (Ret != AL_OK) {
        AL_LOG(AL_ERR_LEVEL_ERROR, "Hal Init error:0x%x\r\n", Ret);
    }

    AL_U32  SrcAddr, DstAddr, TransSize;
    SrcAddr = (AL_REG)SRC_MEM[0];
    DstAddr = (AL_REG)DST_MEM[0];
    TransSize = AL_DMACAHB_TEST_ARRAY_SIZE / (1 << ChConfig.SrcTransWidth);

    //init SRC_MEM
    AlDmacAhb_Test_InitSrc(SrcAddr, TransSize * ChConfig.SrcTransWidth, 'A');

    Ret = AlDmacAhb_Hal_StartBlock(&Handle, SrcAddr, DstAddr, TransSize, Timeout);
    if (Ret != AL_OK) {
        AL_LOG(AL_ERR_LEVEL_ERROR, "Trans error:0x%x\r\n", Ret);
    }

    Ret = AlDmacAhb_Test_DataCheck(SrcAddr, DstAddr, TransSize * (1 << ChConfig.SrcTransWidth));
    if (Ret != AL_OK) {
        AL_LOG(AL_ERR_LEVEL_ERROR, "Data check error:0x%x\r\n", Ret);
    }

    Ret = AlDmacAhb_Hal_DeInit(&Handle);
    if (Ret != AL_OK) {
        AL_LOG(AL_ERR_LEVEL_ERROR, "Deinit error:0x%x\r\n", Ret);
    }

    AL_LOG(AL_ERR_LEVEL_INFO, "Dmacahb single mode test done!\r\n");
#endif
    
#if CONFIG_AUTO_RELOAD_MODE_TEST
    AL_LOG(AL_ERR_LEVEL_INFO, "Dmacahb auto-reload mode test!\r\n");

    ChConfig = ChAutoReloadModeExample1;

    CallBack.Func = AlDmacAhb_Test_AutoReloadExp1CallBack;
    CallBack.Ref = &Handle;

    Ret = AlDmacAhb_Hal_Init(&Handle, &ChConfig, &CallBack, DeviceId);
    if (Ret != AL_OK) {
        AL_LOG(AL_ERR_LEVEL_ERROR, "Hal Init error:0x%x\r\n", Ret);
    }

    AL_U32  SrcAddr, DstAddr, TransSize;
    SrcAddr = (AL_REG)SRC_MEM[1];
    DstAddr = (AL_REG)DST_MEM[1];
    TransSize = AL_DMACAHB_TEST_ARRAY_SIZE / (1 << ChConfig.SrcTransWidth);

    //init SRC_MEM
    AlDmacAhb_Test_InitSrc(SrcAddr, TransSize * ChConfig.SrcTransWidth, 'a');

    Ret = AlDmacAhb_Hal_StartBlock(&Handle, SrcAddr, DstAddr, TransSize, Timeout);
    if (Ret != AL_OK) {
        AL_LOG(AL_ERR_LEVEL_ERROR, "Trans error:0x%x\r\n", Ret);
    }

    Ret = AlDmacAhb_Test_DataCheck(SrcAddr, DstAddr, TransSize * (1 << ChConfig.SrcTransWidth));
    if (Ret != AL_OK) {
        AL_LOG(AL_ERR_LEVEL_ERROR, "Data check error:0x%x\r\n", Ret);
    }

    Ret = AlDmacAhb_Hal_DeInit(&Handle);
    if (Ret != AL_OK) {
        AL_LOG(AL_ERR_LEVEL_ERROR, "Deinit error:0x%x\r\n", Ret);
    }

    AL_LOG(AL_ERR_LEVEL_INFO, "Dmacahb auto-reload mode test done!\r\n");
#endif

#if CONFIG_LLP_MODE_TEST
    AL_LOG(AL_ERR_LEVEL_INFO, "Dmacahb LLP mode test!\r\n");

    ChConfig = ChLlpModeExample1;

    Ret = AlDmacAhb_Hal_Init(&Handle, &ChConfig, AL_NULL, DeviceId);
    if (Ret != AL_OK) {
        AL_LOG(AL_ERR_LEVEL_ERROR, "Hal Init error:0x%x\r\n", Ret);
    }

    AL_U32 TransSize = AL_DMACAHB_TEST_ARRAY_SIZE / (1 << ChConfig.SrcTransWidth);

    AL_DMACAHB_LliStruct Lli[AL_DMACAHB_LLI_STRUCT_NUM];
    for (AL_U32 i = 0; i < AL_DMACAHB_LLI_STRUCT_NUM; i++) {
        Lli[i].SrcAddr = SRC_MEM[i];
        Lli[i].DstAddr = DST_MEM[i];
        if ((i + 1) < AL_DMACAHB_LLI_STRUCT_NUM) {
            Lli[i].LlpNext = &Lli[i + 1];
        } else {
            Lli[i].LlpNext = AL_NULL;
        }
        AlDmacAhb_Dev_FillLliWithCtl(Handle.Channel, &Lli[i]);
        Lli[i].CtlHigh.Bit.BlockTransSize = TransSize;

        AlDmacAhb_Test_InitSrc(Lli[i].SrcAddr, TransSize * ChConfig.SrcTransWidth, 'a' + i);
    }

    Ret = AlDmacAhb_Hal_LlpModeStartBlock(&Handle, Lli, Timeout);
    if (Ret != AL_OK) {
        AL_LOG(AL_ERR_LEVEL_ERROR, "Trans error:0x%x\r\n", Ret);
    }

    for (AL_U32 i = 0; i < AL_DMACAHB_LLI_STRUCT_NUM; i++) {
        Ret = AlDmacAhb_Test_DataCheck(Lli[i].SrcAddr, Lli[i].DstAddr, TransSize * (1 << ChConfig.SrcTransWidth));
        if (Ret != AL_OK) {
            AL_LOG(AL_ERR_LEVEL_ERROR, "Llp[%d] data check error:0x%x\r\n", i, Ret);
            break;
        }
    }

    Ret = AlDmacAhb_Hal_DeInit(&Handle);
    if (Ret != AL_OK) {
        AL_LOG(AL_ERR_LEVEL_ERROR, "Deinit error:0x%x\r\n", Ret);
    }

    AL_LOG(AL_ERR_LEVEL_INFO, "Dmacahb LLP mode test done!\r\n");
#endif

    while (1);

    return Ret;
}

static AL_VOID AlDmacAhb_Test_InitSrc(AL_REG Addr, AL_U32 ByteSize, AL_U32 DataOffset)
{
    AL_U8 *Char = (AL_U8 *)Addr;
    for (AL_U32 i = 0; i < ByteSize; i++) {
        Char[i] = i + DataOffset;
    }
}

static AL_S32 AlDmacAhb_Test_DataCheck(AL_REG SrcAddr, AL_REG DstAddr, AL_U32 ByteSize)
{
    AL_U8 *SrcData = (AL_U8 *)SrcAddr;
    AL_U8 *DstData = (AL_U8 *)DstAddr;
    while (ByteSize--) {
        if (*SrcData == *DstData) {
            SrcData++;
            DstData++;
            continue;
        } else {
            return -1;
        }
    }
    
    return AL_OK;
}

static AL_VOID AlDmacAhb_Test_AutoReloadExp1CallBack(AL_DMACAHB_EventStruct *Event, AL_VOID *CallBackRef)
{
    AL_DMACAHB_HalStruct *Handle = (AL_DMACAHB_HalStruct *)CallBackRef;
    static AL_U32 Count = 0;

    if (Count == AL_DMACAHB_AUTO_RELOAD_COUNT_MAX) {
        AL_BOOL IsChEn = AL_FALSE;
        AlDmacAhb_Hal_IoCtl(Handle, AL_DMACAHB_IOCTL_SET_CHANNEL_EN, &IsChEn);
    }

    switch (Event->EventId)
    {
    case AL_DMACAHB_EVENT_BLOCK_TRANS_COMP:
        AL_REG SrcAddr          = Handle->Channel->Config.SrcAddr;
        AL_U32 SrcTransWidth    = Handle->Channel->Config.SrcTransWidth;
        AL_U32 TransSize        = Handle->Channel->Config.BlockTransSize;

        AlDmacAhb_Test_InitSrc(SrcAddr, TransSize * (1 << SrcTransWidth), Count + 'a');
        Count++;
        break;
    case AL_DMACAHB_EVENT_ERR:
        AL_LOG(AL_ERR_LEVEL_ERROR, "Dmac ahb auto reload call back err!\r\n");
        break;
    default :
        break;
    }
    
}

static AL_S32 AlDmacAhb_Test_FillLliStruct(AL_DMACAHB_HalStruct *Handle, AL_DMACAHB_LliStruct *Lli, AL_U32 SrcAddr,
                                           AL_U32 DstAddr, AL_DMACAHB_LliStruct *Next)
{
    if (Lli == AL_NULL || Handle == AL_NULL) {
        return -1;
    }

    Lli->SrcAddr = SrcAddr;
    Lli->DstAddr = DstAddr;
    Lli->LlpNext = Next;
    AlDmacAhb_Dev_FillLliWithCtl(Handle->Channel, Lli);

    return AL_OK;
}