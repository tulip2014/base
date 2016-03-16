
#include "ProcessUtils.h"
#include "OSUtils.h"
#include "../utils/StringUtils.h"
#include <Psapi.h>
#include <shellapi.h>
#include <Shlwapi.h>
#include <winternl.h>
#include <tchar.h>
#include <strsafe.h>

#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "Psapi.lib")

#ifndef MAX_PATHEX
#define MAX_PATHEX 520
#endif

typedef NTSTATUS (WINAPI *NtQueryInfoProc)(HANDLE,
										   PROCESSINFOCLASS,
										   LPVOID,
										   ULONG,
										   PULONG);

// 替换路径中的特殊字符
#define ADDTAIL(p)									\
	do{												\
		if((p)!=NULL && (p)[wcslen(p)-1]!=L'\\')	\
			wcscat_s((p),MAX_PATHEX,L"\\");			\
	}while(0)

namespace ProcUtils
{
	bool CreateProcess(const std::wstring& sProcessName, const std::wstring& sCmdLine, const std::wstring& sCurrentDirectory, bool bForceAdmin, bool bWaitProcess,bool bIsshow)
	{
		BOOL bRet = FALSE;
		HANDLE hProcess = NULL;
		TCHAR szCmd[MAX_PATH * 2] = {0};

		SHELLEXECUTEINFO sei = {sizeof(SHELLEXECUTEINFO)};

		sei.fMask = SEE_MASK_NOCLOSEPROCESS|SEE_MASK_FLAG_NO_UI;
		sei.hwnd = NULL;
		if (OSUtils::IsVistaOrLater())
			sei.lpVerb = bForceAdmin ? L"runas" : L"open";
		else
			sei.lpVerb = L"open";

		sei.lpFile = sProcessName.c_str();
		sei.lpParameters = sCmdLine.c_str();
		bIsshow ? sei.nShow = SW_SHOWNORMAL : sei.nShow = SW_HIDE;
		sei.lpDirectory = sCurrentDirectory.c_str();

		bRet = ::ShellExecuteEx(&sei);

		hProcess = sei.hProcess;

		if (bRet)
		{
			if (bWaitProcess)
			{
				::WaitForSingleObject(hProcess, INFINITE);
			}
			::CloseHandle(hProcess);
		}

		return bRet ? true : false;
	}

	BOOL GetDosDeviceInfo(PDOSDEVICEINFO pInfo, DWORD& dwInfoNum)
	{
		wchar_t szTemp[520] = {0};
		dwInfoNum = 0;

		if (GetLogicalDriveStrings(520-1, szTemp)) 
		{
			wchar_t* p = szTemp;

			do 
			{
				pInfo[dwInfoNum].tcDosName[0] = *p;
				pInfo[dwInfoNum].tcDosName[1] = L':';
				pInfo[dwInfoNum].tcDosName[2] = L'\0';

				// Look up each device name
				if (QueryDosDevice(pInfo[dwInfoNum].tcDosName, pInfo[dwInfoNum].tcDeviceName, 520))
				{
					pInfo[dwInfoNum].tcDosName[2]=L'\\';
					pInfo[dwInfoNum].tcDosName[3]=L'\0';

					if(pInfo[dwInfoNum].tcDeviceName[wcslen(pInfo[dwInfoNum].tcDeviceName)-1]!=L'\\')
						wcscat_s(pInfo[dwInfoNum].tcDeviceName,520,L"\\");

					//				MyWriteProgramLog(_T("%s %s"),pInfo[dwInfoNum].tcDosName,pInfo[dwInfoNum].tcDeviceName);

					dwInfoNum++;
				}

				// Go to the next NULL character.
				while (*p++);
			} while (*p); // end of string
		}

		return TRUE;
	}

	BOOL TreatSperialPath(LPWSTR lpszPath)
	{
		if(lpszPath==NULL)
			return TRUE;

#define MAX_SP_COUNT  9

		static LPCWSTR g_pSRC[MAX_SP_COUNT] = 
		{
			L"%SystemRoot%\\",
			L"%ProgramFiles%\\",
			L"%windir%\\",
			L"\\SystemRoot\\",
			L"GlobalRoot\\systemroot\\",
			L"\\??\\GlobalRoot\\systemroot\\",
			L"system32\\",
			L"system\\",
			L"\\??\\"
		};

		static wchar_t g_pDES[MAX_SP_COUNT][MAX_PATHEX]={0};

		if(g_pDES[0][0]==L'\0')
		{
			GetEnvironmentVariableW(L"SystemRoot",g_pDES[0],MAX_PATHEX);
			ADDTAIL(g_pDES[0]);

			GetEnvironmentVariableW(L"ProgramFiles",g_pDES[1],MAX_PATHEX);
			ADDTAIL(g_pDES[1]);

			GetEnvironmentVariableW(L"windir",g_pDES[2],MAX_PATHEX);
			ADDTAIL(g_pDES[2]);

			wcscpy_s(g_pDES[3],MAX_PATHEX,g_pDES[0]);
			wcscpy_s(g_pDES[4],MAX_PATHEX,g_pDES[0]);
			wcscpy_s(g_pDES[5],MAX_PATHEX,g_pDES[0]);

			GetSystemDirectory(g_pDES[6],MAX_PATHEX);
			ADDTAIL(g_pDES[6]);

			wcscpy_s(g_pDES[7],MAX_PATHEX,g_pDES[6]);
			DWORD dwLength=wcslen(g_pDES[7]);
			g_pDES[7][dwLength-3]=L'\\';
			g_pDES[7][dwLength-2]= L'\0';

			g_pDES[8][0]=L'\0';
		}

		for(DWORD i=0;i<MAX_SP_COUNT;i++)
		{
			if(_wcsnicmp(lpszPath,g_pSRC[i],wcslen(g_pSRC[i]))==0)
			{
				DWORD dwSrcLength=wcslen(g_pSRC[i]);
				DWORD dwDesLength=wcslen(g_pDES[i]);

				memmove(lpszPath+dwDesLength,lpszPath+dwSrcLength,(wcslen(lpszPath)-dwSrcLength+1)*sizeof(wchar_t));
				memmove(lpszPath,g_pDES[i],wcslen(g_pDES[i])*sizeof(wchar_t));

				return TRUE;
			}
		}

		return TRUE;
	}

	BOOL GetProcessImagePath(HANDLE hProcess, LPWSTR lpszIamge, PDOSDEVICEINFO pInfo, DWORD dwInfoNum)
	{
		typedef DWORD (WINAPI *PGetProcessImageFileName)(
			__in   HANDLE hProcess,
			__out  LPWSTR lpImageFileName,
			__in   DWORD nSize
			);

		if(OSUtils::IsWow64())
		{
			static PGetProcessImageFileName m_pGetProcessImageFileName=NULL;
			if(m_pGetProcessImageFileName == NULL)
			{
				wchar_t tcPath[520]={0};
				GetSystemDirectory(tcPath,520);

				if(tcPath[wcslen(tcPath)-1]!=L'\\')
					wcscat_s(tcPath,520,L"\\");

				wcscat_s(tcPath,520,L"psapi.dll");

				HINSTANCE hPSAPI=LoadLibrary(tcPath);
				if(hPSAPI!=NULL)
					m_pGetProcessImageFileName=(PGetProcessImageFileName)GetProcAddress(hPSAPI,"GetProcessImageFileNameW");
			}

			if(m_pGetProcessImageFileName==NULL)
				return FALSE;

			if(!m_pGetProcessImageFileName(hProcess,lpszIamge,520))
				return FALSE;

			for(DWORD i=0;i<dwInfoNum;i++)
			{
				if(_wcsnicmp(pInfo[i].tcDeviceName,lpszIamge,wcslen(pInfo[i].tcDeviceName))==0)
				{
					memmove(lpszIamge,pInfo[i].tcDosName,(wcslen(pInfo[i].tcDosName)+1)*sizeof(wchar_t));
					memmove(lpszIamge+wcslen(lpszIamge),lpszIamge+wcslen(pInfo[i].tcDeviceName),(wcslen(lpszIamge+wcslen(pInfo[i].tcDeviceName))+1)*sizeof(wchar_t));
				}
			}

			return TRUE;
		}
		else
		{
			HMODULE hMod;
			DWORD cbNeeded;

			if(EnumProcessModules(hProcess,&hMod,sizeof(hMod),&cbNeeded))
			{
				GetModuleFileNameEx(hProcess,hMod,lpszIamge,520);

				TreatSperialPath(lpszIamge);

				return TRUE;
			}
			else
			{
				DWORD dwError=GetLastError();
				//			MyWriteProgramLog("EnumProcessModules hProcess=%u Error=%u",hProcess,dwError);

				return FALSE;
			}
		}

		return TRUE;
	}

	BOOL GetProcessImage(DWORD dwPID,LPWSTR lpszImage,PDOSDEVICEINFO pInfo,DWORD dwInfoNum)
	{
		lpszImage[0]=L'\0';
		LPCTSTR lpszHeadPath=L"\\??\\";

		HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
			PROCESS_VM_READ,
			FALSE, dwPID );

		if ( hProcess )
		{
			if(!GetProcessImagePath(hProcess,lpszImage,pInfo,dwInfoNum))
				return FALSE;

			if(wcsncmp(lpszImage,lpszHeadPath,wcslen(lpszHeadPath))==0)
				memmove(lpszImage,lpszImage+4,(wcslen(lpszImage)-4)*sizeof(wchar_t)+1);

			//		MyWriteProgramLog(_T("PID=%u Path=%s"),dwPID,lpszImage);
		}
		else
		{
			DWORD dwError=GetLastError();
			//		MyWriteProgramLog("OpenProcess PID=%u Error=%u",dwPID,dwError);
		}

		CloseHandle(hProcess);

		return TRUE;
	}

	bool CheckProcess(const std::wstring& sProcessName)
	{
		bool bRet = true;
		DWORD aProcesses[1024],cbNeeded,cProcesses;
		wchar_t tcProcessPath[520] = {0};

		if(!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
			return bRet;

		cProcesses=cbNeeded/sizeof(DWORD);

		DOSDEVICEINFO info[26] = {0};
		DWORD dwInfoNum = 0;
		GetDosDeviceInfo(info, dwInfoNum);

		for(DWORD j = 0; j < cProcesses; j++)
		{
			GetProcessImage(aProcesses[j], tcProcessPath, info, dwInfoNum);
			std::wstring strFileName = StringUtils::to_lower(PathFindFileName(tcProcessPath));
			if(_wcsicmp(strFileName.c_str(), sProcessName.c_str()) == 0)
				return true;
		}

		return false;
	}

	bool TerminateProcess(const std::wstring& szPath, DWORD dwMilliseconds)
	{
		bool bRet = true;
		DWORD aProcesses[1024],cbNeeded,cProcesses;
		wchar_t tcProcessPath[520] = {0};

		if(!EnumProcesses(aProcesses,sizeof(aProcesses),&cbNeeded))
			return bRet;

		cProcesses=cbNeeded/sizeof(DWORD);

		DOSDEVICEINFO info[26]={0};
		DWORD dwInfoNum=0;
		GetDosDeviceInfo(info,dwInfoNum);

		for(DWORD j=0;j<cProcesses;j++)
		{
			//		MyWriteProgramLog("PID=%u",aProcesses[j]);

			GetProcessImage(aProcesses[j],tcProcessPath,info,dwInfoNum);

			//change to long path
			wchar_t tcLongPath[520]={0};
			::GetLongPathNameW(tcProcessPath,tcLongPath,520);
			wcscpy_s(tcProcessPath,tcLongPath);

			if(_wcsnicmp(szPath.c_str(),tcProcessPath,szPath.size())==0)
			{

				HANDLE hProcess=OpenProcess(PROCESS_TERMINATE | SYNCHRONIZE,FALSE,aProcesses[j]);
				if(hProcess!=NULL)
				{
					::TerminateProcess(hProcess,1);

					DWORD dwRet=::WaitForSingleObject(hProcess,dwMilliseconds);
					if(dwRet!=WAIT_OBJECT_0)
					{
						dwRet=GetLastError();
						bRet = false;
					}

					CloseHandle(hProcess);
				}
				else
				{
					DWORD dwRet=GetLastError();
					bRet = false;
				}
			}
		}

		return bRet;
	}

	bool unRegComdll(const std::wstring& sProcessName)
	{
		//unreg com
		std::wstring sCmdline;
		sCmdline = L"/s /u ";
		sCmdline.append(sProcessName);
		return CreateProcess(L"regsvr32.exe",sCmdline,L"",false,true,false);
	}

#define PROC_USERS_PARAMS_OFFSET 16
#define PROC_CMDLINE_STR_OFFSET 64

#define SHIFT_TO_VA(BaseAddress, Offset)	(((LPBYTE)(BaseAddress)) + (Offset))

	typedef enum _PROCESSINFOCLASS
	{
		ProcessBasicInformation,	// 0, q: PROCESS_BASIC_INFORMATION, PROCESS_EXTENDED_BASIC_INFORMATION
		ProcessQuotaLimits,			// qs: QUOTA_LIMITS, QUOTA_LIMITS_EX
		ProcessIoCounters,			// q: IO_COUNTERS
		ProcessVmCounters,			// q: VM_COUNTERS, VM_COUNTERS_EX
		ProcessTimes,				// q: KERNEL_USER_TIMES
		ProcessBasePriority,		// s: KPRIORITY
		ProcessRaisePriority,		// s: ULONG
		ProcessDebugPort,			// q: HANDLE
		ProcessExceptionPort,		// s: HANDLE
		ProcessAccessToken,			// s: PROCESS_ACCESS_TOKEN
		ProcessLdtInformation,		// 10
		ProcessLdtSize,
		ProcessDefaultHardErrorMode,// qs: ULONG
		ProcessIoPortHandlers,		// (kernel-mode only)
		ProcessPooledUsageAndLimits,// q: POOLED_USAGE_AND_LIMITS
		ProcessWorkingSetWatch,		// q: PROCESS_WS_WATCH_INFORMATION[]; s: void
		ProcessUserModeIOPL,
		ProcessEnableAlignmentFaultFixup, // s: BOOLEAN
		ProcessPriorityClass,		// qs: PROCESS_PRIORITY_CLASS
		ProcessWx86Information,
		ProcessHandleCount,			// 20, q: ULONG, PROCESS_HANDLE_INFORMATION
		ProcessAffinityMask,		// s: KAFFINITY
		ProcessPriorityBoost,		// qs: ULONG
		ProcessDeviceMap,			// qs: PROCESS_DEVICEMAP_INFORMATION, PROCESS_DEVICEMAP_INFORMATION_EX
		ProcessSessionInformation,	// q: PROCESS_SESSION_INFORMATION
		ProcessForegroundInformation, // s: PROCESS_FOREGROUND_BACKGROUND
		ProcessWow64Information,	// q: ULONG_PTR
		ProcessImageFileName,		// q: UNICODE_STRING
		ProcessLUIDDeviceMapsEnabled, // q: ULONG
		ProcessBreakOnTermination,	// qs: ULONG
		ProcessDebugObjectHandle,	// 30, q: HANDLE
		ProcessDebugFlags,			// qs: ULONG
		ProcessHandleTracing,		// q: PROCESS_HANDLE_TRACING_QUERY; s: size 0 disables, otherwise enables
		ProcessIoPriority,			// qs: ULONG
		ProcessExecuteFlags,		// qs: ULONG
		ProcessResourceManagement,
		ProcessCookie,				// q: ULONG
		ProcessImageInformation,	// q: SECTION_IMAGE_INFORMATION
		ProcessCycleTime,			// q: PROCESS_CYCLE_TIME_INFORMATION
		ProcessPagePriority,		// q: ULONG
		ProcessInstrumentationCallback, // 40
		ProcessThreadStackAllocation, // s: PROCESS_STACK_ALLOCATION_INFORMATION, PROCESS_STACK_ALLOCATION_INFORMATION_EX
		ProcessWorkingSetWatchEx,	// q: PROCESS_WS_WATCH_INFORMATION_EX[]
		ProcessImageFileNameWin32,	// q: UNICODE_STRING
		ProcessImageFileMapping,	// q: HANDLE (input)
		ProcessAffinityUpdateMode,	// qs: PROCESS_AFFINITY_UPDATE_MODE
		ProcessMemoryAllocationMode,// qs: PROCESS_MEMORY_ALLOCATION_MODE
		ProcessGroupInformation,	// q: USHORT[]
		ProcessTokenVirtualizationEnabled, // s: ULONG
		ProcessConsoleHostProcess,	// q: ULONG_PTR
		ProcessWindowInformation,	// 50, q: PROCESS_WINDOW_INFORMATION
		ProcessHandleInformation,	// q: PROCESS_HANDLE_SNAPSHOT_INFORMATION // since WIN8
		ProcessMitigationPolicy,	// s: PROCESS_MITIGATION_POLICY_INFORMATION
		ProcessDynamicFunctionTableInformation,
		ProcessHandleCheckingMode,
		ProcessKeepAliveCount,		// q: PROCESS_KEEPALIVE_COUNT_INFORMATION
		ProcessRevokeFileHandles,	// s: PROCESS_REVOKE_FILE_HANDLES_INFORMATION
		ProcessWorkingSetControl,
		ProcessHandleTable,			// since WINBLUE
		ProcessCheckStackExtentsMode,
		ProcessCommandLineInformation,
		ProcessProtectionInformation,
		MaxProcessInfoClass
	} PROCESSINFOCLASS;

	typedef NTSTATUS (WINAPI *NtQueryInfoProc)(HANDLE,
		PROCESSINFOCLASS,
		LPVOID,
		ULONG,
		PULONG);

	LPTSTR GetCommandLine(HANDLE hProc)
	{
		NtQueryInfoProc pfn = (NtQueryInfoProc)GetProcAddress(GetModuleHandle(_T("ntdll.dll")), "NtQueryInformationProcess");
		if (pfn == NULL)
		{
			return NULL;
		}

		PROCESS_BASIC_INFORMATION pbi = {0};
		if (pfn(hProc
			, ProcessBasicInformation
			, &pbi
			, sizeof(pbi)
			, NULL)
			== ERROR_SUCCESS)
		{
			DWORD dwVAddr;
			if (ReadProcessMemory(hProc
				, SHIFT_TO_VA(pbi.PebBaseAddress, PROC_USERS_PARAMS_OFFSET)
				, &dwVAddr
				, sizeof(dwVAddr)
				, NULL)
				== FALSE)
			{
				return NULL;
			}

			struct
			{
				USHORT Length;
				USHORT MaximumLength;
				DWORD  Buffer;
			}STRING;

			if (ReadProcessMemory(hProc
				, SHIFT_TO_VA(dwVAddr, PROC_CMDLINE_STR_OFFSET)
				, &STRING
				, sizeof(STRING)
				, NULL)
				== FALSE)
			{
				return NULL;
			}

			DWORD dwNeededSize = (STRING.Length + 1) * sizeof(wchar_t);
			LPTSTR pBuf = (LPTSTR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwNeededSize);
			if (pBuf)
			{
				if (ReadProcessMemory(hProc
					, (LPBYTE)STRING.Buffer
					, pBuf
					, dwNeededSize
					, &dwNeededSize)
					== FALSE)
				{
					HeapFree(GetProcessHeap(), 0, pBuf);
					pBuf = NULL;
				}
			}

			return pBuf;
		}

		return NULL;
	}

	std::wstring GetProcCmdLine(const DWORD& dwPid)
	{
		std::wstring szCmdLine = _T("");

		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
		if (hProcess)
		{
			LPTSTR lpCmdLine = GetCommandLine(hProcess);
			if (lpCmdLine)
			{
				TCHAR tcBuf[MAX_PATHEX] = {0};
				StringCbCopy(tcBuf, MAX_PATHEX * sizeof(TCHAR), lpCmdLine);
				HeapFree(GetProcessHeap(), 0, lpCmdLine);

				LPTSTR lpcsArg = PathGetArgs(tcBuf);
				if (lpcsArg)
				{
					szCmdLine = lpcsArg;
				}
			}
		}

		return szCmdLine;
	}
}