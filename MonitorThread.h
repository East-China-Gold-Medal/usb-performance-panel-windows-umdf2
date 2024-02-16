/* @file

    The main monitor thread that collects & sends data.
    SPDX-License-Identifier: WTFPL

*/
#ifndef MONITOR_THREAD
#define MONITOR_THREAD
#include "Device.h"

WDFTIMER StartMonitorThread(WDFDEVICE Device);
#endif
