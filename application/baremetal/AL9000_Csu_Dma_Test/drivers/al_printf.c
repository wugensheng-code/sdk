/*****************************************************************************/
/**
 * @file xxx.c
 * This is the source file which contains definitions for the
 * xxx functions.
 *
 * <pre>
 * MODIFICATION HISTORY:
 *
 * Ver   Who  Date        Changes
 * ----- ---- -------- -------------------------------------------------------
 * 1.00  c.z  4/18/22  Initial release
 * </pre>
 *
 * @note
 ******************************************************************************/
#include "al_csu_config.h"

#ifdef CSU_PRINT
#ifdef CSU_SIMULATION
int al_printf(const char *format, ...) { return 0; }
#endif
#endif