
/********************************* including Files *********************************/
#include "al_uart_hw.h"

/********************************* Constant Definitions *****************************/


/********************************* Type definitions *********************************/


/********************************* Macros  & Inline Functions Definitions ***********/


/******************************** Function Prototypes ******************************/


/******************************** Variable Prototypes ******************************/

AL_UART_HwConfigStruct AlUart_HwCfg[AL_UART_NUM_INSTANCE] =
{
    {
        .DeviceId           = 0,
        .BaseAddress        = UART__UART0__BASE_ADDR,
        .InputClockHz       = 25000000,
        .ModemPinsConnected = 1
    },

    {
        .DeviceId           = 1,
        .BaseAddress        = UART__UART1__BASE_ADDR,
        .InputClockHz       = 25000000,
        .ModemPinsConnected = 1
    }
};