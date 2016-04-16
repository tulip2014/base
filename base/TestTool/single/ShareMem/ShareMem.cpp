// ShareMem.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "ShareMem.h"

#define BUF_SIZE 256
TCHAR szName[] = TEXT("Global\\MyFileMappingObject");    //指向同一块共享内存的名字

int main()
{
	HANDLE hMapFile;
	LPCTSTR pBuf;

	hMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,    // use paging file
		NULL,                    // default security
		PAGE_READWRITE,          // read/write access
		0,                       // maximum object size (high-order DWORD)
		BUF_SIZE,                // maximum object size (low-order DWORD)
		szName);                 // name of mapping object

	if (hMapFile == NULL)
	{
		//OutputDebugString(TEXT("Could not create file mapping object.\n"));
		return 1;
	}

	pBuf = (LPTSTR) MapViewOfFile(hMapFile,   // handle to map object
		FILE_MAP_ALL_ACCESS, // read/write permission
		0,
		0,
		BUF_SIZE);

	if (pBuf == NULL)
	{
		OutputDebugString(TEXT("Could not map view of file ().\n"));
		CloseHandle(hMapFile);
		return 1;
	}

	//从main开始至此，A B process代码一样，都是获取名为"Global\\MyFileMappingObject"的共享内存的指针

	//以下代码，B不停写共享内存pBuf
	while(1)
	{
		TCHAR s[BUF_SIZE];
		//cout<<"B process: plz input sth. to be transfered to A process."<<endl;
		//cin>>s;
		memcpy((PVOID)pBuf, s, BUF_SIZE);
	}
	
	return 0;
}
