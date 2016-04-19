
#include "PipeCommuUtils.h"
#include "PipeUtils.h"
#include <process.h>
#include <iostream>

PipeCommuUtils::PipeCommuUtils()
{
	m_dwStatus = THREAD_STATUS_RUN;
	m_hInstance = NULL;
}

PipeCommuUtils::~PipeCommuUtils()
{

}

DWORD PipeCommuUtils::GetStatus()
{
	return m_dwStatus;
}

DWORD PipeCommuUtils::SetStatus(DWORD dwStatus)
{
	m_dwStatus = dwStatus;
	return 0;
}

unsigned WINAPI PipeWorkThread(void* lpParam)
{
	std::cout<<"start client thread :"<<GetCurrentThreadId()<<std::endl;
	PipeUtils* hPipeInstance = (PipeUtils*)lpParam;

	char szReceive[MAX_PATH] = {0};
	DWORD dwSize = MAX_PATH;
	hPipeInstance->Receive(szReceive, &dwSize, 100000);
	std::cout<<GetCurrentThreadId()<<", receive:"<<szReceive<<std::endl;

	char szSend[MAX_PATH] = {0};
	dwSize = MAX_PATH;
	sprintf_s(szSend, MAX_PATH, "%d ***server send id:%d", GetCurrentThreadId(), GetCurrentThreadId());
	hPipeInstance->Send(szSend, &dwSize, 100000);
	std::cout<<szSend<<std::endl;

	//这里需要断开连接
	hPipeInstance->Close();
	return 0;
}

unsigned WINAPI PipeServerThread(void* lpParam)
{
	PipeCommuUtils* pipeUtils = (PipeCommuUtils*)lpParam;
	std::cout<<"start server thread"<<std::endl;
	while (1)
	{
		DWORD dwStatus = pipeUtils->GetStatus();
		if (THREAD_STATUS_PAUSE == dwStatus)
		{
			Sleep(1000);
			continue;
		}

		if (THREAD_STATUS_STOP == dwStatus)
		{
			break;
		}

		PipeUtils* pipeServer = new PipeUtils();
		while (1)
		{
			HANDLE hTempHandle = pipeServer->CreateNewPipe(PIPESERVERNAME);
			if (hTempHandle != INVALID_HANDLE_VALUE)
			{
				break;
			}

			WaitNamedPipe(PIPESERVERNAME, 100);
			Sleep(100);
		}

		DWORD dwRet = pipeServer->WaitforConnect();
		if (dwRet != 0)
		{
			delete pipeServer;
			Sleep(100);
			continue;
		}
		
		UINT nThreadID = 0;
		HANDLE hThread = (HANDLE)_beginthreadex( NULL, 0, PipeWorkThread, (void*)pipeServer, 0, &nThreadID );

		CloseHandle(hThread); 
	}

	delete pipeUtils;
	return 0;
}

DWORD PipeCommuUtils::StartPipeServer( LPCWSTR lpPipeName )
{
	UINT nThreadID = 0;
	m_hInstance = (HANDLE)_beginthreadex( NULL, 0, PipeServerThread, this, 0, &nThreadID );

	if ( NULL != m_hInstance )
	{
		return S_OK;
	}

	return E_FAIL;
}

DWORD PipeCommuUtils::StopPipeServer()
{
	SetStatus(THREAD_STATUS_STOP);
	CloseHandle(m_hInstance);
	m_hInstance = NULL;
	return 0;
}

DWORD PipeCommuUtils::StartClient( LPCWSTR lpPipeName )
{
	std::cout<<"client thread :"<<GetCurrentThreadId()<<std::endl;
	PipeUtils pipeClient;
	DWORD dwRet = pipeClient.Connect(lpPipeName);
	if (dwRet != 0)
	{
		return 1;
	}

	//Sleep(50000);
	//std::cout<<"start write"<<std::endl;

	char szSend[MAX_PATH] = {0};
	DWORD dwSize = MAX_PATH;
	sprintf_s(szSend, MAX_PATH, "%d ### id: %d, client send:%d", GetCurrentThreadId(), GetCurrentThreadId(), &pipeClient);
	pipeClient.Send(szSend, &dwSize, 100000);
	std::cout<<szSend<<std::endl;

	char szReceive[MAX_PATH] = {0};
	dwSize = MAX_PATH;
	pipeClient.Receive(szReceive, &dwSize, 100000);
	std::cout<<GetCurrentThreadId()<<", receive: "<<szReceive<<std::endl;

	//这里需要断开连接
	//pipeClient.Close();

	return 0;
}




