// CreateDump.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "CreateDump.h"
#include <imagehlp.h>
#include <windows.h>
#include <string>

#pragma comment(lib,"Dbghelp.lib")

// definition of COCriticalSectionHelper
class COCriticalSectionHelper
{
public:
	COCriticalSectionHelper( CRITICAL_SECTION* cs )	{
		m_cs = cs;
		EnterCriticalSection( m_cs );
	}
	~COCriticalSectionHelper()	{
		LeaveCriticalSection( m_cs );
	}
protected:
	CRITICAL_SECTION* m_cs;
};	// end of class COCriticalSectionHelper

#define _MANAGE_CRITICAL_SECTION( cs )	COCriticalSectionHelper helper( cs )

// 异常处理函数
LONG WINAPI EL_UnhandledExceptionFilterFunc(PEXCEPTION_POINTERS pExceptionInfo)
{
	HANDLE hFile = CreateFile(_T("d:\\test.dmp"), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if((hFile != NULL) && (hFile != INVALID_HANDLE_VALUE))
	{
		MINIDUMP_EXCEPTION_INFORMATION eInfo={0};
		eInfo.ThreadId=GetCurrentThreadId();
		eInfo.ExceptionPointers=pExceptionInfo;
		eInfo.ClientPointers=FALSE;

		MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),

			hFile, MiniDumpNormal, &eInfo, 0, 0);



		CloseHandle(hFile);

	}
	return true;
}

std::wstring Test()
{
	return false;
}


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: 在此放置代码。
	SetUnhandledExceptionFilter(EL_UnhandledExceptionFilterFunc);

	CRITICAL_SECTION cs; //可以理解为锁定一个资源
	InitializeCriticalSection( &cs );
	
	_MANAGE_CRITICAL_SECTION( &cs );

	std::wstring tmp = Test();

	return 0;
}

