#include <common.h>
#include <string.h>

#define DMACAHB_DEVICE_ID           (0)
#define DMACAHB_TRANS_TIMEOUT_MS    (pdMS_TO_TICKS(100))
#define DMACAHB_INTERVAL_TIME       (pdMS_TO_TICKS(100))
#define DMACAHB_ARRAY_SIZE          (64)

static AL_DMACAHB_ChInitStruct DmaChConfig = {
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

static int AlDma_ahb_Test_Universal_Setup(void **state) {
    AL_DMACAHB_HalStruct *Dma;
    AL_U32 DeviceId = 0;
    AL_S32 Ret = AL_OK;

    Ret = AlDmacAhb_Hal_Init(&Dma, DeviceId, &DmaChConfig, AL_NULL);

    *state = Dma;

    assert_int_equal(Ret, AL_OK);
}

/* A test case that does nothing and succeeds. */
static void AlDma_Ahb_Tc(void **state) {
    AL_S32 Ret = AL_OK;
    AL_U8 InitData = 0;
    AL_DMACAHB_HalStruct *Dma = (AL_DMACAHB_HalStruct *)*state;
    AL_DMACAHB_ChTransStruct *ChTransCfg = AL_NULL;
    AL_U8 *MemSrc = (AL_U8 *)pvPortMalloc(DMACAHB_ARRAY_SIZE);
    AL_U8 *MemDst = (AL_U8 *)pvPortMalloc(DMACAHB_ARRAY_SIZE);
    TickType_t Start = xTaskGetTickCount();

    ChTransCfg = &Dma->Channel.Trans;
    ChTransCfg->SrcAddr = (AL_REG)MemSrc;
    ChTransCfg->DstAddr = (AL_REG)MemDst;
    ChTransCfg->TransSize = DMACAHB_ARRAY_SIZE / (1 << DmaChConfig.SrcTransWidth);


    while ((xTaskGetTickCount() - Start) < TEST_TIME)
    {
        memset(ChTransCfg->SrcAddr, InitData++, DMACAHB_ARRAY_SIZE);

        Ret = AlDmacAhb_Hal_StartBlock(Dma, pdMS_TO_TICKS(100));
        assert_int_equal(Ret, AL_OK);

        Ret = memcmp(ChTransCfg->SrcAddr, ChTransCfg->DstAddr, DMACAHB_ARRAY_SIZE);
        assert_int_equal(Ret, AL_OK);

        memset(ChTransCfg->DstAddr, 0, DMACAHB_ARRAY_SIZE);

        #ifdef ENABLE_MMU
        AlCache_FlushDcacheRange(ChTransCfg->DstAddr, ChTransCfg->DstAddr + DMACAHB_ARRAY_SIZE);
        #endif
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

CMOCKA_TC_DEFINE(AlDma_Ahb_Tc, AlDma_ahb_Test_Universal_Setup, NULL, NULL);

