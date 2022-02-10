#ifndef MONITOR_THREAD
#define MONITOR_THREAD
#include <processthreadsapi.h>
#include <sysinfoapi.h>
#include "Device.h"

DWORD StartMonitorThread(PDEVICE_CONTEXT DevContext);
#endif
