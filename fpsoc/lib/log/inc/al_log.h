#ifndef AL_LOG_H
#define AL_LOG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "al_uart_hal.h"

typedef enum {
	AL_LOG_UART0,
	AL_LOG_UART1,
	AL_LOG_MAX_DEV,
} AL_LOG_DEV;

AL_S32 AlLog_Init();

AL_U32 AlLog_Write(const void* Data, AL_U32 Len);

#ifdef __cplusplus
}
#endif

#endif