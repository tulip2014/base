// PipeClient.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "PipeClient.h"
#include <iostream>

#define		PIPENAME		_T("\\\\.\\pipe\\TESTPIPE")

void Connect()
{
	BOOL bExist = WaitNamedPipe(PIPENAME, 3);
	if (!bExist)
	{
		return ;
	}

	HANDLE hNamePipe = CreateFile(PIPENAME, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (NULL == hNamePipe)
	{
		return ;
	}

	TCHAR szWrite[] = _T("start task");
	DWORD dwHasWrite = 0;
	WriteFile(hNamePipe, szWrite, _tcslen(szWrite), &dwHasWrite, NULL);

	std::cout<<"end"<<std::endl;
}


int  _tmain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: 在此放置代码。

	Connect();

	return 0;
}
