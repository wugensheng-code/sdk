/* FreeRTOS includes. */
#include "FreeRTOS_POSIX.h"

/* FreeRTOS+POSIX. */
#include "FreeRTOS_POSIX/pthread.h"
#include "FreeRTOS_POSIX/mqueue.h"
#include "FreeRTOS_POSIX/time.h"
#include "FreeRTOS_POSIX/fcntl.h"
#include "FreeRTOS_POSIX/errno.h"

/* System headers. */
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>
#include <al_core.h>
#include <cmocka_utils.h>
#include "shell.h"

#define LOOP_NUM    3
#define RUN_ALL     0xFFFA
extern statistics_init();
extern TimerHandle_t statistics_timer;

static struct {
    const struct CMUnitTest *tc_table;
    AL_U32 *fail_num;
    size_t tc_num;
    AL_U32 tc_loop;
    AL_U32 run_num;
    AL_BOOL stress_testing;
    // TaskHandle_t caller;
} _control_block = {0};


static AL_S32 cmocka_tc_list(AL_U32 Argc, AL_S8 *Argv[])
{
    size_t i = 0;

    if (_control_block.tc_num) {
        AL_LOG(AL_LOG_LEVEL_INFO, " \r\ntestcase list : \r\n");
    } else {
        AL_LOG(AL_LOG_LEVEL_INFO, " \r\nNo test cases\r\n");
    }
    

    for (i = 0; i < _control_block.tc_num; i++)
    {
        AL_LOG(AL_LOG_LEVEL_INFO,"[test case]: %10s;\r\n", _control_block.tc_table[i].name);
    }

    return AL_OK;
}
SHELL_EXPORT_CMD(
SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN)|SHELL_CMD_DISABLE_RETURN,
tc_list, cmocka_tc_list, list test case);

static void print_result()
{
        // xTimerDelete(statistics_timer, 0);
        AL_LOG(AL_LOG_LEVEL_INFO, "\r\n\r\n\r\n");
        AL_LOG(AL_LOG_LEVEL_INFO, "[ RESULT ] Run %d times in total: \r\n", _control_block.tc_loop);
        AL_LOG(AL_LOG_LEVEL_INFO, "[ RESULT ] test cases result list below: \r\n");


        for (int i=0; i < ((_control_block.run_num == RUN_ALL)? _control_block.tc_num: 1); i++) {
            AL_LOG(AL_LOG_LEVEL_INFO, "[ RESULT ] %s test failed %d times\r\n", _control_block.tc_table[i].name,
            _control_block.fail_num[i]);
        }

        
        AL_LOG(AL_LOG_LEVEL_INFO, "[ FINISHED ]\r\n");
}

static void run(const char *tc_name)
{
    AL_U32 index;
    size_t i = 0;
    AL_BOOL found = AL_FALSE;
    size_t num_tests = _control_block.tc_num;
    
    _control_block.run_num = RUN_ALL;

    for (index = 0; index < _control_block.tc_loop; index++) 
    {
        if (tc_name) {
            AL_U32 len = strlen(tc_name);

            while (i < num_tests)
            {
                if (memcmp(_control_block.tc_table[i].name, tc_name, len) != 0) {
                    i++;
                    continue;
                }
                num_tests = 1;
                found = AL_TRUE;
                _control_block.run_num = i;
                break;
            }
        }


        if (tc_name && !found) {
            AL_LOG(AL_LOG_LEVEL_INFO, "[ FINISHED ] %s Not find\r\n", tc_name);
            return;
        }

        
        _control_block.fail_num = (AL_U32*)calloc(num_tests, sizeof(AL_U32));

        AL_LOG(AL_LOG_LEVEL_INFO, "\r\n");

        _control_block.fail_num[i] += _cmocka_run_group_tests("tests", &_control_block.tc_table[i], 
            num_tests, NULL, NULL);

        if (!_control_block.stress_testing) {
            free(_control_block.fail_num);
            _control_block.fail_num = NULL;
        }

    }

        // xTimerDelete(statistics_timer, 0);
    return;
}

static pthread_t** create_thread_pool()
{
    int ret;
    pthread_attr_t thread_attr = {0};
    pthread_t **thread_pool = (pthread_t*)malloc(sizeof(pthread_t*) * _control_block.tc_num);

    if (!thread_pool)
    {
        return NULL;
    }

    pthread_attr_init(&thread_attr);
    pthread_attr_setstacksize(&thread_attr, 8192);
    pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_JOINABLE);

    for (int i=0; i < _control_block.tc_num; i++)
    {
        ret = pthread_create(&thread_pool[i], &thread_attr, run, _control_block.tc_table[i].name);
        if (ret)
        {
            free(thread_pool);
            return NULL;
        }
    }

    return thread_pool;
}

static AL_S32 cmocka_tc_run(int argc, char *argv[])
{
    static char test_name[256];
    int ret = 0;
    void *param;
    memset(test_name, 0x0, sizeof(test_name));
    // _control_block.caller = xTaskGetCurrentTaskHandle();
    _control_block.tc_loop = 1;
    pthread_t **thread_pool;

    if (argc == 1) {
        param = NULL;
    } else if (argc ==2 || argc == 3) {

        if (strcmp(argv[1], "-s") == 0) {
            _control_block.stress_testing = true;
            if (argc == 3) {
                _control_block.tc_loop = atoi((const char *)argv[2]);
            } else {
                _control_block.tc_loop = LOOP_NUM;
            }
                
        } else {
            strncpy(test_name, (const char *)argv[1], sizeof(test_name) -1);
            param = (void*)test_name;

            if (argc == 3) _control_block.tc_loop = atoi((const char *)argv[2]);
        }

    } else {
        AL_LOG(AL_LOG_LEVEL_INFO, 'Unsupported parameters');
    }

    // statistics_init();


    // run(param);

    if (_control_block.stress_testing) {
        thread_pool = create_thread_pool();
        for (int i = 0; i < _control_block.tc_num; i++) {
            ret = pthread_join(thread_pool[i], NULL);
            if (ret) 
            {
                AL_LOG(AL_LOG_LEVEL_FATAL, "Wait for thread termination failed\r\n");
                return AL_OK;
            }
        }
    } else {
        pthread_attr_t thread_attr = {0};
        struct sched_param thread_sched_param = {0};
        pthread_attr_init(&thread_attr);
        pthread_attr_setstacksize(&thread_attr, 81920);
        pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_JOINABLE);
        pthread_t cmock_handler;
        ret = pthread_create(&cmock_handler, &thread_attr, run, param);
        if (ret) 
        {
            AL_LOG(AL_LOG_LEVEL_INFO, 'Failed to create test thread');
            return AL_OK;
        }
        ret = pthread_join(cmock_handler, NULL); 
        if (ret) 
        {
            AL_LOG(AL_LOG_LEVEL_FATAL, "Wait for thread termination failed\r\n");
            return AL_OK;
        }
    }

    if (_control_block.fail_num)
    {
        print_result();
        free(_control_block.fail_num);
    }

    return AL_OK;
}
SHELL_EXPORT_CMD(
SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN)|SHELL_CMD_DISABLE_RETURN,
tc_run, cmocka_tc_run, run test cases);


void cmocka_init(void)
{
                      /* for GCC Compiler */
    extern const int __rt_utest_tc_tab_start;
    extern const int __rt_utest_tc_tab_end;
    _control_block.tc_table = (struct CMUnitTest*)&__rt_utest_tc_tab_start;
    _control_block.tc_num = (struct CMUnitTest*)&__rt_utest_tc_tab_end - _control_block.tc_table;
    AL_LOG(AL_LOG_LEVEL_INFO, "cmocka is initialize success.\r\n");
    AL_LOG(AL_LOG_LEVEL_INFO, "total testcase num: (%d)\r\n", _control_block.tc_num);

    return;
}
