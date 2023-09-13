/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_ALFSBL_DATA_H_
#define __AL_ALFSBL_DATA_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "alfsbl_defines.h"
#include "alfsbl_secure.h"
#include "al_cache.h"

#if defined (CPU_RPU_64)
#define PTRSIZE uint64_t
#elif CPU_RPU_32
#define PTRSIZE uint32_t
#elif CPU_APU_64
#define PTRSIZE uint64_t
#elif CPU_APU_32
#define PTRSIZE uint32_t
#endif




#define READ_BUFFER_SIZE                  (16 * 1024)
#define ALFSBL_AUTH_BUFFER_SIZE           (0x180U)
#define AL_WORD_LENGTH                    (4U)



#define QSPI_FLASH_SEARCH_STEP            (32 * 1024)


#define ALIH_MIN_PARTITIONS			      (1U)
#define ALIH_MAX_PARTITIONS			      (6U)


#define ALIH_BH_SIZE	                  (0x300U)
#define ALIH_IHT_SIZE                     (0x40U)

#define ALIH_PH_SIZE                      (64U)

#define ALIH_BH_ATTRB_OFFSET              (0x8C)
#define ALIH_BH_ATTRB_HD_AUTH_ONLY_MASK   (0x03 << 04)   // no use
#define ALIH_BH_ATTRB_HD_HASH_SEL_MASK    (0x03 << 08)   // no use
#define ALIH_BH_ATTRB_CPU_SEL_MASK        (0x03 << 10)   // no use

#define ALIH_BH_ATTRB_HD_AC_SEL_MASK      (0x03 << 12)
#define ALIH_BH_ATTRB_HD_AC_SEL_NONE      (0x00 << 12)
#define ALIH_BH_ATTRB_HD_AC_SEL_SM2       (0x01 << 12)
#define ALIH_BH_ATTRB_HD_AC_SEL_ECC256  (0x02 << 12)

#define ALIH_BH_ATTRB_HD_PPK_HASH_MASK    (0x03 << 14)

#define ALIH_BH_QSPI_WIDTH_SEL_OFFSET     (0x60)
#define ALIH_BH_IMAGE_ID_OFFSET           (0x64)
#define ALIH_BH_ENC_STATUS_OFFSET         (0x68)
#define ALIH_BH_AC_OFFSET                 (0x90)
#define ALIH_BH_FIRST_PARTIADDR_OFFSET    (0x94)
#define ALIH_BH_PARTINUM_OFFSET           (0x98)
#define ALIH_BH_CHECKSUM_OFFSET           (0x9C)

#define ALIH_BH_ENC_KEY_OFFSET            (0xA0)
#define ALIH_BH_ENC_KEY_LENGTH            (0x20)

#define ALIH_BH_IV_OFFSET                 (0xC0)
#define ALIH_BH_IV_LENGTH                 (0x20)


#define ALIH_PH_ATTRIB_PART_OWNER_MASK     (0x07 << 16)
#define ALIH_PH_ATTRIB_PART_OWNER_FSBL     (0x00 << 16)
#define ALIH_PH_ATTRIB_PART_OWNER_UBOOT    (0x01 << 16)

#define ALIH_PH_ATTRIB_AUTH_TYPE_MASK      (0x03 << 14)
#define ALIH_PH_ATTRIB_AUTH_TYPE_NONE      (0x00 << 14)
#define ALIH_PH_ATTRIB_AUTH_TYPE_SM2       (0x01 << 14)
#define ALIH_PH_ATTRIB_AUTH_TYPE_ECC256    (0x02 << 14)
#define ALIH_PH_ATTRIB_AUTH_TYPE_MAXVAL    (0x02 << 14)

#define ALIH_PH_ATTRIB_ENC_TYPE_MASK       (0x03 << 12)
#define ALIH_PH_ATTRIB_ENC_TYPE_NONE       (0x00 << 12)
#define ALIH_PH_ATTRIB_ENC_TYPE_SM4        (0x01 << 12)
#define ALIH_PH_ATTRIB_ENC_TYPE_AES256     (0x02 << 12)
#define ALIH_PH_ATTRIB_ENC_TYPE_MAXVAL     (0x02 << 12)

#define ALIH_PH_ATTRIB_HASH_TYPE_MASK      (0x03 << 10)
#define ALIH_PH_ATTRIB_HASH_TYPE_NONE      (0x00 << 10)
#define ALIH_PH_ATTRIB_HASH_TYPE_SM3       (0x01 << 10)
#define ALIH_PH_ATTRIB_HASH_TYPE_SHA256    (0x02 << 10)
#define ALIH_PH_ATTRIB_HASH_TYPE_MAXVAL    (0x02 << 10)

#define ALIH_PH_ATTRIB_DEST_DEV_MASK       (0x07 <<  7)
#define ALIH_PH_ATTRIB_DEST_DEV_PS         (0x00 <<  7)
#define ALIH_PH_ATTRIB_DEST_DEV_PL         (0x01 <<  7)

#define ALIH_PH_ATTRIB_DEST_CPU_MASK       (0x03 <<  5)
#define ALIH_PH_ATTRIB_DEST_CPU_RPU        (0x00 <<  5)
#define ALIH_PH_ATTRIB_DEST_CPU_APU0       (0x01 <<  5)
#define ALIH_PH_ATTRIB_DEST_CPU_APU1       (0x02 <<  5)




#define ALAC_ACHEADER_OFFSET         (0x000)
#define ALAC_SPKID_OFFSET            (0x004)
#define ALAC_CUSTOMIZED_OFFSET       (0x008)

#define ALAC_PPK_OFFSET              (0x040)
#define ALAC_PPK_X_OFFSET            (0x040)
#define ALAC_PPK_Y_OFFSET            (0x060)

#define ALAC_SPK_OFFSET              (0x080)
#define ALAC_SPK_X_OFFSET            (0x080)
#define ALAC_SPK_Y_OFFSET            (0x0A0)

#define ALAC_SPK_SIGNATURE_OFFSET    (0x0C0)
#define ALAC_BTHDR_SIGNATURE_OFFSET  (0x100)
#define ALAC_PART_SIGNATURE_OFFSET   (0x140)


typedef struct _AlFsbl_BootHeaderFull_ {
	uint32_t VectorTable[24];      // 0x00~0x5c,   vector table in xip mode, not used
	uint32_t QspiWidthSel;         // 0x60,        qspi bit width select, 0x55aa: x1 mode only, other: auto detect
	uint32_t ImageId;              // 0x64,        header signature
	uint32_t EncStatus;            // 0x68,        encryption encrypt key source
	uint32_t FsblInfo[8];          // 0x6c~0x88,   fsbl information,
	uint32_t BhAttr;               // 0x8c,        bootheader attribute
	uint32_t BhAcOffset;           // 0x90,        bootheader ac offset
	uint32_t FirstPartiHdrOffset;  // 0x94,        1st partition header offset
	uint32_t PartitionNum;         // 0x98,        count of partition headers
	uint32_t BhChecksum;           // 0x9c,        boot header checksum
	uint8_t  EncKey[32];           // 0xa0~0xbc,   encryption key
	uint8_t  SecHdrIv[32];         // 0xc0~0xdc,   secure header iv
	uint8_t  Reserved_E0[32];      // 0xe0~0xfc,   reserved
	uint32_t RegInitVal[128];      // 0x100~0x2fc, register intialization data
} AlFsbl_BootHeaderFull;


typedef struct _AlFsbl_PartitionHeader_ {
	uint32_t PartitionLen;              /// 0x00 Partition Byte Length, including padding
	uint32_t ExtractedPartitionLen;     /// 0x04 Partition Byte Length, not including padding
	uint32_t TotalPartitionLen;         /// 0x08 Partition Total Byte Length, including padding and AC (if exists)
	uint32_t NextPartHdrOffset;         /// 0x0c Next partition header address offset
	uint64_t DestExecAddr;              /// 0x10 partition execution address (64 bit)
	uint64_t DestLoadAddr;              /// 0x18 partition load address (64 bit)
	uint32_t PartitionOffset;           /// 0x20 Actual partition address byte offset
	uint32_t PartitionAttribute;        /// 0x24 Partition attribute
	uint32_t Reserved_28;               /// 0x28 Reserved 0x28
	uint32_t HashDataOffset;            /// 0x2c partition checksum address byte offset
	uint32_t Reserved_30;               /// 0x30 Reserved 0x30
	uint32_t AcOffset;                  /// 0x34 Authentication Certificate byte Offset
	uint32_t Reserved_38;               /// 0x38 Reserved 0x38
	uint32_t PartiHdrChecksum;          /// 0x3c Partition header checksum
} AlFsbl_PartitionHeader;


typedef struct _AlFsbl_ImageHeader_ {
	AlFsbl_BootHeaderFull   BootHeader CACHE_LINE_ALIGN;
	AlFsbl_PartitionHeader  PartitionHeader[ALIH_MAX_PARTITIONS];
} AlFsbl_ImageHeader;


typedef struct _AlFsbl_HandoffValues_ {
	uint32_t CpuSettings;
	uint64_t HandoffAddress;
} AlFsbl_HandoffValues;


typedef struct _AlFsbl_DeviceOps_ {
	uint32_t DeviceBaseAddress;
	uint32_t BlockSizeMax;
	uint32_t (*DeviceInit)(uint32_t *pBlockSizeMax);
	uint32_t (*DeviceCopy)(PTRSIZE SrcAddress, PTRSIZE DestAddress, uint32_t Length, SecureInfo *pSecureInfo);
	uint32_t (*DeviceRelease)(void);
} AlFsbl_DeviceOps;


typedef struct _AlFsblInfo_ {
	uint32_t             Version;
	uint32_t             ProcessorID;
	PTRSIZE              ImageOffsetAddress;
	uint32_t             ErrorCode;
	uint32_t             PrimaryBootDevice;
	uint32_t             HandoffCpuNum;
	uint32_t             ResetReason;
	AlFsbl_DeviceOps     DeviceOps;
	AlFsbl_ImageHeader   ImageHeader;  /// todo, need to definition
	AlFsbl_HandoffValues HandoffValues[3];
} AlFsblInfo;








#ifdef __cplusplus
}
#endif

#endif /* AL_ALFSBL_DATA_H */
