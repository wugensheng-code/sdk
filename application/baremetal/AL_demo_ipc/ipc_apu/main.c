#include <stdio.h>
#include <stdlib.h>
#include "nuclei_sdk_soc.h"
#include "al9000_ipc.h"
// #include "al9000_dmac.h"

int main(){
	// APU2CSU_IPC();
	// CSU2APU_IPC();
	RPU2CSUACK_IPC();
	RPU2CSU_IPC();
	// CSU2RPU_IPC();
	// CSU2RPUACK_IPC();
	return 0;
}
