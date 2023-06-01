#include "gic_v3.h"
#include "gic_v3_addr.h"
#include "al_core.h"
#include "al_errno.h"
#include "al_intr.h"
#include "al_reg_io.h"

AL_S32 AlIntr_RegHandler(AL_S32 IntrId, AL_INTR_AttrStrct *IntrAttr, AL_INTR_Func Func, AL_VOID *Param)
{
    AL_INTR_AttrStrct *Attr;
    AL_DEFAULT_ATTR(DefAttr);

    Attr = (IntrAttr != AL_NULL) ? IntrAttr : &DefAttr;

    if (Attr->TrigMode == 0) {
        uint32_t *addr  = GICD_ICFGR + ((IntrId) / 16) * 4;
        uint32_t mask   = *(uint32_t *)addr;
        uint32_t offset = (((IntrId) % 16) << 1);
        mask &= ~(0x3 << offset);
        mask |= 0x2 << offset;
        AL_REG32_WRITE(addr, mask);
    }

#ifndef SWITCH_TO_EL1_EL0_FROM_EL3
    request_fiq(IntrId + SPI_ID_OFFSET_APU_TO_RPU, Func, Param);
    printf("currentel is el3, request fiq!\r\n");
#else
    request_irq(IntrId, Func, Param);
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

AL_S32 AlIntr_SetGlobalInterrupt(AL_FUNCTION state)
{
    AL_S32 Ret = AL_OK;

    switch (state) {
    case AL_FUNC_DISABLE:
        irq_disable();
        Ret = AL_OK;
        break;

    case AL_FUNC_ENABLE:
        irq_enable();
        break;

    default:
        Ret = AL_INTR_ERR_ILLEGAL_PARAM;
        break;
    }

    return Ret;
}


AL_VOID AlIntr_ClearAllPending(AL_VOID)
{
    AL_REG32_WRITE(GICD_ICENABLER,  ~0);
	AL_REG32_WRITE(GICD_ICPENDR,    ~0);
	AL_REG32_WRITE(GICD_ACTIVE_CLEAR, ~0);

	AL_REG32_WRITE(GICD_ICENABLER+4, ~0);
	AL_REG32_WRITE(GICD_ICPENDR+4, ~0);
	AL_REG32_WRITE(GICD_ACTIVE_CLEAR+4, ~0);

	AL_REG32_WRITE(GICD_ICENABLER+8, ~0);
	AL_REG32_WRITE(GICD_ICPENDR+8, ~0);
	AL_REG32_WRITE(GICD_ACTIVE_CLEAR+8, ~0);

	AL_REG32_WRITE(GICD_ICENABLER+12, ~0);
	AL_REG32_WRITE(GICD_ICPENDR+12, ~0);
	AL_REG32_WRITE(GICD_ACTIVE_CLEAR+12, ~0);

	AL_REG32_WRITE(GICD_ICENABLER+16, ~0);
	AL_REG32_WRITE(GICD_ICPENDR+16, ~0);
	AL_REG32_WRITE(GICD_ACTIVE_CLEAR+16, ~0);

	AL_REG32_WRITE(GICD_ICENABLER+20, ~0);
	AL_REG32_WRITE(GICD_ICPENDR+20, ~0);
	AL_REG32_WRITE(GICD_ACTIVE_CLEAR+20, ~0);

	return;
}
