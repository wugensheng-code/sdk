#ifndef UNITY_TEST_H
#define UNITY_TEST_H

#define SECTION(x) __attribute__((section(x)))
#define USED __attribute__((used))

struct UNITY_TEST_CASE_T
{
    const char *name;
    int (*setup)(void);
    void (*testfunc)(void);
    int (*teardown)(void);

    const char *file;
    int line;
};

#define UNITY_TESTCASE_REGISTER(testfunc, name, setup, teardown)            \
    USED static const struct UNITY_TEST_CASE_T testfunc##_test_testcase     \
    SECTION("unity_section") =                                              \
    {                                                                       \
        name,                                                               \
        setup,                                                              \
        testfunc,                                                           \
        teardown,                                                           \
        __FILE__,                                                           \
        __LINE__,                                                           \
    }                                                                       \

struct UNITY_FIXTURE_T
{
    int Verbose;
    int Silent;
    unsigned int RepeatCount;
    const char* NameFilter;
    const char* GroupFilter;
};

int UityTestInit(void);

int UnityMain(int argc, const char* argv[]);

#endif /* UNITY_TEST_H */