/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_absmax_f16.c
 * Description:  Maximum value of a absolute values of a floating-point vector
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

#include "dsp/statistics_functions_f16.h"

#if defined(RISCV_FLOAT16_SUPPORTED)


/**
  @ingroup groupStats
 */


/**
  @addtogroup AbsMax
  @{
 */

/**
  @brief         Maximum value of absolute values of a floating-point vector.
  @param[in]     pSrc       points to the input vector
  @param[in]     blockSize  number of samples in input vector
  @param[out]    pResult    maximum value returned here
  @param[out]    pIndex     index of maximum value returned here
  @return        none
 */

#if defined(RISCV_MATH_LOOPUNROLL)
void riscv_absmax_f16(
  const float16_t * pSrc,
        uint32_t blockSize,
        float16_t * pResult,
        uint32_t * pIndex)
{
        float16_t cur_absmax, out;                     /* Temporary variables to store the output value. */\
        uint32_t blkCnt, outIndex;                     /* Loop counter */                                   \
        uint32_t index;                                /* index of maximum value */                         \
                                                                                                            \
  /* Initialize index value to zero. */                                                                     \
  outIndex = 0U;                                                                                            \
  /* Load first input value that act as reference value for comparision */                                  \
  out = *pSrc++;                                                                                            \
  out = ((_Float16)out > 0.0f16) ? out : -(_Float16)out;                                                                             \
  /* Initialize index of extrema value. */                                                                  \
  index = 0U;                                                                                               \
                                                                                                            \
  /* Loop unrolling: Compute 4 outputs at a time */                                                         \
  blkCnt = (blockSize - 1U) >> 2U;                                                                          \
                                                                                                            \
  while (blkCnt > 0U)                                                                                       \
  {                                                                                                         \
    /* Initialize cur_absmax to next consecutive values one by one */                                         \
    cur_absmax = *pSrc++;                                                                                     \
    cur_absmax = ((_Float16)cur_absmax > 0.0f16) ? cur_absmax : -(_Float16)cur_absmax;                                                                 \
    /* compare for the extrema value */                                                                     \
    if ((_Float16)cur_absmax > (_Float16)out)                                                                         \
    {                                                                                                       \
      /* Update the extrema value and it's index */                                                         \
      out = cur_absmax;                                                                                       \
      outIndex = index + 1U;                                                                                \
    }                                                                                                       \
                                                                                                            \
    cur_absmax = *pSrc++;                                                                                     \
    cur_absmax = ((_Float16)cur_absmax > 0.0f16) ? cur_absmax : -(_Float16)cur_absmax;                                                                 \
    if ((_Float16)cur_absmax > (_Float16)out)                                                                         \
    {                                                                                                       \
      out = cur_absmax;                                                                                       \
      outIndex = index + 2U;                                                                                \
    }                                                                                                       \
                                                                                                            \
    cur_absmax = *pSrc++;                                                                                     \
    cur_absmax = ((_Float16)cur_absmax > 0.0f16) ? cur_absmax : -(_Float16)cur_absmax;                                                                 \
    if ((_Float16)cur_absmax > (_Float16)out)                                                                          \
    {                                                                                                       \
      out = cur_absmax;                                                                                       \
      outIndex = index + 3U;                                                                                \
    }                                                                                                       \
                                                                                                            \
    cur_absmax = *pSrc++;                                                                                     \
    cur_absmax = ((_Float16)cur_absmax > 0.0f16) ? cur_absmax : -(_Float16)cur_absmax;                                                                 \
    if ((_Float16)cur_absmax > (_Float16)out)                                                                          \
    {                                                                                                       \
      out = cur_absmax;                                                                                       \
      outIndex = index + 4U;                                                                                \
    }                                                                                                       \
                                                                                                            \
    index += 4U;                                                                                            \
                                                                                                            \
    /* Decrement loop counter */                                                                            \
    blkCnt--;                                                                                               \
  }                                                                                                         \
                                                                                                            \
  /* Loop unrolling: Compute remaining outputs */                                                           \
  blkCnt = (blockSize - 1U) & 3U;                                                                           \
                                                                                                            \
                                                                                                            \
  while (blkCnt > 0U)                                                                                       \
  {                                                                                                         \
    cur_absmax = *pSrc++;                                                                                     \
    cur_absmax = ((_Float16)cur_absmax > 0.0f16) ? cur_absmax : -(_Float16)cur_absmax;                                                                 \
    if ((_Float16)cur_absmax > (_Float16)out)                                                                         \
    {                                                                                                       \
      out = cur_absmax;                                                                                       \
      outIndex = blockSize - blkCnt;                                                                        \
    }                                                                                                       \
                                                                                                            \
    /* Decrement loop counter */                                                                            \
    blkCnt--;                                                                                               \
  }                                                                                                         \
                                                                                                            \
  /* Store the extrema value and it's index into destination pointers */                                    \
  *pResult = out;                                                                                           \
  *pIndex = outIndex;
}
#else
void riscv_absmax_f16(
  const float16_t * pSrc,
        uint32_t blockSize,
        float16_t * pResult,
        uint32_t * pIndex)
{
        float16_t maxVal, out;                         /* Temporary variables to store the output value. */
        uint32_t blkCnt, outIndex;                     /* Loop counter */

  /* Initialise index value to zero. */
  outIndex = 0U;

  /* Load first input value that act as reference value for comparision */
  out = (_Float16)fabsf((float32_t)*pSrc++);

  /* Initialize blkCnt with number of samples */
  blkCnt = (blockSize - 1U);

  while (blkCnt > 0U)
  {
    /* Initialize maxVal to the next consecutive values one by one */
    maxVal = (_Float16)fabsf((float32_t)*pSrc++);

    /* compare for the maximum value */
    if ((_Float16)out < (_Float16)maxVal)
    {
      /* Update the maximum value and it's index */
      out = maxVal;
      outIndex = blockSize - blkCnt;
    }

    /* Decrement loop counter */
    blkCnt--;
  }

  /* Store the maximum value and it's index into destination pointers */
  *pResult = out;
  *pIndex = outIndex;
}
#endif /* defined(RISCV_MATH_LOOPUNROLL) */
/**
  @} end of AbsMax group
 */

#endif /* #if defined(RISCV_FLOAT16_SUPPORTED) */

