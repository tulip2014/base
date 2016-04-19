
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


class PipeCommuUtils
{
private:
	DWORD m_dwStatus;	  //serverµ±Ç°×´Ì¬
	HANDLE m_hInstance;   //pipe instance

public:
	PipeCommuUtils();
	~PipeCommuUtils();
	DWORD GetStatus();
	DWORD SetStatus(DWORD dwStatus);
	DWORD StartPipeServer(LPCWSTR lpPipeName);
	DWORD StopPipeServer();

	DWORD StartClient(LPCWSTR lpPipeName);
};