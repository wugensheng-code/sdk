#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jpuapi.h"
#include "regdefine.h"
#include "jpulog.h"
#include "jpuapifunc.h"
#include "jpu_oneshot_api.h"

static void GetSourceYuvAttributes(JpgEncOpenParam encOP, YuvAttr* retAttr);
static void CalcSliceHeight(JpgEncOpenParam* encOP, Uint32 sliceHeight);
static Uint8* DumpES(JpgEncHandle handle, jpu_buffer_t bs, JpgEncOutputInfo* pEncOutput, Uint32* rsize/*OUTPUT*/, EndianMode endian);

BOOL JPU_encode_oneShot(EncConfigParam* param, const uint8_t* src, uint8_t* dst, uint32_t* bsize)
{
    JpgEncHandle        handle      = { 0 };
    JpgEncOpenParam     encOP       = { 0 };
    JpgEncParam         encParam    = { 0 };
    JpgEncOutputInfo    outputInfo    = { 0 };
    JpgEncParamSet      encHeaderParam = {0};
    jpu_buffer_t        vbStream     = {0};
    FRAME_BUF *         pFrame[NUM_FRAME_BUF];
    FrameBuffer         frameBuf[NUM_FRAME_BUF];
    JpgRet              ret = JPG_RET_SUCCESS;
    Uint32              i = 0, srcFrameIdx = 0, frameIdx = 0;
    int                 srcFrameFormat = 0;
    int                 framebufWidth = 0, framebufHeight = 0;
    BOOL                suc = FALSE;
    int                 int_reason = 0;
    int                 instIdx = 0;
    Uint32              needFrameBufCount;
    EncConfigParam      encConfig;
    BOOL                boolVal;
    YuvFeeder           yuvFeeder = NULL;
    YuvAttr             sourceAttr;
#ifdef CNM_SIM_PLATFORM
    jpu_sim_context_t   sim_ctx;
#endif
    Uint32              apiVersion;
    Uint32              hwRevision;
    Uint32              hwProductId;
    char                yuvPath[MAX_FILE_PATH];
    Uint32              bitDepth    = 8;
    BSWriter            writer;
    Uint32              esSize;
    Uint32              wSize  = 0;
    Uint8*              data        = NULL;

    memcpy(&encConfig, param, sizeof(EncConfigParam));
    memset(&pFrame[0], 0x00, sizeof(FRAME_BUF *)*NUM_FRAME_BUF);
    memset(&frameBuf[0], 0x00, sizeof(FrameBuffer)*NUM_FRAME_BUF);
    memset(&encHeaderParam, 0x00, sizeof(encHeaderParam));

    encConfig.outNum = 1;
    encConfig.writerType = BSWRITER_MEMORY;

    ret = JPU_Init();
    if (ret != JPG_RET_SUCCESS && ret != JPG_RET_CALLED_BEFORE) {
        JLOG(ERR, "JPU_Init failed Error code is 0x%x \n", ret );
        goto ERR_ENC_INIT;
    }

    JPU_GetVersionInfo(&apiVersion, &hwRevision, &hwProductId);
    JLOG(INFO, "JPU Version API_VERSION=0x%x, HW_REVISION=%d, HW_PRODUCT_ID=0x%x\n", apiVersion, hwRevision, hwProductId);

    if (hwProductId != PRODUCT_ID_CODAJ12) {
        JLOG(ERR, "Error JPU HW_PRODUCT_ID=0x%x is not match with API_VERSION=0x%x\n", hwProductId ,apiVersion);
        goto ERR_ENC_INIT;
    }

    if (strlen(encConfig.cfgFileName) != 0) {
        boolVal = GetJpgEncOpenParam(&encOP, &encConfig);
    }
    else {
        boolVal = getJpgEncOpenParamDefault(&encOP, &encConfig);
    }
    if (boolVal == FALSE) {
        goto ERR_ENC_INIT;
    }

    if (NULL == (writer = BitstreamWriter_Create(encConfig.writerType, &encConfig, dst))) {
        return FALSE;
    }

    if ((encConfig.bsSize%BS_SIZE_ALIGNMENT) != 0 || encConfig.bsSize < MIN_BS_SIZE) {
        JLOG(ERR, "Invalid size of bitstream buffer\n");
        goto ERR_ENC_INIT;
    }

    vbStream.size = encConfig.bsSize;
    if (jdi_allocate_dma_memory(&vbStream) < 0) {
        JLOG(ERR, "fail to allocate bitstream buffer\n" );
        goto ERR_ENC_INIT;
    }

    encOP.intrEnableBit = ((1<<INT_JPU_DONE) | (1<<INT_JPU_ERROR) | (1<<INT_JPU_BIT_BUF_FULL));
    if (encConfig.sliceInterruptEnable)
        encOP.intrEnableBit |= (1<<INT_JPU_SLICE_DONE);
    encOP.streamEndian          = encConfig.StreamEndian;
    encOP.frameEndian           = encConfig.FrameEndian;
    encOP.bitstreamBuffer       = vbStream.phys_addr;
    encOP.bitstreamBufferSize   = vbStream.size;
    encOP.pixelJustification    = encConfig.pixelJustification;
    encOP.rotation              = encConfig.rotation;
    encOP.mirror                = encConfig.mirror;

    if(encOP.packedFormat) {
        if (encOP.packedFormat==PACKED_FORMAT_444 && encOP.sourceFormat != FORMAT_444) {
            JLOG(ERR, "Invalid operation mode : In case of using packed mode. sourceFormat must be FORMAT_444\n" );
            goto ERR_ENC_INIT;
        }
    }

    // srcFrameFormat means that it is original source image format.
    srcFrameFormat = encOP.sourceFormat;
    if (encOP.rotation == 90 || encOP.rotation == 270) {
        if (srcFrameFormat == FORMAT_422) 
            srcFrameFormat = FORMAT_440;
        else if (srcFrameFormat == FORMAT_440) 
            srcFrameFormat = FORMAT_422;
    }
    framebufWidth  = (srcFrameFormat == FORMAT_420 || srcFrameFormat == FORMAT_422) ? JPU_CEIL(16, encOP.picWidth)  : JPU_CEIL(8, encOP.picWidth);
    framebufHeight = (srcFrameFormat == FORMAT_420 || srcFrameFormat == FORMAT_440) ? JPU_CEIL(16, encOP.picHeight) : JPU_CEIL(8, encOP.picHeight);

    CalcSliceHeight(&encOP, encConfig.sliceHeight);

    // Open an instance and get initial information for encoding.
    if ((ret=JPU_EncOpen(&handle, &encOP)) != JPG_RET_SUCCESS) {
        JLOG(ERR, "JPU_EncOpen failed Error code is 0x%x \n", ret);
        goto ERR_ENC_INIT;
    }
   // JPU_EncGiveCommand(handle, ENABLE_LOGGING, NULL);
    instIdx = handle->instIndex;

    JPU_EncGiveCommand(handle, SET_JPG_USE_STUFFING_BYTE_FF, &encConfig.bEnStuffByte);
    if (encConfig.encQualityPercentage > 0)  {
        JPU_EncGiveCommand(handle, SET_JPG_QUALITY_FACTOR, &encConfig.encQualityPercentage);
    }

    needFrameBufCount = ENC_SRC_BUF_NUM;
    bitDepth          = (encOP.jpg12bit == FALSE) ? 8 : 12;

    // Initialize frame buffers for encoding and source frame
    if (!AllocateFrameBuffer(instIdx, encOP.sourceFormat, encOP.chromaInterleave, encOP.packedFormat, 0, FALSE, framebufWidth, framebufHeight, bitDepth, needFrameBufCount)) {
        goto ERR_ENC_OPEN;
    }

    for( i = 0; i < needFrameBufCount; ++i ) {
        pFrame[i] = GetFrameBuffer(instIdx, i);

        frameBuf[i].stride  = pFrame[i]->strideY;
        frameBuf[i].strideC = pFrame[i]->strideC;
        frameBuf[i].bufY    = pFrame[i]->vbY.phys_addr;
        frameBuf[i].bufCb   = pFrame[i]->vbCb.phys_addr;
        frameBuf[i].bufCr   = pFrame[i]->vbCr.phys_addr;
#ifdef SUPPORT_PADDING_UNALIGNED_YUV
        frameBuf[i].fbLumaHeight   = pFrame[i]->fbLumaHeight;
        frameBuf[i].fbChromaHeight   = pFrame[i]->fbChromaHeight;
#endif
    }

    JLOG(INFO, "framebuffer stride = %d, width = %d, height = %d\n", frameBuf[0].stride, framebufWidth, framebufHeight);
    JLOG(INFO, "framebuffer format = %d, packed format = %d, Interleave = %d\n", srcFrameFormat, encOP.packedFormat, encOP.chromaInterleave);

    sprintf(yuvPath, "%s/%s", encConfig.strYuvDir, encConfig.yuvFileName);
    GetSourceYuvAttributes(encOP, &sourceAttr);
    if ((yuvFeeder=YuvFeeder_Create(YUV_FEEDER_MODE_MEMIO, src, sourceAttr, encOP.frameEndian, NULL)) == NULL) {
        goto ERR_ENC_OPEN;
    }

    outputInfo.encodeState = ENCODE_STATE_NEW_FRAME;

    while (TRUE) {
        if (outputInfo.encodeState == ENCODE_STATE_NEW_FRAME) {  // means new frame start
#ifdef CNM_SIM_PLATFORM
            sim_ctx.frameIdx = frameIdx;
            sim_ctx.instIdx = instIdx;
            jdi_set_event_to_sim(&sim_ctx);
#endif
            srcFrameIdx = (frameIdx%ENC_SRC_BUF_NUM);

            wSize = 0;
            // Write picture header
            if (encConfig.encHeaderMode == ENC_HEADER_MODE_NORMAL) {
                encHeaderParam.size = vbStream.size;
                encHeaderParam.pParaSet = malloc(STREAM_BUF_SIZE);
                encHeaderParam.headerMode = ENC_HEADER_MODE_NORMAL;            //Encoder header disable/enable control. Annex:A 1.2.3 item 13
                encHeaderParam.quantMode = JPG_TBL_NORMAL; //JPG_TBL_MERGE    // Merge quantization table. Annex:A 1.2.3 item 7
                encHeaderParam.huffMode  = JPG_TBL_NORMAL; // JPG_TBL_MERGE    //Merge huffman table. Annex:A 1.2.3 item 6
                encHeaderParam.disableAPPMarker = 0;                        //Remove APPn. Annex:A item 11
                encHeaderParam.enableSofStuffing = TRUE;                        //Remove zero stuffing bits before 0xFFDA. Annex:A item 16.
                if (encHeaderParam.headerMode == ENC_HEADER_MODE_NORMAL) {
                    if (encHeaderParam.pParaSet) {
                        //make picture header
                        JPU_EncGiveCommand(handle, ENC_JPG_GET_HEADER, &encHeaderParam); // return exact header size int endHeaderparam.siz;
                        JLOG(INFO, "JPU_EncGiveCommand[ENC_JPG_GET_HEADER] header size=%d\n", encHeaderParam.size);
                        BitstreamWriter_Act(writer, encHeaderParam.pParaSet, encHeaderParam.size, TRUE);
                        free(encHeaderParam.pParaSet);
                        wSize += encHeaderParam.size;
                    }
                }
            }

            if (YuvFeeder_Feed(yuvFeeder, &frameBuf[srcFrameIdx]) == FALSE) {
                goto ERR_ENC_OPEN;
            }

            encParam.sourceFrame = &frameBuf[srcFrameIdx];
        }

        
        // Start encoding a frame.
        ret = JPU_EncStartOneFrame(handle, &encParam);
        if( ret != JPG_RET_SUCCESS ) {
            JLOG(ERR, "JPU_EncStartOneFrame failed Error code is 0x%x \n", ret );
            goto ERR_ENC_OPEN;
        }

        while(1) {
            int_reason = JPU_WaitInterrupt(handle, JPU_INTERRUPT_TIMEOUT_MS);
            if (int_reason == -1) {
                JLOG(ERR, "Error : inst %d timeout happened\n", instIdx);
                JPU_SWReset(handle);
                goto ERR_ENC_OPEN;
            }
            if (int_reason == -2) {
                JLOG(ERR, "Interrupt occurred. but this interrupt is not for my instance\n");
                break;
            }


            if (int_reason & (1<<INT_JPU_DONE) || int_reason & (1<<INT_JPU_SLICE_DONE)) {  // Must catch PIC_DONE interrupt before catching EMPTY interrupt
                // Do no clear INT_JPU_DONE these will be cleared in JPU_EncGetOutputInfo.
                break;
            }

            if (int_reason & (1<<INT_JPU_BIT_BUF_FULL)) {
                JLOG(INFO, "INT_JPU_BIT_BUF_FULL interrupt issued INSTANCE %d \n", instIdx);
                if (NULL == (data=DumpES(handle, vbStream, NULL, &esSize, (EndianMode)encOP.streamEndian))) {
                    JLOG(INFO, "DumpES error inst=%d\n", instIdx);
                    JpgLeaveLock();
                    goto ERR_ENC_OPEN;
                }
                if (FALSE == BitstreamWriter_Act(writer, data, esSize, TRUE)) {
                    JLOG(INFO, "BitstreamWriter_Act error inst=%d\n",instIdx);
                    JpgLeaveLock();
                    goto ERR_ENC_OPEN;
                }
                wSize += esSize;
                free(data), data = NULL;
                JPU_ClrStatus(handle, (1<<INT_JPU_BIT_BUF_FULL));
            }
        }

        if ((ret=JPU_EncGetOutputInfo(handle, &outputInfo)) != JPG_RET_SUCCESS) {
            JLOG(ERR, "JPU_EncGetOutputInfo failed Error code is 0x%x \n", ret );
            goto ERR_ENC_OPEN;
        }

        data=DumpES(handle, vbStream, &outputInfo, &esSize, (EndianMode)encOP.streamEndian);

        if (FALSE == BitstreamWriter_Act(writer, data, esSize, FALSE)) {
            goto ERR_ENC_OPEN;
        }
        wSize += esSize;
        *bsize = wSize;
        free(data), data = NULL;

        if (outputInfo.encodeState == ENCODE_STATE_SLICE_DONE) {
            JLOG(TRACE, "Enc Slice: %d:%d, rdPtr=0x%x, wrPtr=0x%x, slice height=%d\n", instIdx, frameIdx, outputInfo.streamRdPtr, outputInfo.streamWrPtr, outputInfo.encodedSliceYPos);
        }
        else {
            JLOG(TRACE, "Enc: %d:%d, rdPtr=0x%x, wrPtr=0x%x cycles=%d wrSize=%d\n", instIdx, frameIdx, outputInfo.streamRdPtr, outputInfo.streamWrPtr, outputInfo.frameCycle, wSize);
            frameIdx++;
            if (frameIdx > (encConfig.outNum-1)) {
                suc = TRUE;
                break;
            }
        }
    }

    BitstreamWriter_Destroy(writer);

ERR_ENC_OPEN:
    if (data) {
        free(data);
    }

    if (int_reason < 0) {
        JPU_EncGetOutputInfo( handle, &outputInfo );
    }

    FreeFrameBuffer(instIdx);

    if (JPU_EncClose(handle) == JPG_RET_FRAME_NOT_COMPLETE) {
        JPU_EncGetOutputInfo( handle, &outputInfo );
        JPU_EncClose(handle);
    }

    JLOG(INFO, "\nEnc End. instIdx=%d, Tot Frame %d\n" , instIdx, frameIdx);

ERR_ENC_INIT:

    jdi_free_dma_memory(&vbStream);

    if (yuvFeeder != NULL) {
        YuvFeeder_Destroy(yuvFeeder);
    }

    JPU_DeInit();

    return suc;
}

static void GetSourceYuvAttributes(
    JpgEncOpenParam encOP,
    YuvAttr*        retAttr
    )
{
    retAttr->bigEndian         = TRUE;
    retAttr->bpp               = encOP.jpg12bit == TRUE ? 12 : 8;
    retAttr->chromaInterleaved = encOP.chromaInterleave;
    retAttr->packedFormat      = encOP.packedFormat;
    if (encOP.packedFormat == PACKED_FORMAT_NONE) {
        retAttr->format            = encOP.sourceFormat;
    }
    else {
        switch (encOP.packedFormat) {
        case PACKED_FORMAT_422_YUYV:
        case PACKED_FORMAT_422_UYVY:
        case PACKED_FORMAT_422_YVYU:
        case PACKED_FORMAT_422_VYUY:
            retAttr->format = FORMAT_422;
            break;
        case PACKED_FORMAT_444:
            retAttr->format = FORMAT_444;
            break;
        default:
            retAttr->format = FORMAT_MAX;
            break;
        }
    }
    retAttr->width             = encOP.picWidth;
    retAttr->height            = encOP.picHeight;
    retAttr->lsbJustification  = encOP.pixelJustification;

    return;
}

static void CalcSliceHeight(JpgEncOpenParam* encOP, Uint32 sliceHeight)
{
    Uint32 width    = encOP->picWidth;
    Uint32 height   = encOP->picHeight;
    Uint32 aligned_buf_height;
    FrameFormat format = encOP->sourceFormat;

    if (encOP->rotation == 90 || encOP->rotation == 270) {
        width  = encOP->picHeight;
        height = encOP->picWidth;
        if (format == FORMAT_422) format = FORMAT_440;
        else if (format == FORMAT_440) format = FORMAT_422;
    }
    
    if (format == FORMAT_420 || format == FORMAT_440)
        aligned_buf_height = JPU_CEIL(16, height);
    else
        aligned_buf_height = JPU_CEIL(8, height);
    
    if (sliceHeight == 0) {
        if (format == FORMAT_420 || format == FORMAT_440)
            encOP->sliceHeight = aligned_buf_height;
        else
            encOP->sliceHeight = aligned_buf_height;
    }
    else
        encOP->sliceHeight = sliceHeight;
        
    if (encOP->sliceHeight != aligned_buf_height) {
        if (format == FORMAT_420 || format == FORMAT_422)
            encOP->restartInterval = (width+15)/16;
        else
            encOP->restartInterval = (width+7)/8;

        if (format == FORMAT_420 || format == FORMAT_440)
            encOP->restartInterval *= (encOP->sliceHeight/16);
        else
            encOP->restartInterval *= (encOP->sliceHeight/8);
        encOP->sliceInstMode = TRUE;
    }
}

static Uint8* DumpES(JpgEncHandle handle, jpu_buffer_t bs, JpgEncOutputInfo* pEncOutput, Uint32* rsize/*OUTPUT*/, EndianMode endian)
{
    JpgRet          ret = JPG_RET_SUCCESS;
    PhysicalAddress paBsBufStart;
    PhysicalAddress paBsBufEnd;
    PhysicalAddress paBsBufWrPtr;
    PhysicalAddress paBsBufRdPtr;
    int             size        = 0;
    Uint32          loadSize    = 0;
    Uint8*          data        = NULL;

    paBsBufStart = bs.phys_addr;
    paBsBufEnd   = bs.phys_addr + bs.size;

    if (pEncOutput) {
        paBsBufWrPtr = pEncOutput->streamWrPtr;
        paBsBufRdPtr = pEncOutput->streamRdPtr;
        size         = (Uint32)(paBsBufWrPtr - paBsBufRdPtr);;
    }
    else {
        if (JPG_RET_SUCCESS != (ret=JPU_EncGetBitstreamBuffer(handle, &paBsBufRdPtr, &paBsBufWrPtr, &size))) {
            JLOG(ERR, "<%s:%d> JPU_EncGetBitstreamBuffer failed Error code is 0x%x \n", __FUNCTION__, __LINE__, ret);
            return NULL;
        }
    }

    loadSize = size;
    if (loadSize > 0) {
        Uint8* val = 0;
        int room = 0;

        if (NULL == (val=(Uint8*)malloc(loadSize))) {
            JLOG(ERR, "<%s:%d> Failed to allocate memory. size(%d)\n", __FUNCTION__, __LINE__, loadSize);
            return NULL;
        }

        if ((paBsBufRdPtr+size) > paBsBufEnd) {
            room = paBsBufEnd - paBsBufRdPtr;
            JpuReadMem(paBsBufRdPtr, val, room,  endian);
            JpuReadMem(paBsBufStart, val+room, (size-room), endian);
        }
        else {
            JpuReadMem(paBsBufRdPtr, val, size, endian);
        }

        if (pEncOutput && (ENCODE_STATE_FRAME_DONE == pEncOutput->encodeState)) {
            loadSize = STREAM_END_SIZE;	// after done of one frame. set the current wrPtr to to the base address in the bitstream buffer.
        }

        ret = JPU_EncUpdateBitstreamBuffer(handle, loadSize);
        if (JPG_RET_SUCCESS != ret) {
            JLOG(ERR, "<%s:%d> VPU_EncUpdateBitstreamBuffer failed Error code is 0x%x \n", __FUNCTION__, __LINE__, ret);
            free(val);
            return NULL;
        }

        data = val;
    }
    *rsize = size;

    return data;
}
