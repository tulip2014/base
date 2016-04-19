
#pragma once

#include <windows.h>

int InitialPipe(unsigned __int32 a1, unsigned __int32 a2);
int ClearPipe();
int StopPipe();

class CMyPipe
{
public:
	static int StaticSendBufferLength;
	static int StaticReceiveBufferLength;
	static HANDLE StaticExitEvent;
	static int StaticFlag;

	static int initial(unsigned __int32 a1, unsigned __int32 a2);
	static int clear();
	static int stop();

	BOOL IsConnected();
	CMyPipe();
	~CMyPipe();
	int ClearCriticalSection();
	int ClearLocaleValue();
	int ClearOverlapped();
	int CreateCriticalSection();
	HANDLE CreateNewPipe(LPCWSTR lpName, int a3);
	int CreateOverlapped();
	void GetRandomName(wchar_t *Dst, int SizeInWords);
	int InitialLocaleValue();
	int ResetOverlapped();
	int accept(CMyPipe *hFile);
	int close();
	int connect(const wchar_t *Str);
	int listen(wchar_t *Str, int a3);
	int receive(void *lpBuffer, int *a3, int a4);
	int send(LPCVOID lpBuffer, int nNumberOfBytesToWrite);
	int set(int a2, int a3, int a4, int a5, int a6);

private:
	DWORD dwArg0;
	DWORD dwArg1;  //等待时间
	DWORD dwArg2;
	DWORD dwArg3;
	DWORD dwArg4;  //等待时间
	DWORD dwArg20[25];
	DWORD dwArg30;
	HANDLE dwArg31;    //创建的新管道 //(创建的文件句柄)
	OVERLAPPED overlap;
	//HANDLE hdArg36;   //enent handle
	HANDLE hdArg37;   //event handle
	LPCRITICAL_SECTION* lpc_section38;  //临界区指针
};