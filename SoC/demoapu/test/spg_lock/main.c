#include <stdio.h>

#define TEST_TOTAL 59
//lock reg_addr
#define  CFG_LOCK0  0xF8807000
#define  CFG_LOCK1  0xF8807004
#define  CFG_LOCK2  0xF8807008

typedef unsigned int  U32;
typedef unsigned char U8;

#define BIT(x)   ((U32)((U32)0x01U<<(x)))

#define READ_REG(reg_addr)     (*(volatile U32 *) (U32) (reg_addr))

#define WRITE_REG(reg_addr, value) do { \
    volatile U32 *LocalAddress = (volatile U32 *) (reg_addr); \
    *LocalAddress = value; \
} while (0);


struct reg_info
{
    U8 name[32];
    U32 address;
};

struct reg_info reg_instance[TEST_TOTAL] = 
{
//lock0
    {"i2c1", 0xF8415014},
    {"i2c0", 0xF8414014},
    {"ttc1", 0xF840D030},
    {"ttc0", 0xF840C030},
    {"can1", 0xF8409050},
    {"can0", 0xF8408050},
    {"spi1", 0xF8405004},
    {"spi0", 0xF8404004},
    {"uart1", 0xF84012bc},
    {"uart0", 0xF84002bc},
    {"sdio1", 0xF804A028},
    {"sdio0", 0xF8049028},
    {"gbe1", 0xF8111120},
    {"gbe0", 0xF8101120},
    {"usb1", 0xF81C005c},
    {"usb0", 0xF818005c}, \
//lock1
    {"top_ns", 0xF8800134},   //top cfg 1-control bits for usb1},
    {"crs", 0xF8801228},   //top cfg 2-IOPLL_CTRL10},
    {"apu", 0xF8802008},    //top cfg 3-bit0 dbgl1rstdisable},
    {"top_s", 0xF8806304},   //cfg_ctrl_apu0_reset_vector_l},
    {"pin", 0xF88039a8},
    {"xadc_ps", 0xF840B000},
    {"mpu_apu", 0xF841D014},
    {"mpu_ocms2", 0xF841B014},
    {"mpu_npu", 0xF841C014},
    {"mpu_hpm1", 0xF8413014},
    {"mpu_hpm0", 0xF8412014},
    {"mpu_ddrs1", 0xF840F014},
    {"mpu_ddrs0", 0xF840E014},
    {"DDR", 0xF8420000},   //all value are 0x0 now
    {"jpu_cfg", 0xF8460014},
    {"ipc",  0xF841F000},
    {"dmac_ahb", 0xF804D000},
    {"dmac_axi", 0xF8418020},
    {"gpio", 0xF8411060},
    {"qspi", 0xF804E22c},
    {"smc", 0xF841A408},
    {"wdt", 0xF8410000}, \
//lock2-xmon
    {"xmon_pl2acp",  0xF84468b0},
    {"xmon_hp12bus", 0xF8444C34},
    {"xmon_hp02bus", 0xF8444834},
    {"xmon_gp12bus", 0xF8444434},
    {"xmon_gp02bus", 0xF8444034},
    {"xmon_rpu2bus", 0xF8446034},
    {"xmon_jpu2bus", 0xF8445434},
    {"xmon_dmacx2bus", 0xF8442434},
    {"xmon_bus2ocm", 0xF8441834},
    {"xmon_bus2smc", 0xF8442034},
    {"xmon_bus2sh ", 0xF8441C34},
    {"xmon_bus2rpu", 0xF8446434},
    {"xmon_npu2bus", 0xF8445834},
    {"xmon_bus2npu", 0xF8445C34},
    {"xmon_apu2bus", 0xF8445034},
    {"xmon_bus2gp1", 0xF8441434},
    {"xmon_bus2gp0", 0xF8441034},
    {"xmon_ocmbus2ddrs3", 0xF8440C34},
    {"xmon_ocmbus2ddrs2", 0xF8440834},
    {"xmon_bus2ddrs1", 0xF8440434},
    {"xmon_bus2ddrs0", 0xF8440034}
};

struct reg_info *p = reg_instance;

//set all the regs's value to 0xffffffff, these are the expected ret values.
U32 expected_ret[TEST_TOTAL] = { \
    0x0000ffff, 0x0000ffff, 0x0000ffff, 0x0000ffff, 0xffffffff, 0xffffffff, 0x0000ffff, 0x0000ffff, 0xffffffff, 0xffffffff, \
    0x070fffff, 0x070fffff, 0xfffffffc, 0xfffffffc, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0x00000001, 0xffffffff, \
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0x00000000, \
    0xffffffff, 0xffffffff, 0xffffffff, 0x0000ffff, 0x00000001, 0x0000003f, 0x01ffffff, 0x0000003f, 0xffffffff, 0xffffffff, \
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, \
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff};

U32 success_counter = 0;


void __disable_irq_abort(void)
{
    asm volatile(
        "msr    daifset, #0x7"
        :
        :
        : "memory");
}



void repeat_test(U32 reg_addr, U32 bit_mask, U32 order)
{
    U32 i = 0;

    printf("\r\n");
    printf("Doing lock test of %s: ", reg_instance[order - 1].name);

    //Clear all values
    for (i=0; i< TEST_TOTAL; i++)
    {
        WRITE_REG(reg_instance[i].address, 0x00000000);
    }

    //Set up lock protection
    WRITE_REG(reg_addr, bit_mask);

    //xmon_pl2acp need acp reset.
    if (order == 39)
    {
        WRITE_REG(0xf8801070, READ_REG(0xf8801070) | 0x100);
    }        

    //Set value to 0xffffffff
    for (i=0; i< TEST_TOTAL; i++)
    {
        WRITE_REG(reg_instance[i].address, 0xffffffff);
    }

    //Disable lock protection, if not, there maybe no print in uart0.
    WRITE_REG(reg_addr, 0x00000000);

    //Whose get_value != set_value? Then it's locked.
    for (i=0; i< TEST_TOTAL; i++)
    {   
        if ((READ_REG(reg_instance[i].address) != expected_ret[i]) )
        {
            printf("%s is locked successfully. \n", reg_instance[i].name);
            //printf("reg value is 0x%x \n", READ_REG(reg_instance[i].address));
            success_counter ++;
        }
    }
}

void main(void)
{
    U32 i;

    __disable_irq_abort();

    printf("write lock protection test start:\n");
    printf("default value is: \n");

    for (i=0; i< TEST_TOTAL; i++)	
    {
        printf("0x%08x\n", READ_REG(reg_instance[i].address));
    }

//lock0
#if 1
    repeat_test(CFG_LOCK0, BIT(29), 1);
    repeat_test(CFG_LOCK0, BIT(28), 2);
    repeat_test(CFG_LOCK0, BIT(25), 3);   //ttc1
    repeat_test(CFG_LOCK0, BIT(24), 4);   //ttc0
    repeat_test(CFG_LOCK0, BIT(21), 5);
    repeat_test(CFG_LOCK0, BIT(20), 6);
    repeat_test(CFG_LOCK0, BIT(17), 7);
    repeat_test(CFG_LOCK0, BIT(16), 8);
    repeat_test(CFG_LOCK0, BIT(13), 9);  //uart1
    repeat_test(CFG_LOCK0, BIT(12), 10);  //uar0
    repeat_test(CFG_LOCK0, BIT(9), 11);  //sdio1
    repeat_test(CFG_LOCK0, BIT(8), 12);  //sdio0
    repeat_test(CFG_LOCK0, BIT(5), 13);
    repeat_test(CFG_LOCK0, BIT(4), 14);
    repeat_test(CFG_LOCK0, BIT(1), 15);
    repeat_test(CFG_LOCK0, BIT(0), 16);
#endif


//lock1
#if 1
    repeat_test(CFG_LOCK1, BIT(31), 17);
    repeat_test(CFG_LOCK1, BIT(30), 18);
    repeat_test(CFG_LOCK1, BIT(29), 19);
    repeat_test(CFG_LOCK1, BIT(28), 20);
    repeat_test(CFG_LOCK1, BIT(27), 21);
    repeat_test(CFG_LOCK1, BIT(26), 22);
    repeat_test(CFG_LOCK1, BIT(15), 23);
    repeat_test(CFG_LOCK1, BIT(14), 24);
    repeat_test(CFG_LOCK1, BIT(13), 25);
    repeat_test(CFG_LOCK1, BIT(12), 26);
    repeat_test(CFG_LOCK1, BIT(11), 27);
    repeat_test(CFG_LOCK1, BIT(10), 28);
    repeat_test(CFG_LOCK1, BIT(9), 29);
    //repeat_test(CFG_LOCK1, BIT(8), 30);   //No need to test dmc_cfg(DDR memory controller) now, all its value are 0x0. 
    repeat_test(CFG_LOCK1, BIT(7), 31);
    repeat_test(CFG_LOCK1, BIT(6), 32);
    repeat_test(CFG_LOCK1, BIT(5), 33);
    repeat_test(CFG_LOCK1, BIT(4), 34);
    repeat_test(CFG_LOCK1, BIT(3), 35);
    repeat_test(CFG_LOCK1, BIT(2), 36);
    repeat_test(CFG_LOCK1, BIT(1), 37);
    repeat_test(CFG_LOCK1, BIT(0), 38);
#endif

#if 1
//lock2
    repeat_test(CFG_LOCK2, BIT(20), 39);  //xmon_pl2acp
    repeat_test(CFG_LOCK2, BIT(19), 40);
    repeat_test(CFG_LOCK2, BIT(18), 41);
    repeat_test(CFG_LOCK2, BIT(17), 42);
    repeat_test(CFG_LOCK2, BIT(16), 43);
    repeat_test(CFG_LOCK2, BIT(15), 44);
    repeat_test(CFG_LOCK2, BIT(14), 45);
    repeat_test(CFG_LOCK2, BIT(13), 46);
    repeat_test(CFG_LOCK2, BIT(12), 47);
    repeat_test(CFG_LOCK2, BIT(11), 48);
    repeat_test(CFG_LOCK2, BIT(10), 49);
    repeat_test(CFG_LOCK2, BIT(9), 50);
    repeat_test(CFG_LOCK2, BIT(8), 51);
    repeat_test(CFG_LOCK2, BIT(7), 52);
    repeat_test(CFG_LOCK2, BIT(6), 53);
    repeat_test(CFG_LOCK2, BIT(5), 54);
    repeat_test(CFG_LOCK2, BIT(4), 55);
    repeat_test(CFG_LOCK2, BIT(3), 56);
    repeat_test(CFG_LOCK2, BIT(2), 57);
    repeat_test(CFG_LOCK2, BIT(1), 58);
    repeat_test(CFG_LOCK2, BIT(0), 59);
#endif
    printf("\r\n");
    printf("Test 1 result: \r\n");
    if (success_counter == 58)  //dmc_cfg is not tested, the actucal total is 58.
        printf("All write lock protection tests are successful.\n");
    else
        printf("There is lock protection error, please check.\n");

    printf("\r\n");
    printf("Doing irq abort test:\n");
    //Set up lock protection of i2c1
    WRITE_REG(CFG_LOCK0, BIT(29));


    printf("\r\n");
    printf("Test 2 result: \r\n");

    printf("If you can see the message below: 'Error at current.....', then irq abort test is successful");
    printf("\r\n");
    __enable_irq_abort();
    //Write to i2c1 will cause system error print
    WRITE_REG(reg_instance[0].address, 0xffffffff);

}
