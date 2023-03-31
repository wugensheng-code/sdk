#include "al_csu_config.h"
#include "al_types.h"
#include "msg_send.h"
#include "stdio.h"

u32 CheckBuf(u32 *Buf0, u32 *Buf1, u32 Length)
{
    u32 i;
    /* Check for DMA transfer result */
    for (i = 0; i < Length; i++) {
        if (*(Buf0 + i*4)  != *(Buf1 +i*4)) {
            al_printf("Data not equal\n");
            al_printf("Fail ptr0 = %x\n",Buf0 + i*4);
            al_printf("Fail ptr0 = %x\n",Buf1 + i*4);
            al_printf("Fail buf0 = %x\n",*(Buf0 + i*4));
            al_printf("Fail buf1 = %x\n",*(Buf1 + i*4));
//            return AL_FAILED;
        }
    }
//    al_printf("Data equal\n");
    return AL_SUCCESS;
}

u32 CheckAckValid(Ack_t *Ack)
{
//    al_printf("Check for CSU Ack..\n");
    /* Chekc for ACK */
    while (MsgFlag == 0) {
    };           /* Wait for flag */
    MsgFlag = 0; /* Clear flag */

    if (Ack->Cmd != CMD_ACK) {
        al_printf("ACK command incorrect!\n");
        return AL_FAILED;
    } else {
        Ack->Cmd = 0; /* Clear ack */
    }

    if (Ack->Option0.Low == OP_DMA_DONE) {
//        al_printf("ACK with DMA done\n");
    } else if (Ack->Option0.Low == OP_AUTH_PASS) {
        al_printf("ACK with AUTH PASS\n");
    } else if (Ack->Option0.Low == OP_AUTH_FAIL) {
        al_printf("ACK with AUTH FAIL\n");
    } else if (Ack->Option0.Low == OP_HASH_DONE) {
        al_printf("ACK with HASH done\n");
    } else {
        al_printf("ACK with failed\n");
        return AL_FAILED;
    }
}

void Get_Gold_Data(unsigned int const *ptr_const_data, unsigned int *ptr_data,
                   unsigned int len)
{
    unsigned int i;
    for (i = 0; i < len; i++) {
        *(ptr_data + i) = *(ptr_const_data + i);
    }
}

u32 CheckHash(u32 *HashBuf, u32 *GoldBuf)
{
    for (u32 i = 0; i < 8; i++) {
        if (HashBuf[i] != GoldBuf[i]) {
            al_printf("Hash incorrect\n");
            return AL_FAILED;
        }
    }
    al_printf("Hash correct\n");
    return AL_SUCCESS;
}
