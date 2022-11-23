/*****************************************************************************/
/**
* @file al_mem.c
* This is the source file which contains memory oprations. 
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

/** 
 * Memery copy function.
 * @param Dest is the pointer to desination memory. 
 * @param Src is the pointer to source memory. 
 * @param ByteCount is the length of bytes to be copied.
 * @return None
 */
 
void Al_MemCpy(void* Dest, const void* Src, u32 ByteCount)
{
        char *d = (char*)(void *)Dest;
        const char *s = Src;

        while (ByteCount >= sizeof (s32)) {
                *(s32*)d = *(s32*)s;
                d += sizeof (s32);
                s += sizeof (s32);
                ByteCount -= sizeof (s32);
        }
        while (ByteCount >= sizeof (u16)) {
                *(u16*)d = *(u16*)s;
                d += sizeof (u16);
                s += sizeof (u16);
                ByteCount -= sizeof (u16);
        }
        while ((ByteCount) > 0U){
                *d = *s;
                d += 1U;
                s += 1U;
                ByteCount -= 1U;
        }
}
