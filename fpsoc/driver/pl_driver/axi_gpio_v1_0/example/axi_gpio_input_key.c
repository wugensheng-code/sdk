/**
 * 
 * Date: 2024/04/25
 * axi gpio test, read pin value by interrupt.
 * 
*/

#include "al_axi_gpio_hal.h"

AL_VOID GP_Port_Enable()
{
    AL_U32 val = AL_REG32_READ(0xF8800080);
    val &= ~0x2;
    AL_REG32_WRITE(0xF8800080, val);
    val = 0;
    val = AL_REG32_READ(0xF8801078);
    AL_REG32_WRITE(0xF8801078, val & (~0x33));
    AlSys_MDelay(1);
    
    AL_REG32_WRITE(0xF8801078, val | 0x33);
}

AL_VOID Callback(AlAxiGpio_EventStruct *Event, AL_VOID *CallbackRef)
{
    printf("Event = %d, ReadPin = %d\n", Event->EventData, AlAxiGpio_Hal_ReadPin(&CallbackRef, AL_AXI_GPIO_CHANNEL1, 0));
}

int main()
{

    printf("=== hello axi gpio ===\n");
    GP_Port_Enable();

    AlAxiGpio_Hal_Struct *Gpio;
    AlAxiGpio_Hal_Init(&Gpio, 0, Callback);
    AlAxiGpio_Hal_IntInit(&Gpio, AL_AXI_GPIO_CHANNEL1);

    while (1)
    {

    }
    
    return 0;
}