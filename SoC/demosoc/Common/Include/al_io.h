/*****************************************************************************/
/**
* @file al_csu_io.h
* This is the header file which contains general Register IO interfacing APIs.
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

#ifndef AL_IO_H
#define AL_IO_H

#include "al_types.h"


static inline u32 REG32_READ(UINTPTR Address)
{
    return *(volatile u32 *) Address;
}


static inline u32 REG32_WRITE(UINTPTR Address, u32 Value)
{
    volatile u32 *LocalAddress = (volatile u32 *) Address;
    *LocalAddress = Value;

    return 0;
}

#endif  /* AL_IO_H */
