#ifndef AL_LOG_H
#define AL_LOG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "al_uart_hal.h"

AL_S32 AlLog_Init();

AL_S32 AlLog_Write(const void* Data, size_t len);

#ifdef __cplusplus
}
#endif

#endif