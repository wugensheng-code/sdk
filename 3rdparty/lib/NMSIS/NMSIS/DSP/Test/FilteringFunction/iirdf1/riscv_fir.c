//
// Created by lujun on 19-6-28.
//
// This contains IIR, iir and lms filters
// each one has it's own function.
// All function can be found in main function.
// If you don't want to use it, then comment it.
//
// This part only tests result after 50 dots.If you want to test results' all
// dots, please define  WITH_FRONT
//
// If there are too much numbers which makes it hard to read,in VScode you can
// press alt and z can shrimp them to one line
//
// You MUST be careful about overflow.
#include "riscv_math.h"
#include <stdint.h>
#include <stdio.h>
#include "../common.h"

#include "../HelperFunctions/math_helper.c"
#include "../HelperFunctions/ref_helper.c"

#define SNR_THRESHOLD_F32 (80.0f)

int test_flag_error = 0;

BENCH_DECLARE_VAR();
// #define WITH_FRONT

#define TEST_LENGTH_SAMPLES 320 /* 采样点数 */
#define numStages 2             /* 2阶IIR滤波的个数 */

// f32
float32_t testInput_f32_50Hz_200Hz[TEST_LENGTH_SAMPLES];
float32_t testInput_f32_50Hz_200Hz_temp[TEST_LENGTH_SAMPLES];
float32_t testOutput_f32[TEST_LENGTH_SAMPLES];
float32_t testOutput_f32_ref[TEST_LENGTH_SAMPLES];
float32_t IIRCoeffs32LP[5 * numStages] = {1.0f, 2.0f, 1.0f, 1.11302985416334787593939381622476503253f,  -0.574061915083954765748330828500911593437f,1.0f,  2.0f,  1.0f,   0.855397932775170177777113167394418269396f, - 0.209715357756554754420363906319835223258f};
float32_t IIRStateF32[4 * numStages];

// q31_t
q31_t testInput_q31_50Hz_200Hz[TEST_LENGTH_SAMPLES];
q31_t testOutput_q31[TEST_LENGTH_SAMPLES];
q31_t testOutput_q31_ref[TEST_LENGTH_SAMPLES];
q31_t IIRCoeffsQ31LP[5 * numStages];
q31_t IIRStateq31[4 * numStages];
q63_t IIRState32x64[4 * numStages];

// q15
q15_t testInput_q15_50Hz_200Hz[TEST_LENGTH_SAMPLES];
q15_t testOutput_q15[TEST_LENGTH_SAMPLES];
q15_t testOutput_q15_ref[TEST_LENGTH_SAMPLES];
q15_t IIRCoeffsQ15LP[6 * numStages] = {32767,0,32767,32767,32767,-18810,32767,0,32767,32767,28029,-6871};
q15_t IIRStateq15[4 * numStages];

//***************************************************************************************
//				iir		df1
//***************************************************************************************
static void riscv_iir_f32_lp(void)
{
    generate_rand_f32(testInput_f32_50Hz_200Hz, TEST_LENGTH_SAMPLES);
    memcpy(testInput_f32_50Hz_200Hz_temp, testInput_f32_50Hz_200Hz, sizeof(testInput_f32_50Hz_200Hz));
    /* clang-format off */
    riscv_biquad_casd_df1_inst_f32 S;
    /* clang-format on */
    // float32_t ScaleValue;
    riscv_biquad_cascade_df1_init_f32(&S, numStages, IIRCoeffs32LP, IIRStateF32);
    BENCH_START(riscv_biquad_cascade_df1_f32);
    riscv_biquad_cascade_df1_f32(&S, testInput_f32_50Hz_200Hz, testOutput_f32, TEST_LENGTH_SAMPLES);
    BENCH_END(riscv_biquad_cascade_df1_f32);
    riscv_biquad_cascade_df1_init_f32(&S, numStages, IIRCoeffs32LP, IIRStateF32);
    ref_biquad_cascade_df1_f32(&S, testInput_f32_50Hz_200Hz, testOutput_f32_ref, TEST_LENGTH_SAMPLES);
    // ScaleValue = 0.052219514664161221f * 0.04279801741658381f;
    // 0.115258015230151708574446445254579884931
    // 0.08857935624534613028302487691689748317
#ifndef WITH_FRONT
    float snr = riscv_snr_f32(&testOutput_f32_ref[50], &testOutput_f32[50], 269);

    if (snr < SNR_THRESHOLD_F32) {
        BENCH_ERROR(riscv_biquad_cascade_df1_f32);
        printf("f32 biquad_cascade_df1 failed with snr:%f\n", snr);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_biquad_cascade_df1_f32);
#else
    float snr =
        riscv_snr_f32(&testOutput_ref[0], &testOutput[0], TEST_LENGTH_SAMPLES);

    if (snr < SNR_THRESHOLD_F32) {
        BENCH_ERROR(riscv_biquad_cascade_df1_f32);
        printf("f32 biquad_cascade_df1 failed with snr:%f\n", snr);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_biquad_cascade_df1_f32);
#endif
}
static void riscv_iir_q31_lp(void)
{
    uint32_t i;
    /* clang-format off */
    riscv_biquad_casd_df1_inst_q31 S;
    /* clang-format on */
    // riscv_scale_f32(testInput_f32_50Hz_200Hz, 0.001, testInput_f32_50Hz_200Hz, TEST_LENGTH_SAMPLES);
    //riscv_float_to_q31(testInput_f32_50Hz_200Hz, testInput_q31_50Hz_200Hz, TEST_LENGTH_SAMPLES);
    generate_rand_q31(testInput_q31_50Hz_200Hz, TEST_LENGTH_SAMPLES);
    riscv_float_to_q31(IIRCoeffs32LP, IIRCoeffsQ31LP, 5 * numStages);
    // float32_t ScaleValue;
    riscv_biquad_cascade_df1_init_q31(&S, numStages, IIRCoeffsQ31LP, IIRStateq31, 0);
    BENCH_START(riscv_biquad_cascade_df1_q31);
    riscv_biquad_cascade_df1_q31(&S, testInput_q31_50Hz_200Hz, testOutput_q31, TEST_LENGTH_SAMPLES);
    BENCH_END(riscv_biquad_cascade_df1_q31);
    riscv_biquad_cascade_df1_init_q31(&S, numStages, IIRCoeffsQ31LP, IIRStateq31, 0);
    ref_biquad_cascade_df1_q31(&S, testInput_q31_50Hz_200Hz, testOutput_q31_ref, TEST_LENGTH_SAMPLES);
    // ScaleValue = 0.052219514664161221f * 0.04279801741658381f;
    riscv_q31_to_float(testOutput_q31, testOutput_f32, TEST_LENGTH_SAMPLES);
    riscv_q31_to_float(testOutput_q31_ref, testOutput_f32_ref, TEST_LENGTH_SAMPLES);
#ifndef WITH_FRONT
    float snr = riscv_snr_f32(&testOutput_f32_ref[50], &testOutput_f32[50], 269);

    if (snr < SNR_THRESHOLD_F32) {
        BENCH_ERROR(riscv_biquad_cascade_df1_q31);
        printf("q31 biquad_cascade_df1 failed with snr:%f\n", snr);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_biquad_cascade_df1_q31);
#else
    float snr = riscv_snr_f32(&testOutput_f32_ref[0], &testOutput_f32[0],
                            TEST_LENGTH_SAMPLES);

    if (snr < SNR_THRESHOLD_F32) {
        BENCH_ERROR(riscv_biquad_cascade_df1_q31);
        printf("q31 biquad_cascade_df1 failed with snr:%f\n", snr);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_biquad_cascade_df1_q31);
#endif
}
static void riscv_iir_q15_lp(void)
{
    /* clang-format off */
    riscv_biquad_casd_df1_inst_q15 S;
    /* clang-format on */
    //riscv_scale_f32(testInput_f32_50Hz_200Hz_temp, 0.002, testInput_f32_50Hz_200Hz, TEST_LENGTH_SAMPLES);
    //riscv_float_to_q15(testInput_f32_50Hz_200Hz, testInput_q15_50Hz_200Hz, TEST_LENGTH_SAMPLES);
    generate_rand_q15(testInput_q15_50Hz_200Hz, TEST_LENGTH_SAMPLES);
    float32_t ScaleValue;
    riscv_biquad_cascade_df1_init_q15(&S, numStages, IIRCoeffsQ15LP, IIRStateq15, 1);
    BENCH_START(riscv_biquad_cascade_df1_q15);
    riscv_biquad_cascade_df1_q15(&S, testInput_q15_50Hz_200Hz, testOutput_q15, TEST_LENGTH_SAMPLES);
    BENCH_END(riscv_biquad_cascade_df1_q15);
    riscv_biquad_cascade_df1_init_q15(&S, numStages, IIRCoeffsQ15LP, IIRStateq15, 1);
    ref_biquad_cascade_df1_q15(&S, testInput_q15_50Hz_200Hz, testOutput_q15_ref, TEST_LENGTH_SAMPLES);
    // ScaleValue = 0.052219514664161221f * 0.04279801741658381f;
    riscv_q15_to_float(testOutput_q15, testOutput_f32, TEST_LENGTH_SAMPLES);
    riscv_q15_to_float(testOutput_q15_ref, testOutput_f32_ref, TEST_LENGTH_SAMPLES);
#ifndef WITH_FRONT
    float snr = riscv_snr_f32(&testOutput_f32_ref[50], &testOutput_f32[50], 200);

    if (snr < SNR_THRESHOLD_F32) {
        BENCH_ERROR(riscv_biquad_cascade_df1_q15);
        printf("q15 biquad_cascade_df1 failed with snr:%f\n", snr);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_biquad_cascade_df1_q15);
#else
    float snr = riscv_snr_f32(&testOutput_f32_ref[0], &testOutput_f32[0],
                            TEST_LENGTH_SAMPLES);

    if (snr < SNR_THRESHOLD_F32) {
        BENCH_ERROR(riscv_biquad_cascade_df1_q15);
        printf("q15 biquad_cascade_df1 failed with snr:%f\n", snr);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_biquad_cascade_df1_q15);
#endif
}
static void riscv_iir_q31_fast_lp(void)
{
    /* clang-format off */
    riscv_biquad_casd_df1_inst_q31 S;
    /* clang-format on */
    // riscv_scale_f32(testInput_f32_50Hz_200Hz, 0.001, testInput_f32_50Hz_200Hz, TEST_LENGTH_SAMPLES);
    //riscv_float_to_q31(testInput_f32_50Hz_200Hz, testInput_q31_50Hz_200Hz, TEST_LENGTH_SAMPLES);
    generate_rand_q31(testInput_q31_50Hz_200Hz, TEST_LENGTH_SAMPLES);

    riscv_float_to_q31(IIRCoeffs32LP, IIRCoeffsQ31LP, 5 * numStages);
    // float32_t ScaleValue;
    riscv_biquad_cascade_df1_init_q31(&S, numStages, IIRCoeffsQ31LP, IIRStateq31, 0);
    BENCH_START(riscv_biquad_cascade_df1_fast_q31);
    riscv_biquad_cascade_df1_fast_q31(&S, testInput_q31_50Hz_200Hz, testOutput_q31, TEST_LENGTH_SAMPLES);
    BENCH_END(riscv_biquad_cascade_df1_fast_q31);
    riscv_biquad_cascade_df1_init_q31(&S, numStages, IIRCoeffsQ31LP, IIRStateq31, 0);
    ref_biquad_cascade_df1_fast_q31(&S, testInput_q31_50Hz_200Hz, testOutput_q31_ref, TEST_LENGTH_SAMPLES);
    // ScaleValue = 0.052219514664161221f * 0.04279801741658381f;
    riscv_q31_to_float(testOutput_q31, testOutput_f32, TEST_LENGTH_SAMPLES);
    riscv_q31_to_float(testOutput_q31_ref, testOutput_f32_ref, TEST_LENGTH_SAMPLES);
#ifndef WITH_FRONT
    float snr = riscv_snr_f32(&testOutput_f32_ref[50], &testOutput_f32[50], 269);

    if (snr < SNR_THRESHOLD_F32) {
        BENCH_ERROR(riscv_biquad_cascade_df1_fast_q31);
        printf("q31 biquad_cascade_df1_fast failed with snr:%f\n", snr);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_biquad_cascade_df1_fast_q31);
#else
    float snr = riscv_snr_f32(&testOutput_f32_ref[0], &testOutput_f32[0],
                            TEST_LENGTH_SAMPLES);

    if (snr < SNR_THRESHOLD_F32) {
        BENCH_ERROR(riscv_biquad_cascade_df1_fast_q31);
        printf("q31 biquad_cascade_df1_fast failed with snr:%f\n", snr);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_biquad_cascade_df1_fast_q31);
#endif
}
static void riscv_iir_q15_fast_lp(void)
{
    uint32_t i;
    /* clang-format off */
    riscv_biquad_casd_df1_inst_q15 S;
    /* clang-format on */
    //riscv_scale_f32(testInput_f32_50Hz_200Hz_temp, 0.001, testInput_f32_50Hz_200Hz, TEST_LENGTH_SAMPLES);
    // riscv_float_to_q15(testInput_f32_50Hz_200Hz, testInput_q15_50Hz_200Hz, TEST_LENGTH_SAMPLES);
    generate_rand_q15(testInput_q15_50Hz_200Hz, TEST_LENGTH_SAMPLES);
    // float32_t ScaleValue;
    riscv_biquad_cascade_df1_init_q15(&S, numStages, IIRCoeffsQ15LP, IIRStateq15, 1);
    BENCH_START(riscv_biquad_cascade_df1_fast_q15);
    riscv_biquad_cascade_df1_fast_q15(&S, testInput_q15_50Hz_200Hz, testOutput_q15, TEST_LENGTH_SAMPLES);
    BENCH_END(riscv_biquad_cascade_df1_fast_q15);
    riscv_biquad_cascade_df1_init_q15(&S, numStages, IIRCoeffsQ15LP, IIRStateq15, 1);
    ref_biquad_cascade_df1_fast_q15(&S, testInput_q15_50Hz_200Hz, testOutput_q15_ref, TEST_LENGTH_SAMPLES);
    // ScaleValue
    // = 0.052219514664161221f * 0.04279801741658381f;
    riscv_q15_to_float(testOutput_q15, testOutput_f32, TEST_LENGTH_SAMPLES);
    riscv_q15_to_float(testOutput_q15_ref, testOutput_f32_ref, TEST_LENGTH_SAMPLES);
#ifndef WITH_FRONT
    float snr = riscv_snr_f32(&testOutput_f32_ref[50], &testOutput_f32[50], 269);

    if (snr < SNR_THRESHOLD_F32) {
        BENCH_ERROR(riscv_biquad_cascade_df1_fast_q15);
        printf("q15 biquad_cascade_df1_fast failed with snr:%f\n", snr);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_biquad_cascade_df1_fast_q15);
#else
    float snr = riscv_snr_f32(&testOutput_f32_ref[0], &testOutput_f32[0],
                            TEST_LENGTH_SAMPLES);

    if (snr < SNR_THRESHOLD_F32) {
        BENCH_ERROR(riscv_biquad_cascade_df1_fast_q15);
        printf("q15 biquad_cascade_df1_fast failed with snr:%f\n", snr);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_biquad_cascade_df1_fast_q15);
#endif
}
static void riscv_iir_q31_32x64_lp(void)
{
    uint32_t i;
    /* clang-format off */
    riscv_biquad_cas_df1_32x64_ins_q31 S;
    /* clang-format on */
    // riscv_scale_f32(testInput_f32_50Hz_200Hz, 0.001, testInput_f32_50Hz_200Hz, TEST_LENGTH_SAMPLES);
    //riscv_float_to_q31(testInput_f32_50Hz_200Hz, testInput_q31_50Hz_200Hz, TEST_LENGTH_SAMPLES);
    riscv_float_to_q31(IIRCoeffs32LP, IIRCoeffsQ31LP, 5 * numStages);
    // float32_t ScaleValue;
    riscv_biquad_cas_df1_32x64_init_q31(&S, numStages, IIRCoeffsQ31LP, IIRState32x64, 0);
    BENCH_START(riscv_biquad_cas_df1_32x64_q31);
    riscv_biquad_cas_df1_32x64_q31(&S, testInput_q31_50Hz_200Hz, testOutput_q31, TEST_LENGTH_SAMPLES);
    BENCH_END(riscv_biquad_cas_df1_32x64_q31);
    riscv_biquad_cas_df1_32x64_init_q31(&S, numStages, IIRCoeffsQ31LP, IIRState32x64, 0);
    ref_biquad_cas_df1_32x64_q31(&S, testInput_q31_50Hz_200Hz, testOutput_q31_ref, TEST_LENGTH_SAMPLES);
    // ScaleValue = 0.052219514664161221f * 0.04279801741658381f;
    riscv_q31_to_float(testOutput_q31, testOutput_f32, TEST_LENGTH_SAMPLES);
    riscv_q31_to_float(testOutput_q31_ref, testOutput_f32_ref, TEST_LENGTH_SAMPLES);
#ifndef WITH_FRONT
    float snr = riscv_snr_f32(&testOutput_f32_ref[50], &testOutput_f32[50], 269);

    if (snr < SNR_THRESHOLD_F32) {
        BENCH_ERROR(riscv_biquad_cas_df1_32x64_q31);
        printf("q31 biquad_cas_df1_32x64 failed with snr:%f\n", snr);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_biquad_cas_df1_32x64_q31);
#else
    float snr = riscv_snr_f32(&testOutput_f32_ref[0], &testOutput_f32[0],
                            TEST_LENGTH_SAMPLES);

    if (snr < SNR_THRESHOLD_F32) {
        BENCH_ERROR(riscv_biquad_cas_df1_32x64_q31);
        printf("q31 biquad_cas_df1_32x64 failed with snr:%f\n", snr);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_biquad_cas_df1_32x64_q31);
#endif
}

int main()
{
    BENCH_INIT();

    riscv_iir_f32_lp();
    riscv_iir_q31_lp();
    riscv_iir_q15_lp();
    riscv_iir_q31_fast_lp();
    riscv_iir_q15_fast_lp();
    riscv_iir_q31_32x64_lp();

    if (test_flag_error) {
        printf("test error apprears, please recheck.\n");
        return 1;
    } else {
        printf("all test are passed. Well done!\n");
    }
    return 0;
}
