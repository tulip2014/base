
/********************************************************************
	created:	2016/04/18
	created:	18:4:2016   14:45
	filename: 	baselib\basehelper\process\PipeCommu\PipeCommuUtils.h
	file path:	baselib\basehelper\process\PipeCommu
	file base:	PipeCommuUtils
	file ext:	h
	author:		Author
	
	purpose:	to built pipe communication server and client
*********************************************************************/

#pragma once

#include "PipeUtils.h"
#include <windows.h>

#define		THREAD_STATUS_RUN		0
#define		THREAD_STATUS_PAUSE		1
#define		THREAD_STATUS_STOP		2

#define		PIPESERVERNAME			L"\\\\.\\pipe\\server_{2F39AD4F-25C8-42C7-A66E-06DEEBC0E0B1}"

typedef DWORD (*PipeReadFunc)(void* lpBuffer, LPDWORD dwSize);
typedef DWORD (*PipeWriteFunc)(void** lpBuffer, LPDWORD dwSize);
typedef DWORD (*PipeReadAndWrite)(void* lpReadBuffer, LPDWORD dwReadSize, void** lpWriterBuffer, LPDWORD dwWriteSize);

class PipeCommuUtils;

typedef struct _CALLBACKFUNC
{
	PipeReadFunc readFunc;
	PipeWriteFunc writeFunc;
}CALLBACKFUNC, *PCALLBACKFUNC;

typedef struct _INSTANCEINFO
{
	PipeUtils* pipeUtils;
	PipeCommuUtils* pipeCommu;
}INSTANCEINFO, *PINSTANCEINFO;

class PipeCommuUtils
{
private:
	DWORD  m_dwStatus;	  //server当前状态
	HANDLE m_hInstance;   //pipe instance
	CALLBACKFUNC m_CallbackFunc;  //设置的回调函数
	HANDLE m_Handle[2];

public:
	PipeCommuUtils();
	~PipeCommuUtils();
	DWORD GetStatus();
	DWORD SetStatus(DWORD dwStatus);
	DWORD SetCallbackFunc(CALLBACKFUNC lpCallbackFunc);
	PCALLBACKFUNC GetCallbackFunc();

	DWORD StartPipeServer(LPCWSTR lpPipeName);
	DWORD StopPipeServer();

	DWORD StartClient(LPCWSTR lpPipeName);

public:
	static unsigned WINAPI PipeServerThread(void* lpParam);
	static unsigned WINAPI PipeWorkThread(void* lpParam);
};