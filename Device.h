#ifndef DEVICE_H
#define DEVICE_H
#ifdef __cplusplus
extern "C" {
#endif
#include <windows.h>
#include <wdf.h>
#include <usb.h>
#include <wdfusb.h>
#include <initguid.h>
typedef enum {
    DATA_CPU_USAGE = 0b00000001,
    DATA_RAM_USAGE = 0b00000010,
    //...
    DATA_MAX_OUT = 0b10000000
}PanelDataCapability;
typedef enum {
    COMMAND_QUERY_CAP = 0xFF,
    COMMAND_SET_USAGE = 0xFE
}HostOperationCommand;
typedef struct _DEVICE_CONTEXT {
    WDFUSBDEVICE        UsbDevice;
    FILETIME IdleTime, UserTime, KernelTime, LastIdleTime, LastUserTime, LastKernelTime;
    MEMORYSTATUSEX Statex;
    PanelDataCapability DevicePanelDataCapability;
    ULONG DelayTime;
    WDFTIMER Timer;
} DEVICE_CONTEXT, * PDEVICE_CONTEXT;
#define CONTROL_CHANNEL 0

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(DEVICE_CONTEXT, GetDeviceContext)
extern const __declspec(selectany) LONGLONG DEFAULT_CONTROL_TRANSFER_TIMEOUT = 5 * -1 * WDF_TIMEOUT_TO_SEC;
_IRQL_requires_(PASSIVE_LEVEL)
NTSTATUS SendUsage(PDEVICE_CONTEXT DevContext, PanelDataCapability cap, UCHAR data);

NTSTATUS UsbPerformancePanelCreateDevice(_Inout_ PWDFDEVICE_INIT DeviceInit);

NTSTATUS GetDataCapability(PDEVICE_CONTEXT DevContext, PanelDataCapability* ReceiveBuffer);
EVT_WDF_DEVICE_PREPARE_HARDWARE UsbPerformancePanelEvtDevicePrepareHardware;

#ifdef __cplusplus
}
#endif
#endif