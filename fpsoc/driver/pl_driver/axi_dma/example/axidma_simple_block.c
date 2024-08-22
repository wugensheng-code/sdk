#include "axidma_simple_block.h"

/* Device hardware build related constants. */
#define DMA_DEV_ID                    0
#define GP0_Master                    0x80000000UL
#define STREAM_DATA_GEN_OFFSET        0x100000
#define STREAM_DATA_CHECK_OFFSET      0x200000

#define MAX_PKT_LEN                   0x2000
#define TEST_START_VALUE              0x0
#define TEST_ITERATIONS               256
#define AXIDMA_MM2S_ALIGN_SIZE        (AL_AXI_DMA0_MM2S_MEMORY_MAP_DATA_WIDTH / 8)
#define AXIDMA_S2MM_ALIGN_SIZE        (AL_AXI_DMA0_S2MM_MEMORY_MAP_DATA_WIDTH / 8)

/************************** Function Prototypes ******************************/
AL_S32 AlAxiDma_SimpleTransferExample(AL_U16 DeviceId);
AL_S32 CheckData(AL_VOID);
AL_VOID AlAxiDmaRegDump(AL_U32 baseaddr, AlAxiDma_TransDirEnum Direction);

/************************** Variable Definitions *****************************/
AlAxiDma_InitStruct InitConfig = {
    .DeviceId             = AL_AXI_DMA0_DEVICE_ID,
    .HasKeyHole           = AL_AXI_DMA0_KEYHOLE
};

AL_U8 TransferBuffer[MAX_PKT_LEN] __attribute__((aligned(AXIDMA_MM2S_ALIGN_SIZE)));

AL_S32 main()
{
    AL_S32 Ret = AL_OK;

    AL_LOG(AL_LOG_LEVEL_INFO, "--- Entering main() ---");

    AL_LOG(AL_LOG_LEVEL_INFO, "release gp0 master port");
    AL_REG32_SET_BIT(0xF8800080, 1, 0);

    AL_LOG(AL_LOG_LEVEL_INFO, "release hp0 slave port");
    AL_REG32_SET_BIT(0xF8801078, 0, 1);

    AL_LOG(AL_LOG_LEVEL_INFO, "release gp0 slave port");
    AL_REG32_SET_BIT(0xF8801078, 4, 1);

    AL_LOG(AL_LOG_LEVEL_INFO, "set HP0 HP1 slave data width, 64bit");
    AL_REG32_SET_BIT(0xF8800084, 0, 0);

    AL_LOG(AL_LOG_LEVEL_INFO, "HP0 HP1 slave data width %s", AL_REG32_GET_BIT(0xF8800084, 0) ? "32bit" : "64bit");

    AL_LOG(AL_LOG_LEVEL_INFO, "TransferBuffer addr 0x%p", (void*)TransferBuffer);

    // Loop for multiple tests
    for (int i = 0; i < TEST_ITERATIONS; i++) {
        AL_LOG(AL_LOG_LEVEL_INFO, "---- Test Iteration: %d ----", i + 1);
        memset(TransferBuffer, i, sizeof(TransferBuffer));

        /* Run the example for simple transfer */
        Ret = AlAxiDma_SimpleTransferExample(DMA_DEV_ID);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_INFO, "AlAxiDma_SimpleTransferExample failed on iteration %d with error code %d", i + 1, Ret);
            return Ret;
        }
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "Completed all iterations.");
    AL_LOG(AL_LOG_LEVEL_INFO, "--- Exiting main() ---");

    return Ret;
}

AL_S32 AlAxiDma_SimpleTransferExample(AL_U16 DeviceId)
{
    AlAxiDma_HalStruct *Handle;
    AL_S32 Ret = AL_OK;

    Ret = AlAxiDma_Hal_Init(&Handle, DeviceId, &InitConfig, AL_NULL);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_INFO, "AlAxiDma_Hal_Init failed %d", Ret);
        return Ret;
    }

    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    /* AXI stream generate data */
    AL_LOG(AL_LOG_LEVEL_INFO, "AXI DMA HP0(OCM)<--AXIStream(PL) S2MM Start");
    AL_REG32_WRITE(GP0_Master + STREAM_DATA_GEN_OFFSET + 0x04, MAX_PKT_LEN / (AL_AXI_DMA0_S2MM_STREAM_DATA_WIDTH / 8));
    AL_REG32_WRITE(GP0_Master + STREAM_DATA_GEN_OFFSET + 0x00, TEST_START_VALUE);

    /* receive data form pl's axi stream */
    Ret = AlAxiDma_Hal_Simple_Transfer(Handle, (AL_U64)TransferBuffer, MAX_PKT_LEN, AL_AXIDMA_DEVICE_TO_DMA, AL_WAITFOREVER);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_INFO, "AlAxiDma_Hal_Simple_Transfer S2MM failed %d", Ret);
        return Ret;
    }
    AL_LOG(AL_LOG_LEVEL_INFO, "AXI DMA HP0(OCM)<--AXIStream(PL) S2MM Done");

#if ((AL_AXI_DMA0_HAS_MM2S_DRE == 0) || (AL_AXI_DMA0_HAS_S2MM_DRE == 0))
    Ret = CheckData();
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_INFO, "CheckData failed %d", Ret);
        return Ret;
    }
    AL_LOG(AL_LOG_LEVEL_INFO, "AXI Stream manual data check success!");
#endif

    /* AXI stream is ready check data */
    AL_LOG(AL_LOG_LEVEL_INFO, "AXI DMA HP0(OCM)-->AXIStream(PL) MM2S Start");
    AL_REG32_WRITE(GP0_Master + STREAM_DATA_CHECK_OFFSET + 0x04, MAX_PKT_LEN / (AL_AXI_DMA0_MM2S_STREAM_DATA_WIDTH / 8));
    AL_REG32_WRITE(GP0_Master + STREAM_DATA_CHECK_OFFSET + 0x00, TEST_START_VALUE);

    /* transfer data (received form pl's axi stream) to pl's axi stream */
    Ret = AlAxiDma_Hal_Simple_Transfer(Handle, (AL_U64)TransferBuffer, MAX_PKT_LEN, AL_AXIDMA_DMA_TO_DEVICE, AL_WAITFOREVER);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_INFO, "AlAxiDma_Hal_Simple_Transfer MM2S failed %d", Ret);
        return Ret;
    }
    AL_LOG(AL_LOG_LEVEL_INFO, "AXI DMA HP0(OCM)-->AXIStream(PL) MM2S Done");

    if (AL_REG32_READ(GP0_Master + STREAM_DATA_CHECK_OFFSET + 0x08)) {
        AL_LOG(AL_LOG_LEVEL_INFO, "AXI Stream auto data check fail!");
    } else if (AL_REG32_READ(GP0_Master + STREAM_DATA_CHECK_OFFSET + 0x0c) &&
               AL_REG32_READ(GP0_Master + STREAM_DATA_CHECK_OFFSET + 0x10)) {
        AL_LOG(AL_LOG_LEVEL_INFO, "AXI Stream auto data check success!");
    }

    /* Test finishes successfully */
    return Ret;
}

AL_S32 CheckData(AL_VOID)
{
    AL_U32 *RxPacket;
    AL_U32 Index = 0;
    AL_U32 Value = 0;

    RxPacket = (AL_U32 *)TransferBuffer;
    Value = TEST_START_VALUE;

    for (Index = 0; Index < MAX_PKT_LEN / (AL_AXI_DMA0_MM2S_STREAM_DATA_WIDTH / 8); Index++) {
        if (RxPacket[Index] != Value) {
            AL_LOG(AL_LOG_LEVEL_INFO, "Data error %d: 0x%x/0x%x",
                   Index, (unsigned int)RxPacket[Index], (unsigned int)Value);
            return AL_ERROR;
        }
        Value = (Value + 1);
    }

    return AL_OK;
}

AL_VOID AlAxiDmaRegDump(AL_U32 baseaddr, AlAxiDma_TransDirEnum Direction)
{
    if(Direction == AL_AXIDMA_DMA_TO_DEVICE) {
        AL_LOG(AL_LOG_LEVEL_INFO, "|MM2S DMACR:0x%x",AL_REG32_READ(baseaddr + ALAXIDMA_MM2S_OFFSET + ALAXIDMA_CR_OFFSET));
        AL_LOG(AL_LOG_LEVEL_INFO, "|MM2S DMASR:0x%x",AL_REG32_READ(baseaddr + ALAXIDMA_MM2S_OFFSET + ALAXIDMA_SR_OFFSET));
        AL_LOG(AL_LOG_LEVEL_INFO, "|MM2S SA:0x%x",AL_REG32_READ(baseaddr + ALAXIDMA_MM2S_OFFSET + ALAXIDMA_ADDR_OFFSET));
        AL_LOG(AL_LOG_LEVEL_INFO, "|MM2S SA_MSB:0x%x",AL_REG32_READ(baseaddr + ALAXIDMA_MM2S_OFFSET + ALAXIDMA_ADDR_MSB_OFFSET));
        AL_LOG(AL_LOG_LEVEL_INFO, "|MM2S LENGTH:0x%x",AL_REG32_READ(baseaddr + ALAXIDMA_MM2S_OFFSET + ALAXIDMA_LENTH_OFFSET));
    } else {
        AL_LOG(AL_LOG_LEVEL_INFO, "|S2MM DMACR:0x%x",AL_REG32_READ(baseaddr + ALAXIDMA_S2MM_OFFSET + ALAXIDMA_CR_OFFSET));
        AL_LOG(AL_LOG_LEVEL_INFO, "|S2MM DMASR:0x%x",AL_REG32_READ(baseaddr + ALAXIDMA_S2MM_OFFSET + ALAXIDMA_SR_OFFSET));
        AL_LOG(AL_LOG_LEVEL_INFO, "|S2MM DA:0x%x",AL_REG32_READ(baseaddr + ALAXIDMA_S2MM_OFFSET + ALAXIDMA_ADDR_OFFSET));
        AL_LOG(AL_LOG_LEVEL_INFO, "|S2MM DA_MSB:0x%x",AL_REG32_READ(baseaddr + ALAXIDMA_S2MM_OFFSET + ALAXIDMA_ADDR_MSB_OFFSET));
        AL_LOG(AL_LOG_LEVEL_INFO, "|S2MM LENGTH:0x%x",AL_REG32_READ(baseaddr + ALAXIDMA_S2MM_OFFSET + ALAXIDMA_LENTH_OFFSET));
    }
}
