#ifndef __AL_CORE_H__
#define __AL_CORE_H__

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include "al_core.h"

#ifdef HAVE_CANPS_DRIVER
#include "al_can_hal.h"
#endif

#ifdef HAVE_DMACAHBPS_DRIVER
#include "al_dmacahb_hal.h"
#endif

#ifdef HAVE_GPIOPS_DRIVER
#include "al_gpio_hal.h"
#endif

#ifdef HAVE_IICPS_DRIVER
#include "al_iic_hal.h"
#endif

#ifdef HAVE_MMCPS_DRIVER
#include "al_mmc_hal.h"
#endif

#ifdef HAVE_MPUPS_DRIVER
#include "al_mpu_hal.h"
#endif

#ifdef HAVE_QSPIPS_DRIVER
#include "al_qspi_hal.h"
#endif

#ifdef HAVE_SMCPS_DRIVER
#include "al_smc_hal.h"
#endif

#ifdef HAVE_SPIPS_DRIVER
#include "al_spi_hal.h"
#endif

#ifdef HAVE_UARTPS_DRIVER
#include "al_uart_hal.h"
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* AL_CORE_H */
