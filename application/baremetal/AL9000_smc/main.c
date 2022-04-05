#include <stdio.h>
#include <stdlib.h>
#include "nuclei_sdk_soc.h"
#include "Onfi.h"


int main(void)
{
	/* case 1*/

	uint8_t device_id[5];



	Onfi_CmdReset();
	Onfi_CmdReadId(0x00);

	SmcReadData(ONFI_CMD_READ_ID2, ONFI_CMD_READ_ID_END_TIMING, &device_id[0], 5);

	/* case 2
	Nand_Size_TypeDef nandSize;
*/


	return 0;
}





