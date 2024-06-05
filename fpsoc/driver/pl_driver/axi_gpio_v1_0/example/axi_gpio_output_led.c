/**
 * 
 * Date: 2024/04/25
 * axi gpio test, write pin.
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

int main()
{
    AL_U32 Val = 0;

    printf("=== hello axi gpio ===\n");
    GP_Port_Enable();

    AlAxiGpio_Hal_Struct *Gpio;
    AlAxiGpio_Hal_Init(&Gpio, 0, AL_NULL);

    while (1)
    {
        Val = ~Val;
        AlAxiGpio_Hal_WritePin(&Gpio, AL_AXI_GPIO_CHANNEL1, 0, Val);
        AlSys_MDelay(200);
    }
    
    return 0;
}