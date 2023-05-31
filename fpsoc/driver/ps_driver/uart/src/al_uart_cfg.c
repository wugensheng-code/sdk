
/********************************* including Files *********************************/
#include "al_uart_hw.h"

/********************************* Constant Definitions *****************************/


/********************************* Type definitions *********************************/


/********************************* Macros  & Inline Functions Definitions ***********/


/******************************** Function Prototypes ******************************/


/******************************** Variable Prototypes ******************************/

AL_UART_HwConfigStruct AlUart_HwConfig[AL_UART_NUM_INSTANCE] =
{
    {
        .DeviceId           = 0,
        .BaseAddress        = UART0__BASE_ADDR,
        .InputClockHz       = UART_CLOCK,
        .IrqNum             = SOC_UART0_IRQn,
        .ModemPinsConnected = 1
    },

    {
        .DeviceId           = 1,
        .BaseAddress        = UART1__BASE_ADDR,
        .InputClockHz       = UART_CLOCK,
        .IrqNum             = SOC_UART1_IRQn,
        .ModemPinsConnected = 1
    }
};