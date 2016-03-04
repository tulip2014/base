// nimidump.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
#include <tchar.h>
#include <time.h>
#include <DbgHelp.h>
#include <iostream>

#pragma comment(lib, "DbgHelp.lib")

LONG WINAPI MyUnhandledExceptionFilter(struct _EXCEPTION_POINTERS* lpExceptionInfo)
{
	MessageBox(NULL, _T("Program crashed, generate dump file and call bug report!"), _T("Oops"), MB_OK);
	TCHAR szProgramPath[MAX_PATH] = {0};
	if(GetModuleFileName(NULL, szProgramPath, MAX_PATH))
	{
		LPTSTR lpSlash = _tcsrchr(szProgramPath, '//');
		if(lpSlash)
		{
			*(lpSlash + 1) = '/0';
		}
	}
	TCHAR szDumpFile[MAX_PATH] = {0};
	_stprintf_s(szDumpFile, MAX_PATH, _T("%s%d.dmp"), szProgramPath, time(NULL));
	TCHAR szReportFile[MAX_PATH] = {0};
	_stprintf_s(szReportFile, MAX_PATH, _T("%sBugReport.exe"), szProgramPath);

	HANDLE hDumpFile = CreateFile(szDumpFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL ,NULL);
	MINIDUMP_EXCEPTION_INFORMATION stMiniDumpExceptionInfo;
	stMiniDumpExceptionInfo.ExceptionPointers = lpExceptionInfo;
	stMiniDumpExceptionInfo.ThreadId = GetCurrentThreadId();
	stMiniDumpExceptionInfo.ClientPointers = TRUE;
	MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile,
		MiniDumpNormal, &stMiniDumpExceptionInfo, NULL, NULL);
	CloseHandle(hDumpFile);
	//::ShellExecute(NULL, NULL, szReportFile, szDumpFile, NULL, SW_HIDE);

	//Use your own user-friendly ui prompt here
	MessageBox(NULL, _T("Program crashed, generate dump file and call bug report!"), _T("Oops"), MB_OK);
	return EXCEPTION_EXECUTE_HANDLER;
}


int _tmain(int argc, _TCHAR* argv[])
{
	MessageBox(NULL, _T("Program crashed, generate dump file and call bug report!"), _T("Oops"), MB_OK);
	LPTOP_LEVEL_EXCEPTION_FILTER pPrevFilter = ::SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);

// 	int i = 0;  
// 	i = 3 / i;
	try
	{
		int a[3] = {0};
		a[4] = 0;
	}
	catch (...)
	{
		
	}


	int c = 1;
	std::cout<<"end"<<std::endl;
	std::cin>>c;
	return 0;
}

