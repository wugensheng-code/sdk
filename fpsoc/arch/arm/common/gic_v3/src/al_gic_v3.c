#include "gic_v3.h"
#include "gic_v3_addr.h"
#include "al_type.h"
#include "al_errno.h"
#include "al_intr.h"
#include "al_reg_io.h"

AL_S32 AlIntr_RegHandler(AL_S32 IntrId, AL_INTR_AttrStrct *IntrAttr, AL_INTR_Func *Func, AL_VOID *Param)
{
	/*
	 * RPU SPI interrupt IRQn to Apu SPI IRQn mapping from SPI_START_ID_SHARE_BETWEEN_APU_RPU:
	 * APU_SPI_IDX = (RPU_SPI_IDX + SPI_OFFSET_APU_TO_RPU)
	*/
    #define SPI_ID_OFFSET_APU_TO_RPU		(13)
    AL_INTR_AttrStrct *Attr;
    AL_DEFAULT_ATTR(DefAttr);

    Attr = (IntrAttr != AL_NULL) ? IntrAttr : &DefAttr;

    if (Attr->TrigMode == 0) {
        uint32_t *addr  = GICD_ICFGR + ((IntrId + SPI_ID_OFFSET_APU_TO_RPU) / 16) * 4;
        uint32_t mask   = *(uint32_t *)addr;
        uint32_t offset = (((IntrId + SPI_ID_OFFSET_APU_TO_RPU) % 16) << 1);
        mask &= ~(0x3 << offset);
        mask |= 0x2 << offset;
        AL_REG32_WRITE(addr, mask);
    }

#ifndef SWITCH_TO_EL1_EL0_FROM_EL3
    request_fiq(IntrId + SPI_ID_OFFSET_APU_TO_RPU, Func, Param);
    printf("currentel is el3, request fiq!\r\n");
#else
    request_irq(IntrId + SPI_ID_OFFSET_APU_TO_RPU, Func, Param);
    printf("currentel is not el3, request irq!\r\n");
#endif

	return 0;
}


AL_S32 AlIntr_SetInterrupt(AL_U32 IntrId, AL_FUNCTION state)
{
    AL_S32 Ret = AL_OK;

    switch (state) {
    case AL_FUNC_DISABLE:
        gicv3_disable_irq(IntrId);
        Ret = AL_OK;
        break;

    case AL_FUNC_ENABLE:
        gicv3_enable_irq(IntrId);
        Ret = AL_OK;
        break;

    default:
        Ret = AL_INTR_ERR_ILLEGAL_PARAM;
        break;
    }

    return Ret;
}

AL_S32 Al_Intr_SetGlobalInterrupt(AL_FUNCTION state)
{
    AL_S32 Ret = AL_OK;

    switch (state) {
    case AL_FUNC_DISABLE:
        __disable_irq();
        Ret = AL_OK;
        break;

    case AL_FUNC_ENABLE:
        __enable_irq();
        break;

    default:
        Ret = AL_INTR_ERR_ILLEGAL_PARAM;
        break;
    }

    return Ret;
}
