/*
 * Copyright (c) 2014, Mentor Graphics Corporation
 * All rights reserved.
 *
 * Copyright (c) 2015 Xilinx, Inc. All rights reserved.
 *
 * Copyright (c) 2023 Anlogic, Inc. All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <metal/sys.h>
#include <metal/irq.h>
#include "platform_info.h"
#include "al_gicv3.h"
#include "al_intr.h"
#include "al_errno.h"
#include "al_chip.h"
#include "al_reg_io.h"

/* Interrupt Controller setup */
static int app_gic_initialize(void)
{
    al_printf("Compile time: %s %s\r\n", __DATE__, __TIME__);

    (AL_VOID)AlIntr_RegHandler(SGI_NOTIFICATION, AL_NULL, metal_anlogic_irq_isr, (void *)(SGI_NOTIFICATION));

    (AL_VOID)AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    return 0;
}

/* Main hw machinery initialization entry point, called from main()*/
/* return 0 on success */
int init_system(void)
{
    int ret;
    struct metal_init_params metal_param = METAL_INIT_DEFAULTS;

    /* Low level abstraction layer for openamp initialization */
    metal_init(&metal_param);

    /* configure the global interrupt controller */
    app_gic_initialize();

    /* Initialize metal Xilinx IRQ controller */
    ret = metal_anlogic_irq_init();
    if (ret) {
	    al_printf("%s: Anlogic metal IRQ controller init failed.\r\n",
			__func__);
    }

    return ret;
}

void cleanup_system()
{
    metal_finish();
}
