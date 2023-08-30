#include <alfsbl_secure.h>
#include "alfsbl_main.h"
#include "alfsbl_image_header.h"
#include "alfsbl_partition_load.h"
#include "alfsbl_handoff.h"
#include "alfsbl_err_lockdown.h"
#include "alfsbl_misc.h"
#include "alfsbl_hw.h"

#include "demosoc.h"
#include "qspi_drv.h"
#include "qspi_flash_drv.h"

#include "alfsbl_qspi.h"


/********************* global variables *********************/
uint8_t  ReadBuffer[READ_BUFFER_SIZE]__attribute__((section(".read_buffer")));
uint8_t  AuthBuffer[ALFSBL_AUTH_BUFFER_SIZE]__attribute__((aligned(4))) = {0};
AlFsblInfo FsblInstance = {0x00U, 0, 0, 0};
SecureInfo FsblSecInfo = {0};

//extern int soc_platform_init();
extern int Soc_PlatInit();

#define SIMU_PASS (0x600d600d)
#define SIMU_FAIL (0x0bad0bad)
void simu_report(uint32_t status)
{
#if defined SIMU_AL9000_DV
	__asm__ __volatile__(
	"csrw mscratch, %[src]"
	:
	:[src]"r"(status)
	);
#endif
	return;
}



int main(void)
{
	uint32_t FsblStage    = ALFSBL_STAGE1;
	uint32_t FsblStatus   = ALFSBL_SUCCESS;
	uint32_t PartitionIdx = 0;

	/// gp normal access to pl,
	/// required in al9000 dv simulation,
	/// to print infomation in simulation env
#if defined SIMU_AL9000_DV
	REG32(SYSCTRL_NS_PLS_PROT) = REG32(SYSCTRL_NS_PLS_PROT) & (~0x2);
#endif

    /// demo board uart pinmux
    REG32(0xf88030c0) = 3;
    REG32(0xf88030c4) = 3;
    REG32(0xf8803414) = 1;

//    (void)soc_platform_init();
	Soc_PlatInit();

	while (FsblStage <= ALFSBL_STAGE_DFT) {
		switch (FsblStage) {
		case ALFSBL_STAGE1:
			printf("==================== In Stage 1 ====================\r\n");

			/// system initialize
			FsblStatus = AlFsbl_Initialize(&FsblInstance);
			if(FsblStatus == ALFSBL_SUCCESS) {
				FsblStage = ALFSBL_STAGE2;
			}
			else {
				FsblStatus += ALFSBL_ERROR_STAGE_1;
				FsblStage = ALFSBL_STAGE_ERR;
			}
			break;

		case ALFSBL_STAGE2:
			printf("==================== In Stage 2 ====================\r\n");

			/// primary boot device initialize
			/// DeviceOps function pointer initialize
			/// read and validate image header
			/// read partition headers

			FsblStatus = AlFsbl_BootDevInitAndHdrValidate(&FsblInstance);

			if(FsblStatus == ALFSBL_STATUS_JTAG) {
				FsblStage = ALFSBL_STAGE4;
			}
			else if(FsblStatus == ALFSBL_SUCCESS) {
				PartitionIdx = 1;
				if(PartitionIdx < FsblInstance.ImageHeader.BootHeader.PartitionNum) {
					FsblStage = ALFSBL_STAGE3;
				}
				else if(PartitionIdx == FsblInstance.ImageHeader.BootHeader.PartitionNum) {
					printf("Only one partition exists in boot image, which is fsbl itself\r\n");
					printf("for test only, if this message reported, test pass and finished\r\n");
					simu_report(SIMU_PASS);
				}
				else if(PartitionIdx > FsblInstance.ImageHeader.BootHeader.PartitionNum) {
					printf("Boot image format invalid or Image Header data incorrect, need to check\r\n");
					FsblStatus = ALFSBL_ERROR_INVALID_PARTITION_NUM;
					FsblStatus += ALFSBL_ERROR_STAGE_2;
					FsblStage = ALFSBL_STAGE_ERR;
				}

			}
			else {
				FsblStatus += ALFSBL_ERROR_STAGE_2;
				FsblStage = ALFSBL_STAGE_ERR;
			}
			break;

		case ALFSBL_STAGE3:
			printf("==================== In Stage 3 ====================\r\n");

			if(FsblInstance.ImageHeader.BootHeader.PartitionNum <= 1) {
				printf("Info: no partition to load...\r\n");
				FsblStage = ALFSBL_STAGE4;
				break;
			}
			/// load and validate partitions
			FsblStatus = AlFsbl_PartitionLoad(&FsblInstance, PartitionIdx);
			PartitionIdx++;

			if(FsblStatus != ALFSBL_SUCCESS) {
				FsblStatus += ALFSBL_ERROR_STAGE_3;
				FsblStage = ALFSBL_STAGE_ERR;
			}
			else if(PartitionIdx == FsblInstance.ImageHeader.BootHeader.PartitionNum) {
				FsblStage = ALFSBL_STAGE4;
			}
			else {
				/**do nothing**/
			}
			break;

		case ALFSBL_STAGE4:
			printf("==================== In Stage 4 ====================\r\n");

			/// handoff to the applications, let the dest cpu run the ps partition.
			FsblStatus = AlFsbl_Handoff(&FsblInstance);

			if(FsblStatus != ALFSBL_SUCCESS) {
				FsblStatus += ALFSBL_ERROR_STAGE_4;
				FsblStage = ALFSBL_STAGE_ERR;
			}
			else {
				/// we should never be here
				FsblStage = ALFSBL_STAGE_DFT;
			}
			break;

		case ALFSBL_STAGE_ERR:
			printf("=================== In Stage Err ===================\r\n");
			printf("fsbl error: %x\r\n", FsblStatus);
			AlFsbl_ErrorLockDown(&FsblInstance, FsblStatus);

			/// we should never be here
			FsblStage = ALFSBL_STAGE_DFT;
			break;

		case ALFSBL_STAGE_DFT:
		default:
			printf("================= In Stage Default =================\r\n");
			printf("In default stage: we should never be here\r\n");
			simu_report(SIMU_PASS);
			break;
		}


		if(FsblStage == ALFSBL_STAGE_DFT) {
			break;
		}
	}


	return 0;
}




