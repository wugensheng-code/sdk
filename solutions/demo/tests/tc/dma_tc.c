#include <common.h>

#define DMA_TRANS_DATA_SIZE         (1024)
#define AL_DMA_EX_DATA_SIZE             (1024)


/*******************************************************************************/
/**************************** dma mem cpy blocked ******************************/
/*******************************************************************************/


static AL_DMA_ChCfgStruct DmaConfig = {
    .Direction      = DMA_MEM_TO_MEM,
    .SrcAddrWidth   = DMA_CH_BUSWIDTH_32_BYTES,
    .DstAddrWidth   = DMA_CH_BUSWIDTH_32_BYTES,
    .SrcMaxBurst    = 4,
    .DstMaxBurst    = 4,
};

static int AlDma_Test_Universal_Setup(void **state) {
    AL_DMA_HalStruct *Dma;
    AL_U32 DevId = 0;
    AL_S32 Ret = AL_OK;

    Ret = AlDma_Hal_Init(&Dma, DevId, &DmaConfig);

    *state = Dma;

    assert_int_equal(Ret, AL_OK);
}

static void AlDma_Memory_Copy_Block_Tc(void **state) {

    AL_DMA_HalStruct *Dma = (AL_DMA_HalStruct *)*state;
    TickType_t Start = xTaskGetTickCount();
    AL_U32 Ret = AL_OK;
    AL_U32 InitData = 0;
    AL_U32 *Src = pvPortMalloc(DMA_TRANS_DATA_SIZE);
    AL_U32 *Dst = pvPortMalloc(DMA_TRANS_DATA_SIZE);

    while ((xTaskGetTickCount() - Start) < TEST_TIME)
    {
        InitData++;

        for (AL_U32 i = 0; i < DMA_TRANS_DATA_SIZE / sizeof(AL_U32); i++) {
            Src[i] = i + InitData;
        }

        Ret = AlDma_Hal_MemCpyBlock(Dma, Dst, Src, DMA_TRANS_DATA_SIZE);
        assert_int_equal(Ret, AL_OK);

        Ret = memcmp(Dst, Src, DMA_TRANS_DATA_SIZE);
        assert_int_equal(Ret, AL_OK);

        memset(Dst, 0, DMA_TRANS_DATA_SIZE);

        #ifdef ENABLE_MMU
        AlCache_FlushDcacheRange(Dst, Dst + DMA_TRANS_DATA_SIZE);
        #endif

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

/*******************************************************************************/
/**************************** dma mem cpy  *************************************/
/*******************************************************************************/
static AL_S32 AlDma_Test_MemCpy(AL_VOID)
{
    AL_U32 Ret = AL_OK;
    AL_U32 InitData = 0;
    TickType_t Start = xTaskGetTickCount();
    AL_DMA_HalStruct *Handle = AL_NULL;
    AL_U32 *Src = pvPortMalloc(DMA_TRANS_DATA_SIZE);
    AL_U32 *Dst = pvPortMalloc(DMA_TRANS_DATA_SIZE);

    AL_LOG(AL_LOG_LEVEL_INFO, "Copy from %p to %p \r\n", Src, Dst);

    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    while ((xTaskGetTickCount() - Start) < TEST_TIME) {
        AL_LOG(AL_LOG_LEVEL_INFO, "Loop: 0x%d\r\n", InitData++);

        for (AL_U32 i = 0; i < AL_DMA_EX_DATA_SIZE; i++) {
            Src[i] = i + InitData;
        }

        Ret = AlDma_Hal_MemCpy((AL_UINTPTR)Dst, (AL_UINTPTR)Src, AL_DMA_EX_DATA_SIZE * sizeof(AL_U32));
        assert_int_equal(Ret, AL_OK);

        Ret = memcmp(Dst, Src, AL_DMA_EX_DATA_SIZE * sizeof(AL_U32));
        assert_int_equal(Ret, AL_OK);

        memset(Dst, 0, AL_DMA_EX_DATA_SIZE * sizeof(AL_U32));
        #ifdef ENABLE_MMU
        AlCache_FlushDcacheRange(Dst, Dst + AL_DMA_EX_DATA_SIZE * sizeof(AL_U32));
        #endif
        vTaskDelay(pdMS_TO_TICKS(100));
    }

    free(Src);
    free(Dst);

    return Ret;
}



// CMOCKA_TC_DEFINE(AlDma_Memory_Copy_Block_Tc, AlDma_Test_Universal_Setup, NULL, NULL);
// CMOCKA_TC_DEFINE(AlDma_Test_MemCpy, NULL, NULL, NULL);
// dma需要先烧录bitfile，否则无法运行


