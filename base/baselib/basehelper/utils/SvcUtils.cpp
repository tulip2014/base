
/********************************************************************
	created:	2016/03/15
	created:	15:3:2016   13:56
	filename: 	D:\work\tulip\base\baselib\basehelper\utils\SvcUtils.cpp
	file path:	D:\work\tulip\base\baselib\basehelper\utils
	file base:	SvcUtils
	file ext:	cpp
	author:		murisly
	
	purpose:	
*********************************************************************/


#include "SvcUtils.h"
#include "CmdLineUtils.h"

CNTService::CNTService(BOOL bAfterXP, bool bDelayStartup ) 
{
	// copy the address of the current object so we can access it from
	// the static member callback functions. 
	// WARNING: This limits the application to only one CNTService object. 
	m_pThis = this;

	m_bIsAfterXP = bAfterXP;
	// Set the default service name and version

	//m_iMajorVersion = 2;
	//m_iMinorVersion = 0;
	//m_hEventSource = NULL;

	// set up the initial service status 
	//m_hServiceStatus = NULL;
	//m_Status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	//m_Status.dwCurrentState = SERVICE_STOPPED;
	//m_Status.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	//m_Status.dwWin32ExitCode = 0;
	//m_Status.dwServiceSpecificExitCode = 0;
	//m_Status.dwCheckPoint = 0;
	//m_Status.dwWaitHint = 0;
	//m_bIsRunning = FALSE;
	//m_bDelayStartup = bDelayStartup;

	_TCHAR szFilePath[_MAX_PATH] = {0};
	::GetModuleFileName( NULL, szFilePath, sizeof(szFilePath) );
	_tcscpy_s(m_ServiceData.lpszBinaryPathName, szFilePath);
}

CNTService::~CNTService()
{

}

HRESULT CNTService::WinMain(LPTSTR lpCmdLine)
{
	CNTService* pService = m_pThis;
	HRESULT hr = S_OK;

	ParseCommandLine(lpCmdLine, hr);

	//pService->OnFinal();

	return hr;
}

BOOL CNTService::ParseCommandLine(LPTSTR lpCmdLine, HRESULT &hRet)
{
	CCmdLine _CmdLine;
	BOOL bRet = _CmdLine.Analyze(lpCmdLine);

	if (_CmdLine.HasParam(L"svc"))
	{
		_stprintf_s(m_ServiceData.lpszServiceName, L"%s", _CmdLine[L"svc"].String().c_str());
	}

	if (_CmdLine.HasParam(L"svcdisp"))
	{
		_stprintf_s(m_ServiceData.lpszDisplayName, L"%s", _CmdLine[L"svcdisp"].String().c_str());
	}

	if (_CmdLine.HasParam(L"svcdesc"))
	{
		_stprintf_s(m_ServiceData.lpszDescription, L"%s", _CmdLine[L"svcdesc"].String().c_str());
	}

	if (!bRet)
		return FALSE;

	if (_CmdLine.HasParam(CMDLINE_PARAM_NAME_INSTALL))
	{
		hRet = InstallService() ? S_OK : E_FAIL;
		return TRUE;
	}
	else if (_CmdLine.HasParam(CMDLINE_PARAM_NAME_UNINSTALL))
	{
		hRet = UninstallService() ? S_OK : E_FAIL;
		return TRUE;
	}
	else if (_CmdLine.HasParam(CMDLINE_PARAM_NAME_START))
	{
		hRet = RunService() ? S_OK : E_FAIL;
		return TRUE;
	}
	else if (_CmdLine.HasParam(CMDLINE_PARAM_NAME_RUN))
	{
		hRet = InstAndRunService() ? S_OK : E_FAIL;
		return TRUE;
	}
	else if (_CmdLine.HasParam(CMDLINE_PARAM_NAME_STOP))
	{
		hRet = KillService() ? S_OK : E_FAIL;
		return TRUE;
	}
	else if (_CmdLine.HasParam(CMDLINE_PARAM_NAME_KILL))
	{
		hRet = UninstAndKillService() ? S_OK : E_FAIL;
		return TRUE;
	}

	hRet = S_OK;
	return FALSE;
}

BOOL CNTService::UninstAndKillService()
{
	// Request to uninstall.
	if (!IsInstalled())
	{
		DebugMsg( _T("%s is not installed\n"), m_ServiceData.lpszServiceName);
	} 
	else 
	{
		if(!KillService())
			return FALSE;
		// Try and remove the copy that's installed
		if (!Uninstall())
		{
			DebugMsg( _T("Could not remove %s. Error %d\n"), m_ServiceData.lpszServiceName, GetLastError() );
			return FALSE;
		}

	}
	return TRUE; // say we processed the argument
}

BOOL CNTService::KillService()
{
	SC_HANDLE schSCManager;
	SC_HANDLE schService;

	SERVICE_STATUS_PROCESS ssp;
	DWORD dwStartTime = GetTickCount();
	DWORD dwBytesNeeded;
	DWORD dwTimeout = 30000; // 30-second time-out

	BOOL bRet = FALSE;

	// Get a handle to the SCM database. 

	schSCManager = OpenSCManager( 
		NULL,                    // local computer
		NULL,                    // ServicesActive database 
		SC_MANAGER_ALL_ACCESS);  // full access rights 

	if (NULL == schSCManager) 
	{
		DebugMsg(_T("OpenSCManager failed (%d)\n"), GetLastError());
		return bRet;
	}

	// Get a handle to the service.

	schService = OpenService( 
		schSCManager,         // SCM database 
		m_ServiceData.lpszServiceName,             // name of service 
		SERVICE_STOP | 
		SERVICE_QUERY_STATUS | 
		SERVICE_ENUMERATE_DEPENDENTS);  

	if (schService == NULL)
	{ 
		DebugMsg(_T("OpenService failed (%d)\n"), GetLastError()); 
		CloseServiceHandle(schSCManager);
		return bRet;
	}    

	// Make sure the service is not already stopped.

	if ( !QueryServiceStatusEx( 
		schService, 
		SC_STATUS_PROCESS_INFO,
		(LPBYTE)&ssp, 
		sizeof(SERVICE_STATUS_PROCESS),
		&dwBytesNeeded ) )
	{
		DebugMsg(_T("QueryServiceStatusEx failed (%d)\n"), GetLastError()); 
		goto stop_cleanup;
	}

	if ( ssp.dwCurrentState == SERVICE_STOPPED )
	{
		DebugMsg(_T("Service is already stopped.\n"));
		bRet = TRUE;
		goto stop_cleanup;
	}

	// If a stop is pending, wait for it.

	while ( ssp.dwCurrentState == SERVICE_STOP_PENDING ) 
	{
		DebugMsg(_T("Service stop pending...\n"));
		Sleep( ssp.dwWaitHint );
		if ( !QueryServiceStatusEx( 
			schService, 
			SC_STATUS_PROCESS_INFO,
			(LPBYTE)&ssp, 
			sizeof(SERVICE_STATUS_PROCESS),
			&dwBytesNeeded ) )
		{
			DebugMsg(_T("QueryServiceStatusEx failed (%d)\n"), GetLastError()); 
			goto stop_cleanup;
		}

		if ( ssp.dwCurrentState == SERVICE_STOPPED )
		{
			DebugMsg(_T("Service stopped successfully.\n"));
			bRet = TRUE;
			goto stop_cleanup;
		}

		if ( GetTickCount() - dwStartTime > dwTimeout )
		{
			DebugMsg(_T("Service stop timed out.\n"));
			goto stop_cleanup;
		}
	}

	// If the service is running, dependencies must be stopped first.

	StopDependentServices(schService,schSCManager);

	// Send a stop code to the service.

	if ( !ControlService( 
		schService, 
		SERVICE_CONTROL_STOP, 
		(LPSERVICE_STATUS) &ssp ) )
	{
		DebugMsg(_T("ControlService failed (%d)\n"), GetLastError() );
		goto stop_cleanup;
	}

	// Wait for the service to stop.

	while ( ssp.dwCurrentState != SERVICE_STOPPED ) 
	{
		Sleep( ssp.dwWaitHint );
		if ( !QueryServiceStatusEx( 
			schService, 
			SC_STATUS_PROCESS_INFO,
			(LPBYTE)&ssp, 
			sizeof(SERVICE_STATUS_PROCESS),
			&dwBytesNeeded ) )
		{
			DebugMsg(_T("QueryServiceStatusEx failed (%d)\n"), GetLastError() );
			goto stop_cleanup;
		}

		if ( ssp.dwCurrentState == SERVICE_STOPPED )
			break;

		if ( GetTickCount() - dwStartTime > dwTimeout )
		{
			DebugMsg(_T("Wait timed out\n" ));
			goto stop_cleanup;
		}
	}
	DebugMsg(_T("Service stopped successfully\n"));
	bRet = TRUE;

stop_cleanup:
	CloseServiceHandle(schService); 
	CloseServiceHandle(schSCManager);
	return bRet;
}

BOOL CNTService::InstAndRunService()
{
	if (IsInstalled())
	{
		DebugMsg( _T("%s is already installed\n"), m_ServiceData.lpszServiceName);
	} 
	else 
	{
		if (Install()) {
			DebugMsg( _T("%s installed\n"), m_ServiceData.lpszServiceName);
		}
		else
		{
			DebugMsg( _T("%s failed to install. Error %d\n"), m_ServiceData.lpszServiceName, GetLastError());
			return FALSE;
		}
	}
	return RunService();
}

BOOL CNTService::RunService()
{
	SC_HANDLE schSCManager;
	SC_HANDLE schService;

	SERVICE_STATUS_PROCESS ssStatus; 
	DWORD dwOldCheckPoint; 
	DWORD dwStartTickCount;
	DWORD dwWaitTime;
	DWORD dwBytesNeeded;
	BOOL bRet = FALSE;

	// Get a handle to the SCM database. 

	schSCManager = OpenSCManager( 
		NULL,                    // local computer
		NULL,                    // servicesActive database 
		SC_MANAGER_ALL_ACCESS);  // full access rights 

	if (NULL == schSCManager) 
	{
		DebugMsg(_T("OpenSCManager failed (%d)\n"), GetLastError());
		return bRet;
	}

	// Get a handle to the service.

	schService = OpenService( 
		schSCManager,         // SCM database 
		m_ServiceData.lpszServiceName,            // name of service 
		SERVICE_ALL_ACCESS);  // full access 

	if (schService == NULL)
	{ 
		DebugMsg(_T("OpenService failed (%d)\n"), GetLastError()); 
		CloseServiceHandle(schSCManager);
		return bRet;
	}    

	// Check the status in case the service is not stopped. 

	if (!QueryServiceStatusEx( 
		schService,                     // handle to service 
		SC_STATUS_PROCESS_INFO,         // information level
		(LPBYTE) &ssStatus,             // address of structure
		sizeof(SERVICE_STATUS_PROCESS), // size of structure
		&dwBytesNeeded ) )              // size needed if buffer is too small
	{
		DebugMsg(_T("QueryServiceStatusEx failed (%d)\n"), GetLastError());
		CloseServiceHandle(schService); 
		CloseServiceHandle(schSCManager);
		return bRet; 
	}

	// Check if the service is already running. It would be possible
	// to stop the service here, but for simplicity this example just returns. 

	if(ssStatus.dwCurrentState != SERVICE_STOPPED && ssStatus.dwCurrentState != SERVICE_STOP_PENDING)
	{
		DebugMsg(_T("Cannot start the service because it is already running\n"));
		CloseServiceHandle(schService); 
		CloseServiceHandle(schSCManager);
		return bRet; 
	}

	// Wait for the service to stop before attempting to start it.

	while (ssStatus.dwCurrentState == SERVICE_STOP_PENDING)
	{
		// Save the tick count and initial checkpoint.

		dwStartTickCount = GetTickCount();
		dwOldCheckPoint = ssStatus.dwCheckPoint;

		// Do not wait longer than the wait hint. A good interval is 
		// one-tenth of the wait hint but not less than 1 second  
		// and not more than 10 seconds. 

		dwWaitTime = ssStatus.dwWaitHint / 10;

		if( dwWaitTime < 1000 )
			dwWaitTime = 1000;
		else if ( dwWaitTime > 10000 )
			dwWaitTime = 10000;

		Sleep( dwWaitTime );

		// Check the status until the service is no longer stop pending. 

		if (!QueryServiceStatusEx( 
			schService,                     // handle to service 
			SC_STATUS_PROCESS_INFO,         // information level
			(LPBYTE) &ssStatus,             // address of structure
			sizeof(SERVICE_STATUS_PROCESS), // size of structure
			&dwBytesNeeded ) )              // size needed if buffer is too small
		{
			DebugMsg(_T("QueryServiceStatusEx failed (%d)\n"), GetLastError());
			CloseServiceHandle(schService); 
			CloseServiceHandle(schSCManager);
			return bRet; 
		}

		if ( ssStatus.dwCheckPoint > dwOldCheckPoint )
		{
			// Continue to wait and check.

			dwStartTickCount = GetTickCount();
			dwOldCheckPoint = ssStatus.dwCheckPoint;
		}
		else
		{
			if(GetTickCount()-dwStartTickCount > ssStatus.dwWaitHint)
			{
				DebugMsg(_T("Timeout waiting for service to stop\n"));
				CloseServiceHandle(schService); 
				CloseServiceHandle(schSCManager);
				return bRet; 
			}
		}
	}

	// Attempt to start the service.

	if (!::StartService(
		schService,  // handle to service 
		0,           // number of arguments 
		NULL) )      // no arguments 
	{
		DebugMsg(_T("StartService failed (%d)\n"), GetLastError());
		CloseServiceHandle(schService); 
		CloseServiceHandle(schSCManager);
		return bRet; 
	}
	else DebugMsg(_T("Service start pending...\n")); 

	// Check the status until the service is no longer start pending. 

	if (!QueryServiceStatusEx( 
		schService,                     // handle to service 
		SC_STATUS_PROCESS_INFO,         // info level
		(LPBYTE) &ssStatus,             // address of structure
		sizeof(SERVICE_STATUS_PROCESS), // size of structure
		&dwBytesNeeded ) )              // if buffer too small
	{
		DebugMsg(_T("QueryServiceStatusEx failed (%d)\n"), GetLastError());
		CloseServiceHandle(schService); 
		CloseServiceHandle(schSCManager);
		return FALSE; 
	}

	// Save the tick count and initial checkpoint.

	dwStartTickCount = GetTickCount();
	dwOldCheckPoint = ssStatus.dwCheckPoint;

	while (ssStatus.dwCurrentState == SERVICE_START_PENDING) 
	{ 
		// Do not wait longer than the wait hint. A good interval is 
		// one-tenth the wait hint, but no less than 1 second and no 
		// more than 10 seconds. 

		dwWaitTime = ssStatus.dwWaitHint / 10;

		if( dwWaitTime < 1000 )
			dwWaitTime = 1000;
		else if ( dwWaitTime > 10000 )
			dwWaitTime = 10000;

		Sleep( dwWaitTime );

		// Check the status again. 

		if (!QueryServiceStatusEx( 
			schService,             // handle to service 
			SC_STATUS_PROCESS_INFO, // info level
			(LPBYTE) &ssStatus,             // address of structure
			sizeof(SERVICE_STATUS_PROCESS), // size of structure
			&dwBytesNeeded ) )              // if buffer too small
		{
			DebugMsg(_T("QueryServiceStatusEx failed (%d)\n"), GetLastError());
			break; 
		}

		if ( ssStatus.dwCheckPoint > dwOldCheckPoint )
		{
			// Continue to wait and check.

			dwStartTickCount = GetTickCount();
			dwOldCheckPoint = ssStatus.dwCheckPoint;
		}
		else
		{
			if(GetTickCount()-dwStartTickCount > ssStatus.dwWaitHint)
			{
				// No progress made within the wait hint.
				break;
			}
		}
	} 

	// Determine whether the service is running.

	if (ssStatus.dwCurrentState == SERVICE_RUNNING) 
	{
		DebugMsg(_T("Service started successfully.\n")); 
		bRet = TRUE;
	}
	else 
	{ 
		DebugMsg(_T("Service not started. \n"));
		DebugMsg(_T(" Current State: %d\n"), ssStatus.dwCurrentState); 
		DebugMsg(_T("Exit Code: %d\n"), ssStatus.dwWin32ExitCode); 
		DebugMsg(_T("Check Point: %d\n"), ssStatus.dwCheckPoint); 
		DebugMsg(_T("Wait Hint: %d\n"), ssStatus.dwWaitHint); 
	} 

	CloseServiceHandle(schService); 
	CloseServiceHandle(schSCManager);
	return bRet;
}

// Test if the service is currently installed
BOOL CNTService::IsInstalled()
{
	BOOL bResult = FALSE;

	// Open the Service Control Manager
	SC_HANDLE hSCM = ::OpenSCManager(NULL, // local machine
		NULL, // ServicesActive database
		SC_MANAGER_ALL_ACCESS); // full access

	if (hSCM) {
		// Try to open the service
		SC_HANDLE hService = ::OpenService(hSCM, m_ServiceData.lpszServiceName, SERVICE_QUERY_CONFIG);
		if (hService) {
			bResult = TRUE;
			::CloseServiceHandle(hService);
		}

		::CloseServiceHandle(hSCM);
	}

	return bResult;
}

//////////////////////////////////////////////////////////////////////////
// install service
BOOL CNTService::InstallService()
{
	if (IsInstalled())
	{
		DebugMsg( _T("%s is already installed\n"), m_ServiceData.lpszServiceName);

		BOOL bResult = TRUE;
		SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
		if (hSCM != NULL)
		{
			SC_HANDLE hService = ::OpenService(
				hSCM, 
				m_ServiceData.lpszServiceName, 
				SERVICE_QUERY_CONFIG | SERVICE_CHANGE_CONFIG
				);
			if (hService != NULL)
			{
				ChangeServiceConfig( 
					hService,
					SERVICE_WIN32_OWN_PROCESS,
					SERVICE_AUTO_START,  // default auto start
					SERVICE_ERROR_NORMAL,
					m_ServiceData.lpszBinaryPathName,
					m_bIsAfterXP ? SVC_ORDERGROUP_1 : SVC_ORDERGROUP_2,
					NULL,
					_T("RPCSS\0"),
					NULL,
					NULL,
					m_ServiceData.lpszDisplayName
					);
				::CloseServiceHandle(hService);
			}
			::CloseServiceHandle(hSCM);

			return StartService();
		}
		return bResult;
	} 
	else 
	{
		// Try and install the copy that's running
		if (Install()) {
			DebugMsg( _T("%s installed\n"), m_ServiceData.lpszServiceName);
		}
		else
		{
			DebugMsg( _T("%s failed to install. Error %d\n"), m_ServiceData.lpszServiceName, GetLastError());
			return FALSE;
		}
	}
	return TRUE; // say we processed the argument
}

BOOL CNTService::Install()
{
	// Open the Service Control Manager
	SC_HANDLE hSCM = ::OpenSCManager(NULL, // local machine
		NULL, // ServicesActive database
		SC_MANAGER_ALL_ACCESS); // full access
	if (!hSCM) return FALSE;


	// Create the service
	SC_HANDLE hService = ::CreateService(
		hSCM,								// SCManager database
		m_ServiceData.lpszServiceName,		// name of service
		m_ServiceData.lpszDisplayName,		// name to display
		SERVICE_ALL_ACCESS,					// desired access
		m_ServiceData.dwServiceType,		// service type
		m_ServiceData.dwStartType,			// start type
		m_ServiceData.dwErrorControl,       // error control type
		m_ServiceData.lpszBinaryPathName,	// service's binary
		m_bIsAfterXP ? SVC_ORDERGROUP_1 : SVC_ORDERGROUP_2,
		m_ServiceData.dwTagId!=0 ? &m_ServiceData.dwTagId : NULL,				// no tag identifier
		m_ServiceData.lpszDependencies,		// dependencies
		NULL,								// LocalSystem account
		NULL);								// no password
	if ( !hService )
	{
		::CloseServiceHandle(hSCM);
		return FALSE;
	}

	SERVICE_DESCRIPTION des;
	des.lpDescription=m_ServiceData.lpszDescription;

	ChangeServiceConfig2(
		hService,                // handle to service
		SERVICE_CONFIG_DESCRIPTION, // change: description
		&des);

	if ( m_bDelayStartup == true )
	{	// setup delay start up
		SERVICE_DELAYED_AUTO_START_INFO info = { TRUE };
		if ( 0 != ::ChangeServiceConfig2( hService,  SERVICE_CONFIG_DELAYED_AUTO_START_INFO, &info) )
		{
			DebugMsg( _T("change configuration to delay auto-startup failed") );
		}
	}

	// make registry entries to support logging messages
	// Add the source name as a subkey under the Application
	// key in the EventLog service portion of the registry.
	_TCHAR szKey[256];
	HKEY hKey = NULL;
	_tcscpy_s( szKey, 256, _T( "SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application\\") );
	_tcscat_s( szKey, 256, m_ServiceData.lpszServiceName );
	if (::RegCreateKey(HKEY_LOCAL_MACHINE, szKey, &hKey) != ERROR_SUCCESS) 
	{
		::CloseServiceHandle(hService);
		::CloseServiceHandle(hSCM);
		return FALSE;
	}

	// Add the Event ID message-file name to the 'EventMessageFile' subkey.
	::RegSetValueEx(hKey, _T("EventMessageFile"), 0, REG_EXPAND_SZ, (CONST BYTE*)m_ServiceData.lpszBinaryPathName, _tcslen(m_ServiceData.lpszBinaryPathName) + 1);     

	// Set the supported types flags.
	DWORD dwData = EVENTLOG_ERROR_TYPE | EVENTLOG_WARNING_TYPE | EVENTLOG_INFORMATION_TYPE;
	::RegSetValueEx(hKey, _T("TypesSupported"), 0, REG_DWORD, (CONST BYTE*)&dwData, sizeof(DWORD));
	::RegCloseKey(hKey);

	//LogEvent(EVENTLOG_INFORMATION_TYPE, EVMSG_INSTALLED, m_ServiceData.lpszServiceName);

	::CloseServiceHandle(hService);
	::CloseServiceHandle(hSCM);
	return TRUE;
}

BOOL CNTService::UninstallService()
{
	if (!IsInstalled())
	{
		DebugMsg( _T("%s is not installed\n"), m_ServiceData.lpszServiceName);
	} 
	else 
	{
		// Try and remove the copy that's installed
		if (Uninstall())
		{
			// Get the executable file path
			_TCHAR szFilePath[_MAX_PATH];
			ZeroMemory( szFilePath, MAX_PATH * sizeof( _TCHAR ) );
			::GetModuleFileName(NULL, szFilePath, sizeof(szFilePath));
			DebugMsg( _T("%s removed. (You must delete the file (%s) yourself.)\n"), m_ServiceData.lpszServiceName, szFilePath);
		} 
		else 
		{
			DebugMsg( _T("Could not remove %s. Error %d\n"), m_ServiceData.lpszServiceName, GetLastError() );
			return FALSE;

		}
	}
	return TRUE; // say we processed the argument
}

BOOL CNTService::Uninstall()
{
	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS); // full access
	if (!hSCM) return FALSE;

	BOOL bResult = FALSE;
	SC_HANDLE hService = ::OpenService(hSCM, m_ServiceData.lpszServiceName, DELETE);
	if (hService) {
		if (::DeleteService(hService)) {
			//LogEvent(EVENTLOG_INFORMATION_TYPE, EVMSG_REMOVED, m_ServiceData.lpszServiceName);
			bResult = TRUE;
		} else {
			//LogEvent(EVENTLOG_ERROR_TYPE, EVMSG_NOTREMOVED, m_ServiceData.lpszServiceName);
		}
		::CloseServiceHandle(hService);
	}

	::CloseServiceHandle(hSCM);
	return bResult;
}

BOOL CNTService::StartService()
{
#ifdef _DEBUG
	ServiceMain(0, NULL);
	return TRUE;
#endif

	SERVICE_TABLE_ENTRY st[] = {
		{m_ServiceData.lpszServiceName, ServiceMain},
		{NULL, NULL}
	};

	DebugMsg( _T("Calling StartServiceCtrlDispatcher()"));
	BOOL b = ::StartServiceCtrlDispatcher(st);
	DebugMsg( _T("Returned from StartServiceCtrlDispatcher()"));
	return b;  
}

// static member function (callback)
void CNTService::ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv)
{
	// Get a pointer to the C++ object
	CNTService* pService = m_pThis;

	pService->DebugMsg( _T("Entering CNTService::ServiceMain()"));

#ifndef _DEBUG
	// Register the control request handler
	pService->m_Status.dwCurrentState = SERVICE_START_PENDING;
	pService->m_hServiceStatus = RegisterServiceCtrlHandlerEx(pService->m_ServiceData.lpszServiceName, (LPHANDLER_FUNCTION_EX)Handler,0);
	if (pService->m_hServiceStatus == NULL) {
		pService->LogEvent(EVENTLOG_ERROR_TYPE, EVMSG_CTRLHANDLERNOTINSTALLED);
		return;
	}
#endif


	if (pService->Initialize()) {

		// Do the real work. 
		// When the Run function returns, the service has stopped.
		pService->m_bIsRunning = TRUE;
		pService->m_Status.dwWin32ExitCode = 0;
		pService->m_Status.dwCheckPoint = 0;
		pService->m_Status.dwWaitHint = 0;
		pService->Run();
	}

	// Tell the service manager we are stopped
	pService->SetStatus(SERVICE_STOPPED);

	pService->DebugMsg( _T("Leaving CNTService::ServiceMain()"));
}


////////////////////////////////////////////////////////////////////////////////////////////
// Debugging support

void CNTService::DebugMsg(const _TCHAR* pszFormat, ...)
{
	_TCHAR buf[1024];
	ZeroMemory( buf, 1024*sizeof(_TCHAR ) );
	_stprintf_s(buf, 1024, _T("[%s](%lu): "), m_ServiceData.lpszServiceName, GetCurrentThreadId());
	va_list arglist;
	va_start( arglist, pszFormat );
	int nBuffSize= _tcslen(buf);
	_vstprintf_s( &buf[nBuffSize], 1024-nBuffSize, pszFormat, arglist);
	va_end( arglist );
	nBuffSize= _tcslen(buf);
	_tcscat_s( buf, 1024-nBuffSize, _T("\n") );
	OutputDebugString( buf );
}