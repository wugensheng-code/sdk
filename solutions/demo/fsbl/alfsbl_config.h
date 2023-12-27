/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_ALFSBL_CONFIG_H_
#define __AL_ALFSBL_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif


/*
* FSBL code include options
* FSBL includes all code by default.
* Unwanted code can be excluded from the elf by macro defines here
* Below blocks can be excluded from the code:
    - ALFSBL_WDT_EXCLUDE:     WDT code will be excluded, suggest to exclude it while debugging
    - ALFSBL_PMU_EXCLUDE:     PMU init code will be excluded, suggest to exclude it while debugging

*/

#ifndef ALFSBL_WDT_EXCLUDE_VAL
#define ALFSBL_WDT_EXCLUDE_VAL    (0U)
#endif

#ifndef ALFSBL_PMU_EXCLUDE_VAL
#define ALFSBL_PMU_EXCLUDE_VAL    (0U)
#endif


#if ALFSBL_WDT_EXCLUDE_VAL
#define ALFSBL_WDT_EXCLUDE
#endif

#if ALFSBL_PMU_EXCLUDE_VAL
#define ALFSBL_PMU_EXCLUDE
#endif



#ifdef __cplusplus
}
#endif

#endif /* AL_ALFSBL_CONFIG_H */
