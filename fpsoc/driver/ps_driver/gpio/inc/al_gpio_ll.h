/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */



#ifndef __AL_GPIO_LL_H_
#define __AL_GPIO_LL_H_

#ifdef __cplusplus
extern "C" {
#endif

/********************************** Include Files ********************************/
#include "al_reg_io.h"
#include "al_gpio_hw.h"

/****************************** Exported Function ******************************/

static inline AL_U32 AlGpio_ll_ReadInput(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + GPIO__EXT__PORTA__BLK0__OFFSET);
}

static inline AL_U32 AlGpio_ll_ReadOutput(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + GPIO__SWPORTA__DR__BLK0__OFFSET);
}

static inline AL_VOID AlGpio_ll_Write(AL_REG BaseAddr, AL_U32 Data)
{
    AL_REG32_WRITE(BaseAddr + GPIO__SWPORTA__DR__BLK0__OFFSET, Data);
}

static inline AL_VOID AlGpio_ll_ClrWrite(AL_REG BaseAddr, AL_U32 Data)
{
    AL_REG32_WRITE(BaseAddr + GPIO__SWPORTA__DR__CLR__BLK0__OFFSET, Data);
}

static inline AL_VOID AlGpio_ll_SetDirection(AL_REG BaseAddr, AL_U32 Direction)
{
    AL_REG32_WRITE(BaseAddr + GPIO__SWPORTA__DDR__BLK0__OFFSET, Direction);
}

static inline AL_VOID AlGpio_ll_ClrDirection(AL_REG BaseAddr, AL_U32 Direction)
{
    AL_REG32_WRITE(BaseAddr + GPIO__SWPORTA__DDR__CLR__BLK0__OFFSET, Direction);
}

static inline AL_U32 AlGpio_ll_GetDirection(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + GPIO__SWPORTA__DDR__BLK0__OFFSET);
}

static inline AL_VOID AlGpio_ll_EnableDebounce(AL_REG BaseAddr, AL_U32 Debounce)
{
    AL_REG32_WRITE(BaseAddr + GPIO__DEBOUNCE__BLK0__OFFSET, Debounce);
}

static inline AL_VOID AlGpio_ll_DisableDebounce(AL_REG BaseAddr, AL_U32 Debounce)
{
    AL_REG32_WRITE(BaseAddr + GPIO__DEBOUNCE__CLR__BLK0__OFFSET, Debounce);
}

static inline AL_U32 AlGpio_ll_GetDebounce(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + GPIO__DEBOUNCE__BLK0__OFFSET);
}

static inline AL_VOID AlGpio_ll_EnableSync(AL_REG BaseAddr, AL_U32 Debounce)
{
    AL_REG32_WRITE(BaseAddr + GPIO__LS__SYNC__BLK0__OFFSET, Debounce);
}

static inline AL_U32 AlGpio_ll_GetSync(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + GPIO__LS__SYNC__BLK0__OFFSET);
}

static inline AL_VOID AlGpio_ll_EnableIntr(AL_REG BaseAddr, AL_U32 Enable)
{
    AL_REG32_WRITE(BaseAddr + GPIO__INTEN__BLK0__OFFSET, Enable);
}

static inline AL_VOID AlGpio_ll_DisableIntr(AL_REG BaseAddr, AL_U32 Disable)
{
    AL_REG32_WRITE(BaseAddr + GPIO__INTEN__CLR__BLK0__OFFSET, Disable);
}

static inline AL_VOID AlGpio_ll_ClrIntr(AL_REG BaseAddr, AL_U32 Disable)
{
    AL_REG32_WRITE(BaseAddr + GPIO__PORTA__EOI__BLK0__OFFSET, Disable);
}

static inline AL_U32 AlGpio_ll_GetIntrEnable(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + GPIO__INTEN__BLK0__OFFSET);
}

static inline AL_VOID AlGpio_ll_EnableIntrMask(AL_REG BaseAddr, AL_U32 Enable)
{
    AL_REG32_WRITE(BaseAddr + GPIO__INTMASK__BLK0__OFFSET, Enable);
}

static inline AL_U32 AlGpio_ll_GetIntrMask(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + GPIO__INTMASK__BLK0__OFFSET);
}

static inline AL_VOID AlGpio_ll_DisableIntrMask(AL_REG BaseAddr, AL_U32 Disable)
{
    AL_REG32_WRITE(BaseAddr + GPIO__INTMASK__CLR__BLK0__OFFSET, Disable);
}

static inline AL_U32 AlGpio_ll_GetIntrStatus(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + GPIO__INTSTATUS__BLK0__OFFSET);
}

static inline AL_U32 AlGpio_ll_GetRawIntrStatus(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + GPIO__RAW__INTSTATUS_BLK0__OFFSET);
}

static inline AL_VOID AlGpio_ll_ClrIntrType(AL_REG BaseAddr, AL_U32 Type)
{
    AL_REG32_WRITE(BaseAddr + GPIO__INTTYPE__LEVEL__CLR__BLK0__OFFSET, Type);
}

static inline AL_VOID AlGpio_ll_ClrIntrPolarity(AL_REG BaseAddr, AL_U32 Polarity)
{
    AL_REG32_WRITE(BaseAddr + GPIO__INT__POLARITY__CLR__BLK0__OFFSET, Polarity);
}

static inline AL_VOID AlGpio_ll_ClrIntrBothEdge(AL_REG BaseAddr, AL_U32 ClrEdge)
{
    AL_REG32_WRITE(BaseAddr + GPIO__INT__BOTHEDGE__CLR__BLK0__OFFSET, ClrEdge);
}

static inline AL_VOID AlGpio_ll_SetIntrType(AL_REG BaseAddr, AL_U32 Type)
{
    AL_REG32_WRITE(BaseAddr + GPIO__INTTYPE__LEVEL__BLK0__OFFSET, Type);
}

static inline AL_VOID AlGpio_ll_SetIntrPolarity(AL_REG BaseAddr, AL_U32 Polarity)
{
    AL_REG32_WRITE(BaseAddr + GPIO__INT__POLARITY__BLK0__OFFSET, Polarity);
}

static inline AL_VOID AlGpio_ll_SetIntrBothEdge(AL_REG BaseAddr, AL_U32 Edge)
{
    AL_REG32_WRITE(BaseAddr + GPIO__INT__BOTHEDGE__BLK0__OFFSET, Edge);
}

static inline AL_U32 AlGpio_ll_GetIntrType(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + GPIO__INTTYPE__LEVEL__BLK0__OFFSET);
}

static inline AL_U32 AlGpio_ll_GetIntrPolarity(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + GPIO__INT__POLARITY__BLK0__OFFSET);
}

static inline AL_U32 AlGpio_ll_GetIntrBothEdge(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + GPIO__INT__BOTHEDGE__BLK0__OFFSET);
}

#ifdef __cplusplus
}
#endif

#endif
