#include "MonitorThread.h"
typedef struct {
    ULONG sleepTime;
    PDEVICE_CONTEXT DevContext;
} THREADPARAM;
inline ULONGLONG CalculateDelta(FILETIME* first, FILETIME* last)
{
    ULONGLONG val1 = ((ULONGLONG)first->dwHighDateTime << 32) | (first->dwLowDateTime), val2 = ((ULONGLONG)last->dwHighDateTime << 32) | (last->dwLowDateTime);
    return val2 - val1;
}
DWORD WINAPI MonitorThreadFunction(LPVOID lpParam)
{
    MEMORYSTATUSEX statex;
    FILETIME idleTime, userTime, kernelTime, lastIdleTime, lastUserTime, lastKernelTime;
    statex.dwLength = sizeof(statex);
    GetSystemTimes(&lastIdleTime, &lastKernelTime, &lastUserTime);
    idleTime = lastIdleTime;
    kernelTime = lastKernelTime;
    userTime = lastUserTime;
    while (1) {
        Sleep(((THREADPARAM*)lpParam)->sleepTime);
        BOOL status = GlobalMemoryStatusEx(&statex);
        if (!status) {
#pragma message ("Exit With Val!")
        }
        else {
            status = GetSystemTimes(&idleTime, &kernelTime, &userTime);
            if (!status) {
#pragma message ("Exit With Val!")
            }
            else {
                ULONGLONG idleDelta = CalculateDelta(&lastIdleTime, &idleTime);
                ULONGLONG kernelDelta = CalculateDelta(&lastKernelTime, &kernelTime);
                ULONGLONG userDelta = CalculateDelta(&lastUserTime, &userTime);
                SendUsage(((THREADPARAM*)lpParam)->DevContext, DATA_CPU_USAGE, ((UCHAR)((kernelDelta + userDelta - idleDelta) * 255.0 / (kernelDelta + userDelta))));
                SendUsage(((THREADPARAM*)lpParam)->DevContext, DATA_RAM_USAGE, (UCHAR)(statex.dwMemoryLoad*2.55));
                lastIdleTime = idleTime;
                lastKernelTime = kernelTime;
                lastUserTime = userTime;
            }
        }
    }
}
DWORD StartMonitorThread(PDEVICE_CONTEXT DevContext)
{
    DWORD threadId;
    HANDLE threadHandle;
    THREADPARAM param = {
        .sleepTime = 1000,
        .DevContext = DevContext,
    };
    threadHandle = CreateThread(NULL,0,MonitorThreadFunction,&param,0,&threadId);
    if (threadHandle == NULL)
    {
        return 0;
    }
    return threadId;
}