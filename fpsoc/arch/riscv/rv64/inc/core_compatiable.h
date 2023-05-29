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
#ifndef __CORE_COMPATIABLE_H__
#define __CORE_COMPATIABLE_H__
/*!
 * @file     core_compatiable.h
 * @brief    ARM compatiable function definitions header file
 */
#ifdef __cplusplus
 extern "C" {
#endif

/* ===== ARM Compatiable Functions ===== */
/**
 * \defgroup NMSIS_Core_ARMCompatiable_Functions   ARM Compatiable Functions
 * \ingroup  NMSIS_Core
 * \brief    A few functions that compatiable with ARM CMSIS-Core.
 * \details
 *
 * Here we provided a few functions that compatiable with ARM CMSIS-Core,
 * mostly used in the DSP and NN library.
 * @{
 */
/** \brief Instruction Synchronization Barrier, compatiable with ARM */

#ifdef __riscv
#define __ISB()                             __RWMB()

/** \brief Data Synchronization Barrier, compatiable with ARM */
#define __DSB()                             __RWMB()

/** \brief Data Memory Barrier, compatiable with ARM */
#define __DMB()                             __RWMB()
#endif

/** \brief LDRT Unprivileged (8 bit), ARM Compatiable */
#define __LDRBT(ptr)                        __LB((ptr))
/** \brief LDRT Unprivileged (16 bit), ARM Compatiable */
#define __LDRHT(ptr)                        __LH((ptr))
/** \brief LDRT Unprivileged (32 bit), ARM Compatiable */
#define __LDRT(ptr)                         __LW((ptr))

/** \brief STRT Unprivileged (8 bit), ARM Compatiable */
#define __STRBT(val, ptr)                   __SB((ptr), (val))
/** \brief STRT Unprivileged (16 bit), ARM Compatiable */
#define __STRHT(val, ptr)                   __SH((ptr), (val))
/** \brief STRT Unprivileged (32 bit), ARM Compatiable */
#define __STRT(val, ptr)                    __SW((ptr), (val))


/** @} */ /* End of Doxygen Group NMSIS_Core_ARMCompatiable_Functions */

#ifdef __cplusplus
}
#endif
#endif /* __CORE_COMPATIABLE_H__ */
