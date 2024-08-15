#include <common.h>

static AL_WDT_InitStruct WdtConfig = {
    .ResetPuaseLength = WDT_RPL_PCLK_CYCLES_8,
    .ResponseMode = WDT_INTR_MODE,
    .TimeOutValue = WDT_TIMEOUT_PERIOD_2G_CLOCKS,
};


static int AlWdt_Test_Universal_Setup(void **state) {
    AL_WDT_HalStruct *Wdt;
    AL_U32 DevId = 0;
    AL_S32 Ret;

    Ret = AlWdt_Hal_Init(&Wdt, DevId, &WdtConfig, AL_NULL);

    *state = Wdt;

    assert_int_equal(Ret, AL_OK);
}

static void AlWdt_Intr_Tc(void **state) {

    AL_WDT_HalStruct *Wdt = (AL_WDT_HalStruct *)*state;
    TickType_t Start = xTaskGetTickCount();
    AL_S32 Ret;

    while ((xTaskGetTickCount() - Start) < TEST_TIME)
    {
        Ret = AlWdt_Hal_Feed(Wdt);
        vTaskDelay(pdMS_TO_TICKS(2000));
    }

    assert_int_equal(Ret, AL_OK);

}

CMOCKA_TC_DEFINE(AlWdt_Intr_Tc, AlWdt_Test_Universal_Setup, NULL, NULL);

