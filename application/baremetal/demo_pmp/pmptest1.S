    .section  .text
    .align 2
    .globl pmpTest1
pmpTest1:
    addi sp,sp,-48
	sw s0,44(sp)
    addi s0,sp,48
    #  switch M-mode to U-Mode
    li t0, 0x00001800
    csrc mstatus ,t0  # 将mstatus的 MPP域（即[12:11]位）清0
    la t0 ,1f   #将前面标签1所在PC地址赋给t0
    csrw mepc,t0 # 将t0的值赋给mepc
    mret        #执行mret ,切回到U-Mode，从1处开始执行程序
    .align 2
1:
    li t1,0x3

    #li t1,0x4


  #  lw s0, 44(sp)
	addi sp, sp, 48
	ret
