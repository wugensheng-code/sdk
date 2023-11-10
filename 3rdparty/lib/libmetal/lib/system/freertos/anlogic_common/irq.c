/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * @file	generic/anlogic_common/irq.c
 * @brief	generic libmetal Anlogic irq controller definitions.
 */

#include <metal/errno.h>
#include <metal/irq_controller.h>
#include <metal/sys.h>
#include <metal/log.h>
#include <metal/mutex.h>
#include <metal/list.h>
#include <metal/utilities.h>
#include <metal/alloc.h>

#define MAX_IRQS (100) /* FIXME TODO... */

static struct metal_irq irqs[MAX_IRQS]; /**< Linux IRQs array */

static void metal_anlogic_irq_set_enable(struct metal_irq_controller *irq_cntr,
				      int irq, unsigned int state)
{
	if (irq < irq_cntr->irq_base ||
	    irq >= irq_cntr->irq_base + irq_cntr->irq_num) {
		metal_log(METAL_LOG_ERROR, "%s: invalid irq %d\n",
			  __func__, irq);
		return;
	} else if (state == METAL_IRQ_ENABLE) {
		sys_irq_enable((unsigned int)irq);
	} else {
		sys_irq_disable((unsigned int)irq);
	}
}

/**< Anlogic common platform IRQ controller */
static METAL_IRQ_CONTROLLER_DECLARE(anlogic_irq_cntr,
				    0, MAX_IRQS,
				    NULL,
				    metal_anlogic_irq_set_enable, NULL,
				    irqs);

/**
 * @brief default handler
 */
void metal_anlogic_irq_isr(void *arg)
{
	unsigned int vector;

	vector = (uint64_t)arg;
	if (vector >= MAX_IRQS) {
		return;
	}
	(void)metal_irq_handle(&irqs[vector], (int)vector);
}

int metal_anlogic_irq_init(void)
{
	int ret;

	ret =  metal_irq_register_controller(&anlogic_irq_cntr);
	if (ret < 0) {
		metal_log(METAL_LOG_ERROR, "%s: register irq controller failed.\n",
			  __func__);
		return ret;
	}
	return 0;
}
