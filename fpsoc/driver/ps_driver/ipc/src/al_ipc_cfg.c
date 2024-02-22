/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


/********************************* including Files *********************************/
#include "al_ipc_hw.h"

/********************************* Constant Definitions *****************************/


/********************************* Type definitions *********************************/


/********************************* Macros  & Inline Functions Definitions ***********/


/******************************** Function Prototypes ******************************/


/******************************** Variable Prototypes ******************************/

AL_IPCINTR_HwConfigStruct AlIpcIntr_HwConfig[AL_IPC_INTR_NUM_INSTANCE] =
{
    {
        .Type               = AL_IPC_APU2RPU_REQ,
        .IntrNum            = SOC_APU2RPU_REQ_IRQn,
    },

    {
        .Type               = AL_IPC_APU2RPU_ACK,
        .IntrNum            = SOC_APU2RPU_ACK_IRQn,
    },

    {
        .Type               = AL_IPC_RPU2APU_REQ,
        .IntrNum            = SOC_RPU2APU_REQ_IRQn,
    },
    {
        .Type               = AL_IPC_RPU2APU_ACK,
        .IntrNum            = SOC_RPU2APU_ACK_IRQn,
    },
};
