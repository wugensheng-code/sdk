/*****************************************************************************/
/**
* @file al_assert.h
* This file contains assertion related APIs and Macros. 
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

#ifndef AL_ASSERT_H
#define AL_ASSERT_H

#include "al_types.h"

#ifdef NDEBUG
#define Al_Assert(...)
#else
void Al_Assert(const u8 *File, u32 Line);

#endif  /* NDEBUG */
#endif  /* AL_ASSERT_H */
