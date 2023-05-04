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


static inline AL_U32 AlGpio_ll_OutputRead(AL_REG32 BaseAddr)
{
    return READ_REG(BaseAddr + GPIO__SWPORTA__DR__BLK0__OFFSET);
}

static inline AL_U32 AlGpio_ll_InputRead(AL_REG32 BaseAddr)
{
    return READ_REG(BaseAddr + GPIO__EXT__PORTA__BLK0__OFFSET);
}

static inline AL_VOID AlGpio_ll_Write(AL_REG32 BaseAddr, AL_U32 Data)
{
    WRITE_REG(BaseAddr + GPIO__SWPORTA__DR__BLK0__OFFSET, Data);
}

static inline AL_VOID AlGpio_ll_ClrWrite(AL_REG32 BaseAddr, AL_U32 Data)
{
    WRITE_REG(BaseAddr + GPIO__SWPORTA__DR__CLR__BLK0__OFFSET, Data);
}

static inline AL_VOID AlGpio_ll_SetDirection(AL_REG32 BaseAddr, AL_U32 Direction)
{
    WRITE_REG(BaseAddr + GPIO__SWPORTA__DDR__BLK0__OFFSET, Direction);
}

static inline AL_VOID AlGpio_ll_ClrDirection(AL_REG32 BaseAddr, AL_U32 Direction)
{
    WRITE_REG(BaseAddr + GPIO__SWPORTA__DDR__CLR__BLK0__OFFSET, Direction);
}

static inline AL_U32 AlGpio_ll_GetDirection(AL_REG32 BaseAddr)
{
    return READ_REG(BaseAddr + GPIO__SWPORTA__DDR__BLK0__OFFSET);
}

static inline AL_VOID AlGpio_ll_DebounceEnable(AL_REG32 BaseAddr, AL_U32 Debounce)
{
    WRITE_REG(BaseAddr + GPIO__DEBOUNCE__BLK0__OFFSET, Debounce);
}

static inline AL_VOID AlGpio_ll_DebounceClearEnable(AL_REG32 BaseAddr, AL_U32 Debounce)
{
    WRITE_REG(BaseAddr + GPIO__DEBOUNCE__CLR__BLK0__OFFSET, Debounce);
}

static inline AL_U32 AlGpio_ll_GetDebounceEnable(AL_REG32 BaseAddr)
{
    return READ_REG(BaseAddr + GPIO__DEBOUNCE__BLK0__OFFSET);
}

static inline AL_U32 AlGpio_ll_GetDebounceClearEnable(AL_REG32 BaseAddr)
{
    return READ_REG(BaseAddr + GPIO__DEBOUNCE__CLR__BLK0__OFFSET);
}

static inline AL_VOID AlGpio_ll_SyncEnable(AL_REG32 BaseAddr, AL_U32 Debounce)
{
    WRITE_REG(BaseAddr + GPIO__LS__SYNC__BLK0__OFFSET, Debounce);
}

static inline AL_U32 AlGpio_ll_GetSyncEnable(AL_REG32 BaseAddr)
{
    return READ_REG(BaseAddr + GPIO__LS__SYNC__BLK0__OFFSET);
}

static inline AL_VOID AlGpio_ll_IntrEnable(AL_REG32 BaseAddr, AL_U32 Enable)
{
    WRITE_REG(BaseAddr + GPIO__INTEN__BLK0__OFFSET, Enable);
}

static inline AL_VOID AlGpio_ll_IntrClearEnable(AL_REG32 BaseAddr, AL_U32 Disable)
{
    WRITE_REG(BaseAddr + GPIO__INTEN__CLR__BLK0__OFFSET, Disable);
}

static inline AL_VOID AlGpio_ll_IntrClear(AL_REG32 BaseAddr, AL_U32 Disable)
{
    WRITE_REG(BaseAddr + GPIO__PORTA__EOI__BLK0__OFFSET, Disable);
}

static inline AL_U32 AlGpio_ll_IntrGetEnable(AL_REG32 BaseAddr)
{
    return READ_REG(BaseAddr + GPIO__INTEN__BLK0__OFFSET);
}

static inline AL_VOID AlGpio_ll_IntrEnableMask(AL_REG32 BaseAddr, AL_U32 Enable)
{
    WRITE_REG(BaseAddr + GPIO__INTMASK__BLK0__OFFSET, Enable);
}

static inline AL_U32 AlGpio_ll_IntrGetEnableMask(AL_REG32 BaseAddr)
{
    return READ_REG(BaseAddr + GPIO__INTMASK__BLK0__OFFSET);
}

static inline AL_VOID AlGpio_ll_IntrClearEnableMask(AL_REG32 BaseAddr, AL_U32 Disable)
{
    WRITE_REG(BaseAddr + GPIO__INTMASK__CLR__BLK0__OFFSET, Disable);
}

static inline AL_U32 AlGpio_ll_IntrGetStatus(AL_REG32 BaseAddr)
{
    return READ_REG(BaseAddr + GPIO__INTSTATUS__BLK0__OFFSET);
}

static inline AL_U32 AlGpio_ll_RawIntrGetStatus(AL_REG32 BaseAddr)
{
    return READ_REG(BaseAddr + GPIO__RAW__INTSTATUS_BLK0__OFFSET);
}

static inline AL_VOID AlGpio_ll_IntrClearType(AL_REG32 BaseAddr, AL_U32 Type)
{
    WRITE_REG(BaseAddr + GPIO__INTTYPE__LEVEL__CLR__BLK0__OFFSET, Type);
}

static inline AL_VOID AlGpio_ll_IntrClearPolarity(AL_REG32 BaseAddr, AL_U32 Polarity)
{
    WRITE_REG(BaseAddr + GPIO__INT__POLARITY__CLR__BLK0__OFFSET, Polarity);
}

static inline AL_VOID AlGpio_ll_IntrClearBothEdge(AL_REG32 BaseAddr, AL_U32 ClearEdge)
{
    WRITE_REG(BaseAddr + GPIO__INT__BOTHEDGE__CLR__BLK0__OFFSET, ClearEdge);
}

static inline AL_VOID AlGpio_ll_IntrSetType(AL_REG32 BaseAddr, AL_U32 Type)
{
    WRITE_REG(BaseAddr + GPIO__INTTYPE__LEVEL__BLK0__OFFSET, Type);
}

static inline AL_VOID AlGpio_ll_IntrSetPolarity(AL_REG32 BaseAddr, AL_U32 Polarity)
{
    WRITE_REG(BaseAddr + GPIO__INT__POLARITY__BLK0__OFFSET, Polarity);
}

static inline AL_VOID AlGpio_ll_IntrSetBothEdge(AL_REG32 BaseAddr, AL_U32 Edge)
{
    WRITE_REG(BaseAddr + GPIO__INT__BOTHEDGE__BLK0__OFFSET, Edge);
}

static inline AL_U32 AlGpio_ll_IntrGetType(AL_REG32 BaseAddr)
{
    return READ_REG(BaseAddr + GPIO__INTTYPE__LEVEL__BLK0__OFFSET);
}

static inline AL_U32 AlGpio_ll_IntrGetPolarity(AL_REG32 BaseAddr)
{
    return READ_REG(BaseAddr + GPIO__INT__POLARITY__BLK0__OFFSET);
}

static inline AL_U32 AlGpio_ll_IntrGetBothEdge(AL_REG32 BaseAddr)
{
    return READ_REG(BaseAddr + GPIO__INT__BOTHEDGE__BLK0__OFFSET);
}


#ifdef __cplusplus
}
#endif

#endif
