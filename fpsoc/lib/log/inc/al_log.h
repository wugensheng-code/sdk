#ifndef AL_LOG_H
#define AL_LOG_H

#ifdef __cplusplus
extern "C" {
#endif
#include "al_type.h"
#include "al_uart_hal.h"

#if ((LOG_DEV == AL_LOG_UART0) || (LOG_DEV == AL_LOG_UART1))
extern AL_UART_HalStruct AlLog;
#endif

#define AL_LOG_UART0 0
#define AL_LOG_UART1 1

AL_S32 AlLog_Init();

AL_U32 AlLog_Write(const void* Data, AL_U32 Len);

#ifdef __cplusplus
}
#endif

#endif
