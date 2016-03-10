/********************************************************************
	created:	2016/03/10
	created:	10:3:2016   20:02
	filename: 	base\baselib\basehelper\utils\AppUtils.cpp
	file path:	base\baselib\basehelper\utils
	file base:	AppUtils
	file ext:	cpp
	author:		murisly
	
	purpose:	a module to get app info
*********************************************************************/


#include "AppUtils.h"


//****************************************************************
// determine whether application is already running. based on mutex

bool CAppUtils::IsAppExist( LPCTSTR csAppGUID )
{
	HANDLE hMutex = ::CreateMutex( NULL, FALSE, csAppGUID );
	int nLastErr = GetLastError();
	if ( hMutex == NULL )
	{
		return false;
	}

	::CloseHandle( hMutex );
	return ( nLastErr == ERROR_ALREADY_EXISTS ) ? true : false;
}