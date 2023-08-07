#ifndef AL_IIC_DEV_H
#define AL_IIC_DEV_H

#ifdef __cplusplus
extern "C" {
#endif

#include "al_iic_ll.h"

#define AL_IIC_CMD_WRITE            (0 << 8)
#define AL_IIC_CMD_READ             (1 << 8)
#define AL_IIC_STOP_ENABLE          (1 << 9)
#define AL_IIC_STOP_DISABLE         (0 << 9)
#define AL_IIC_RESTART_ENABLE       (1 << 10)
#define AL_IIC_RESTART_DISABLE      (0 << 10)

/* Refer to spec */
#define AL_IIC_SS_MIN_SCL_HIGH         4000
#define AL_IIC_SS_MIN_SCL_LOW          4700
#define AL_IIC_FS_MIN_SCL_HIGH         600
#define AL_IIC_FS_MIN_SCL_LOW          1300
#define AL_IIC_HS_MIN_SCL_HIGH_100PF   60
#define AL_IIC_HS_MIN_SCL_LOW_100PF    120

#define AL_IIC_MAX_SLAVE_ADDR          0x3FF

#define DEFAULT_SDA_SCL_AT_LOW_TIMEOUT 40000

typedef enum
{
    AL_IIC_MODE_MASTER     = 0,
    AL_IIC_MODE_SLAVE      = 1
} AL_IIC_OperationModeEnum;

typedef struct
{
    AL_IIC_OperationModeEnum    Mode;
    AL_IIC_AddrModeEnum         AddrMode;

    /* Just for master, slave not used */
    AL_IIC_SpeedModeEnum        SpeedMode;

    /* Just for slave, master not used */
    AL_U16                      SarAddress;
} AL_IIC_InitStruct;

typedef struct
{
    AL_U8                     *BufferPtr;
    AL_U32                    RequestedCnt;
    AL_U32                    HandledCnt;
} AL_IIC_BufferStruct;

typedef enum
{
    AL_IIC_STATE_NOT_INIT     = (0x0),
    AL_IIC_STATE_READY        = (0x01 << 0),
    AL_IIC_STATE_TX_BUSY      = (0x01 << 1),
    AL_IIC_STATE_RX_BUSY      = (0x01 << 2)
} AL_IIC_StateEnum;

typedef enum
{
    AL_IIC_INTR_RX_UNDER        = (0x1 << 0),
    AL_IIC_INTR_RX_OVER         = (0x1 << 1),
    AL_IIC_INTR_RX_FULL         = (0x1 << 2),
    AL_IIC_INTR_TX_OVER         = (0x1 << 3),
    AL_IIC_INTR_TX_EMPTY        = (0x1 << 4),
    AL_IIC_INTR_RD_REQ          = (0x1 << 5),
    AL_IIC_INTR_TX_ABRT         = (0x1 << 6),
    AL_IIC_INTR_RX_DONE         = (0x1 << 7),
    AL_IIC_INTR_ACTIVITY        = (0x1 << 8),
    AL_IIC_INTR_STOP_DET        = (0x1 << 9),
    AL_IIC_INTR_START_DET       = (0x1 << 10),
    AL_IIC_INTR_GEN_CALL        = (0x1 << 11),
    AL_IIC_INTR_RESTART_DET     = (0x1 << 12),
    AL_IIC_INTR_MASTER_ON_HOLD  = (0x1 << 13),
    AL_IIC_INTR_SCL_STUCK_ATLOW = (0x1 << 14),
} AL_IIC_IntrStatusEnum;

typedef enum
{
    AL_IIC_EVENT_RX_UNDER       = (0x1 << 0),
    AL_IIC_EVENT_RX_OVER        = (0x1 << 1),
    AL_IIC_EVENT_TX_OVER        = (0x1 << 2),
    AL_IIC_EVENT_RD_REQ         = (0x1 << 3),
    AL_IIC_EVENT_TX_ABRT        = (0x1 << 4),
    AL_IIC_EVENT_RX_DONE        = (0x1 << 5),
    AL_IIC_EVENT_ACTIVITY       = (0x1 << 6),
    AL_IIC_EVENT_STOP_DET       = (0x1 << 7),
    AL_IIC_EVENT_START_DET      = (0x1 << 8),
    AL_IIC_EVENT_GEN_CALL       = (0x1 << 9),
} AL_IIC_EventIdEnum;

typedef struct
{
    AL_IIC_EventIdEnum      Event;
    AL_U32                  EventData;
} AL_IIC_EventStruct;

typedef AL_VOID (*AL_IIC_EventCallBack)(AL_IIC_EventStruct *IicEvent, AL_VOID *CallbackRef);

typedef enum
{
    AL_IIC_OP_STATE_MASTER_TX       = (0x1 << 0),
    AL_IIC_OP_STATE_MASTER_RX       = (0x1 << 1),
    AL_IIC_OP_STATE_SLAVE_TX        = (0x1 << 2),
    AL_IIC_OP_STATE_SLAVE_RX        = (0x1 << 3),
} AL_IIC_OperationStateEnum;

/* Only for master */
typedef enum
{
    AL_IIC_CMD_OPTION_NONE                  = (0x0 << 0),
    AL_IIC_CMD_OPTION_NO_STOP_RESTART       = (0x1 << 0),
    AL_IIC_CMD_OPTION_STOP                  = (0x1 << 1),
    AL_IIC_CMD_OPTION_RESTART               = (0x1 << 2),
} AL_IIC_CmdOptionEnum;

typedef struct
{
    AL_IIC_HwConfigStruct       HwConfig;
    AL_IIC_InitStruct           Configs;
    AL_IIC_BufferStruct         SendBuffer;
    AL_IIC_BufferStruct         RecvBuffer;
    AL_IIC_EventCallBack        EventCallBack;
    AL_VOID                     *EventCallBackRef;
    AL_U32                      ErrorCode;
    AL_IIC_StateEnum            State;
    AL_IIC_OperationStateEnum   OpState;
    AL_IIC_CmdOptionEnum        CmdOption;
} AL_IIC_DevStruct;

typedef enum
{
    AL_IIC_IOCTL_SET_ADDR_MODE,
    AL_IIC_IOCTL_GET_ADDR_MODE,
    AL_IIC_IOCTL_SET_SPEED_MODE,
    AL_IIC_IOCTL_GET_SPEED_MODE,
} AL_IIC_IoCtlCmdEnum;

/* IIC error code define */
typedef enum
{
    AL_IIC_INVALID_DEVICE_ID    = 0x100,
    AL_IIC_NOT_READY            = 0x101,
    AL_IIC_BUSY                 = 0x102,
    AL_IIC_ERR_IOCTL_CMD        = 0x103,

} AL_IIC_ErrorCodeEnum;

#define AL_IIC_ERR_ILLEGAL_PARAM            (AL_DEF_ERR(AL_IIC, AL_LOG_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM))
#define AL_IIC_ERR_NOT_READY                (AL_DEF_ERR(AL_IIC, AL_LOG_LEVEL_ERROR, AL_IIC_NOT_READY))
#define AL_IIC_ERR_BUSY                     (AL_DEF_ERR(AL_IIC, AL_LOG_LEVEL_ERROR, AL_IIC_BUSY))
#define AL_IIC_ERR_IOCTL_CMD_NOT_SUPPORT    (AL_DEF_ERR(AL_IIC, AL_LOG_LEVEL_ERROR, AL_IIC_ERR_IOCTL_CMD))

AL_IIC_HwConfigStruct *AlIic_Dev_LookupConfig(AL_U32 DevId);

AL_S32 AlIic_Dev_Init(AL_IIC_DevStruct *Iic, AL_IIC_HwConfigStruct *HwConfig, AL_IIC_InitStruct *InitConfig);

AL_S32 AlIic_Dev_MasterSendData(AL_IIC_DevStruct *Iic, AL_U16 SlaveAddr, AL_U8 *SendBuf, AL_U32 SendSize);

AL_S32 AlIic_Dev_MasterRecvData(AL_IIC_DevStruct *Iic, AL_U16 SlaveAddr, AL_U8 *RecvBuf, AL_U32 RecvSize);

AL_S32 AlIic_Dev_SlaveSendData(AL_IIC_DevStruct *Iic, AL_U8 *SendBuf, AL_U32 SendSize);

AL_S32 AlIic_Dev_SlaveRecvData(AL_IIC_DevStruct *Iic, AL_U8 *RecvBuf, AL_U32 RecvSize);

AL_VOID AlIic_Dev_IntrHandler(AL_VOID *instance);

AL_S32 AlIic_Dev_RegisterEventCallBack(AL_IIC_DevStruct *Iic, AL_IIC_EventCallBack Callback, void *CallbackRef);

AL_S32 AlIic_Dev_MasterSetCmdOption(AL_IIC_DevStruct *Iic, AL_IIC_CmdOptionEnum CmdOption);

AL_IIC_CmdOptionEnum AlIic_Dev_MasterGetCmdOption(AL_IIC_DevStruct *Iic);

AL_S32 AlIic_Dev_IoCtl(AL_IIC_DevStruct *Iic, AL_IIC_IoCtlCmdEnum Cmd, AL_VOID *Data);

#ifdef __cplusplus
}
#endif

#endif /* AL_IIC_DEV_H */
