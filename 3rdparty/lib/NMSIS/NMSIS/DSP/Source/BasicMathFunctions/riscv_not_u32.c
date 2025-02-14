/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_not_u32.c
 * Description:  uint32_t bitwise NOT
 *
 * $Date:        23 April 2021
 * $Revision:    V1.9.0
 *
 * Target Processor: RISC-V Cores
 * -------------------------------------------------------------------- */
/*
 * Copyright (C) 2010-2021 ARM Limited or its affiliates. All rights reserved.
 * Copyright (c) 2019 Nuclei Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "dsp/basic_math_functions.h"

/**
  @ingroup groupMath
 */

/**
  @addtogroup Not
  @{
 */

/**
  @brief         Compute the logical bitwise NOT of a fixed-point vector.
  @param[in]     pSrc       points to input vector
  @param[out]    pDst       points to output vector
  @param[in]     blockSize  number of samples in each vector
  @return        none
 */

void riscv_not_u32(
    const uint32_t * pSrc,
          uint32_t * pDst,
          uint32_t blockSize)
{
    uint32_t blkCnt;      /* Loop counter */

    /* Initialize blkCnt with number of samples */
    blkCnt = blockSize;

#if defined(RISCV_MATH_VECTOR)
    size_t l;
    vuint32m8_t vx;

    for (; (l = vsetvl_e32m8(blkCnt)) > 0; blkCnt -= l)
    {
        vx = vle32_v_u32m8(pSrc, l);
        pSrc += l;
        vse32_v_u32m8(pDst, vnot_v_u32m8(vx, l), l);
        pDst += l;
    }
#else

    const uint64_t * pSrc_temp = (const uint64_t * )pSrc;
    uint64_t * pDst_temp = (uint64_t *)pDst;
    uint32_t *pDst_remain = NULL;
    /* Compute 2 outputs at a time */
    if (blkCnt = blockSize >> 1)
    {
        while (blkCnt > 0U)
        {
            *pDst_temp++ = ~(*pSrc_temp++);

            /* Decrement the loop counter */
            blkCnt--;
        }
    }
    /* Tail */
    if (blkCnt = blockSize & 0x1)
    {
        pSrc = (const uint32_t * )pSrc_temp;
        pDst_remain = (uint32_t *)pDst_temp;
        while (blkCnt > 0U)
        {
            *pDst_remain++ = ~(*pSrc++);

            /* Decrement the loop counter */
            blkCnt--;
        }
    }
#endif /* defined(RISCV_MATH_VECTOR) */
}

/**
  @} end of Not group
 */
