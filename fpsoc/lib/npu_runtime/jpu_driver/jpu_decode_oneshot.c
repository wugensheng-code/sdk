#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jpuapi.h"
#include "regdefine.h"
#include "jpulog.h"
#include "jpuapifunc.h"
#include "jpu_oneshot_api.h"

BOOL JPU_decode_oneShot(DecConfigParam* param, const uint8_t* src, uint32_t bsize, uint32_t* dst_addr, picInfo* retInfo)
{
    JpgDecHandle        handle        = {0};
    JpgDecOpenParam     decOP        = {0};
    JpgDecInitialInfo   initialInfo = {0};
    JpgDecOutputInfo    outputInfo    = {0};
    JpgDecParam         decParam    = {0};
    JpgRet              ret = JPG_RET_SUCCESS;
    FrameBuffer         frameBuf[NUM_FRAME_BUF];
    jpu_buffer_t        vbStream    = {0};
    FRAME_BUF*          pFrame[NUM_FRAME_BUF];
    Uint32              framebufWidth=0, framebufHeight=0, framebufStride = 0;
    Int32               i = 0, frameIdx = 0, saveIdx =0, totalNumofErrMbs = 0;
    BOOL                suc = FALSE;
    // Uint8*              pYuv     =    NULL;
    FILE*               fpYuv     =    NULL;
    Int32               needFrameBufCount = 0;
    Int32               int_reason = 0;
    Int32               instIdx = 0;
    Uint32              outbufSize=0;
    DecConfigParam      decConfig;
    Uint32              decodingWidth, decodingHeight;
    Uint32              displayWidth, displayHeight;
    FrameFormat         subsample;
    Uint32              bitDepth = 0;
    Uint32              temp;
    BOOL                scalerOn = FALSE;
    BSFeeder            feeder;

    memcpy(&decConfig, param, sizeof(DecConfigParam));
    memset(pFrame, 0x00, sizeof(pFrame));
    memset(frameBuf, 0x00, sizeof(frameBuf));

    decConfig.outNum = 1;
    decConfig.feedingMode = FEEDING_METHOD_FIXED_SIZE_MEMIO;
    decConfig.checkeos = 0;

    // Check Parameters
    if ((decConfig.iHorScaleMode || decConfig.iVerScaleMode) && decConfig.roiEnable) {
        JLOG(ERR, "Invalid operation mode : ROI cannot work with the scaler\n");
        return FALSE;
    }
    if(decConfig.packedFormat && decConfig.roiEnable) {
        JLOG(ERR, "Invalid operation mode : ROI cannot work with the packed format conversion\n");
        return FALSE;
    }
    if (decConfig.roiEnable && (decConfig.rotation || decConfig.mirror)) {
        JLOG(ERR, "Invalid operation mode : ROI cannot work with the PPU.\n");
    }
    if (decConfig.packedFormat == PACKED_FORMAT_422_YUYV || 
        decConfig.packedFormat == PACKED_FORMAT_422_UYVY ||
        decConfig.packedFormat == PACKED_FORMAT_422_YVYU ||
        decConfig.packedFormat == PACKED_FORMAT_422_VYUY ||
        decConfig.packedFormat == PACKED_FORMAT_444) {
        if (decConfig.subsample != FORMAT_MAX) {
            JLOG(ERR, "Invalid operation mode : subsample cannot be enabled if ordering is YUYV or UYVY or YVYU or VYUY or AYUV\n");
            return FALSE;
        }
    }

    ret = JPU_Init();
    if (ret != JPG_RET_SUCCESS && ret != JPG_RET_CALLED_BEFORE) {
        suc = 0;
        JLOG(ERR, "JPU_Init failed Error code is 0x%x \n", ret );
        goto ERR_DEC_INIT;
    }

    BSChunk srcChunk = {src, bsize, FALSE};
    if ((feeder=BitstreamFeeder_Create(&srcChunk, decConfig.feedingMode, (EndianMode)decConfig.StreamEndian)) == NULL) {
        goto ERR_DEC_INIT;
    }

    if (strlen(decConfig.yuvFileName)) {
        if ((fpYuv=fopen(decConfig.yuvFileName, "wb")) == NULL) {
            JLOG(ERR, "Can't open %s \n", decConfig.yuvFileName );
            goto ERR_DEC_INIT;
        }
    }

    // Open an instance and get initial information for decoding.
    vbStream.size = (decConfig.bsSize == 0) ? STREAM_BUF_SIZE : decConfig.bsSize;
    vbStream.size = (vbStream.size + 1023) & ~1023; // ceil128(size)
    if (jdi_allocate_dma_memory(&vbStream) < 0) {
        JLOG(ERR, "fail to allocate bitstream buffer\n" );
        goto ERR_DEC_INIT;
    }

    decOP.streamEndian          = decConfig.StreamEndian;
    decOP.frameEndian           = decConfig.FrameEndian;
    decOP.bitstreamBuffer       = vbStream.phys_addr;
    decOP.bitstreamBufferSize   = vbStream.size;
    //set virtual address mapped of physical address
    decOP.pBitStream            = (BYTE *)vbStream.virt_addr; //lint !e511
    decOP.chromaInterleave      = decConfig.cbcrInterleave;
    decOP.packedFormat          = decConfig.packedFormat;
    decOP.roiEnable             = decConfig.roiEnable;
    decOP.roiOffsetX            = decConfig.roiOffsetX;
    decOP.roiOffsetY            = decConfig.roiOffsetY;
    decOP.roiWidth              = decConfig.roiWidth;
    decOP.roiHeight             = decConfig.roiHeight;
    decOP.rotation              = decConfig.rotation;
    decOP.mirror                = decConfig.mirror;
    decOP.pixelJustification    = decConfig.pixelJustification;
    decOP.outputFormat          = decConfig.subsample;
    decOP.intrEnableBit         = ((1<<INT_JPU_DONE) | (1<<INT_JPU_ERROR) | (1<<INT_JPU_BIT_BUF_EMPTY));
    ret = JPU_DecOpen(&handle, &decOP);
    if( ret != JPG_RET_SUCCESS ) {
        JLOG(ERR, "JPU_DecOpen failed Error code is 0x%x \n", ret );
        goto ERR_DEC_INIT;
    }
    instIdx = handle->instIndex;
    //JPU_DecGiveCommand(handle, ENABLE_LOGGING, NULL);

    do {
        /* Fill jpeg data in the bitstream buffer */
        BitstreamFeeder_Act(feeder, handle, &vbStream);

        if ((ret=JPU_DecGetInitialInfo(handle, &initialInfo)) != JPG_RET_SUCCESS) {
            if (JPG_RET_BIT_EMPTY == ret) {
                JLOG(INFO, "<%s:%d> BITSTREAM EMPTY\n", __FUNCTION__, __LINE__);
                continue;
            }
            else {
                JLOG(ERR, "JPU_DecGetInitialInfo failed Error code is 0x%x, inst=%d \n", ret, instIdx);
                goto ERR_DEC_OPEN;
            }
        }
    } while (JPG_RET_SUCCESS != ret);

    if (initialInfo.sourceFormat == FORMAT_420 || initialInfo.sourceFormat == FORMAT_422)
        framebufWidth = JPU_CEIL(16, initialInfo.picWidth);
    else
        framebufWidth  = JPU_CEIL(8, initialInfo.picWidth);

    if (initialInfo.sourceFormat == FORMAT_420 || initialInfo.sourceFormat == FORMAT_440)
        framebufHeight = JPU_CEIL(16, initialInfo.picHeight);
    else
        framebufHeight = JPU_CEIL(8, initialInfo.picHeight);

    decodingWidth  = framebufWidth  >> decConfig.iHorScaleMode;
    decodingHeight = framebufHeight >> decConfig.iVerScaleMode;
    if (decOP.packedFormat != PACKED_FORMAT_NONE && decOP.packedFormat != PACKED_FORMAT_444) {
        // When packed format, scale-down resolution should be multiple of 2.
        decodingWidth  = JPU_CEIL(2, decodingWidth);
    }

    subsample = (decConfig.subsample == FORMAT_MAX) ? initialInfo.sourceFormat : decConfig.subsample;
    temp           = decodingWidth;
    decodingWidth  = (decConfig.rotation == 90 || decConfig.rotation == 270) ? decodingHeight : decodingWidth;
    decodingHeight = (decConfig.rotation == 90 || decConfig.rotation == 270) ? temp           : decodingHeight;
    if(decConfig.roiEnable == TRUE) {
        decodingWidth  = framebufWidth  = initialInfo.roiFrameWidth ;
        decodingHeight = framebufHeight = initialInfo.roiFrameHeight;
    }

    if (0 != decConfig.iHorScaleMode || 0 != decConfig.iVerScaleMode) {
        displayWidth  = JPU_FLOOR(2, (framebufWidth >> decConfig.iHorScaleMode));
        displayHeight = JPU_FLOOR(2, (framebufHeight >> decConfig.iVerScaleMode));
    }
    else {
        displayWidth  = decodingWidth;
        displayHeight = decodingHeight;
    }
    JLOG(INFO, "decodingWidth: %d, decodingHeight: %d\n", decodingWidth, decodingHeight);

    // Check restrictions
    if (decOP.rotation != 0 || decOP.mirror != MIRDIR_NONE) {
        if (decOP.outputFormat != FORMAT_MAX && decOP.outputFormat != initialInfo.sourceFormat) {
            JLOG(ERR, "The rotator cannot work with the format converter together.\n");
            goto ERR_DEC_OPEN;
        }
    }

    JLOG(INFO, "<INSTANCE %d>\n", instIdx);
    JLOG(INFO, "SOURCE PICTURE SIZE : W(%d) H(%d)\n", initialInfo.picWidth, initialInfo.picHeight);
    JLOG(INFO, "DECODED PICTURE SIZE: W(%d) H(%d)\n", displayWidth, displayHeight);
    JLOG(INFO, "SUBSAMPLE           : %d\n",          subsample);

    //Allocate frame buffer
    needFrameBufCount = initialInfo.minFrameBufferCount;
    bitDepth          = initialInfo.bitDepth;
    scalerOn          = (BOOL)(decConfig.iHorScaleMode || decConfig.iVerScaleMode);
    if (AllocateFrameBuffer(instIdx, subsample, decOP.chromaInterleave, decOP.packedFormat, decConfig.rotation, scalerOn, decodingWidth, decodingHeight, bitDepth, needFrameBufCount) == FALSE) {
        JLOG(ERR, "Failed to AllocateFrameBuffer()\n");
        goto ERR_DEC_OPEN;
    }

    for( i = 0; i < needFrameBufCount; ++i ) {
        pFrame[i] = GetFrameBuffer(instIdx, i);
        frameBuf[i].bufY  = pFrame[i]->vbY.phys_addr;
        frameBuf[i].bufCb = pFrame[i]->vbCb.phys_addr;
        if (decOP.chromaInterleave == CBCR_SEPARATED)
            frameBuf[i].bufCr = pFrame[i]->vbCr.phys_addr;
        frameBuf[i].stride  = pFrame[i]->strideY;
        frameBuf[i].strideC = pFrame[i]->strideC;
        frameBuf[i].endian  = decOP.frameEndian;
        frameBuf[i].format  = (FrameFormat)pFrame[i]->Format;
    }
    framebufStride = frameBuf[0].stride;

    outbufSize = decodingWidth * decodingHeight * 3 * (bitDepth+7)/8;
    if (retInfo != NULL) {
        retInfo->picWidth  = decodingWidth;
        retInfo->picHeight = decodingHeight;
        retInfo->subsample = subsample;
    }
    // if ((pYuv=malloc(outbufSize)) == NULL) {
    //     JLOG(ERR, "Fail to allocation memory for display buffer\n");
    //     goto ERR_DEC_OPEN;
    // }

    // Register frame buffers requested by the decoder.
    *dst_addr = frameBuf[0].bufY;
    if ((ret=JPU_DecRegisterFrameBuffer(handle, frameBuf, needFrameBufCount, framebufStride)) != JPG_RET_SUCCESS) {
        JLOG(ERR, "JPU_DecRegisterFrameBuffer failed Error code is 0x%x \n", ret );
        goto ERR_DEC_OPEN;
    }

   if ((ret = JPU_DecGiveCommand(handle, SET_JPG_SCALE_HOR,  &(decConfig.iHorScaleMode))) != JPG_RET_SUCCESS) {
        JLOG(ERR, "JPU_DecGiveCommand[SET_JPG_SCALE_HOR] failed Error code is 0x%x \n", ret );
        goto ERR_DEC_OPEN;
    }
    if ((ret = JPU_DecGiveCommand(handle, SET_JPG_SCALE_VER,  &(decConfig.iVerScaleMode))) != JPG_RET_SUCCESS) {
        JLOG(ERR, "JPU_DecGiveCommand[SET_JPG_SCALE_VER] failed Error code is 0x%x \n", ret );
        goto ERR_DEC_OPEN;
    }

    /* LOG HEADER */
    JLOG(INFO, "I   F    FB_INDEX  FRAME_START  ECS_START  CONSUME   RD_PTR   WR_PTR      CYCLE\n");
    JLOG(INFO, "-------------------------------------------------------------------------------\n");

    while(1) {
        // Start decoding a frame.
        ret = JPU_DecStartOneFrame(handle, &decParam);
        if (ret != JPG_RET_SUCCESS && ret != JPG_RET_EOS) {
            if (ret == JPG_RET_BIT_EMPTY) {
                JLOG(INFO, "BITSTREAM NOT ENOUGH.............\n");
                BitstreamFeeder_Act(feeder, handle, &vbStream);
                continue;
            }

            JLOG(ERR, "JPU_DecStartOneFrame failed Error code is 0x%x \n", ret );
            goto ERR_DEC_OPEN;
        }
        if (ret == JPG_RET_EOS) {
            JPU_DecGetOutputInfo(handle, &outputInfo);
            suc = TRUE;
            break;
        }

        //JLOG(INFO, "\t<+>INSTANCE #%d JPU_WaitInterrupt\n", handle->instIndex);
        while(1) {
            if ((int_reason=JPU_WaitInterrupt(handle, JPU_INTERRUPT_TIMEOUT_MS)) == -1) {
                JLOG(ERR, "Error : timeout happened\n");
                JPU_SWReset(handle);
                break;
            }
            if (int_reason == -2) {
                JLOG(ERR, "Interrupt occurred. but this interrupt is not for my instance \n");
                continue;
            }

            if (int_reason & ((1<<INT_JPU_DONE) | (1<<INT_JPU_ERROR) | (1<<INT_JPU_SLICE_DONE))) {
                // Do no clear INT_JPU_DONE and INT_JPU_ERROR interrupt. these will be cleared in JPU_DecGetOutputInfo.
                JLOG(INFO, "\tINSTANCE #%d int_reason: %08x\n", handle->instIndex, int_reason);
                break;
            }

            if (int_reason & (1<<INT_JPU_BIT_BUF_EMPTY)) {
                if (decConfig.feedingMode != FEEDING_METHOD_FRAME_SIZE) {
                    BitstreamFeeder_Act(feeder, handle, &vbStream);
                }
                JPU_ClrStatus(handle, (1<<INT_JPU_BIT_BUF_EMPTY));
            }
        }
        //JLOG(INFO, "\t<->INSTANCE #%d JPU_WaitInterrupt\n", handle->instIndex);

        if ((ret=JPU_DecGetOutputInfo(handle, &outputInfo)) != JPG_RET_SUCCESS) {
            JLOG(ERR, "JPU_DecGetOutputInfo failed Error code is 0x%x \n", ret );
            goto ERR_DEC_OPEN;
        }

        if (outputInfo.decodingSuccess == 0)
            JLOG(ERR, "JPU_DecGetOutputInfo decode fail framdIdx %d \n", frameIdx);

        JLOG(INFO, "%02d %04d  %8d     %8x %8x %10d  %8x  %8x %10d\n",
            instIdx, frameIdx, outputInfo.indexFrameDisplay, outputInfo.bytePosFrameStart, outputInfo.ecsPtr, outputInfo.consumedByte,
            outputInfo.rdPtr, outputInfo.wrPtr, outputInfo.frameCycle);

        if (outputInfo.indexFrameDisplay == -1)
            break;

        // saveIdx = outputInfo.indexFrameDisplay;
        // if (!SaveYuvImageHelperFormat_V20(fpYuv, dst, &frameBuf[saveIdx], decOP.chromaInterleave, decOP.packedFormat, displayWidth, displayHeight, bitDepth, FALSE)) {
        //     goto ERR_DEC_OPEN;
        // }

        if (outputInfo.numOfErrMBs) {
            Int32 errRstIdx, errPosX, errPosY;
            errRstIdx = (outputInfo.numOfErrMBs & 0x0F000000) >> 24;
            errPosX = (outputInfo.numOfErrMBs & 0x00FFF000) >> 12;
            errPosY = (outputInfo.numOfErrMBs & 0x00000FFF);
            JLOG(ERR, "Error restart Idx : %d, MCU x:%d, y:%d, in Frame : %d \n", errRstIdx, errPosX, errPosY, frameIdx);
        }
        frameIdx++;

        if (decConfig.outNum && (frameIdx == decConfig.outNum)) {
            suc = TRUE;
            break;
        }

        if (decConfig.feedingMode == FEEDING_METHOD_FRAME_SIZE) {
            JPU_DecSetRdPtrEx(handle, vbStream.phys_addr, TRUE);
            BitstreamFeeder_Act(feeder, handle, &vbStream);
        }
    }

    if (totalNumofErrMbs) {
        suc = 0;
        JLOG(ERR, "Total Num of Error MBs : %d\n", totalNumofErrMbs);
    }

ERR_DEC_OPEN:
    FreeFrameBuffer(instIdx);

    // Now that we are done with decoding, close the open instance.
    ret = JPU_DecClose(handle);
    if (ret != JPG_RET_SUCCESS)
        suc = 0;
    JLOG(INFO, "\nDec End. Tot Frame %d\n", frameIdx);

    BitstreamFeeder_Destroy(feeder);

ERR_DEC_INIT:
    jdi_free_dma_memory(&vbStream);

    // if (pYuv)
    //     free(pYuv);

    if (fpYuv)
        fclose(fpYuv);

    JPU_DeInit();

    return suc;
}
