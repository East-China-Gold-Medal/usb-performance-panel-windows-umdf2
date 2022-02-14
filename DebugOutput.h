#pragma once
#include <windows.h>
#include <stdio.h>

extern FILE* debugFp;
VOID StartDebug(VOID);
#define PrintDebug(x,...) fprintf(debugFp,x,__VA_ARGS__);fflush(debugFp)
VOID StopDebug(VOID);