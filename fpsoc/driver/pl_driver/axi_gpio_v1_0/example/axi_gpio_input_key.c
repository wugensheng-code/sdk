/**
 * 
 * Date: 2024/04/25
 * axi gpio test, read pin value by interrupt.
 * 
*/

#include "al_axi_gpio_hal.h"

AL_VOID Callback(AlAxiGpio_EventStruct *Event, AL_VOID *CallbackRef)
{
    AL_LOG(AL_LOG_LEVEL_INFO, "Event = %d, ReadPin = %d", Event->EventData, AlAxiGpio_Hal_ReadPin(&CallbackRef, AL_AXI_GPIO_CHANNEL1, 0));
}

int main()
{
    AL_S32 ret = AL_OK;

    AL_LOG(AL_LOG_LEVEL_INFO, "Axi gpio input key interrupt test!");

    /* Instantiate a axi gpio and init it */
    AlAxiGpio_Hal_Struct *Gpio;
    ret = AlAxiGpio_Hal_Init(&Gpio, 0, Callback);
    if (ret != AL_OK)
    {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Axi gpio init failed!");
        return ret;
    }

    /* Enable axi gpio channel1 interrupt */
    AlAxiGpio_Hal_IntInit(&Gpio, AL_AXI_GPIO_CHANNEL1);

    while (1)
    {

    }
    
    return 0;
}