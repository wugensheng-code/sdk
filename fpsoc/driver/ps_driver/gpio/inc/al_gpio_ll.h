#ifndef AL_GPIO_LL_H
#define AL_GPIO_LL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "al_type.h"
#include "al_reg_io.h"
#include "al_errno.h"
#include "al_gpio_hw.h"

typedef enum
{
    GPIO_INTR_TYPE_EDGE_RISING    =	 0x00U,   
    GPIO_INTR_TYPE_EDGE_FALLING   =  0x01U,   
    GPIO_INTR_TYPE_EDGE_BOTH      =	 0x02U,   
    GPIO_INTR_TYPE_LEVEL_HIGH     =	 0x03U,   
    GPIO_INTR_TYPE_LEVEL_LOW      =	 0x04U,    
} AL_GPIO_InterruptEnum;


static inline AL_U32 AlGpio_ll_OutputRead(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + GPIO__SWPORTA__DR__BLK0__OFFSET);
}

static inline AL_U32 AlGpio_ll_InputRead(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + GPIO__EXT__PORTA__BLK0__OFFSET);
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

static inline AL_VOID AlGpio_ll_DebounceEnable(AL_REG BaseAddr, AL_U32 Debounce)
{
    AL_REG32_WRITE(BaseAddr + GPIO__DEBOUNCE__BLK0__OFFSET, Debounce);
}

static inline AL_VOID AlGpio_ll_DebounceClrEnable(AL_REG BaseAddr, AL_U32 Debounce)
{
    AL_REG32_WRITE(BaseAddr + GPIO__DEBOUNCE__CLR__BLK0__OFFSET, Debounce);
}

static inline AL_U32 AlGpio_ll_GetDebounceEnable(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + GPIO__DEBOUNCE__BLK0__OFFSET);
}

static inline AL_U32 AlGpio_ll_GetDebounceClrEnable(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + GPIO__DEBOUNCE__CLR__BLK0__OFFSET);
}

static inline AL_VOID AlGpio_ll_SyncEnable(AL_REG BaseAddr, AL_U32 Debounce)
{
    AL_REG32_WRITE(BaseAddr + GPIO__LS__SYNC__BLK0__OFFSET, Debounce);
}

static inline AL_U32 AlGpio_ll_GetSyncEnable(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + GPIO__LS__SYNC__BLK0__OFFSET);
}

static inline AL_VOID AlGpio_ll_IntrEnable(AL_REG BaseAddr, AL_U32 Enable)
{
    AL_REG32_WRITE(BaseAddr + GPIO__INTEN__BLK0__OFFSET, Enable);
}

static inline AL_VOID AlGpio_ll_IntrClrEnable(AL_REG BaseAddr, AL_U32 Disable)
{
    AL_REG32_WRITE(BaseAddr + GPIO__INTEN__CLR__BLK0__OFFSET, Disable);
}

static inline AL_VOID AlGpio_ll_IntrEoi(AL_REG BaseAddr, AL_U32 Disable)
{
    AL_REG32_WRITE(BaseAddr + GPIO__PORTA__EOI__BLK0__OFFSET, Disable);
}

static inline AL_U32 AlGpio_ll_IntrGetEnable(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + GPIO__INTEN__BLK0__OFFSET);
}

static inline AL_VOID AlGpio_ll_IntrEnableMask(AL_REG BaseAddr, AL_U32 Enable)
{
    AL_REG32_WRITE(BaseAddr + GPIO__INTMASK__BLK0__OFFSET, Enable);
}

static inline AL_U32 AlGpio_ll_IntrGetEnableMask(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + GPIO__INTMASK__BLK0__OFFSET);
}

static inline AL_VOID AlGpio_ll_IntrClrEnableMask(AL_REG BaseAddr, AL_U32 Disable)
{
    AL_REG32_WRITE(BaseAddr + GPIO__INTMASK__CLR__BLK0__OFFSET, Disable);
}

static inline AL_U32 AlGpio_ll_IntrGetStatus(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + GPIO__INTSTATUS__BLK0__OFFSET);
}

static inline AL_U32 AlGpio_ll_RawIntrGetStatus(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + GPIO__RAW__INTSTATUS_BLK0__OFFSET);
}

static inline AL_VOID AlGpio_ll_IntrClrType(AL_REG BaseAddr, AL_U32 Type)
{
    AL_REG32_WRITE(BaseAddr + GPIO__INTTYPE__LEVEL__CLR__BLK0__OFFSET, Type);
}

static inline AL_VOID AlGpio_ll_IntrClrPolarity(AL_REG BaseAddr, AL_U32 Polarity)
{
    AL_REG32_WRITE(BaseAddr + GPIO__INT__POLARITY__CLR__BLK0__OFFSET, Polarity);
}

static inline AL_VOID AlGpio_ll_IntrClrBothEdge(AL_REG BaseAddr, AL_U32 ClrEdge)
{
    AL_REG32_WRITE(BaseAddr + GPIO__INT__BOTHEDGE__CLR__BLK0__OFFSET, ClrEdge);
}

static inline AL_VOID AlGpio_ll_IntrSetType(AL_REG BaseAddr, AL_U32 Type)
{
    AL_REG32_WRITE(BaseAddr + GPIO__INTTYPE__LEVEL__BLK0__OFFSET, Type);
}

static inline AL_VOID AlGpio_ll_IntrSetPolarity(AL_REG BaseAddr, AL_U32 Polarity)
{
    AL_REG32_WRITE(BaseAddr + GPIO__INT__POLARITY__BLK0__OFFSET, Polarity);
}

static inline AL_VOID AlGpio_ll_IntrSetBothEdge(AL_REG BaseAddr, AL_U32 Edge)
{
    AL_REG32_WRITE(BaseAddr + GPIO__INT__BOTHEDGE__BLK0__OFFSET, Edge);
}

static inline AL_U32 AlGpio_ll_IntrGetType(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + GPIO__INTTYPE__LEVEL__BLK0__OFFSET);
}

static inline AL_U32 AlGpio_ll_IntrGetPolarity(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + GPIO__INT__POLARITY__BLK0__OFFSET);
}

static inline AL_U32 AlGpio_ll_IntrGetBothEdge(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + GPIO__INT__BOTHEDGE__BLK0__OFFSET);
}


#ifdef __cplusplus
}
#endif

#endif
