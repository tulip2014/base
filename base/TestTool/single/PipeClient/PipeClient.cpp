// PipeClient.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "PipeClient.h"
#include <iostream>

using namespace std;

#define		BUFSIZE			255
#define		PIPENAME		_T("\\\\.\\pipe\\mynamedpipe")

void Client()
{
	HANDLE hPipe; 
	LPTSTR lpvMessage=TEXT("Default message from client."); 
	TCHAR  chBuf[BUFSIZE]; 
	BOOL   fSuccess = FALSE; 
	DWORD  cbRead, cbToWrite, cbWritten, dwMode; 
	LPTSTR lpszPipename = TEXT("\\\\.\\pipe\\mynamedpipe"); 

	// Try to open a named pipe; wait for it, if necessary. 

	while (1) 
	{ 
		hPipe = CreateFile( 
			lpszPipename,   // pipe name 
			GENERIC_READ |  // read and write access 
			GENERIC_WRITE, 
			0,              // no sharing 
			NULL,           // default security attributes
			OPEN_EXISTING,  // opens existing pipe 
			0,              // default attributes 
			NULL);          // no template file 

		// Break if the pipe handle is valid. 

		if (hPipe != INVALID_HANDLE_VALUE) 
			break; 

		// Exit if an error other than ERROR_PIPE_BUSY occurs. 

		if (GetLastError() != ERROR_PIPE_BUSY) 
		{
			_tprintf( TEXT("Could not open pipe. GLE=%d\n"), GetLastError() ); 
			return ;
		}

		// All pipe instances are busy, so wait for 20 seconds. 

		if ( ! WaitNamedPipe(lpszPipename, 20000)) 
		{ 
			printf("Could not open pipe: 20 second wait timed out."); 
			return ;
		} 
	} 

	// The pipe connected; change to message-read mode. 

	dwMode = PIPE_READMODE_MESSAGE; 
	fSuccess = SetNamedPipeHandleState( 
		hPipe,    // pipe handle 
		&dwMode,  // new pipe mode 
		NULL,     // don't set maximum bytes 
		NULL);    // don't set maximum time 
	if ( ! fSuccess) 
	{
		_tprintf( TEXT("SetNamedPipeHandleState failed. GLE=%d\n"), GetLastError() ); 
		return ;
	}

	// Send a message to the pipe server. 

	cbToWrite = (lstrlen(lpvMessage)+1)*sizeof(TCHAR);
	_tprintf( TEXT("Sending %d byte message: \"%s\"\n"), cbToWrite, lpvMessage); 

	fSuccess = WriteFile( 
		hPipe,                  // pipe handle 
		lpvMessage,             // message 
		cbToWrite,              // message length 
		&cbWritten,             // bytes written 
		NULL);                  // not overlapped 

	if ( ! fSuccess) 
	{
		_tprintf( TEXT("WriteFile to pipe failed. GLE=%d\n"), GetLastError() ); 
		return ;
	}

	printf("\nMessage sent to server, receiving reply as follows:\n");

	do 
	{ 
		// Read from the pipe. 

		fSuccess = ReadFile( 
			hPipe,    // pipe handle 
			chBuf,    // buffer to receive reply 
			BUFSIZE*sizeof(TCHAR),  // size of buffer 
			&cbRead,  // number of bytes read 
			NULL);    // not overlapped 

		if ( ! fSuccess && GetLastError() != ERROR_MORE_DATA )
			break; 

		_tprintf( TEXT("\"%s\"\n"), chBuf ); 
	} while ( ! fSuccess);  // repeat loop if ERROR_MORE_DATA 

	if ( ! fSuccess)
	{
		_tprintf( TEXT("ReadFile from pipe failed. GLE=%d\n"), GetLastError() );
		return ;
	}

	printf("\n<End of message, press ENTER to terminate connection and exit>");

	CloseHandle(hPipe); 
}

void Connect(int iNum)
{
	HANDLE hNamePipe = NULL;
	while (1)
	{
		BOOL bExist = WaitNamedPipe(PIPENAME, NMPWAIT_WAIT_FOREVER);
		if (!bExist)
		{
			std::cout<<"fail1: "<<GetLastError()<<std::endl;
			//continue;
		}

		hNamePipe = CreateFile(PIPENAME, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);

		if (NULL == hNamePipe || hNamePipe == INVALID_HANDLE_VALUE)
		{
			std::cout<<"fail2:"<<GetLastError()<<std::endl;
			continue ;
		}

		break;
	}


	int i = 0;
	//while (++i < 100)
	{
		DWORD dwThreadId = GetCurrentThreadId();
		char szWrite[MAX_PATH] = {0};
		sprintf_s(szWrite, MAX_PATH, "threadid:%d, value:%d", dwThreadId, iNum);
		DWORD dwHasWrite = 0;
		bool bRet = WriteFile(hNamePipe, szWrite, strlen(szWrite) + 1, &dwHasWrite, NULL);
		std::cout<<"write value "<<i<<":, result:"<<bRet<<", "<<szWrite<<std::endl;
	}
	std::cout<<"write end"<<std::endl;

	OVERLAPPED overlap = {0};
	overlap.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	char cReceive[MAX_PATH] = {0};
	DWORD dwRead = 0;
	ReadFile(hNamePipe, cReceive, MAX_PATH, &dwRead, &overlap);

	//WaitForSingleObject(overlap.hEvent, 5000000);
	cout<<"receive:"<<cReceive<<endl;

// 	Sleep(2000);
// 	char szRead[MAX_PATH] = {0};
// 	DWORD dwHadRead = 0;
// 	ReadFile(hNamePipe, szRead, MAX_PATH, &dwHadRead, NULL);
// 	std::cout<<"read value:"<<szRead<<std::endl;
// 	std::cout<<"read end"<<std::endl;

	CloseHandle(hNamePipe);
}


int  _tmain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: 在此放置代码。
	DWORD dwThreadId = GetCurrentThreadId();
	int i = 0;
	while (++i < 50)
	{
		Connect(i);
		Sleep(1000);
	}
	

	system("pause");
	return 0;
}
