// MaillistClient.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <windows.h>

#define		MAILLISTNAME	_T("\\\\.\\mailslot\\maillist-{81F9FB33-EF8C-485A-AD93-68D7A914A794}")

void ClientWork()
{
	HANDLE hMailist = CreateFile(MAILLISTNAME, 
		GENERIC_WRITE, 
		FILE_SHARE_READ, 
		NULL, 
		OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL);

	if (INVALID_HANDLE_VALUE == hMailist)
	{
		std::cout<<"create mail fail"<<std::endl;
		return;
	}

	TCHAR szWrite[] = _T("i'm client!");
	DWORD dwWrite = 0;
	BOOL bWrite = WriteFile(hMailist, szWrite, _tcslen(szWrite), &dwWrite, NULL);
	if (bWrite)
	{
		std::cout<<"wtire success"<<std::endl;
	}
	else
	{
		std::cout<<"wtire fail"<<std::endl;
	}

	if (INVALID_HANDLE_VALUE != hMailist)
	{
		CloseHandle(hMailist);
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	ClientWork();
	return 0;
}

