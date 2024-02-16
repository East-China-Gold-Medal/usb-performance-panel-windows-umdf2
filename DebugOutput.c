/* @file

	Implementation of an alternative debug interface that logs onto disk.
	SPDX-License-Identifier: WTFPL

*/
#define _CRT_SECURE_NO_WARNINGS
#include "DebugOutput.h"
#ifdef DEBUG_OUTPUT
FILE* debugFp;

VOID StartDebug(VOID) {
	debugFp = fopen("D:\\usb-perf-panel.log","w+");
	if (debugFp == NULL) {
		ExitProcess(3);
	}
}
VOID StopDebug(VOID) {
	fclose(debugFp);
}
#endif