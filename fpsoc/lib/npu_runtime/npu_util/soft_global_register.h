#ifndef __SOFT_GLOBAL_REGISTER_H__
#define __SOFT_GLOBAL_REGISTER_H__

typedef struct r_SOFT_INIT_CFG {
    union {
        struct {
            unsigned int cfg_soft_rst : 1 ;
            unsigned int cfg_work_mode : 1 ;
            unsigned int RSVD_2_10 : 9 ;
            unsigned int cfg_yolo_sw : 1 ;
            unsigned int cfg_rdreg_sw : 1 ;
            unsigned int cfg_rdfm_sw : 1 ;
            unsigned int cfg_lyr_strt : 1 ;
            unsigned int cfg_os_num : 7 ;
            unsigned int cfg_burst_len : 8 ;
            unsigned int RSVD_30_31 : 2 ;
        } val ;
        unsigned int _v_ ;
    } cfg_init_addr ;
} r_SOFT_INIT_CFG;

/* addresses for soft npu shared registers */
#define SOFTNPU__SOFT_SHARE__CFG_INIT_ADDR__ADDR 0x0
#define SOFTNPU__SOFT_SHARE__INTERRUPT_STATUS__ADDR 0xa0
#define SOFTNPU__SOFT_SHARE__CLEAR_INTERRUPT__ADDR 0xa4
#define SOFTNPU__SOFT_SHARE__ENABLE_INTERRUPT__ADDR 0xa8
#define SOFTNPU__SOFT_SHARE2__CFG_INIT__ADDR 0x200

#endif