
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


//#include "ntservmsg.h" // Event message ids
#include <windows.h>

#define SERVICE_CONTROL_USER					128
#define SERVICE_CONTROL_STOP_ALL_VDISK_SRV		SERVICE_CONTROL_USER + 1
#define SERVICE_CONTROL_START_CLIENT			SERVICE_CONTROL_USER + 2
#define SERVICE_CONTROL_PROTECT_CLIENT			SERVICE_CONTROL_USER + 3
//#define SERVICE_CONTROL_LOGIN		SERVICE_CONTROL_USER + 4

#define MAX_SERVICE_NAME_LENGTH		260

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
	DWORD dwControlsAccepted;							// accessable control
	TCHAR lpszDescription[MAX_SERVICE_NAME_LENGTH];		// description
}SERVICEDATA, *PSERVICEDATA;


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

class CNTService
{
public:
	CNTService(BOOL bAfterXP, BOOL bDelayStartup = FALSE );
	virtual ~CNTService();

	HRESULT WinMain(LPTSTR lpCmdLine);
	
	
	void LogEvent(WORD wType, DWORD dwID,
		const _TCHAR* pszS1 = NULL,
		const _TCHAR* pszS2 = NULL,
		const _TCHAR* pszS3 = NULL);

	BOOL StartSvc();
	BOOL InstallService();
	BOOL UninstallService();
	BOOL InstAndRunService();
	BOOL RunSvc();
	BOOL KillService();
	BOOL UninstAndKillService();
	BOOL Initialize();
	void SetStatus(DWORD dwState);
	

	virtual void Run();
	virtual BOOL OnInit();
	virtual void OnStop();
	virtual void OnInterrogate();
	virtual void OnPause();
	virtual void OnContinue();
	virtual void OnShutdown();
	virtual void OnFinal();
	virtual BOOL OnUserControl(DWORD dwOpcode);
	void DebugMsg(const _TCHAR* pszFormat, ...);

	// static member functions
	static void WINAPI ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv);
	static void WINAPI Handler(DWORD dwOpcode,DWORD evtype, PVOID evdata, PVOID Context);

	// static service data
	static SERVICEDATA	m_ServiceData;
	// static object
	static CNTService* m_pThis; // nasty hack to get object ptr

private:
	BOOL ParseCommandLine(LPTSTR lpCmdLine, HRESULT &hRet);
	BOOL Install();
	BOOL IsInstalled();
	BOOL Uninstall();
	

	HANDLE	m_hEventSource;
	BOOL	m_bIsAfterXP;
	BOOL	m_bIsRunning;
	BOOL	m_bDelayStartup;
	int		m_iMajorVersion;
	int		m_iMinorVersion;
	SERVICE_STATUS_HANDLE m_hServiceStatus;
	SERVICE_STATUS m_Status;

};