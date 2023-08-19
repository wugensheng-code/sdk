#include "al_core.h"
#include "al_errno.h"
#include "al_intr.h"
#include "al_reg_io.h"
#include "gicv3_private.h"
#include "gicv3_dist.h"
#include "gicv3_rdist.h"
#include "arch.h"


#ifdef CONFIG_PRINTK
#define gic_print(...) printf(__VA_ARGS__)
#else
#define gic_print(...)
#endif

#define GICV3_SPECIAL_START				(1020)
#define GICV3_SPECIAL_END				(1023)

#define GICV3_SPECIAL_NUM				(GICV3_SPECIAL_END - GICV3_SPECIAL_START +1)

static AL_INTR_HandlerStruct irq_handler_list[SOC_INT_MAX + GICV3_SPECIAL_NUM];
static AL_INTR_HandlerStruct fiq_handler_list[SOC_INT_MAX + GICV3_SPECIAL_NUM];

/**
 * @desc  : irq handle implement
 * @flow  : read irq number -> deal with this irq event -> 	write eoi
 */
AL_VOID do_irq_handle(AL_VOID)
{
	u32	int_id;
	AL_INTR_HandlerStruct Handler;

#if 0
	/* enable sre */
	if (!gic_enable_sre()) {
		/* interrupr acknowledge by read gicc_iar*/
		int_id = readl_relaxed(GICC_IAR) & 0xffffff;
	} else
#endif
	{
		/* interrupr acknowledge by read iar*/
        int_id = read_icc_iar1_el1();
        dsbsy();
		int_id = int_id & 0xffffff;
	}

	if (int_id < SOC_INT_MAX) {
		Handler = irq_handler_list[int_id];
	} else {
		Handler = irq_handler_list[int_id -GICV3_SPECIAL_START + SOC_INT_MAX];
	}

	if (Handler.Func == NULL) {
		gic_print("can not found your irq handle at number: %d\n", int_id);
	} else {
		Handler.Func(Handler.Param);
	}

#if 0
	/* write end of interrupt to deactivate the interrupt */
	if (!gic_enable_sre()) {
		writel_relaxed(int_id, GICC_EOIR);
	} else
#endif
	{
        write_icc_eoir1_el1(int_id);
        isb();
	}
}

/**
 * @desc  : irq handle implement; this is weak function because we need to read ICC_IAR0_EL1 if group0 case
 * @flow  : read irq number -> deal with this irq event -> 	write eoi
 */
AL_VOID do_fiq_handle(AL_VOID)
{
	u32	int_id;
	AL_INTR_HandlerStruct Handler;

	int_id = read_icc_iar1_el1();
    dsbsy();
    int_id = int_id & 0xffffff;

	if (int_id < SOC_INT_MAX) {
		Handler = fiq_handler_list[int_id];
	} else {
		Handler = fiq_handler_list[int_id -GICV3_SPECIAL_START + SOC_INT_MAX];
	}

	if (Handler.Func == NULL) {
		gic_print("can not found your fiq handle at number: %d\n", int_id);
	} else {
		Handler.Func(Handler.Param);
	}

	/* write end of interrupt to deactivate the interrupt */
	if (read_icc_sre_el1() & ICC_SRE_SRE_BIT) {
        write_icc_eoir1_el1(int_id);
        isb();
	} else {
		write_icc_eoir1_el1(int_id);
        isb();
	}
}

static AL_VOID AlIntr_RequestIntr(AL_U32 IntrId, AL_VOID *Handler, AL_VOID *Param)
{
    AL_INTR_HandlerStruct *HandleArray;
    /*
	 * > SOC_INT_MAX && (not in the range of special range)
	*/
	if (IntrId >= SOC_INT_MAX && (IntrId < GICV3_SPECIAL_START || IntrId > GICV3_SPECIAL_END)) {
		AL_LOG(AL_LOG_LEVEL_ERROR, "init id error IntrId = %d\n", IntrId);
		return;
	}

#ifdef SWITCH_TO_EL1_EL0_FROM_EL3
    HandleArray = irq_handler_list;
#else
    HandleArray = fiq_handler_list;
#endif

	if (IntrId < SOC_INT_MAX) {
		HandleArray[IntrId].Func    = Handler;
		HandleArray[IntrId].Param   = Param;
		AlGicv3_EnableInterrupt(IntrId, *(Gicv3DrvData->CpuId));
	} else {
		HandleArray[IntrId - GICV3_SPECIAL_START + SOC_INT_MAX].Func = Handler;
		HandleArray[IntrId - GICV3_SPECIAL_START + SOC_INT_MAX].Param = Param;
	}
}

AL_S32 AlIntr_RegHandler(AL_S32 IntrId, AL_INTR_AttrStrct *IntrAttr, AL_INTR_Func Func, AL_VOID *Param)
{
    AL_U32 CpuId = *(Gicv3DrvData->CpuId);
    AL_INTR_AttrStrct *Attr;
    AL_DEFAULT_ATTR(DefAttr);

    Attr = (IntrAttr != AL_NULL) ? IntrAttr : &DefAttr;

    if ((Attr->TrigMode != LEVEL_HIGH_TRIGGER) && (Attr->TrigMode != POSTIVE_EDGE_TRIGGER)) {
        AL_LOG(AL_LOG_LEVEL_WARNING, "Gic only support high level or rising edge trigger\r\n");
    }

    if (Attr->TrigMode != DOUBLE_EDGE_TRIGGER) {
        AlGicv3_SetInterruptTriggerMode(IntrId, CpuId, Attr->TrigMode);
    } else {
        AlGicv3_SetInterruptTriggerMode(IntrId, CpuId, POSTIVE_EDGE_TRIGGER);
        AL_LOG(AL_LOG_LEVEL_WARNING, "Gic switch double edge trigger to rising edge trigger\r\n");
    }

    AlGicv3_SetInterruptPriority(IntrId, CpuId, Attr->Priority);

    AlIntr_RequestIntr(IntrId, Func, Param);

	return 0;
}


AL_S32 AlIntr_SetInterrupt(AL_U32 IntrId, AL_FUNCTION state)
{
    AL_S32 Ret = AL_OK;

    switch (state) {
    case AL_FUNC_DISABLE:
        AlGicv3_DisableInterrupt(IntrId, *(Gicv3DrvData->CpuId));
        Ret = AL_OK;
        break;

    case AL_FUNC_ENABLE:
        AlGicv3_EnableInterrupt(IntrId, *(Gicv3DrvData->CpuId));
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
        disable_all_intr();
        Ret = AL_OK;
        break;

    case AL_FUNC_ENABLE:
        enable_all_intr();
        break;

    default:
        Ret = AL_INTR_ERR_ILLEGAL_PARAM;
        break;
    }

    return Ret;
}


AL_VOID AlIntr_ClearAllPending(AL_VOID)
{
    AL_U32 ClearVal = 0xFFFFFFFF;
    AL_U32 NumSpi = AlGicv3_GetSpiLimit(Gicv3DrvData->GicdBase);

    for (AL_U32 Id = MIN_SPI_ID; Id < NumSpi; Id += (1U << ICENABLER_SHIFT)) {
        Gicd_WriteIcenabler(Gicv3DrvData->GicdBase, Id, ClearVal);
        Gicd_WriteIcpendr(Gicv3DrvData->GicdBase, Id, ClearVal);
        Gicd_WriteIcactiver(Gicv3DrvData->GicdBase, Id, ClearVal);
    }

    Gicr_WriteIcpendr0(Gicv3DrvData->RdistBaseAddrs[*(Gicv3DrvData->CpuId)], ClearVal);

	return;
}
