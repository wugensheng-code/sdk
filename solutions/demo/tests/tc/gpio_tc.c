#include <common.h>

#define GPIO_LED_PIN_NUM            (14)

static AL_S32 AlGpio_Test_EventCallBack(AL_GPIO_EventStruct Event, AL_VOID *CallBackRef)
{
    print_message("Bank 0x%x: IntrStatus is 0x%x", Event.Bank, Event.EventData);
    AL_GPIO_HalStruct *Handle = (AL_GPIO_HalStruct *)CallBackRef;
    AlGpio_Hal_MaskBankIntr(Handle, Event.Bank, Event.EventData);
}

static int AlGpio_Test_Universal_Setup(void **state) {
    AL_GPIO_HalStruct *GPIO;
    AL_U32 DevId = 0;

    AL_S32 Ret = AlGpio_Hal_Init(&GPIO, DevId, AlGpio_Test_EventCallBack);

    *state = GPIO;

    assert_int_equal(Ret, AL_OK);
}

static void AlGpio_Write_Tc(void **state)
{
    AL_S32 Ret = AL_OK;
    AL_GPIO_HalStruct *GPIO = (AL_GPIO_HalStruct *)*state;
    TickType_t Start = xTaskGetTickCount();

    while ((xTaskGetTickCount() - Start) < TEST_TIME)
    {
        Ret = AlGpio_Hal_WritePin(GPIO, GPIO_LED_PIN_NUM, 0x0);
        (AlGpio_Hal_ReadPinOutput(GPIO, GPIO_LED_PIN_NUM) == 0x0) ? printf("Write pin succeed \r\n") : printf("Write pin failed \r\n");
        vTaskDelay(pdMS_TO_TICKS(100));
        Ret = AlGpio_Hal_WritePin(GPIO, GPIO_LED_PIN_NUM, 0x1);
        (AlGpio_Hal_ReadPinOutput(GPIO, GPIO_LED_PIN_NUM) == 0x1) ? printf("Write pin succeed \r\n") : printf("Write pin failed \r\n");
        vTaskDelay(pdMS_TO_TICKS(100));
    }

    assert_int_equal(Ret, AL_OK);

    return;
}

CMOCKA_TC_DEFINE(AlGpio_Write_Tc, AlGpio_Test_Universal_Setup, NULL, NULL);
