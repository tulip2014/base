
#include "ExceptionLogUtils.h"
#include <windows.h>
#include <TCHAR.h>
#include <strsafe.h>
#include <stdio.h>
#include <DbgHelp.h>
#include <Rpc.h>

#pragma comment(lib,"Dbghelp.lib")
#pragma comment(lib,"Rpcrt4.lib")

namespace ExceptionLogUtils{

	static expCallBack g_callback = NULL;
	static BOOL m_bInitialSuccess = FALSE;
	static LPTOP_LEVEL_EXCEPTION_FILTER m_pReviousFilter = NULL;
	static TCHAR m_tcDmpFilePath[520] = {0};

	// 异常处理函数
	LONG WINAPI EL_UnhandledExceptionFilterFunc(PEXCEPTION_POINTERS pExceptionInfo)
	{

		SYSTEMTIME st = {0};
		GetLocalTime(&st);

		TCHAR tcDmpFilePath[520] = {0};

		UUID uuid = {0};
		RPC_WSTR pUUID = NULL;

		if(UuidCreate(&uuid) == RPC_S_OK && UuidToString(&uuid, &pUUID) == RPC_S_OK)
		{
			_stprintf_s(tcDmpFilePath, 520, _T("%s_%s.dmp"), m_tcDmpFilePath,pUUID);
			RpcStringFree(&pUUID);
		}
		else
		{
			SYSTEMTIME st = {0};
			GetSystemTime(&st);

			_stprintf_s(tcDmpFilePath,520,_T("%s_%04u%02u%02u%02u%02u%02u%03u.dmp"), m_tcDmpFilePath, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
		}

		HANDLE hFile=CreateFile(tcDmpFilePath,GENERIC_READ | GENERIC_WRITE,FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, 
			NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
		if(INVALID_HANDLE_VALUE != hFile)
		{
			MINIDUMP_EXCEPTION_INFORMATION eInfo = {0};
			eInfo.ThreadId = GetCurrentThreadId();
			eInfo.ExceptionPointers = pExceptionInfo;
			eInfo.ClientPointers = FALSE;

			MINIDUMP_TYPE DumpType = MiniDumpNormal;

			MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, DumpType, &eInfo, NULL, NULL);

			CloseHandle(hFile);
		}

		if (g_callback)
		{
			g_callback(tcDmpFilePath);
		}

		//直接退出，不弹框
		//	return EXCEPTION_EXECUTE_HANDLER;

		if(m_pReviousFilter != NULL)
			return m_pReviousFilter(pExceptionInfo);

		return EXCEPTION_CONTINUE_SEARCH;
	}

	// 初始化异常转储功能(进程初始化的时候调用，dll不需要调用)
	// DWORD dwFlag：预留的标志位，暂时全部填充为0
	// LPCTSTR lpszVersion：版本
	// LPCTSTR lpszLogFilePath：日志文件的路径，为NULL的话，则是进程主模块所在的路径的Log目录下
	// 日志文件名的组成规则：项目名称+版本+日期+时间.dmp，比如eSafeSvc_1.0.10582.47_201304080312090012.dmp
	bool InitialExceptionLog(unsigned int  dwFlag,LPCTSTR lpszProjectName,LPCTSTR lpszVersion,LPCTSTR lpszLogFilePath, expCallBack farProc )
	{
		if(m_bInitialSuccess)
			return true;

		g_callback = farProc;
		m_tcDmpFilePath[0] = _T('\0');
		if(lpszLogFilePath != NULL && _tcslen(lpszLogFilePath) > 0)
			_tcscpy_s(m_tcDmpFilePath, 520, lpszLogFilePath);
		else
		{
			GetModuleFileName(NULL, m_tcDmpFilePath, 520);
			TCHAR *p = _tcsrchr(m_tcDmpFilePath, _T('\\'));
			if(p != NULL)
				p[1] = _T('\0');

			_tcscat_s(m_tcDmpFilePath + _tcslen(m_tcDmpFilePath), 520 - _tcslen(m_tcDmpFilePath), _T("Log"));

			CreateDirectory(m_tcDmpFilePath, NULL);

			_tcscat_s(m_tcDmpFilePath + _tcslen(m_tcDmpFilePath), 520 - _tcslen(m_tcDmpFilePath), _T("\\"));
		}

		if(lpszProjectName != NULL)
			_tcscat_s(m_tcDmpFilePath + _tcslen(m_tcDmpFilePath), 520 - _tcslen(m_tcDmpFilePath), lpszProjectName);
		if(lpszVersion != NULL)
			_tcscat_s(m_tcDmpFilePath + _tcslen(m_tcDmpFilePath), 520 - _tcslen(m_tcDmpFilePath), lpszVersion);

		m_pReviousFilter = SetUnhandledExceptionFilter(EL_UnhandledExceptionFilterFunc);

		m_bInitialSuccess = TRUE;

		return true;
	}

	// 清理异常转储功能(进程退出前调用)
	bool ClearExceptionLog()
	{
		if(!m_bInitialSuccess)
			return true;

		if(m_pReviousFilter != NULL)
			SetUnhandledExceptionFilter(m_pReviousFilter);

		m_bInitialSuccess = FALSE;

		return true;
	}
}