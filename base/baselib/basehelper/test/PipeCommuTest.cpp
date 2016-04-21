
#include "..\stdafx.h"
#include "PipeCommuTest.h"
#include "..\process\PipeCommu\PipeCommuUtils.h"
#include <iostream>

using namespace std;

DWORD ReadFunc(void* lpReadBuffer, LPDWORD dwReadSize)
{
	cout<<(char*)lpReadBuffer<<endl;

	return 0;
}

DWORD WriteFunc(void** lpWriteBuffer, LPDWORD dwWriteSize)
{
	char sSend[MAX_PATH] = {0};
	sprintf_s(sSend, MAX_PATH, "server send id : %d", GetCurrentThreadId());
	*dwWriteSize = strlen(sSend) + 1;
	void* lpBuf = new char[*dwWriteSize];
	memcpy(lpBuf, sSend, *dwWriteSize);

	*lpWriteBuffer = lpBuf;
	cout<<sSend<<endl;

	return 0;
}

void PipeServerTest()
{
	PipeCommuUtils* test = new PipeCommuUtils();
	CALLBACKFUNC tmp = {0};
	tmp.readFunc = ReadFunc;
	tmp.writeFunc = WriteFunc;
	test->SetCallbackFunc(tmp);
	test->StartPipeServer(PIPESERVERNAME);
}

void PipeClientTest()
{
	PipeCommuUtils client;
	CALLBACKFUNC tmp = {0};
	tmp.readFunc = ReadFunc;
	tmp.writeFunc = WriteFunc;
	client.SetCallbackFunc(tmp);
	client.StartClient(PIPESERVERNAME);
}
