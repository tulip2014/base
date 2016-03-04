// MaillistSvc.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <windows.h>

#define		MAILLISTNAME	_T("\\\\.\\mailslot\\maillist-{81F9FB33-EF8C-485A-AD93-68D7A914A794}")

void MailWork()
{
	HANDLE hMalilist = CreateMailslot(MAILLISTNAME, 0, MAILSLOT_WAIT_FOREVER, NULL);
	if (INVALID_HANDLE_VALUE == hMalilist)
	{
		std::cout<<"create mail fail"<<std::endl;
		return ;
	}

	TCHAR szRead[MAX_PATH] = {0};
	DWORD dwReadSize = 0;
	BOOL bRead = ReadFile(hMalilist, szRead, MAX_PATH, &dwReadSize, NULL);
	if (bRead)
	{
		std::wcout<<_T("read text:")<<szRead<<std::endl;
	}
	else
	{
		std::wcout<<_T("read fail")<<std::endl;
	}

	CloseHandle(hMalilist);
	return ;
}


int _tmain(int argc, _TCHAR* argv[])
{
	MailWork();

	Sleep(5000);
	return 0;
}

