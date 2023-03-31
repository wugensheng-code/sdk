/*****************************************************************************/
/**
* @file al_types.h
* The al_types.h file contains basic types which are applicable for all 
* processors supported by Anlogic.
* 
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who    Date   Changes
* ----- ---- -------- -------------------------------------------------------
* 1.00  c.z  21/03/22 First release
* </pre>
******************************************************************************/

#ifndef AL_TYPES_H	
#define AL_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>


/**
 * Bool values
 */
#ifndef AL_SUCCESS
#define AL_SUCCESS		0U
#endif

#ifndef AL_FAILED
#define AL_FAILED		1U
#endif

   
/**
 * New basic types 
 */
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
//typedef uint64_t u64;

typedef intptr_t INTPTR;
typedef uintptr_t UINTPTR;

#ifdef __cplusplus
}
#endif

#endif	/* end of protection macro */
