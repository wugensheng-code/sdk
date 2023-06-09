#ifndef __YUV2RGB_REGISTER_H__
#define __YUV2RGB_REGISTER_H__

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

/* yuv2rgb related addresses */
#define SOFTNPU__SOFT_YUV2RGB__Y_BS_ADDR_LO__ADDR 0x300
#define SOFTNPU__SOFT_YUV2RGB__U_BS_ADDR_LO__ADDR 0x304
#define SOFTNPU__SOFT_YUV2RGB__V_BS_ADDR_LO__ADDR 0x308
#define SOFTNPU__SOFT_YUV2RGB__R_BS_ADDR_LO__ADDR 0x30c
#define SOFTNPU__SOFT_YUV2RGB__G_BS_ADDR_LO__ADDR 0x310
#define SOFTNPU__SOFT_YUV2RGB__B_BS_ADDR_LO__ADDR 0x314
#define SOFTNPU__SOFT_YUV2RGB__IMG_BS_ADDR_HI__ADDR 0x318
#define SOFTNPU__SOFT_YUV2RGB__Y_SIZE__ADDR 0x31c
#define SOFTNPU__SOFT_YUV2RGB__Y_TSIZE__ADDR 0x320
#define SOFTNPU__SOFT_YUV2RGB__U_SIZE__ADDR 0x324
#define SOFTNPU__SOFT_YUV2RGB__U_TSIZE__ADDR 0x328
#define SOFTNPU__SOFT_YUV2RGB__V_SIZE__ADDR 0x32c
#define SOFTNPU__SOFT_YUV2RGB__V_TSIZE__ADDR 0x330
#define SOFTNPU__SOFT_YUV2RGB__OUT_SIZE__ADDR 0x334
#define SOFTNPU__SOFT_YUV2RGB__OUT_TSIZE__ADDR 0x338
#define SOFTNPU__SOFT_YUV2RGB__CFG_DT__ADDR 0x33c
#define SOFTNPU__SOFT_YUV2RGB__R_MEAN_DT__ADDR 0x340
#define SOFTNPU__SOFT_YUV2RGB__G_MEAN_DT__ADDR 0x344
#define SOFTNPU__SOFT_YUV2RGB__B_MEAN_DT__ADDR 0x348
#define SOFTNPU__SOFT_YUV2RGB__R_QUANT_DT__ADDR 0x34c
#define SOFTNPU__SOFT_YUV2RGB__G_QUANT_DT__ADDR 0x350
#define SOFTNPU__SOFT_YUV2RGB__B_QUANT_DT__ADDR 0x354
#define SOFTNPU__SOFT_YUV2RGB__RGB_OUTPUT_ZERO_DT__ADDR 0x358

/* variable define */
#define Y_BS_ADDR_HI 0x0
#define U_BS_ADDR_HI 0x0
#define V_BS_ADDR_HI 0x0
#define R_BS_ADDR_HI 0x0
#define G_BS_ADDR_HI 0x0
#define B_BS_ADDR_HI 0x0

#define SOFT_BURST_LEN 16
#define SOFT_OUTSTANDING 16
#define MATCH_REAL_BURST 0

#define YUV_MODE_444 0
#define YUV_MODE_422 1
#define YUV_MODE_420 2



#endif