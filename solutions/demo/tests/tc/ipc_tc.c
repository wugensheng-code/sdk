#include <common.h>
#include <string.h>
#include <stdlib.h>

/*******************************************************************************/
/**************************** Ipc Mailbox test *********************************/
/*******************************************************************************/
AL_VOID AlIpc_Mailbox_tc(AL_VOID)
{
    AL_S32 Ret = AL_OK;
    AL_U32 Value = 0xffee;
    AL_U32 OutValue;

    AL_IpcMailBox_HalStruct *Mailbox_Handle;

    AL_LOG(AL_LOG_LEVEL_INFO, "Ipc Mailbox test\r\n");

    for (int i = 0; i < 3; i++)
    {
        Ret = AlIPC_Hal_MboxInit(&Mailbox_Handle, i);
        if ( Ret != AL_OK) {
            break;
        }

        Ret = AlIpc_Hal_MboxWrite(Mailbox_Handle, Value);
        if ( Ret != AL_OK) {
            break;
        }

        AL_LOG(AL_LOG_LEVEL_INFO, "Ipc Mailbox write 0x%x\r\n", Value);

        Ret = AlIpc_Hal_MboxRead(Mailbox_Handle, &OutValue);
        if ( Ret != AL_OK) {
            break;
        }

        if (Value != OutValue) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Ipc Mailbox fail write(0x%x) read(0x%x)\r\n", Value, OutValue);
            Ret = AL_ERROR;
            break;
        }

        Value++;
    }

    if (Ret == AL_OK) {
        AL_LOG(AL_LOG_LEVEL_INFO, "Ipc Mailbox test success\r\n");
    } else {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Ipc Mailbox test fail\r\n");
    }

    assert_int_equal(Ret, AL_OK);
}


/*******************************************************************************/
/**************************** Ipc Spinlock test *********************************/
/*******************************************************************************/

AL_VOID AlIpc_Spinlock_tc(AL_VOID)
{
    AL_S32 Ret = AL_OK;
    AL_U32 Key = 0;
    AL_U32 Same_Key = 0;
    AL_IpcSpinLock_HalStruct *Spinlock_Handle;

    AL_LOG(AL_LOG_LEVEL_INFO, "Ipc spin lock test\r\n");

    for (int i = 0; i < 4; i++)
    {
        AlIpc_Hal_SpinLockInit(&Spinlock_Handle, i);

        AL_LOG(AL_LOG_LEVEL_INFO, "try take the spin lock\r\n");

        Key = AlIpc_Hal_SpinLockTake(&Spinlock_Handle, AL_WAITING_NO);

        if (Key) {
            AL_LOG(AL_LOG_LEVEL_INFO, "Successfully obtained the spin lock\r\n");
        }

        Same_Key = AlIpc_Hal_SpinLockTake(&Spinlock_Handle, AL_WAITING_NO);

        if (Same_Key) {
            AL_LOG(AL_LOG_LEVEL_INFO, "Attempt to obtain the same spin lock, acquisition failed \r\n");
        } else {
            AL_LOG(AL_LOG_LEVEL_ERROR, "Successfully obtained the same spin lock, testing failed \r\n");
            Ret = AL_ERROR;
            break;
        }

        AlIpc_Hal_SpinLockRelease(&Spinlock_Handle);

        AL_LOG(AL_LOG_LEVEL_INFO, "release the spin lock\r\n");
    }

    if (Ret == AL_OK) {
        AL_LOG(AL_LOG_LEVEL_INFO, "Ipc spinlock test success\r\n");
    } else {
        AL_LOG(AL_LOG_LEVEL_INFO, "Ipc spinlock test failed\r\n");
    }


    assert_int_equal(Ret, AL_OK);
}




CMOCKA_TC_DEFINE(AlIpc_Mailbox_tc, NULL, NULL, NULL);
CMOCKA_TC_DEFINE(AlIpc_Spinlock_tc, NULL, NULL, NULL);