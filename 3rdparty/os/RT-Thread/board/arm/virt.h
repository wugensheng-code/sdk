/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-17     GuEe-GUI     the first version
 */

#ifndef VIRT_H__
#define VIRT_H__

#include <rtdef.h>
#include <al_chip.h>

/* GIC */
#define MAX_HANDLERS        96
#define GIC_IRQ_START       0
#define ARM_GIC_NR_IRQS     96
#define ARM_GIC_MAX_NR      1

#define IRQ_ARM_IPI_KICK    0
#define IRQ_ARM_IPI_CALL    1

/* GICv2 */
#define GIC_PL390_DISTRIBUTOR_PPTR      0x08000000
#define GIC_PL390_CONTROLLER_PPTR       0x08010000

/* GICv3 */
#define GIC_PL500_DISTRIBUTOR_PPTR      AL_DR1M90_GICD_BASE
#define GIC_PL500_REDISTRIBUTOR_PPTR    AL_DR1M90_GICR_BASE
#define GIC_PL500_CONTROLLER_PPTR       0xDD000000
// #define GIC_PL500_ITS_PPTR              0x08080000

/* the basic constants and interfaces needed by gic */
rt_inline rt_uint32_t platform_get_gic_dist_base(void)
{
#ifdef BSP_USING_GICV2
    return GIC_PL390_DISTRIBUTOR_PPTR;
#else
    return GIC_PL500_DISTRIBUTOR_PPTR;
#endif
}

rt_inline rt_uint32_t platform_get_gic_redist_base(void)
{
    return GIC_PL500_REDISTRIBUTOR_PPTR;
}

rt_inline rt_uint32_t platform_get_gic_cpu_base(void)
{
#ifdef BSP_USING_GICV2
    return GIC_PL390_CONTROLLER_PPTR;
#else
    return GIC_PL500_CONTROLLER_PPTR;
#endif
}
#endif
