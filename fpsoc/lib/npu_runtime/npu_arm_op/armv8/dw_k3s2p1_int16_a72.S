//      x0: input data  (int 8)
//      x1: kernel data (int 8)
//      x2: output      (int 8)
//      x3: bias data (int 32)
//      x4: out_h
//      x5: out_w
//      x6: multi
//      x7: shift
//      sp: input_w
//      sp + 8: act_min
//      sp + 16: act_max

// input:
//              0  1  2  3  4  5  6  7  8  9
//      line0:  a0 b0 c0 d0 e0 f0 g0 h0 i0 j0
//      line1:  a1 b1 c1 d1 e1 f1 g1 h1 i1 j1
//      line2:  a2 b2 c2 d2 e2 f2 g2 h2 i2 j2
// weight:
//      v20：k00  k01  k02   0   k00   k01   k02   0
//      v21： 0   k00  k01  k02   0    k00   k01  k02
//      v22：k10  k11  k12   0   k10   k11   k12   0
//      v23： 0   k10  k11  k12   0    k10   k11  k12
//      v24：k20  k21  k22   0   k20   k21   k22   0
//      v25： 0   k20  k21  k22   0    k20   k21   k22

// line 0 : v0,v1
// line 1 : v2,v3
// line 2 : v4,v5
// out_scale: 
// int32x4: v6  ~ v9
// not used: v10 ~ v15
// int32x4: v16 ~ v19
// kernel : v20 ~ v25
// out    : v2,v3   
// mutli  : v27
// bias   : v26
// shift  : v28 v29
// relu   : v30 v31

// line 0:-----------smull-------------> int16x8
//      v7 v8 v9 v10
// line 1:-----------smlal-------------> int16x8
//      v7 v8 v9 v10
//  ------------------sadalp-----------> int32x4
//      v16 v17 v18 v19 = saddlp(v7 v8 v9 v10)
// line 2:
//  ------------------smull------------> int16x8
//      v11 v12 v14 v15
//  -----------------sadalp------------> int32x4
//      v16 v17 v18 v19 = sadalp(v11 v12 v14 v15)

// out:
//    v0.4s = addp v7 v8
//    v1.4s = addp v9 v10
// =======================for each line ============================
// step1:
//      a0    b0   c0   d0   e0   f0   g0   h0     ----> out0   out4
//      k00  k01  k02   0   k00   k01  k02  0
//      a0    b0  c0    d0   e0   f0   g0   h0     ----> out1   out5
//      0    k00  k01   k02  0    k00  k01  k02
// step1: extern
//      c0   d0   e0   f0   g0    h0   i0   j0     ----> out2   out6
//      k00  k01  k02   0   k00   k01  k02   0
//      c0   d0    e0    f0  g0   h0   i0   j0     ----> out3   out7
//      0    k00  k01   k02  0    k00  k01  k02
#ifndef KERNEL_NAME
#define KERNEL_NAME depthwise_k3s2p1_int16_a72
#endif

.text
.align 5
.global KERNEL_NAME
.hidden KERNEL_NAME
.type KERNEL_NAME, %function

KERNEL_NAME:
    sub     sp,sp,0x40
    stp     d8,d9,[sp]
    stp     d10,d11,[sp,0x10]
    stp     d12,d13,[sp,0x20]
    stp     d14,d15,[sp,0x30]

    ldr         x9, [sp, #0x40]     //input_w_pad

    //load the relu value
    ldr         s30,[sp,0x48]
    ldr         s31,[sp,0x50]
    dup         v27.4s, w6          //mutli
    dup         v28.4s, w7          //shift
    dup         v30.4s,v30.s[0]     //min
    dup         v31.4s,v31.s[0]     //max

    //clear the weight vector
    movi d20,0
    movi d21,0
    movi d22,0
    movi d23,0
    movi d24,0
    movi d25,0
    movi d26,0

    smax    v29.4s, v26.4s, v28.4s      //sshl
    smin    v28.4s, v26.4s, v28.4s      //srshl
    cbz     x3, no_bias
    ldr     s26, [x3]
    dup     v26.4s, v26.s[0]
no_bias:
    // init the weight data
    ld1 {v20.h}[0],[x1],#2
    ld1 {v20.h}[1],[x1],#2
    ld1 {v20.h}[2],[x1],#2
    // ext v21.8b,v25.8b,v20.8b,#6
    ld1 {v22.h}[0],[x1],#2
    ld1 {v22.h}[1],[x1],#2
    ld1 {v22.h}[2],[x1],#2
    // ext v23.8b,v25.8b,v22.8b,#6
    ld1 {v24.h}[0],[x1],#2
    ld1 {v24.h}[1],[x1],#2
    ld1 {v24.h}[2],[x1],#2
    // ext v25.8b,v25.8b,v24.8b,#6

    mov     x7, x4

loop_h:
    mov     x10, x0
    add     x11, x0, x9               // input line1
    add     x12, x0, x9, lsl #1       // input line2
    subs    x8, x5, #0x2
    lsr     x6, x8, #0x2
    cmp     x4, #0x1
    beq     last_row
    cmp     x4, x7
    bne     mid_row

first_row_left:
    ld1     {v0.8h},[x10]
    ld1     {v1.8h},[x11]

    ins     v3.h[0],v22.h[1]
    ins     v3.h[1],v22.h[2]
    ins     v4.h[0],v24.h[1]
    ins     v4.h[1],v24.h[2]

    smull       v6.4s, v0.4h, v3.4h
    smlal       v6.4s, v1.4h, v4.4h
    saddlp      v7.2d, v6.4s
    add         v7.4s, v7.4s, v26.4s    // add bias
    SQRDMULH    v2.4s, v7.4s, v27.4s    // multi
    sshl        v2.4s, v2.4s, v29.4s    // left shift
    srshl       v0.4s, v2.4s, v28.4s    // Signed Rounding Shift Left
    smax        v0.4s, v0.4s, v30.4s
    smin        v0.4s, v0.4s, v31.4s

    st1     {v0.b}[0], [x2], 0x1

    add     x10, x10, #0x2
    add     x11, x11, #0x2

first_row_mid4:
    cmp     x6, #0x0
    beq     first_row_remains
    ld1     {v0.8h},[x10]
    ld1     {v1.8h},[x11]
    ext     v3.16b, v0.16b, v0.16b, #4
    ext     v4.16b, v1.16b, v1.16b, #4

    add     x10, x10, #0x8
    add     x11, x11, #0x8

    ld1     {v6.8h},[x10]
    ld1     {v7.8h},[x11]
    ext     v9.16b, v6.16b, v6.16b, #4
    ext     v10.16b, v7.16b, v7.16b, #4

    smull   v16.4s, v0.4h, v22.4h      //k1x  0     a00 x k10, a01 x k11, a02 x k12,     0
    smull   v17.4s, v3.4h, v22.4h      //k1x  1     a02 x k10, a03 x k11, a04 x k12,     0
    smull   v18.4s, v6.4h, v22.4h      //k1x  2     a04 x k10, a05 x k11, a06 x k12,     0
    smull   v19.4s, v9.4h, v22.4h      //k1x  3     a06 x k10, a07 x k11, a08 x k12,     0

    smlal   v16.4s, v1.4h, v24.4h      //k2x  0     a10 x k20, a11 x k21, a12 x k22,     0
    smlal   v17.4s, v4.4h, v24.4h      //k2x  1     a12 x k20, a13 x k21, a14 x k22,     0
    smlal   v18.4s, v7.4h, v24.4h      //k2x  2     a14 x k20, a15 x k21, a16 x k22,     0
    smlal   v19.4s, v10.4h, v24.4h     //k2x  3     a16 x k20, a17 x k21, a18 x k22,     0

    saddlp  v16.2d, v16.4s             // out0
    saddlp  v17.2d, v17.4s             // out1
    saddlp  v18.2d, v18.4s             // out2
    saddlp  v19.2d, v19.4s             // out3

    addp    v2.2d,  v16.2d, v17.2d    // out0 out1
    addp    v3.2d,  v18.2d, v19.2d    // out2 out3

    ins     v2.4s[0], v2.4s[0]
    ins     v2.4s[1], v2.4s[2]
    ins     v2.4s[2], v3.4s[0]
    ins     v2.4s[3], v3.4s[2]

    add     v2.4s,  v2.4s,  v26.4s

    SQRDMULH    v2.4s, v2.4s, v27.4s
    sshl        v2.4s, v2.4s, v29.4s
    srshl       v0.4s, v2.4s, v28.4s
    smax        v0.4s, v0.4s, v30.4s
    smin        v0.4s, v0.4s, v31.4s

    // storage
    st1     {v0.b}[0], [x2], 0x1
    st1     {v0.b}[4], [x2], 0x1
    st1     {v0.b}[8], [x2], 0x1
    st1     {v0.b}[12], [x2], 0x1

    // loop control
    add     x10, x10, #0x8
    add     x11, x11, #0x8
    subs    x6, x6, 0x1
    cmp     x6, #0x0
    bne     first_row_mid4

first_row_remains:
    and     x6, x8, #0x3
    cmp     x6, #0x0
    beq     first_row_right

    ld1     {v0.8h},[x10]
    ld1     {v1.8h},[x11]
    ext     v3.16b, v0.16b, v0.16b, #4
    ext     v4.16b, v1.16b, v1.16b, #4

    add     x10, x10, #0x8
    add     x11, x11, #0x8

    ld1     {v6.8h},[x10]
    ld1     {v7.8h},[x11]
    ext     v9.16b, v6.16b, v6.16b, #4
    ext     v10.16b, v7.16b, v7.16b, #4

    sub     x10, x10, #0x8
    sub     x11, x11, #0x8

    smull   v16.4s, v0.4h, v22.4h      //k1x  0     a00 x k10, a01 x k11, a02 x k12,     0
    smull   v17.4s, v3.4h, v22.4h      //k1x  1     a02 x k10, a03 x k11, a04 x k12,     0
    smull   v18.4s, v6.4h, v22.4h      //k1x  2     a04 x k10, a05 x k11, a06 x k12,     0
    smull   v19.4s, v9.4h, v22.4h      //k1x  3     a06 x k10, a07 x k11, a08 x k12,     0

    smlal   v16.4s, v1.4h, v24.4h      //k2x  0     a10 x k20, a11 x k21, a12 x k22,     0
    smlal   v17.4s, v4.4h, v24.4h      //k2x  1     a12 x k20, a13 x k21, a14 x k22,     0
    smlal   v18.4s, v7.4h, v24.4h      //k2x  2     a14 x k20, a15 x k21, a16 x k22,     0
    smlal   v19.4s, v10.4h, v24.4h     //k2x  3     a16 x k20, a17 x k21, a18 x k22,     0

    saddlp  v16.2d, v16.4s             // out0
    saddlp  v17.2d, v17.4s             // out1
    saddlp  v18.2d, v18.4s             // out2
    saddlp  v19.2d, v19.4s             // out3

    addp    v2.2d,  v16.2d, v17.2d    // out0 out1
    addp    v3.2d,  v18.2d, v19.2d    // out2 out3

    ins     v2.4s[0], v2.4s[0]
    ins     v2.4s[1], v2.4s[2]
    ins     v2.4s[2], v3.4s[0]
    ins     v2.4s[3], v3.4s[2]

    add     v2.4s,  v2.4s,  v26.4s

    SQRDMULH    v2.4s, v2.4s, v27.4s
    sshl        v2.4s, v2.4s, v29.4s
    srshl       v0.4s, v2.4s, v28.4s
    smax        v0.4s, v0.4s, v30.4s
    smin        v0.4s, v0.4s, v31.4s

 first_row_save_1:
    // If remains less than 4 is valid, save data one by one.
    st1    {v0.b}[0], [x2], 0x1
    add    x10, x10, #0x4
    add    x11, x11, #0x4
    ext    v0.16b, v0.16b, v0.16b, #4
    subs   x6, x6, #0x1
    bne    first_row_save_1

first_row_right:
    ld1         {v0.8h},[x10]
    ld1         {v1.8h},[x11]

    ins     v0.h[3],v21.h[0]
    ins     v1.h[3],v21.h[0]

    lsr     x14, x9, #0x1
    and     x6, x14, #0x1
    cmp     x6, #0x0
    beq     first_even
    ins     v0.h[2],v21.h[0]
    ins     v1.h[2],v21.h[0]

first_even:
    smull       v6.4s, v0.4h, v22.4h
    smlal       v6.4s, v1.4h, v24.4h
    saddlp      v7.2d, v6.4s
    ins         v7.s[1], v7.s[2]
    saddlp      v7.2d, v7.4s

    add         v7.4s, v7.4s, v26.4s    // add bias
    SQRDMULH    v2.4s, v7.4s, v27.4s    // multi
    sshl        v2.4s, v2.4s, v29.4s    // left shift
    srshl       v0.4s, v2.4s, v28.4s    // Signed Rounding Shift Left
    smax        v0.4s, v0.4s, v30.4s
    smin        v0.4s, v0.4s, v31.4s
    st1         {v0.b}[0], [x2], 0x1
    add         x0, x0, x9
    b           loop_h_end

mid_row:
mid_row_left:
    ld1     {v0.8h}, [x10]
    ld1     {v1.8h}, [x11]
    ld1     {v2.8h}, [x12]

    ins     v3.h[0],v20.h[1]
    ins     v3.h[1],v20.h[2]
    ins     v4.h[0],v22.h[1]
    ins     v4.h[1],v22.h[2]
    ins     v5.h[0],v24.h[1]
    ins     v5.h[1],v24.h[2]

    smull       v6.4s, v0.4h, v3.4h
    smlal       v6.4s, v1.4h, v4.4h
    saddlp      v7.2d, v6.4s
    smull       v6.4s, v2.4h, v5.4h
    saddlp      v5.2d, v6.4s
    add         v7.2d, v7.2d, v5.2d
    add         v7.4s, v7.4s, v26.4s  // add bias
    SQRDMULH    v2.4s, v7.4s, v27.4s    // multi
    sshl        v2.4s, v2.4s, v29.4s    // left shift
    srshl       v0.4s, v2.4s, v28.4s    // right shift
    smax        v0.4s, v0.4s, v30.4s
    smin        v0.4s, v0.4s, v31.4s

    st1         {v0.b}[0], [x2], 0x1

    add     x10, x10, #0x2
    add     x11, x11, #0x2
    add     x12, x12, #0x2

mid_row_mid4:
    cmp     x6, #0x0
    beq     mid_row_remains

    ld1     {v0.8h}, [x10]              // a0x
    ld1     {v1.8h}, [x11]              // a1x
    ld1     {v2.8h}, [x12]              // a2x

    ext     v3.16b, v0.16b, v0.16b, #4
    ext     v4.16b, v1.16b, v1.16b, #4
    ext     v5.16b, v2.16b, v2.16b, #4

    add     x10, x10, #0x8
    add     x11, x11, #0x8
    add     x12, x12, #0x8

    ld1     {v6.8h}, [x10]              // a0x
    ld1     {v7.8h}, [x11]              // a1x
    ld1     {v8.8h}, [x12]              // a2x

    ext     v9.16b,  v6.16b, v6.16b, #4
    ext     v10.16b, v7.16b, v7.16b, #4
    ext     v11.16b, v8.16b, v8.16b, #4

    smull   v16.4s, v0.4h, v20.4h      //k0x  0     a00 x k10, a01 x k11, a02 x k02,     0
    smull   v17.4s, v3.4h, v20.4h      //k0x  1     a02 x k10, a03 x k11, a04 x k02,     0
    smull   v18.4s, v6.4h, v20.4h      //k0x  2     a04 x k10, a05 x k11, a06 x k02,     0
    smull   v19.4s, v9.4h, v20.4h      //k0x  3     a06 x k10, a07 x k11, a08 x k02,     0

    smlal   v16.4s, v1.4h,  v22.4h     //k1x  0     a10 x k10, a11 x k11, a12 x k12,     0
    smlal   v17.4s, v4.4h,  v22.4h     //k1x  1     a12 x k10, a13 x k11, a14 x k12,     0
    smlal   v18.4s, v7.4h,  v22.4h     //k1x  2     a14 x k10, a15 x k11, a16 x k12,     0
    smlal   v19.4s, v10.4h, v22.4h     //k1x  3     a16 x k10, a17 x k11, a18 x k12,     0

    smlal   v16.4s, v2.4h,  v24.4h     //k2x  0     a10 x k20, a11 x k21, a12 x k22,     0
    smlal   v17.4s, v5.4h,  v24.4h     //k2x  1     a12 x k20, a13 x k21, a14 x k22,     0
    smlal   v18.4s, v8.4h,  v24.4h     //k2x  2     a14 x k20, a15 x k21, a16 x k22,     0
    smlal   v19.4s, v11.4h, v24.4h     //k2x  3     a16 x k20, a17 x k21, a18 x k22,     0

    saddlp  v16.2d, v16.4s
    saddlp  v17.2d, v17.4s
    saddlp  v18.2d, v18.4s
    saddlp  v19.2d, v19.4s

    addp    v2.2d,  v16.2d, v17.2d    // out0 out1
    addp    v3.2d,  v18.2d, v19.2d    // out2 out3

    ins     v2.4s[0], v2.4s[0]
    ins     v2.4s[1], v2.4s[2]
    ins     v2.4s[2], v3.4s[0]
    ins     v2.4s[3], v3.4s[2]

    add     v2.4s,  v2.4s,  v26.4s

    SQRDMULH    v2.4s, v2.4s, v27.4s
    sshl        v2.4s, v2.4s, v29.4s
    srshl       v0.4s, v2.4s, v28.4s
    smax        v0.4s, v0.4s, v30.4s
    smin        v0.4s, v0.4s, v31.4s

    // storage
    st1     {v0.b}[0], [x2], 0x1
    st1     {v0.b}[4], [x2], 0x1
    st1     {v0.b}[8], [x2], 0x1
    st1     {v0.b}[12], [x2], 0x1

    // loop control
    add     x10, x10, #0x8
    add     x11, x11, #0x8
    add     x12, x12, #0x8
    subs    x6, x6, 0x1
    cmp     x6, #0x0
    bne     mid_row_mid4

mid_row_remains:
    and     x6, x8, #0x3
    cmp     x6, #0x0
    beq     mid_row_right

    ld1     {v0.8h}, [x10]              // a0x
    ld1     {v1.8h}, [x11]              // a1x
    ld1     {v2.8h}, [x12]              // a2x

    ext     v3.16b, v0.16b, v0.16b, #4
    ext     v4.16b, v1.16b, v1.16b, #4
    ext     v5.16b, v2.16b, v2.16b, #4

    add     x10, x10, #0x8
    add     x11, x11, #0x8
    add     x12, x12, #0x8

    ld1     {v6.8h}, [x10]              // a0x
    ld1     {v7.8h}, [x11]              // a1x
    ld1     {v8.8h}, [x12]              // a2x

    sub     x10, x10, #0x8
    sub     x11, x11, #0x8
    sub     x12, x12, #0x8

    ext     v9.16b,  v6.16b, v6.16b, #4
    ext     v10.16b, v7.16b, v7.16b, #4
    ext     v11.16b, v8.16b, v8.16b, #4

    smull   v16.4s, v0.4h, v20.4h      //k0x  0     a00 x k10, a01 x k11, a02 x k02,     0
    smull   v17.4s, v3.4h, v20.4h      //k0x  1     a02 x k10, a03 x k11, a04 x k02,     0
    smull   v18.4s, v6.4h, v20.4h      //k0x  2     a04 x k10, a05 x k11, a06 x k02,     0
    smull   v19.4s, v9.4h, v20.4h      //k0x  3     a06 x k10, a07 x k11, a08 x k02,     0

    smlal   v16.4s, v1.4h,  v22.4h     //k2x  0     a10 x k20, a11 x k21, a12 x k12,     0
    smlal   v17.4s, v4.4h,  v22.4h     //k2x  1     a12 x k20, a13 x k21, a14 x k12,     0
    smlal   v18.4s, v7.4h,  v22.4h     //k2x  2     a14 x k20, a15 x k21, a16 x k12,     0
    smlal   v19.4s, v10.4h, v22.4h     //k2x  3     a16 x k20, a17 x k21, a18 x k12,     0

    smlal   v16.4s, v2.4h,  v24.4h     //k2x  0     a10 x k20, a11 x k21, a12 x k22,     0
    smlal   v17.4s, v5.4h,  v24.4h     //k2x  1     a12 x k20, a13 x k21, a14 x k22,     0
    smlal   v18.4s, v8.4h,  v24.4h     //k2x  2     a14 x k20, a15 x k21, a16 x k22,     0
    smlal   v19.4s, v11.4h, v24.4h     //k2x  3     a16 x k20, a17 x k21, a18 x k22,     0

    saddlp  v16.2d, v16.4s
    saddlp  v17.2d, v17.4s
    saddlp  v18.2d, v18.4s
    saddlp  v19.2d, v19.4s

    addp    v2.2d,  v16.2d, v17.2d    // out0 out1
    addp    v3.2d,  v18.2d, v19.2d    // out2 out3

    ins     v2.4s[0], v2.4s[0]
    ins     v2.4s[1], v2.4s[2]
    ins     v2.4s[2], v3.4s[0]
    ins     v2.4s[3], v3.4s[2]

    add     v2.4s,  v2.4s,  v26.4s

    SQRDMULH    v2.4s, v2.4s, v27.4s
    sshl        v2.4s, v2.4s, v29.4s
    srshl       v0.4s, v2.4s, v28.4s
    smax        v0.4s, v0.4s, v30.4s
    smin        v0.4s, v0.4s, v31.4s

mid_row_mid4_save_1:
     // If remains less than 4 is valid, save data one by one.
     st1    {v0.b}[0], [x2], 0x1
     add    x10, x10, #0x4
     add    x11, x11, #0x4
     add    x12, x12, #0x4
     ext    v0.16b, v0.16b, v0.16b, #4
     subs   x6, x6, #0x1
     bne    mid_row_mid4_save_1

mid_row_right:
    ld1         {v0.8h}, [x10]
    ld1         {v1.8h}, [x11]
    ld1         {v2.8h}, [x12]

    ins         v0.h[3],v21.h[0]
    ins         v1.h[3],v21.h[0]
    ins         v2.h[3],v21.h[0]

    lsr         x14, x9, #0x1
    and         x6, x14, #0x1
    cmp         x6, #0x0
    beq         mid_even
    ins         v0.h[2],v21.h[0]
    ins         v1.h[2],v21.h[0]
    ins         v2.h[2],v21.h[0]

mid_even:
    smull       v6.4s, v0.4h, v20.4h
    smlal       v6.4s, v1.4h, v22.4h
    smlal       v6.4s, v2.4h, v24.4h
    saddlp      v7.2d, v6.4s
    ins         v7.s[1], v7.s[2]
    saddlp      v7.2d, v7.4s
    add         v7.4s, v7.4s, v26.4s    // add bias
    SQRDMULH    v2.4s, v7.4s, v27.4s    // multi
    sshl        v2.4s, v2.4s, v29.4s    // left shift
    srshl       v0.4s, v2.4s, v28.4s    // Signed Rounding Shift Left
    smax        v0.4s, v0.4s, v30.4s
    smin        v0.4s, v0.4s, v31.4s
    st1         {v0.b}[0], [x2], 0x1
    add         x0, x0, x9, lsl #0x1
    b           loop_h_end

last_row:
    lsr     x14, x9, #0x1
    and     x13, x14, #0x1     // assum input_h = input_w
    cmp     x13, #0x0
    beq     mid_row

last_row_left:
    ld1     {v0.8h},[x10]
    ld1     {v1.8h},[x11]

    ins     v3.h[0],v20.h[1]
    ins     v3.h[1],v20.h[2]
    ins     v4.h[0],v22.h[1]
    ins     v4.h[1],v22.h[2]

    smull       v6.4s, v0.4h, v3.4h
    smlal       v6.4s, v1.4h, v4.4h
    saddlp      v7.2d, v6.4s
    add         v7.4s, v7.4s, v26.4s    // add bias
    SQRDMULH    v2.4s, v7.4s, v27.4s    // multi
    sshl        v2.4s, v2.4s, v29.4s    // left shift
    srshl       v0.4s, v2.4s, v28.4s    // Signed Rounding Shift Left
    smax        v0.4s, v0.4s, v30.4s
    smin        v0.4s, v0.4s, v31.4s

    st1     {v0.b}[0], [x2], 0x1

    add     x10, x10, #0x2
    add     x11, x11, #0x2

last_row_mid4:
    cmp     x6,#0x0
    beq     last_row_remains

    ld1     {v0.8h},[x10]
    ld1     {v1.8h},[x11]

    ext     v3.16b, v0.16b, v0.16b, #4
    ext     v4.16b, v1.16b, v1.16b, #4

    add     x10, x10, #0x8
    add     x11, x11, #0x8

    ld1     {v6.8h}, [x10]              // a0x
    ld1     {v7.8h}, [x11]              // a1x

    ext     v9.16b,  v6.16b, v6.16b, #4
    ext     v10.16b, v7.16b, v7.16b, #4

    smull   v16.4s , v0.4h, v20.4h      //k0x  0     a00 x k00, a01 x k01, a02 x k02,     0
    smull   v17.4s , v3.4h, v20.4h      //k0x  1     a02 x k00, a03 x k01, a04 x k02,     0
    smull   v18.4s , v6.4h, v20.4h      //k0x  2     a04 x k10, a05 x k11, a06 x k02,     0
    smull   v19.4s , v9.4h, v20.4h      //k0x  3     a06 x k10, a07 x k11, a08 x k02,     0

    smlal   v16.4s, v1.4h,  v22.4h      //k1x  0     a10 x k10, a11 x k11, a12 x k12,     0
    smlal   v17.4s, v4.4h,  v22.4h      //k1x  1     a12 x k10, a13 x k11, a14 x k12,     0
    smlal   v18.4s, v7.4h,  v22.4h      //k1x  2     a14 x k10, a15 x k11, a16 x k12,     0
    smlal   v19.4s, v10.4h, v22.4h      //k1x  3     a16 x k10, a17 x k11, a18 x k12,     0

    saddlp  v16.2d, v16.4s              // out0
    saddlp  v17.2d, v17.4s              // out1
    saddlp  v18.2d, v18.4s              // out2
    saddlp  v19.2d, v19.4s              // out3

    addp    v2.2d,  v16.2d, v17.2d    // out0 out1
    addp    v3.2d,  v18.2d, v19.2d    // out2 out3

    ins     v2.4s[0], v2.4s[0]
    ins     v2.4s[1], v2.4s[2]
    ins     v2.4s[2], v3.4s[0]
    ins     v2.4s[3], v3.4s[2]

    add     v2.4s,  v2.4s,  v26.4s

    SQRDMULH    v2.4s, v2.4s, v27.4s
    sshl        v2.4s, v2.4s, v29.4s
    srshl       v0.4s, v2.4s, v28.4s
    smax        v0.4s, v0.4s, v30.4s
    smin        v0.4s, v0.4s, v31.4s

    // storage
    st1     {v0.b}[0], [x2], 0x1
    st1     {v0.b}[4], [x2], 0x1
    st1     {v0.b}[8], [x2], 0x1
    st1     {v0.b}[12], [x2], 0x1

    // Increment, sliding window
    add     x10, x10, #0x8
    add     x11, x11, #0x8
    subs    x6, x6, 0x1
    cmp     x6, #0x0
    bne     last_row_mid4

last_row_remains:
    and     x6, x8, #0x3
    cmp     x6, #0x0
    beq     last_row_right
    ld1     {v0.8h},[x10]
    ld1     {v1.8h},[x11]

    ext     v3.16b, v0.16b, v0.16b, #4
    ext     v4.16b, v1.16b, v1.16b, #4

    add     x10, x10, #0x8
    add     x11, x11, #0x8

    ld1     {v6.8h}, [x10]              // a0x
    ld1     {v7.8h}, [x11]              // a1x

    sub     x10, x10, #0x8
    sub     x11, x11, #0x8

    ext     v9.16b,  v6.16b, v6.16b, #4
    ext     v10.16b, v7.16b, v7.16b, #4

    smull   v16.4s , v0.4h, v20.4h      //k0x  0     a00 x k00, a01 x k01, a02 x k02,     0
    smull   v17.4s , v3.4h, v20.4h      //k0x  1     a02 x k00, a03 x k01, a04 x k02,     0
    smull   v18.4s , v6.4h, v20.4h      //k0x  2     a04 x k10, a05 x k11, a06 x k02,     0
    smull   v19.4s , v9.4h, v20.4h      //k0x  3     a06 x k10, a07 x k11, a08 x k02,     0

    smlal   v16.4s, v1.4h,  v22.4h      //k1x  0     a10 x k10, a11 x k11, a12 x k12,     0
    smlal   v17.4s, v4.4h,  v22.4h      //k1x  1     a12 x k10, a13 x k11, a14 x k12,     0
    smlal   v18.4s, v7.4h,  v22.4h      //k1x  2     a14 x k10, a15 x k11, a16 x k12,     0
    smlal   v19.4s, v10.4h, v22.4h      //k1x  3     a16 x k10, a17 x k11, a18 x k12,     0

    saddlp  v16.2d, v16.4s              // out0
    saddlp  v17.2d, v17.4s              // out1
    saddlp  v18.2d, v18.4s              // out2
    saddlp  v19.2d, v19.4s              // out3

    addp    v2.2d,  v16.2d, v17.2d    // out0 out1
    addp    v3.2d,  v18.2d, v19.2d    // out2 out3

    ins     v2.4s[0], v2.4s[0]
    ins     v2.4s[1], v2.4s[2]
    ins     v2.4s[2], v3.4s[0]
    ins     v2.4s[3], v3.4s[2]

    add     v2.4s,  v2.4s,  v26.4s

    SQRDMULH    v2.4s, v2.4s, v27.4s
    sshl        v2.4s, v2.4s, v29.4s
    srshl       v0.4s, v2.4s, v28.4s
    smax        v0.4s, v0.4s, v30.4s
    smin        v0.4s, v0.4s, v31.4s

last_row_save_1:
     // If remains less than 4 is valid, save data one by one.
     st1    {v0.b}[0], [x2], 0x1
     add    x10, x10, #0x4
     add    x11, x11, #0x4
     ext    v0.16b, v0.16b, v0.16b, #4
     subs   x6, x6, #0x1
     bne    last_row_save_1

last_row_right:
    ld1         {v0.8h},[x10]
    ld1         {v1.8h},[x11]

    ins     v0.h[3],v21.h[0]
    ins     v1.h[3],v21.h[0]

    lsr     x14, x9, #0x1
    and     x6, x14, #0x1
    cmp     x6, #0x0
    beq     last_even
    ins     v0.h[2],v21.h[0]
    ins     v1.h[2],v21.h[0]
last_even:
    smull       v6.4s , v0.4h, v20.4h
    smlal       v6.4s , v1.4h, v22.4h
    saddlp      v7.2d, v6.4s
    ins         v7.s[1], v7.s[2]
    saddlp      v7.2d, v7.4s
    add         v7.4s, v7.4s, v26.4s    // add bias
    SQRDMULH    v2.4s, v7.4s, v27.4s    // multi
    sshl        v2.4s, v2.4s, v29.4s    // left shift
    srshl       v0.4s, v2.4s, v28.4s    // Signed Rounding Shift Left
    smax        v0.4s, v0.4s, v30.4s
    smin        v0.4s, v0.4s, v31.4s
    st1         {v0.b}[0], [x2], 0x1
    b           loop_h_end

loop_h_end:
    subs    x4, x4, 0x1
    bne     loop_h

loop_end:
    ldp d8, d9, [sp]
    ldp d10,d11,[sp, 0x10]
    ldp d12,d13,[sp, 0x20]
    ldp d14,d15,[sp, 0x30]
    add sp, sp, 0x40

    ret
