#include "al_core.h"
#include "core_feature_base.h"
#include "al_chip.h"
#include "ext_timer.h"

AL_VOID AlChip_Dr1V90Init(AL_VOID)
{
	__RV_CSR_CLEAR(CSR_MMISC_CTL,MMISC_CTL_BPU);

	Exception_Init();
	ECLIC_Init();
	SysTimer_clk_sel();
}
