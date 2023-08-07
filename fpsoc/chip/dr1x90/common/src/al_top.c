#include "al_type.h"
#include "al_core.h"
#include "al_params.h"

#define TSG_CTRL_EN_CNT_BIT_POS  (0)

AL_VOID Altop_Syscnts_CounterCtrl(AL_FUNCTION CntStatus)
{
    AL_REG32_SET_BIT(SYSCNT_S_BASE, TSG_CTRL_EN_CNT_BIT_POS, CntStatus);
}

