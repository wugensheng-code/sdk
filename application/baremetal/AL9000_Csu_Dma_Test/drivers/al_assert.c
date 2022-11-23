/*****************************************************************************/
/**
* @file al_assert.c
* This file contains basic assert related functions.
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

#include "al_types.h"

#ifndef NDEBUG

/**
 * This global variable allows assertion status to be abserved easily.
 */
u32 Al_AssertStatus = AL_FALSE;

/** 
 * This function handles assertion.
 * @param   File: file name of source
 * @param   Line: line number of source 
 * @return  None
 */
void Al_Assert(const u8 *File, u32 Line)
{
    Al_AssertStatus = AL_TRUE;
    al_printf("Assert in File: %s, Line: %d\n", File, Line);

    while (1) {

    }
}

#endif /* NDEBUG*/

