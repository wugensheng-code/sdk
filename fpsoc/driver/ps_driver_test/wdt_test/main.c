#include "al_wdt_test_config.h"

static AL_S32 AlWdt_Test_Interrupt_Mode(AL_VOID);

AL_S32 main(AL_VOID)
{
    AL_S32 Ret = AL_OK;
#if CONFIG_ALWDT_INTERRUP_MODE_TEST
    printf("AlWdt Interrupt mode example testing...\r\n");
    Ret = AlWdt_Test_Interrupt_Mode();
    if (Ret != AL_OK) {
        printf("AlWdt Interrupt mode examplee test error\r\n");
        return Ret;
    }
#endif
}

AL_VOID AlWdt_Hal_CustomEventHandler(AL_VOID *CallbackRef)
{
    AL_UNUSED(CallbackRef);

    printf("WDT interrupt occurred");
}

static AL_S32 AlWdt_Test_Interrupt_Mode(AL_VOID)
{
    AL_WDT_HalStruct wdt0;

    AL_S32 Ret = AlWdt_Hal_Init(&wdt0, 0, &WDT_InitStruct, AlWdt_Hal_CustomEventHandler);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlWdt_Hal_Init error\r\n");
        return Ret;
    }

    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);
}
