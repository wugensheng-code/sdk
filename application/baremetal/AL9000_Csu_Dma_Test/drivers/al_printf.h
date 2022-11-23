/*****************************************************************************/
/**
 * @file al_csu_print.h
 * This is the header file which contains definitions for print functions.
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

#ifndef AL_CSU_PRINT_H
#define AL_CSU_PRINT_H

#include "al_types.h"
#include "stdarg.h"

#ifdef CSU_PRINT
#ifdef CSU_SIMULATION
int al_printf(const char *format, ...);
#else // !CSU_SIMULATION
#define al_printf(...) printf(__VA_ARGS__)
#endif
#else // !CSU_PRINT
#define al_printf(type, ...)
#endif

#endif /* AL_CSU_PRINT_H */
