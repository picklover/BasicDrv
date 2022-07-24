#include <stdio.h>
#include <windows.h>
#include <winsvc.h>

#define DrvName L"Drv"
#define DrvRegPath L"C:\\Users\\52pojie\\Desktop\\Drv.sys"
int main()
{
	int errCode;
	BOOL bRet = FALSE;
	SC_HANDLE hServiceMgr = NULL;
	SC_HANDLE hServiceDriver = NULL;
	hServiceMgr = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!hServiceMgr)
	{
		errCode = GetLastError();
		printf("Open SCManager failed! ErrCode: %d\n",GetLastError());

	}
	hServiceDriver = CreateService(hServiceMgr, DrvName, DrvName, SERVICE_ALL_ACCESS, SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START, SERVICE_ERROR_IGNORE, DrvRegPath, NULL, NULL, NULL, NULL, NULL);
	if (!hServiceDriver)
	{
		errCode = GetLastError();
		printf("Create Service failed! ErrCode: %d\n", errCode);
		if (errCode == ERROR_SERVICE_EXISTS)
		{
			printf("Service has been created!\n");
			hServiceDriver = OpenService(hServiceMgr, DrvName, SERVICE_ALL_ACCESS);
			if (!hServiceDriver)
			{
				printf("Open Service failed!\n");
			}
		}
	}
	bRet = StartService(hServiceDriver, NULL, NULL);
	if (!bRet)
	{
		printf("Start Service failed!\n");
	}
	CloseServiceHandle(hServiceMgr);
	CloseServiceHandle(hServiceDriver);
	return 0;
}