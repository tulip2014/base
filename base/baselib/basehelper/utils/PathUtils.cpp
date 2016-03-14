/********************************************************************
	created:	2016/03/10
	created:	10:3:2016   22:34
	filename: 	E:\work\git\base\base\base\baselib\basehelper\utils\PathUtils.cpp
	file path:	E:\work\git\base\base\base\baselib\basehelper\utils
	file base:	PathUtils
	file ext:	cpp
	author:		murisly
	version:    1.0.0
	purpose:	
*********************************************************************/


#include "PathUtils.h"
#include <shlwapi.h>
#include <strsafe.h>
#include <crtdbg.h>
#include <tchar.h>
#include <ShlObj.h>
#include <shellapi.h>

#pragma comment(lib, "shlwapi.lib")


CPathUtils::CPathUtils(){ Initialize( NULL ) ;};

CPathUtils::~CPathUtils(){};

//************************************
// Method:    Initialize
// FullName:  CPathUtils::Initialize
// Access:    protected 
// Returns:   bool
// Qualifier:
// Parameter: HINSTANCE hInst
// purpose:   initialize path manager
//************************************
bool CPathUtils::Initialize( HINSTANCE hInst )
{
	TCHAR szModulePath[MAX_PATH];
	ZeroMemory( szModulePath, MAX_PATH*sizeof( TCHAR ) );
	::GetModuleFileName( hInst, szModulePath, MAX_PATH - 1 );

	// get application name
	LPTSTR lpModPath = PathFindFileName( szModulePath );
	StringCbCopy( m_csAppName, MAX_PATH, lpModPath );
	PathRemoveExtension( m_csAppName );
	
	// get application directory
	if ( TRUE == PathRemoveFileSpec( szModulePath ) )
	{
		StringCbCopy( m_csAppDir, MAX_PATH, szModulePath );
		return true;
	}
	_ASSERT( 0 );
	return false;
}


bool CPathUtils::HasInitialized()
{
	return ( _tcslen( m_csAppDir ) > 0 ) ? true : false;
}



//************************************
// Method:    GetWinDir
// FullName:  CPathUtils::GetWinDir
// Access:    public 
// Returns:   bool
// Qualifier:
// Parameter: LPTSTR lpWindir
// Parameter: DWORD dwBuffer
// Purpose:   get environment variable, first from local user, second from system
//************************************
bool CPathUtils::GetWinDir(LPTSTR lpWindir, DWORD dwBuffer, LPCTSTR lpcVarname)
{
	// method 1, return directly from environment path
	size_t dwSize = 0;
	return 0 == _tgetenv_s( &dwSize, lpWindir, dwBuffer, lpcVarname );
}


//****************************************************************
// get system temporary directory
bool CPathUtils::GetTempDir( LPTSTR lpcsFilePath, DWORD  nDestCnt, HANDLE hToken, LPCTSTR lpcsProductName)
{
	TCHAR tcsTempDir[MAX_PATH] = { 0 };
	if ( S_OK == SHGetFolderPath( NULL, CSIDL_APPDATA, hToken, SHGFP_TYPE_CURRENT, tcsTempDir ) )
	{
		PathCombine( lpcsFilePath, tcsTempDir, lpcsProductName ? lpcsProductName : _T("TestApp") );
		if ( false == PathFileExists( lpcsFilePath ) )
		{
			if ( CreateDirectory( lpcsFilePath, NULL ) )
			{
				return true;
			}
		}
		else return true;
	}
	return false;
}


//****************************************************************
bool CPathUtils::CopyFolder( LPCTSTR lpszFrom, LPCTSTR lpszTo )
{
	SHFILEOPSTRUCT FileOp;  
	ZeroMemory((void*)&FileOp,sizeof(SHFILEOPSTRUCT));  
	FileOp.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_NOCONFIRMMKDIR;  
	FileOp.hNameMappings = NULL;  
	FileOp.hwnd = NULL;  
	FileOp.lpszProgressTitle = NULL;  
	FileOp.pFrom = lpszFrom;  
	FileOp.pTo = lpszTo;  
	FileOp.wFunc = FO_COPY;  
	int nError = SHFileOperation(&FileOp);
	return ( nError == 0 ) ? true : false;  
}


//****************************************************************
bool CPathUtils::DeleteFolder( LPCTSTR lpFolder )
{
	SHFILEOPSTRUCT FileOp;  
	ZeroMemory((void*)&FileOp,sizeof(SHFILEOPSTRUCT));  
	FileOp.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_NOCONFIRMMKDIR;  
	FileOp.hNameMappings = NULL;  
	FileOp.hwnd = NULL;  
	FileOp.lpszProgressTitle = NULL; 
	_TCHAR tcsFolder[MAX_PATH] = { 0 };
	StringCbPrintf( tcsFolder, MAX_PATH*sizeof(_TCHAR), _T("%s\0\0"), lpFolder );
	FileOp.pFrom = tcsFolder;
	FileOp.wFunc = FO_DELETE;  
	int nError = SHFileOperation(&FileOp);
	return ( nError == 0 ) ? true : false;  
}


//****************************************************************
// create directory path
bool CPathUtils::CreateFolder( LPCTSTR lpcsDirPath )
{
	if ( false == PathFileExists( lpcsDirPath ) )
	{
		if ( ERROR_SUCCESS != SHCreateDirectory( NULL, lpcsDirPath ) )
		{
			return false;
		}
	}
	return true;
}