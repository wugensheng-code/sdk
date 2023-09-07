/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_ALFSBL_ERR_CODE_H_
#define __AL_ALFSBL_ERR_CODE_H_


#ifdef __cplusplus
extern "C" {
#endif


/// error code format (32 bits): AABBCCCC
///
/// CCCC: error code irrespective of stage
///
/// AA: stage which error happened at
///       00 -> error at stage 1
///       01 -> error at stage 2
///       02 -> error at stage 3
///       03 -> error at stage 4
///
/// BB: reserved for now


#define ALFSBL_SUCCESS		                   (0x0)
#define ALFSBL_STATUS_JTAG                     (0x1)
#define ALFSBL_SUCCESS_NOT_PARTITION_OWNER     (0x2)
#define ALFSBL_ERROR_INVALID_PARTITION_NUM     (0x3)

#define ALFSBL_ERROR_UNSUPPORTED_BOOT_MODE   (0x06U)
#define ALFSBL_ERROR_DEVICE_INIT_FAILED      (0x07U)

/// error code 0x10~0x16 is each kind of cfg state error status
#define ALFSBL_ERROR_PL_CFG_STATE_ERROR      (0x17U)
#define ALFSBL_ERROR_PL_INIT_TIMEOUT         (0x18U)

#define ALFSBL_ERROR_CHECKSUM_ERROR          (0x60U)
#define ALFSBL_ERROR_IMAGE_HEADER_ACOFFSET   (0x64U)
#define ALFSBL_ERROR_SEC_PARAM_INVALID       (0x65U)


#define ALFSBL_INVALID_DEST_CPU              (0x30U)
#define ALFSBL_INVALID_DEST_DEV              (0x31U)
#define ALFSBL_INVALID_HASH_TYPE             (0x32U)
#define ALFSBL_ENCTYPE_NOT_MATCH_EFUSE       (0x33U)
#define ALFSBL_AUTHTYPE_NOT_MATCH_EFUSE      (0x34U)
#define ALFSBL_SEC_TYPE_MISMACTCH            (0x38U)


#define ALFSBL_INVALID_PARTITION_LENGTH      (0x35U)
#define ALFSBL_INVALID_LOAD_ADDR             (0x36U)
#define ALFSBL_INVALID_EXEC_ADDR             (0x37U)
#define ALFSBL_ERROR_PH_ACOFFSET             (0x38U)

#define ALFSBL_ERROR_INVALID_CSU_ACK         (0x66U)
#define ALFSBL_HASH_FAIL                     (0x67U)
#define ALFSBL_ERROR_AUTH_FAIL               (0x68U)
#define ALFSBL_NO_VALID_PPK                  (0x69U)



#define ALFSBL_ERROR_STAGE_1          (uint32_t)(0x01000000UL)
#define ALFSBL_ERROR_STAGE_2          (uint32_t)(0x02000000UL)
#define ALFSBL_ERROR_STAGE_3          (uint32_t)(0x03000000UL)
#define ALFSBL_ERROR_STAGE_4          (uint32_t)(0x04000000UL)

#define ALFSBL_ERR_SYS_WDT_RESET      (0x9U)

#ifdef __cplusplus
}
#endif



#endif /* AL_ALFSBL_ERR_CODE_H */
