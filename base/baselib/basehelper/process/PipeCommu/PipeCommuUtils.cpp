
#include "..\..\stdafx.h"
#include "PipeCommuUtils.h"
#include "PipeUtils.h"
#include <process.h>

PipeCommuUtils::PipeCommuUtils()
{
	dwStatus = 0;
	hInstance = 0;
}

PipeCommuUtils::~PipeCommuUtils()
{

}

unsigned WINAPI PipeServerThread(void* lpParam)
{

	while (1)
	{
#define  PIPENAME	L"\\\\.\\pipe\\server_{2F39AD4F-25C8-42C7-A66E-06DEEBC0E0B1}"

		PipeUtils* pipeServer = new PipeUtils();

		while (1)
		{
			HANDLE hTempHandle = pipeServer->CreateNewPipe(PIPENAME);
			if (hTempHandle != NULL)
			{
				break;
			}
		}
		

		
		pipeServer->WaitforConnect();

		pipeServer->Connect(PIPENAME);

	}
}

DWORD PipeCommuUtils::StartPipeServer( LPCWSTR lpPipeName )
{
	UINT nThreadID = 0;
	hInstance = (HANDLE)_beginthreadex( NULL, 0, PipeServerThread, this, 0, &nThreadID );

	if ( NULL != hInstance )
	{
		return S_OK;
	}

	return E_FAIL;
}

DWORD PipeCommuUtils::StopPipeServer()
{
	dwStatus = THREAD_STATUS_STOP;
}


