
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

		pipeServer->CreateNewPipe(PIPENAME);
		
		pipeServer->WaitforConnect();

		pipeServer->Connect(PIPENAME);

	}
}

DWORD PipeCommuUtils::StartPipeServer( LPCWSTR lpPipeName )
{
	UINT nThreadID = 0;
	m_hRequestThread = (HANDLE)_beginthreadex(NULL, 0, PipeServerThread, this, 0, &nThreadID);

	if (NULL != m_hRequestThread)
	{
		m_bStarted = TRUE;

		return S_OK;
	}

	return E_FAIL;
	return 0;
}

DWORD PipeCommuUtils::StopPipeServer()
{

}


