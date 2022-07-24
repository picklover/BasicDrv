#include <stdio.h>
#include <windows.h>
#include <winioctl.h>

#define add_code CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define sub_code CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS)
int main()
{
	HANDLE device =  CreateFileW(L"\\\\.\\Mywdm_device_symbollink2", GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	
	/*
	BYTE buffer[24];
	DWORD written;
	memset(buffer, 0, sizeof(buffer));
	for (int i = 0; i < 8; i++)
		buffer[i] = i;
	WriteFile(device, buffer, 8, &written, 0);
	ReadFile(device, buffer, 6, &written, 0);
	printf("%s", buffer);
	*/
	
	int input[2] = { 0x4321,0x1234 };
	ULONG output;
	DWORD retsize;
	DeviceIoControl(device, add_code, &input, 8, &output, 4, &retsize, 0);
	printf("add return size: %d and result: %d\n", retsize, output);

	DeviceIoControl(device, sub_code, &input, 8, &output, 4, &retsize, 0);
	printf("sub return size: %d and result: %d\n", retsize, output);

	CloseHandle(device);
	system("pause");
	return 0;
}