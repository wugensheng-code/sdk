#include "unity_test.h"
#include "unity.h"
#include "al_cli.h"

#include <string.h>

struct UNITY_FIXTURE_T UnityFixture;

struct UNITY_TEST_CASE_T *TestCaseTable;
int TestCaseNum;

/* defined in the link script */
extern unsigned int _unity_test_start;
extern unsigned int _unity_test_end;

static unsigned int UityStrlen(const char* str)
{
    unsigned int str_len = 0;

    while (*str != 0)
    {
        str_len++;
        str++;
    }

    return str_len;
}

static int UityStrncmp(const char* s1, const char* s2, unsigned int n)
{
    unsigned char c1 = '\0';
    unsigned char c2 = '\0';

    if (n >= 4)
    {
        unsigned int n4 = n >> 2;
        do
        {
            c1 = (unsigned char) *s1++;
            c2 = (unsigned char) *s2++;
            if (c1 == '\0' || c1 != c2)
                return c1 - c2;
            c1 = (unsigned char) *s1++;
            c2 = (unsigned char) *s2++;
            if (c1 == '\0' || c1 != c2)
                return c1 - c2;
            c1 = (unsigned char) *s1++;
            c2 = (unsigned char) *s2++;
            if (c1 == '\0' || c1 != c2)
                return c1 - c2;
            c1 = (unsigned char) *s1++;
            c2 = (unsigned char) *s2++;
            if (c1 == '\0' || c1 != c2)
            return c1 - c2;
        } while (--n4 > 0);
        n &= 3;
    }

    while (n > 0)
    {
        c1 = (unsigned char) *s1++;
        c2 = (unsigned char) *s2++;
        if (c1 == '\0' || c1 != c2)
            return c1 - c2;
        n--;
    }

  return c1 - c2;
}

static int UityStrcmp(const char* p1, const char* p2)
{
    const unsigned char *s1 = (const unsigned char *) p1;
    const unsigned char *s2 = (const unsigned char *) p2;
    unsigned char c1, c2;

    do
    {
        c1 = (unsigned char) *s1++;
        c2 = (unsigned char) *s2++;
        if (c1 == '\0')
        return c1 - c2;
    } while (c1 == c2);

    return c1 - c2;
}

static char *UityStrstr(const char *haystack, const char *needle)
{
    const char* s1 = haystack;
    const char* s2 = needle;
    const char* cur = haystack;

    while (*cur)
    {
        s1 = cur;
        s2 = needle;
        while (*s1 && *s2 && (*s1 == *s2))
        {
            s1++;
            s2++;
        }
        if (*s2 == '\0')
        {
            return (char*)cur;
        }
        cur++;
    }

    return NULL;
}

int UityTestInit(void)
{
    /* initialize the test case table.*/
    TestCaseTable = (struct UNITY_TEST_CASE_T *)&_unity_test_start;
    TestCaseNum = (struct UNITY_TEST_CASE_T *)&_unity_test_end - TestCaseTable;

    UnityPrint("Unity test is initialize success.");
    UNITY_PRINT_EOL();
    UnityPrint("total unity test case num: ");
    UnityPrintNumberUnsigned(TestCaseNum);
    UNITY_PRINT_EOL();

    return TestCaseNum;
}

void UnityConcludeFixtureTest(void)
{
    if (Unity.CurrentTestIgnored)
    {
        Unity.TestIgnores++;
        UNITY_PRINT_EOL();
    }
    else if (!Unity.CurrentTestFailed)
    {
        if (UnityFixture.Verbose)
        {
            UnityPrint(" ");
            UnityPrint(UnityStrPass);
            UNITY_EXEC_TIME_STOP();
            UNITY_PRINT_EXEC_TIME();
            UNITY_PRINT_EOL();
        }
    }
    else /* Unity.CurrentTestFailed */
    {
        Unity.TestFailures++;
        UNITY_PRINT_EOL();
    }

    Unity.CurrentTestFailed = 0;
    Unity.CurrentTestIgnored = 0;
}

static int selected(const char* filter, const char* name)
{
    if (filter == 0)
        return 1;
    return UityStrstr(name, filter) ? 1 : 0;
}

static int testSelected(const char* test)
{
    if (UnityFixture.NameFilter == 0)
        return 1;

    if (UityStrlen(test) != UityStrlen(UnityFixture.NameFilter))
        return 0;

    return ((UityStrncmp(UnityFixture.NameFilter, test, UityStrlen(test))) ? 0 : 1);
}

static int groupSelected(const char* group)
{
    return selected(UnityFixture.GroupFilter, group);
}

void UnityTestRunner(struct UNITY_TEST_CASE_T* test)
{
    if (testSelected(test->name) && groupSelected(test->name))
    {
        Unity.TestFile = test->file;
        Unity.CurrentTestName = test->name;
        Unity.CurrentTestLineNumber = test->line;
        if (UnityFixture.Verbose)
        {
            UnityPrint(test->name);
        #ifndef UNITY_REPEAT_TEST_NAME
            Unity.CurrentTestName = NULL;
        #endif
        }
        else if (UnityFixture.Silent)
        {
            /* Do Nothing */
        }
        else
        {
            UNITY_OUTPUT_CHAR('.');
            UNITY_OUTPUT_CHAR('.');
            UNITY_OUTPUT_CHAR('.');
            UNITY_PRINT_EOL();
        }

        Unity.NumberOfTests++;

        UNITY_EXEC_TIME_START();

        if (TEST_PROTECT())
        {
            if (test->setup != NULL)
                test->setup();
            test->testfunc();
        }
        if (TEST_PROTECT())
        {
            if (test->teardown != NULL)
                test->teardown();
        }
        UnityConcludeFixtureTest();
    }
}

int runAllTests()
{
    for (int i = 0; i < TestCaseNum; i++) {
        UnityTestRunner(&TestCaseTable[i]);
    }
}

static void announceTestRun(unsigned int runNumber)
{
    UNITY_PRINT_EOL();
    UnityPrint("Unity test run ");
    UnityPrintNumberUnsigned(runNumber+1);
    UnityPrint(" of ");
    UnityPrintNumberUnsigned(UnityFixture.RepeatCount);
    UNITY_PRINT_EOL();
}

static void UnityDisplayTestCase()
{
    UNITY_PRINT_EOL();
    UnityPrint("Unity test case list:");
    UNITY_PRINT_EOL();
    UNITY_PRINT_EOL();
    for (int i = 0; i < TestCaseNum; i++) {
        UnityPrint("  Test case name  :");
        UnityPrint(TestCaseTable[i].name);
        UNITY_PRINT_EOL();
    }

    UNITY_PRINT_EOL();
    UNITY_PRINT_EOL();
}

static void UnityHelp()
{
    /* Usage */
    UNITY_PRINT_EOL();
    UnityPrint("Command: unitytest");
    UNITY_PRINT_EOL();
    UnityPrint("Runs a series of unit tests.");
    UNITY_PRINT_EOL();
    UNITY_PRINT_EOL();
    UnityPrint("When no flag is specified, all tests are run.");
    UNITY_PRINT_EOL();
    UNITY_PRINT_EOL();
    UnityPrint("Optional flags:");
    UNITY_PRINT_EOL();
    UnityPrint("  -v          Verbose output: show all tests executed even if they pass");
    UNITY_PRINT_EOL();
    UnityPrint("  -s          Silent mode: minimal output showing only test failures");
    UNITY_PRINT_EOL();
    UnityPrint("  -g NAME     Only run tests in groups that contain the string NAME");
    UNITY_PRINT_EOL();
    UnityPrint("  -n NAME     Only run tests whose name perfect match the string NAME");
    UNITY_PRINT_EOL();
    UnityPrint("  -r NUMBER   Repeatedly run all tests NUMBER times");
    UNITY_PRINT_EOL();
    UnityPrint("  -h, -help   Display this help message");
    UNITY_PRINT_EOL();
    UnityPrint("  -a, -all    Display all test case name");
    UNITY_PRINT_EOL();
    UNITY_PRINT_EOL();
    UNITY_PRINT_EOL();
}

int UnityGetCommandLineOptions(int argc, const char* argv[])
{
    int i;
    UnityFixture.Verbose = 0;
    UnityFixture.Silent = 0;
    UnityFixture.GroupFilter = 0;
    UnityFixture.NameFilter = 0;
    UnityFixture.RepeatCount = 1;

    if (argc == 1)
        return 0;

    for (i = 1; i < argc; )
    {
        if (UityStrcmp(argv[i], "-h") == 0 || UityStrcmp(argv[i], "-help") == 0)
        {
            UnityHelp();
            return 1;  /* Exit without running the tests */
        }
        else if (UityStrcmp(argv[i], "-a") == 0 || UityStrcmp(argv[i], "-all") == 0)
        {
            UnityDisplayTestCase();
            return 1;  /* Exit without running the tests */
        }
        else if (UityStrcmp(argv[i], "-v") == 0)
        {
            UnityFixture.Verbose = 1;
            i++;
        }
        else if (UityStrcmp(argv[i], "-s") == 0)
        {
            UnityFixture.Silent = 1;
            i++;
        }
        else if (UityStrcmp(argv[i], "-g") == 0)
        {
            i++;
            if (i >= argc)
                return 1;
            UnityFixture.GroupFilter = argv[i];
            i++;
        }
        else if (UityStrcmp(argv[i], "-n") == 0)
        {
            i++;
            if (i >= argc)
                return 1;
            UnityFixture.NameFilter = argv[i];
            i++;
        }
        else if (UityStrcmp(argv[i], "-r") == 0)
        {
            UnityFixture.RepeatCount = 2;
            i++;
            if (i < argc)
            {
                if (*(argv[i]) >= '0' && *(argv[i]) <= '9')
                {
                    unsigned int digit = 0;
                    UnityFixture.RepeatCount = 0;
                    while (argv[i][digit] >= '0' && argv[i][digit] <= '9')
                    {
                        UnityFixture.RepeatCount *= 10;
                        UnityFixture.RepeatCount += (unsigned int)argv[i][digit++] - '0';
                    }
                    i++;
                }
            }
        }
        else
        {
            /* ignore unknown parameter */
            i++;
        }
    }
    return 0;
}

int UnityMain(int argc, const char* argv[])
{
    int result = UnityGetCommandLineOptions(argc, argv);
    unsigned int r;
    if (result != 0)
        return result;

    for (r = 0; r < UnityFixture.RepeatCount; r++)
    {
        UnityBegin(argv[0]);
        announceTestRun(r);
        runAllTests();
        if (!UnityFixture.Verbose) UNITY_PRINT_EOL();
        UnityEnd();
    }

    return (int)Unity.TestFailures;
}

void UnityIgnoreTest(const char* printableName, const char* group, const char* name)
{
    if (testSelected(name) && groupSelected(group))
    {
        Unity.NumberOfTests++;
        Unity.TestIgnores++;
        if (UnityFixture.Verbose)
        {
            UnityPrint(printableName);
            UNITY_PRINT_EOL();
        }
        else if (UnityFixture.Silent)
        {
            /* Do Nothing */
        }
        else
        {
            UNITY_OUTPUT_CHAR('!');
        }
    }
}

AL_CLI_CMD_REGISTER(UnityMain, unitytest, unity test);