/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_copy_f16.c
 * Description:  Copies the elements of a floating-point vector
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

#include "dsp/support_functions_f16.h"

#if defined(RISCV_FLOAT16_SUPPORTED)


/**
  @ingroup groupSupport
 */


/**
  @addtogroup copy
  @{
 */

/**
  @brief         Copies the elements of a f16 vector.
  @param[in]     pSrc       points to input vector
  @param[out]    pDst       points to output vector
  @param[in]     blockSize  number of samples in each vector
  @return        none
 */

void riscv_copy_f16(
  const float16_t * pSrc,
        float16_t * pDst,
        uint32_t blockSize)
{
  uint32_t blkCnt;                               /* Loop counter */

#if defined (RISCV_MATH_LOOPUNROLL)

  /* Loop unrolling: Compute 4 outputs at a time */
  blkCnt = blockSize >> 2U;

  while (blkCnt > 0U)
  {
    /* C = A */

    /* Copy and store result in destination buffer */
    *pDst++ = *pSrc++;
    *pDst++ = *pSrc++;
    *pDst++ = *pSrc++;
    *pDst++ = *pSrc++;

    /* Decrement loop counter */
    blkCnt--;
  }

  /* Loop unrolling: Compute remaining outputs */
  blkCnt = blockSize & 0x3U;

#else

  /* Initialize blkCnt with number of samples */
  blkCnt = blockSize;

#endif /* #if defined (RISCV_MATH_LOOPUNROLL) */

  while (blkCnt > 0U)
  {
    /* C = A */

    /* Copy and store result in destination buffer */
    *pDst++ = *pSrc++;

    /* Decrement loop counter */
    blkCnt--;
  }
}

/**
  @} end of BasicCopy group
 */

#endif /* #if defined(RISCV_FLOAT16_SUPPORTED) */ 

