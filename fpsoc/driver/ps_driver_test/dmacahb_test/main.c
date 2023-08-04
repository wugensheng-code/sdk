#include "al_dmacahb_test_config.h"
#include "al_errno.h"
#include "string.h"

AL_U8 __attribute__((aligned(64))) SRC_MEM[AL_DMACAHB_TEST_MEM_SIZE][AL_DMACAHB_TEST_ARRAY_SIZE];
AL_U8 __attribute__((aligned(64))) DST_MEM[AL_DMACAHB_TEST_MEM_SIZE][AL_DMACAHB_TEST_ARRAY_SIZE];

static AL_VOID AlDmacAhb_Test_InitSrc(AL_REG Addr, AL_U32 ByteSize, AL_U32 DataOffset);
static AL_S32 AlDmacAhb_Test_DataCheck(AL_REG SrcAddr, AL_REG DstAddr, AL_U32 ByteSize);
static AL_VOID AlDmacAhb_Test_OtherTransTypeEgCallBack(AL_DMACAHB_EventStruct *Event, AL_VOID *CallBackRef);
static AL_VOID AlDmacAhb_Test_SingleMode(AL_VOID);
static AL_VOID AlDmacAhb_Test_AutoReloadMode(AL_VOID);
static AL_VOID AlDmacAhb_Test_LlpMode(AL_VOID);
static AL_VOID AlDmacAhb_Test_OtherTransType(AL_VOID);

AL_U32 main(AL_VOID)
{
#if CONFIG_SINGLE_MODE_TEST
    AlDmacAhb_Test_SingleMode();
#endif

#if CONFIG_RELOAD_MODE_TEST
    AlDmacAhb_Test_AutoReloadMode();
#endif

#if CONFIG_LLP_MODE_TEST
    AlDmacAhb_Test_LlpMode();
#endif

#if CONFIG_OTHER_TRANS_TYPE_TEST
    AlDmacAhb_Test_OtherTransType();
#endif

    while (1);

    return 0;
}

static AL_VOID AlDmacAhb_Test_InitSrc(AL_REG Addr, AL_U32 ByteSize, AL_U32 DataOffset)
{
    AL_U8 *Char = (AL_U8 *)Addr;
    for (AL_U32 i = 0; i < ByteSize; i++) {
        *Char++ = i + DataOffset;
    }
}

static AL_S32 AlDmacAhb_Test_DataCheck(AL_REG SrcAddr, AL_REG DstAddr, AL_U32 ByteSize)
{
    AL_U32 *SrcData = (AL_U32 *)SrcAddr;
    AL_U32 *DstData = (AL_U32 *)DstAddr;
    AL_U32 WordSize = ByteSize >> 2;
    while (WordSize--) {
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

static AL_VOID AlDmacAhb_Test_OtherTransTypeEgCallBack(AL_DMACAHB_EventStruct *Event, AL_VOID *CallBackRef)
{
    AL_DMACAHB_HalStruct *Handle = (AL_DMACAHB_HalStruct *)CallBackRef;
    AL_DMACAHB_TransTypeEnum Type = Handle->Channel->Config.TransType;
    static AL_U32 Type2Count = 0;
    static AL_U32 Type3Count = 0;
    AL_BOOL IsLastTransSet;

    switch (Event->EventId)
    {
    case AL_DMACAHB_EVENT_BLOCK_TRANS_COMP:
        if (Type == AL_DMACAHB_TRANS_TYPE_2) {
            if (Type2Count == AL_DMACAHB_RELOAD_COUNT_MAX) {
                IsLastTransSet = AL_TRUE;
                AlDmacAhb_Dev_IoCtl(Handle->Channel, AL_DMACAHB_IOCTL_SET_RELOAD_LAST_TRANS, &IsLastTransSet);
            } else {
                Type2Count++;
            }
        } else if (Type == AL_DMACAHB_TRANS_TYPE_3) {
            if (Type3Count == AL_DMACAHB_RELOAD_COUNT_MAX) {
                IsLastTransSet = AL_TRUE;
                AlDmacAhb_Dev_IoCtl(Handle->Channel, AL_DMACAHB_IOCTL_SET_RELOAD_LAST_TRANS, &IsLastTransSet);
            } else {
                Type3Count++;
            }
        }
        break;
    case AL_DMACAHB_EVENT_ERR:
        AL_LOG(AL_LOG_LEVEL_ERROR, "Dmac ahb auto reload call back err!\r\n");
        break;
    default :
        break;
    }

}

static AL_VOID AlDmacAhb_Test_SingleMode(AL_VOID)
{
    AL_DMACAHB_HalStruct        Handle;
    AL_DMACAHB_ChInitStruct     ChConfig;
    AL_DMACAHB_ChTransStruct    *Trans;
    AL_U32 DeviceId = 0;
    AL_U32 Ret = AL_OK;
    AL_U32 Timeout = 100000000;

    AL_LOG(AL_LOG_LEVEL_INFO, "Dmacahb single mode test!\r\n");

    ChConfig = ChSingleModeEg1;

    Ret = AlDmacAhb_Hal_Init(&Handle, &ChConfig, AL_NULL, DeviceId);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hal Init error:0x%x\r\n", Ret);
    }
    AlIntr_SetGlobalInterrupt(AL_FUNC_ENABLE);

    Trans = &(Handle.Channel->Trans);
    Trans->SrcAddr = (AL_REG)SRC_MEM[0];
    Trans->DstAddr = (AL_REG)DST_MEM[0];
    Trans->TransSize = AL_DMACAHB_TEST_ARRAY_SIZE / (1 << ChConfig.SrcTransWidth);

    //init SRC_MEM
    AlDmacAhb_Test_InitSrc(Trans->SrcAddr, Trans->TransSize * ChConfig.SrcTransWidth, 'A');

    Ret = AlDmacAhb_Hal_StartBlock(&Handle, Timeout);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Trans error:0x%x\r\n", Ret);
    }

    Ret = AlDmacAhb_Test_DataCheck(Trans->SrcAddr, Trans->DstAddr, AL_DMACAHB_TEST_ARRAY_SIZE);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Data check error:0x%x\r\n", Ret);
    }

    Ret = AlDmacAhb_Hal_DeInit(&Handle);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Deinit error:0x%x\r\n", Ret);
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "Dmacahb single mode test done!\r\n");
}

static AL_VOID AlDmacAhb_Test_AutoReloadMode(AL_VOID)
{
    AL_DMACAHB_HalStruct        Handle;
    AL_DMACAHB_ChInitStruct     ChConfig;
    // AL_DMACAHB_ChCallBackStruct CallBack;
    AL_DMACAHB_ChTransStruct    *Trans;
    AL_U32 DeviceId = 0;
    AL_U32 Ret = AL_OK;
    AL_U32 Timeout = 10000000;

    AL_LOG(AL_LOG_LEVEL_INFO, "Dmacahb auto-reload mode test!\r\n");

    ChConfig = ChAutoReloadModeEg1;

    // CallBack.Func = AlDmacAhb_Test_AutoReloadEg1CallBack;
    // CallBack.Ref = &Handle;

    Ret = AlDmacAhb_Hal_Init(&Handle, &ChConfig, AL_NULL, DeviceId);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hal Init error:0x%x\r\n", Ret);
    }
    AlIntr_SetGlobalInterrupt(AL_FUNC_ENABLE);

    Trans = &(Handle.Channel->Trans);
    Trans->SrcAddr    = (AL_REG)SRC_MEM[1];
    Trans->DstAddr    = (AL_REG)DST_MEM[1];
    Trans->TransSize  = AL_DMACAHB_TEST_ARRAY_SIZE / (1 << ChConfig.SrcTransWidth);
    Trans->ReloadCountNum   = AL_DMACAHB_RELOAD_COUNT_MAX;
    Trans->ReloadCount      = 0;

    //init SRC_MEM
    AlDmacAhb_Test_InitSrc(Trans->SrcAddr, Trans->TransSize * ChConfig.SrcTransWidth, 'a');

    Ret = AlDmacAhb_Hal_StartBlock(&Handle, Timeout);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Trans error:0x%x\r\n", Ret);
    }

    Ret = AlDmacAhb_Test_DataCheck(Trans->SrcAddr, Trans->DstAddr, AL_DMACAHB_TEST_ARRAY_SIZE);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Data check error:0x%x\r\n", Ret);
    }

    Ret = AlDmacAhb_Hal_DeInit(&Handle);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Deinit error:0x%x\r\n", Ret);
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "Dmacahb auto-reload mode test done!\r\n");
}

static AL_VOID AlDmacAhb_Test_LlpMode(AL_VOID)
{
    AL_DMACAHB_HalStruct        Handle;
    AL_DMACAHB_ChInitStruct     ChConfig;
    AL_DMACAHB_ChTransStruct     *Trans;
    AL_U32 DeviceId = 0;
    AL_U32 Ret = AL_OK;
    AL_U32 Timeout = 10000000;

    AL_LOG(AL_LOG_LEVEL_INFO, "Dmacahb LLP mode test!\r\n");

    ChConfig = ChLlpModeEg1;

    Ret = AlDmacAhb_Hal_Init(&Handle, &ChConfig, AL_NULL, DeviceId);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hal Init error:0x%x\r\n", Ret);
    }
    AlIntr_SetGlobalInterrupt(AL_FUNC_ENABLE);

    AL_U32 TransSize = AL_DMACAHB_TEST_ARRAY_SIZE / (1 << ChConfig.SrcTransWidth);

    AL_DMACAHB_LliStruct Lli[AL_DMACAHB_LLI_STRUCT_NUM];
    for (AL_U32 i = 0; i < AL_DMACAHB_LLI_STRUCT_NUM; i++) {
        Lli[i].SrcAddr = (AL_REG)SRC_MEM[i];
        Lli[i].DstAddr = (AL_REG)DST_MEM[i];

        AlDmacAhb_Dev_FillLliWithCtl(Handle.Channel, &Lli[i]);
        Lli[i].CtlHigh.Bit.BlockTransSize = TransSize;

        if ((i + 1) == AL_DMACAHB_LLI_STRUCT_NUM) {
            Lli[i].LlpNext = (AL_REG)AL_NULL;
            Lli[i].CtlLow.Bit.LlpSrcEn = AL_FALSE;
            Lli[i].CtlLow.Bit.LlpDstEn = AL_FALSE;
        } else {
            Lli[i].LlpNext = (AL_REG)&Lli[i + 1];
        }

        AlDmacAhb_Test_InitSrc(Lli[i].SrcAddr, AL_DMACAHB_TEST_ARRAY_SIZE, 'a' + i);
    }

    Trans = &(Handle.Channel->Trans);
    Trans->Lli = Lli;

    Ret = AlDmacAhb_Hal_StartBlock(&Handle, Timeout);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Trans error:0x%x\r\n", Ret);
    }

    for (AL_U32 i = 0; i < AL_DMACAHB_LLI_STRUCT_NUM; i++) {
        Ret = AlDmacAhb_Test_DataCheck(Lli[i].SrcAddr, Lli[i].DstAddr, AL_DMACAHB_TEST_ARRAY_SIZE);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Llp[%d] data check error:0x%x\r\n", i, Ret);
            break;
        }
    }

    Ret = AlDmacAhb_Hal_DeInit(&Handle);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Deinit error:0x%x\r\n", Ret);
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "Dmacahb LLP mode test done!\r\n");
}

static AL_VOID AlDmacAhb_Test_OtherTransType(AL_VOID)
{
    AL_DMACAHB_HalStruct        Handle;
    AL_DMACAHB_ChInitStruct     ChConfig;
    AL_DMACAHB_ChTransStruct    *Trans;
    AL_U32 DeviceId = 0;
    AL_U32 Ret = AL_OK;
    AL_U32 Timeout = 10000000;

    /*-------------------- trans type 2 --------------------*/
    AL_LOG(AL_LOG_LEVEL_INFO, "Dmacahb other trans type 2 test!\r\n");

    ChConfig = ChOtherTransTypeEg1;

    Ret = AlDmacAhb_Hal_Init(&Handle, &ChConfig, AL_NULL, DeviceId);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hal Init error:0x%x\r\n", Ret);
    }
    AlIntr_SetGlobalInterrupt(AL_FUNC_ENABLE);

    //init SRC_MEM and DST_MEM
    memset(SRC_MEM[0], 0, (AL_DMACAHB_TEST_MEM_SIZE * AL_DMACAHB_TEST_ARRAY_SIZE));
    memset(DST_MEM[0], 0, (AL_DMACAHB_TEST_MEM_SIZE * AL_DMACAHB_TEST_ARRAY_SIZE));
    for (AL_U32 i = 0; i < AL_DMACAHB_RELOAD_COUNT_MAX; i++) {
        AlDmacAhb_Test_InitSrc((AL_REG)SRC_MEM[i], AL_DMACAHB_TEST_ARRAY_SIZE, ('a' + i));
    }

    Trans = &(Handle.Channel->Trans);
    Trans->SrcAddr = (AL_REG)SRC_MEM[0];
    Trans->DstAddr = (AL_REG)DST_MEM[0];
    Trans->TransSize = AL_DMACAHB_TEST_ARRAY_SIZE / (1 << ChConfig.SrcTransWidth);
    Trans->ReloadCountNum = AL_DMACAHB_RELOAD_COUNT_MAX;
    Trans->ReloadCount = 0;

    Ret = AlDmacAhb_Hal_StartBlock(&Handle, Timeout);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Trans error:0x%x\r\n", Ret);
    }

    Ret = AlDmacAhb_Test_DataCheck((AL_REG)SRC_MEM[AL_DMACAHB_RELOAD_COUNT_MAX - 1], Trans->DstAddr,
                                   AL_DMACAHB_TEST_ARRAY_SIZE);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Data check error:0x%x\r\n", Ret);
    }

    Ret = AlDmacAhb_Hal_DeInit(&Handle);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Deinit error:0x%x\r\n", Ret);
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "Dmacahb other trans type 2 test done!\r\n");

    /*-------------------- trans type 2 --------------------*/
    /*-------------------- trans type 3 --------------------*/
    AL_LOG(AL_LOG_LEVEL_INFO, "Dmacahb other trans type 3 test!\r\n");

    ChConfig = ChOtherTransTypeEg2;

    Ret = AlDmacAhb_Hal_Init(&Handle, &ChConfig, AL_NULL, DeviceId);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hal Init error:0x%x\r\n", Ret);
    }

    //init SRC_MEM and DST_MEM
    memset(SRC_MEM[0], 0, (AL_DMACAHB_TEST_MEM_SIZE * AL_DMACAHB_TEST_ARRAY_SIZE));
    memset(DST_MEM[0], 0, (AL_DMACAHB_TEST_MEM_SIZE * AL_DMACAHB_TEST_ARRAY_SIZE));
    AlDmacAhb_Test_InitSrc((AL_REG)SRC_MEM[0], AL_DMACAHB_TEST_ARRAY_SIZE, 'a');

    Trans = &(Handle.Channel->Trans);
    Trans->SrcAddr = (AL_REG)SRC_MEM[0];
    Trans->DstAddr = (AL_REG)DST_MEM[0];
    Trans->TransSize = AL_DMACAHB_TEST_ARRAY_SIZE / (1 << ChConfig.SrcTransWidth);
    Trans->ReloadCountNum = AL_DMACAHB_RELOAD_COUNT_MAX;
    Trans->ReloadCount = 0;

    Ret = AlDmacAhb_Hal_StartBlock(&Handle, Timeout);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Trans error:0x%x\r\n", Ret);
    }

    Ret = AlDmacAhb_Test_DataCheck(Trans->SrcAddr, (AL_REG)DST_MEM[AL_DMACAHB_RELOAD_COUNT_MAX - 1],
                                   AL_DMACAHB_TEST_ARRAY_SIZE);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Data check error:0x%x\r\n", Ret);
    }

    Ret = AlDmacAhb_Hal_DeInit(&Handle);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Deinit error:0x%x\r\n", Ret);
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "Dmacahb other trans type 3 test done!\r\n");

    /*-------------------- trans type 3 --------------------*/
    AL_U32 TransSize;
    AL_DMACAHB_LliStruct Lli[AL_DMACAHB_LLI_STRUCT_NUM];
    /*-------------------- trans type 6 --------------------*/
    AL_LOG(AL_LOG_LEVEL_INFO, "Dmacahb other trans type 6 test!\r\n");

    ChConfig = ChOtherTransTypeEg3;

    Ret = AlDmacAhb_Hal_Init(&Handle, &ChConfig, AL_NULL, DeviceId);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hal Init error:0x%x\r\n", Ret);
    }

    memset(SRC_MEM[0], 0, (AL_DMACAHB_TEST_MEM_SIZE * AL_DMACAHB_TEST_ARRAY_SIZE));
    memset(DST_MEM[0], 0, (AL_DMACAHB_TEST_MEM_SIZE * AL_DMACAHB_TEST_ARRAY_SIZE));

    TransSize = AL_DMACAHB_TEST_ARRAY_SIZE / (1 << ChConfig.SrcTransWidth);

    for (AL_U32 i = 0; i < AL_DMACAHB_LLI_STRUCT_NUM; i++) {
        memset (&Lli[i], 0, sizeof(AL_DMACAHB_LliStruct));
        Lli[i].DstAddr = (AL_REG)DST_MEM[i];

        AlDmacAhb_Dev_FillLliWithCtl(Handle.Channel, &Lli[i]);
        Lli[i].CtlHigh.Bit.BlockTransSize = TransSize;

        if ((i + 1) == AL_DMACAHB_LLI_STRUCT_NUM) {
            Lli[i].LlpNext = (AL_REG)AL_NULL;
            Lli[i].CtlLow.Bit.LlpSrcEn = AL_FALSE;
            Lli[i].CtlLow.Bit.LlpDstEn = AL_FALSE;
        } else {
            Lli[i].LlpNext = (AL_REG)&Lli[i + 1];
        }
        AlDmacAhb_Test_InitSrc((AL_REG)SRC_MEM[i], AL_DMACAHB_TEST_ARRAY_SIZE, 'a' + i);
    }

    Trans = &(Handle.Channel->Trans);
    Trans->Lli      = Lli;
    Trans->SrcAddr  = (AL_REG)SRC_MEM[0];

    Ret = AlDmacAhb_Hal_StartBlock(&Handle, Timeout);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Trans error:0x%x\r\n", Ret);
    }

    for (AL_U32 i = 0; i < AL_DMACAHB_LLI_STRUCT_NUM; i++) {
        Ret = AlDmacAhb_Test_DataCheck((AL_REG)SRC_MEM[i], (AL_REG)DST_MEM[i], AL_DMACAHB_TEST_ARRAY_SIZE);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Llp[%d] data check error:0x%x\r\n", i, Ret);
            break;
        }
    }

    Ret = AlDmacAhb_Hal_DeInit(&Handle);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Deinit error:0x%x\r\n", Ret);
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "Dmacahb other trans type 6 test done!\r\n");
    /*-------------------- trans type 6 --------------------*/
    /*-------------------- trans type 7 --------------------*/
    AL_LOG(AL_LOG_LEVEL_INFO, "Dmacahb other trans type 7 test!\r\n");

    ChConfig = ChOtherTransTypeEg4;

    Ret = AlDmacAhb_Hal_Init(&Handle, &ChConfig, AL_NULL, DeviceId);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hal Init error:0x%x\r\n", Ret);
    }

    TransSize = AL_DMACAHB_TEST_ARRAY_SIZE / (1 << ChConfig.SrcTransWidth);

    memset(SRC_MEM[0], 0, (AL_DMACAHB_TEST_MEM_SIZE * AL_DMACAHB_TEST_ARRAY_SIZE));
    memset(DST_MEM[0], 0, (AL_DMACAHB_TEST_MEM_SIZE * AL_DMACAHB_TEST_ARRAY_SIZE));

    for (AL_U32 i = 0; i < AL_DMACAHB_LLI_STRUCT_NUM; i++) {
        memset (&Lli[i], 0, sizeof(AL_DMACAHB_LliStruct));
        Lli[i].DstAddr = (AL_REG)DST_MEM[i];

        AlDmacAhb_Dev_FillLliWithCtl(Handle.Channel, &Lli[i]);
        Lli[i].CtlHigh.Bit.BlockTransSize = TransSize;

        if ((i + 1) == AL_DMACAHB_LLI_STRUCT_NUM) {
            Lli[i].LlpNext = (AL_REG)AL_NULL;
            Lli[i].CtlLow.Bit.LlpSrcEn = AL_FALSE;
            Lli[i].CtlLow.Bit.LlpDstEn = AL_FALSE;
        } else {
            Lli[i].LlpNext = (AL_REG)&Lli[i + 1];
        }
        AlDmacAhb_Test_InitSrc((AL_REG)SRC_MEM[i], AL_DMACAHB_TEST_ARRAY_SIZE, 'a' + i);
    }

    Trans = &(Handle.Channel->Trans);
    Trans->Lli              = Lli;
    Trans->SrcAddr          = (AL_REG)SRC_MEM[0];
    Trans->ReloadCountNum   = AL_DMACAHB_LLI_STRUCT_NUM;
    Trans->ReloadCount      = 0;

    Ret = AlDmacAhb_Hal_StartBlock(&Handle, Timeout);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Trans error:0x%x\r\n", Ret);
    }

    for (AL_U32 i = 0; i < AL_DMACAHB_LLI_STRUCT_NUM; i++) {
        Ret = AlDmacAhb_Test_DataCheck((AL_REG)SRC_MEM[0], (AL_REG)DST_MEM[i], AL_DMACAHB_TEST_ARRAY_SIZE);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Data check error:0x%x\r\n", Ret);
            break;
        }
    }

    Ret = AlDmacAhb_Hal_DeInit(&Handle);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Deinit error:0x%x\r\n", Ret);
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "Dmacahb other trans type 7 test done!\r\n");
    /*-------------------- trans type 7 --------------------*/
    /*-------------------- trans type 8 --------------------*/
    AL_LOG(AL_LOG_LEVEL_INFO, "Dmacahb other trans type 8 test!\r\n");

    ChConfig = ChOtherTransTypeEg5;

    Ret = AlDmacAhb_Hal_Init(&Handle, &ChConfig, AL_NULL, DeviceId);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hal Init error:0x%x\r\n", Ret);
    }

    TransSize = AL_DMACAHB_TEST_ARRAY_SIZE / (1 << ChConfig.SrcTransWidth);

    memset(SRC_MEM[0], 0, (AL_DMACAHB_TEST_MEM_SIZE * AL_DMACAHB_TEST_ARRAY_SIZE));
    memset(DST_MEM[0], 0, (AL_DMACAHB_TEST_MEM_SIZE * AL_DMACAHB_TEST_ARRAY_SIZE));

    for (AL_U32 i = 0; i < AL_DMACAHB_LLI_STRUCT_NUM; i++) {
        memset (&Lli[i], 0, sizeof(AL_DMACAHB_LliStruct));
        Lli[i].SrcAddr = (AL_REG)SRC_MEM[i];

        AlDmacAhb_Dev_FillLliWithCtl(Handle.Channel, &Lli[i]);
        Lli[i].CtlHigh.Bit.BlockTransSize = TransSize;

        if ((i + 1) == AL_DMACAHB_LLI_STRUCT_NUM) {
            Lli[i].LlpNext = (AL_REG)AL_NULL;
            Lli[i].CtlLow.Bit.LlpSrcEn = AL_FALSE;
            Lli[i].CtlLow.Bit.LlpDstEn = AL_FALSE;
        } else {
            Lli[i].LlpNext = (AL_REG)&Lli[i + 1];
        }

        AlDmacAhb_Test_InitSrc(Lli[i].SrcAddr, AL_DMACAHB_TEST_ARRAY_SIZE, 'a' + i);
    }

    Trans = &(Handle.Channel->Trans);
    Trans->Lli      = Lli;
    Trans->DstAddr  = (AL_REG)DST_MEM[0];

    Ret = AlDmacAhb_Hal_StartBlock(&Handle, Timeout);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Trans error:0x%x\r\n", Ret);
    }

    for (AL_U32 i = 0; i < AL_DMACAHB_LLI_STRUCT_NUM; i++) {
        Ret = AlDmacAhb_Test_DataCheck((AL_REG)SRC_MEM[i], (AL_REG)DST_MEM[i], AL_DMACAHB_TEST_ARRAY_SIZE);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Llp[%d] data check error:0x%x\r\n", i, Ret);
            break;
        }
    }

    Ret = AlDmacAhb_Hal_DeInit(&Handle);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Deinit error:0x%x\r\n", Ret);
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "Dmacahb other trans type 8 test done!\r\n");
    /*-------------------- trans type 8 --------------------*/
    /*-------------------- trans type 9 --------------------*/
    AL_LOG(AL_LOG_LEVEL_INFO, "Dmacahb other trans type 9 test!\r\n");

    ChConfig = ChOtherTransTypeEg6;

    Ret = AlDmacAhb_Hal_Init(&Handle, &ChConfig, AL_NULL, DeviceId);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hal Init error:0x%x\r\n", Ret);
    }

    TransSize = AL_DMACAHB_TEST_ARRAY_SIZE / (1 << ChConfig.SrcTransWidth);

    memset(SRC_MEM[0], 0, (AL_DMACAHB_TEST_MEM_SIZE * AL_DMACAHB_TEST_ARRAY_SIZE));
    memset(DST_MEM[0], 0, (AL_DMACAHB_TEST_MEM_SIZE * AL_DMACAHB_TEST_ARRAY_SIZE));

    for (AL_U32 i = 0; i < AL_DMACAHB_LLI_STRUCT_NUM; i++) {
        memset (&Lli[i], 0, sizeof(AL_DMACAHB_LliStruct));
        Lli[i].SrcAddr = (AL_REG)SRC_MEM[i];

        AlDmacAhb_Dev_FillLliWithCtl(Handle.Channel, &Lli[i]);
        Lli[i].CtlHigh.Bit.BlockTransSize = TransSize;

        if ((i + 1) == AL_DMACAHB_LLI_STRUCT_NUM) {
            Lli[i].LlpNext = (AL_REG)AL_NULL;
            Lli[i].CtlLow.Bit.LlpSrcEn = AL_FALSE;
            Lli[i].CtlLow.Bit.LlpDstEn = AL_FALSE;
        } else {
            Lli[i].LlpNext = (AL_REG)&Lli[i + 1];
        }

        AlDmacAhb_Test_InitSrc(Lli[i].SrcAddr, AL_DMACAHB_TEST_ARRAY_SIZE, 'a' + i);
    }

    Trans = &(Handle.Channel->Trans);
    Trans->Lli              = Lli;
    Trans->DstAddr          = (AL_REG)DST_MEM[0];
    Trans->ReloadCountNum   = AL_DMACAHB_LLI_STRUCT_NUM;
    Trans->ReloadCount      = 0;

    Ret = AlDmacAhb_Hal_StartBlock(&Handle, Timeout);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Trans error:0x%x\r\n", Ret);
    }

    Ret = AlDmacAhb_Test_DataCheck((AL_REG)SRC_MEM[AL_DMACAHB_LLI_STRUCT_NUM-1], (AL_REG)DST_MEM[0],
                                   AL_DMACAHB_TEST_ARRAY_SIZE);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Data check error:0x%x\r\n", Ret);
    }

    Ret = AlDmacAhb_Hal_DeInit(&Handle);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Deinit error:0x%x\r\n", Ret);
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "Dmacahb other trans type 9 test done!\r\n");
    /*-------------------- trans type 9 --------------------*/
}
