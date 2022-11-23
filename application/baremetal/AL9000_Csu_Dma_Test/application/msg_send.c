/*
 * msg_send.c
 *
 *  Created on: 2022年6月22日
 *      Author: chao.zhang
 */

#include "msg_send.h"
#include "al_printf.h"
#include "al_types.h"
#include "nuclei_sdk_soc.h"

volatile u32 MsgFlag = 0;

u32 MsgSend(Msg_t *Msg)
{
    /* Trigger interrupt to CSU */
    *(volatile u32 *)(RPU2CSU_REQ_ADDR) = 1;
    *(volatile u32 *)(RPU2CSU_REQ_ADDR) = 0;

    return AL_SUCCESS;
}

u32 MsgRecv()
{
    MsgFlag = 1; /* Massege done Flag */
    al_printf("In CSU ACK handler\n");
    return AL_SUCCESS;
}

void RpuCsuAckHandler() { MsgRecv(); }

u32 SetupIrq()
{
    u32 returnCode;

    // initialize RPU2CSU_ACK as non-vector interrupt
    returnCode =
        ECLIC_Register_IRQ(RPU2CSU_ACK_IRQN, ECLIC_NON_VECTOR_INTERRUPT,
                           ECLIC_POSTIVE_EDGE_TRIGGER, 1, 0, RpuCsuAckHandler);

    // Enable interrupts in general.
    __enable_irq();

    return AL_SUCCESS;
}
