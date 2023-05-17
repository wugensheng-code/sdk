#ifndef __r_SOFT_YUV2RGB_TEST_h__
#define __r_SOFT_YUV2RGB_TEST_h__

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

typedef struct r_SOFT_YUV2RGB {

    union {
        struct {
            unsigned int Y_bs_addr_LO : 32 ;
        } val ;
        unsigned int _v_ ;
    } Y_bs_addr_LO ;
    union {
        struct {
            unsigned int U_bs_addr_LO : 32 ;
        } val ;
        unsigned int _v_ ;
    } U_bs_addr_LO ;
    union {
        struct {
            unsigned int V_bs_addr_LO : 32 ;
        } val ;
        unsigned int _v_ ;
    } V_bs_addr_LO ;
    union {
        struct {
            unsigned int R_bs_addr_LO : 32 ;
        } val ;
        unsigned int _v_ ;
    } R_bs_addr_LO ;
    union {
        struct {
            unsigned int G_bs_addr_LO : 32 ;
        } val ;
        unsigned int _v_ ;
    } G_bs_addr_LO ;
    union {
        struct {
            unsigned int B_bs_addr_LO : 32 ;
        } val ;
        unsigned int _v_ ;
    } B_bs_addr_LO ;
    union {
        struct {
            unsigned int Y_bs_addr_HI : 1 ;
            unsigned int U_bs_addr_HI : 1 ;
            unsigned int V_bs_addr_HI : 1 ;
            unsigned int R_bs_addr_HI : 1 ;
            unsigned int G_bs_addr_HI : 1 ;
            unsigned int B_bs_addr_HI : 1 ;
            unsigned int RSVD_6_31 : 26 ;
        } val ;
        unsigned int _v_ ;
    } img_bs_addr_HI ;
    union {
        struct {
            unsigned int Y_w : 13 ;
            unsigned int Y_h : 13 ;
            unsigned int RSVD_26_31 : 6 ;
        } val ;
        unsigned int _v_ ;
    } Y_size ;
    union {
        struct {
            unsigned int Y_tsize : 32 ;
        } val ;
        unsigned int _v_ ;
    } Y_tsize ;
    union {
        struct {
            unsigned int U_w : 13 ;
            unsigned int U_h : 13 ;
            unsigned int RSVD_26_31 : 6 ;
        } val ;
        unsigned int _v_ ;
    } U_size ;
    union {
        struct {
            unsigned int U_tsize : 32 ;
        } val ;
        unsigned int _v_ ;
    } U_tsize ;
    union {
        struct {
            unsigned int V_w : 13 ;
            unsigned int V_h : 13 ;
            unsigned int RSVD_26_31 : 6 ;
        } val ;
        unsigned int _v_ ;
    } V_size ;
    union {
        struct {
            unsigned int V_tsize : 32 ;
        } val ;
        unsigned int _v_ ;
    } V_tsize ;
    union {
        struct {
            unsigned int Out_w : 13 ;
            unsigned int Out_h : 13 ;
            unsigned int RSVD_26_31 : 6 ;
        } val ;
        unsigned int _v_ ;
    } Out_size ;
    union {
        struct {
            unsigned int Out_tsize : 32 ;
        } val ;
        unsigned int _v_ ;
    } Out_tsize ;
    union {
        struct {
            unsigned int YUV_mode : 4 ;
            unsigned int max_req_len : 8 ;
            unsigned int RGB_output_mode : 1 ;
            unsigned int RSVD_13_31 : 19 ;
        } val ;
        unsigned int _v_ ;
    } cfg_dt ;
    union {
        struct {
            unsigned int R_mean_rshft : 8 ;
            unsigned int R_mean_param : 16 ;
            unsigned int RSVD_24_31 : 8 ;
        } val ;
        unsigned int _v_ ;
    } R_mean_dt ;
    union {
        struct {
            unsigned int G_mean_rshft : 8 ;
            unsigned int G_mean_param : 16 ;
            unsigned int RSVD_24_31 : 8 ;
        } val ;
        unsigned int _v_ ;
    } G_mean_dt ;
    union {
        struct {
            unsigned int B_mean_rshft : 8 ;
            unsigned int B_mean_param : 16 ;
            unsigned int RSVD_24_31 : 8 ;
        } val ;
        unsigned int _v_ ;
    } B_mean_dt ;
    union {
        struct {
            unsigned int R_quant_rshft : 8 ;
            unsigned int R_quant_param : 16 ;
            unsigned int RSVD_24_31 : 8 ;
        } val ;
        unsigned int _v_ ;
    } R_quant_dt ;
    union {
        struct {
            unsigned int G_quant_rshft : 8 ;
            unsigned int G_quant_param : 16 ;
            unsigned int RSVD_24_31 : 8 ;
        } val ;
        unsigned int _v_ ;
    } G_quant_dt ;
    union {
        struct {
            unsigned int B_quant_rshft : 8 ;
            unsigned int B_quant_param : 16 ;
            unsigned int RSVD_24_31 : 8 ;
        } val ;
        unsigned int _v_ ;
    } B_quant_dt ;
    union {
        struct {
            unsigned int RGB_output_zero_dt : 32 ;
        } val ;
        unsigned int _v_ ;
    } RGB_output_zero_dt ;
} r_SOFT_YUV2RGB;

#endif