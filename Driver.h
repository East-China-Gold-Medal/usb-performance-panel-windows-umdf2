/* @file

    Definition of flow (Ba-Gu) code.
    SPDX-License-Identifier: WTFPL

*/


#include <windows.h>
#include <wdf.h>
#include <usb.h>
#include <wdfusb.h>
#include <initguid.h>

#include "device.h"
#include "trace.h"

EXTERN_C_START

//
// WDFDRIVER Events
//

DRIVER_INITIALIZE DriverEntry;
EVT_WDF_DRIVER_DEVICE_ADD UsbPerformancePanelEvtDeviceAdd;
EVT_WDF_OBJECT_CONTEXT_CLEANUP UsbPerformancePanelEvtDriverContextCleanup;

EXTERN_C_END
