#include "al_type.h"
#include "al_chip.h"

#include "gic_v3_addr.h"
#include "gic_v3.h"

AL_VOID AlChip_Dr1m90Init(AL_VOID)
{
	#if (defined SUPPORT_NONSECURE || defined SWITCH_TO_EL0_FROM_EL3)
		//if SUPPORT_NONSECURE, gic-v3 init in EL3
		//if SWITCH_TO_EL0_FROM_EL3, boot to EL0, only to test
	#else
		gicv3_init();
	#endif

	#ifndef SWITCH_TO_EL0_FROM_EL3
		generic_timer_init();
	#endif
}