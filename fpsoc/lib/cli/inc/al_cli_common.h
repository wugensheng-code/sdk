#ifndef AL_CLI_COMMON_H
#define AL_CLI_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>

#define AL_CLI_PROMT                            "alsh >"

/* define error code */
#define AL_CLI_OK                               0x0

#define AL_CLI_ERROR_ILLEGAL_PARAM              0x101
#define AL_CLI_ERROR_MALLOC_FAILED              0x102
#define AL_CLI_ERROR_INVALID_CONSOLE            0x103
#define AL_CLI_ERROR_BUF_OVERFLOW               0x104
#define AL_CLI_ERROR_CONSOLE_READ               0x105
#define AL_CLI_ERROR_CMD_NOT_EXIST              0x106
#define AL_CLI_ERROR_INVALID_CMD                0x107
#define AL_CLI_ERROR_DEV_CONSOLE_INIT_FAILED    0x108
#define AL_CLI_ERROR_UART_CONSOLE_WRITE_FAILED  0x109
#define AL_CLI_ERROR_UART_QUEUE_SEND_FAILED     0x10a
#define AL_CLI_ERROR_UART_QUEUE_RECV_FAILED     0x10b

#ifdef USE_RTOS
#ifdef RTOS_RTTHREAD
#define AL_CLI_MALLOC       rt_malloc
#define AL_CLI_CALLOC       rt_calloc
#define AL_CLI_FREE         rt_free
#define AL_CLI_MEMMOVE      rt_memmove

#define AL_CLI_PRINTF       rt_kprintf
#elif RTOS_FREERTOS

#endif
#else
#define AL_CLI_MALLOC       malloc
#define AL_CLI_CALLOC       calloc
#define AL_CLI_FREE         free

#define AL_CLI_PRINTF       printf
#define AL_CLI_MEMMOVE      memmove

#endif


#ifdef __cplusplus
}
#endif

#endif /* AL_CLI_COMMON_H */
