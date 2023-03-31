#include "al_csu_config.h"
#include "al_csu_reg.h"
#include "al_io.h"
#include "msg_send.h"
#include "stdio.h"
#include "test_data.h"

/* Select test items */
#define TEST_ROM
#define TEST_RAM
#define TEST_MSG_RAM
#define TEST_SOC
#define DMA_DDR_LEN 8192
#define DMA_MSG_RAM_LEN 3520
/* Extenal functions prototype */
extern u32 Dma_Test(unsigned int * ibuff, unsigned int * obuff,u32 length);
extern u32 EncHashTest(u32 IsAes, u32 IsModeEcb, u32 IsUserKey);
extern u32 EncryptTest(u32 IsAes, u32 IsModeEcb, u32 IsUserKey);
extern u32 AuthTest(u32 IsEcc);
extern u32 HashTest(u32 IsSha);

u32 PrintBanner()
{
    al_printf("================================================\n");
    al_printf("=================DMA TEST APP===============\n");
    al_printf("================================================\n");

    return 0;
}

u32 SystemBootInfo()
{
    u32 MultiBoot, BootMode, RpuRstVec, ApuRstVec, CsuLifeTime = 0;

    JtagCtrlReg_t JtagCtrl;

    BootMode     = REG32_READ(CSU_BOOT_MODE_ADDR);
    MultiBoot    = REG32_READ(MULTI_BOOT_ADDR);
    CsuLifeTime  = REG32_READ(CSU_LIFETIME_REG_ADDR);
    RpuRstVec    = REG32_READ(RPU_RESET_VECTOR_L_ADDR);
    ApuRstVec    = REG32_READ(APU0_RESET_VECTOR_L_ADDR);
    JtagCtrl.reg = REG32_READ(JTAG_CTRL_ADDR);

    al_printf("Boot Mode: 0x%x\n", BootMode);
    al_printf("Multi-boot: 0x%x\n", MultiBoot);
    al_printf("CSU life time: 0x%x\n", CsuLifeTime);
    al_printf("RPU reset address: 0x%x\n", RpuRstVec);
    al_printf("APU0 reset address: 0x%x\n", ApuRstVec * 4);
    al_printf("System JTAG status:\n");
    al_printf("\tPL TAP off: 0x%x\n", JtagCtrl.pl_tap_off);
    al_printf("\tPS TAP off: 0x%x\n", JtagCtrl.ps_tap_off);
    al_printf("\tARM DAP off: 0x%x\n", JtagCtrl.arm_dap_off);
    al_printf("\tRV TAP off: 0x%x\n", JtagCtrl.rv_tap_off);

    return 0;
}

/**
 * Main function
 */
int main(void)
{
    u32 Status;
    PrintBanner();
    SystemBootInfo();

    SetupIrq();
    *(u32 *)(0xf840e004) = 0x0;
    *(u32 *)(0xf840f004) = 0x0;
    for (u32 i = 0; i < 2048; i++) {
    	*((unsigned int*)(SOC_BASEADDR + i * 4)) = i;
    }
#ifdef TEST_ROM
    al_printf("DMA Access Rom Test Begin:\n");
    Status = Dma_Test((unsigned int*)(ROM_BASEADDR ),(unsigned int*)(RAM_BASEADDR + 0x1000),4096);
    if (AL_SUCCESS != Status) {
        al_printf("DMA Access Rom Test FAILED!\n");
    } else {
        al_printf("DMA Access Rom Test SUCCESS!\n");
    }
#endif

#ifdef TEST_RAM
    al_printf("DMA Access Rom Test Begin:\n");
    Status = Dma_Test((unsigned int*)(SOC_BASEADDR ),(unsigned int*)(RAM_BASEADDR),4096);
    if (AL_SUCCESS != Status) {
        al_printf("DMA Access Ram Test FAILED!\n");
    } else {
        al_printf("DMA Access Ram Test SUCCESS!\n");
    }
#endif

#ifdef TEST_MSG_RAM
    u32 msg_ram_pass_num=0;
    u32 msg_ram_fail_num=0;
    al_printf("DMA Access Msg Ram Test Begin\n");
    while((msg_ram_pass_num < 100) && (msg_ram_fail_num < 100))
    {
		Status = Dma_Test((unsigned int*)(SOC_BASEADDR), \
						  (unsigned int*)(MSG_RAM_CSU + 0x240),DMA_MSG_RAM_LEN);
		Status = CheckBuf((unsigned int*)SOC_BASEADDR, (unsigned int*)(MSG_RAM_BASEADDR + 0x240), DMA_MSG_RAM_LEN / 16);
		if (AL_SUCCESS != Status) {
			msg_ram_fail_num++;
			continue;
		} else {
			msg_ram_pass_num++;
		}
    }
    al_printf("DMA Access DDR Test Pass Num=%d\n",msg_ram_pass_num);
    al_printf("DMA Access DDR Test Fail Num=%d\n",msg_ram_fail_num);
#endif

#ifdef TEST_SOC
    u32 ddr_pass_num=0;
    u32 ddr_fail_num=0;
    al_printf("DMA Access DDR Test Begin\n");
    while((ddr_pass_num < 100) && (ddr_fail_num < 100))
    {
		Status = Dma_Test((unsigned int*)(SOC_BASEADDR), (unsigned int*)(SOC_ENDADDR),DMA_DDR_LEN);
		Status = CheckBuf(SOC_BASEADDR, SOC_ENDADDR, DMA_DDR_LEN / 16);
		if (AL_SUCCESS != Status) {
			ddr_fail_num++;
			continue;
		} else {
			ddr_pass_num++;
		}
    }
    al_printf("DMA Access DDR Test Pass Num=%d\n",ddr_pass_num);
    al_printf("DMA Access DDR Test Fail Num=%d\n",ddr_fail_num);
#endif

#ifdef TEST_MSG_RAM
    msg_ram_pass_num=0;
    msg_ram_fail_num=0;
    al_printf("DMA Access Msg Ram Test Begin\n");
    for (u32 i = 0; i < 110; i++) {
    	*((unsigned int*)(MSG_RAM_BASEADDR + 0x240 + i * 4)) = i;
    }
    while((msg_ram_pass_num < 100) && (msg_ram_fail_num < 100))
    {
		Status = Dma_Test((unsigned int*)(MSG_RAM_CSU + 0x240), \
						  (unsigned int*)(MSG_RAM_CSU + 0x240 + 0x6e0),DMA_MSG_RAM_LEN/2);
		Status = CheckBuf((unsigned int*)SOC_BASEADDR, (unsigned int*)(MSG_RAM_BASEADDR + 0x240), DMA_MSG_RAM_LEN / 2 / 16);
		if (AL_SUCCESS != Status) {
			msg_ram_fail_num++;
			continue;
		} else {
			msg_ram_pass_num++;
		}
    }
    al_printf("DMA Access DDR Test Pass Num=%d\n",msg_ram_pass_num);
    al_printf("DMA Access DDR Test Fail Num=%d\n",msg_ram_fail_num);
#endif
    al_printf("Test Finished\n");
}
