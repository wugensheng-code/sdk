/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_cmplx_mag_squared_f16.c
 * Description:  Floating-point complex magnitude squared
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

#include "dsp/complex_math_functions_f16.h"

#if defined(RISCV_FLOAT16_SUPPORTED)

/**
  @ingroup groupCmplxMath
 */


/**
  @addtogroup cmplx_mag_squared
  @{
 */

/**
  @brief         Floating-point complex magnitude squared.
  @param[in]     pSrc        points to input vector
  @param[out]    pDst        points to output vector
  @param[in]     numSamples  number of samples in each vector
  @return        none
 */

void riscv_cmplx_mag_squared_f16(
  const float16_t * pSrc,
        float16_t * pDst,
        uint32_t numSamples)
{
        uint32_t blkCnt;                               /* Loop counter */
        _Float16 real, imag;                          /* Temporary input variables */

#if defined (RISCV_MATH_LOOPUNROLL)

  /* Loop unrolling: Compute 4 outputs at a time */
  blkCnt = numSamples >> 2U;

  while (blkCnt > 0U)
  {
    /* C[0] = (A[0] * A[0] + A[1] * A[1]) */

    real = *pSrc++;
    imag = *pSrc++;
    *pDst++ = (real * real) + (imag * imag);

    real = *pSrc++;
    imag = *pSrc++;
    *pDst++ = (real * real) + (imag * imag);

    real = *pSrc++;
    imag = *pSrc++;
    *pDst++ = (real * real) + (imag * imag);

    real = *pSrc++;
    imag = *pSrc++;
    *pDst++ = (real * real) + (imag * imag);

    /* Decrement loop counter */
    blkCnt--;
  }

  /* Loop unrolling: Compute remaining outputs */
  blkCnt = numSamples & 0x3U;

#else

  /* Initialize blkCnt with number of samples */
  blkCnt = numSamples;

#endif /* #if defined (RISCV_MATH_LOOPUNROLL) */

  while (blkCnt > 0U)
  {
    /* C[0] = (A[0] * A[0] + A[1] * A[1]) */

    real = *pSrc++;
    imag = *pSrc++;

    /* store result in destination buffer. */
    *pDst++ = (real * real) + (imag * imag);

    /* Decrement loop counter */
    blkCnt--;
  }

}

/**
  @} end of cmplx_mag_squared group
 */

#endif /* #if defined(RISCV_FLOAT16_SUPPORTED) */
