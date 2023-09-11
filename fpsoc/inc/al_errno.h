/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

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
    AL_LOG_LEVEL_DEBUG = 0,      /* debug-level                                  */
    AL_LOG_LEVEL_INFO,           /* informational                                */
    AL_LOG_LEVEL_NOTICE,         /* normal but significant condition             */
    AL_LOG_LEVEL_WARNING,        /* warning conditions                           */
    AL_LOG_LEVEL_ERROR,          /* error conditions                             */
    AL_LOG_LEVEL_CRIT,           /* critical conditions                          */
    AL_LOG_LEVEL_ALERT,          /* action must be taken immediately             */
    AL_LOG_LEVEL_FATAL,          /* just for compatibility with previous version */
    AL_LOG_LEVEL_BUTT
} ENUM_AL_LOG_LEVEL;


/******************************************************************************
|--------------------------------------|
| 1 |   MOD_ID  |  LEVEL    |  ERR_ID  |
|---------------------------------------|
|<--><--5bits-->|<--3bits-->|<-23bits->|
******************************************************************************/
#define AL_DEF_ERR(module, level, errid) \
     ((AL_S32)((AL_ERR_BAREMETAL) | ((module) << 26 ) | (level << 23) | (errid)))

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
    AL_ERR_NOMEM          = 7,
    AL_ERR_UNAVAILABLE    = 8,
    AL_ERR_MAX
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
    AL_MMC                  = 9,
    AL_QSPI                 = 0x0A,
    AL_UTEST                = 0x0B,
    AL_IIC                  = 0x0C,
    AL_CIPHER               = 0x0D,
    AL_GBE                  = 0x0F,
    AL_XADC                 = 0x10,
    AL_TTC                  = 0x11,
} AL_MODULE_TYPE;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif  /* __AL_ERRNO_H__ */

