
#include "al_intr.h"
#include "al_params.h"
#include "npu_interrupt.h"
#include "npu_driver.h"

volatile AL_S8 soft_irq = 0 ; // 0: no soft node interrupt, 1: soft node interrupt triggered.
volatile AL_S8 hard_irq = 0 ; // 0: no hard node interrupt, 1: hard node interrupt triggered.
static volatile AL_S8 yuv2rgb_finish_flag = 0;

static AL_VOID hard_node_irq_handler(AL_VOID* param) {
    // printf("entering hard node ISR\n") ;

    /* set hard_irq flag */
    hard_irq = 1 ;

    /* clear interrupt */
    hard_npu_write(HARDNPU__CCU__CLEAR_INTERRUPT__ADDR, 0x1) ;
}

/* soft node irq handler */
static AL_VOID soft_node_irq_handler(AL_VOID* param) {

    // printf("entering soft node ISR\n") ;

    /* special process for yuv2rgb */
    AL_U32 res = soft_npu_read(SOFTNPU__SOFT_SHARE2__CFG_INIT__ADDR) ;
    AL_U32 mask = 0x01 ;
    res &= mask ;
    if (0x01 == res) {
        yuv2rgb_finish_flag = 1;
        // printf("test irq\n");
        soft_npu_write(SOFTNPU__SOFT_SHARE2__CFG_INIT__ADDR, 0x00); // select device
        soft_npu_write(SOFTNPU__SOFT_SHARE__CLEAR_INTERRUPT__ADDR, 0x01) ;
        return;
    }

    /* set soft_irq flag */
    soft_irq = 1 ;

    /* clear interrupt */
    soft_npu_write(SOFTNPU__SOFT_SHARE__CLEAR_INTERRUPT__ADDR, 0x1) ;
}

AL_S8 intr_init() {

    /* register irq handler */
    AlIntr_RegHandler(NPU_SOFT_IRQn, NULL, soft_node_irq_handler, NULL);
    AlIntr_RegHandler(NPU_HARD_IRQn, NULL, hard_node_irq_handler, NULL);

    return 0;

}

AL_S8 intr_deinit() {
    return 0;
}

AL_S8 wait_for_hard_npu_irq() {
    return 0;
}

AL_S8 wait_for_soft_npu_irq() {
    return 0;
}

AL_S8 wait_for_yuv2rgb_irq() {
    while(yuv2rgb_finish_flag == 0) { }
    yuv2rgb_finish_flag = 0;
    return 0;
}

