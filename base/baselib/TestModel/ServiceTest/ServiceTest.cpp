// ServiceTest.cpp : 定义应用程序的入口点。
//
/********************************************************************
	created:	2016/03/09
	created:	9:3:2016   17:58
	filename: 	base\baselib\TestModel\ServiceTest\ServiceTest.cpp
	file path:	base\baselib\TestModel\ServiceTest
	file base:	ServiceTest
	file ext:	cpp
	author:		murisly
	
	purpose:	to test win32 sercive function
*********************************************************************/

#include "stdafx.h"
#include <stdio.h>
#include "ServiceTest.h"


#define SERVICE_NAME	L"srv_test"  

SERVICE_STATUS ServiceStatus;  
SERVICE_STATUS_HANDLE hServiceStatusHandle;  
void WINAPI service_main(int argc, char** argv);   
void WINAPI ServiceHandler(DWORD fdwControl);  


TCHAR szSvcName[80] = {0};
SC_HANDLE schSCManager;
SC_HANDLE schService;
int uaquit;  
FILE* log;  


DWORD WINAPI srv_core_thread(LPVOID para)  
{     
	int i = 0;  
	for(;;)
	{  
		if(uaquit)  
		{  
			break;   
		}  
		Sleep(5000);   
	}      
	return NULL;   
}     
  
  
void WINAPI ServiceHandler(DWORD fdwControl)  
{  
	switch(fdwControl)   
	{  
	case SERVICE_CONTROL_STOP:  
	case SERVICE_CONTROL_SHUTDOWN:  
		ServiceStatus.dwWin32ExitCode = 0;   
		ServiceStatus.dwCurrentState  = SERVICE_STOPPED;   
		ServiceStatus.dwCheckPoint    = 0;   
		ServiceStatus.dwWaitHint      = 0;  
		uaquit= 1;  
		//add you quit code here  
		if(log != NULL)  
			fclose(log);  
		break;   
	default:  
		return;   
	};

	if (!SetServiceStatus(hServiceStatusHandle,  &ServiceStatus))   
	{   
		DWORD nError = GetLastError();  
	}   
}  
  
    
void WINAPI service_main(int argc, char** argv)   
{         
    ServiceStatus.dwServiceType        = SERVICE_WIN32;   
    ServiceStatus.dwCurrentState       = SERVICE_START_PENDING;   
    ServiceStatus.dwControlsAccepted   = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_PAUSE_CONTINUE;   
    ServiceStatus.dwWin32ExitCode      = 0;   
    ServiceStatus.dwServiceSpecificExitCode = 0;   
    ServiceStatus.dwCheckPoint         = 0;   
    ServiceStatus.dwWaitHint           = 0;

    hServiceStatusHandle = RegisterServiceCtrlHandler((SERVICE_NAME), ServiceHandler);   
    if (hServiceStatusHandle == 0)   
    {  
        DWORD nError = GetLastError();  
    }

    //add your init code here  
    _tfopen_s(&log, L"d:\\test.txt", L"w");

    //add your service thread here  
    HANDLE task_handle = CreateThread(NULL,NULL,srv_core_thread,NULL,NULL,NULL);  
    if(task_handle == NULL)  
    {  
        _ftprintf(log, L"create srv_core_thread failed\n");  
    }  
      
    // Initialization complete - report running status   
    ServiceStatus.dwCurrentState       = SERVICE_RUNNING;   
    ServiceStatus.dwCheckPoint         = 0;   
    ServiceStatus.dwWaitHint           = 9000;
    if(!SetServiceStatus(hServiceStatusHandle, &ServiceStatus))   
    {   
        DWORD nError = GetLastError();  
    }   
   
}


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
					   _In_opt_ HINSTANCE hPrevInstance,
					   _In_ LPTSTR    lpCmdLine,
					   _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	SERVICE_TABLE_ENTRY ServiceTable[2];  

	ServiceTable[0].lpServiceName = (SERVICE_NAME);  
	ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)service_main;  

	ServiceTable[1].lpServiceName = NULL;  
	ServiceTable[1].lpServiceProc = NULL;

	// start dispatch thread  
	StartServiceCtrlDispatcher(ServiceTable);   

	return 0;
}
