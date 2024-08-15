#ifndef __COMMON_H
#define __COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include "al_core.h"
#include "al_spinor.h"
#include "al_gpio_hal.h"
#include "al_can_hal.h"
#include "al_dmacahb_hal.h"
#include "al_wdt_hal.h"
#include "al_iic_hal.h"
#include "al_mmc_hal.h"
#include "al_dma_hal.h"
#include "al_spinor.h"
#include "al_spi_hal.h"
#include "al_qspi_hal.h"
#include "al_uart_hal.h"
#include "al_tc_hal.h"
#include "al_gbe_ethernetif.h"
#include "al_ipc_hal.h"
#include "al_xmon_hal.h"
#include "al_mpu_hal.h"
#include <cmocka_utils.h>

/* Kernel includes. */
#include "FreeRTOS.h" /* Must come first. */
#include "queue.h"    /* RTOS queue related API prototypes. */
#include "semphr.h"   /* Semaphore related API prototypes. */
#include "task.h"     /* RTOS task related API prototypes. */
#include "timers.h"   /* Software timer related API prototypes. */

#include <stdio.h>
#include <stdlib.h>

/* Test case runtime (tick) */
#define TEST_TIME                   (pdMS_TO_TICKS(1000))

#ifdef __cplusplus
}
#endif

#endif
