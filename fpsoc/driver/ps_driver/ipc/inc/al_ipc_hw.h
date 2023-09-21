/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_IPC_HW_H_
#define __AL_IPC_HW_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files ********************************/
#include "al_core.h"

/************************** Constant Definitions ****************************/

/**************************** Type Definitions ******************************/

/***************** Macros (Inline Functions) Definitions ********************/
#define IPC_SPIN_LOCK_OFFSET    (0x800)
#define IPC_SPIN_LOCK_NUM       (4)


/**************************** Type Definitions ******************************/

#ifdef __cplusplus
}
#endif

#endif
