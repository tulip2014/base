// ShareMemClient.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "ShareMemClient.h"

#define BUF_SIZE 256
TCHAR szName[] = TEXT("Global\\MyFileMappingObject");    //ָ��ͬһ�鹲���ڴ������


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
		_tprintf(TEXT("Could not create file mapping object (%d).\n"), GetLastError());
		return 1;
	}
	pBuf = (LPTSTR) MapViewOfFile(hMapFile,   // handle to map object
		FILE_MAP_ALL_ACCESS, // read/write permission
		0,
		0,
		BUF_SIZE);

	if (pBuf == NULL)
	{
		_tprintf(TEXT("Could not map view of file (%d).\n"),
			GetLastError());

		CloseHandle(hMapFile);

		return 1;
	}
	//��main��ʼ���ˣ�A B process����һ�������ǻ�ȡ��Ϊ"Global\\MyFileMappingObject"�Ĺ����ڴ��ָ��

	//���´��룬A��ͣ�ض������ڴ�pBuf
	while(1)
	{
		
		std::cin>>
	}

	UnmapViewOfFile(pBuf);
	CloseHandle(hMapFile);
	return 0;
	

	return 0;
}
