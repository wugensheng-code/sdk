//-----------------------------------------------------------------------------
// COPYRIGHT (C) 2020   CHIPS&MEDIA INC. ALL RIGHTS RESERVED
// 
// This file is distributed under BSD 3 clause and LGPL2.1 (dual license)
// SPDX License Identifier: BSD-3-Clause
// SPDX License Identifier: LGPL-2.1-only
// 
// The entire notice above must be reproduced on all authorized copies.
// 
// Description  : 
//-----------------------------------------------------------------------------
#include <stdlib.h>
#include <string.h>
#include "main_helper.h"

// #define DEFAULT_FEEDING_SIZE    8192         // 8KB
// #define DEFAULT_FEEDING_SIZE    0x20000         /* 128KBytes */
#define DEFAULT_FEEDING_SIZE    1024*160         /* 128KBytes */

typedef struct FeederFixedMemIOContext {
    const uint8_t* data;
    uint32_t size;
    uint32_t pos;
} FeederFixedMemIOContext;

void* BSFeederFixedSizeMemIO_Create(const BSChunk* parm)
{
    FeederFixedMemIOContext*  context=NULL;

    context = (FeederFixedMemIOContext*)malloc(sizeof(FeederFixedMemIOContext));
    if (context == NULL) {
        JLOG(ERR, "%s:%d failed to allocate memory\n", __FUNCTION__, __LINE__);
        return NULL;
    }

    context->data        = parm->data;
    context->size        = parm->size;
    context->pos         = 0;

    return (void*)context;
}

BOOL BSFeederFixedSizeMemIO_Destroy(void* feeder)
{
    FeederFixedMemIOContext* context = (FeederFixedMemIOContext*)feeder;

    if (context == NULL) {
        JLOG(ERR, "%s:%d Null handle\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    free(context);
    return TRUE;
}

Int32 BSFeederFixedSizeMemIO_Act(void* feeder, BSChunk* chunk)
{
    FeederFixedMemIOContext* context = (FeederFixedMemIOContext*)feeder;

    if (context == NULL) {
        JLOG(ERR, "%s:%d Null handle\n", __FUNCTION__, __LINE__);
        return 0;
    }

    uint32_t remain = context->size - context->pos;
    if ((int32_t)remain <= 0) {
        chunk->eos = TRUE;
        return 0;
    }

    uint32_t nTransfer = (chunk->size < DEFAULT_FEEDING_SIZE) ? chunk->size : DEFAULT_FEEDING_SIZE;
    if (nTransfer >= remain) {
        nTransfer = remain;
        chunk->eos = TRUE;
    }

    memcpy(chunk->data, context->data + context->pos, nTransfer);
    context->pos += nTransfer;

    return nTransfer;
}
