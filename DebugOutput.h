/* @file

	Definition of an alternative debug interface that logs onto disk. 
	SPDX-License-Identifier: WTFPL

*/
#pragma once
#include <windows.h>
#include <stdio.h>

//#define DEBUG_OUTPUT

#ifdef DEBUG_OUTPUT
extern FILE* debugFp;
VOID StartDebug(VOID);
#define PrintDebug(x,...) fprintf(debugFp,x,__VA_ARGS__);fflush(debugFp)
VOID StopDebug(VOID);
#else
#define StartDebug()
#define PrintDebug(x,...)
#define StopDebug()
#endif