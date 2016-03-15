
/********************************************************************
	created:	2016/03/15
	created:	15:3:2016   13:46
	filename: 	base\baselib\basehelper\utils\SvcUtils.h
	file path:	base\baselib\basehelper\utils
	file base:	SvcUtils
	file ext:	h
	author:		murisly
	
	purpose:	
*********************************************************************/

#pragma once


#include <windows.h>

#define MAX_SERVICE_NAME_LENGTH		260

#define SVC_ORDERGROUP_1        _T("SchedulerGroup")
#define SVC_ORDERGROUP_2        _T("SpoolerGroup")

#define CMDLINE_PARAM_NAME_INSTALL       L"install"
#define CMDLINE_PARAM_NAME_UNINSTALL     L"uninstall"
#define CMDLINE_PARAM_NAME_DEBUG         L"debug"
#define CMDLINE_PARAM_NAME_START         L"start"
#define CMDLINE_PARAM_NAME_RUN           L"run"
#define CMDLINE_PARAM_NAME_RESTART       L"restart"
#define CMDLINE_PARAM_NAME_STOP          L"stop"
#define CMDLINE_PARAM_NAME_KILL          L"kill"


typedef struct tagServiceData
{
	TCHAR lpszServiceName[MAX_SERVICE_NAME_LENGTH];		// name of service to start
	TCHAR lpszDisplayName[MAX_SERVICE_NAME_LENGTH];		// display name
	DWORD dwServiceType;								// type of service
	DWORD dwStartType;									// when to start service
	DWORD dwErrorControl;								// severity of service failure
	TCHAR lpszBinaryPathName[MAX_SERVICE_NAME_LENGTH];	// name of binary file
	TCHAR lpszLoadOrderGroup[MAX_SERVICE_NAME_LENGTH];	// name of load ordering group
	DWORD dwTagId;										// tag identifier
	TCHAR lpszDependencies[MAX_SERVICE_NAME_LENGTH];	// array of dependency names
	DWORD dwControlsAccepted;							// Acceptable control
	TCHAR lpszDescription[MAX_SERVICE_NAME_LENGTH];		// description
}SERVICEDATA, *PSERVICEDATA;

class CNTService
{
	CNTService(BOOL bAfterXP, bool bDelayStartup = false );
	virtual ~CNTService();

	HRESULT WinMain(LPTSTR lpCmdLine);
	BOOL IsInstalled();
	BOOL Install();


	BOOL InstallService();
	BOOL UninstallService();
	BOOL StartService();
	BOOL RunService();
	BOOL InstAndRunService();
	BOOL KillService();
	BOOL UninstAndKillService();

	void DebugMsg(const _TCHAR* pszFormat, ...);

	// static member functions
	static void WINAPI ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv);

	// static data
	static CNTService* m_pThis;
	static SERVICEDATA	m_ServiceData;

	SERVICE_STATUS m_Status;

private:
	BOOL ParseCommandLine(LPTSTR lpCmdLine, HRESULT &hRet);
	BOOL Uninstall();

	BOOL m_bIsAfterXP;
};