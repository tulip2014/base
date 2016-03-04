// MessageFormat.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "MessageFormat.h"


void ErrorExit(LPTSTR lpszFunction)   
{   
	// Retrieve the system error message for the last-error code  

	LPVOID lpMsgBuf = NULL; 
	TCHAR szDisplayBuf[MAX_PATH] = {0};  
	DWORD dw = GetLastError();   

	FormatMessage(  
		FORMAT_MESSAGE_ALLOCATE_BUFFER |   
		FORMAT_MESSAGE_FROM_SYSTEM |  
		FORMAT_MESSAGE_IGNORE_INSERTS,  
		NULL,  
		dw,  
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),  
		(LPTSTR) &lpMsgBuf,  
		0, 
		NULL );  

	// Display the error message and exit the process  

	_stprintf_s(szDisplayBuf, MAX_PATH, TEXT("%s failed with error %d: %s"), lpszFunction, dw, lpMsgBuf);   
	MessageBox(NULL, szDisplayBuf, TEXT("Error"), MB_OK);   

	LocalFree(lpMsgBuf);  
	ExitProcess(dw);   
}  


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: 在此放置代码。

	if(!GetProcessId(NULL))  
		ErrorExit(TEXT("GetProcessId")); 
	
	return 0;
}

