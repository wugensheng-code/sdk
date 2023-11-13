/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author         Notes
 * 2019-07-29     zdzn           first version
 * 2021-07-31     GuEe-GUI       config the memory/io address map
 * 2021-09-11     GuEe-GUI       remove do-while in rt_hw_timer_isr
 * 2021-12-28     GuEe-GUI       add smp support
 */

#include <rthw.h>
#include <rtthread.h>

#include "board.h"
#include <mmu.h>
#include <gic.h>
#include <gicv3.h>
#include <psci.h>
#include <gtimer.h>
#include <cpuport.h>
#include <interrupt.h>

void idle_wfi(void)
{
    asm volatile ("wfi");
}

/**
 *  Initialize the Hardware related stuffs. Called from rtthread_startup()
 *  after interrupt disabled.
 */
void rt_hw_board_init(void)
{
    /* initialize hardware interrupt */
    rt_hw_interrupt_init();

    /* initialize timer for os tick */
    rt_hw_gtimer_init();
    rt_thread_idle_sethook(idle_wfi);

    arm_psci_init(PSCI_METHOD_HVC, RT_NULL, RT_NULL);

#if defined(RT_USING_CONSOLE) && defined(RT_USING_DEVICE)
    /* set console device */
    rt_console_set_device(RT_CONSOLE_DEVICE_NAME);
#endif

#ifdef RT_USING_HEAP
    /* initialize memory system */
    rt_system_heap_init(RT_HW_HEAP_BEGIN, RT_HW_HEAP_END);
#endif

#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif

#ifdef RT_USING_SMP
    /* install IPI handle */
    rt_hw_ipi_handler_install(RT_SCHEDULE_IPI, rt_scheduler_ipi_handler);
    arm_gic_umask(0, IRQ_ARM_IPI_KICK);
#endif
}

rt_isr_handler_t rt_hw_interrupt_install(int vector, rt_isr_handler_t handler,
        void *param, const char *name)
{
    (AL_VOID)AlIntr_RegHandler(vector, AL_NULL, handler, AL_NULL);
}

void rt_hw_trap_fiq(void)
{
    if (rt_hw_get_current_el() < 2)
    {
	    do_irq_handle();
    }
    else
    {
        do_fiq_handle();
    }
}

void rt_hw_trap_irq(void)
{
    if (rt_hw_get_current_el() < 2)
    {
	    do_irq_handle();
    }
    else
    {
        do_fiq_handle();
    }
}

void poweroff(void)
{
    arm_psci_system_off();
}
MSH_CMD_EXPORT(poweroff, poweroff...);

void rt_hw_cpu_shutdown()
{
    rt_kprintf("shutdown...\n");

    poweroff();
}

void reboot(void)
{
    arm_psci_system_reboot();
}
MSH_CMD_EXPORT(reboot, reboot...);

#ifdef RT_USING_SMP
void rt_hw_secondary_cpu_up(void)
{
    int i;
    extern void secondary_cpu_start(void);
    extern rt_uint64_t rt_cpu_mpidr_early[];

    for (i = 1; i < RT_CPUS_NR; ++i)
    {
        arm_psci_cpu_on(rt_cpu_mpidr_early[i], (uint64_t)(secondary_cpu_start));
    }
}

void secondary_cpu_c_start(void)
{
    rt_hw_mmu_init();
    rt_hw_spin_lock(&_cpus_lock);

    arm_gic_cpu_init(0, platform_get_gic_cpu_base());
#ifdef BSP_USING_GICV3
    arm_gic_redist_init(0, platform_get_gic_redist_base());
#endif
    rt_hw_vector_init();
    rt_hw_gtimer_local_enable();
    arm_gic_umask(0, IRQ_ARM_IPI_KICK);

    rt_kprintf("\rcall cpu %d on success\n", rt_hw_cpu_id());

    rt_system_scheduler_start();
}

void rt_hw_secondary_cpu_idle_exec(void)
{
    __WFE();
}
#endif
