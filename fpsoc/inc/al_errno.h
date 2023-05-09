#ifndef __AL_ERRNO_H__
#define __AL_ERRNO_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif

#endif

#define AL_ERR_BAREMETAL        (0x1 << 31)

typedef enum
{
    AL_ERR_LEVEL_DEBUG = 0,      /* debug-level                                  */
    AL_ERR_LEVEL_INFO,           /* informational                                */
    AL_ERR_LEVEL_NOTICE,         /* normal but significant condition             */
    AL_ERR_LEVEL_WARNING,        /* warning conditions                           */
    AL_ERR_LEVEL_ERROR,          /* error conditions                             */
    AL_ERR_LEVEL_CRIT,           /* critical conditions                          */
    AL_ERR_LEVEL_ALERT,          /* action must be taken immediately             */
    AL_ERR_LEVEL_FATAL,          /* just for compatibility with previous version */
    AL_ERR_LEVEL_BUTT
} ENUM_AL_ERR_LEVEL;


/******************************************************************************
|--------------------------------------|
| 1 |   MOD_ID  |  LEVEL    |  ERR_ID  |
|---------------------------------------|
|<--><--8bits-->|<--8bits-->|<-16bits->|
******************************************************************************/
#define AL_DEF_ERR(module, level, errid) \
     ((AL_S32)((AL_ERR_BAREMETAL) | ((module) << 24 ) | (level << 16) | (errid)))

/* NOTE! the following defined all common error code,
** all module must reserved 0~63 for their common error code
*/
typedef enum
{
    AL_ERR_NULL_PTR       = 1,

    AL_ERR_ILLEGAL_PARAM  = 2,

    AL_ERR_NOT_READY      = 3,

    AL_ERR_NOT_SUPPORT    = 4,

    AL_ERR_TIMEOUT        = 5,

    AL_ERR_BUSY           = 6,

} AL_ERR_CODE;


#define    AL_OK    (0x00U)


typedef enum
{
    AL_UART                 = 1,
    AL_MPU                  = 2,
    AL_CAN                  = 3,
    AL_SPI                  = 4,
    AL_SMC                  = 5,
    AL_GPIO                 = 6,
    AL_DMACAHB              = 7,
    AL_INTR                 = 8,
} AL_MODULE_TYPE;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif  /* __AL_ERRNO_H__ */

