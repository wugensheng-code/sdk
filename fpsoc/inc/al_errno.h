#ifndef __AL_ERRNO_H__
#define __AL_ERRNO_H__

#ifdef __cplusplus
extern "C"{
#endif

#define AL_ERR_BAREMETAL        (0x0)

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
|----------------------------------------------------------------|
| 1 |   APP_ID   |   MOD_ID    | ERR_LEVEL | LAYER_ID |  ERR_ID  |
|----------------------------------------------------------------|
|<--><--7bits----><----8bits---><--3bits--><--3bits---><-10bits->|
******************************************************************************/
#define AL_DEF_ERR_LAYER( module, level, layer, errid) \
    ((AL_S32)( (AL_ERR_BAREMETAL) | ((module) << 16 ) | ((level)<<13) | ((layer)<<10) | (errid)))

#define LOW_LEVEL             (0x01)

#define AL_DEF_ERR(module, level, errid)  AL_DEF_ERR_LAYER(module, level, LOW_LEVEL, errid)

#define AL_GET_ERRID(err)     ((AL_S32)(err & ((1 << 10)-1)))

#define AL_ERR_OK             (0x00U)

/* NOTE! the following defined all common error code,
** all module must reserved 0~63 for their common error code
*/
typedef enum
{
    AL_ERR_INVALID_DEVID = 1, /* invlalid device ID                           */
    AL_ERR_INVALID_CHNID = 2, /* invlalid channel ID                          */
    AL_ERR_ILLEGAL_PARAM = 3, /* at lease one parameter is illagal
                               * eg, an illegal enumeration value             */
    AL_ERR_EXIST         = 4, /* resource exists                              */
    AL_ERR_UNEXIST       = 5, /* resource unexists                            */

    AL_ERR_NULL_PTR      = 6, /* using a NULL point                           */

    AL_ERR_NOT_CONFIG    = 7, /* try to enable or initialize system, device
                              ** or channel, before configing attribute       */

    AL_ERR_NOT_SUPPORT   = 8, /* operation or type is not supported by NOW    */

    AL_ERR_TIMEOUT       = 9, /* operation or type is not supported by NOW    */

    AL_ERR_BUSY          = 0xa, /* Device is in busy status                   */

} AL_ERR_CODE;

typedef enum
{
    AL_UART                 = 1,
    AL_MPU                  = 2,
    AL_CAN                  = 3,
} AL_MODULE_TYPE;

#ifdef __cplusplus
}
#endif

#endif  /* __AL_ERRNO_H__ */