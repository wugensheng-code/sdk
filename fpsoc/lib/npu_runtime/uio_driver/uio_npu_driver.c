#include <stdio.h>
#include "driver_api.h"
#include "uio_npu_driver.h"
#include "npu_defines.h"

static int8_t write_pkg_normal_mode (pkg_info_t* pkg_info, uint8_t is_hard_npu) {

    uint32_t pkg_len = pkg_info->length;
    uint32_t* pkg_data = (uint32_t*) pkg_info->virt_addr ;
    uint8_t stage = 0 ;
    uint8_t is_same_addr = 0 ;
    uint8_t is_err_data = 0 ;
    uint32_t len = 0 ;
    uint32_t cur_pos = 0 ;
    uint32_t start_addr = 0 ;

    for(uint32_t i = 0; i < pkg_len; i++) {
        uint32_t data = pkg_data[i] ;
        switch (stage) {
            case 0 /* first data decode */:
            {
                is_err_data = (data >> 24) == 0xff ? 0 : 1 ;
                if (is_err_data) {
                    stage = 3 ;
                    break ;
                }

                is_same_addr = ((data & 0x00c00000) >> 22) == 0x00 ? 0 : 1 ;

                len = (data & 0x003fffff) ;

                stage = 1 ;

                break;
            }
            case 1 /* second data decode */:
            {
                start_addr = data ;

                // if (len == 0) {
                //     stage = 0 ;
                //     break ;
                // }
                
                stage = 2 ;

                break;
            }
            case 2 /* pkg data transfer */:
            {
                uint32_t cur_addr = 0 ;
                if (is_same_addr) {
                    cur_addr = start_addr ;
                } else {
                    cur_addr = start_addr + cur_pos * 4 ;
                }
                if (is_hard_npu) {
                    hard_npu_write((cur_addr-HARD_NPU_MASK), data) ;
#ifdef PRINT_NPU_CMD
                    printf("write data: %x to hard_npu at addr: %x\n", data, (cur_addr-HARD_NPU_MASK)) ;
#endif
                } else {
                    soft_npu_write((cur_addr-SOFT_NPU_MASK), data) ;
#ifdef PRINT_NPU_CMD
                    printf("write data: %x to soft_npu at addr: %x\n", data, (cur_addr-SOFT_NPU_MASK)) ;
#endif
                }
                
                cur_pos++ ;

                if (cur_pos == len) {
                    stage = 0 ;
                    cur_pos = 0 ;
                }

                break;
            }            
            case 3 /* err data in pkg */:
            {
                printf("err data in pkg\n") ;
                return -1 ;
            }             
            default:
            {   
                printf("err stage in decode pkg data\n") ;
                break;
            }
        }
    }

    return 0 ;
}

int8_t run_npu_hard_normal_mode (hard_npu_pkg_info_t* pkg) {

    hard_npu_write(HARD_NPU_MODE_LOAD_DATA_ADDR, pkg->mode_load_data) ;
#ifdef PRINT_NPU_CMD
    printf("write data: %x to hard_npu at addr: %x\n", pkg->mode_load_data, HARD_NPU_MODE_LOAD_DATA_ADDR) ;
#endif
    
    hard_npu_write(HARD_NPU_IFM_BASE_ADDR, pkg->ifm_base_addr) ;
#ifdef PRINT_NPU_CMD
    printf("write data: %x to hard_npu at addr: %x\n", pkg->ifm_base_addr, HARD_NPU_IFM_BASE_ADDR) ;
#endif
    
    hard_npu_write(HARD_NPU_FILTER_BASE_ADDR, pkg->flt_base_addr) ;
#ifdef PRINT_NPU_CMD
    printf("write data: %x to hard_npu at addr: %x\n", pkg->flt_base_addr, HARD_NPU_FILTER_BASE_ADDR) ;
#endif

    hard_npu_write(HARD_NPU_OFM_BASE_ADDR, pkg->ofm_base_addr) ;
#ifdef PRINT_NPU_CMD
    printf("write data: %x to hard_npu at addr: %x\n", pkg->ofm_base_addr, HARD_NPU_OFM_BASE_ADDR) ;
#endif

    write_pkg_normal_mode(&(pkg->pkg_info), 1) ;

    return 0 ;
}

int8_t run_npu_hard_dma_mode (hard_npu_pkg_info_t* pkg) {

    hard_npu_write(HARD_NPU_MODE_LOAD_DATA_ADDR, pkg->mode_load_data) ;
#ifdef PRINT_NPU_CMD
    printf("write data: %x to hard_npu at addr: %x\n", pkg->mode_load_data, HARD_NPU_MODE_LOAD_DATA_ADDR) ;
#endif
    
    hard_npu_write(HARD_NPU_IFM_BASE_ADDR, pkg->ifm_base_addr) ;
#ifdef PRINT_NPU_CMD
    printf("write data: %x to hard_npu at addr: %x\n", pkg->ifm_base_addr, HARD_NPU_IFM_BASE_ADDR) ;
#endif
    
    hard_npu_write(HARD_NPU_FILTER_BASE_ADDR, pkg->flt_base_addr) ;
#ifdef PRINT_NPU_CMD
    printf("write data: %x to hard_npu at addr: %x\n", pkg->flt_base_addr, HARD_NPU_FILTER_BASE_ADDR) ;
#endif

    hard_npu_write(HARD_NPU_OFM_BASE_ADDR, pkg->ofm_base_addr) ;
#ifdef PRINT_NPU_CMD
    printf("write data: %x to hard_npu at addr: %x\n", pkg->ofm_base_addr, HARD_NPU_OFM_BASE_ADDR) ;
#endif

    // config dma axi_master_transfer_limit
    hard_npu_write(HARD_AXI_MASTER_TRANSFER_LIMIT_ADDR, 0x1010) ;
#ifdef PRINT_NPU_CMD
    printf("write data: %x to hard_npu at addr: %x\n", 0x1010, HARD_AXI_MASTER_TRANSFER_LIMIT_ADDR) ;
#endif

    // config dma pkg addr
    hard_npu_write(HARD_DMA_REG_BASE_ADDR, pkg->pkg_info.addr) ;
#ifdef PRINT_NPU_CMD
    printf("write data: %x to hard_npu at addr: %x\n", pkg->pkg_info.addr, HARD_DMA_REG_BASE_ADDR) ;
#endif

    // config dma pkg length
    hard_npu_write(HARD_DMA_LENGTH_ADDR, (pkg->pkg_info.length)) ;
#ifdef PRINT_NPU_CMD
    printf("write data: %x to hard_npu at addr: %x\n", (pkg->pkg_info.length), HARD_DMA_LENGTH_ADDR) ;
#endif

    // config dma reg cfg start
    hard_npu_write(HARD_DMA_READ_CFG_START_ADDR, 0x01) ;
#ifdef PRINT_NPU_CMD
    printf("write data: %x to hard_npu at addr: %x\n", 0x01, HARD_DMA_READ_CFG_START_ADDR) ;
#endif

    return 0 ;
}

int8_t run_npu_soft_normal_mode (soft_npu_pkg_info_t* pkg, rt_node_t* nd) {

    soft_npu_write(SOFT_NPU_OFM_BASE_ADDR, pkg->ofm_base_addr) ;
#ifdef PRINT_NPU_CMD
    printf("write data: %x to soft_npu at addr: %x\n", pkg->ofm_base_addr, SOFT_NPU_OFM_BASE_ADDR) ;
#endif

    for(unsigned int i = 0; i < nd->input_num; i++) {
        switch (i)
        {
            case 0/* input 0 */:
            {
                soft_npu_write(SOFT_NPU_IFM0_BASE_ADDR, pkg->ifm0_base_addr) ;
#ifdef PRINT_NPU_CMD
                printf("write data: %x to soft_npu at addr: %x\n", pkg->ifm0_base_addr, SOFT_NPU_IFM0_BASE_ADDR) ;
#endif
                break ;
            }
            case 1/* input 1 */:
            {
                soft_npu_write(SOFT_NPU_IFM1_BASE_ADDR, pkg->ifm1_base_addr) ;
#ifdef PRINT_NPU_CMD
                printf("write data: %x to soft_npu at addr: %x\n", pkg->ifm1_base_addr, SOFT_NPU_IFM1_BASE_ADDR) ;
#endif
                break ;
            }
            case 2/* input 2 */:
            {
                soft_npu_write(SOFT_NPU_IFM2_BASE_ADDR, pkg->ifm2_base_addr) ;
#ifdef PRINT_NPU_CMD
                printf("write data: %x to soft_npu at addr: %x\n", pkg->ifm2_base_addr, SOFT_NPU_IFM2_BASE_ADDR) ;
#endif
                break ;
            }            
            case 3/* input 3 */:
            {
                soft_npu_write(SOFT_NPU_IFM3_BASE_ADDR, pkg->ifm3_base_addr) ;
#ifdef PRINT_NPU_CMD
                printf("write data: %x to soft_npu at addr: %x\n", pkg->ifm3_base_addr, SOFT_NPU_IFM3_BASE_ADDR) ;
#endif
                break ;
            }            
            default:
            {
                printf("the input_num is more than 4 of node: %s, idx: %u\n", nd->name, i) ;
                break;
            }
        }
    }

    uint32_t freg = pkg->first_reg ;
    soft_npu_write(SOFT_NPU_FIRST_REG_ADDR, freg) ;
#ifdef PRINT_NPU_CMD
    printf("write data: %x to soft_npu at addr: %x\n", freg, SOFT_NPU_FIRST_REG_ADDR) ;
#endif

    write_pkg_normal_mode(&(pkg->pkg_info), 0) ;

    freg |= (1 << 14) ;
    soft_npu_write(SOFT_NPU_FIRST_REG_ADDR, freg) ;
#ifdef PRINT_NPU_CMD
    printf("write data: %x to soft_npu at addr: %x\n", freg, SOFT_NPU_FIRST_REG_ADDR) ;
#endif

    return 0 ;
}

int8_t run_npu_soft_dma_mode (soft_npu_pkg_info_t* pkg, rt_node_t* nd) {

    soft_npu_write(SOFT_NPU_OFM_BASE_ADDR, pkg->ofm_base_addr) ;
#ifdef PRINT_NPU_CMD
    printf("write data: %x to soft_npu at addr: %x\n", pkg->ofm_base_addr, SOFT_NPU_OFM_BASE_ADDR) ;
#endif

    for(unsigned int i = 0; i < nd->input_num; i++) {
        switch (i)
        {
            case 0/* input 0 */:
            {
                soft_npu_write(SOFT_NPU_IFM0_BASE_ADDR, pkg->ifm0_base_addr) ;
#ifdef PRINT_NPU_CMD
                printf("write data: %x to soft_npu at addr: %x\n", pkg->ifm0_base_addr, SOFT_NPU_IFM0_BASE_ADDR) ;
#endif
                break ;
            }
            case 1/* input 1 */:
            {
                soft_npu_write(SOFT_NPU_IFM1_BASE_ADDR, pkg->ifm1_base_addr) ;
#ifdef PRINT_NPU_CMD
                printf("write data: %x to soft_npu at addr: %x\n", pkg->ifm1_base_addr, SOFT_NPU_IFM1_BASE_ADDR) ;
#endif
                break ;
            }
            case 2/* input 2 */:
            {
                soft_npu_write(SOFT_NPU_IFM2_BASE_ADDR, pkg->ifm2_base_addr) ;
#ifdef PRINT_NPU_CMD
                printf("write data: %x to soft_npu at addr: %x\n", pkg->ifm2_base_addr, SOFT_NPU_IFM2_BASE_ADDR) ;
#endif
                break ;
            }            
            case 3/* input 3 */:
            {
                soft_npu_write(SOFT_NPU_IFM3_BASE_ADDR, pkg->ifm3_base_addr) ;
#ifdef PRINT_NPU_CMD
                printf("write data: %x to soft_npu at addr: %x\n", pkg->ifm3_base_addr, SOFT_NPU_IFM3_BASE_ADDR) ;
#endif
                break ;
            }            
            default:
            {
                printf("the input_num is more than 4 of node: %s, idx: %u\n", nd->name, i) ;
                break;
            }
        }
    }

    // config dma axi_master_transfer_limit
    uint32_t freg = pkg->first_reg ;
    freg |= (1 << 12) ;
    soft_npu_write(SOFT_NPU_FIRST_REG_ADDR, freg) ;
#ifdef PRINT_NPU_CMD
    printf("write data: %x to soft_npu at addr: %x\n", freg, SOFT_NPU_FIRST_REG_ADDR) ;
#endif

    // config dma pkg addr
    soft_npu_write(SOFT_DMA_REG_BASE_ADDR, pkg->pkg_info.addr) ;
#ifdef PRINT_NPU_CMD
    printf("write data: %x to soft_npu at addr: %x\n", pkg->pkg_info.addr, SOFT_DMA_REG_BASE_ADDR) ;
#endif

    // config dma pkg length
    soft_npu_write(SOFT_DMA_LENGTH_ADDR, pkg->pkg_info.length) ;
#ifdef PRINT_NPU_CMD
    printf("write data: %x to soft_npu at addr: %x\n", pkg->pkg_info.length, SOFT_DMA_LENGTH_ADDR) ;
#endif

    // config dma reg cfg start
    freg |= (1 << 14) ;
    soft_npu_write(SOFT_NPU_FIRST_REG_ADDR, freg) ;
#ifdef PRINT_NPU_CMD
    printf("write data: %x to soft_npu at addr: %x\n", freg, SOFT_NPU_FIRST_REG_ADDR) ;
#endif

    return 0 ;
}

int8_t run_npu_soft_yolo_normal_mode (yolo_pkg_info_t* pkg) {

    soft_npu_write(SOFT_NPU_OFM_BASE_ADDR, pkg->ofm_base_addr) ;
#ifdef PRINT_NPU_CMD
    printf("write data: %x to yolo at addr: %x\n", pkg->ofm_base_addr, SOFT_NPU_OFM_BASE_ADDR) ;
#endif
    soft_npu_write(SOFT_NPU_IFM0_BASE_ADDR, pkg->ifm0_base_addr) ;
#ifdef PRINT_NPU_CMD
    printf("write data: %x to yolo at addr: %x\n", pkg->ifm0_base_addr, SOFT_NPU_IFM0_BASE_ADDR) ;
#endif

    uint32_t freg = pkg->first_reg ;
    soft_npu_write(SOFT_NPU_FIRST_REG_ADDR, freg) ;
#ifdef PRINT_NPU_CMD
    printf("write data: %x to soft_npu at addr: %x\n", freg, SOFT_NPU_FIRST_REG_ADDR) ;
#endif

    write_pkg_normal_mode(&(pkg->pkg_info), 0) ;

    freg |= (1 << 14) ;
    soft_npu_write(SOFT_NPU_FIRST_REG_ADDR, freg) ;
#ifdef PRINT_NPU_CMD
    printf("write data: %x to soft_npu at addr: %x\n", freg, SOFT_NPU_FIRST_REG_ADDR) ;
#endif

    return 0 ;
}

int8_t run_npu_soft_yolo_dma_mode (yolo_pkg_info_t* pkg) {

    soft_npu_write(SOFT_NPU_OFM_BASE_ADDR, pkg->ofm_base_addr) ;
#ifdef PRINT_NPU_CMD
    printf("write data: %x to yolo at addr: %x\n", pkg->ofm_base_addr, SOFT_NPU_OFM_BASE_ADDR) ;
#endif
    soft_npu_write(SOFT_NPU_IFM0_BASE_ADDR, pkg->ifm0_base_addr) ;
#ifdef PRINT_NPU_CMD
    printf("write data: %x to yolo at addr: %x\n", pkg->ifm0_base_addr, SOFT_NPU_IFM0_BASE_ADDR) ;
#endif

    // config dma axi_master_transfer_limit
    uint32_t freg = pkg->first_reg ;
    freg |= (1 << 12) ;
    soft_npu_write(SOFT_NPU_FIRST_REG_ADDR, freg) ;
#ifdef PRINT_NPU_CMD
    printf("write data: %x to soft_npu at addr: %x\n", freg, SOFT_NPU_FIRST_REG_ADDR) ;
#endif

    // config dma pkg addr
    soft_npu_write(SOFT_DMA_REG_BASE_ADDR, pkg->pkg_info.addr) ;
#ifdef PRINT_NPU_CMD
    printf("write data: %x to soft_npu at addr: %x\n", pkg->pkg_info.addr, SOFT_DMA_REG_BASE_ADDR) ;
#endif

    // config dma pkg length
    soft_npu_write(SOFT_DMA_LENGTH_ADDR, pkg->pkg_info.length) ;
#ifdef PRINT_NPU_CMD
    printf("write data: %x to soft_npu at addr: %x\n", pkg->pkg_info.length, SOFT_DMA_LENGTH_ADDR) ;
#endif

    // config dma reg cfg start
    freg |= (1 << 14) ;
    soft_npu_write(SOFT_NPU_FIRST_REG_ADDR, freg) ;
#ifdef PRINT_NPU_CMD
    printf("write data: %x to soft_npu at addr: %x\n", freg, SOFT_NPU_FIRST_REG_ADDR) ;
#endif

    return 0 ;
}
