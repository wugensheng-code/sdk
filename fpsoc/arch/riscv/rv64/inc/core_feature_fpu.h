/*
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
#ifndef __CORE_FEATURE_FPU_H__
#define __CORE_FEATURE_FPU_H__
/*!
 * @file     core_feature_fpu.h
 * @brief    FPU feature API header file for Nuclei N/NX Core
 */
/*
 * FPU Feature Configuration Macro:
 * 1. __FPU_PRESENT:  Define whether Floating Point Unit(FPU) is present or not
 *   * 0: Not present
 *   * 1: Single precision FPU present, __RISCV_FLEN == 32
 *   * 2: Double precision FPU present, __RISCV_FLEN == 64
 */
#ifdef __cplusplus
 extern "C" {
#endif

/* ===== FPU Operations ===== */
/**
 * \defgroup NMSIS_Core_FPU_Functions   FPU Functions
 * \ingroup  NMSIS_Core
 * \brief    Functions that related to the RISC-V FPU (F and D extension).
 * \details
 *
 * Nuclei provided floating point unit by RISC-V F and D extension.
 * * `F extension` adds single-precision floating-point computational
 * instructions compliant with the IEEE 754-2008 arithmetic standard, __RISCV_FLEN = 32.
 *   The F extension adds 32 floating-point registers, f0-f31, each 32 bits wide,
 *   and a floating-point control and status register fcsr, which contains the
 *   operating mode and exception status of the floating-point unit.
 * * `D extension` adds double-precision floating-point computational instructions
 * compliant with the IEEE 754-2008 arithmetic standard.
 *   The D extension widens the 32 floating-point registers, f0-f31, to 64 bits, __RISCV_FLEN = 64
 *   @{
 */
#if defined(__FPU_PRESENT) && (__FPU_PRESENT > 0)

#if __FPU_PRESENT == 1
  /** \brief Refer to the width of the floating point register in bits(either 32 or 64) */
  #define __RISCV_FLEN          32
#elif __FPU_PRESENT == 2
  #define __RISCV_FLEN          64
#else
  #define __RISCV_FLEN          __riscv_flen
#endif /* __FPU_PRESENT == 1 */


/** \brief Enable FPU Unit */
#define __enable_FPU()          __RV_CSR_SET(CSR_MSTATUS, MSTATUS_FS)
/**
 * \brief Disable FPU Unit
 * \details
 * * We can save power by disable FPU Unit.
 * * When FPU Unit is disabled, any access to FPU related CSR registers
 * and FPU instructions will cause illegal Instuction Exception.
 * */
#define __disable_FPU()         __RV_CSR_CLEAR(CSR_MSTATUS, MSTATUS_FS)


/** @} */ /* End of Doxygen Group NMSIS_Core_FPU_Functions */

#ifdef __cplusplus
}
#endif
#endif
#endif /** __RISCV_EXT_FPU_H__  */
