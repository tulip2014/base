// PipeTest.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "PipeTest.h"
#include <iostream>
#include <Strsafe.h>
#include "PipeCommuUtils.h"

using namespace std;

#define		BUFSIZE		255

VOID GetAnswerToRequest( LPTSTR pchRequest, LPTSTR pchReply, LPDWORD pchBytes );

DWORD WINAPI InstanceThread(LPVOID lpvParam)
	// This routine is a thread processing function to read from and reply to a client
	// via the open pipe connection passed from the main loop. Note this allows
	// the main loop to continue executing, potentially creating more threads of
	// of this procedure to run concurrently, depending on the number of incoming
	// client connections.
{ 
	char pchRequest[MAX_PATH] = {0};
	char pchReply[MAX_PATH]   = "123qweads";

	DWORD cbBytesRead = MAX_PATH, cbReplyBytes = MAX_PATH, cbWritten = 0; 
	BOOL fSuccess = FALSE;
	HANDLE hPipe  = NULL;

	// Do some extra error checking since the app will keep running even if this
	// thread fails.

	if (lpvParam == NULL)
	{
		printf( "   InstanceThread got an unexpected NULL value in lpvParam.\n");
		return (DWORD)-1;
	}

	// Print verbose messages. In production code, this should be for debugging only.
	printf("InstanceThread created, receiving and processing messages.\n");

	// The thread's parameter is a handle to a pipe object instance. 

	hPipe = (HANDLE) lpvParam; 

	// Loop until done reading
	//while (1) 
	{ 
		// Read client requests from the pipe. This simplistic code only allows messages
		// up to BUFSIZE characters in length.
		fSuccess = ReadFile( 
			hPipe,        // handle to pipe 
			pchRequest,   // buffer to receive data 
			MAX_PATH,	  // size of buffer 
			&cbBytesRead, // number of bytes read 
			NULL);        // not overlapped I/O 

		if (!fSuccess || cbBytesRead == 0)
		{   
			if (GetLastError() == ERROR_BROKEN_PIPE)
			{
				_tprintf(TEXT("InstanceThread: client disconnected.\n"), GetLastError()); 
			}
			else
			{
				_tprintf(TEXT("InstanceThread ReadFile failed, GLE=%d.\n"), GetLastError()); 
			}
			//break;
		}
		cout<<"value:"<<pchRequest<<endl;
	}

	fSuccess = WriteFile( 
		hPipe,        // handle to pipe 
		pchReply,     // buffer to write from 
		cbReplyBytes, // number of bytes to write 
		&cbWritten,   // number of bytes written 
		NULL);        // not overlapped I/O 

	// Flush the pipe to allow the client to read the pipe's contents 
	// before disconnecting. Then disconnect the pipe, and close the 
	// handle to this pipe instance. 
	Sleep(2000);
	FlushFileBuffers(hPipe); 
	DisconnectNamedPipe(hPipe); 
	CloseHandle(hPipe); 

	printf("InstanceThread exitting.\r\n");
	return 1;
}

VOID GetAnswerToRequest( LPTSTR pchRequest, 
						LPTSTR pchReply, 
						LPDWORD pchBytes )
						// This routine is a simple function to print the client request to the console
						// and populate the reply buffer with a default data string. This is where you
						// would put the actual client request processing code that runs in the context
						// of an instance thread. Keep in mind the main thread will continue to wait for
						// and receive other client connections while the instance thread is working.
{
	_tprintf( TEXT("Client Request String:\"%s\"\n\r"), pchRequest );

	// Check the outgoing message to make sure it's not too long for the buffer.
// 	if (FAILED(StringCchCopy( pchReply, BUFSIZE, TEXT("default answer from server") )))
// 	{
// 		*pchBytes = 0;
// 		pchReply[0] = 0;
// 		printf("StringCchCopy failed, no outgoing message.\n");
// 		return;
// 	}
// 	*pchBytes = (lstrlen(pchReply)+1)*sizeof(TCHAR);
}

int MultiPipe()
{
	BOOL   fConnected = FALSE; 
	DWORD  dwThreadId = 0; 
	HANDLE hPipe = INVALID_HANDLE_VALUE, hThread = NULL; 
	LPTSTR lpszPipename = TEXT("\\\\.\\pipe\\mynamedpipe"); 

	// The main loop creates an instance of the named pipe and 
	// then waits for a client to connect to it. When the client 
	// connects, a thread is created to handle communications 
	// with that client, and this loop is free to wait for the
	// next client connect request. It is an infinite loop.
	
	for (;;) 
	{ 
		_tprintf( TEXT("\nPipe Server: Main thread awaiting client connection on %s\n"), lpszPipename);
		hPipe = CreateNamedPipe( 
			lpszPipename,             // pipe name 
			PIPE_ACCESS_DUPLEX,       // read/write access 
			PIPE_TYPE_MESSAGE |       // message type pipe 
			PIPE_READMODE_MESSAGE |   // message-read mode 
			PIPE_WAIT,                // blocking mode 
			2,//PIPE_UNLIMITED_INSTANCES, // max. instances  
			BUFSIZE,                  // output buffer size 
			BUFSIZE,                  // input buffer size 
			0,                        // client time-out 
			NULL);                    // default security attribute 

		if (hPipe == INVALID_HANDLE_VALUE) 
		{
			_tprintf(TEXT("CreateNamedPipe failed, GLE=%d.\n"), GetLastError()); 
			if (GetLastError() == ERROR_PIPE_BUSY)
			{
				cout<<"busy"<<endl;
				WaitNamedPipe(lpszPipename, 500000000);
				continue;
			}
			else
			{
				return -1;
			}
			Sleep(100);
		}

		// Wait for the client to connect; if it succeeds, 
		// the function returns a nonzero value. If the function
		// returns zero, GetLastError returns ERROR_PIPE_CONNECTED. 

		fConnected = ConnectNamedPipe(hPipe, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED); 
		
		if (fConnected) 
		{ 
			printf("Client connected, creating a processing thread.\n"); 

			// Create a thread for this client. 
			hThread = CreateThread( 
				NULL,              // no security attribute 
				0,                 // default stack size 
				InstanceThread,    // thread proc
				(LPVOID) hPipe,    // thread parameter 
				0,                 // not suspended 
				&dwThreadId);      // returns thread ID 

			if (hThread == NULL) 
			{
				_tprintf(TEXT("CreateThread failed, GLE=%d.\n"), GetLastError()); 
				return -1;
			}
			else CloseHandle(hThread); 
		} 
		else 
			// The client could not connect, so close the pipe. 
			CloseHandle(hPipe); 
	} 
}

void CreateTest()
{
	HANDLE hPipe = CreateNamedPipe(_T("\\\\.\\pipe\\TESTPIPE"), 
		PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED , 
		0,
		10,
		MAX_PATH,
		MAX_PATH,
		10,
		NULL
		);

	if(INVALID_HANDLE_VALUE == hPipe)
	{
		hPipe = NULL;
		cout<<"创建命名管道失败 ..."<<endl;
		return ;
	}

	int i = 0;
	while (++i < 1000)
	{

	HANDLE hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (NULL == hEvent)
	{
		cout<<"创建事件失败 ..."<<endl;
		return ;
	}


	OVERLAPPED overlap = {0};
	overlap.hEvent = hEvent;
	BOOL bConnect = ConnectNamedPipe(hPipe, &overlap);
	if (!bConnect)
	{
		if(ERROR_IO_PENDING != GetLastError())
		{
			CloseHandle(hPipe);
			CloseHandle(hEvent);
			cout<<"等待客户端连接失败 ..."<<endl<<endl;
			return;
		}
	}

	WaitForSingleObject(hEvent, INFINITE);

	CloseHandle(hEvent);
	std::cout<<"wait end"<<std::endl;


		char szRead[MAX_PATH] = {0};
		DWORD dwHadRead = 0;
		ReadFile(hPipe, szRead, MAX_PATH, &dwHadRead, NULL);
		std::cout<<"read value "<<i<<": "<<szRead<<std::endl;

		Sleep(1000);

		//GetNamedPipeInfo(hPipe);
	}
	std::cout<<"read end"<<std::endl;


	//char szWrite[10] = ("qwer");
	//DWORD dwHasWrite = 0;
	//WriteFile(hPipe, szWrite, strlen(szWrite) + 1, &dwHasWrite, NULL);
	//std::cout<<"write end"<<std::endl;
}


void test()
{
	PipeCommuUtils* test = new PipeCommuUtils();
	test->StartPipeServer(PIPESERVERNAME);
}

int  _tmain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: 在此放置代码。

	int iSize = sizeof(OVERLAPPED);
	std::cout<<"start"<<std::endl;
	//CreateTest();
	//MultiPipe();

	test();

	Sleep(500000000);
	system("pause");
	return 0;
}
