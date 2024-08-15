#ifndef __AL_CMOCKA_UTILS_HAL_H_
#define __AL_CMOCKA_UTILS_HAL_H_

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

struct test_group{
    const char *name;
    const size_t tc_num;
    const struct CMUnitTest *group_tests;
    CMFixtureFunction group_setup;
    CMFixtureFunction group_teardown;
};

void cmocka_init(void);

#ifdef UNIT_TESTING
#endif

#define CMOCKA_TC_DEFINE(_group, _group_setup, _group_teardown, _initial_state)    \
    __attribute__((used)) static const struct CMUnitTest _cmocka_test_case##_group  \
    __attribute__((section("UtestTcTab"))) =                                      \
    {                                                                             \
        .name = #_group,                                                          \
        .test_func = (_group),                                                    \
        .setup_func = (_group_setup),                                             \
        .teardown_func = (_group_teardown),                                       \
        .initial_state = (_initial_state),                                        \
    }

#endif
