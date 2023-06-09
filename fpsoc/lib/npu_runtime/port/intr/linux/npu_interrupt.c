#include <pthread.h>

#include "npu_interrupt.h"
#include "npu_driver.h"

#define LOG_TAG "NPU_INTR"
#include "elog.h"

volatile int hard_irq_fd = -1;
volatile int soft_irq_fd = -1;
static AL_U32 info = 1;

static pthread_mutex_t cond_mutex;
static pthread_cond_t hardnn_finish_flag;
static pthread_cond_t softnn_finish_flag;
static pthread_cond_t yuv2rgb_finish_flag;

static pthread_t t_hard_id, t_soft_id;

static AL_S8 wait_irq_from_soft() {
    AL_U64 nb = read(soft_irq_fd, &info, sizeof(info));
    if (nb == (AL_U64)sizeof(info)) {

        /* Do something in response to the interrupt. */
        log_d("soft_npu Interrupt #%u!\n", info);

        /* recognize which device the interrupt is generated from */
        uint32_t res = soft_npu_read(SOFTNPU__SOFT_SHARE2__CFG_INIT__ADDR) ;
        uint32_t mask = 0x01 ;
        res &= mask ;
        if (0x01 == res) {
            log_d("intr from yuv2rgb\n");
            pthread_cond_signal(&yuv2rgb_finish_flag);

            soft_npu_write(SOFTNPU__SOFT_SHARE2__CFG_INIT__ADDR, 0x00); // select device
        } else {
            log_d("intr from softnn\n");
            pthread_cond_signal(&softnn_finish_flag);
        }

        // reset interrupt in soft npu
        soft_npu_write(SOFTNPU__SOFT_SHARE__CLEAR_INTERRUPT__ADDR, 0x1);
        
        // reset interrupt in UIO
        info = 1;
        nb = write(soft_irq_fd, &info, sizeof(info));
        if (nb != (AL_U64)sizeof(info)) {
            log_e("write soft_npu failed\n");
            return -1;
        }
    } else {
        log_e("read from soft_npu_device failed, expected size: %d, read size: %d\n", sizeof(info), nb);
        return -1;
    }
    return 0;
}

static AL_S8 wait_irq_from_hard() {
    AL_U64 nb = read(hard_irq_fd, &info, sizeof(info));
    if (nb == (AL_U64)sizeof(info)) {
        pthread_cond_signal(&hardnn_finish_flag);

        // reset interrupt in hard npu
        hard_npu_write(HARDNPU__CCU__CLEAR_INTERRUPT__ADDR, 0x1) ;
        log_d("hard_npu Interrupt #%u!\n", info);

        // reset interrupt in UIO
        info = 1;
        nb = write(hard_irq_fd, &info, sizeof(info));
        if (nb != (AL_U64)sizeof(info)) {
            log_e("write hard_npu failed\n");
            return -1;
        }
    } else {
        log_w("read from hard_npu_device failed, expected size: %d, read size: %d\n", sizeof(info), nb);
        return -1;
    }

    return 0;
}

static AL_VOID* thread_func_check_hard_irq(AL_VOID* arg) {
    while (1) {
        if (wait_irq_from_hard()) {
            log_e("fail to wait irq from hard npu\n");
            pthread_exit(NULL);
        }
    }
    return NULL;
}

static AL_VOID* thread_func_check_soft_irq(AL_VOID* arg) {
    while (1) {
        if (wait_irq_from_soft()) {
            log_e("fail to wait irq from soft npu\n");
            pthread_exit(NULL);
        }
    }
    return NULL;
}

AL_S8 intr_init() {
    pthread_mutex_init(&cond_mutex, NULL);
    pthread_cond_init(&hardnn_finish_flag, NULL);
    pthread_cond_init(&softnn_finish_flag, NULL);
    pthread_cond_init(&yuv2rgb_finish_flag, NULL);
        
    /* creat a thread to check irq from hard */
    pthread_create(&t_hard_id, NULL, thread_func_check_hard_irq, NULL);
    pthread_detach(t_hard_id);
    
    /* creat a thread to check irq from soft */
    pthread_create(&t_soft_id, NULL, thread_func_check_soft_irq, NULL);
    pthread_detach(t_soft_id);

    return 0;
}

AL_S8 intr_deinit() {
    pthread_mutex_destroy(&cond_mutex);
    pthread_cond_destroy(&hardnn_finish_flag);
    pthread_cond_destroy(&softnn_finish_flag);
    pthread_cond_destroy(&yuv2rgb_finish_flag);

    /* destroy the thread to check irq from hard */
    // could do better to make sure that the thread is cancelled successfully
    pthread_cancel(t_hard_id);

    /* destroy the thread to check irq from soft */
    // could do better to make sure that the thread is cancelled successfully
    pthread_cancel(t_soft_id);

    return 0;
}


AL_S8 wait_for_hard_npu_irq() {

    pthread_mutex_lock(&cond_mutex);
    pthread_cond_wait(&hardnn_finish_flag, &cond_mutex);
    pthread_mutex_unlock(&cond_mutex);

    return 0;
}

AL_S8 wait_for_soft_npu_irq() {

    pthread_mutex_lock(&cond_mutex);
    pthread_cond_wait(&softnn_finish_flag, &cond_mutex);
    pthread_mutex_unlock(&cond_mutex);

    return 0;
}

AL_S8 wait_for_yuv2rgb_irq() {

    pthread_mutex_lock(&cond_mutex);
    pthread_cond_wait(&yuv2rgb_finish_flag, &cond_mutex);
    pthread_mutex_unlock(&cond_mutex);

    return 0;
}