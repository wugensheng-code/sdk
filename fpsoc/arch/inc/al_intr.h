#ifndef AL_INTR_H
#define AL_INTR_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include "al_type.h"
#include "al_errno.h"

#define   AL_INTR_NAME_MAX       (32)

typedef enum
{
    ILLEGAL_TRIG_MODE       = 0x100,
} AL_INTR_ErrorCode;


/* Error define */
#define AL_INTR_ERR_ILLEGAL_MODE         (AL_DEF_ERR(AL_INTR, AL_ERR_LEVEL_ERROR, ILLEGAL_TRIG_MODE))
#define AL_INTR_ERR_ILLEGAL_PARAM        (AL_DEF_ERR(AL_INTR, AL_ERR_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM))


typedef enum
{
    NON_VECTOR_INTERRUPT    = 0x0,
    VECTOR_INTERRUPT        = 0x1,
} AL_INTR_VectorCfg;

typedef enum {
    LEVEL_HIGH_TRIGGER      = 0x0,
    LEVEL_LOW_TRIGGER       = 0x1,
    POSTIVE_EDGE_TRIGGER    = 0x2,
    NEGTIVE_EDGE_TRIGGER    = 0x3,
    DOUBLE_EDGE_TRIGGER     = 0x4,
} AL_INTR_TriggerMode;

typedef AL_VOID (*AL_INTR_Func)(AL_VOID *Param);

typedef struct {
#ifdef __riscv
    AL_INTR_VectorCfg       VectorMode;
#endif
    AL_INTR_TriggerMode     TrigMode;
    AL_S32                  Priority;
#ifdef SUPPORT_DEBUG_INFO
    AL_U8                   IntrName[AL_INTR_NAME_MAX];
#endif
} AL_INTR_AttrStrct;

typedef struct
{
    AL_INTR_Func 	        Func;
    AL_VOID            	    *Param;
} AL_INTR_HandlerStruct;


#if (defined __riscv || defined __riscv__)
#define AL_DEFAULT_ATTR(name)  AL_INTR_AttrStrct name = {   \
            .TrigMode   =  LEVEL_HIGH_TRIGGER,              \
            .Priority   =  1,                               \
            .VectorMode =  NON_VECTOR_INTERRUPT,            \
};
#else
#define AL_DEFAULT_ATTR(Name)  AL_INTR_AttrStrct Name = {   \
            .TrigMode   =  LEVEL_HIGH_TRIGGER,              \
            .Priority   =  1,                               \
};
#endif


AL_VOID AlIntr_ControllerInit(void);

AL_S32 AlIntr_SetInterrupt(AL_U32 IntrId, AL_FUNCTION state);

AL_S32 AlIntr_RegHandler(AL_S32 IntrId, AL_INTR_AttrStrct *IntrAttr, AL_INTR_Func *Func, AL_VOID *Paran);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* AL_INTR_H */
