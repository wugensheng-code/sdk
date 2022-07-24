/*
 * @Author: Guangxiang Rao
 * @Date: 2022-01-13 14:32:11
 * @LastEditors: Guangxiang Rao
 * @LastEditTime: 2022-04-29 02:40:12
 * @Description: ...
 * @FilePath: /alsoc-embedded-DE/bsp/drives/apu/src/exception_64.c
 */

#include "compiler_attributes.h"
#include "config.h"
#include "exception.h"

#ifdef CONFIG_PRINTK
#define error_print(...) printf(__VA_ARGS__)
#else
#define error_print(...)
#endif

static void panic(void)
{
	error_print("warning: system hang here, waiting for exiting\n");

	while (1)
		;
}

/**
 * @desc  : do_bad_sync handles the impossible case in the Synchronous Abort vector,
 * 		     you must re-implement event handle.
 * @param {pt_regs} *pt_regs
 * @param {unsigned int} esr
 * @return {*}
 */
__weak void do_bad_sync(struct pt_regs *pt_regs, unsigned int esr)
{
	error_print("Bad mode in \"Synchronous Abort at current el with sp0\" handler, but not found your handle implement.\n");
	panic();
}

/**
 * @desc  : do_bad_irq handles the impossible case in the Irq vector,
 * you must re-implement event handle.
 * @param {pt_regs} *pt_regs
 * @param {unsigned int} esr
 * @return {*}
 */
__weak void do_bad_irq(struct pt_regs *pt_regs, unsigned int esr)
{
	error_print("Bad mode in \"Irq at current el with sp0 \" handler\n, but not found your handle implement.");
	panic();
}

/**
 * @desc  : do_bad_fiq handles the impossible case in the Fiq vector,
 * you must re-implement event handle.
 * @param {pt_regs} *pt_regs
 * @param {unsigned int} esr
 * @return {*}
 */
__weak void do_bad_fiq(struct pt_regs *pt_regs, unsigned int esr)
{
	error_print("Bad mode in \"Fiq at current el with sp0\" handler, but not found your handle implement.\n");
	panic();
}

/**
 * @desc  : do_bad_error handles the impossible case in the Error vector,
 *  you must re-implement event handle.
 * @param {pt_regs} *pt_regs
 * @param {unsigned int} esr
 * @return {*}
 */
__weak void do_bad_error(struct pt_regs *pt_regs, unsigned int esr)
{
	error_print("Bad mode in \"Error at current el with sp0\" handler, but not found your handle implement.\n");
	panic();
}

/**
 * @desc  : do_sync handles the Synchronous Abort exception,
 * you must re-implement event handle.
 * @param {pt_regs} *pt_regs
 * @param {unsigned int} esr
 * @return {*}
 */
__weak void do_sync_handle(struct pt_regs *pt_regs, unsigned int esr)
{
	error_print("\"Synchronous Abort at current el with spx\" handler, but not found your handle implement.\n\r");
	panic();
}

/**
 * @desc  : do_irq handles the Irq exception,
 * you must re-implement event handle.
 * @param {pt_regs} *pt_regs
 * @param {unsigned int} esr
 * @return {*}
 */
__weak void do_irq_handle()
{
	error_print("do Irq handler at current el with spx, but can not found your irq handle.\n");
	panic();
}

/**
 * @desc  : do_fiq handles the Fiq exception, you must re-implement event handle.
 * @param {pt_regs} *pt_regs
 * @param {unsigned int} esr
 * @return {*}
 */
__weak void do_fiq_handle(struct pt_regs *pt_regs, unsigned int esr)
{
	error_print("\"Fiq at current el with spx\" handler, but not found your handle implement.");
	panic();
}


/**
 * @desc  : do_error handles the Error exception, you must re-implement event handle.
 * @param {pt_regs} *pt_regs
 * @param {unsigned int} esr
 * @return {*}
 */
__weak void do_error(struct pt_regs *pt_regs, unsigned int esr)
{
	error_print("\"Error at current el with spx\" handler, but not found your handle implement.");
	panic();
}
