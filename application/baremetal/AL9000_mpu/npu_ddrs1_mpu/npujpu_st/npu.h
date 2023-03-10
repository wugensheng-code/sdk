
#ifndef __ALSOC_NPU_H
#define __ALSOC_NPU_H
/* status code */
#define NPU_IDLE            0X00
#define NPU_CONFIG          0x01
#define NPU_LOAD_DATA       0X02
#define NPU_MAC             0X03
#define NPU_POOL            0X04
#define NPU_ACT             0X05
#define NPU_WB              0X06
#define NPU_DONE            0x07

#define NPU_READ_ERROR      0x10
#define NPU_WRITE_ERROR     0x11

#define TOP_NS_BASE_ADDR 0xf8800000
#define GP0_ENABLE (TOP_NS_BASE_ADDR+0x80)
#define CFG_CTRL  (TOP_NS_BASE_ADDR+0x188)

/* register offset */

#define TOP_NS__CFG_CTRL_NPU_ADDR (0xf8800000+0x1b8)
#define SRST_CTRL2 (TOP_NS_BASE_ADDR+0x01000 + 0x078)
#define PLS_PROT   (TOP_NS_BASE_ADDR+0x080)
#define Hard_NPU__Share__Soft_reset__ADDR 0x0
#define Hard_NPU__Share__Filter_size__ADDR 0x4
#define Hard_NPU__Share__Kernal_size__ADDR 0x8
#define Hard_NPU__Share__FM_size__ADDR 0xc
#define Hard_NPU__Share__FM_plane__ADDR 0x10
#define Hard_NPU__Share__Grid_pad__ADDR 0x14
#define Hard_NPU__Share__Grid_size__ADDR 0x18
#define Hard_NPU__Share__Grid_plane__ADDR 0x1c
#define Hard_NPU__Share__Tile_total_size__ADDR 0x20
#define Hard_NPU__Share__Tile_size__ADDR 0x24
#define Hard_NPU__Share__Tile_plane__ADDR 0x28
#define Hard_NPU__Share__Tile_out_size__ADDR 0x2c
#define Hard_NPU__Share__Tile_out_plane__ADDR 0x30
#define Hard_NPU__Share__Work_mode__ADDR 0x34
#define Hard_NPU__Share__Tile_out_order__ADDR 0x38
#define Hard_NPU__Share__Axi_master_transfer_limit__ADDR 0x3c
#define Hard_NPU__Share__RAM_scaling_param__ADDR 0x40
#define Hard_NPU__Share__RAM_bias_param__ADDR 0x44
#define Hard_NPU__Share__Conv_output_zero_enable__ADDR 0x48
#define Hard_NPU__Share__Conv_output_zero_data__ADDR 0x4c
#define Hard_NPU__Share__Mode_load_data__ADDR 0x50
#define Hard_NPU__Share__Tile_out_point__ADDR 0x54
#define Hard_NPU__Share__Clock_gate_enable__ADDR 0x58
#define Hard_NPU__Share__Add_bias_en__ADDR 0x5c
#define Hard_NPU__CCU__Read_CFG_Start__ADDR 0x1000
#define Hard_NPU__CCU__Start_CCU__ADDR 0x1004
#define Hard_NPU__CCU__Map_reg_mode__ADDR 0x1008
#define Hard_NPU__CCU__Reg_base_addr_lo__ADDR 0x100c
#define Hard_NPU__CCU__Reg_length__ADDR 0x1010
#define Hard_NPU__CCU__Working_status__ADDR 0x1014
#define Hard_NPU__CCU__Interrupt_status__ADDR 0x1018
#define Hard_NPU__CCU__Clear_interrupt__ADDR 0x101c
#define Hard_NPU__CCU__Enable_interrupt__ADDR 0x1020
#define Hard_NPU__Bank_DMA__FM_base_addr_LO__ADDR 0x2000
#define Hard_NPU__Bank_DMA__Filter_base_addr_LO__ADDR 0x2004
#define Hard_NPU__Bank_DMA__FM_Filter_base_addr_HI__ADDR 0x2008
#define Hard_NPU__Bank_DMA__Debug_bank_dma_sel__ADDR 0x200c
#define Hard_NPU__Feature_Map_Bank__Input_fm_zero_point__ADDR 0x3000
#define Hard_NPU__Feature_Map_Bank__Tile_total_cycles__ADDR 0x3004
#define Hard_NPU__Feature_Map_Bank__Debug_fm_bank_sel__ADDR 0x3008
#define Hard_NPU__Filter_Bank__Input_filter_zero_point__ADDR 0x4000
#define Hard_NPU__Filter_Bank__Debug_flt_bank_sel__ADDR 0x4004
#define Hard_NPU__MAC_Array__Int_scale0__ADDR 0x5000
#define Hard_NPU__MAC_Array__Int_scale1__ADDR 0x5004
#define Hard_NPU__MAC_Array__Int_scale2__ADDR 0x5008
#define Hard_NPU__MAC_Array__Int_scale3__ADDR 0x500c
#define Hard_NPU__MAC_Array__Int_scale4__ADDR 0x5010
#define Hard_NPU__MAC_Array__Int_scale5__ADDR 0x5014
#define Hard_NPU__MAC_Array__Int_scale6__ADDR 0x5018
#define Hard_NPU__MAC_Array__Int_scale7__ADDR 0x501c
#define Hard_NPU__MAC_Array__Int_scale8__ADDR 0x5020
#define Hard_NPU__MAC_Array__Int_scale9__ADDR 0x5024
#define Hard_NPU__MAC_Array__Int_scale10__ADDR 0x5028
#define Hard_NPU__MAC_Array__Int_scale11__ADDR 0x502c
#define Hard_NPU__MAC_Array__Int_scale12__ADDR 0x5030
#define Hard_NPU__MAC_Array__Int_scale13__ADDR 0x5034
#define Hard_NPU__MAC_Array__Int_scale14__ADDR 0x5038
#define Hard_NPU__MAC_Array__Int_scale15__ADDR 0x503c
#define Hard_NPU__MAC_Array__Uint_scale__ADDR 0x5040
#define Hard_NPU__Write_Back__Out_FM_base_addr_LO__ADDR 0x6000
#define Hard_NPU__Write_Back__Out_grid_size__ADDR 0x6004
#define Hard_NPU__Write_Back__Out_grid_plane_size__ADDR 0x6008
#define Hard_NPU__Write_Back__Out_fm_size__ADDR 0x600c
#define Hard_NPU__Write_Back__Out_fm_plane_size__ADDR 0x6010
#define Hard_NPU__Write_Back__Output_data_size__ADDR 0x6014
#define Hard_NPU__Write_Back__Act_pooling_en__ADDR 0x6018
#define Hard_NPU__Write_Back__Act_scale__ADDR 0x601c
#define Hard_NPU__Write_Back__Act_zero_point_output__ADDR 0x6020
#define Hard_NPU__Write_Back__fifo_watermark__ADDR 0x6024
#define Hard_NPU__Write_Back__RAM_act_table_param__ADDR 0x6028
#define Hard_NPU__Write_Back__Act_scale_data__ADDR 0x602c
#define Hard_NPU__Write_Back__Act_scale_quant__ADDR 0x6030
#define Hard_NPU__Write_Back__Act_rshift__ADDR 0x6034
#define Hard_NPU__Write_Back__Ch_runtime__ADDR 0x6038
#define Hard_NPU__Write_Back__Out_cnt__ADDR 0x603c
#define Hard_NPU__Write_Back__Error_info0__ADDR 0x6040
#define Hard_NPU__Write_Back__Error_info1__ADDR 0x6044
#define Hard_NPU__Write_Back__Ch_error_info_0__ADDR 0x6048
#define Hard_NPU__Write_Back__Ch_error_info_1__ADDR 0x604c
#define Hard_NPU__Write_Back__Debug_sel__ADDR 0x6050
#define Hard_NPU__Write_Back__Debug_addr_tag__ADDR 0x6054

#define Hard_NPU__Write_Back__crop_info_en  0x6058 //c_en, h_en,w_en
#define Hard_NPU__Write_Back__crop_info_start 0x605c // c:11 h:10 w:10

#define HARDNPU__SHARE__SOFT_RESET__ADDR 0x0
#define HARDNPU__SHARE__FILTER_SIZE__ADDR 0x4
#define HARDNPU__SHARE__KERNAL_SIZE__ADDR 0x8
#define HARDNPU__SHARE__FM_SIZE__ADDR 0xc
#define HARDNPU__SHARE__FM_PLANE__ADDR 0x10
#define HARDNPU__SHARE__GRID_PAD__ADDR 0x14
#define HARDNPU__SHARE__GRID_SIZE__ADDR 0x18
#define HARDNPU__SHARE__GRID_PLANE__ADDR 0x1c
#define HARDNPU__SHARE__TILE_TOTAL_SIZE__ADDR 0x20
#define HARDNPU__SHARE__TILE_SIZE__ADDR 0x24
#define HARDNPU__SHARE__TILE_PLANE__ADDR 0x28
#define HARDNPU__SHARE__TILE_OUT_SIZE__ADDR 0x2c
#define HARDNPU__SHARE__TILE_OUT_PLANE__ADDR 0x30
#define HARDNPU__SHARE__WORK_MODE__ADDR 0x34
#define HARDNPU__SHARE__TILE_OUT_ORDER__ADDR 0x38
#define HARDNPU__SHARE__AXI_MASTER_TRANSFER_LIMIT__ADDR 0x3c
#define HARDNPU__SHARE__RAM_SCALING_PARAM__ADDR 0x40
#define HARDNPU__SHARE__RAM_BIAS_PARAM__ADDR 0x44
#define HARDNPU__SHARE__CONV_OUTPUT_ZERO_ENABLE__ADDR 0x48
#define HARDNPU__SHARE__CONV_OUTPUT_ZERO_DATA__ADDR 0x4c
#define HARDNPU__SHARE__MODE_LOAD_DATA__ADDR 0x50
#define HARDNPU__SHARE__TILE_OUT_POINT__ADDR 0x54
#define HARDNPU__SHARE__CLOCK_GATE_ENABLE__ADDR 0x58
#define HARDNPU__SHARE__ADD_BIAS_EN__ADDR 0x5c
#define HARDNPU__CCU__READ_CFG_START__ADDR 0x1000
#define HARDNPU__CCU__START_CCU__ADDR 0x1004
#define HARDNPU__CCU__MAP_REG_MODE__ADDR 0x1008
#define HARDNPU__CCU__REG_BASE_ADDR_LO__ADDR 0x100c
#define HARDNPU__CCU__REG_LENGTH__ADDR 0x1010
#define HARDNPU__CCU__WORKING_STATUS__ADDR 0x1014
#define HARDNPU__CCU__INTERRUPT_STATUS__ADDR 0x1018
#define HARDNPU__CCU__CLEAR_INTERRUPT__ADDR 0x101c
#define HARDNPU__CCU__ENABLE_INTERRUPT__ADDR 0x1020
#define HARDNPU__BANK_DMA__FM_BASE_ADDR_LO__ADDR 0x2000
#define HARDNPU__BANK_DMA__FILTER_BASE_ADDR_LO__ADDR 0x2004
#define HARDNPU__BANK_DMA__FM_FILTER_BASE_ADDR_HI__ADDR 0x2008
#define HARDNPU__BANK_DMA__DEBUG_BANK_DMA_SEL__ADDR 0x200c
#define HARDNPU__FEATURE_MAP_BANK__INPUT_FM_ZERO_POINT__ADDR 0x3000
#define HARDNPU__FEATURE_MAP_BANK__TILE_TOTAL_CYCLES__ADDR 0x3004
#define HARDNPU__FEATURE_MAP_BANK__DEBUG_FM_BANK_SEL__ADDR 0x3008
#define HARDNPU__FILTER_BANK__INPUT_FILTER_ZERO_POINT__ADDR 0x4000
#define HARDNPU__FILTER_BANK__DEBUG_FLT_BANK_SEL__ADDR 0x4004
#define HARDNPU__MAC_ARRAY__INT_SCALE0__ADDR 0x5000
#define HARDNPU__MAC_ARRAY__INT_SCALE1__ADDR 0x5004
#define HARDNPU__MAC_ARRAY__INT_SCALE2__ADDR 0x5008
#define HARDNPU__MAC_ARRAY__INT_SCALE3__ADDR 0x500c
#define HARDNPU__MAC_ARRAY__INT_SCALE4__ADDR 0x5010
#define HARDNPU__MAC_ARRAY__INT_SCALE5__ADDR 0x5014
#define HARDNPU__MAC_ARRAY__INT_SCALE6__ADDR 0x5018
#define HARDNPU__MAC_ARRAY__INT_SCALE7__ADDR 0x501c
#define HARDNPU__MAC_ARRAY__INT_SCALE8__ADDR 0x5020
#define HARDNPU__MAC_ARRAY__INT_SCALE9__ADDR 0x5024
#define HARDNPU__MAC_ARRAY__INT_SCALE10__ADDR 0x5028
#define HARDNPU__MAC_ARRAY__INT_SCALE11__ADDR 0x502c
#define HARDNPU__MAC_ARRAY__INT_SCALE12__ADDR 0x5030
#define HARDNPU__MAC_ARRAY__INT_SCALE13__ADDR 0x5034
#define HARDNPU__MAC_ARRAY__INT_SCALE14__ADDR 0x5038
#define HARDNPU__MAC_ARRAY__INT_SCALE15__ADDR 0x503c
#define HARDNPU__MAC_ARRAY__UINT_SCALE__ADDR 0x5040
#define HARDNPU__WRITE_BACK__OUT_FM_BASE_ADDR_LO__ADDR 0x6000
#define HARDNPU__WRITE_BACK__OUT_GRID_SIZE__ADDR 0x6004
#define HARDNPU__WRITE_BACK__OUT_GRID_PLANE_SIZE__ADDR 0x6008
#define HARDNPU__WRITE_BACK__OUT_FM_SIZE__ADDR 0x600c
#define HARDNPU__WRITE_BACK__OUT_FM_PLANE_SIZE__ADDR 0x6010
#define HARDNPU__WRITE_BACK__OUTPUT_DATA_SIZE__ADDR 0x6014
#define HARDNPU__WRITE_BACK__ACT_POOLING_EN__ADDR 0x6018
#define HARDNPU__WRITE_BACK__ACT_SCALE__ADDR 0x601c
#define HARDNPU__WRITE_BACK__ACT_ZERO_POINT_OUTPUT__ADDR 0x6020
#define HARDNPU__WRITE_BACK__FIFO_WATERMARK__ADDR 0x6024
#define HARDNPU__WRITE_BACK__RAM_ACT_TABLE_PARAM__ADDR 0x6028
#define HARDNPU__WRITE_BACK__ACT_SCALE_DATA__ADDR 0x602c
#define HARDNPU__WRITE_BACK__ACT_SCALE_QUANT__ADDR 0x6030
#define HARDNPU__WRITE_BACK__ACT_RSHIFT__ADDR 0x6034
#define HARDNPU__WRITE_BACK__CH_RUNTIME__ADDR 0x6038
#define HARDNPU__WRITE_BACK__OUT_CNT__ADDR 0x603c
#define HARDNPU__WRITE_BACK__ERROR_INFO0__ADDR 0x6040
#define HARDNPU__WRITE_BACK__ERROR_INFO1__ADDR 0x6044
#define HARDNPU__WRITE_BACK__CH_ERROR_INFO_0__ADDR 0x6048
#define HARDNPU__WRITE_BACK__CH_ERROR_INFO_1__ADDR 0x604c
#define HARDNPU__WRITE_BACK__DEBUG_SEL__ADDR 0x6050
#define HARDNPU__WRITE_BACK__DEBUG_ADDR_TAG__ADDR 0x6054
#define HARDNPU__WRITE_BACK__CROP_INFO_EN  0x6058 //c_en, h_en,w_en
#define HARDNPU__WRITE_BACK__CROP_INFO_START 0x605c // c:11 h:10 w:10

#define R_MAX ((Hard_NPU__Write_Back__crop_info_start>>2) + 1)
#define fm_size 0x10000000
#define flt_size 0x10000000

/* BASE ADDR */
#define ALSOC_NPU_BASE 0X63f00000

#define ALSOC_NPU_ADDR(x) (ALSOC_NPU_BASE+(x*4))

/* register offset */
#define SOFTNPU__SOFT_SHARE__CFG_INIT_ADDR__ADDR 0x0
#define SOFTNPU__SOFT_SHARE__CFG_BS_ADDR__ADDR 0x4
#define SOFTNPU__SOFT_SHARE__REG_BASE_ADDR_LO__ADDR 0x4
#define SOFTNPU__SOFT_SHARE__ACTIVATE_TBL_BS_ADDR__ADDR 0x8
#define SOFTNPU__SOFT_SHARE__REG_LENGTH__ADDR 0x8
#define SOFTNPU__SOFT_SHARE__FM_WB_ADDR__ADDR 0xc
#define SOFTNPU__SOFT_SHARE__FM0_BS_ADDR__ADDR 0x10
#define SOFTNPU__SOFT_SHARE__FM1_BS_ADDR__ADDR 0x14
#define SOFTNPU__SOFT_SHARE__FM2_BS_ADDR__ADDR 0x18
#define SOFTNPU__SOFT_SHARE__FM3_BS_ADDR__ADDR 0x1c
#define SOFTNPU__SOFT_SHARE__ALGO_BS_ADDR__ADDR 0x20
#define SOFTNPU__SOFT_SHARE__FM0_SIZE__ADDR 0x24
#define SOFTNPU__SOFT_SHARE__FM0_TSIZE__ADDR 0x28
#define SOFTNPU__SOFT_SHARE__FM1_SIZE__ADDR 0x2c
#define SOFTNPU__SOFT_SHARE__FM1_TSIZE__ADDR 0x30
#define SOFTNPU__SOFT_SHARE__FM2_SIZE__ADDR 0x34
#define SOFTNPU__SOFT_SHARE__FM2_TSIZE__ADDR 0x38
#define SOFTNPU__SOFT_SHARE__FM3_SIZE__ADDR 0x3c
#define SOFTNPU__SOFT_SHARE__FM3_TSIZE__ADDR 0x40
#define SOFTNPU__SOFT_SHARE__POOLING_CFG_DT__ADDR 0x44
#define SOFTNPU__SOFT_SHARE__POOLING_ZERO_DT__ADDR 0x48
#define SOFTNPU__SOFT_SHARE__POOLING_TRANS_DT__ADDR 0x4c
#define SOFTNPU__SOFT_SHARE__ACTIVATE_CFG_DT__ADDR 0x50
#define SOFTNPU__SOFT_SHARE__ACTIVATE_ZERO_DT__ADDR 0x54
#define SOFTNPU__SOFT_SHARE__ACTIVATE_N_TRANS_DT__ADDR 0x58
#define SOFTNPU__SOFT_SHARE__ACTIVATE_P_TRANS_DT__ADDR 0x5c
#define SOFTNPU__SOFT_SHARE__ACTIVATE_I_TRANS_DT__ADDR 0x60
#define SOFTNPU__SOFT_SHARE__ACTIVATE_O_TRANS_DT__ADDR 0x64
#define SOFTNPU__SOFT_SHARE__UPSAMPLE_CFG_DT__ADDR 0x68
#define SOFTNPU__SOFT_SHARE__UPSAMPLE_ZERO_DT__ADDR 0x6c
#define SOFTNPU__SOFT_SHARE__UPSAMPLE_TRANS_DT__ADDR 0x70
#define SOFTNPU__SOFT_SHARE__SHORTCUT_CFG_DT__ADDR 0x74
#define SOFTNPU__SOFT_SHARE__SHORTCUT_ZERO_DT__ADDR 0x78
#define SOFTNPU__SOFT_SHARE__SHORTCUT_FM0_TRANS_DT__ADDR 0x7c
#define SOFTNPU__SOFT_SHARE__SHORTCUT_FM1_TRANS_DT__ADDR 0x80
#define SOFTNPU__SOFT_SHARE__ROUTE_CFG_DT__ADDR 0x84
#define SOFTNPU__SOFT_SHARE__ROUTE_ZERO_DT_A__ADDR 0x88
#define SOFTNPU__SOFT_SHARE__ROUTE_ZERO_DT_B__ADDR 0x8c
#define SOFTNPU__SOFT_SHARE__ROUTE_TRANS_0DT__ADDR 0x90
#define SOFTNPU__SOFT_SHARE__ROUTE_TRANS_1DT__ADDR 0x94
#define SOFTNPU__SOFT_SHARE__ROUTE_TRANS_2DT__ADDR 0x98
#define SOFTNPU__SOFT_SHARE__ROUTE_TRANS_3DT__ADDR 0x9c
#define SOFTNPU__SOFT_SHARE__INTERRUPT_STATUS__ADDR 0xa0
#define SOFTNPU__SOFT_SHARE__CLEAR_INTERRUPT__ADDR 0xa4
#define SOFTNPU__SOFT_SHARE__ENABLE_INTERRUPT__ADDR 0xa8
#define SOFTNPU__SOFT_SHARE__ACTIVATE_TBL_DT__ADDR 0xac

#define SOFTNPU__SOFT_YOLO__DEPTH__ADDR 0x100
#define SOFTNPU__SOFT_YOLO__FM_PLANE__ADDR 0x104
#define SOFTNPU__SOFT_YOLO__SCALE_LAYER__ADDR 0x108
#define SOFTNPU__SOFT_YOLO__YOLO_C__ADDR 0x10c
#define SOFTNPU__SOFT_YOLO__YOLO_TRESH__ADDR 0x110
#define SOFTNPU__SOFT_YOLO__P_SIZE_0__ADDR 0x114
#define SOFTNPU__SOFT_YOLO__P_SIZE_1__ADDR 0x118
#define SOFTNPU__SOFT_YOLO__P_SIZE_2__ADDR 0x11c
#define SOFTNPU__SOFT_YOLO__YOLO_CTRL__ADDR 0x120

#define SOFTNPU__SOFT_SHARE2__CFG_INIT__ADDR 0x200
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

#define SOFT_NPU_BASE 0X91000000
#define REG_BASE_ADDR_LO 0x40000000

/* declare irq port number*/
#define ALSOC_HARDNPU_IRQ      144
#define ALSOC_SOFTNPU_IRQ      129 //114 pl[0] //129 pl[15]

/* function */
void alsoc_hardnpu_init(void);
void alsoc_softnpu_init(void);
unsigned int alsoc_npu_test_reg(unsigned int addr , unsigned int data) ;
void reg_write(unsigned int addr , unsigned int data) ;
unsigned int reg_read(unsigned int addr) ;
void soft_reg_write(unsigned int addr , unsigned int data) ;
unsigned int soft_reg_read(unsigned int addr) ;
void mem_write(unsigned int addr , unsigned int data) ;
unsigned int mem_read(unsigned int addr);
void mem_write_64(unsigned long long  addr , unsigned long long  data);
unsigned long long  mem_read_64(unsigned long long addr);
void alsoc_hardnpu_irq_handler(void);
void alsoc_softnpu_irq_handler(void);
#define  hard_reg_write(addr,data)  reg_write(addr,data) 
#define  hard_reg_read(addr)  reg_read(addr) 

int layer1_grid0_frame0_test();
int layer1_grid0_frame0();
int hard_register_test();
int soft_register_test();
int layer1_grid0_all_frame0(char* ifm , char* filter , char*    wb_message);
int layer1_grid1_all_frame0(char* ifm , char* filter , char*    wb_message);
int layer1_grid2_all_frame0(char* ifm , char* filter , char*    wb_message);
int layer1_grid0_all_frame0(char*    ifm , char*    filter , char*    wb_message);
int layer1_grid1_all_frame0(char*    ifm , char*    filter , char*    wb_message);
int layer1_grid2_all_frame0(char*    ifm , char*    filter , char*    wb_message);
int layer1_grid0_all_frame0_fpga_test();
int layer1_grid1_all_frame0_fpga_test();
int layer1_grid2_all_frame0_fpga_test();
int layer1_frame0_fpga_test();
int layer1_grid0_all_frame1_fpga_test();
int layer1_grid1_all_frame1_fpga_test();
int layer1_grid2_all_frame1_fpga_test();
int layer1_frame1_fpga_test();

int layer3_grid0_all_frame0_fpga_test();
int layer3_grid1_all_frame0_fpga_test();
int layer3_grid2_all_frame0_fpga_test();
int layer3_grid3_all_frame0_fpga_test();
int layer3_frame0_fpga_test();
int layer3_grid0_all_frame1_fpga_test();
int layer3_grid1_all_frame1_fpga_test();
int layer3_grid2_all_frame1_fpga_test();
int layer3_grid3_all_frame1_fpga_test();
int layer3_frame1_fpga_test();

int layer16_grid0_all_frame0_fpga_test();
int layer16_grid1_all_frame0_fpga_test();
int layer16_frame0_fpga_test();
int layer16_grid0_all_frame1_fpga_test();
int layer16_grid1_all_frame1_fpga_test();
int layer16_frame1_fpga_test();
int main_npujpu();

#endif /* __ALSOC_NPU_H */