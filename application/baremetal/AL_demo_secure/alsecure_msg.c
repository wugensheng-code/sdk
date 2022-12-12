/*
 * alsecure_msg_send.c
 *
 *  Created on: 2022年6月22日
 *      Author: chao.zhang
 */

#include "alsecure_msg.h"
#include "al_types.h"
#include "nuclei_sdk_soc.h"

volatile u32 MsgFlag = 0;

u32 Msg_Send(Msg_t *Msg)
{
    /* Trigger interrupt to CSU */
    *(volatile u32 *)(RPU2CSU_REQ_ADDR) = 1;
    *(volatile u32 *)(RPU2CSU_REQ_ADDR) = 0;

    return AL_SUCCESS;
}

u32 Msg_Recv()
{
    MsgFlag = 1; /* Massege done Flag */
    ALSECURE_PRINT("In CSU ACK handler\n");
    return AL_SUCCESS;
}

void RpuCsuAckHandler() { Msg_Recv(); }

u32 Msg_CheckAckValid(Ack_t *Ack)
{
    ALSECURE_PRINT("Check for CSU Ack..\n");
    /* Chekc for ACK */
    while (MsgFlag == 0) {
    };           /* Wait for flag */
    MsgFlag = 0; /* Clear flag */

    if (Ack->Cmd != CMD_ACK) {
        ALSECURE_PRINT("ACK command incorrect!\n");
        return AL_FAILED;
    } else {
        Ack->Cmd = 0; /* Clear ack */
    }

    if (Ack->Option0.Low == OP_DMA_DONE) {
        ALSECURE_PRINT("ACK with DMA done\n");
    } else if (Ack->Option0.Low == OP_AUTH_PASS) {
        ALSECURE_PRINT("ACK with AUTH PASS\n");
    } else if (Ack->Option0.Low == OP_AUTH_FAIL) {
        ALSECURE_PRINT("ACK with AUTH FAIL\n");
    } else if (Ack->Option0.Low == OP_HASH_DONE) {
        ALSECURE_PRINT("ACK with HASH done\n");
    } else {
        ALSECURE_PRINT("ACK with failed\n");
        return AL_FAILED;
    }
}

u32 Msg_SetupIrq()
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
