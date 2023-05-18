#ifndef JPU_ONESHOT_API_H_
#define JPU_ONESHOT_API_H_

#include "main_helper.h"

#define NUM_FRAME_BUF               3
#define ENC_SRC_BUF_NUM             1
#define BS_SIZE_ALIGNMENT           4096
#define MIN_BS_SIZE                 8192

typedef struct {
    int picWidth;
    int picHeight;
    FrameFormat subsample;
} picInfo;

BOOL JPU_decode_oneShot(DecConfigParam* param, const uint8_t* src, uint32_t bsize, uint32_t* dst_addr, picInfo* retInfo);
BOOL JPU_encode_oneShot(EncConfigParam* param, const uint8_t* src, uint8_t* dst, uint32_t* bsize);

#endif
