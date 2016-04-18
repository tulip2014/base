/********************************************************************
	created:	2016/04/18
	created:	18:4:2016   11:02
	filename: 	base\baselib\basehelper\PipeUtils.h
	file path:	base\baselib\basehelper
	file base:	PipeUtils
	file ext:	h
	author:		Author
	
	purpose:	to help process commucation
*********************************************************************/

#pragma once

#define		PIPENAMESIZE	100
#define		PIPENAME		L"\\\\.\\pipe\\pipeutils_{D5C192C6-FD51-4EC6-B8F3-F06F34EE4000}"

class PipeUtils
{
private:
	DWORD m_dwSendBufferLength;
	DWORD m_dwReceiveBufferLength;
	TCHAR m_szPipeName[MAX_PATH];
public:
	PipeUtils();
	~PipeUtils();
	int close();
	HANDLE CreateNewPipe(LPCWSTR lpName);
};