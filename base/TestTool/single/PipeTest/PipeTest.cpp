// PipeTest.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "PipeTest.h"
#include <iostream>

void CreateTest()
{
	HANDLE hPipe = CreateNamedPipe(_T("\\\\.\\pipe\\TESTPIPE"), 
		PIPE_ACCESS_DUPLEX , 
		FILE_FLAG_OVERLAPPED,
		PIPE_TYPE_BYTE,
		MAX_PATH,
		MAX_PATH,
		30,
		NULL
		);

	HANDLE hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (NULL == hEvent)
	{
		return ;
	}

	OVERLAPPED overlap = {0};
	overlap.hEvent = hEvent;
	BOOL bConnect = ConnectNamedPipe(hPipe, &overlap);

	WaitForSingleObject(hEvent, INFINITE);

	CloseHandle(hEvent);
	std::cout<<"wait end"<<std::endl;

	TCHAR szRead[MAX_PATH] = {0};
	DWORD dwHadRead = 0;
	ReadFile(hPipe, szRead, MAX_PATH, &dwHadRead, NULL);

	std::wcout<<szRead<<std::endl;

}


int  _tmain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: 在此放置代码。
	std::cout<<"start"<<std::endl;
	CreateTest();


	Sleep(300000);
	
	return 0;
}
