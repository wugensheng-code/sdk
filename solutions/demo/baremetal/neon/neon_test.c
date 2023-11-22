//add for float array. assumed that count is multiple of 4

#include <arm_neon.h>
#include <stdio.h>
#include <math.h>
#include "al_core.h"


void add_float_c(float* dst, float* src1, float* src2, int count)
{
     int i;
     for (i = 0; i < count; i++)
         dst[i] = src1[i] + src2[i];
}

void add_float_neon_intrinsics(float* dst, float* src1, float* src2, int count)
{
     int i;

     for (i = 0; i < count; i += 4)
     {
         float32x4_t in1, in2, out;
         in1 = vld1q_f32(src1);
         src1 += 4;
         in2 = vld1q_f32(src2);
         src2 += 4;
         out = vaddq_f32(in1, in2);
         vst1q_f32(dst, out);
         dst += 4;

         // The following is only an example describing how to use AArch64 specific NEON instructions

#if (defined _AARCH_64 || defined __aarch64__)
         float32_t tmp = vaddvq_f32(in1);
#endif
     }
}

#define EPS (float)(1e-7)

AL_S32 main()
{
    #define SIZE    (128)

    int i = 0;
    float src1[SIZE];
    float src2[SIZE];
    float dst[SIZE], dst1[SIZE], dst2[SIZE];

    for ( ; i < SIZE; i ++) {
        src1[ i ] = i * EPS;
    }

    add_float_c(dst, src1, src2, SIZE);
    add_float_neon_intrinsics(dst1, src1, src2, SIZE);
    add_float_neon_asm(dst2, src1, src2, SIZE);

    for ( ; i < SIZE; i ++) {
        if ((dst[i] - dst1[i] <= EPS) || (dst1[i] - dst2[i] <= EPS)) {
            AL_LOG(AL_LOG_LEVEL_FATAL, "fail: [%d]: %f %f %f \r\n", dst[i], dst1[i], dst2[i]);
        }
    }

    printf("test pass \r\n");
    AL_LOG(AL_LOG_LEVEL_NOTICE, "test pass \r\n");
}