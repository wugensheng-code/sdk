/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_core.h"
#include "al_gicv3_private.h"
#include "al_gicv3_dist.h"
#include "al_gicv3_rdist.h"

#define GICV3_SPECIAL_START     (1020)
#define GICV3_SPECIAL_END       (1023)
#define GICV3_SPECIAL_NUM       (GICV3_SPECIAL_END - GICV3_SPECIAL_START +1)

AL_INTR_HandlerStruct irq_handler_list[SOC_INT_MAX + GICV3_SPECIAL_NUM];

/**
 * @desc  : irq handle implement
 * @flow  : read irq number -> deal with this irq event ->     write eoi
 */
AL_VOID do_irq_handle(AL_VOID)
{
    AL_U32 IntrId;
    AL_INTR_HandlerStruct Handler;
    IntrId = AlGicv3_AckIntrSel1();

    enable_irq();

    if (IntrId < SOC_INT_MAX) {
        Handler = irq_handler_list[IntrId];
    } else {
        Handler = irq_handler_list[IntrId - GICV3_SPECIAL_START + SOC_INT_MAX];
    }

    if (Handler.Func == NULL) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "can not found your irq handle at number: %d\r\n", IntrId);
    } else {
        Handler.Func(Handler.Param);
    }

    disable_irq();

    AlGicv3_EndOfIntrSel1(IntrId);
}

/**
 * @desc  : irq handle implement; this is weak function because we need to read ICC_IAR0_EL1 if group0 case
 * @flow  : read irq number -> deal with this irq event ->     write eoi
 */
AL_VOID do_fiq_handle(AL_VOID)
{
    AL_U32 IntrId;
    AL_INTR_HandlerStruct Handler;

    IntrId = AlGicv3_AckIntrSel1();
    enable_fiq();

    if (IntrId < SOC_INT_MAX) {
        Handler = irq_handler_list[IntrId];
    } else {
        Handler = irq_handler_list[IntrId - GICV3_SPECIAL_START + SOC_INT_MAX];
    }

    if (Handler.Func == NULL) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "can not found your fiq handle at number: %d\n", IntrId);
    } else {
        Handler.Func(Handler.Param);
    }

    disable_fiq();
    AlGicv3_EndOfIntrSel1(IntrId);
}

static AL_VOID AlIntr_RequestIntr(AL_U32 IntrId, AL_VOID *Handler, AL_VOID *Param)
{
    AL_INTR_HandlerStruct *HandleArray;

    HandleArray = irq_handler_list;

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

    /* > SOC_INT_MAX && (not in the range of special range) */
    if (IntrId >= SOC_INT_MAX && (IntrId < GICV3_SPECIAL_START || IntrId > GICV3_SPECIAL_END)) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "init id error IntrId = %d\n", IntrId);
        return AL_ERR_ILLEGAL_PARAM;
    }

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

#ifdef ARM_CORE_SLAVE
    if (IS_SPI(IntrId)) {
#ifdef SUPPORT_NONSECURE
        AlGicv3_SetInterruptType(IntrId, CpuId, INTR_GROUP1NS);
#else
        AlGicv3_SetInterruptType(IntrId, CpuId, INTR_GROUP1S);
#endif
        AlGicv3_SetSpiRouting(IntrId, GICV3_IRM_PE, CpuId);
    }
#endif

    AlIntr_RequestIntr(IntrId, Func, Param);

    return AL_OK;
}


AL_S32 AlIntr_SetInterrupt(AL_U32 IntrId, AL_FUNCTION State)
{
    AL_S32 Ret = AL_OK;

    switch (State) {
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

AL_S32 AlIntr_SetLocalInterrupt(AL_FUNCTION State)
{
    AL_S32 Ret = AL_OK;

    switch (State) {
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

AL_VOID AlIntr_RestoreLocalInterruptMask(AL_S32 Mask)
{
    set_intr_mask(Mask);
}

AL_S32 AlIntr_SaveLocalInterruptMask(AL_VOID)
{
#if defined(__aarch64__)
    AL_U32 ExceptionState ;
    AL_U32 EXCEPTION_ALL = (DAIF_IRQ_BIT | DAIF_FIQ_BIT) << 6;
    ExceptionState = get_intr_mask();
    ExceptionState = ExceptionState & EXCEPTION_ALL;
    if (ExceptionState != EXCEPTION_ALL) {
        (AL_U32)AlIntr_SetLocalInterrupt(AL_FUNC_DISABLE);
        return ExceptionState;
    }
    return ExceptionState;
#endif
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
}


AL_S32 AlIntr_SetPreemptionBitsCount(AL_U32 Bits)
{
    GICV3_SYSREG_WRITE(icc_bpr1_el1, Bits);
}

AL_VOID AlIntr_SetPriorityMask(AL_U32 Mask)
{
    GICV3_SYSREG_WRITE(icc_pmr_el1, Mask);
}

AL_U32 AlIntr_GetPriorityMask(AL_VOID)
{
    return (GICV3_SYSREG_READ(icc_pmr_el1) & 0xFF);
}

AL_VOID AlIntr_GenSoftIntr(AL_U32 IntrNum, AL_U64 CpuId)
{
    AlGicv3_RaiseSgi(IntrNum, AL_GICV3_SAME, CpuId);
}

AL_VOID AlIntr_GenNonSecSoftIntr(AL_U32 IntrNum, AL_U64 CpuId)
{
    AlGicv3_RaiseSgi(IntrNum, AL_GICV3_G1NS, CpuId);
}