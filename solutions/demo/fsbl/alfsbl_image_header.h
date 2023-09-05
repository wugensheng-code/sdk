/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef ALFSBL_IMAGE_HEADER_H_
#define ALFSBL_IMAGE_HEADER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "alfsbl_data.h"
#include "alfsbl_err_code.h"
#include "alfsbl_boot.h"
#include "alfsbl_misc.h"
#include "alfsbl_hw.h"
#include "alfsbl_secure.h"


uint32_t AlFsbl_BootDevInitAndHdrValidate(AlFsblInfo *FsblInstancePtr);
uint32_t AlFsbl_ValidateImageHeader(AlFsblInfo *FsblInstancePtr);

uint32_t AlFsbl_ImgHdrAuth(AlFsblInfo *FsblInstancePtr, uint32_t EfuseCtrl);
uint32_t AlFsbl_PpkVerification(AlFsblInfo *FsblInstancePtr, uint32_t BootHdrAttrb, uint32_t EfuseCtrl);
uint32_t AlFsbl_SpkVerification(AlFsblInfo *FsblInstancePtr, SecureInfo *pFsblIHSecInfo);



#ifdef __cplusplus
}
#endif


#endif /* ALFSBL_IMAGE_HEADER_H_ */
