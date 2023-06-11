#ifndef __AL_CORE_H__
#define __AL_CORE_H__

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include "al_core.h"

#ifdef HAVE_UARTPS_DRIVER
#include "al_uart_hal.h"
#endif

#ifdef HAVE_SPIPS_DRIVER
#include "al_spi_hal.h"
#endif

#ifdef HAVE_CANPS_DRIVER
#include "al_can_hal.h"
#endif


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* AL_CORE_H */
