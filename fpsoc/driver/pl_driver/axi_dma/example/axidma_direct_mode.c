#include "al_axi_dma_hal.h"

/* Device hardware build related constants. */
#define DMA_DEV_ID                    0
#define GP0_Master                    0x80000000UL
#define STREAM_DATA_GEN_OFFSET        0x100000UL
#define STREAM_DATA_CHECK_OFFSET      0x200000UL
#define TRANSFER_LEN                  0x2000
#define TEST_START_VALUE              0x0
#define TEST_ITERATIONS               100
#define TRANSFER_METHOD               1  // 1 = BLOCK, 0 = POLLING

/************************** Function Prototypes ******************************/
static AL_VOID ReleasePorts(AL_VOID);
static AL_VOID GenerateData(AL_VOID);
static AL_VOID PrepareDataCheck(AL_VOID);
static AL_S32 DmaTransferCheck(AL_U32 direction);
static AL_S32 AlAxiDma_DirectTransferExample(AL_U16 DeviceId);
static AL_VOID AlAxiDmaRegDump(AlAxiDma_HalStruct *Handle, AlAxiDma_TransDirEnum Direction);

/************************** Variable Definitions *****************************/
AXIDMA_S2MM_BUFFER_ALIGN AL_U8 TransferBuffer[TRANSFER_LEN];

AlAxiDma_InitStruct InitConfig = {0};

AL_S32 main(AL_VOID)
{
    AL_S32 Ret = AL_OK;

    AL_LOG(AL_LOG_LEVEL_INFO, "--- Entering main() ---");

    ReleasePorts();

    // Loop for multiple tests
    for (int i = 0; i < TEST_ITERATIONS; i++) {
        AL_LOG(AL_LOG_LEVEL_INFO, "---- Direct mode test Iteration: %d ----", i + 1);

        /* Run the example for direct transfer */
        Ret = AlAxiDma_DirectTransferExample(DMA_DEV_ID);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_INFO, "Failed on iteration %d with error code 0x%x", i + 1, Ret);
            return Ret;
        }
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "Completed all iterations.");
    AL_LOG(AL_LOG_LEVEL_INFO, "--- Exiting main() ---");

    return Ret;
}

static AL_S32 AlAxiDma_DirectTransferExample(AL_U16 DeviceId)
{
    AlAxiDma_HalStruct *Handle;
    AL_S32 Ret = AL_OK;

    memset(TransferBuffer, 0, sizeof(TransferBuffer));

    Ret = AlAxiDma_Hal_Init(&Handle, DeviceId, &InitConfig, AL_NULL);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_INFO, "AlAxiDma_Hal_Init failed %d", Ret);
        return Ret;
    }

    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);
/************************************ S2MM ******************************************/
    AL_LOG(AL_LOG_LEVEL_INFO, "AXI DMA HP0(OCM/DDR)<--AXIStream(PL) S2MM Start");

    // Generate data for S2MM transfer (DEVICE_TO_DMA)
    GenerateData();

#if TRANSFER_METHOD == 1
    Ret = AlAxiDma_Hal_DirectMode_TransferBlock(Handle, TransferBuffer, TRANSFER_LEN, AL_AXIDMA_DEVICE_TO_DMA, AL_WAITFOREVER);
#else
    Ret = AlAxiDma_Hal_DirectMode_TransferPolling(Handle, TransferBuffer, TRANSFER_LEN, AL_AXIDMA_DEVICE_TO_DMA);
#endif
    if (Ret != AL_OK) {
        return Ret;
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "AXI DMA HP0(OCM/DDR)<--AXIStream(PL) S2MM Done");

    // DATA Check
    Ret = DmaTransferCheck(AL_AXIDMA_DEVICE_TO_DMA);
    if (Ret != AL_OK) {
        return Ret;
    }
/************************************ MM2S ******************************************/
    AL_LOG(AL_LOG_LEVEL_INFO, "AXI DMA HP0(OCM/DDR)-->AXIStream(PL) MM2S Start");

    // Prepare for MM2S transfer check
    PrepareDataCheck();

#if TRANSFER_METHOD == 1
    Ret = AlAxiDma_Hal_DirectMode_TransferBlock(Handle, TransferBuffer, TRANSFER_LEN, AL_AXIDMA_DMA_TO_DEVICE, AL_WAITFOREVER);
#else
    Ret = AlAxiDma_Hal_DirectMode_TransferPolling(Handle, TransferBuffer, TRANSFER_LEN, AL_AXIDMA_DMA_TO_DEVICE);
#endif
    if (Ret != AL_OK) {
        return Ret;
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "AXI DMA HP0(OCM/DDR)-->AXIStream(PL) MM2S Done");

    // DATA Check
    Ret = DmaTransferCheck(AL_AXIDMA_DMA_TO_DEVICE);
    if (Ret != AL_OK) {
        return Ret;
    }

    return Ret;
}

static AL_VOID ReleasePorts(AL_VOID)
{
    AL_LOG(AL_LOG_LEVEL_INFO, "Release gp master port:");
    AL_REG32_SET_BIT(0xF8800080, 1, 0);

    AL_LOG(AL_LOG_LEVEL_INFO, "Release hp0 slave port:");
    AL_REG32_SET_BIT(0xF8801078, 0, 1);

    AL_LOG(AL_LOG_LEVEL_INFO, "Release gp0 slave port:");
    AL_REG32_SET_BIT(0xF8801078, 4, 1);

    AL_LOG(AL_LOG_LEVEL_INFO, "HP0 HP1 slave data width %s!", ((*(unsigned int *)0xF8800084) != 0) ? "32bit" : "64bit");
}

static AL_VOID GenerateData(AL_VOID)
{
    AL_REG32_WRITE(GP0_Master + STREAM_DATA_GEN_OFFSET + 0x04, TRANSFER_LEN / (AL_AXI_DMA0_S2MM_STREAM_DATA_WIDTH / 8));
    AL_REG32_WRITE(GP0_Master + STREAM_DATA_GEN_OFFSET + 0x00, TEST_START_VALUE);
}

static AL_VOID PrepareDataCheck(AL_VOID)
{
    AL_REG32_WRITE(GP0_Master + STREAM_DATA_CHECK_OFFSET + 0x04, TRANSFER_LEN / (AL_AXI_DMA0_MM2S_STREAM_DATA_WIDTH / 8));
    AL_REG32_WRITE(GP0_Master + STREAM_DATA_CHECK_OFFSET + 0x00, TEST_START_VALUE);
}

static AL_S32 DmaTransferCheck(AL_U32 direction)
{
    AL_S32 Ret = AL_OK;

    if (direction == AL_AXIDMA_DEVICE_TO_DMA) {
        // S2MM direction check
        AL_U32 *RxPacket;
        AL_U32 Index = 0;
        AL_U32 Value = 0;

        RxPacket = (AL_U32 *)TransferBuffer;
        Value = TEST_START_VALUE;

        for (Index = 0; Index < TRANSFER_LEN / (AL_AXI_DMA0_S2MM_STREAM_DATA_WIDTH / 8); Index++) {
            if (RxPacket[Index] != Value) {
                AL_LOG(AL_LOG_LEVEL_INFO, "S2MM Data error %d: 0x%x/0x%x",
                       Index, (unsigned int)RxPacket[Index], (unsigned int)Value);
                return AL_ERROR;
            }
            Value = (Value + 1);
        }
        AL_LOG(AL_LOG_LEVEL_INFO, "S2MM AXI Stream manual data check success!");
    }
    else if (direction == AL_AXIDMA_DMA_TO_DEVICE) {
        // MM2S direction check
        if (AL_REG32_READ(GP0_Master + STREAM_DATA_CHECK_OFFSET + 0x08)) {
            AL_LOG(AL_LOG_LEVEL_INFO, "MM2S AXI Stream auto data check fail!");
            Ret = AL_ERROR;
        }
        else if (AL_REG32_READ(GP0_Master + STREAM_DATA_CHECK_OFFSET + 0x0c) &&
                 AL_REG32_READ(GP0_Master + STREAM_DATA_CHECK_OFFSET + 0x10)) {
            AL_LOG(AL_LOG_LEVEL_INFO, "MM2S AXI Stream auto data check success!");
        }
    }

    return Ret;
}

static AL_VOID AlAxiDmaRegDump(AlAxiDma_HalStruct *Handle, AlAxiDma_TransDirEnum Direction)
{
    AL_U64 ChanBase = Handle->Dma.RegBase + (ALAXIDMA_S2MM_OFFSET * Direction);
    const char *direction = (Direction == AL_AXIDMA_DEVICE_TO_DMA) ? "S2MM" : "MM2S";

    AL_LOG(AL_LOG_LEVEL_INFO, "%s DMACR:  0x%x", direction, AL_REG32_READ(ChanBase + ALAXIDMA_CR_OFFSET));
    AL_LOG(AL_LOG_LEVEL_INFO, "%s DMASR:  0x%x", direction, AL_REG32_READ(ChanBase + ALAXIDMA_SR_OFFSET));
    AL_LOG(AL_LOG_LEVEL_INFO, "%s SA:     0x%x", direction, AL_REG32_READ(ChanBase + ALAXIDMA_ADDR_OFFSET));
    AL_LOG(AL_LOG_LEVEL_INFO, "%s SA_MSB: 0x%x", direction, AL_REG32_READ(ChanBase + ALAXIDMA_ADDR_MSB_OFFSET));
    AL_LOG(AL_LOG_LEVEL_INFO, "%s LENGTH: 0x%x", direction, AL_REG32_READ(ChanBase + ALAXIDMA_LENTH_OFFSET));
}
