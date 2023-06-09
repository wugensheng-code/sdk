#include <pthread.h>
#include "rg_main.h"
#include "npu_interrupt.h"

#define LOG_TAG "RG_MAIN"
#include "elog.h"

extern AL_S8 handle_soft_node_irq ();
extern AL_S8 handle_hard_node_irq ();

extern volatile AL_U32 hard_node_num;
extern volatile AL_U32 soft_node_num;

extern volatile AL_U32 hard_finish_cnt;
extern volatile AL_U32 soft_finish_cnt;


AL_VOID* thread_func_hard_npu(AL_VOID* arg) {
    AL_S8 exit_val = 0;

    while (1) {
        if (wait_for_hard_npu_irq()) {
            log_e("wait for hard npu irq failed\n");
            exit_val = -1;
            pthread_exit((AL_VOID*)&exit_val);
        }

        hard_finish_cnt += 1;

        if (handle_hard_node_irq()) {
            log_e("handle hard node irq failed\n");
            exit_val = -1;
            pthread_exit((AL_VOID*)&exit_val);
        }

        if (hard_finish_cnt == hard_node_num) {
            pthread_exit((AL_VOID*)&exit_val);
        }
    }

    return NULL;
}

AL_VOID* thread_func_soft_npu(AL_VOID* arg) {
    AL_S8 exit_val = 0;

    while (1) {
        if (wait_for_soft_npu_irq()) {
            log_e("wait for soft npu irq failed\n");
            exit_val = -1;
            pthread_exit((AL_VOID*)&exit_val);
        }

        soft_finish_cnt += 1;

        if (handle_soft_node_irq()) {
            log_e("handle soft node irq failed\n");
            exit_val = -1;
            pthread_exit((AL_VOID*)&exit_val);
        }

        if (soft_finish_cnt == soft_node_num) {
            pthread_exit((AL_VOID*)&exit_val);
        }
    }

    return NULL;
}


AL_S8 run_main() {

    pthread_t tid1, tid2;
    pthread_create(&tid1, NULL, thread_func_hard_npu, NULL);
    pthread_create(&tid2, NULL, thread_func_soft_npu, NULL);

    AL_S8 *re_val_1;
    AL_S8 *re_val_2;
    pthread_join(tid1, (AL_VOID**)&re_val_1);
    pthread_join(tid2, (AL_VOID**)&re_val_2);

    if ((0 != (*re_val_1)) || (0 != (*re_val_2))) {
        return -1;
    }

    return 0;

}
