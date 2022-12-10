#include <stdio.h>
#include <stdlib.h>
#include "nuclei_sdk_soc.h"
#include "al9000_ipc.h"
// #include "al9000_dmac.h"

int main(){
	// __RV_CSR_CLEAR(CSR_MMISC_CTL,MMISC_CTL_BPU);
	RPU2APU3_IPC_test();
	return 0;
}
