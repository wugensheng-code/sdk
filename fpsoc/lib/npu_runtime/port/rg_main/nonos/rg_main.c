

#include "rg_main.h"

#define LOG_TAG "RG_MAIN"
#include "elog.h"

extern AL_S8 handle_soft_node_irq ();
extern AL_S8 handle_hard_node_irq ();

AL_S8 run_main() {

    extern volatile AL_U32 hard_node_num;
    extern volatile AL_U32 soft_node_num;

    extern volatile AL_U32 hard_finish_cnt;
    extern volatile AL_U32 soft_finish_cnt;

    extern volatile AL_S8 soft_irq;
    extern volatile AL_S8 hard_irq;

        
    while ((hard_finish_cnt < hard_node_num) || (soft_finish_cnt < soft_node_num)) {
        if (soft_irq) {
            /* reset soft_irq flag */
            soft_irq = 0 ;

            soft_finish_cnt++;

            if (handle_soft_node_irq ()) {
                log_e("handle soft node irq failed\n");
                return -1;
            }
        }
        if (hard_irq) {
            /* reset hard_irq flag */
            hard_irq = 0 ;

            hard_finish_cnt++;

            if (handle_hard_node_irq ()) {
                log_e("handle hard node irq failed\n");
                return -1;
            }
        }
    }

    return 0;

}
