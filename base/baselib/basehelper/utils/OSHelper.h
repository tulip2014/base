/********************************************************************
	created:	2016/03/04
	created:	4:3:2016   16:26
	filename: 	OSHelper.h
	file path:	
	file base:	OSHelper
	file ext:	h
	author:		murisly
	
	purpose:	
*********************************************************************/

#pragma once

#include "Winbase.h"

class OSHelper {

public:
	static bool IsX86() {
		SYSTEM_INFO sysInfo;
		memset(&sysInfo, 0, sizeof(sysInfo));
		GetSystemInfo(&sysInfo);

		if (PROCESSOR_ARCHITECTURE_INTEL == sysInfo.wProcessorArchitecture)
			return TRUE;

		return FALSE;
	}


	//************************************
	// Method:    IsWow64
	// FullName:  OSHelper::IsWow64
	// Access:    public static 
	// Returns:   BOOL
	// Qualifier: Determines whether the specified process is running under WOW64. 
	//************************************
	typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS)(HANDLE, PBOOL);
	static BOOL IsWow64()
	{
		LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(L"kernel32"), "IsWow64Process");
		BOOL bIsWow64 = false;

		if (NULL != fnIsWow64Process)
		{
			if (!fnIsWow64Process(GetCurrentProcess(), &bIsWow64))
			{
				return false;
			}
		}
		
		return  bIsWow64;
	}
};