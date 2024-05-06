/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_ALFSBL_QSPI_H_
#define __AL_ALFSBL_QSPI_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "alfsbl_secure.h"
#include "alfsbl_data.h"
#include "alfsbl_hw.h"

AL_U32 AlFsbl_QspiInit(void);

AL_U32 AlFsbl_Qspi24Init(void);
AL_U32 AlFsbl_Qspi24Copy(PTRSIZE SrcAddress, PTRSIZE DestAddress, AL_U32 Length, SecureInfo *pSecureInfo);
AL_U32 AlFsbl_Qspi24Release(void);
AL_U32 AlFsbl_QspiXipInit(void);

AL_U32 AlFsbl_Qspi32Init(void);
AL_U32 AlFsbl_Qspi32Copy(PTRSIZE SrcAddress, PTRSIZE DestAddress, AL_U32 Length, SecureInfo *pSecureInfo);
AL_U32 AlFsbl_Qspi32Release(void);


#ifdef __cplusplus
}
#endif

#endif /* AL_ALFSBL_QSPI_H */
