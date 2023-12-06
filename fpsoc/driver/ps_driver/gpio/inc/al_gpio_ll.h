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

/**
 * @brief  This function reads Data from the specified Bank in EXT register.
 * @param  BaseAddr
 * @return AL_U32
 */
static inline AL_U32 AlGpio_ll_ReadInput(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + GPIO__EXT__PORTA__BLK0__OFFSET);
}

/**
 * @brief  This function reads data from the specified Bank in Data register.
 * @param  BaseAddr
 * @return AL_U32
 */
static inline AL_U32 AlGpio_ll_ReadOutput(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + GPIO__SWPORTA__DR__BLK0__OFFSET);
}

/**
 * @brief  This function writes data to the specified Bank in Data register.
 * @param  BaseAddr
 * @param  Data
 * @return AL_VOID
 */
static inline AL_VOID AlGpio_ll_Write(AL_REG BaseAddr, AL_U32 Data)
{
    AL_REG32_WRITE(BaseAddr + GPIO__SWPORTA__DR__BLK0__OFFSET, Data);
}

/**
 * @brief  This function clears data of the specified Bank in Data register.
 * @param  BaseAddr
 * @param  ClrData
 * @return AL_VOID
 */
static inline AL_VOID AlGpio_ll_ClrWrite(AL_REG BaseAddr, AL_U32 ClrData)
{
    AL_REG32_WRITE(BaseAddr + GPIO__SWPORTA__DR__CLR__BLK0__OFFSET, ClrData);
}

/**
 * @brief  This function sets the Direction from the specified Bank.
 * @param  BaseAddr
 * @param  Direction
 * @return AL_VOID
 */
static inline AL_VOID AlGpio_ll_SetDirection(AL_REG BaseAddr, AL_U32 Direction)
{
    AL_REG32_WRITE(BaseAddr + GPIO__SWPORTA__DDR__BLK0__OFFSET, Direction);
}

/**
 * @brief  This function clears the Direction from the specified Bank.
 * @param  BaseAddr
 * @param  ClrDirection
 * @return AL_VOID
 */
static inline AL_VOID AlGpio_ll_ClrDirection(AL_REG BaseAddr, AL_U32 ClrDirection)
{
    AL_REG32_WRITE(BaseAddr + GPIO__SWPORTA__DDR__CLR__BLK0__OFFSET, ClrDirection);
}

/**
 * @brief  This function gets the Direction from the specified Bank.
 * @param  BaseAddr
 * @return AL_U32
 */
static inline AL_U32 AlGpio_ll_GetDirection(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + GPIO__SWPORTA__DDR__BLK0__OFFSET);
}

/**
 * @brief  This function controls whether an external signal that is the source of an interrupt needs to be debounced to remove
 * any spurious glitches.
 * @param  BaseAddr
 * @param  Debounce
 * @return AL_VOID
 */
static inline AL_VOID AlGpio_ll_EnableDebounce(AL_REG BaseAddr, AL_U32 Debounce)
{
    AL_REG32_WRITE(BaseAddr + GPIO__DEBOUNCE__BLK0__OFFSET, Debounce);
}

/**
 * @brief  This function clears an external signal that is the source of an interrupt needs to be debounced to remove
 * any spurious glitches.
 * @param  BaseAddr
 * @param  ClrDebounce
 * @return AL_VOID
 */
static inline AL_VOID AlGpio_ll_DisableDebounce(AL_REG BaseAddr, AL_U32 ClrDebounce)
{
    AL_REG32_WRITE(BaseAddr + GPIO__DEBOUNCE__CLR__BLK0__OFFSET, ClrDebounce);
}

/**
 * @brief  This function returns whether an external signal that is the source of an interrupt needs to be debounced to remove
 * any spurious glitches.
 * @param  BaseAddr
 * @return AL_U32
 */
static inline AL_U32 AlGpio_ll_GetDebounce(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + GPIO__DEBOUNCE__BLK0__OFFSET);
}

/**
 * @brief  This function controls whether all level-sensitive interrupts being synchronized to pclk_intr.
 * @param  BaseAddr
 * @param  Value
 * @return AL_VOID
 */
static inline AL_VOID AlGpio_ll_EnableSync(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + GPIO__LS__SYNC__BLK0__OFFSET, Value);
}

/**
 * @brief  This function gets whether all level-sensitive interrupts being synchronized to pclk_intr.
 * @param  BaseAddr
 * @return AL_U32
 */
static inline AL_U32 AlGpio_ll_GetSync(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + GPIO__LS__SYNC__BLK0__OFFSET);
}

/**
 * @brief  This function enables the interrupt for the specified bank.
 * @param  BaseAddr
 * @param  Enable
 * @return AL_VOID
 */
static inline AL_VOID AlGpio_ll_EnableIntr(AL_REG BaseAddr, AL_U32 Enable)
{
    AL_REG32_WRITE(BaseAddr + GPIO__INTEN__BLK0__OFFSET, Enable);
}

/**
 * @brief  This function clears the interrupt for the specified bank.
 * @param  BaseAddr
 * @param  Disable
 * @return AL_VOID
 */
static inline AL_VOID AlGpio_ll_DisableIntr(AL_REG BaseAddr, AL_U32 Disable)
{
    AL_REG32_WRITE(BaseAddr + GPIO__INTEN__CLR__BLK0__OFFSET, Disable);
}

/**
 * @brief  This function disables the interrupt for the specified bank.
 * @param  BaseAddr
 * @param  Disable
 * @return AL_VOID
 */
static inline AL_VOID AlGpio_ll_ClrIntr(AL_REG BaseAddr, AL_U32 Disable)
{
    AL_REG32_WRITE(BaseAddr + GPIO__PORTA__EOI__BLK0__OFFSET, Disable);
}

/**
 * @brief  This function gets the whether the interrupt for the specified bank is enabled.
 * @param  BaseAddr
 * @param  Disable
 * @return AL_VOID
 */
static inline AL_U32 AlGpio_ll_GetIntrEnable(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + GPIO__INTEN__BLK0__OFFSET);
}

/**
 * @brief  This function masks interrupt for the specific bank.
 * @param  BaseAddr
 * @param  Enable
 * @return AL_VOID
 */
static inline AL_VOID AlGpio_ll_EnableIntrMask(AL_REG BaseAddr, AL_U32 Enable)
{
    AL_REG32_WRITE(BaseAddr + GPIO__INTMASK__BLK0__OFFSET, Enable);
}

/**
 * @brief  This function gets whether the interrupt for the specific bank is masked.
 * @param  BaseAddr
 * @return AL_VOID
 */
static inline AL_U32 AlGpio_ll_GetIntrMask(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + GPIO__INTMASK__BLK0__OFFSET);
}

/**
 * @brief  This function clears the function of interrupt mask for the specific bank.
 * @param  BaseAddr
 * @param  Disable
 * @return AL_VOID
 */
static inline AL_VOID AlGpio_ll_DisableIntrMask(AL_REG BaseAddr, AL_U32 Disable)
{
    AL_REG32_WRITE(BaseAddr + GPIO__INTMASK__CLR__BLK0__OFFSET, Disable);
}

/**
 * @brief  This function gets the raw interrupt status for the specific bank.
 * @param  BaseAddr
 * @return AL_U32
 */
static inline AL_U32 AlGpio_ll_GetIntrStatus(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + GPIO__INTSTATUS__BLK0__OFFSET);
}

/**
 * @brief  This function gets the raw interrupt status for the specific bank.
 * @param  BaseAddr
 * @return AL_U32
 */
static inline AL_U32 AlGpio_ll_GetRawIntrStatus(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + GPIO__RAW__INTSTATUS_BLK0__OFFSET);
}

/**
 * @brief  This function clears the set of interrupt type for the specific bank.
 * @param  BaseAddr
 * @param  ClrType
 * @return AL_VOID
 */
static inline AL_VOID AlGpio_ll_ClrIntrType(AL_REG BaseAddr, AL_U32 ClrType)
{
    AL_REG32_WRITE(BaseAddr + GPIO__INTTYPE__LEVEL__CLR__BLK0__OFFSET, ClrType);
}

/**
 * @brief  This function clears the set of interrupt polarity for the specific bank.
 * @param  BaseAddr
 * @param  ClrPolarity
 * @return AL_VOID
 */
static inline AL_VOID AlGpio_ll_ClrIntrPolarity(AL_REG BaseAddr, AL_U32 ClrPolarity)
{
    AL_REG32_WRITE(BaseAddr + GPIO__INT__POLARITY__CLR__BLK0__OFFSET, ClrPolarity);
}

/**
 * @brief  This function clears the set of interrupt edge type for the specific bank.
 * @param  BaseAddr
 * @param  ClrEdge
 * @return AL_VOID
 */
static inline AL_VOID AlGpio_ll_ClrIntrBothEdge(AL_REG BaseAddr, AL_U32 ClrEdge)
{
    AL_REG32_WRITE(BaseAddr + GPIO__INT__BOTHEDGE__CLR__BLK0__OFFSET, ClrEdge);
}

/**
 * @brief  This function sets the interrupt type for the specific bank.
 * @param  BaseAddr
 * @param  Type
 * @return AL_VOID
 */
static inline AL_VOID AlGpio_ll_SetIntrType(AL_REG BaseAddr, AL_U32 Type)
{
    AL_REG32_WRITE(BaseAddr + GPIO__INTTYPE__LEVEL__BLK0__OFFSET, Type);
}

/**
 * @brief  This function sets the interrupt priority for the specific bank.
 * @param  BaseAddr
 * @param  Priority
 * @return AL_VOID
 */
static inline AL_VOID AlGpio_ll_SetIntrPolarity(AL_REG BaseAddr, AL_U32 Polarity)
{
    AL_REG32_WRITE(BaseAddr + GPIO__INT__POLARITY__BLK0__OFFSET, Polarity);
}

/**
 * @brief  This function sets the interrupt edge type for the specific bank.
 * @param  BaseAddr
 * @param  Priority
 * @return AL_VOID
 */
static inline AL_VOID AlGpio_ll_SetIntrBothEdge(AL_REG BaseAddr, AL_U32 Edge)
{
    AL_REG32_WRITE(BaseAddr + GPIO__INT__BOTHEDGE__BLK0__OFFSET, Edge);
}

/**
 * @brief  This function gets the interrupt type for the specific bank.
 * @param  BaseAddr
 * @return AL_U32
 */
static inline AL_U32 AlGpio_ll_GetIntrType(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + GPIO__INTTYPE__LEVEL__BLK0__OFFSET);
}

/**
 * @brief  This function gets the interrupt polarity for the specific bank.
 * @param  BaseAddr
 * @return AL_U32
 */
static inline AL_U32 AlGpio_ll_GetIntrPolarity(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + GPIO__INT__POLARITY__BLK0__OFFSET);
}

/**
 * @brief  This function gets the interrupt edge type for the specific bank.
 * @param  BaseAddr
 * @return AL_U32
 */
static inline AL_U32 AlGpio_ll_GetIntrBothEdge(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + GPIO__INT__BOTHEDGE__BLK0__OFFSET);
}

#ifdef __cplusplus
}
#endif

#endif
