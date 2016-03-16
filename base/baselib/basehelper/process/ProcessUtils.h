/********************************************************************
	created:	2016/03/16
	created:	16:3:2016   16:55
	filename: 	base\baselib\basehelper\process\ProcessUtils.h
	file path:	base\baselib\basehelper\process
	file base:	ProcessUtils
	file ext:	h
	author:		
	
	purpose:	
*********************************************************************/

#pragma once


#include <windows.h>
#include <string>

namespace ProcUtils{

	typedef struct tagDOSDEVICEINFO
	{
		wchar_t tcDosName[4];
		wchar_t tcDeviceName[520];
	}DOSDEVICEINFO, *PDOSDEVICEINFO;

	bool TerminateProcess(const std::wstring& szPath, DWORD dwMilliseconds = 2000);
	bool CreateProcess(const std::wstring& sProcessName, const std::wstring& sCmdLine, const std::wstring& sCurrentDirectory, bool bForceAdmin, bool bWaitProcess,bool bIsshow);
	bool unRegComdll(const std::wstring& sProcessName);
	bool CheckProcess(const std::wstring& sProcessName);

	// 获取指定进程的命令行参数
	//LPTSTR GetCommandLine(HANDLE hProc);

	std::wstring GetProcCmdLine(const DWORD& dwPid);
}

