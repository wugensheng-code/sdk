
 .text
    .align 4
    .global add_float_neon_asm
    .type add_float_neon_asm, %function

add_float_neon_asm:

.L_loop:

    ld1     {v0.4s}, [x1], #16
    ld1     {v1.4s}, [x2], #16
    fadd    v0.4s, v0.4s, v1.4s
    subs x3, x3, #4
    st1  {v0.4s}, [x0], #16
    bgt .L_loop

    ret
