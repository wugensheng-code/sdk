#include "tx_api.h"

/* Define default main entry point. You may override the main entry if needed */
__attribute__((weak)) int main(void)
{
    /* Enter the ThreadX kernel.  */
    tx_kernel_enter();

    return 0;
}
