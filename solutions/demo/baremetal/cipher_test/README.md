# CSU jtag可用的测试方法
### CSU jtag执行流程
    开启CSU openocd
    开启CSU gdb
    tar ext:3333    # target连接openocd
    set *0x69010 = 0x0    # 设置CSU reset vector为0
    source pinmux.gdb   # pinmux config
    add-symbol-file rom_app.elf     # 添加rom app符号表
    y
    restore rom_app_crc.bin binary 0x0  # 0地址加载带CRC的rom bin
    set *0xf8800200 = 0x0   # 设置boot mode为jtag mode
    b Csu_JtagBootFlow      # 断点
    c
    # 设置encrypt key
    set CsuParam->Bhdr.EncryptKeySrc = 0x55aabbcc
    set CsuParam->Bhdr.EncryptKey[0] = 0xB82BB78D
    set CsuParam->Bhdr.EncryptKey[1] = 0x06928435
    set CsuParam->Bhdr.EncryptKey[2] = 0x6FFC32A6
    set CsuParam->Bhdr.EncryptKey[3] = 0x054A4C73
    set CsuParam->Bhdr.EncryptKey[4] = 0xBAF6AF50
    set CsuParam->Bhdr.EncryptKey[5] = 0xE5045340
    set CsuParam->Bhdr.EncryptKey[6] = 0x66A47271
    set CsuParam->Bhdr.EncryptKey[7] = 0xE72FF6A6
    set CsuParam->Bhdr.EncryptIv[0] = 0xC624E1DB
    set CsuParam->Bhdr.EncryptIv[1] = 0x141E678E
    set CsuParam->Bhdr.EncryptIv[2] = 0x91346E89
    set CsuParam->Bhdr.EncryptIv[3] = 0x1378FD4E
    set CsuParam->Efuse.CpuTypeSel = 0x2
    # load test data and golden data
    restore image_1k.bin binary 0x40050000
    restore image_1k_encrypt_aes.bin binary 0x40060000
    restore image_1k_encrypt_sm4.bin binary 0x40070000
    restore image_1k_aes256_enc_cbc.txt binary 0x40090000
    restore image_1k_sm4_enc_cbc.txt binary 0x400a0000
    restore image_1k_hash_aes.bin binary 0x40080000
    restore image_1k_hash_sm3.bin binary 0x40080100
    restore image_1k_encrypt_aes_hash.bin binary 0x40080200
    restore image_1k_encrypt_sm4_hash.bin binary 0x40080300
    restore image_1k.bin binary 0x400b0000
    restore image_4k.bin binary 0x400b0400
    restore image_8k.bin binary 0x400b1400
    restore image_16k.bin binary 0x400b3400
    restore image_32k.bin binary 0x400b7400
    restore image_64k.bin binary 0x400bf400
    restore image_128k.bin binary 0x400cf400
    c

### XPU jtag执行流程
    执行前先确保CSU处于全速运行状态，然后关闭openocd及gdb
    开启xpu openocd
    开始xpu gdb
    tar ext:3333
    file cipher_test.elf
    load
    y
    c