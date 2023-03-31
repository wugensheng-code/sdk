/*****************************************************************************/
/**
 * @file al_csu_config.h
 * This is the header file which contains definitions for project
 *configurations.
 *
 * <pre>
 * MODIFICATION HISTORY:
 *
 * Ver   Who  Date        Changes
 * ----- ---- -------- -------------------------------------------------------
 * 1.00  c.z  3/10/22  Initial release
 * </pre>
 *
 * @note
 ******************************************************************************/

#ifndef AL_CSU_CONFIG_H
#define AL_CSU_CONFIG_H

#include "al_types.h"

/* Select between bootrom Lite and Full version */
#ifndef BOOTROM_LITE
#define BOOTROM_LITE 0x0
#endif // !BOOTROM_LITE

/* CSU life time counter granule time */
#define CSU_LIFETIME_COUNTER_STEP 100000 /* 100ms */

/* Simulation environment ON/OFF select */
#ifndef CSU_SIMULATION_ON
#define CSU_SIMULATION_ON 0x0
#endif // !CSU_SIMULATION_ON

#if (AL_TRUE == CSU_SIMULATION_ON)
#define CSU_SIMULATION
#endif

/* Debug function (assert/print) ON/OFF select */
#ifndef CSU_DEBUG_ON
#define CSU_DEBUG_ON 0x1
#endif // !CSU_DEBUG_ON

#if (AL_TRUE == CSU_DEBUG_ON)
#undef NDEBUG /* Enable assert */
#include "al_assert.h"
#define CSU_PRINT /* Enable print */
#include "al_printf.h"
#else
#define NDEBUG /* Disable assert */
#include "al_assert.h"
#undef CSU_PRINT /* Disable print */
#include "al_printf.h"
#endif

/* Select to enable footprint - write current func name into global register for
 * debugging purpose */
#define CSU_FOOTPRINT_ON 0x1

#if (CSU_FOOTPRINT_ON == AL_TRUE)
#define CSU_FOOTPRINT
#endif

/* Select to exclude functions */
#define CSU_QSPI_EXCLUDE 0x0
#define CSU_NAND_EXCLUDE 0x0
#define CSU_SD_EMMC_EXCLUDE 0x0
#define CSU_RAW_EMMC_EXCLUDE 0x0
#define CSU_SECURE_EXCLUDE 0x0

/* FSBL image copy and handoff are excluded in BOOTROM_LITE */
#if (BOOTROM_LITE == 0x1)
#define CSU_HANDOFF_EXCLUDE 0x1
#define CSU_IMGCPY_EXCLUDE 0x1
#else
#define CSU_HANDOFF_EXCLUDE 0x0
#define CSU_IMGCPY_EXCLUDE 0x0
#endif /* BOOTROM_LITE */

#if (CSU_QSPI_EXCLUDE != AL_TRUE)
#define CSU_QSPI
#endif

#if (CSU_NAND_EXCLUDE != AL_TRUE)
#define CSU_NAND
#endif

#if (CSU_SD_EMMC_EXCLUDE != AL_TRUE)
#define CSU_SD_EMMC
#endif

#if (CSU_RAW_EMMC_EXCLUDE != AL_TRUE)
#define CSU_RAW_EMMC
#endif

#if (CSU_SECURE_EXCLUDE != AL_TRUE)
#define CSU_SECURE
#endif

#if (CSU_HANDOFF_EXCLUDE != AL_TRUE)
#define CSU_HANDOFF
#endif

#if (CSU_IMGCPY_EXCLUDE != AL_TRUE)
#define CSU_IMGCPY
#endif

#endif /* AL_CSU_CONFIG_H */
