
/********************************************************************
	created:	2016/03/14
	created:	14:3:2016   17:05
	filename: 	base\baselib\basehelper\utils\FileUtils.cpp
	file path:	base\baselib\basehelper\utils
	file base:	FileUtils
	file ext:	cpp
	author:		murisly
	
	purpose:	file utils
*********************************************************************/

#include "FileUtils.h"


BOOL WriteDataFileW(PVOID pBuf, DWORD dwLength, const wchar_t* const ptcPath)
{
	SetFileAttributesW(ptcPath, FILE_ATTRIBUTE_NORMAL);

	HANDLE hFile=CreateFileW(ptcPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile==INVALID_HANDLE_VALUE)
	{
		DWORD dwError=GetLastError();
		return FALSE;
	}

	DWORD dwRet;
	BOOL bRet=::WriteFile(hFile, (PBYTE)pBuf, dwLength, &dwRet, NULL);
	if(bRet)
		FlushFileBuffers(hFile);
	else
	{
		DWORD dwError=GetLastError();
	}

	CloseHandle(hFile);

	return bRet;
}

BOOL ReadDataFileW(PVOID *ppBuf, DWORD *pLength, const wchar_t* const ptcPath, DWORD dwExtendLength)
{
	HANDLE hFile=::CreateFileW(ptcPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile==INVALID_HANDLE_VALUE)
	{
		DWORD dwError=GetLastError();

		return FALSE;
	}

	DWORD dwLength=::GetFileSize(hFile, NULL);
	if(dwLength<=0)
	{
		CloseHandle(hFile);
		return FALSE;
	}

	BYTE *pBuf = new BYTE[dwLength + dwExtendLength];
	if(pBuf==NULL)
	{
		CloseHandle(hFile);
		return FALSE;
	}

	DWORD dwRead=0;
	DWORD dwRet;

	while(dwRead < dwLength)
	{
		if(!::ReadFile(hFile, pBuf + dwRead, dwLength - dwRead, &dwRet, NULL))
			break;
		if(dwRet>=0)
			dwRead += dwRet;
	}
	CloseHandle(hFile);

	if(dwRead < dwLength)
	{
		delete[] pBuf;
		return FALSE;
	}

	*ppBuf = pBuf;
	*pLength = dwLength;

	return TRUE;
}

BOOL WriteDataFileA(PVOID pBuf, DWORD dwLength, const char* const ptcPath)
{
	SetFileAttributesA(ptcPath, FILE_ATTRIBUTE_NORMAL);

	HANDLE hFile=CreateFileA(ptcPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		DWORD dwError = GetLastError();
		return FALSE;
	}

	DWORD dwRet;
	BOOL bRet=::WriteFile(hFile, (PBYTE)pBuf, dwLength, &dwRet, NULL);
	if(bRet)
		FlushFileBuffers(hFile);
	else
	{
		DWORD dwError = GetLastError();
	}

	CloseHandle(hFile);

	return bRet;
}
BOOL ReadDataFileA(PVOID *ppBuf, DWORD *pLength, const char* const ptcPath, DWORD dwExtendLength)
{
	HANDLE hFile = ::CreateFileA(ptcPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		DWORD dwError = GetLastError();

		return FALSE;
	}

	DWORD dwLength = ::GetFileSize(hFile, NULL);
	if(dwLength <= 0)
	{
		CloseHandle(hFile);
		return FALSE;
	}

	BYTE *pBuf = new BYTE[dwLength + dwExtendLength];
	if(pBuf == NULL)
	{
		CloseHandle(hFile);
		return FALSE;
	}

	DWORD dwRead = 0;
	DWORD dwRet;

	while(dwRead < dwLength)
	{
		if(!::ReadFile(hFile, pBuf + dwRead, dwLength - dwRead, &dwRet, NULL))
			break;
		if(dwRet >= 0)
			dwRead += dwRet;
	}
	CloseHandle(hFile);

	if(dwRead < dwLength)
	{
		delete[] pBuf;
		return FALSE;
	}

	*ppBuf = pBuf;
	*pLength = dwLength;

	return TRUE;
}