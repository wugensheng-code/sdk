/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef AL_AXI_GPIO_ll_H_
#define AL_AXI_GPIO_ll_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files ********************************/
#include "al_axi_gpio_hw.h"
#include "al_reg_io.h"

typedef enum
{
    AL_AXI_GPIO_OUT         = 0,
    AL_AXI_GPIO_IN          = 1
} AlAxiGpio_ll_DirEnum;

typedef enum
{
    AL_AXI_GPIO_CHANNEL_OUT = 0x0           ,
    AL_AXI_GPIO_CHANNEL_IN  = 0xffffffff
} AlAxiGpio_ll_ChannelDirEnum;

typedef enum
{
    AL_AXI_GPIO_INT_DISABLE = 0,
    AL_AXI_GPIO_INT_ENABLE  = 1
} AlAxiGpio_ll_IntEnum;

typedef enum
{
    AL_AXI_GPIO_CHANNEL1    = 0,
    AL_AXI_GPIO_CHANNEL2    = 1,
    AL_AXI_GPIO_ALL_CHAN    = 2
} AlAxiGpio_ll_ChannelEnum;

typedef enum
{
    AL_AXI_GPIO_NINT    = 0,
    AL_AXI_GPIO_INT     = 1
} AlAxiGpio_ll_InrStatusEnum;
/***************************** AL_AXI_GPIO_DATA ********************************/
static inline AL_VOID AlAxiGpio_ll_SetData(AL_U32 BaseAddr, AL_U32 Bit, AL_U32 Value)
{
    AL_REG32_SET_BIT(BaseAddr + AL_AXI_GPIO_DATA, Bit, Value);
}

static inline AL_U32 AlAxiGpio_ll_GetData(AL_U32 BaseAddr, AL_U32 Bit)
{
    return AL_REG32_GET_BIT(BaseAddr + AL_AXI_GPIO_DATA, Bit);
}

static inline AL_VOID AlAxiGpio_ll_SetChannelData(AL_U32 BaseAddr, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + AL_AXI_GPIO_DATA, Value);
}

static inline AL_U32 AlAxiGpio_ll_GetChannelData(AL_U32 BaseAddr)
{
    return AL_REG32_READ(BaseAddr + AL_AXI_GPIO_DATA);
}

/***************************** AL_AXI_GPIO_TRI ********************************/
static inline AL_VOID AlAxiGpio_ll_SetDir(AL_U32 BaseAddr, AL_U32 Bit, AlAxiGpio_ll_DirEnum Dir)
{
    AL_REG32_SET_BIT(BaseAddr + AL_AXI_GPIO_TRI, Bit, Dir);
}

static inline AlAxiGpio_ll_DirEnum AlAxiGpio_ll_GetDir(AL_U32 BaseAddr, AL_U32 Bit)
{
    return AL_REG32_GET_BIT(BaseAddr + AL_AXI_GPIO_TRI, Bit);
}

static inline AL_VOID AlAxiGpio_ll_SetChannelDir(AL_U32 BaseAddr, AlAxiGpio_ll_ChannelDirEnum Dir)
{
    AL_REG32_WRITE(BaseAddr + AL_AXI_GPIO_TRI, Dir);
}

static inline AlAxiGpio_ll_ChannelDirEnum AlAxiGpio_ll_GetChannelDir(AL_U32 BaseAddr)
{
    return AL_REG32_READ(BaseAddr + AL_AXI_GPIO_TRI);
}

/***************************** AL_AXI_GPIO_DATA2 ********************************/
static inline AL_VOID AlAxiGpio_ll_SetData2(AL_U32 BaseAddr, AL_U32 Bit, AL_U32 Value)
{
    AL_REG32_SET_BIT(BaseAddr + AL_AXI_GPIO_DATA2, Bit, Value);
}

static inline AL_U32 AlAxiGpio_ll_GetData2(AL_U32 BaseAddr, AL_U32 Bit)
{
    return AL_REG32_GET_BIT(BaseAddr + AL_AXI_GPIO_DATA2, Bit);
}

static inline AL_VOID AlAxiGpio_ll_SetChannelData2(AL_U32 BaseAddr, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + AL_AXI_GPIO_DATA2, Value);
}

static inline AL_U32 AlAxiGpio_ll_GetChannelData2(AL_U32 BaseAddr)
{
    return AL_REG32_READ(BaseAddr + AL_AXI_GPIO_DATA2);
}
/***************************** AL_AXI_GPIO_TRI2 ********************************/
static inline AL_VOID AlAxiGpio_ll_SetDir2(AL_U32 BaseAddr, AL_U32 Bit, AlAxiGpio_ll_DirEnum Dir)
{
    AL_REG32_SET_BIT(BaseAddr + AL_AXI_GPIO_TRI2, Bit, Dir);
}

static inline AlAxiGpio_ll_DirEnum AlAxiGpio_ll_GetDir2(AL_U32 BaseAddr, AL_U32 Bit)
{
    return AL_REG32_GET_BIT(BaseAddr + AL_AXI_GPIO_TRI2, Bit);
}

static inline AL_VOID AlAxiGpio_ll_SetChannelDir2(AL_U32 BaseAddr, AlAxiGpio_ll_ChannelDirEnum Dir)
{
    AL_REG32_WRITE(BaseAddr + AL_AXI_GPIO_TRI2, Dir);
}

static inline AlAxiGpio_ll_ChannelDirEnum AlAxiGpio_ll_GetChannelDir2(AL_U32 BaseAddr)
{
    return AL_REG32_READ(BaseAddr + AL_AXI_GPIO_TRI2);
}

/***************************** AL_AXI_GPIO_GIER ********************************/
static inline AL_VOID AlAxiGpio_ll_SetGlobalInt(AL_U32 BaseAddr, AlAxiGpio_ll_IntEnum Int)
{
    AL_REG32_SET_BIT(BaseAddr + AL_AXI_GPIO_GIER, 31, Int);
}

/***************************** AL_AXI_GPIO_ISR ********************************/
static inline AL_U32 AlAxiGpio_ll_GetIntStatus(AL_U32 BaseAddr)
{
    return (AL_REG32_READ(BaseAddr + AL_AXI_GPIO_ISR) & 0x3);
}

static inline AL_VOID AlAxiGpio_ll_ClearInt(AL_U32 BaseAddr, AlAxiGpio_ll_ChannelEnum Channel)
{
    AL_REG32_SET_BIT(BaseAddr + AL_AXI_GPIO_ISR, Channel, 1);
}

/***************************** AL_AXI_GPIO_IER ********************************/
static inline AL_VOID AlAxiGpio_ll_SetChannelInt(AL_U32 BaseAddr, AlAxiGpio_ll_ChannelEnum Channel, AlAxiGpio_ll_IntEnum Int)
{
    AL_REG32_SET_BIT(BaseAddr + AL_AXI_GPIO_IER, Channel, Int);
}

static inline AL_U32 AlAxiGpio_ll_GetChannelInt(AL_U32 BaseAddr, AlAxiGpio_ll_ChannelEnum Channel)
{
    return AL_REG32_GET_BIT(BaseAddr + AL_AXI_GPIO_IER, Channel);
}

#ifdef __cplusplus
}
#endif

#endif


