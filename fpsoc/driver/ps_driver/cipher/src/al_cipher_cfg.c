#include "al_cipher_hw.h"

AL_CIPHER_HwConfigStruct AlCipher_HwConfig[AL_CIPHER_NUM_INSTANCE] = {
    {
        .DeviceId       = 0,
        .BaseAddress    = CIPHER__BASE_ADDR,
        .ReqIntrAddr    = CIPHER_REQ_INTR_ADDR,
        .AckIntrId      = SOC_ACK2_IRQn,
    }
};