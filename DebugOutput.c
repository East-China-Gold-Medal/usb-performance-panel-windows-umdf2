#define _CRT_SECURE_NO_WARNINGS
#include "DebugOutput.h"
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