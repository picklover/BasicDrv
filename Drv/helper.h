#pragma once
#include <ntifs.h>
//#include <ntstrsafe.h>

#define DEVICE_NAME	L"\\Device\\mywdm_device"
#define SYMBOL_LINK_NAME L"\\??\\Mywdm_device_symbollink2"

#define add_code CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define sub_code CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS)

NTSTATUS DispatchRoutine(PDEVICE_OBJECT pDeviceObj, PIRP pIrp);

NTSTATUS CreateMyDevice(PDRIVER_OBJECT pDriverObj)
{
	NTSTATUS status;

	UNICODE_STRING devName; // 设备名称
	RtlInitUnicodeString(&devName, DEVICE_NAME);
	// 创建设备
	PDEVICE_OBJECT pDevObj;
	status = IoCreateDevice(pDriverObj, 0, &devName, FILE_DEVICE_UNKNOWN, 0, TRUE, &pDevObj);

	if (!NT_SUCCESS(status))
	{
		DbgPrint("create device failed!\r\n");
		return status;
	}
	pDevObj->Flags |= DO_BUFFERED_IO; // 设置缓冲区读写方式

	// 创建符号链接
	UNICODE_STRING symLinkName;
	RtlInitUnicodeString(&symLinkName, SYMBOL_LINK_NAME);
	status = IoCreateSymbolicLink(&symLinkName, &devName);
	if (!NT_SUCCESS(status))
	{
		DbgPrint("Create Symbol link failed!\r\n");
		IoDeleteDevice(pDevObj);
		return status;
	}

	return STATUS_SUCCESS;
}

NTSTATUS DispatchRoutine(PDEVICE_OBJECT pDeviceObj, PIRP pIrp)
{
	UNREFERENCED_PARAMETER(pDeviceObj);
	ULONG info = 0;
	PUCHAR buffer = (PUCHAR)pIrp->AssociatedIrp.SystemBuffer;
	PIO_STACK_LOCATION pstack = IoGetCurrentIrpStackLocation(pIrp); // 获取当前irp堆栈指针
	ULONG mf = pstack->MajorFunction;

	switch (mf)
	{
		case IRP_MJ_CREATE:
		{
			DbgPrint("IRP create!\r\n");
			break;
		}
		case IRP_MJ_CLOSE:
		{
			DbgPrint("IRP close!\r\n");
			break;
		}
		case IRP_MJ_READ:
		{
			DbgPrint("IRP read!\r\n");
			buffer[0] = 0x41;
			buffer[1] = 0x42;
			buffer[2] = 0x43;
			buffer[3] = 0x44;
			buffer[4] = 0x45;
			buffer[5] = 0x00;
			info = 6;
			break;
		}
		case IRP_MJ_WRITE:
		{
			DbgPrint("IRP write!\r\n");
			DbgPrint("%d%d%d%d%d%d%d%d", buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5], buffer[6], buffer[7]);
			break;
		}
		case IRP_MJ_DEVICE_CONTROL:
		{
			DbgPrint("IRP control!\r\n");
			
			ULONG in_size = pstack->Parameters.DeviceIoControl.InputBufferLength;
			ULONG out_size = pstack->Parameters.DeviceIoControl.OutputBufferLength;
			DbgPrint("in_size: %d, out_size: %d", in_size, out_size);
			ULONG code = pstack->Parameters.DeviceIoControl.IoControlCode;
			switch (code)
			{
				case add_code:
				{
					int a = *(int*)&buffer[0];
					int b = *(int*)&buffer[4];
					*(int*)&buffer[0] = a + b;
					info = 4;
					break;
				}
				case sub_code:
				{
					int a = *(int*)&buffer[0];
					int b = *(int*)&buffer[4];
					*(int*)&buffer[0] = a - b;
					info = 4;
					break;
				}
			}		
			break;
		}
	}
	// return modify
	pIrp->IoStatus.Information = info;
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}
