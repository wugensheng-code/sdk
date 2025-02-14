/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_max_q31.c
 * Description:  Maximum value of a Q31 vector
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

#include "dsp/statistics_functions.h"

/**
  @ingroup groupStats
 */

/**
  @addtogroup Max
  @{
 */

/**
  @brief         Maximum value of a Q31 vector.
  @param[in]     pSrc       points to the input vector
  @param[in]     blockSize  number of samples in input vector
  @param[out]    pResult    maximum value returned here
  @param[out]    pIndex     index of maximum value returned here
  @return        none
 */
void riscv_max_q31(
  const q31_t * pSrc,
        uint32_t blockSize,
        q31_t * pResult,
        uint32_t * pIndex)
{
  q31_t maxVal, out;   /* Temporary variables to store the output value. */
  uint32_t blkCnt, outIndex;   /* Loop counter */

#if defined(RISCV_MATH_VECTOR)
  q31_t max_temp;
  size_t l;
  const q31_t *inputx = pSrc;
  vint32m8_t v_x;
  vint32m1_t v_tempa;
  unsigned long last_suf = 0, temp_index = 0;
  vbool4_t mask;
  out = pSrc[0];
  outIndex = 0;
  l = vsetvl_e32m1(1);
  v_tempa = vmv_s_x_i32m1(v_tempa, pSrc[0], l);
  blkCnt = blockSize;
  for (; (l = vsetvl_e32m8(blkCnt)) > 0; blkCnt -= l)
  {
    v_x = vle32_v_i32m8(inputx, l);
    inputx += l;
    max_temp = vmv_x_s_i32m1_i32(vredmax_vs_i32m8_i32m1(v_tempa,v_x,v_tempa, l));
    if (max_temp > out)
    {
      out = max_temp;
      mask = vmseq_vx_i32m8_b4(v_x, max_temp, l);
      temp_index = vfirst_m_b4(mask, l);
      outIndex = last_suf + temp_index;
    }
    last_suf += l;
  }

#else

#if defined (RISCV_MATH_LOOPUNROLL)
  uint32_t index;          /* index of maximum value */
#endif

  /* Initialise index value to zero. */
  outIndex = 0U;
  /* Load first input value that act as reference value for comparision */
  out = *pSrc++;

#if defined (RISCV_MATH_LOOPUNROLL)
  /* Initialise index of maximum value. */
  index = 0U;

  /* Loop unrolling: Compute 4 outputs at a time */
  blkCnt = (blockSize - 1U) >> 2U;

  while (blkCnt > 0U)
  {
    /* Initialize maxVal to next consecutive values one by one */
    maxVal = *pSrc++;

    /* compare for the maximum value */
    if (out < maxVal)
    {
      /* Update the maximum value and it's index */
      out = maxVal;
      outIndex = index + 1U;
    }

    maxVal = *pSrc++;
    if (out < maxVal)
    {
      out = maxVal;
      outIndex = index + 2U;
    }

    maxVal = *pSrc++;
    if (out < maxVal)
    {
      out = maxVal;
      outIndex = index + 3U;
    }

    maxVal = *pSrc++;
    if (out < maxVal)
    {
      out = maxVal;
      outIndex = index + 4U;
    }

    index += 4U;

    /* Decrement loop counter */
    blkCnt--;
  }

  /* Loop unrolling: Compute remaining outputs */
  blkCnt = (blockSize - 1U) & 3U;

#else

  /* Initialize blkCnt with number of samples */
  blkCnt = (blockSize - 1U);

#endif /* #if defined (RISCV_MATH_LOOPUNROLL) */

  while (blkCnt > 0U)
  {
    /* Initialize maxVal to the next consecutive values one by one */
    maxVal = *pSrc++;

    /* compare for the maximum value */
    if (out < maxVal)
    {
      /* Update the maximum value and it's index */
      out = maxVal;
      outIndex = blockSize - blkCnt;
    }

    /* Decrement loop counter */
    blkCnt--;
  }
#endif /* defined(RISCV_MATH_VECTOR) */
  /* Store the maximum value and it's index into destination pointers */
  *pResult = out;
  *pIndex = outIndex;
}

/**
  @} end of Max group
 */
