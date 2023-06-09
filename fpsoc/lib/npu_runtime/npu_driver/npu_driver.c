#include <stdio.h>
#include "driver_api.h"
#include "npu_driver.h"
#include "npu_defines.h"

#define LOG_TAG "NPU_DRIVER"
#include "elog.h"

static AL_S8 write_pkg_normal_mode (pkg_info_t* pkg_info, AL_U8 is_hard_npu) {

    AL_U32 pkg_len = pkg_info->length;
    AL_U32* pkg_data = (AL_U32*) pkg_info->virt_addr ;
    AL_U8 stage = 0 ;
    AL_U8 is_same_addr = 0 ;
    AL_U8 is_err_data = 0 ;
    AL_U32 len = 0 ;
    AL_U32 cur_pos = 0 ;
    AL_U32 start_addr = 0 ;

    for(AL_U32 i = 0; i < pkg_len; i++) {
        AL_U32 data = pkg_data[i] ;
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
                AL_U32 cur_addr = 0 ;
                if (is_same_addr) {
                    cur_addr = start_addr ;
                } else {
                    cur_addr = start_addr + cur_pos * 4 ;
                }
                if (is_hard_npu) {
                    hard_npu_write((cur_addr-HARD_NPU_MASK), data) ;
                    log_d("write data: %x to hard_npu at addr: %x\n", data, (cur_addr-HARD_NPU_MASK)) ;
                } else {
                    soft_npu_write((cur_addr-SOFT_NPU_MASK), data) ;
                    log_d("write data: %x to soft_npu at addr: %x\n", data, (cur_addr-SOFT_NPU_MASK)) ;
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
                log_e("err data in pkg\n") ;
                return -1 ;
            }             
            default:
            {   
                log_e("err stage in decode pkg data\n") ;
                break;
            }
        }
    }

    return 0 ;
}

AL_S8 run_npu_hard_normal_mode (hard_npu_pkg_info_t* pkg) {

    hard_npu_write(HARD_NPU_MODE_LOAD_DATA_ADDR, pkg->mode_load_data) ;
    log_d("write data: %x to hard_npu at addr: %x\n", pkg->mode_load_data, HARD_NPU_MODE_LOAD_DATA_ADDR) ;
    
    hard_npu_write(HARD_NPU_IFM_BASE_ADDR, pkg->ifm_base_addr) ;
    log_d("write data: %x to hard_npu at addr: %x\n", pkg->ifm_base_addr, HARD_NPU_IFM_BASE_ADDR) ;
    
    hard_npu_write(HARD_NPU_FILTER_BASE_ADDR, pkg->flt_base_addr) ;
    log_d("write data: %x to hard_npu at addr: %x\n", pkg->flt_base_addr, HARD_NPU_FILTER_BASE_ADDR) ;

    hard_npu_write(HARD_NPU_OFM_BASE_ADDR, pkg->ofm_base_addr) ;
    log_d("write data: %x to hard_npu at addr: %x\n", pkg->ofm_base_addr, HARD_NPU_OFM_BASE_ADDR) ;

    write_pkg_normal_mode(&(pkg->pkg_info), 1) ;

    return 0 ;
}

AL_S8 run_npu_hard_dma_mode (hard_npu_pkg_info_t* pkg) {

    hard_npu_write(HARD_NPU_MODE_LOAD_DATA_ADDR, pkg->mode_load_data) ;
    log_d("write data: %x to hard_npu at addr: %x\n", pkg->mode_load_data, HARD_NPU_MODE_LOAD_DATA_ADDR) ;
    
    hard_npu_write(HARD_NPU_IFM_BASE_ADDR, pkg->ifm_base_addr) ;
    log_d("write data: %x to hard_npu at addr: %x\n", pkg->ifm_base_addr, HARD_NPU_IFM_BASE_ADDR) ;
    
    hard_npu_write(HARD_NPU_FILTER_BASE_ADDR, pkg->flt_base_addr) ;
    log_d("write data: %x to hard_npu at addr: %x\n", pkg->flt_base_addr, HARD_NPU_FILTER_BASE_ADDR) ;

    hard_npu_write(HARD_NPU_OFM_BASE_ADDR, pkg->ofm_base_addr) ;
    log_d("write data: %x to hard_npu at addr: %x\n", pkg->ofm_base_addr, HARD_NPU_OFM_BASE_ADDR) ;

    // config dma axi_master_transfer_limit
    hard_npu_write(HARD_AXI_MASTER_TRANSFER_LIMIT_ADDR, 0x1010) ;
    log_d("write data: %x to hard_npu at addr: %x\n", 0x1010, HARD_AXI_MASTER_TRANSFER_LIMIT_ADDR) ;

    // config dma pkg addr
    hard_npu_write(HARD_DMA_REG_BASE_ADDR, pkg->pkg_info.addr) ;
    log_d("write data: %x to hard_npu at addr: %x\n", pkg->pkg_info.addr, HARD_DMA_REG_BASE_ADDR) ;

    // config dma pkg length
    hard_npu_write(HARD_DMA_LENGTH_ADDR, (pkg->pkg_info.length)) ;
    log_d("write data: %x to hard_npu at addr: %x\n", (pkg->pkg_info.length), HARD_DMA_LENGTH_ADDR) ;

    // config dma reg cfg start
    hard_npu_write(HARD_DMA_READ_CFG_START_ADDR, 0x01) ;
    log_d("write data: %x to hard_npu at addr: %x\n", 0x01, HARD_DMA_READ_CFG_START_ADDR) ;

    return 0 ;
}

AL_S8 run_npu_soft_normal_mode (soft_npu_pkg_info_t* pkg, rt_node_t* nd) {

    soft_npu_write(SOFT_NPU_OFM_BASE_ADDR, pkg->ofm_base_addr) ;
    log_d("write data: %x to soft_npu at addr: %x\n", pkg->ofm_base_addr, SOFT_NPU_OFM_BASE_ADDR) ;

    for(unsigned int i = 0; i < nd->input_num; i++) {
        switch (i)
        {
            case 0/* input 0 */:
            {
                soft_npu_write(SOFT_NPU_IFM0_BASE_ADDR, pkg->ifm0_base_addr) ;
                log_d("write data: %x to soft_npu at addr: %x\n", pkg->ifm0_base_addr, SOFT_NPU_IFM0_BASE_ADDR) ;
                break ;
            }
            case 1/* input 1 */:
            {
                soft_npu_write(SOFT_NPU_IFM1_BASE_ADDR, pkg->ifm1_base_addr) ;
                log_d("write data: %x to soft_npu at addr: %x\n", pkg->ifm1_base_addr, SOFT_NPU_IFM1_BASE_ADDR) ;
                break ;
            }
            case 2/* input 2 */:
            {
                soft_npu_write(SOFT_NPU_IFM2_BASE_ADDR, pkg->ifm2_base_addr) ;
                log_d("write data: %x to soft_npu at addr: %x\n", pkg->ifm2_base_addr, SOFT_NPU_IFM2_BASE_ADDR) ;
                break ;
            }            
            case 3/* input 3 */:
            {
                soft_npu_write(SOFT_NPU_IFM3_BASE_ADDR, pkg->ifm3_base_addr) ;
                log_d("write data: %x to soft_npu at addr: %x\n", pkg->ifm3_base_addr, SOFT_NPU_IFM3_BASE_ADDR) ;
                break ;
            }            
            default:
            {
                log_d("the input_num is more than 4 of node: %s, idx: %u\n", nd->name, i) ;
                break;
            }
        }
    }

    AL_U32 freg = pkg->first_reg ;
    soft_npu_write(SOFT_NPU_FIRST_REG_ADDR, freg) ;
    log_d("write data: %x to soft_npu at addr: %x\n", freg, SOFT_NPU_FIRST_REG_ADDR) ;

    write_pkg_normal_mode(&(pkg->pkg_info), 0) ;

    freg |= (1 << 14) ;
    soft_npu_write(SOFT_NPU_FIRST_REG_ADDR, freg) ;
    log_d("write data: %x to soft_npu at addr: %x\n", freg, SOFT_NPU_FIRST_REG_ADDR) ;

    return 0 ;
}

AL_S8 run_npu_soft_dma_mode (soft_npu_pkg_info_t* pkg, rt_node_t* nd) {

    soft_npu_write(SOFT_NPU_OFM_BASE_ADDR, pkg->ofm_base_addr) ;
    log_d("write data: %x to soft_npu at addr: %x\n", pkg->ofm_base_addr, SOFT_NPU_OFM_BASE_ADDR) ;

    for(unsigned int i = 0; i < nd->input_num; i++) {
        switch (i)
        {
            case 0/* input 0 */:
            {
                soft_npu_write(SOFT_NPU_IFM0_BASE_ADDR, pkg->ifm0_base_addr) ;
                log_d("write data: %x to soft_npu at addr: %x\n", pkg->ifm0_base_addr, SOFT_NPU_IFM0_BASE_ADDR) ;
                break ;
            }
            case 1/* input 1 */:
            {
                soft_npu_write(SOFT_NPU_IFM1_BASE_ADDR, pkg->ifm1_base_addr) ;
                log_d("write data: %x to soft_npu at addr: %x\n", pkg->ifm1_base_addr, SOFT_NPU_IFM1_BASE_ADDR) ;
                break ;
            }
            case 2/* input 2 */:
            {
                soft_npu_write(SOFT_NPU_IFM2_BASE_ADDR, pkg->ifm2_base_addr) ;
                log_d("write data: %x to soft_npu at addr: %x\n", pkg->ifm2_base_addr, SOFT_NPU_IFM2_BASE_ADDR) ;
                break ;
            }            
            case 3/* input 3 */:
            {
                soft_npu_write(SOFT_NPU_IFM3_BASE_ADDR, pkg->ifm3_base_addr) ;
                log_d("write data: %x to soft_npu at addr: %x\n", pkg->ifm3_base_addr, SOFT_NPU_IFM3_BASE_ADDR) ;
                break ;
            }            
            default:
            {
                log_d("the input_num is more than 4 of node: %s, idx: %u\n", nd->name, i) ;
                break;
            }
        }
    }

    // config dma axi_master_transfer_limit
    AL_U32 freg = pkg->first_reg ;
    freg |= (1 << 12) ;
    soft_npu_write(SOFT_NPU_FIRST_REG_ADDR, freg) ;
    log_d("write data: %x to soft_npu at addr: %x\n", freg, SOFT_NPU_FIRST_REG_ADDR) ;

    // config dma pkg addr
    soft_npu_write(SOFT_DMA_REG_BASE_ADDR, pkg->pkg_info.addr) ;
    log_d("write data: %x to soft_npu at addr: %x\n", pkg->pkg_info.addr, SOFT_DMA_REG_BASE_ADDR) ;

    // config dma pkg length
    soft_npu_write(SOFT_DMA_LENGTH_ADDR, pkg->pkg_info.length) ;
    log_d("write data: %x to soft_npu at addr: %x\n", pkg->pkg_info.length, SOFT_DMA_LENGTH_ADDR) ;

    // config dma reg cfg start
    freg |= (1 << 14) ;
    soft_npu_write(SOFT_NPU_FIRST_REG_ADDR, freg) ;
    log_d("write data: %x to soft_npu at addr: %x\n", freg, SOFT_NPU_FIRST_REG_ADDR) ;

    return 0 ;
}

AL_S8 run_npu_soft_yolo_normal_mode (yolo_pkg_info_t* pkg) {

    soft_npu_write(SOFT_NPU_OFM_BASE_ADDR, pkg->ofm_base_addr) ;
    log_d("write data: %x to yolo at addr: %x\n", pkg->ofm_base_addr, SOFT_NPU_OFM_BASE_ADDR) ;
    soft_npu_write(SOFT_NPU_IFM0_BASE_ADDR, pkg->ifm0_base_addr) ;
    log_d("write data: %x to yolo at addr: %x\n", pkg->ifm0_base_addr, SOFT_NPU_IFM0_BASE_ADDR) ;

    AL_U32 freg = pkg->first_reg ;
    soft_npu_write(SOFT_NPU_FIRST_REG_ADDR, freg) ;
    log_d("write data: %x to soft_npu at addr: %x\n", freg, SOFT_NPU_FIRST_REG_ADDR) ;

    write_pkg_normal_mode(&(pkg->pkg_info), 0) ;

    freg |= (1 << 14) ;
    soft_npu_write(SOFT_NPU_FIRST_REG_ADDR, freg) ;
    log_d("write data: %x to soft_npu at addr: %x\n", freg, SOFT_NPU_FIRST_REG_ADDR) ;

    return 0 ;
}

AL_S8 run_npu_soft_yolo_dma_mode (yolo_pkg_info_t* pkg) {

    soft_npu_write(SOFT_NPU_OFM_BASE_ADDR, pkg->ofm_base_addr) ;
    log_d("write data: %x to yolo at addr: %x\n", pkg->ofm_base_addr, SOFT_NPU_OFM_BASE_ADDR) ;
    soft_npu_write(SOFT_NPU_IFM0_BASE_ADDR, pkg->ifm0_base_addr) ;
    log_d("write data: %x to yolo at addr: %x\n", pkg->ifm0_base_addr, SOFT_NPU_IFM0_BASE_ADDR) ;

    // config dma axi_master_transfer_limit
    AL_U32 freg = pkg->first_reg ;
    freg |= (1 << 12) ;
    soft_npu_write(SOFT_NPU_FIRST_REG_ADDR, freg) ;
    log_d("write data: %x to soft_npu at addr: %x\n", freg, SOFT_NPU_FIRST_REG_ADDR) ;

    // config dma pkg addr
    soft_npu_write(SOFT_DMA_REG_BASE_ADDR, pkg->pkg_info.addr) ;
    log_d("write data: %x to soft_npu at addr: %x\n", pkg->pkg_info.addr, SOFT_DMA_REG_BASE_ADDR) ;

    // config dma pkg length
    soft_npu_write(SOFT_DMA_LENGTH_ADDR, pkg->pkg_info.length) ;
    log_d("write data: %x to soft_npu at addr: %x\n", pkg->pkg_info.length, SOFT_DMA_LENGTH_ADDR) ;

    // config dma reg cfg start
    freg |= (1 << 14) ;
    soft_npu_write(SOFT_NPU_FIRST_REG_ADDR, freg) ;
    log_d("write data: %x to soft_npu at addr: %x\n", freg, SOFT_NPU_FIRST_REG_ADDR) ;

    return 0 ;
}
