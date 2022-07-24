#include <stdio.h>
#include <windows.h>
#include <winsvc.h>

#define DrvName L"Drv"
#define DrvRegPath L"C:\\Users\\52pojie\\Desktop\\Drv.sys"
int main()
{
	BOOL bRet = FALSE;
	SC_HANDLE hServiceMgr = NULL;
	SC_HANDLE hServiceDriver = NULL;
	SERVICE_STATUS SvrSta;
	hServiceMgr = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!hServiceMgr)
	{
		printf("Open SCManager failed! ErrCode: %d\n", GetLastError());

	}
	hServiceDriver = OpenService(hServiceMgr, DrvName, SERVICE_ALL_ACCESS);
	if (!hServiceDriver)
	{
		printf("Open Service failed!\n");
	}
	bRet = ControlService(hServiceDriver,SERVICE_CONTROL_STOP,&SvrSta);
	if (!bRet)
	{
		printf("Control Service (stop) failed!\n");
	}
	bRet =  DeleteService(hServiceDriver);
	if (!bRet)
	{
		printf("Delete Service failed!\n");
	}
	CloseServiceHandle(hServiceMgr);
	CloseServiceHandle(hServiceDriver);
	return 0;
}