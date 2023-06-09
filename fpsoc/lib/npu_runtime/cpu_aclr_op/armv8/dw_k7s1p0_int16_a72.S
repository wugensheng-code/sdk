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
// int16x8: v6  ~ v9
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
#define KERNEL_NAME depthwise_k7s1p0_int16_a72
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
    movi d6,0
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
no_bias:

    mov x4, 0


    mov x10, x0
    mov x11, x1

loop_7:
    // Load input feature maps
    ld1 {v0.4h}, [x10]
    add x10, x10, #0x8
    ld1 {v1.4h}, [x10]
    add x10, x10, #0x6
    ins v1.h[3], v23.h[0]

    // Init the weight data
    // ldr     q10, [x1], #0x8
    ld1 {v20.4h}, [x11]
    add x11, x11, #0x8
    ld1 {v21.4h}, [x11]
    add x11, x11, #0x6
    ins v21.h[3], v23.h[0]

    // Do multiplication
    smlal   v6.4s, v0.4h,  v20.4h
    saddlp  v22.2d, v6.4s
    ins     v22.4s[1], v22.4s[2]
    saddlp  v22.2d, v22.4s

    smlal   v6.4s, v1.4h,  v21.4h
    saddlp  v22.2d, v6.4s
    ins     v22.4s[1], v22.4s[2]
    saddlp  v22.2d, v22.4s
    // FIXME : Figure out addv
    // addv    v22.f[0], v6.4s

    // Increment
    add x4, x4, #0x1
    cmp x4, #0x7
    blt loop_7

    saddlp      v22.2d, v6.4s
    ins         v22.4s[1], v22.4s[2]
    ins         v22.4s[2], v23.4s[0]
    ins         v22.4s[3], v23.4s[0]
    saddlp      v22.2d, v22.4s

    // This is only fit for float
    // addv        s6, v6.4s

    add         v22.4s, v22.4s, v26.4s
    SQRDMULH    v23.4s, v22.4s, v27.4s
    sshl        v24.4s, v23.4s, v29.4s
    srshl       v24.4s, v24.4s, v28.4s
    smax        v24.4s, v24.4s, v30.4s
    smin        v24.4s, v24.4s, v31.4s

    st1     {v24.b}[0], [x2], 0x1

end:
    ldp d8, d9, [sp]
    ldp d10,d11,[sp, 0x10]
    ldp d12,d13,[sp, 0x20]
    ldp d14,d15,[sp, 0x30]
    add sp, sp, 0x40

    ret
