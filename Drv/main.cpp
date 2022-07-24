#include "helper.h"
VOID DriverUnload(PDRIVER_OBJECT objDriver)
{
    UNREFERENCED_PARAMETER(objDriver);
    // ж���豸
    PDEVICE_OBJECT pDevice;
    pDevice = objDriver->DeviceObject;
    IoDeleteDevice(pDevice);

    // ɾ����������
    UNICODE_STRING symLinkName;
    RtlInitUnicodeString(&symLinkName,SYMBOL_LINK_NAME);
    IoDeleteSymbolicLink(&symLinkName);

    DbgPrint("Driver unloaded\r\n");
}

extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT objDriver, PUNICODE_STRING strRegPath)
{
    UNREFERENCED_PARAMETER(strRegPath);

    DbgPrint("Driver loaded!\r\n");

    // ������ǲ����
    objDriver->MajorFunction[IRP_MJ_CREATE] = DispatchRoutine;
    objDriver->MajorFunction[IRP_MJ_CLOSE] = DispatchRoutine;
    objDriver->MajorFunction[IRP_MJ_READ] = DispatchRoutine;
    objDriver->MajorFunction[IRP_MJ_WRITE] = DispatchRoutine;
    objDriver->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DispatchRoutine;
    // �����豸
    CreateMyDevice(objDriver);

    ASSERT(objDriver);
    objDriver->DriverUnload = DriverUnload;
    return STATUS_SUCCESS;
}