
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

#define		THREAD_STATUS_STOP		2

typedef struct _PIPECOMMUINFO 
{
	DWORD dwStatus;
	HANDLE hThreadHandle;
}PIPECOMMUINFO;

class PipeCommuUtils
{
private:
	DWORD dwStatus;		//µ±Ç°×´Ì¬
	HANDLE hInstance;   //pipe instance

public:
	PipeCommuUtils();
	~PipeCommuUtils();
	DWORD StartPipeServer(LPCWSTR lpPipeName);
	DWORD StopPipeServer();

};