/********************************************************************
	created:	2016/03/10
	created:	10:3:2016   22:22
	filename: 	E:\work\git\base\base\base\baselib\basehelper\utils\PathUtils.h
	file path:	E:\work\git\base\base\base\baselib\basehelper\utils
	file base:	PathUtils
	file ext:	h
	author:		murisly
	version:    1.0.0
	purpose:	a module for path manager
*********************************************************************/

#pragma once


#include "..\common\Singleton.h"
#include <windows.h>

class CPathUtils : public SingletonStatic<CPathUtils> 
{
public:
	CPathUtils();
	~CPathUtils();

	bool HasInitialized();
	void UpdateUserFolder( LPCTSTR lpcsUser );

	// get system window directory
	bool GetWinDir( LPTSTR lpWindir, DWORD dwBuffer, LPCTSTR lpcVarname = L"TEMP" );
	// get system temporary directory
	bool GetTempDir( LPTSTR lpTmpDir, DWORD  nDestCnt = MAX_PATH, HANDLE hToken = NULL, LPCTSTR lpcsProductName = NULL );

protected:
	bool Initialize( HINSTANCE hInst );

	TCHAR	m_csAppDir[MAX_PATH];
	TCHAR	m_csAppName[MAX_PATH];
};