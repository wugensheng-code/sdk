/*
 * Copyright (c) 2009-2018 Arm Limited. All rights reserved.
 * Copyright (c) 2019 Nuclei Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/******************************************************************************
 * @file     system_demosoc.c
 * @brief    NMSIS Nuclei Core Device Peripheral Access Layer Source File for
 *           Nuclei Demo SoC which support Nuclei N/NX class cores
 * @version  V1.00
 * @date     22. Nov 2019
 ******************************************************************************/
#include <stdint.h>
#include <stdio.h>
#include "nuclei_sdk_hal.h"
#include "config.h"
#include "gic_v3_addr.h"
#include "gic_v3.h"

/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/
/* ToDo: add here your necessary defines for device initialization
         following is an example for different system frequencies */
#ifndef SYSTEM_CLOCK
#define SYSTEM_CLOCK    (50000000UL)  // uart test
//#define SYSTEM_CLOCK    (200000000UL)
#endif

void Enablepinmux1(void);
void Enablepinmux1_mode2(void);

/**
 * \defgroup  NMSIS_Core_SystemConfig       System Device Configuration
 * \brief Functions for system and clock setup available in system_<device>.c.
 * \details
 * Nuclei provides a template file **system_Device.c** that must be adapted by
 * the silicon vendor to match their actual device. As a <b>minimum requirement</b>,
 * this file must provide:
 *  -  A device-specific system configuration function, \ref SystemInit.
 *  -  A global variable that contains the system frequency, \ref SystemCoreClock.
 *  -  A global eclic configuration initialization, \ref ECLIC_Init.
 *  -  Global c library \ref _init and \ref _fini functions called right before calling main function.
 *  -  Vendor customized interrupt, exception and nmi handling code, see \ref NMSIS_Core_IntExcNMI_Handling
 *
 * The file configures the device and, typically, initializes the oscillator (PLL) that is part
 * of the microcontroller device. This file might export other functions or variables that provide
 * a more flexible configuration of the microcontroller system.
 *
 * And this file also provided common interrupt, exception and NMI exception handling framework template,
 * Silicon vendor can customize these template code as they want.
 *
 * \note Please pay special attention to the static variable \c SystemCoreClock. This variable might be
 * used throughout the whole system initialization and runtime to calculate frequency/time related values.
 * Thus one must assure that the variable always reflects the actual system clock speed.
 *
 * \attention
 * Be aware that a value stored to \c SystemCoreClock during low level initialization (i.e. \c SystemInit()) might get
 * overwritten by C libray startup code and/or .bss section initialization.
 * Thus its highly recommended to call \ref SystemCoreClockUpdate at the beginning of the user \c main() routine.
 *
 * @{
 */

/*----------------------------------------------------------------------------
  System Core Clock Variable
 *----------------------------------------------------------------------------*/
/* ToDo: initialize SystemCoreClock with the system core clock frequency value
         achieved after system intitialization.
         This means system core clock frequency after call to SystemInit() */
/**
 * \brief      Variable to hold the system core clock value
 * \details
 * Holds the system core clock, which is the system clock frequency supplied to the SysTick
 * timer and the processor core clock. This variable can be used by debuggers to query the
 * frequency of the debug timer or to configure the trace clock speed.
 *
 * \attention
 * Compilers must be configured to avoid removing this variable in case the application
 * program is not using it. Debugging systems require the variable to be physically
 * present in memory so that it can be examined to configure the debugger.
 */
uint32_t SystemCoreClock = SYSTEM_CLOCK;  /* System Clock Frequency (Core Clock) */

/*----------------------------------------------------------------------------
  Clock functions
 *----------------------------------------------------------------------------*/

/**
 * \brief      Function to update the variable \ref SystemCoreClock
 * \details
 * Updates the variable \ref SystemCoreClock and must be called whenever the core clock is changed
 * during program execution. The function evaluates the clock register settings and calculates
 * the current core clock.
 */
void SystemCoreClockUpdate(void)             /* Get Core Clock Frequency */
{
    /* ToDo: add code to calculate the system frequency based upon the current
     *    register settings.
     * Note: This function can be used to retrieve the system core clock frequeny
     *    after user changed register settings.
     */
    SystemCoreClock = SYSTEM_CLOCK;
}

/**
 * \brief      Function to Initialize the system.
 * \details
 * Initializes the microcontroller system. Typically, this function configures the
 * oscillator (PLL) that is part of the microcontroller device. For systems
 * with a variable clock speed, it updates the variable \ref SystemCoreClock.
 * SystemInit is called from the file <b>startup<i>_device</i></b>.
 */
void SystemInit(void)
{
    /* ToDo: add code to initialize the system
     * Warn: do not use global variables because this function is called before
     * reaching pre-main. RW section maybe overwritten afterwards.
     */
    SystemCoreClock = SYSTEM_CLOCK;
}


/** @} */ /* End of Doxygen Group NMSIS_Core_ExceptionAndNMI */

/** Banner Print for Nuclei SDK */
void SystemBannerPrint(void)
{
#if defined(NUCLEI_BANNER) && (NUCLEI_BANNER == 1)
#ifndef DOWNLOAD_MODE
#error DOWNLOAD_MODE is not defined via build system, please check!
#endif
    const char* download_modes[] = {"FLASHXIP", "FLASH", "ILM", "DDR"};
    printf("Anlogic APU SDK Build Time: %s, %s\r\n", __DATE__, __TIME__);
    printf("Download Mode: %s\r\n", download_modes[DOWNLOAD_MODE]);
    printf("CPU Frequency %lu Hz\r\n", SystemCoreClock);
#endif
}

/**
 * \brief: Initialize a specific IRQ and register the handler

 * \details
 * This function set vector mode, trigger mode and polarity, interrupt level and priority,
 * assign handler for specific IRQn.
 * \param [in]  IRQn        IRQn is the rpu irq_id, the apu id is
 * \param [in]  shv         \ref ECLIC_NON_VECTOR_INTERRUPT means non-vector mode, and \ref ECLIC_VECTOR_INTERRUPT is vector mode
 * \param [in]  trig_mode   see \ref ECLIC_TRIGGER_Type
 * \param [in]  lvl         interupt level
 * \param [in]  priority    interrupt priority
 * \param [in]  handler     interrupt handler, if NULL, handler will not be installed
 * \return       -1 means invalid input parameter. 0 means successful.
 * \remarks

 * - This function use to configure specific eclic interrupt and register its interrupt handler and enable its interrupt.
 * - If the vector table is placed in read-only section(FLASHXIP mode), handler could not be installed
 */
int32_t ECLIC_Register_IRQ(IRQn_Type IRQn, uint8_t shv, ECLIC_TRIGGER_Type trig_mode, uint8_t lvl, uint8_t priority, void* handler)
{
	/*
	 * RPU SPI interrupt IRQn to Apu SPI IRQn mapping from SPI_START_ID_SHARE_BETWEEN_APU_RPU:
	 *	 APU_SPI_IDX = (RPU_SPI_IDX + SPI_OFFSET_APU_TO_RPU)
	*/
    #define SPI_ID_OFFSET_APU_TO_RPU					(13)

    interrupt_table *callback = (interrupt_table *)handler;

    //is edge triger?
    if (trig_mode != 0) {
        uint32_t *addr = GICD_ICFGR + ((IRQn + SPI_ID_OFFSET_APU_TO_RPU) / 16) * 4;
        uint32_t mask = *(uint32_t *)addr;
        uint32_t offset = (((IRQn + SPI_ID_OFFSET_APU_TO_RPU) % 16) << 1);
        mask &= ~(0x3 << offset);
        mask |= 0x2 << offset;
        // writel_relaxed(mask, addr);
        *(uint32_t *)addr = mask;
    }

#ifndef SWITCH_TO_EL1_EL0_FROM_EL3
    request_fiq(IRQn + SPI_ID_OFFSET_APU_TO_RPU, callback->handler, callback->ref);
    printf("currentel is el3, request fiq!\r\n");
#else
    request_irq(IRQn + SPI_ID_OFFSET_APU_TO_RPU, callback->handler, callback->ref);
    printf("currentel is not el3, request irq!\r\n");
#endif
    //asm volatile("msr %w0, [%1]" : : "rZ" (val), "r" (addr));
	return 0;
}

/** @} */ /* End of Doxygen Group NMSIS_Core_ExceptionAndNMI */

/**
 * \brief early init function before main
 * \details
 * This function is executed right before main function.
 * For RISC-V gnu toolchain, _init function might not be called
 * by __libc_init_array function, so we defined a new function
 * to do initialization
 */
void _premain_init(void)
{
     *(uint32_t *)(0xf8803068u) =0x3;    //uart0  MIO26/27
	 *(uint32_t *)(0xf880306cu) =0x3;
#if ENABLE_PINMUX_MODE1 == 1
    Enablepinmux1();
#endif

#if ENABLE_PINMUX_MODE2 == 1
    Enablepinmux1_mode2();
#endif

	AlUart_Init(AL_UART0, 115200, UART_BIT_LENGTH_8, AL_UART_STOP_BIT_1);
	/* Display banner after UART initialized */
	SystemBannerPrint();
    
#if (defined SUPPORT_NONSECURE || defined SWITCH_TO_EL0_FROM_EL3)
	//if SUPPORT_NONSECURE, gic-v3 init in EL3
	//if SWITCH_TO_EL0_FROM_EL3, boot to EL0, only to test
#else
	gicv3_init();
#endif

#ifndef SWITCH_TO_EL0_FROM_EL3
	generic_timer_init();
#endif
}

/** @} */ /* End of Doxygen Group NMSIS_Core_SystemAndClock */
