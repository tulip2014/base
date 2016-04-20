
#include "PipeCommuUtils.h"
#include "PipeUtils.h"
#include <process.h>
#include <iostream>


PipeCommuUtils::PipeCommuUtils()
{
	m_dwStatus = THREAD_STATUS_RUN;
	m_hInstance = NULL;
	m_Handle[0] = 0;
	m_Handle[1] = CreateEvent(NULL, TRUE, FALSE, NULL);
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

DWORD PipeCommuUtils::SetCallbackFunc( PipeReadAndWrite lpCallbackFunc )
{
	m_CallbackFunc = lpCallbackFunc;
	return 0;
}

PipeReadAndWrite PipeCommuUtils::GetCallbackFunc()
{
	return m_CallbackFunc;
}

unsigned PipeCommuUtils::PipeWorkThread(void* lpParam)
{
	//std::cout<<"start client thread :"<<GetCurrentThreadId()<<std::endl;
	PINSTANCEINFO hInstance = (PINSTANCEINFO)lpParam;
	PipeCommuUtils * commuUtils = hInstance->pipeCommu;

	char szReceive[MAX_PATH] = {0};
	DWORD dwSize = MAX_PATH;
	DWORD dwRet = hInstance->pipeUtils->Receive(szReceive, &dwSize, 100000);
	if (dwRet != 0)
	{
		std::cout<<"receive fail"<<std::endl;
	}

	void* lpSendBuffer = NULL;
	DWORD dwSendSize = 0;
	commuUtils->GetCallbackFunc()(szReceive, &dwSize, &lpSendBuffer, &dwSendSize);

	char* lpSend = NULL;
	if (lpSendBuffer == NULL || dwSendSize == 0)
	{
		lpSend = new char(0);
		dwSendSize = 1;
	}

	char szSend[MAX_PATH] = {0};
	dwSize = MAX_PATH;
	sprintf_s(szSend, MAX_PATH, "%d ***server send id:%d", GetCurrentThreadId(), GetCurrentThreadId());
	dwRet = hInstance->pipeUtils->Send(szSend, &dwSize, 100000);
	if (dwRet != 0)
	{
		std::cout<<"Send fail"<<std::endl;
	}
	//std::cout<<szSend<<std::endl;

	//这里需要断开连接
	hInstance->pipeUtils->Close();
	return 0;
}

unsigned PipeCommuUtils::PipeServerThread(void* lpParam)
{
	PipeCommuUtils* pipeCommuUtils = (PipeCommuUtils*)lpParam;
	//std::cout<<"start server thread"<<std::endl;
	while (1)
	{
		DWORD dwStatus = pipeCommuUtils->GetStatus();
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
		INSTANCEINFO instanceInfo = {0};
		instanceInfo.pipeUtils = pipeServer;
		instanceInfo.pipeCommu = pipeCommuUtils;
		HANDLE hThread = (HANDLE)_beginthreadex( NULL, 0, PipeWorkThread, (void*)&instanceInfo, 0, &nThreadID );

		CloseHandle(hThread); 
	}

	delete pipeCommuUtils;
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
		std::cout<<"Connect fail"<<std::endl;
		return 1;
	}

	Sleep(100);
	//std::cout<<"start write"<<std::endl;

	char szSend[MAX_PATH] = {0};
	DWORD dwSize = MAX_PATH;
	sprintf_s(szSend, MAX_PATH, "%d ### id: %d, client send:%d", GetCurrentThreadId(), GetCurrentThreadId(), &pipeClient);
	dwRet = pipeClient.Send(szSend, &dwSize, 100000);
	if (dwRet != 0)
	{
		std::cout<<"Send fail"<<std::endl;
	}
	//std::cout<<szSend<<std::endl;

	char szReceive[MAX_PATH] = {0};
	dwSize = MAX_PATH;
	dwRet = pipeClient.Receive(szReceive, &dwSize, 100000);
	if (dwRet != 0)
	{
		std::cout<<"Send fail"<<std::endl;
	}
	//std::cout<<GetCurrentThreadId()<<", receive: "<<szReceive<<std::endl;

	//这里需要断开连接
	//pipeClient.Close();

	return 0;
}




