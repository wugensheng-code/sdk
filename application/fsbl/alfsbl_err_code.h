/*
 * alfsbl_err.h
 *
 *  Created on: May 18, 2022
 *      Author: qsxu
 */

#ifndef ALFSBL_ERR_CODE_H_
#define ALFSBL_ERR_CODE_H_


#ifdef __cplusplus
extern "C" {
#endif


/// error code format (16 bits): XXYY
///
/// YY: error code irrespective of stage
///
/// XX -> x1 x2 x3 x4 x5 x6 x7 x8
///
/// x1: FSBL / CSUROM error
///
/// x2x3: stage which error happened at
///       00 -> error at stage 1
///       01 -> error at stage 2
///       02 -> error at stage 3
///       04 -> error at stage 4
/// x4x5x6x7x8: reserved for now


#define ALFSBL_SUCCESS		                   (0x0)
#define ALFSBL_STATUS_JTAG                     (0x1)
#define ALFSBL_SUCCESS_NOT_PARTITION_OWNER     (0x2)
#define ALFSBL_ERROR_INVALID_PARTITION_NUM     (0x3)

#define ALFSBL_ERROR_UNSUPPORTED_BOOT_MODE   (0x06U)
#define ALFSBL_ERROR_DEVICE_INIT_FAILED      (0x07U)

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

#define ALFSBL_ERROR_PL_INIT_TIMEOUT         (0x07U)

#define ALFSBL_ERROR_STAGE_1          (uint32_t)(0x0000)
#define ALFSBL_ERROR_STAGE_2          (uint32_t)(0x2000)
#define ALFSBL_ERROR_STAGE_3          (uint32_t)(0x4000)
#define ALFSBL_ERROR_STAGE_4          (uint32_t)(0x6000)

#define ALFSBL_ERR_SYS_WDT_RESET      (0x9U)

#ifdef __cplusplus
}
#endif



#endif /* ALFSBL_ERR_CODE_H_ */
