/*
 * xil_assert.c
 *
 *  Created on: 2022年7月18日
 *      Author: caojiacheng
 */

# include"xil_assert.h"
u32 Xil_AssertStatus;
s32 Xil_AssertWait = 1;

void Xil_Assert(const char8 *File, s32 Line)
{
	if (Xil_AssertCallbackRoutine != 0) {
		(*Xil_AssertCallbackRoutine)(File, Line);
	}

	while (Xil_AssertWait != 0) {
	}
}

void Xil_AssertSetCallback(Xil_AssertCallback Routine)
{
	Xil_AssertCallbackRoutine = Routine;
}
