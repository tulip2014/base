/********************************************************************
	created:	2016/03/04
	created:	4:3:2016   15:30
	filename: 	OSHelper.cpp
	file path:	
	file base:	OSHelper
	file ext:	cpp
	author:		murisly
	
	purpose:	
*********************************************************************/

#pragma once

#include "OSUtils.h"

#define WINMOD_UNKNOWN_OS   0x00000000
#define WINMOD_XP_SP0       0x05010000
#define WINMOD_XP_SP1       0x05010100
#define WINMOD_XP_SP2       0x05010200
#define WINMOD_XP_SP3       0x05010300
#define WINMOD_2K3_SP0      0x05020000
#define WINMOD_2K3_SP1      0x05020100
#define WINMOD_2K3_SP2      0x05020200
#define WINMOD_VISTA_SP0    0x06000000
#define WINMOD_VISTA_SP1    0x06000100
#define WINMOD_VISTA_SP2    0x06000200
#define WINMOD_WIN7_SP0     0x06010000


namespace OSUtils
{
	enum {
		em_OS_MajorVer_Win7     = 6,
		em_OS_MajorVer_Vista    = 6,
		em_OS_MajorVer_Win2k3   = 5,
		em_OS_MajorVer_WinXP    = 5,

		em_OS_MinorVer_Win7     = 1,

		em_OS_MinorVer_Win2k3   = 2,
		em_OS_MinorVer_WinXP    = 1,
	};


	BOOL IsX86()
	{
		SYSTEM_INFO sysInfo;
		memset(&sysInfo, 0, sizeof(sysInfo));
		GetSystemInfo(&sysInfo);

		if (PROCESSOR_ARCHITECTURE_INTEL == sysInfo.wProcessorArchitecture)
			return TRUE;

		return FALSE;
	}

	BOOL IsWow64()
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

	BOOL RebootSystem()
	{
		HANDLE hToken = NULL; 
		BOOL bRet = TRUE;

		if(OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) 
		{
			TOKEN_PRIVILEGES tkp; 
			LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid); 

			tkp.PrivilegeCount = 1;
			tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 

			AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0); 

			if(GetLastError() != ERROR_SUCCESS) 
				bRet = FALSE; 

			CloseHandle(hToken);
		}

		if(bRet)
			bRet = ExitWindowsEx(EWX_REBOOT | EWX_FORCE, 0);

		return bRet;
	}

	/**
	* @retval   >0  current OS is greater than compared version
	* @retval   <0  current OS is less than compared version
	* @retval   0   current OS is equal to compared version
	*/
	int CompareVersion(DWORD dwMajorVer, DWORD dwMinorVer)
	{
		OSVERSIONINFO osInfo;
		osInfo.dwOSVersionInfoSize = sizeof(osInfo);
		::GetVersionEx(&osInfo);

		if (osInfo.dwMajorVersion > dwMajorVer)
		{
			return 1;
		}
		else if (osInfo.dwMajorVersion < dwMajorVer)
		{
			return -1;
		}

		return osInfo.dwMinorVersion - dwMinorVer;
	}


	int CompareMajor(DWORD dwMajorVer)
	{
		OSVERSIONINFO osInfo;
		osInfo.dwOSVersionInfoSize = sizeof(osInfo);
		::GetVersionEx(&osInfo);

		return osInfo.dwMajorVersion - dwMajorVer;
	}


	BOOL IsWin7OrLater()
	{
		return 0 <= CompareVersion(em_OS_MajorVer_Win7, em_OS_MinorVer_Win7);
	}

	BOOL IsVistaOrLater()
	{
		return 0 <= CompareMajor(em_OS_MajorVer_Vista);
	}

	BOOL IsWinXPOrLater()
	{
		return 0 <= CompareVersion(em_OS_MajorVer_WinXP, em_OS_MinorVer_WinXP);
	}

	BOOL IsWin2k3()
	{
		return 0 == CompareVersion(em_OS_MajorVer_Win2k3, em_OS_MinorVer_Win2k3);
	}

};
