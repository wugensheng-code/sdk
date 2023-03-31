/*****************************************************************************/
/**
* @file al_mem.h
* This is the header file which contains oprations for memory copy.
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

#ifndef AL_MEM_H
#define AL_MEM_H

#include "al_types.h"

void Al_MemCpy(void* Dest, const void* Src, u32 ByteCount);

#endif  /* AL_MEM_H */
