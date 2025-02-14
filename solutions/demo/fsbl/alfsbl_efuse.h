/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_ALFSBL_EFUSE_H_
#define __AL_ALFSBL_EFUSE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "alfsbl_hw.h"

#define EFUSE_GLB_ECC_DW0_OFFSET                     (0U)
#define EFUSE_GLB_ECC_DW0_MASK                       (0xFFU << EFUSE_GLB_ECC_DW0_OFFSET)
#define EFUSE_GLB_ECC_DW1_OFFSET                     (8U)
#define EFUSE_GLB_ECC_DW1_MASK                       (0xFFU << EFUSE_GLB_ECC_DW1_OFFSET)
#define EFUSE_GLB_ECC_DW2_OFFSET                     (16U)
#define EFUSE_GLB_ECC_DW2_MASK                       (0xFFU << EFUSE_GLB_ECC_DW2_OFFSET)
#define EFUSE_GLB_ECC_DW3_OFFSET                     (24U)
#define EFUSE_GLB_ECC_DW3_MASK                       (0xFFU << EFUSE_GLB_ECC_DW3_OFFSET)

#define EFUSE_GLB18_TEST_FIELD_OFFSET                (0U)
#define EFUSE_GLB18_TEST_FIELD_MASK                  (0xffU << EFUSE_GLB18_TEST_FIELD_OFFSET)

#define EFUSE_GLB18_AUTH_TYPE_OFFSET                 (8U)
#define EFUSE_GLB18_AUTH_TYPE_MASK                   (0x03U << EFUSE_GLB18_AUTH_TYPE_OFFSET)
#define EFUSE_GLB18_AUTH_TYPE_HEADER_SET             (0x00U << EFUSE_GLB18_AUTH_TYPE_OFFSET)
#define EFUSE_GLB18_AUTH_TYPE_SM2                    (0x01U << EFUSE_GLB18_AUTH_TYPE_OFFSET)
#define EFUSE_GLB18_AUTH_TYPE_ECDSA256               (0x02U << EFUSE_GLB18_AUTH_TYPE_OFFSET)

#define EFUSE_GLB18_ENC_TYPE_OFFSET                  (10U)
#define EFUSE_GLB18_ENC_TYPE_MASK                    (0x03U << EFUSE_GLB18_ENC_TYPE_OFFSET)
#define EFUSE_GLB18_ENC_TYPE_HEADER_SET              (0x00U << EFUSE_GLB18_ENC_TYPE_OFFSET)
#define EFUSE_GLB18_ENC_TYPE_SM4                     (0x01U << EFUSE_GLB18_ENC_TYPE_OFFSET)
#define EFUSE_GLB18_ENC_TYPE_AES256                  (0x02U << EFUSE_GLB18_ENC_TYPE_OFFSET)

#define EFUSE_GLB18_PPK_HASH_TYPE_OFFSET             (12U)
#define EFUSE_GLB18_PPK_HASH_TYPE_MASK               (0x03U << EFUSE_GLB18_PPK_HASH_TYPE_OFFSET)
#define EFUSE_GLB18_PPK_HASH_TYPE_HEADER_SET         (0x00U << EFUSE_GLB18_PPK_HASH_TYPE_OFFSET)
#define EFUSE_GLB18_PPK_HASH_TYPE_SM3                (0x01U << EFUSE_GLB18_PPK_HASH_TYPE_OFFSET)
#define EFUSE_GLB18_PPK_HASH_TYPE_SHA256             (0x02U << EFUSE_GLB18_PPK_HASH_TYPE_OFFSET)

#define EFUSE_GLB18_ENC_KEY0_WLOCK_OFFSET            (14U)
#define EFUSE_GLB18_ENC_KEY0_WLOCK_MASK              (0x01U << EFUSE_GLB18_ENC_KEY0_WLOCK_OFFSET)
#define EFUSE_GLB18_ENC_KEY0_INVALID_OFFSET          (15U)
#define EFUSE_GLB18_ENC_KEY0_INVALID_MASK            (0x01U << EFUSE_GLB18_ENC_KEY0_INVALID_OFFSET)

#define EFUSE_GLB18_ENC_KEY1_WLOCK_OFFSET            (16U)
#define EFUSE_GLB18_ENC_KEY1_WLOCK_MASK              (0x01U << EFUSE_GLB18_ENC_KEY1_WLOCK_OFFSET)
#define EFUSE_GLB18_ENC_KEY1_INVALID_OFFSET          (17U)
#define EFUSE_GLB18_ENC_KEY1_INVALID_MASK            (0x01U << EFUSE_GLB18_ENC_KEY1_INVALID_OFFSET)

#define EFUSE_GLB18_ENC_KEY_SOURCE_EFUSE_OFFSET      (18U)
#define EFUSE_GLB18_ENC_KEY_SOURCE_EFUSE_MASK        (0x01U << EFUSE_GLB18_ENC_KEY_SOURCE_EFUSE_OFFSET)

#define EFUSE_GLB18_PJTAG_IOMUX_DEFAULT_DIS_OFFSET   (19U)
#define EFUSE_GLB18_PJTAG_IOMUX_DEFAULT_DIS_MASK     (0x01U << EFUSE_GLB18_PJTAG_IOMUX_DEFAULT_DIS_OFFSET)

#define EFUSE_GLB18_PPK0_HASH_WLOCK_OFFSET           (20U)
#define EFUSE_GLB18_PPK0_HASH_WLOCK_MASK             (0x01U << EFUSE_GLB18_PPK0_HASH_WLOCK_OFFSET)
#define EFUSE_GLB18_PPK0_INVALID_OFFSET              (21U)
#define EFUSE_GLB18_PPK0_INVALID_MASK                (0x01U << EFUSE_GLB18_PPK0_INVALID_OFFSET)

#define EFUSE_GLB18_PPK1_HASH_WLOCK_OFFSET           (22U)
#define EFUSE_GLB18_PPK1_HASH_WLOCK_MASK             (0x01U << EFUSE_GLB18_PPK1_HASH_WLOCK_OFFSET)
#define EFUSE_GLB18_PPK1_INVALID_OFFSET              (23U)
#define EFUSE_GLB18_PPK1_INVALID_MASK                (0x01U << EFUSE_GLB18_PPK1_INVALID_OFFSET)

#define EFUSE_GLB18_PSJTAG_IOMUX_DEFAULT_DIS_OFFSET  (24U)
#define EFUSE_GLB18_PSJTAG_IOMUX_DEFAULT_DIS_MASK    (0x01U << EFUSE_GLB18_PSJTAG_IOMUX_DEFAULT_DIS_OFFSET)
#define EFUSE_GLB18_SYSJTAG_DIS_OFFSET               (25U)
#define EFUSE_GLB18_SYSJTAG_DIS_MASK                 (0x01U << EFUSE_GLB18_SYSJTAG_DIS_OFFSET)


#ifdef __cplusplus
}
#endif



#endif /* AL_ALFSBL_EFUSE_H */
