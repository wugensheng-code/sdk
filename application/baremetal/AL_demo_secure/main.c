#include "alsecure_misc.h"
#include "alsecure_auth.h"
#include "alsecure_hash.h"
#include "alsecure_cipher.h"
#include "alsecure_msg.h"
#include "alsecure_test_data.h"

Msg_t *Msg = (Msg_t *)(MSG_RAM_M_START_MAPPING);
Ack_t *Ack = (Ack_t *)(MSG_RAM_M_START_MAPPING + 64);

void TestDma_WholeBlockAndNoneHash(void)
{
    u32 Status = AL_SECURE_SUCCESS;
    u32 CaseNum = sizeof(TestCfgBothIncData[0])/sizeof(TestCfgBothInc_t);
    TestCfgBothInc_t *Cfg = NULL;
    u32 DataLength = INPUT_DATA_SIZE_128B;

    Msg_SetupIrq();

    for (u32 i = 0; i < CaseNum; i++) {
        Cfg = &TestCfgBothIncData[i];
        Status = AlSecure_EncryptInitialize(Msg, Cfg->EncryptMethod, Cfg->EcbCbc, Cfg->EncDec, \
                                            Cfg->HashMode, Cfg->KeyMode, IvDataBuff, KeyDataBuff);
        if (Status != AL_SECURE_SUCCESS) {
            ALSECURE_PRINT("Encrypt Initialize %d error: 0x%x", i, Status);
            continue;
        }
        Status = AlSecure_EncryptData(Msg, Cfg->DmaIncMode, Cfg->DmaSrcAddr, \ 
                        Cfg->DmaDstAddr, Cfg->HashOutAddr, DataLength);
        if (Status != AL_SECURE_SUCCESS) {
            ALSECURE_PRINT("Encrypt Data %d error: 0x%x", i, Status);
            continue;
        }
    }
}

int main(void)
{

    TestDma_WholeBlockAndNoneHash();

    while (1);
    return 0;
}