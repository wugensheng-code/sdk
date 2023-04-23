/*****************************************************************************
*****************************************************************************/

#ifndef __AL_TYPE_H__
#define __AL_TYPE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/*----------------------------------------------*
 * The common data type, will be used in the whole project.*
 *----------------------------------------------*/

typedef unsigned char           AL_U8;
typedef unsigned short          AL_U16;
typedef unsigned int            AL_U32;
typedef signed char             AL_S8;
typedef short                   AL_S16;
typedef int                     AL_S32;
typedef float                   AL_FLOAT;
typedef double                  AL_DOUBLE;
typedef unsigned long long      AL_U64;
typedef long long               AL_S64;
typedef char                    AL_CHAR;
typedef unsigned char           AL_UCHAR;
typedef unsigned int            AL_HANDLE;

typedef volatile AL_U32         AL_REG32;

typedef void                    AL_VOID;
typedef long                    AL_INTPTR;

typedef unsigned long           AL_UINTPTR;


/*----------------------------------------------*
 * const defination                             *
 *----------------------------------------------*/
typedef enum
{
    AL_FALSE = 0,
    AL_TRUE  = 1
} AL_BOOL;

typedef enum
{
    AL_FUNC_DISABLE   = 0,
    AL_FUNC_ENABLE    = 1
} AL_FUNCTION;

#define AL_NULL         ((void *)0L)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_TYPE_H__ */

