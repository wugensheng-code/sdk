#include "al_printf.h"
#include "al_types.h"
#include "msg_send.h"
#include "test_data.h"
#include "test_func.h"

//#define TransLen 2048

#define TransLen 2048
u32 Dma_Test(unsigned int * ibuff, unsigned int * obuff,u32 length)
{
    u32    Status;
    Msg_t *Msg = (Msg_t *)(MSG_RAM_BASEADDR);
    Ack_t *Ack = (Ack_t *)(MSG_RAM_BASEADDR + 64);

    TestDataInit();

    /* Prepare and send command */
    Msg->MsgData.DmaParam.CipherParam.TotalLength = length;
    Msg->MsgData.DmaParam.CipherParam.pbIV        = iv_data_buff;
    Msg->MsgData.DmaParam.CipherParam.pbKey       = key_data_buff;
    Msg->MsgData.DmaParam.CipherParam.pbInput     = ibuff;
    Msg->MsgData.DmaParam.CipherParam.pbOutput    = obuff;
    Msg->MsgData.DmaParam.CipherParam.OpMode.All  = 0x40; // ECB en
    Msg->Option1.Low = OP_USER_KEY;
    Msg->Cmd          = CMD_DMA;
    Msg->Option0.Low  = OP_ENCRYPT_NONE;
    Msg->Option0.High = OP_HASH_NONE;

    Status = MsgSend(Msg);

    /* Chekc for ACK */
    Status = CheckAckValid(Ack);
    if (Status == AL_FAILED) {
        return AL_FAILED;
    }

    /* Check for DMA transfer result */
//    Status = CheckBuf(ibuff, obuff, TransLen);
//    if (Status == AL_FAILED) {
//        return AL_FAILED;
//    }
    return AL_SUCCESS;
}
