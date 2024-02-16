/* @file

    The main monitor thread that collects & sends data.
    SPDX-License-Identifier: WTFPL

*/
#include "MonitorThread.h"
#include "DebugOutput.h"
#include <processthreadsapi.h>
#include <sysinfoapi.h>
#include <wdftimer.h>
inline ULONGLONG CalculateDelta(FILETIME* first, FILETIME* last)
{
    ULONGLONG val1 = ((ULONGLONG)first->dwHighDateTime << 32) | (first->dwLowDateTime), val2 = ((ULONGLONG)last->dwHighDateTime << 32) | (last->dwLowDateTime);
    return val2 - val1;
}
void MonitorThreadFunction(WDFTIMER Timer)
{
    PDEVICE_CONTEXT DevContext = GetDeviceContext(WdfTimerGetParentObject(Timer));
    UCHAR cpu, mem;
    BOOL status = GlobalMemoryStatusEx(&(DevContext->Statex));
    if (!status) {
        PrintDebug("Get Memory Info failed:%x.\n", status);
        StopDebug();
        ExitProcess(3);
    }
    else {
        status = GetSystemTimes(&(DevContext->IdleTime), &(DevContext->KernelTime), &(DevContext->UserTime));
        if (!status) {
            PrintDebug("Get CPU Info failed:%x.\n", status);
            StopDebug();
            ExitProcess(3);
        }
        else {
            ULONGLONG idleDelta = CalculateDelta(&(DevContext->LastIdleTime), &(DevContext->IdleTime));
            ULONGLONG kernelDelta = CalculateDelta(&(DevContext->LastKernelTime), &(DevContext->KernelTime));
            ULONGLONG userDelta = CalculateDelta(&(DevContext->LastUserTime), &(DevContext->UserTime));
            cpu = ((UCHAR)((kernelDelta + userDelta - idleDelta) * 255.0 / (kernelDelta + userDelta)));
            mem = (UCHAR)(DevContext->Statex.dwMemoryLoad * 2.55);
            PrintDebug("CPU:%x,MEM:%x\n", cpu, mem);
            SendUsage(DevContext, DATA_CPU_USAGE, cpu);
            SendUsage(DevContext, DATA_RAM_USAGE, mem);
            DevContext->LastIdleTime = DevContext->IdleTime;
            DevContext->LastKernelTime = DevContext->KernelTime;
            DevContext->LastUserTime = DevContext->UserTime;
        }
    }
}
WDFTIMER StartMonitorThread(WDFDEVICE Device)
{
    PrintDebug("Starting monitor thread......");
    WDF_TIMER_CONFIG  timerConfig;
    WDF_OBJECT_ATTRIBUTES  timerAttributes;
    WDFTIMER  timerHandle;
    NTSTATUS  status;
    PDEVICE_CONTEXT DevContext = GetDeviceContext(Device);
    GetSystemTimes(&(DevContext->LastIdleTime), &(DevContext->LastKernelTime), &(DevContext->LastUserTime));
    DevContext->IdleTime = DevContext->LastIdleTime;
    DevContext->KernelTime = DevContext->LastKernelTime;
    DevContext->UserTime = DevContext->LastUserTime;
    DevContext->Statex.dwLength = sizeof(MEMORYSTATUSEX);
    PrintDebug("Config Init.\n");
    WDF_TIMER_CONFIG_INIT_PERIODIC(&timerConfig,MonitorThreadFunction, DevContext->DelayTime);
    timerConfig.TolerableDelay = TolerableDelayUnlimited;
    PrintDebug("Attribute Init.\n");
    WDF_OBJECT_ATTRIBUTES_INIT(&timerAttributes);
    timerAttributes.ParentObject = Device;
    PrintDebug("Timer Create:%p,%p,%p.\n", &timerConfig, &timerAttributes, &timerHandle);
    status = WdfTimerCreate(&timerConfig,&timerAttributes,&timerHandle);
    if (!NT_SUCCESS(status)) {
        PrintDebug("Failed:%x.\n",status);
        timerHandle = NULL;
    }
    else {
        PrintDebug("Success.\n");
        WdfTimerStart(timerHandle, WDF_REL_TIMEOUT_IN_MS(DevContext->DelayTime));
    }
    return timerHandle;
}