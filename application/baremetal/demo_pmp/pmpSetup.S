    #//add by hunter
    #define ALL_REGION

    .section  .text
    .align 2
    .globl pmpSetup
pmpSetup:
    addi sp,sp,-48
	sw s0,44(sp)
    addi s0,sp,48

 #ifndef ALL_REGION
    # PMPADDR
    li t0,0x200000D4
    					      # {bit[31:30]}   {bit[29:0]}     {numb of 1 in low bits}   {Address Region of entries,after left shift 2bit}
    csrrs zero, pmpaddr0, t0  # 00  100000000000000000000011010100  0       Region Addr:    0x80000350 ~0x80000357
    li t0,0x200000D6
    csrrs zero, pmpaddr1, t0  # 00 	100000000000000000000011010110  0                       0x80000358 ~0x8000035F
    li t0,0x200000D8
    csrrs zero, pmpaddr2, t0  # 00	100000000000000000000011011000	0                       0x80000360 ~0x80000367
    li t0,0x200000DA
    csrrs zero, pmpaddr3, t0  # 00	100000000000000000000011011010 	0                       0x80000368 ~0x8000036F
 #else
    #//add by hunter
    # PMPADDR
    li t0,0x3FFFFFFF
    csrrs zero, pmpaddr0, t0  # 00 111111111111111111111111111111 1(30)            bit[29..0]=1, region: cover all addr size.
 #endif

    #PMPCFG
    #li t0 , 0x1F1F1F1F
    #li t0 , 0x1F1F1F1B
    #li t0 , 0x1F1F1F1E
    li t0 , 0x1F1F1F1D

    csrw pmpcfg0, t0

    lw s0, 44(sp)
	addi sp, sp, 48
	ret
