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
#define		PIPEWAITTIME	10000		//默认等待时间10s

class PipeUtils
{
private:
	DWORD m_dwSendBufferLength;
	DWORD m_dwReceiveBufferLength;
	DWORD m_dwWaitTime;	  //等待时间
	HANDLE m_hInstance;   //server端表示创建的管道，client端表示打开的文件
	TCHAR m_szPipeName[MAX_PATH];  //管道名
	OVERLAPPED m_Overlap; //

private:
	DWORD ResetOverlapped();
	DWORD ClearOverlapped();
	DWORD CreateOverlapped();

public:
	PipeUtils();
	~PipeUtils();
	DWORD Close();
	HANDLE CreateNewPipe(LPCWSTR lpName);
	DWORD WaitforConnect();
	DWORD Connect(LPCTSTR lpName);
	DWORD Receive(void* lpBuffer, LPDWORD lpSize, DWORD dwWaitTime = PIPEWAITTIME);
	DWORD Send(void* lpBuffer, LPDWORD lpSize, DWORD dwWaitTime = PIPEWAITTIME);
};